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

// import slip api
var slip = require('node-slip');

// stream
var stream = require('stream');

// import data_types
var data_types = require('./data_types');


// port
var port = 12345;
if(process.argv.length >= 3)
{
  port = parseInt(process.argv[2]);
}


// number of data_points to generate
var num_points = 500;

// numver of scans till back at zero point
var num_scans = 100; 

// max distance for testing
var max_distance = 30;

// simulated scans per second
var scans_per_second = 10;

// slew rate
var slew_rate = 10;

// array of data points to generate
var data_points = [];

// create a packet stream that generates data randomly
util.inherits(PacketGenerator, stream.Readable);

function PacketGenerator(options)
{
  if(!(this instanceof PacketGenerator))
  {
    return new PacketGenerator(options);
  } 

  stream.Readable.call(this, options);


}

PacketGenerator.prototype._read = function(n) {
  var self = this;
  setTimeout(function() {
    
    self.push(slip.generator(get_next_packet()));  
    
  }, 1000 / scans_per_second);   
  

}

var generator = new PacketGenerator();


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
  generator.pipe(c);
});

// listen on port
server.listen(port, function() {
  
  console.log('server bound to port ' + port);

});

// on error
server.on('error', function(err) {
  console.log(err);
});

var index = 0;
var max_index = num_scans;
// generate next data packet for testing 
function get_next_packet()
{
  if(data_points.length == 0)
  {
    for(var i = 0; i < num_points; i++)
    {
        var x = i;
	var y = i;
	var z = i;

        // generate random data   
        data_points.push(new data_types.Point3d(x,y,z));
    }
  }
  else
  {
    // generate next data set
    for(var i = 0; i < num_points; i++)
    {
      var x = max_distance * Math.cos(i/num_points*Math.PI*2) * Math.sin(index/num_scans*Math.PI);
      var y = max_distance * Math.sin(i/num_points*Math.PI*2) * Math.sin(index/num_scans*Math.PI); 
      var z = max_distance * Math.cos(index/num_scans*Math.PI);

      var rand_err = (1+Math.random()/slew_rate);
      x*=rand_err;
      y*=rand_err;
      z*=rand_err;
      

      data_points[i].x = x; 
      data_points[i].y = y;
      data_points[i].z = z;
    }
  }
  
  index++;
  if(index == max_index)
  {
    index = 0;
  }

  return data_types.Point3dArrayToBuffer(data_points);
}


// define the function to plot here
// z = f(x,y)
function math_func(x,y)
{
  // mwhahahah just for fun
//  return 2*Math.max(-2*(Math.round(Math.pow(Math.E, -1*(-2*x)*(-2*x)))+Math.round(Math.pow(Math.E, -1*(-2*y)*(-2*y))))+2+2*Math.cos((x*x+y*y)/100 - index/num_scans*Math.PI*2), 25*Math.pow(Math.E, -1*(x*x+y*y)*3));


  return max_distance * Math.sqrt(1-x*x+y*y); 
}



