/* Jackson Ritchey
 * July 28, 2015
 * 
 * Test Client:
 * The purpose of the test client is to connect to the data server  
 * and display the data to the console.  The main use of this client
 * is to test and see if the data is actually being streamed.
 */




// make sure ip address and port are passed in
if(process.argv.length < 4)
{
  throw 'need ip address and port';
}

// set ip address
var ip_address = process.argv[2];
// set port
var port = parseInt(process.argv[3]);

// import network api
var net = require('net');
// import data_types
var data_types = require('./data_types');

// packet number
var packet_number = 0;

// create a client socket and connect to ip_address:port
var client = net.connect({port:port, host:ip_address}, function() {
 
  console.log('connected to server ' + ip_address + ':' + port);
});

// on data from server
client.on('data', function(data) {

  // log data
  var point_data = data_types.BufferToPoint3dArray(data);
  console.log('data packet(' + point_data.length + '): ' + packet_number);
  if(point_data.length < 20)
  {
    console.log(point_data);
  }
  packet_number++;
});

// on end of connection
client.on('end', function()
{
  console.log('disconnected from server');
});

// on error
client.on('error', function(err)
{
  console.log(err);
});
