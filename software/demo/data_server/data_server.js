/* Jackson Ritchey
 * July 28, 2015
 *
 * Test Data Server:
 * This server behaves the same way as tehe actual server 
 * but instead of reading data off of the serial port it 
 * generates random data and sends it to its connected clients.
 */

// util 
var util = require('util');

// import network api
var net = require('net');

var SerialPortStream = require('serial-port-stream');
 
// import slip api
var slip = require('node-slip');

// streaming slip data
var SlipStream = require('./slip_stream').SlipStream;

// stream
var stream = require('stream');

// import data_types
var data_types = require('./data_types');

var vektor = require('vektor');

// field of view of the sensor
var fov = 170/180 * Math.PI;
// number of data points to chunk up
var packet_chunks = 20;

// port
var port = 12345;
if(process.argv.length >= 3)
{
  port = parseInt(process.argv[2]);
}

// create a serial port stream to get data from the sensor
var serial_stream = new SerialPortStream('/dev/ttyACM0', { baudRate : 115200 });
serial_stream.on('error', function(err) {
  console.log(err);
});

// create a stream to convert 2d data to 3d for data plot
util.inherits(DataConverter, stream.Transform);
 
function DataConverter(options) {
  if(!(this instanceof DataConverter))
  {
    return new DataConverter(options);
  }
  stream.Transform.call(this, options);
}

// current packet in chunk
var current_chunk = 0;
// total length of chunked packets
var total_length = 0;
// chunk of data packets
var data_buffer = new Buffer(0);
DataConverter.prototype._transform = function(chunk, encoding, done) {
// if chuunk.length not divisible by 8 than its a bad packet
  if(chunk.length % 8 == 0 && chunk.length != 0)
  { 
    // number of data items in the packet
    var data_items = chunk.length / 8;
    // create buffer to hold the data items
    var buffer = new Buffer(data_items * 12);
    // convert the data to 3d then repack it in a buffer
    for(var i = 0; i < data_items; i++)
    {
      var theta = chunk.readInt32LE(i*8 + 0) / 1000 * fov; // angular position of motor
      var distance = chunk.readUInt32LE(i*8 + 4)/100; // distance reading in meters
      // convert to cartesian
      var x = distance * Math.cos(theta); 
      var y = distance * Math.sin(theta);
      var z = 0;
      // store in buffer
      buffer.writeFloatLE(x, i*12 + 0);
      buffer.writeFloatLE(y, i*12 + 4);
      buffer.writeFloatLE(z, i*12 + 8);
    } 
    
    // concat buffer to chunked buffer
    total_length = data_buffer.length + buffer.length; 
    data_buffer = Buffer.concat([data_buffer, buffer], total_length);
    current_chunk++;     
   
    // total number of chunks reached 
    if(current_chunk == packet_chunks)
    { 
      // encode in slip then send through pipe
      this.push(slip.generator(data_buffer)); 
      current_chunk = 0;
      data_buffer = new Buffer(0);
    }
  }
  done();
}

// error logger
var data_converter = new DataConverter();
data_converter.on('error', function(err) {
  console.log(err);
});

// print data coming from sensor
data_converter.on('data', function(data) {
  console.log("SLIP PACKET - received " + data.length + " bytes");
});

// create a slip stream to decode data from sensor
var slip_stream = new SlipStream();

// error logger
slip_stream.on('error', function(err) {
  console.log(err);
});

// pipe serial_stream through slip parser and then pipe it through data converter
serial_stream.pipe(slip_stream).pipe(data_converter);


// create a server
var server = net.createServer(function(c) {

  console.log('client connected');
  
  // on data
  c.on('data', function(data)
  {
    // just print it out
    console.log('client data: ' + data.length);
  });
  
  c.on('close', function(err) {
    console.log('client disconnected');
  });

  c.on('error', function(err) {
    console.log(err);
  });
//  generator.pipe(c);

  // pipe converted data from the sensor to the connected client
  data_converter.pipe(c);
});

// listen on port
server.listen(port, function() {
  
  console.log('server bound to port ' + port);

});

// on error
server.on('error', function(err) {
  console.log(err);
});

