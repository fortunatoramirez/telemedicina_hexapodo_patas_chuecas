var port = 5001;
var socket;

function init() {
	try {
		socket = io.connect('http://201.174.122.203:5001', {'forceNew': true});

		socket.on('messages', function(data){
		    render(data);
		});

	}
	catch(ex){ 
		console.log(ex); 
	}
}

function render(data){

	var html = "Muestra: "+data;
	document.getElementById('messages').innerHTML = html;
}


  // var i=0;
    function funcion_1()
{
         //i++;
    // console.log("Click a BOTON 1 = "); //+i);
        var valor_1 = document.getElementById("txt_1").value;
        
        valor_1_int = parseInt(valor_1);
        if (valor_1_int >=0 && valor_1_int <=180)
             {
             	valor_1_int=valor_1_int;
         console.log(valor_1_int);
     }
         else 

            {
            	valor_1_int=undefined;
         
            }
       
        

        socket.emit("angulo", valor_1_int.toString());
}

function funcion_2()
{
     var valor_1 = document.getElementById("txt_2").value;
        
        valor_1_int = parseInt(valor_1);
        if (valor_1_int >=0 && valor_1_int <=180)
             {
             	valor_1_int=valor_1_int + 200;
         console.log(valor_1_int);
     }
         else 

            {
            	valor_1_int=undefined;
        
            }

        socket.emit("angulo", valor_1_int.toString());
}




