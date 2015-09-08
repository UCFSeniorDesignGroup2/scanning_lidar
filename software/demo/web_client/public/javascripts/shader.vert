attribute vec3 position;
attribute vec3 normal;
attribute vec2 texcoords;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

uniform mat3 normalMat;

varying vec3 fposition;
varying vec3 fnormal;
varying vec2 ftexcoords;

void main()
{
  gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0);

  fnormal = normalize(normalMat * normal);
  fposition = (modelMat * vec4(position, 1.0)).xyz;

  ftexcoords = texcoords;
}
