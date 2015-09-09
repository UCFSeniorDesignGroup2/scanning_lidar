/* Programmer: Jackson Ritchey
 *
 * Shader code for creating shaders, and
 * passing vbos to shader program.
 */
// constructor for shader program
// optional: projMatName, viewMAtname, modelMatName
function ShaderProgram(gl, vertex_src, fragment_src)
{
  
  this.vertexShader = null;
  this.fragmentShader = null;   
  this.attributeLocations = [];
  this.shaderProgram = null;
  this.gl = gl;

  this.getVertexShaderSourceCode = function()
  {
    return vertex_src;
  }
  this.getFragmentShaderSourceCode = function()
  {
    return fragment_src;
  }
}

// compile shaders
ShaderProgram.prototype.compile = function()
{
  this.vertexShader = this.gl.createShader(this.gl.VERTEX_SHADER); 
  this.gl.shaderSource(this.vertexShader, this.getVertexShaderSourceCode());
  this.gl.compileShader(this.vertexShader);

  if(!this.gl.getShaderParameter(this.vertexShader, this.gl.COMPILE_STATUS))
  {
    throw "Engine:vertex shader: " + this.gl.getShaderInfoLog(this.vertexShader);
  } 
  
  this.fragmentShader = this.gl.createShader(this.gl.FRAGMENT_SHADER);
  this.gl.shaderSource(this.fragmentShader, this.getFragmentShaderSourceCode());
  this.gl.compileShader(this.fragmentShader);

  if(!this.gl.getShaderParameter(this.fragmentShader, this.gl.COMPILE_STATUS))
  {
    throw "Engine:fragment shader: " + this.gl.getShaderInfoLog(this.fragmentShader);
  }

  this.shaderProgram = this.gl.createProgram();
  this.gl.attachShader(this.shaderProgram, this.vertexShader);
  this.gl.attachShader(this.shaderProgram, this.fragmentShader); 
  this.gl.linkProgram(this.shaderProgram);    

  if(!this.gl.getProgramParameter(this.shaderProgram, this.gl.LINK_STATUS))
  {
    throw "Engine:Error linking Shader!";
  }
}

// make shader active
ShaderProgram.prototype.makeActive = function()
{
  this.gl.useProgram(this.shaderProgram);
}

ShaderProgram.prototype.setUniform = function(name, val)
{
  var loc = this.gl.getUniformLocation(this.shaderProgram, name);
  if(loc < 0)
  {
    return loc;
  }
  if(val.length)
  {
  switch(val.length)
  {
  case 1:
    this.gl.uniform1f(loc, val);
    break;

  case 2:
    this.gl.uniform2f(loc, val[0], val[1]);
    break;

  case 3:
    this.gl.uniform3f(loc, val[0], val[1], val[2]);
    break;

  case 4:
    this.gl.uniform4f(loc, val[0], val[1], val[2], val[3]);
    break;

  case 9:
    this.gl.uniformMatrix3fv(loc, false, val);
    break;

  case 16:
    this.gl.uniformMatrix4fv(loc, false, val);
    break;

  default:
    throw "not a datatype";
    break;
  }
  }
  else
  {
    this.gl.uniform1i(loc, val);
  }
  return loc;
}

// constructor for VBO
function VertexBufferObject(shader)
{
  this.shader = shader;
  this.attributeBuffers = [];
  this.instancedBuffers = [];
  this.instancedExt = null;
  this.indexBuffer = null;
 
}

// add an attribute array
VertexBufferObject.prototype.addAttributeArray = function(name, ary, itemSize)
{
  var numItems = ary.length / itemSize; 
  if(ary.length != itemSize * numItems)
  {
    throw "Engine:bad array dimensions"
  } 
  var gl = this.shader.gl;
  var buffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
     
  // other datatypes? just static draw?
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(ary), gl.STATIC_DRAW);
  buffer.itemSize = itemSize;
  buffer.numItems = numItems;
  buffer.attributeLocation = gl.getAttribLocation(this.shader.shaderProgram, name);//this.shader[name]; 
  buffer.name = name;  


  if(buffer.attributeLocation < 0)
  {
    throw "Engine:attribute not found in shader!";
  }

  gl.enableVertexAttribArray(buffer.attributeLocation);   
  this.attributeBuffers.push(buffer);
  this.mAssets = [];
}

