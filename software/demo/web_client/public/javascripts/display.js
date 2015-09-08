var data_points = [];

var shader = null;
var engine = null;

function main()
{

  // get contex
  var canvas = document.getElementById("display_canvas");
  canvas.width  = canvas.clientWidth;
  canvas.height = canvas.clientHeight;
 
  var gl = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");
  if(gl == null)
  {
    throw "cannot get gl context";
  }
  gl.viewport(0,0,canvas.width, canvas.height); 
  // create engine
  engine = new Engine(gl);
  engine.setClearColor(1,1,1,1); 
  engine.renderScene();
 
  // create shader
  var vertexShaderSource = loadTextFile("javascripts/shader.vert");
  var fragShaderSource = loadTextFile("javascripts/shader.frag");  
  shader = new ShaderProgram(gl, vertexShaderSource, fragShaderSource);
  shader.compile();
  shader.makeActive(); 


  var sphere = new Sphere(2, 50, 50, 1);
  var sphere_vbo = new VertexBufferObject(shader);
  sphere_vbo.addAttributeArray("position", sphere.mPosition, 3); 
  sphere_vbo.addAttributeArray("normal", sphere.mNormal, 3);
  sphere_vbo.addAttributeArray("texcoords", sphere.mTex, 2);
  var ary = [];
  for(var i = 0; i < 3; i++)
    ary.push(0);
  sphere_vbo.addInstancedArray("offset", ary, 3);
  sphere_vbo.setIndices(sphere.mIndex);

  var cube_vbo = new VertexBufferObject(shader);
  cube_vbo.addAttributeArray("position", cube_positions, 3);
  cube_vbo.addAttributeArray("normal", cube_colors, 3);
  cube_vbo.addAttributeArray("texcoords", cube_texcoords, 2);
  cube_vbo.addInstancedArray("offset", ary, 3);
  cube_vbo.setIndices(cube_indices);
  var cubeNode1 = create_sphere_node(shader, cube_vbo, [0,0,0], [1,0,0]);
  cubeNode1.scale([2,.5,.5]);
  cubeNode1.translate([1,0,0]);  

  var cubeNode2 = create_sphere_node(shader, cube_vbo, [0,0,0], [0,1,0]);
  cubeNode2.scale([.5,2,.5]);
  cubeNode2.translate([0,1,0]);  

  var cubeNode3 = create_sphere_node(shader, cube_vbo, [0,0,0], [0,0,1]);
  cubeNode3.scale([.5,.5,2]);
  cubeNode3.translate([0,0,1]);  




  var sensor_origin = new Node();
  sensor_origin.addChild(create_sphere_node(shader, sphere_vbo, [0,0,0], [1,1,1]));
  sensor_origin.addChild(cubeNode1); 
  sensor_origin.addChild(cubeNode2);
  sensor_origin.addChild(cubeNode3);




  engine.addNode(sensor_origin);



  // projection matrix
  var projection_matrix = mat4.create();
  mat4.identity(projection_matrix, projection_matrix);
  mat4.perspective(projection_matrix, Math.PI/3, canvas.width / canvas.height, .1, 1000);

  // set global shader properties
  shader.setUniform("projMat", projection_matrix); 
  shader.setUniform("light[0].mI0", [1,1,1]);
  shader.setUniform("light[0].mPosition", [0,100,150]);
  shader.setUniform("light[0].mQuadraticAttenuation", [0.01]);
  shader.setUniform("light[0].mEnabled", 1); 
  shader.setUniform("light[1].mI0", [1,1,1]);
  shader.setUniform("light[1].mPosition", [0,150,-100]);
  shader.setUniform("light[1].mQuadraticAttenuation", [0.01]);
  shader.setUniform("light[1].mEnabled", 1);   
  shader.setUniform("light[2].mI0", [1,1,1]);
  shader.setUniform("light[2].mPosition", [0,-100,150]);
  shader.setUniform("light[2].mQuadraticAttenuation", [0.01]);
  shader.setUniform("light[2].mEnabled", 1);   
  shader.setUniform("light[3].mI0", [1,1,1]);
  shader.setUniform("light[3].mPosition", [0,-150,-100]);
  shader.setUniform("light[3].mQuadraticAttenuation", [0.01]);
  shader.setUniform("light[3].mEnabled", 1);  
  shader.setUniform("ambientLight", [0,0,0]); 
 
  // create camera and make it orbit scene.(will have a better way to do this some day...
  var camera = new Camera(); 
  // starting pos
  camera.setPosition([0,25,0]);
  camera.rotate(Math.PI/2, [1,0,0]);
 
  var mouseLoc = [0,0,0,0];
  var keyspressed = {};
  keyspressed['q'] = 0;
  keyspressed['e'] = 0;
  keyspressed['w'] = 0;
  keyspressed['s'] = 0;
  keyspressed['a'] = 0;
  keyspressed['d'] = 0;
  keyspressed['z'] = 0;
  keyspressed['x'] = 0;

  // need a way to do this in engine loop
  // some kind of callback not sure yet
  setInterval(function()
  {

    var normalizedMouseLoc = vec4.create();
    vec4.normalize(normalizedMouseLoc, mouseLoc); 

    //var matrix = camera.getMatrix();
    var matrix = mat4.create();
    mat4.invert(matrix, camera.getMatrix());
    vec4.transformMat4(normalizedMouseLoc, normalizedMouseLoc, matrix);

    var axis = vec3.create();
    vec3.cross(axis, normalizedMouseLoc, camera.getForwardVector());

    vec3.normalize(axis,axis);

    var test = camera.getForwardVector();
   
    var length = vec3.length(mouseLoc);

    if(length > 100)
    {
      camera.rotate(length / 10000, axis);

    }
    if(keyspressed['e'] == 1)
    {
      camera.rotate(.05, camera.getForwardVector());
    }
    if(keyspressed['q'] == 1)
    {
      camera.rotate(-.05, camera.getForwardVector());
    }
    if(keyspressed['w'] == 1)
    {
      var vec = vec3.create();
      vec3.scale(vec, camera.getForwardVector(), -1);
      camera.translate(vec);
    }
    if(keyspressed['s'] == 1)
    {
      var vec = vec3.create();
      vec3.scale(vec, camera.getForwardVector(), 1);
      camera.translate(vec);
    }
    if(keyspressed['a'] == 1)
    {
      var vec = vec3.create();
      vec3.cross(vec, camera.getUpVector(), camera.getForwardVector());
      camera.translate(vec);
    }
    if(keyspressed['d'] == 1)
    {
      var vec = vec3.create();
      vec3.cross(vec, camera.getForwardVector(), camera.getUpVector());
      camera.translate(vec);  
    }
    if(keyspressed['z'] == 1)
    {
      var vec = vec3.create();
      vec3.scale(vec, camera.getUpVector(), 1);
      camera.translate(vec);
    }
    if(keyspressed['x'] == 1)
    {
      var vec = vec3.create();
      vec3.scale(vec, camera.getUpVector(), -1);
      camera.translate(vec);
    }

    shader.setUniform("viewMat", camera.getMatrix());
    shader.setUniform("viewPosition", camera.getPosition());  
  }, 30);

  // user input
  canvas.onmousemove = function(mouseEvent)
  {
    mouseLoc[0] = mouseEvent.x - canvas.width / 2;
    mouseLoc[1] = canvas.height / 2 - mouseEvent.y;  
  }
  
  window.onkeyup = function(key)
  {
    if(key.keyCode == 81)
    {
      keyspressed['q'] = 0;
    }
    if(key.keyCode == 69)
    {
      keyspressed['e'] = 0;
    }
    if(key.keyCode == 87)
    {
      keyspressed['w'] = 0;
    }
    if(key.keyCode == 83)
    {
      keyspressed['s'] = 0;
    }
    if(key.keyCode == 68)
    {
      keyspressed['d'] = 0;
    }
    if(key.keyCode == 65)
    {
      keyspressed['a'] = 0;
    }
    if(key.keyCode == 90)
    {
      keyspressed['z'] = 0;
    }
    if(key.keyCode == 88)
    {
      keyspressed['x'] = 0;
    }
  }
  window.onkeydown = function(key)
  {
    if(key.keyCode == 81)
    {
      keyspressed['q'] = 1;
    }
    if(key.keyCode == 69)
    {
      keyspressed['e'] = 1;
    }
    if(key.keyCode == 87)
    {
      keyspressed['w'] = 1;
    }
    if(key.keyCode == 83)
    {
      keyspressed['s'] = 1;
    } 
    if(key.keyCode == 68)
    {
      keyspressed['d'] = 1;
    }
    if(key.keyCode == 65)
    {
      keyspressed['a'] = 1;
    } 
    if(key.keyCode == 90)
    {
      keyspressed['z'] = 1;
    }
    if(key.keyCode == 88)
    {
      keyspressed['x'] = 1;
    } 


  }

  var full_screen_node = new Node();
  full_screen_node.addAction(new SimpleSynchronousAction(function() {
    canvas.width = window.innerWidth; 
    canvas.height = window.innerHeight;
    gl.viewport(0,0,canvas.width, canvas.height);
  
   
    // projection matrix
    var projection_matrix = mat4.create();
    mat4.identity(projection_matrix, projection_matrix);
    mat4.perspective(projection_matrix, Math.PI/4, canvas.width / canvas.height, 1, 1000);


    // calculate new projection matrix 
    shader.setUniform("projMat", projection_matrix);
 
   
  }, 1000));
  engine.addNode(full_screen_node);

  
  WebSocketInit();

}



