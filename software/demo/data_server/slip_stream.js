var stream = require('stream');
var util = require('util');
var slip = require('node-slip');


util.inherits(SlipStream, stream.Transform);

exports.SlipStream = SlipStream;

function SlipStream(options)
{
  if(!(this instanceof SlipStream))
  {
    return new SlipStream(options);
  }

  stream.Transform.call(this, options);
  
  var self = this;


 
  // slip receiver
  this._slip_receiver = {
    // data packet received
    data: function(input) {
      if(input != null)
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
  this._slip_parser = new slip.parser(this._slip_receiver, false);
}




// get data from data stream and decode it
SlipStream.prototype._transform = function(chunk, encoding, done) {

    
  this._slip_parser.write(chunk); // parse data
 
  done(); 
} 