// add attribute array that is used in instance drawing mode for particle effects
VertexBufferObject.prototype.addInstancedArray = function(name, ary, itemSize) {

  var numItems = ary.length / itemSize;

  if(ary.length != itemSize * numItems)
  {
    throw "Engine:bad array dimensions";
  }
  
  var gl = this.shader.gl;
  this.instancedExt = gl.getExtension("ANGLE_instanced_arrays"); 


  var buffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
  if(ary instanceof Float32Array)
  {
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(ary), gl.STREAM_DRAW);
  }
  else
  {
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(ary), gl.STREAM_DRAW);
  }
  buffer.itemSize = itemSize;
  buffer.numItems = numItems;
  buffer.attributeLocation = gl.getAttribLocation(this.shader.shaderProgram, name);
  buffer.name = name;
  
  if(buffer.attributeLocation < 0)
  {
    throw "Engine:attribute not found in shader!"; 
  }
 
  gl.enableVertexAttribArray(buffer.attributeLocation);
  this.instancedBuffers.push(buffer);
}

// update data in a instanced buffer object
VertexBufferObject.prototype.updateInstancedBufferData = function(name, data)
{ 
  var buffer = null;
  // find the buffer to update
  for(var i = 0; i < this.instancedBuffers.length; i++)
  { 
    if(name == this.instancedBuffers[i].name)
    {
      buffer = this.instancedBuffers[i];
    }
  }
  if(buffer)
  {
    var gl = this.shader.gl;
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    if(data instanceof Float32Array)
    {
      gl.bufferData(gl.ARRAY_BUFFER, data, gl.STREAM_DRAW);
    }
    else
    {
      gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STREAM_DRAW);
    }
  }
}

VertexBufferObject.prototype.addAsset = function(asset)
{
  if(!(asset instanceof Asset))
  {
    throw "Engine: parameter not instance of Asset";
  }
  this.mAssets.push(asset);
}
VertexBufferObject.prototype.removeAsset = function(asset)
{
  if(!(asset instanceof Asset))
  {
    throw "Engine: parameter not instance of Asset";
  }
  this.mAssets.splice(this.mAssets.indexOf(asset), 1);
}

  


// set indices into attribute arrays(optional; if null use drawArraws)
VertexBufferObject.prototype.setIndices = function(indices)
{
  var gl = this.shader.gl;
  this.indexBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
  // static draw for now?
  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);
  this.indexBuffer.itemSize = 1;
  this.indexBuffer.numItems = indices.length;
}

// bind to all assets in vbo
VertexBufferObject.prototype.bind = function()
{
  var gl = this.shader.gl;

  for(var i = 0; i < this.mAssets.length; i++)
  {
    this.mAssets[i].bind();
  }


  for(var i in this.attributeBuffers)
  {
    var buffer = this.attributeBuffers[i];
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    gl.vertexAttribPointer(buffer.attributeLocation, buffer.itemSize, gl.FLOAT, false, buffer.length, 0); 
  }

  if(this.indexBuffer != null)
  {
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
  } 
  
  if(this.instancedExt)
  {
    for(var i in this.instancedBuffers)
    {
      var buffer = this.instancedBuffers[i];
      gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
      gl.vertexAttribPointer(buffer.attributeLocation, buffer.itemSize, gl.FLOAT, false, buffer.length, 0);
      this.instancedExt.vertexAttribDivisorANGLE(buffer.attributeLocation, 1);
    }
  }
}

