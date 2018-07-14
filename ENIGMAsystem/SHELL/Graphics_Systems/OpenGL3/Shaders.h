const char* VertexShaderPrefix =
#ifdef ENIGMA_GRAPHICS_OPENGLES3
  "#version 300 es\n"
  "precision mediump float;\n"
#elif defined ENIGMA_GRAPHICS_OPENGLES2
  "#version 200 es\n"
  "precision mediump float;\n"
#else
  "#version 330 core\n"
#endif
  
"#define MATRIX_VIEW                                    0\n"
"#define MATRIX_PROJECTION                              1\n"
"#define MATRIX_WORLD                                   2\n"
"#define MATRIX_WORLD_VIEW                              3\n"
"#define MATRIX_WORLD_VIEW_PROJECTION                   4\n"
"#define MATRICES_MAX                                   5\n"

"uniform mat4 transform_matrix[MATRICES_MAX];\n"
"#define gm_Matrices transform_matrix\n"
"#define modelMatrix transform_matrix[MATRIX_WORLD]\n"
"#define modelViewMatrix transform_matrix[MATRIX_WORLD_VIEW]\n"
"#define projectionMatrix transform_matrix[MATRIX_PROJECTION]\n"
"#define viewMatrix transform_matrix[MATRIX_VIEW]\n"
"#define modelViewProjectionMatrix transform_matrix[MATRIX_WORLD_VIEW_PROJECTION]\n"
"#define in_Colour in_Color\n"

"uniform mat3 normalMatrix;\n"
"#line 0\n";

const char* DefaultVertexShader =
"in vec3 in_Position;   // (x,y,z)\n"
"in vec3 in_Normal;           // (x,y,z)\n"
"in vec4 in_Color;            // (r,g,b,a)\n"
"in vec2 in_TextureCoord;     // (u,v)\n"

"out vec2 v_TextureCoord;\n"
"out vec4 v_Color;\n"
"uniform int en_ActiveLights;\n"
"uniform bool en_ColorEnabled;\n"

"uniform bool en_LightingEnabled;\n"
"uniform bool en_VS_FogEnabled;\n"
"uniform float en_FogStart;\n"
"uniform float en_RcpFogRange;\n"

"uniform vec4 en_BoundColor;\n"

"#define MAX_LIGHTS   8\n"

"uniform vec4   en_AmbientColor;                              // rgba=color\n"

"struct LightInfo {\n"
"  vec4 Position; // Light position in eye coords\n"
"  vec4 La; // Ambient light intensity\n"
"  vec4 Ld; // Diffuse light intensity\n"
"  vec4 Ls; // Specular light intensity\n"
"  float cA, lA, qA; // Attenuation for point lights\n"
"};"
"uniform LightInfo Light[MAX_LIGHTS];\n"

"struct MaterialInfo {\n"
"  vec4 Ka; // Ambient reflectivity\n"
"  vec4 Kd; // Diffuse reflectivity\n"
"  vec4 Ks; // Specular reflectivity\n"
"  float Shininess; // Specular shininess factor\n"
"};\n"
"uniform MaterialInfo Material;\n"

"void getEyeSpace( inout vec3 norm, inout vec4 position )\n"
"{\n"
"  norm = normalize( normalMatrix * in_Normal );\n"
"  position = modelViewMatrix * vec4(in_Position, 1.0);\n"
"}\n"