function create_sphere_node(shader, vbo, pos, color)
{




  var kdiff = new UniformVariable(shader, "material.mKDiff");
  kdiff.setValue([1,1,1]);
  var kspec = new UniformVariable(shader, "material.mKSpec");
  kspec.setValue([1,1,1]);
  var kambient = new UniformVariable(shader, "material.mKAmbient");
  kambient.setValue([0,0,0]);
  var kshininess = new UniformVariable(shader, "material.mShininess");
  kshininess.setValue([50]);
  var kambient = new UniformVariable(shader, "material.mKAmbient");
  kambient.setValue([1,1,1]);
  var diffusemap = new UniformVariable(shader, "diffusemap");
  diffusemap.setValue(0);


  if(color)
  {
    kdiff.setValue(color)
  }

  var sphereNode = new Node();
  sphereNode.addAsset(kdiff);
  sphereNode.addAsset(kspec);
  sphereNode.addAsset(kambient);
  sphereNode.addAsset(kshininess);
  sphereNode.addAsset(kambient);
  sphereNode.addAsset(diffusemap);
  sphereNode.addDrawInterface(new DrawVertexBufferObject(vbo, "modelMat", "normalMat"));

  sphereNode.translate(pos);
 
  return sphereNode; 
}

function WebSocketInit()
{
  var sphere_vbo = null;
  if ("WebSocket" in window)
  {

    var ip_address = "ws:" + window.location.href.split(":")[1] + ":8080";
              
    // Let us open a web socket
    var ws = new WebSocket(ip_address);
    ws.binaryType = "arraybuffer"; 
    ws.onopen = function()
    {
      // Web Socket is connected, send data using send()


    };
        
    ws.onmessage = function (evt) 
    { 
      console.log("new data set received");
      
      var buffer = new Float32Array(evt.data);
      
      if(data_points.length == 0)
      {
        var sphere = new Sphere(.1, 10, 10);
        sphere_vbo = new VertexBufferObject(shader);
        sphere_vbo.addAttributeArray("position", sphere.mPosition, 3); 
        sphere_vbo.addAttributeArray("normal", sphere.mNormal, 3);
        sphere_vbo.addAttributeArray("texcoords", sphere.mTex, 2);
   
 

        sphere_vbo.addInstancedArray("offset", buffer, 3);
        
        sphere_vbo.setIndices(sphere.mIndex);

        var node = create_sphere_node(shader, sphere_vbo, [0,0,0])
	engine.addNode(node);
        data_points.push(node);
      }      

      sphere_vbo.updateInstancedBufferData("offset", buffer);  
    };
        
    ws.onclose = function()
    { 
      // websocket is closed.
      console.log('disconnected'); 
    };
  }
            
  else
  {
    // The browser doesn't support WebSocket
    alert("WebSocket NOT supported by your Browser!");
  }
}