// make draw call on vbo
VertexBufferObject.prototype.draw = function(interpolator)
{
  var gl = this.shader.gl;
  
  //gl.loadMatrixf(interpolator);
  if(!this.instancedExt)
  {
    if(this.indexBuffer == null)
    {
      gl.drawArrays(gl.TRIANGLES, 0, this.attributeBuffers[0].numItems);
    }
    else
    {
      gl.drawElements(gl.TRIANGLES, this.indexBuffer.numItems, gl.UNSIGNED_SHORT, 0);
    }
  }
  else
  {
    if(this.indexBuffer != null)
    {
      this.instancedExt.drawElementsInstancedANGLE(gl.TRIANGLES, this.indexBuffer.numItems, gl.UNSIGNED_SHORT, 0, this.instancedBuffers[0].numItems);
    }
  }
}

VertexBufferObject.prototype.unbind = function()
{
  for(var i = 0; i < this.mAssets.length; i++)
  {
    this.mAssets[i].unbind();
  } 
}


// node in a scenegraph-like structure
function Node()
{
  
  var mParentNode = null;
  var mChildNodes = [];
  var mDrawInterfaces = [];
  var mActions = [];
  var mAssets = [];

  var mTransform = mat4.create();
  mat4.identity(mTransform); 

  this.setLocalPosition = function(position)
  {
    mTransform[12] = position[0];
    mTransform[13] = position[1];
    mTransform[14] = position[2];
  }
  this.getLocalPosition = function()
  {
    var position = [mTransform[12], mTransform[13], mTransform[14]];
    return position;
  }
  this.getGlobalPosition = function()
  {
    var glbtran = this.getGlobalTransform();

    return [glbtran[12],glbtran[13],glbtran[14]];
  }
  this.rotate = function(angle, axis)
  {
    mat4.rotate(mTransform, mTransform, angle, axis);
  }
  this.translate = function(vector)
  {
    mat4.translate(mTransform, mTransform, vector);
  }
  this.scale = function(scale)
  {
    mat4.scale(mTransform, mTransform, scale);
  } 
  this.setParent = function(parentNode)
  {
    if(!(parentNode instanceof Node))
    {
      throw "Engine:Parameter not an instance of Node!";
    }
    mParentNode = parentNode;
  }
  this.getParent = function()
  {
    return parentNode;
  }
  this.addChild = function(childNode)
  {
    if(!(childNode instanceof Node))
    {
      throw "Engine:Paramenter not an instance of Node!";
    }
    childNode.setParent(this);
    mChildNodes.push(childNode);
  }
  this.removeChild = function(childNode)
  {
    if(!(childNode instanceof Node))
    {
      throw "Engine:Parameter not instance of Node!";
    }
    return mChildNodes.splice(mChildNodes.indexOf(childNode), 1);
  }
  this.removeChildAt = function(index)
  {
    return mChildnodes.splice(index, 1);
  }
  this.getNumChildren = function()
  {
    return mChildNodes.length;
  }
  this.getChildAt = function(index)
  {
    return mChildNodes[index];
  }
  this.addDrawInterface = function(drawInterface)
  {
    if(!(drawInterface instanceof DrawInterface))
    {
      throw "Engine:Parameter not instance of DrawInterface!";
    }
    mDrawInterfaces.push(drawInterface);
  }
  this.removeDrawInterface = function(drawInterface)
  { 
    if(!(drawInterface instanceof DrawInterface))
    {
      throw "Engine:Parameter not instance of DrawInterface!";
    }
    
    return mDrawInterfaces.splice(mDrawInterfaces.indexOf(drawInterface), 1);
  }
  this.removeDrawInterfaceAt = function(index)
  {
    return mDrawInterfaces.splice(index, 1);
  }
  this.getNumDrawInterfaces = function()
  {
    return mDrawInterfaces.length;
  }
  this.getDrawInterfaceAt = function(index)
  {
    return mDrawInterfaces[index];
  }
  this.addAction = function(action)
  {
    if(!(action instanceof Action))
    {
      throw "Engine:Parameter not instance of Action!";
    }

    mActions.push(action);
  }
  this.removeAction = function(action)
  { 
    if(!(action instanceof Action))
    {
      throw "Engine:Parameter not instance of Action!";
    }

  
    return mActions.splice(mActions.indexOf(action), 1);
  }
  this.removeActionAt = function(index)
  {
    return mActions.spice(index, 1);
  }
  this.getNumActions = function()
  {
    return mActions.length;
  } 
  this.getActionAt = function(index)
  {
    return mActions[index];
  }
  this.addAsset = function(asset)
  {
    if(!asset instanceof Asset)
    {
      throw "Engine: parameter not an Asset!";
    }
    mAssets.push(asset);
  }
  this.removeAsset = function(asset)
  {
    if(!(asset instanceof Asset))
    {
      throw "Engine: parameter not an Asset!";
    }
    mAssets.splice(mAssets.indexOf(asset), 1);
  }
  

  this.setTransform = function(transform)
  {
    mTransform = transform;
  }
  this.getTransform = function()
  {
    return mTransform;
  }
  this.getOrientation = function()
  {
    var rotationMat = mat4.clone(mTransform);
    
    rotationMat[12] = 0;
    rotationMat[13] = 0;
    rotationMat[14] = 0;
    return vec3.normalize(mat4.multiplyVec3(rotationMat, [1,0,0,1]));
  }
  this.getGlobalTransform = function()
  {
    var transform = mat4.clone(mTransform);
    if(mParentNode == null)
    {
      return transform;
    }
    var result = mat4.create();
    mat4.multiply(result, mParentNode.getGlobalTransform(), transform);

    return result;
  }

 this.draw = function(delta)
  {
    var interpolator = this.getGlobalTransform(); // for now
    for(var i = 0; i < mAssets.length; i++)
    {
      mAssets[i].bind();
    }    



    for(var i = 0; i < mDrawInterfaces.length; i++)
    {
      mDrawInterfaces[i].draw(interpolator);
    }   
    for(var i = 0; i < mAssets.length; i++)
    {
      mAssets[i].unbind();
    } 

    for(var i = 0; i < mChildNodes.length; i++)
    {
      mChildNodes[i].draw(delta);
    }

  }
  
  this.update = function()
  {
    for(var i = 0; i < mActions.length; i++)
    {
      mActions[i].performAction();
    }    
    for(var i = 0; i < mChildNodes.length; i++)
    {
      mChildNodes[i].update();
    }
  }
  


 }

