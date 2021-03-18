/* Comunicacion serial Arduino Nano R3 -NodeMCU ESP8266
 * Parte de Arduino Nano r3.
 */
 
#include <Servo.h>
#include <SoftwareSerial.h>

// ********SERVOMOTORES******* a utilizar
Servo ServoULX;
Servo ServoULY;

Servo ServoURX;
Servo ServoURY;
// **********FIN DE SERVOMOTORES******** a utilizar

SoftwareSerial s(3,5);//(Rx, Tx)
unsigned int data_in;
unsigned int data_low;
unsigned int data_high;
unsigned int data_int;
unsigned int data_div;
unsigned int angulo;

boolean incoming = false;
boolean lowcond = false;
boolean highcond = false;

void setup() {
  ServoULX.attach(9);  // Servo eje X
  ServoULY.attach(6);  // Servo eje YM

  ServoURX.attach(10);  // Servo eje X
  ServoURY.attach(11);  // Servo eje Y
  s.begin(9600);
  Serial.begin(9600); 
}

void loop() {
  if (s.available()>0){
    data_in=s.read();
  if(incoming == false){
    if(data_in == 255)
    //Serial.println("*IN");
    incoming = true;
  }
  else if(lowcond == false){
    data_low = data_in;
    lowcond = true;
  }
  else if(highcond==false){
    data_int = (data_in*256) + data_low;
    //Serial.print("int: ");
    //Serial.print(data_int);
    highcond = true;
  }

  if(highcond == true){
    incoming = false;
    lowcond = false;
    highcond = false;
    
    //Serial.print("int: ");
    //Serial.print(data_int);
    data_div = data_int/200;
    //Serial.print("\tdiv: ");
    //Serial.print(data_div);
    angulo = data_int - (200*data_div);
    //Serial.print("\t angulo: ");
    //Serial.println(angulo);
    
    switch (data_div) {
  case 0: // ULX
    ServoULX.write(angulo);  // Servo ULX
    break;
  case 1: // ULY
    ServoULY.write(angulo);  // Servo ULY
    break;
  case 2: // URX
    ServoURX.write(angulo);  // Servo URX
    break;
  case 3: // URY
    ServoURY.write(angulo);  // Servo URY
    break;  
  default:
    data_div=0;
    break;
    } // fin de Switch-Case
  }// Fin del caso donde el dato esta completo

  }
}
