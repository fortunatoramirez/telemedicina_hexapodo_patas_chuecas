//server

var express = require('express');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);

app.use(express.static('public'));
app.get('/hello', function(req, res){
	res.status(200).send("Hola mundo");
});

io.on('connection',function(socket){
	socket.on('nuevo_mensaje', function(msg){
		//console.log(msg);
		io.sockets.emit('messages',msg);
});

socket.on('angulo', function(msg){
		console.log(msg);
  		io.sockets.emit('angulo',msg);
	});
	
});

server.listen(5001, function(){
	console.log("Servidor corriendo en http://localhost:5001");
});
