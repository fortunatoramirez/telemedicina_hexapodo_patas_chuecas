#include <Servo.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SocketIoClient.h>


// ********SERVOMOTORES******* a utilizar
Servo ServoDLX;
Servo ServoDLY;

Servo ServoDRX;
Servo ServoDRY;

Servo ServoMLX;
Servo ServoMLY;

Servo ServoMRX;
Servo ServoMRY;
// **********FIN DE SERVOMOTORES******** a utilizar


//***********INICIO************ Declaracion de variables
SoftwareSerial s(3,1);//(Rx,Tx) //Pines de comunicacion serial



//********* Configuracion WI-FI
const char* ssid     = "INFINITUM989A_2.4";
const char* password = "8S4jxq94XQ";
const char* server = "201.174.122.203"; //Enter server adress
const uint16_t port = 5001; // Enter server port
uint64_t messageTimestamp;

SocketIoClient socketIO;
//********* FIN configuracion WI-Fi



//**** Declaracion variables hexapodo
const int L=0;
const int R=1;
const int U=0;
const int M=1;
const int D=2;

const int potpin = 0;  // connect potentiometer to A0

int valpot;
const int Ymin[3][2] = {{84, 109 },
                        {96, 114 },
                        {76, 108 }
                        }; //Matriz de los valores minimos en el eje Y
                 
const int Ymax[3][2] = {{172, 25 },
                        {180, 27 },
                        {163, 18 }
                         };//Matriz de los valores maximos en el eje Y

const int Xmin[3][2] = {{180, 0 },
                        {180, 0 },
                        {180, 0 }
                        }; //Matriz de los valores minimos en el eje X
                 
const int Xmax[3][2] = {{68, 106 },
                        {95, 79 },
                        {85, 85 }
                        };//Matriz de los valores maximos en el eje X
                  
int angY[3][2];
int angX[3][2];
//*********FIN******* Declaracion variables hexapodo

boolean ledStatus = true;


void setup() {
  //** Indicacion de ENCENDIDO
  pinMode(LED_BUILTIN, OUTPUT);    
  pinMode(D0, OUTPUT); 
  digitalWrite(LED_BUILTIN, HIGH);//El LED se apaga con estado alto
  digitalWrite(D0, HIGH); //El LED se apaga con estado alto
  for (int i=0; i<6; i++){
    delay(500);
   ledStatus = !ledStatus;
    digitalWrite(D0, ledStatus);
  }
  //Fin secuencia de indicacion de ENCENDIDO


  //***Acoplamiento a Wi-Fi
  connectWiFi_STA();
  
  ServoDLX.attach(D6);  // Servo eje X
  ServoDLY.attach(D5);  // Servo eje Y

  ServoDRX.attach(D7);  // Servo eje X
  ServoDRY.attach(D8);  // Servo eje Y

  ServoMLX.attach(D4);  // Servo eje X
  ServoMLY.attach(D3);  // Servo eje Y
  
  ServoMRX.attach(D1);  // Servo eje X
  ServoMRY.attach(D2);  // Servo eje Y
  
  
  s.begin(9600);
  socketIO.begin(server, port);
  socketIO.on("angulo",messageEventHandler);
}



void loop() {
 //*********INICIO******** Girar servos en X hacia adelante
for(int ang=1;ang<2;ang++){
for(int pos=0; pos<3;pos++) {
    for (int side=0; side<2; side++){
   angX[pos][side] = map(ang, 0, 2, Xmin[pos][side], Xmax[pos][side]);
    }
  }
  WriteX();
}
//***********FIN************


for(int ang=0;ang<1;ang++){
for(int pos=0; pos<3;pos++) {
    for (int side=0; side<2; side++){
   angY[pos][side] = map(ang, 0, 1, Ymin[pos][side], Ymax[pos][side]);
    }
  }
  WriteY();
}
//**********FIN*********


  while(true){
    socketIO.loop();
  }

}