// interface for drawing things
function DrawInterface()
{
}

// called by a node when it is being rendered
DrawInterface.prototype.draw = function()
{
}

// interface for animating things
function Action()
{
}
// called by node when it is being updated
Action.prototype.performAction = function()
{
}

// example draw interface
// optional normalmatrx if rendering phong lighting model
function DrawVertexBufferObject(vertexBufferObject, modelMatrixName, normalMatrixName)
{
  
  DrawInterface.call(this);
  
  this.mVertexBufferObject = vertexBufferObject; 
  this.mUniformNormalMatrixName = normalMatrixName;
  this.mUniformModelMatrixName = modelMatrixName;
 
  this.mNormalMat = mat3.create();
}

DrawVertexBufferObject.prototype = Object.create(DrawInterface.prototype);
DrawVertexBufferObject.constructor = DrawVertexBufferObject;


DrawVertexBufferObject.prototype.draw = function(interpolator)
{
  var shader = this.mVertexBufferObject.shader;

  mat3.normalFromMat4(this.mNormalMat, interpolator);
  
  shader.setUniform(this.mUniformModelMatrixName, interpolator);
  shader.setUniform(this.mUniformNormalMatrixName, this.mNormalMat);
  
  this.mVertexBufferObject.bind(); 
  this.mVertexBufferObject.draw(); 
}

// used for animating things at constant frequency
function SynchronousAction()
{
  Action.call(this);
  this.mLastDraw = 0;
  this.mUpdateFrequency = 1;
}

