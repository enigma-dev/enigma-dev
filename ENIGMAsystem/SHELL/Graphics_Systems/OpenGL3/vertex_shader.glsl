in vec3 in_Position;      // (x,y,z)
in vec3 in_Normal;        // (x,y,z)
in vec4 in_Color;         // (r,g,b,a)
in vec2 in_TextureCoord;  // (u,v)

out vec2 v_TextureCoord;
out vec4 v_Color;
uniform int en_ActiveLights;
uniform bool en_ColorEnabled;

uniform bool en_LightingEnabled;
uniform bool en_VS_FogEnabled;
uniform float en_FogStart;
uniform float en_RcpFogRange;

uniform vec4 en_BoundColor;

#define MAX_LIGHTS   8

uniform vec4   en_AmbientColor;                              // rgba=color

struct LightInfo {
  vec4 Position; // Light position in eye coords
  vec4 La; // Ambient light intensity
  vec4 Ld; // Diffuse light intensity
  vec4 Ls; // Specular light intensity
  float cA, lA, qA; // Attenuation for point lights
};
uniform LightInfo Light[MAX_LIGHTS];

struct MaterialInfo {
  vec4 Ka; // Ambient reflectivity
  vec4 Kd; // Diffuse reflectivity
  vec4 Ks; // Specular reflectivity
  float Shininess; // Specular shininess factor
};
uniform MaterialInfo Material;

void getEyeSpace( inout vec3 norm, inout vec4 position )
{
  norm = normalize( normalMatrix * in_Normal );
  position = modelViewMatrix * vec4(in_Position, 1.0);
}

vec4 phongModel( in vec3 norm, in vec4 position )
{
  vec4 total_light = vec4(0.0);
  vec3 v = normalize(-position.xyz);
  float attenuation;
  for (int index = 0; index < en_ActiveLights; ++index){
    vec3 L;
    if (Light[index].Position.w == 0.0){ //Directional light
      L = normalize(Light[index].Position.xyz);
      attenuation = 1.0;
    }else{ //Point light
      vec3 positionToLightSource = vec3(Light[index].Position.xyz - position.xyz);
      float distance = length(positionToLightSource);
      L = normalize(positionToLightSource);
      attenuation = 1.0 / (Light[index].cA + Light[index].lA * distance + Light[index].qA * distance * distance);
    }
    vec3 r = reflect( -L, norm );
    total_light += Light[index].La * Material.Ka;
    float LdotN = max( dot(norm, L), 0.0 );
    vec4 diffuse = vec4(attenuation * vec3(Light[index].Ld) * vec3(Material.Kd) * LdotN,1.0);
    vec4 spec = vec4(0.0);
    if( LdotN > 0.0 )
        spec = clamp(vec4(attenuation * vec3(Light[index].Ls) * vec3(Material.Ks) * pow( max( dot(r,v), 0.0 ), Material.Shininess ),1.0),0.0,1.0);
    total_light += diffuse + spec;
  }
  return total_light;
}

void main() {
  vec4 iColor = en_BoundColor;
  if (en_ColorEnabled == true){
    iColor = in_Color;
  }
  if (en_LightingEnabled == true){
    vec3 eyeNorm;
    vec4 eyePosition;
    getEyeSpace(eyeNorm, eyePosition);
    v_Color = (en_AmbientColor + phongModel( eyeNorm, eyePosition )) * iColor;
  }else{
    v_Color = iColor;
  }
  gl_Position = modelViewProjectionMatrix * vec4( in_Position.xyz, 1.0);

  v_TextureCoord = in_TextureCoord;
}
