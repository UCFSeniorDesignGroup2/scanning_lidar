/* Jackson Ritchey
 * July 28, 2015
 *
 * Test Data Server:
 * This server behaves the same way as tehe actual server 
 * but instead of reading data off of the serial port it 
 * generates random data and sends it to its connected clients.
 */


// import network api
var net = require('net');

// import slip api
var slip = require('node-slip');


// import data_types
var data_types = require('./data_types');

// port
var port = 12345;
if(process.argv.length >= 3)
{
  port = parseInt(process.argv[2]);
}

// array for storing connections
var connections = [];

// number of data_points to generate
var num_points = 50;

// max distance for testing
var max_distance = 25;

// simulated scans per second
var scans_per_second = 30;

// slew rate
var slew_rate = 100;

// array of data points to generate
var data_points = [];


// create a server
var server = net.createServer(function(c) {

  console.log('client connected');
  
  // on data
  c.on('data', function(data)
  {
    // just print it out
    console.log('client data: ' + data.length);
  });

  // on connection closed
  c.on('end', function()
  {
    console.log('client disconnected');   
    // delete connection from array
    for(var index in connections)
    {
      if(connections[index] == c)
      {
        connections.splice(index, 1);
      }
    }
  });

  // on error
  c.on('error', function(err) {
    // delete connection from array
    for(var index in connections)
    {
      if(connections[index] == c)
      {
        connections.splice(index, 1);
      }
    }
    console.log(err);
    console.log('removing client');
  });

  // add connection to array
  connections.push(c);

  console.log(connections.length + ' connections');
});

// listen on port
server.listen(port, function() {
  
  console.log('server bound to port ' + port);

});

// on error
server.on('error', function(err) {
  console.log(err);
});


// continuously send data at scan_rate
setInterval(function() {
  for(var c in connections)
  {
    // encode packet and send it
    connections[c].write(slip.generator(get_next_packet()));
  } 

}, 1000/scans_per_second);

// generate next data packet for testing 
function get_next_packet()
{
  if(data_points.length == 0)
  {
    for(var i = 0; i < num_points; i++)
    {
      for(var j = 0; j < num_points; j++)
      {    
      // generate random data   
        data_points.push(new data_types.Point3d(
                                      max_distance/2,
                                      Math.PI*j/num_points, 
                                      Math.PI*2*i/num_points));
      
      }
    }
  }
  else
  {
    // generate next data set
    for(var i = 0; i < num_points*num_points; i++)
    {
      var radius = Math.random() * max_distance;
      var theta = Math.random() * Math.PI;
      var phi = Math.random() * 2 * Math.PI;

      data_points[i].radius += ((radius - data_points[i].radius) / slew_rate);
      //data_points[i].theta += ((theta - data_points[i].theta) / slew_rate);
      //data_points[i].phi += ((phi - data_points[i].phi) / slew_rate);

      // if less than zero make it zero
      data_points[i].radius < 0 ? data_points[i].radius = 0 : data_points[i].radius;
      data_points[i].theta < 0 ? data_points[i].theta = 0 : data_points[i].theta; 
      data_points[i].phi < 0 ? data_points[i].phi = 0 : data_points[i].theta;
      
      // if out of bounds set equal to bounds
      data_points[i].radius > max_distance ? data_points[i].radius = max_distance : data_points[i].radius;
      data_points[i].theta > Math.PI ? data_points[i].theta = Math.PI : data_points[i].theta;
      data_points[i].phi > (Math.PI * 2) ? data_points[i].phi = Math.PI * 2 : data_points[i].phi;
    }


  }

  return data_types.Point3dArrayToBuffer(data_points);
}