void writeInt(unsigned int value){
    s.write(255);
    
    s.write(lowByte(value));
    
    s.write(highByte(value));
}

void WriteX(){
  
unsigned int SerialX = 0;
unsigned int sum;
  ServoDLX.write(angX[D][L]);  // Servo eje X
  ServoDRX.write(angX[D][R]);  // Servo eje Y
  ServoMLX.write(angX[M][L]);  // Servo eje Y
  ServoMRX.write(angX[M][R]);  // Servo eje X

  //Envio de angulos U-X
  for (int side=0; side<2; side++){
    sum = 400*side;
    SerialX = sum + angX[U][side];
    writeInt(SerialX);
    delay(5);
  }
}//*************FIN*********** WriteX

void WriteY(){
  unsigned int SerialY = 0;
  unsigned int sum;
  ServoDLY.write(angY[D][L]);  // Servo eje X
  ServoDRY.write(angY[D][R]);  // Servo eje Y
  ServoMLY.write(angY[M][L]);  // Servo eje Y
  ServoMRY.write(angY[M][R]);  // Servo eje X

  //Envio de angulos U-Y
  for (int side=0; side<2; side++){
    sum = (400*side) + 200;
    SerialY = sum+ angY[U][side];
    writeInt(SerialY);
    delay(5);
  }
}//**********FIN*********** WriteY


void messageEventHandler(const char * payload, size_t length) {
 //Serial.printf("got message: %s\n", payload);
 int num = atoi(payload);
  int side=0;
  int pos=0;
  int angx=0;

  int side1;
  int side2;
  int ang;
  char xory;
  
  switch (num) {

      case 0: //Levantar una parte
        //Patas derechas
        upndown(R, L, 8, 'y');//Levantar Y
        
        delay(2000);
        upndown(L, R, 0, 'x');//X atras

        delay(2000);
        upndown(R, L, 7, 'x');//X frente

        delay(2000);
        upndown(R, L, 0, 'y');//Y abajo
        break;
      
      case 1: //Levantar otra parte
        //Patas izquierdas
        upndown(L, R, 8, 'y');//Levantar Y
        
        delay(2000);
        upndown(R, L, 0, 'x');//X atras

        delay(2000);
        upndown(L, R, 7, 'x');//X frente

        delay(2000);
        upndown(L, R, 0, 'y');//Y abajo
        break;
        
      default:
        for (int i=0; i<5; i++){
          delay(500);
          ledStatus = !ledStatus;
          digitalWrite(D0, ledStatus);
        }
        break;
  }
  
  
}


void connectWiFi_STA()
{
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) 
   { 
     delay(100);  
     ledStatus = !ledStatus;
     digitalWrite(LED_BUILTIN, ledStatus);
   }
   digitalWrite(LED_BUILTIN, LOW);
   delay(1500);  
   digitalWrite(LED_BUILTIN, HIGH);
}

void upndown(int side1, int side2, int ang, char xory){
  int pos;
            for(pos=U; pos<3;pos=pos+2) {
              if(xory=='x'){
                angX[pos][side1] = map(ang, 0, 10, Xmin[pos][side1], Xmax[pos][side1]);
              }
              else{
                angY[pos][side1] = map(ang, 0, 10, Ymin[pos][side1], Ymax[pos][side1]);
              }
            }
             //Pata izquierda
            pos=M;
            

            switch(xory){
              case 'x':
              angX[pos][side2] = map(ang, 0, 10, Xmin[pos][side2], Xmax[pos][side2]);
              WriteX();
              break;
              
              case 'y':
              angY[pos][side2] = map(ang, 0, 10, Ymin[pos][side2], Ymax[pos][side2]);
              WriteY();
              break;
              
              default:
                 for (int i=0; i<5; i++){
                  delay(500);
                  ledStatus = !ledStatus;
                  digitalWrite(D0, ledStatus);
                  }
              break;
            }
            
}