SynchronousAction.prototype = Object.create(Action.prototype);
SynchronousAction.constructor = SynchronousAction;

// calls synchronizedAction on a set interval
SynchronousAction.prototype.performAction = function()
{
  
  if((new Date()).getTime() - this.mLastDraw > 1000/this.mUpdateFrequency)
  {
    this.synchronizedAction();
    this.mLastDraw = (new Date()).getTime();
  }
}

SynchronousAction.prototype.synchronizedAction = function()
{
}

SynchronousAction.prototype.setUpdateFrequency = function(freq)
{
  this.mUpdateFrequency = freq;
}
SynchronousAction.prototype.getUpdateFrequency = function()
{
  return this.mUpdateFrequency;
}

function SimpleAction(func)
{
  this.func = func;
  Action.call(this);
}

SimpleAction.prototype = Object.create(Action.prototype);
SimpleAction.constructor = SimpleAction;

SimpleAction.prototype.performAction = function()
{
  this.func();
}

function SimpleSynchronousAction(func, freq)
{
  SynchronousAction.call(this);
  this.func = func;
  this.setUpdateFrequency(freq);
}

SimpleSynchronousAction.prototype = Object.create(SynchronousAction.prototype);
SimpleSynchronousAction.constructor = SimpleSynchronousAction;

SimpleSynchronousAction.prototype.synchronizedAction = function()
{
  this.func();
}



// assets used by shader for rendering ex. Uniform variables and textures
function Asset(shader)
{
  if(!(shader instanceof ShaderProgram))
  {
    throw "Engine: not a shader!";
  }
  this.mShader = shader;  

  
}

// called right before draw interfaces are called if it is added to a node
// if it is added to a VertexBufferObject than it is called whem vbo.bind()
// is called 
Asset.prototype.bind = function()
{
}

// called right after the draw interfaces are called 
Asset.prototype.unbind = function()
{
}

// texture 
function Texture(shader, uniformName, imageName)
{
 
  Asset.call(this, shader);
  this.mUniformName = uniformName;
  this.mImageName = imageName;

  var gl = this.mShader.gl;
  this.mTexture = gl.createTexture();
  
  var that = this;

  this.mImage = new Image();
  this.mLoaded = false;

  this.mImage.onload = function() {
    gl.bindTexture(gl.TEXTURE_2D, that.mTexture);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, that.mImage);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_NEAREST);
    gl.generateMipmap(gl.TEXTURE_2D);
    gl.bindTexture(gl.TEXTURE_2D, null);
    that.mLoaded = true;
  };

  this.mImage.src = imageName;

   

}

Texture.prototype = Object.create(Asset.prototype);
Texture.constructor = Texture;


Texture.prototype.bind = function()
{
  var gl = this.mShader.gl;
  if(this.mLoaded == true)
  {
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, this.mTexture);
    gl.uniform1i(gl.getUniformLocation(this.mShader.shaderProgram, this.mUniformName), 0);
  } 
}


Texture.prototype.unbind = function()
{
  var gl = this.mShader.gl;  
//  gl.bindTexture(gl.TEXTURE2D, null);
 
}

function UniformVariable(shader, uniformName)
{
  Asset.call(this, shader);
  this.mValue = null;
  this.mName = uniformName;

}

UniformVariable.prototype = Object.create(Asset.prototype);
UniformVariable.constructor = UniformVariable;

UniformVariable.prototype.bind = function()
{

  if(this.mValue != null)
  {
    this.mShader.setUniform(this.mName, this.mValue);
  }
}

