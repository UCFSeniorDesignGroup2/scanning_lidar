var stream = require('stream');
var util = require('util');
var slip = require('node-slip');


util.inherits(SlipStream, stream.Duplex);

exports.SlipStream = SlipStream;

function SlipStream(stream_instance)
{
  if(!(this instanceof SlipStream))
  {
    return new SlipStream(stream_instance);
  }

  stream.Duplex.call(this);
  
  var self = this;

  // send data through
  this._stream = stream_instance;
  
  // give it a kick whenever the source is readable
  // read(0) will not consume any bytes
  this._stream.on('readable', function() {
    self.read(0);
  });
 
  // slip receiver
  this._slip_receiver = {
    // data packet received
    data: function(input) {
      self.push(input);
    }, 
    // framing error
    framing: function(input) {
      console.log('framing error');
    },
    // escape error
    escape: function(input) {
      console.log('escape error');
    }
  };
  
  // slip parser
  this._slip_parser = new slip.parser(this._slip_receiver);
}


// encode data and send it through data stream
SlipStream.prototype._write = function(chunk, encoding, cb) {
 
  if(this._stream != null)
  {
    this._stream.write(slip.generator(chunk));
  } 
  cb();
};


// get data from data stream and decode it
SlipStream.prototype._read = function(n) {

  var chunk = null;
  if(this._stream != null)
  {
    chunk = this._stream.read(); // read data from stream
  }
  
  if(chunk != null)
  {
    this._slip_parser.write(chunk); // parse data
  }
  else
  {
    this.push(''); // no data to parse
  } 
}


