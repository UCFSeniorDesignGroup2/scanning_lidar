precision mediump float;

varying vec3 fposition;
varying vec3 fnormal;

varying vec2 ftexcoords;

struct Material
{
  vec3 mKAmbient;
  vec3 mKDiff;
  vec3 mKSpec;
  float mShininess;
};

struct Light
{
  vec3 mI0;
  vec3 mPosition;
  float mQuadraticAttenuation;
  int mEnabled;
};

uniform Material material;
uniform Light light1;
uniform Light light2;
uniform vec3 viewPosition;
uniform vec3 ambientLight;



uniform Light light[8];
uniform int diffusemap;


uniform sampler2D uSampler;

vec3 calculateDiffuseLighting(Material mat, Light light)
{ 
  float distance = length(light.mPosition - fposition);
  float attenuation = light.mQuadraticAttenuation * distance;


  vec3 lightDir = normalize(light.mPosition - fposition);
  vec3 IDiff = light.mI0 * mat.mKDiff * clamp(dot(fnormal, lightDir), 0.0,1.0) / (attenuation+1.0);
  return IDiff; 
}

vec3 calculateSpecularLighting(Material mat, Light light, 
                               vec3 viewDirection) 
{
  float distance = length(light.mPosition - fposition);
  float attenuation = light.mQuadraticAttenuation * distance;

  vec3 lightDir = normalize(light.mPosition - fposition);
  vec3 ISpec = light.mI0 * mat.mKSpec * pow(clamp(dot(reflect(-lightDir, fnormal), viewDirection),0.0,1.0), mat.mShininess) / (attenuation+1.0); 

  return ISpec;
}

void main()
{
  vec3 texcolor = texture2D(uSampler, ftexcoords).xyz; 
  Material mat = material;
  if(diffusemap == 1)
  {
    mat.mKDiff = texcolor;
  }

  vec3 viewDirection = normalize(viewPosition - fposition);
   
  vec3 IDiff = vec3(0.0);
  vec3 ISpec = vec3(0.0); 


  for(int i = 0; i < 8; i++)
  {
    if(light[i].mEnabled == 1)
    {
      IDiff += calculateDiffuseLighting(mat, light[i]); 
      ISpec += calculateSpecularLighting(mat, light[i], viewDirection);
    }
  }

 
  vec3 IAmbient = ambientLight * material.mKAmbient; 
 

  
  gl_FragColor = vec4(IDiff + ISpec + IAmbient, 1.0);
}