UniformVariable.prototype.setValue = function(value)
{
  this.mValue = value;
}
// stupid simple engine 
function Engine(gl)
{
  this.mRootNode = new Node(); 
  this.gl = gl;
  var that = this;
  gl.enable(gl.DEPTH_TEST);
  gl.clearColor(0,1,1,1);
  // add a node to the scene
  this.addNode = function(node)
  {
    if(!(node instanceof Node))
    {
      throw "Engine:Parameter not instance of Node!";
    }
    this.mRootNode.addChild(node);
  } 
  this.removeNode = function(node)
  {
    if(!(node instanceof Node))
    {
      throw "Engine:Parameter node instance of Node!";
    }
    this.mRootNode.remove(node);
  } 
  this.setClearColor = function(r,g,b,a) 
  {
    that.gl.clearColor(r,g,b,a);
  }
  // loop for rendering scene
  this.renderScene = function()
  {
    // update root node 
    that.mRootNode.update();

    // clear
    that.gl.clear(that.gl.COLOR_BUFFER_BIT | that.gl.DEPTH_BUFFER_BIT);
  
    // traverse tree and draw the scene 
    that.mRootNode.draw(1.0);
  
    // continue loping 
    requestAnimationFrame(that.renderScene); 

  }

}

function Camera()
{
  this.viewMatrix = mat4.create();    
  mat4.identity(this.viewMatrix,this.viewMatrix);


  this.mPosition = [0,0,0,1];
  this.mForward = [0,0,-1];
  this.mUp = [0,1,0];

}

Camera.prototype.setMatrix = function(matrix)
{
  this.viewMatrix = matrix;
}
Camera.prototype.getMatrix = function()
{
   return mat4.clone(this.viewMatrix);  
}
Camera.prototype.rotate = function(angle, axis)
{
  var rotMat = mat4.create();
  mat4.identity(rotMat,rotMat);
  mat4.rotate(rotMat, rotMat, angle, axis);
  vec3.transformMat4(this.mForward, this.mForward, rotMat);
  vec3.transformMat4(this.mUp, this.mUp, rotMat);
  vec3.normalize(this.mForward, this.mForward);
  vec3.normalize(this.mUp, this.mUp);
}  
Camera.prototype.setPosition = function(pos)
{
  this.mPosition = vec3.clone(pos);
}
Camera.prototype.getPosition = function()
{
  return vec3.clone(this.mPosition);
}
Camera.prototype.translate = function(vec)
{
  vec3.add(this.mPosition, this.mPosition, vec);


}


Camera.prototype.setForwardVector = function(forward)
{
  this.mForward = vec3.clone(forward);
}
Camera.prototype.getForwardVector = function()
{
  return vec3.clone(this.mForward);
}
Camera.prototype.setUpVector = function(up)
{
  this.mUp = vec3.clone(up);
}
Camera.prototype.getUpVector = function()
{
  return vec3.clone(this.mUp);
}
Camera.prototype.lookAt = function(vec)
{
 
  vec3.subtract(this.mForward, this.mPosition, vec); 

  vec3.normalize(this.mForward, this.mForward);

}
Camera.prototype.getMatrix = function()
{
  vec3.normalize(this.mForward, this.mForward);
  vec3.normalize(this.mUp,this.mUp);


  var matrix = mat4.create();
//  mat4.translate(matrix,matrix,[1,2,3]);
 
  var xaxis = vec3.create();
  vec3.cross(xaxis, this.mForward, this.mUp);

  
  matrix[0] = xaxis[0]; matrix[4] = this.mUp[0]; matrix[8] = this.mForward[0];
  matrix[1] = xaxis[1]; matrix[5] = this.mUp[1]; matrix[9] = this.mForward[1];
  matrix[2] = xaxis[2]; matrix[6] = this.mUp[2]; matrix[10] = this.mForward[2];
  matrix[3] = 0;        matrix[7] = 0;           matrix[11] = 0;

  matrix[12] = this.mPosition[0];
  matrix[13] = this.mPosition[1];
  matrix[14] = this.mPosition[2];
  matrix[15] = 1;


  mat4.invert(matrix,matrix);


  return matrix;
}

function loadTextFile(filename)
{
  var oReq = new XMLHttpRequest();
  oReq.open("GET", filename, false);
  oReq.send();
  return oReq.responseText;
}



