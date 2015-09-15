attribute vec3 position;
attribute vec3 normal;
attribute vec2 texcoords;
attribute vec3 offset;


uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

uniform mat3 normalMat;
uniform int is_particle;

varying vec3 fposition;
varying vec3 fnormal;
varying vec2 ftexcoords;

void main()
{
  vec3 offset_cart = vec3(0.0,0.0,0.0);
  offset_cart.x = offset.x * sin(offset.y) * cos(offset.z);
  offset_cart.y = offset.x * sin(offset.y) * sin(offset.z);
  offset_cart.z = offset.x * cos(offset.y);
  
  if(is_particle == 1)
  {
    gl_Position = projMat * (viewMat * modelMat * vec4(offset_cart, 1.0) + vec4(position,0.0));
  }
  else
  {
    gl_Position = projMat * viewMat * modelMat * vec4(offset_cart + position, 1.0);
  }
  fnormal = normalize(normalMat * normal);
  fposition = (modelMat * vec4(position, 1.0)).xyz;

  ftexcoords = texcoords;
}
