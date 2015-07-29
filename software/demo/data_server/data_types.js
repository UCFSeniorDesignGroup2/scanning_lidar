// simple data point in spherical coordinates
exports.Point3d = function Point3d(r,theta,phi)
{
  this.radius = r;
  this.theta = theta;
  this.phi = phi;
}

// converts an array of Point3d objects to a Buffer object
exports.Point3dArrayToBuffer = function Point3dArrayToBuffer(ary)
{
  // create buffer with size in bytes
  var buffer = new Buffer(ary.length * 3 * 4);
  for(var i in ary)
  {
    // write data to buffer in little endian
    buffer.writeFloatLE(ary[i].radius, i*12 + 0);
    buffer.writeFloatLE(ary[i].theta, i*12 + 4);
    buffer.writeFloatLE(ary[i].phi, i*12 + 8); 
  }
  
  return buffer;
}

// converts a buffer object to an array of Point3d Objects
exports.BufferToPoint3dArray = function BufferToPoint3dArray(buffer)
{
  var ary = [];

  // length in floats
  var length = Math.floor(buffer.length / (4*3));

  for(var i = 0; i < length; i++)
  {
    // parse buffer and store points in array
    var radius = buffer.readFloatLE(i*12 + 0);
    var theta = buffer.readFloatLE(i*12 + 4);
    var phi = buffer.readFloatLE(i*12 + 8);
    var point = new exports.Point3d(radius,theta,phi);
    
    ary.push(point);
  }
  return ary;
}