"vec4 phongModel( in vec3 norm, in vec4 position )\n"
"{\n"
"  vec4 total_light = vec4(0.0);\n"
"  vec3 v = normalize(-position.xyz);\n"
"  float attenuation;\n"
"  for (int index = 0; index < en_ActiveLights; ++index){\n"
"    vec3 L;\n"
"    if (Light[index].Position.w == 0.0){ //Directional light\n"
"      L = normalize(Light[index].Position.xyz);\n"
"      attenuation = 1.0;\n"
"    }else{ //Point light\n"
"      vec3 positionToLightSource = vec3(Light[index].Position.xyz - position.xyz);\n"
"      float distance = length(positionToLightSource);\n"
"      L = normalize(positionToLightSource);\n"
"      attenuation = 1.0 / (Light[index].cA + Light[index].lA * distance + Light[index].qA * distance * distance);\n"
"    }\n"
"    vec3 r = reflect( -L, norm );\n"
"    total_light += Light[index].La * Material.Ka;\n"
"    float LdotN = max( dot(norm, L), 0.0 );\n"
"    vec4 diffuse = vec4(attenuation * vec3(Light[index].Ld) * vec3(Material.Kd) * LdotN,1.0);\n"
"    vec4 spec = vec4(0.0);\n"
"    if( LdotN > 0.0 )\n"
"        spec = clamp(vec4(attenuation * vec3(Light[index].Ls) * vec3(Material.Ks) * pow( max( dot(r,v), 0.0 ), Material.Shininess ),1.0),0.0,1.0);\n"
"    total_light += diffuse + spec;\n"
"  }\n"
"  return total_light;\n"
"}\n"

"void main()\n"
"{\n"
"  vec4 iColor = en_BoundColor;\n"
"  if (en_ColorEnabled == true){\n"
"    iColor = in_Color;\n"
"  }\n"
"  if (en_LightingEnabled == true){\n"
"    vec3 eyeNorm;\n"
"    vec4 eyePosition;\n"
"    getEyeSpace(eyeNorm, eyePosition);\n"
"    v_Color = (en_AmbientColor + phongModel( eyeNorm, eyePosition )) * iColor;\n"
"  }else{\n"
"    v_Color = iColor;\n"
"  }\n"
"  gl_Position = modelViewProjectionMatrix * vec4( in_Position.xyz, 1.0);\n"

"  v_TextureCoord = in_TextureCoord;\n"
"}\n";

const char* FragmentShaderPrefix =
#ifdef ENIGMA_GRAPHICS_OPENGLES3
  "#version 300 es\n"
  "precision mediump float;\n"
#elif defined ENIGMA_GRAPHICS_OPENGLES2
  "#version 200 es\n"
  "precision mediump float;\n"
#else
  "#version 330 core\n"
#endif

"#define MATRIX_VIEW                                    0\n"
"#define MATRIX_PROJECTION                              1\n"
"#define MATRIX_WORLD                                   2\n"
"#define MATRIX_WORLD_VIEW                              3\n"
"#define MATRIX_WORLD_VIEW_PROJECTION                   4\n"
"#define MATRICES_MAX                                   5\n"

"uniform mat4 transform_matrix[MATRICES_MAX];\n"
"#define gm_Matrices transform_matrix\n"
"#define modelMatrix transform_matrix[MATRIX_WORLD]\n"
"#define modelViewMatrix transform_matrix[MATRIX_WORLD_VIEW]\n"
"#define projectionMatrix transform_matrix[MATRIX_PROJECTION]\n"
"#define viewMatrix transform_matrix[MATRIX_VIEW]\n"
"#define modelViewProjectionMatrix transform_matrix[MATRIX_WORLD_VIEW_PROJECTION]\n"

"uniform mat3 normalMatrix;\n"

"uniform sampler2D en_TexSampler;\n"
"#define gm_BaseTexture en_TexSampler\n"
"uniform bool en_TexturingEnabled;\n"
"uniform bool en_ColorEnabled;\n"
"uniform bool en_AlphaTestEnabled;\n"

"uniform float en_AlphaTestValue;\n"
"uniform vec4 en_BoundColor;\n"
"#line 0\n";

const char* DefaultFragmentShader =
"in vec2 v_TextureCoord;\n"
"in vec4 v_Color;\n"
"out vec4 out_FragColor;\n"

"void main()\n"
"{\n"
"  vec4 TexColor;\n"
"  if (en_TexturingEnabled == true){\n"
"    TexColor = texture2D( en_TexSampler, v_TextureCoord.st ) * v_Color;\n"
"  }else{\n"
"    TexColor = v_Color;\n"
"  }\n"
"  if (en_AlphaTestEnabled == true){\n"
"    if (TexColor.a<=en_AlphaTestValue) discard;\n"
"  }\n"
"  out_FragColor = TexColor;\n"
"}\n";
