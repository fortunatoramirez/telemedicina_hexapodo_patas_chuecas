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