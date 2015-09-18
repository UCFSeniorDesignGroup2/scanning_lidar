// this script pipes the incoming tcp data to a websocket
// this is required in order for the web client to receive data

var net = require('net');
var SlipStream = require('./slip_stream').SlipStream;



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

// print out errors
slip_stream.on('error', function(err) {

  console.log(err);
});
  
// pipe the client to slip_stream for decoding
client.pipe(slip_stream);

exports.data_stream = slip_stream;

