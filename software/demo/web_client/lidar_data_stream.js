var net = require('net');
var SlipStream = require('./slip_stream').SlipStream;
var http = require('http');
var websocket_stream = require('websocket-stream');



// connect to data server
var client = net.connect({port:12345,host:"127.0.0.1"}, function() { 

  console.log('connected to server');
});


// create slip stream to decode data from data server 
var slip_stream = new SlipStream(client);

slip_stream.on('data', function(data)
{
//  console.log("slip_packet:" +  data.length);
});

slip_stream.on('error', function(err) {

  console.log("error");
});

// create websocket
var server = http.createServer();
websocket_stream.createServer({server:server}, function(stream) {

  stream.on('error', function(err) {
    console.log("error");
  });  
  slip_stream.pipe(stream);

});

// listen for connections
server.listen(8080);
