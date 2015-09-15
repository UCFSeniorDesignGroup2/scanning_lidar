// this script pipes the incoming tcp data to a websocket
// this is required in order for the web client to receive data

var net = require('net');
var SlipStream = require('./slip_stream').SlipStream;
var http = require('http');
var websocket_stream = require('websocket-stream');



// connect to data server
var client = net.connect({port:12345,host:"127.0.0.1"}, function() { 
  
  console.log('connected to server');
});

// error
client.on('error', function(err) {

  console.log(err);
});

// create slip stream to decode data from data server 
var slip_stream = new SlipStream();

var packet_number = 0;
slip_stream.on('data', function(data) {
  console.log("packet_number: " + packet_number + " size: " + data.length);
  packet_number++;
});

// print out errors
slip_stream.on('error', function(err) {

  console.log(err);
});
  
// pipe the client to slip_stream for decoding
client.pipe(slip_stream);

// create websocket
var server = http.createServer();
websocket_stream.createServer({server:server}, function(stream) {

  stream.on('error', function(err) {
    console.log(err);
  });  
  
  // pipe slip stream to the websocket 
  slip_stream.pipe(stream);

});

// listen for connections
server.listen(8080);

