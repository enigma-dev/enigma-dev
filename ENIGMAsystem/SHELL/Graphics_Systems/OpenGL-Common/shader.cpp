/** Copyright (C) 2013-2014 Robert B. Colton, Harijs Grinbergs
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "version.h"
#include "shader.h"
#include "GLSLshader.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GStextures.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <cstring>      /* memcpy */
#include <math.h>

#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_uniform(uniter,location,usize)\
    string name = enigma::shaderprograms[enigma::bound_shader]->name;\
    char str[128];\
    if (name == ""){\
        sprintf(str, "Program[%i]", enigma::bound_shader);\
    }else{\
        sprintf(str, "Program[%s = %i]", name.c_str(), enigma::bound_shader);\
    }\
    if (location < 0) { DEBUG_MESSAGE(std::string(str) + " - Uniform location < 0 given (" + std::to_string(location) + ")!", MESSAGE_TYPE::M_ERROR); return; }\
    std::unordered_map<GLint,enigma::Uniform>::iterator uniter = enigma::shaderprograms[enigma::bound_shader]->uniforms.find(location);\
    if (uniter == enigma::shaderprograms[enigma::bound_shader]->uniforms.end()){\
        DEBUG_MESSAGE(std::string(str) + " - Uniform at location " + std::to_string(location) + "  not found!", MESSAGE_TYPE::M_ERROR);\
        return;\
    }else if ( uniter->second.size != usize ){\
        DEBUG_MESSAGE(std::string(str) + " - Uniform [" + uniter->second.name + "] at location " + std::to_string(location) + " with " +  std::to_string(uniter->second.size) + " arguments is accesed by a function with " + std::to_string(usize) + " arguments!", MESSAGE_TYPE::M_ERROR);\
    }

  #define get_attribute(atiter,location)\
    string name = enigma::shaderprograms[enigma::bound_shader]->name;\
    char str[128];\
    if (name == ""){\
        sprintf(str, "Program[%i]", enigma::bound_shader);\
    }else{\
        sprintf(str, "Program[%s = %i]", name.c_str(), enigma::bound_shader);\
    }\
    if (location < 0) { DEBUG_MESSAGE(std::string(str) + " - Attribute location < 0 given (" + std::to_string(location) + ")!", MESSAGE_TYPE::M_ERROR); return; }\
    std::unordered_map<GLint,enigma::Attribute>::iterator atiter = enigma::shaderprograms[enigma::bound_shader]->attributes.find(location);\
    if (atiter == enigma::shaderprograms[enigma::bound_shader]->attributes.end()){\
        DEBUG_MESSAGE(std::string(str) + " - Attribute at location " + std::to_string(location) + " not found!", MESSAGE_TYPE::M_ERROR);\
        return;\
    }

    #define get_program(ptiter,program,err)\
    if (program < 0) { DEBUG_MESSAGE("Program id [" + std::to_string(program) + "] < 0 given!", MESSAGE_TYPE::M_ERROR); return err; }\
    if (size_t(program) >= enigma::shaderprograms.size()) { DEBUG_MESSAGE("Program id [" + std::to_string(program) +"] > size() [" + std::to_string(enigma::shaderprograms.size()) +"] given!", MESSAGE_TYPE::M_ERROR); return err; }\
    if (enigma::shaderprograms[program] == nullptr) { DEBUG_MESSAGE("Program with id [" + std::to_string(program) + "] is deleted!", MESSAGE_TYPE::M_ERROR); return err; }\
    enigma::ShaderProgram* ptiter = enigma::shaderprograms[program];
#else
    #define get_uniform(uniter,location,usize)\
    if (location < 0) return; \
    std::unordered_map<GLint,enigma::Uniform>::iterator uniter = enigma::shaderprograms[enigma::bound_shader]->uniforms.find(location);\
    if (uniter == enigma::shaderprograms[enigma::bound_shader]->uniforms.end()){\
        return;\
    }

    #define get_attribute(atiter,location)\
    if (location < 0) return; \
    std::unordered_map<GLint,enigma::Attribute>::iterator atiter = enigma::shaderprograms[enigma::bound_shader]->attributes.find(location);\
    if (atiter == enigma::shaderprograms[enigma::bound_shader]->attributes.end()){\
        return;\
    }

    #define get_program(ptiter,program,err)\
    if (program < 0) { return err; }\
    if (size_t(program) >= enigma::shaderprograms.size()) { return err; }\
    if (enigma::shaderprograms[program] == nullptr) { return err; }\
    enigma::ShaderProgram* ptiter = enigma::shaderprograms[program];
#endif

namespace enigma
{
  std::vector<enigma::Shader*> shaders(0);
  std::vector<enigma::ShaderProgram*> shaderprograms(0);
 // std::vector<enigma::AttributeObject*> attributeobjects(0);

  extern unsigned default_shader;
  extern unsigned main_shader;
  extern unsigned bound_shader;
  extern unsigned bound_vbo;

string getVertexShaderPrefix() {
  return shader_header + "\
  #define MATRIX_VIEW                                    0\n\
  #define MATRIX_PROJECTION                              1\n\
  #define MATRIX_WORLD                                   2\n\
  #define MATRIX_WORLD_VIEW                              3\n\
  #define MATRIX_WORLD_VIEW_PROJECTION                   4\n\
  #define MATRICES_MAX                                   5\n\
  \
  uniform mat4 transform_matrix[MATRICES_MAX];\n\
  #define gm_Matrices transform_matrix\n\
  #define modelMatrix transform_matrix[MATRIX_WORLD]\n\
  #define modelViewMatrix transform_matrix[MATRIX_WORLD_VIEW]\n\
  #define projectionMatrix transform_matrix[MATRIX_PROJECTION]\n\
  #define viewMatrix transform_matrix[MATRIX_VIEW]\n\
  #define modelViewProjectionMatrix transform_matrix[MATRIX_WORLD_VIEW_PROJECTION]\n\
  #define in_Colour in_Color\n\
  \
  uniform mat3 normalMatrix;\n\
  #line 0";
}
  
string getFragmentShaderPrefix(){
  return shader_header + "\
  #define MATRIX_VIEW                                    0\n\
  #define MATRIX_PROJECTION                              1\n\
  #define MATRIX_WORLD                                   2\n\
  #define MATRIX_WORLD_VIEW                              3\n\
  #define MATRIX_WORLD_VIEW_PROJECTION                   4\n\
  #define MATRICES_MAX                                   5\n\
  \
  uniform mat4 transform_matrix[MATRICES_MAX];\n\
  #define gm_Matrices transform_matrix\n\
  #define modelMatrix transform_matrix[MATRIX_WORLD]\n\
  #define modelViewMatrix transform_matrix[MATRIX_WORLD_VIEW]\n\
  #define projectionMatrix transform_matrix[MATRIX_PROJECTION]\n\
  #define viewMatrix transform_matrix[MATRIX_VIEW]\n\
  #define modelViewProjectionMatrix transform_matrix[MATRIX_WORLD_VIEW_PROJECTION]\n\
  \
  uniform mat3 normalMatrix;\n\
  \
  uniform sampler2D en_TexSampler;\n\
  #define gm_BaseTexture en_TexSampler\n\
  uniform bool en_TexturingEnabled;\n\
  uniform bool en_ColorEnabled;\n\
  uniform bool en_AlphaTestEnabled;\n\
  \
  uniform float en_AlphaTestValue;\n\
  uniform vec4 en_BoundColor;\n\
  #line 0";
}
  
  string getDefaultVertexShader(){
    return  R"CODE(

in vec3 in_Position;   // (x,y,z)
in vec3 in_Normal;           // (x,y,z)
in vec4 in_Color;            // (r,g,b,a)
in vec2 in_TextureCoord;     // (u,v)

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

void main()
{
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
            )CODE";
  }
  string getDefaultFragmentShader(){
    return  R"CODE(

in vec2 v_TextureCoord;
in vec4 v_Color;
out vec4 out_FragColor;

void main()
{
  vec4 TexColor;
  if (en_TexturingEnabled == true){
    #if __VERSION__ > 120
        TexColor = texture( en_TexSampler, v_TextureCoord.st ) * v_Color;
    #else
        TexColor = texture2D( en_TexSampler, v_TextureCoord.st ) * v_Color;
    #endif
  }else{
    TexColor = v_Color;
  }
  if (en_AlphaTestEnabled == true){
    if (TexColor.a<=en_AlphaTestValue) discard;
  }
  out_FragColor = TexColor;
}
            )CODE";
  }
  void getUniforms(int prog_id){
    int uniform_count, max_length, uniform_count_arr = 0;
    glGetProgramiv(enigma::shaderprograms[prog_id]->shaderprogram, GL_ACTIVE_UNIFORMS, &uniform_count);
    glGetProgramiv(enigma::shaderprograms[prog_id]->shaderprogram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);
    for (int i = 0; i < uniform_count; ++i)
    {
      Uniform uniform;
      char uniformName[max_length];

      glGetActiveUniform(enigma::shaderprograms[prog_id]->shaderprogram, GLuint(i), max_length, NULL, &uniform.arraySize, &uniform.type, uniformName);

      uniform.name = uniformName;
      uniform.size = getGLTypeSize(uniform.type);
      uniform.data.resize(uniform.size);
      uniform.location = glGetUniformLocation(enigma::shaderprograms[prog_id]->shaderprogram, uniformName);
      enigma::shaderprograms[prog_id]->uniform_names[uniform.name] = uniform.location;
      enigma::shaderprograms[prog_id]->uniforms[uniform.location] = uniform;
      //printf("Program - %i - found uniform - %s - with size - %i\n", prog_id, uniform.name.c_str(), uniform.size);

      if (uniform.arraySize>1){ //It's an array
        string basename(uniform.name, 0, uniform.name.length()-3);
        //This should always work, because in case of an array glGetActiveUniform returns uniform_name[0] (so the first index)
        for (int c = 1; c < uniform.arraySize; ++c){
          Uniform suniform;
          suniform.arraySize = uniform.arraySize;
          suniform.size = uniform.size;
          suniform.data.resize(uniform.size);
          suniform.type = uniform.type;
          std::ostringstream oss;
          oss << basename << "[" << c << "]";
          suniform.name = oss.str();
          suniform.location = glGetUniformLocation(enigma::shaderprograms[prog_id]->shaderprogram, suniform.name.c_str());
          enigma::shaderprograms[prog_id]->uniform_names[suniform.name] = suniform.location;
          enigma::shaderprograms[prog_id]->uniforms[suniform.location] = suniform;
          //printf("Program - %i - found uniform - %s - with size - %i\n", prog_id, suniform.name.c_str(), suniform.size);
        }
        uniform_count_arr += uniform.arraySize;
      }
    }
    shaderprograms[prog_id]->uniform_count = uniform_count+uniform_count_arr;
  }
  void getAttributes(int prog_id){
    int attribute_count, max_length, attribute_count_arr = 0;
    glGetProgramiv(enigma::shaderprograms[prog_id]->shaderprogram, GL_ACTIVE_ATTRIBUTES, &attribute_count);
    glGetProgramiv(enigma::shaderprograms[prog_id]->shaderprogram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);
    for (int i = 0; i < attribute_count; ++i)
    {
      Attribute attribute;
      char attributeName[max_length];

      glGetActiveAttrib(enigma::shaderprograms[prog_id]->shaderprogram, GLuint(i), max_length, NULL, &attribute.arraySize, &attribute.type, attributeName);

      attribute.name = attributeName;
      attribute.size = getGLTypeSize(attribute.type);
      attribute.location = glGetAttribLocation(enigma::shaderprograms[prog_id]->shaderprogram, attributeName);
      attribute.enabled = false;
      attribute.vao = -1;
      glDisableVertexAttribArray(attribute.location);
      enigma::shaderprograms[prog_id]->attribute_names[attribute.name] = attribute.location;
      enigma::shaderprograms[prog_id]->attributes[attribute.location] = attribute;
      //printf("Program - %i - found attribute - %s - with size - %i\n", prog_id, attribute.name.c_str(), attribute.size);

      if (attribute.arraySize>1){ //It's an array
        string basename(attribute.name, 0, attribute.name.length()-3);
        //This should always work, because in case of an array glGetActiveAttrib returns attribute_name[0] (so the first index)
        for (int c = 1; c < attribute.arraySize; ++c){
          Attribute sattribute;
          sattribute.arraySize = attribute.arraySize;
          sattribute.size = attribute.size;
          sattribute.type = attribute.type;
          std::ostringstream oss;
          oss << basename << "[" << c << "]";
          sattribute.name = oss.str();
          sattribute.location = glGetAttribLocation(enigma::shaderprograms[prog_id]->shaderprogram, sattribute.name.c_str());
          enigma::shaderprograms[prog_id]->attribute_names[sattribute.name] = sattribute.location;
          enigma::shaderprograms[prog_id]->attributes[sattribute.location] = sattribute;
          //printf("Array: Program - %i - found attribute - %s - with size - %i\n", prog_id, sattribute.name.c_str(), sattribute.size);
        }
        attribute_count_arr += attribute.arraySize;
      }
    }
    shaderprograms[prog_id]->attribute_count = attribute_count+attribute_count_arr;
  }
  void getDefaultUniforms(int prog_id){
    shaderprograms[prog_id]->uni_viewMatrix = enigma_user::glsl_get_uniform_location(prog_id, "transform_matrix[0]");
    shaderprograms[prog_id]->uni_projectionMatrix = enigma_user::glsl_get_uniform_location(prog_id, "transform_matrix[1]");
    shaderprograms[prog_id]->uni_modelMatrix = enigma_user::glsl_get_uniform_location(prog_id, "transform_matrix[2]");
    shaderprograms[prog_id]->uni_mvMatrix = enigma_user::glsl_get_uniform_location(prog_id, "transform_matrix[3]");
    shaderprograms[prog_id]->uni_mvpMatrix = enigma_user::glsl_get_uniform_location(prog_id, "transform_matrix[4]");
    shaderprograms[prog_id]->uni_normalMatrix = enigma_user::glsl_get_uniform_location(prog_id, "normalMatrix");
    shaderprograms[prog_id]->uni_texSampler = enigma_user::glsl_get_uniform_location(prog_id, "en_TexSampler");

    shaderprograms[prog_id]->uni_textureEnable = enigma_user::glsl_get_uniform_location(prog_id, "en_TexturingEnabled");
    shaderprograms[prog_id]->uni_colorEnable = enigma_user::glsl_get_uniform_location(prog_id, "en_ColorEnabled");
    shaderprograms[prog_id]->uni_lightEnable = enigma_user::glsl_get_uniform_location(prog_id, "en_LightingEnabled");
    shaderprograms[prog_id]->uni_alphaTestEnable = enigma_user::glsl_get_uniform_location(prog_id, "en_AlphaTestEnabled");

    shaderprograms[prog_id]->uni_alphaTest = enigma_user::glsl_get_uniform_location(prog_id, "en_AlphaTestValue");
    shaderprograms[prog_id]->uni_color = enigma_user::glsl_get_uniform_location(prog_id, "en_BoundColor");
    shaderprograms[prog_id]->uni_ambient_color = enigma_user::glsl_get_uniform_location(prog_id, "en_AmbientColor");
    shaderprograms[prog_id]->uni_lights_active = enigma_user::glsl_get_uniform_location(prog_id, "en_ActiveLights");

    char tchars[64];
    for (unsigned int i=0; i<8; ++i){
      sprintf(tchars, "Light[%d].Position", i);
      shaderprograms[prog_id]->uni_light_position[i] = enigma_user::glsl_get_uniform_location(prog_id, tchars);
      sprintf(tchars, "Light[%d].La", i);
      shaderprograms[prog_id]->uni_light_ambient[i] = enigma_user::glsl_get_uniform_location(prog_id, tchars);
      sprintf(tchars, "Light[%d].Ld", i);
      shaderprograms[prog_id]->uni_light_diffuse[i] = enigma_user::glsl_get_uniform_location(prog_id, tchars);
      sprintf(tchars, "Light[%d].Ls", i);
      shaderprograms[prog_id]->uni_light_specular[i] = enigma_user::glsl_get_uniform_location(prog_id, tchars);
      sprintf(tchars, "Light[%d].cA", i);
      shaderprograms[prog_id]->uni_light_cAttenuation[i] = enigma_user::glsl_get_uniform_location(prog_id, tchars);
      sprintf(tchars, "Light[%d].lA", i);
      shaderprograms[prog_id]->uni_light_lAttenuation[i] = enigma_user::glsl_get_uniform_location(prog_id, tchars);
      sprintf(tchars, "Light[%d].qA", i);
      shaderprograms[prog_id]->uni_light_qAttenuation[i] = enigma_user::glsl_get_uniform_location(prog_id, tchars);
    }
    shaderprograms[prog_id]->uni_material_ambient = enigma_user::glsl_get_uniform_location(prog_id, "Material.Ka");
    shaderprograms[prog_id]->uni_material_diffuse = enigma_user::glsl_get_uniform_location(prog_id, "Material.Kd");
    shaderprograms[prog_id]->uni_material_specular = enigma_user::glsl_get_uniform_location(prog_id, "Material.Ks");
    shaderprograms[prog_id]->uni_material_shininess = enigma_user::glsl_get_uniform_location(prog_id, "Material.Shininess");
  }
  void getDefaultAttributes(int prog_id){
    shaderprograms[prog_id]->att_vertex = enigma_user::glsl_get_attribute_location(prog_id, "in_Position");
    shaderprograms[prog_id]->att_color = enigma_user::glsl_get_attribute_location(prog_id, "in_Color");
    shaderprograms[prog_id]->att_texture = enigma_user::glsl_get_attribute_location(prog_id, "in_TextureCoord");
    shaderprograms[prog_id]->att_normal = enigma_user::glsl_get_attribute_location(prog_id, "in_Normal");
  }

  //This seems very stupid for me, but I don't know any more "elegant" way - Harijs G.
  bool UATypeUIComp(UAType i, unsigned int j){
    return (i.ui == j);
  }
  bool UATypeIComp(UAType i, int j){
    return (i.i == j);
  }
  bool UATypeFComp(UAType i, float j){
    return (i.f == j);
  }
}

unsigned long getFileLength(std::ifstream& file)
{
  if(!file.good()) return 0;

  //unsigned long pos=file.tellg();
  file.seekg(0,std::ios::end);
  unsigned long len = file.tellg();
  file.seekg(std::ios::beg);

  return len;
}

namespace enigma_user
{

std::string glsl_shader_get_infolog(int id)
{
  GLint blen = 0;
  GLsizei slen = 0;
  glGetShaderiv(enigma::shaders[id]->shader, GL_INFO_LOG_LENGTH , &blen);
  if (blen > 1)
  {
    GLchar* compiler_log = (GLchar*)malloc(blen);
    glGetShaderInfoLog(enigma::shaders[id]->shader, blen, &slen, compiler_log);
    enigma::shaders[id]->log = (string)compiler_log;
    free(compiler_log);
  } else {
    enigma::shaders[id]->log = "Shader log empty";
  }
  return enigma::shaders[id]->log;
}

std::string glsl_program_get_infolog(int id)
{
  GLint blen = 0;
  GLsizei slen = 0;
  glGetProgramiv(enigma::shaderprograms[id]->shaderprogram, GL_INFO_LOG_LENGTH , &blen);

  if (blen > 1)
  {
    GLchar* compiler_log = (GLchar*)malloc(blen);
    glGetProgramInfoLog(enigma::shaderprograms[id]->shaderprogram, blen, &slen, compiler_log);
    enigma::shaderprograms[id]->log = (string)compiler_log;
    free(compiler_log);
  } else {
    enigma::shaderprograms[id]->log = "Shader program log empty";
  }
  
  return enigma::shaderprograms[id]->log;
}

int glsl_shader_create(int type)
{
  unsigned int id = enigma::shaders.size();
  enigma::shaders.push_back(new enigma::Shader(type));
  return id;
}

bool glsl_shader_load(int id, string fname)
{
  std::ifstream ifs(fname.c_str());
  std::string shaderSource((std::istreambuf_iterator<char>(ifs) ),
                     (std::istreambuf_iterator<char>()    ));
  string source;
  if (enigma::shaders[id]->type == sh_vertex) source = enigma::getVertexShaderPrefix() + shaderSource;
  if (enigma::shaders[id]->type == sh_fragment) source = enigma::getFragmentShaderPrefix() + shaderSource;

  const char *ShaderSource = source.c_str();
  glShaderSource(enigma::shaders[id]->shader, 1, &ShaderSource, NULL);
  return true; // No Error
}

bool glsl_shader_load_string(int id, string shaderSource)
{
  string source;
  if (enigma::shaders[id]->type == sh_vertex) source = enigma::getVertexShaderPrefix() + shaderSource;
  if (enigma::shaders[id]->type == sh_fragment) source = enigma::getFragmentShaderPrefix() + shaderSource;

  const char *ShaderSource = source.c_str();
  glShaderSource(enigma::shaders[id]->shader, 1, &ShaderSource, NULL);
  return true; // No Error
}

bool glsl_shader_compile(int id)
{
  glCompileShader(enigma::shaders[id]->shader);

  GLint compiled;
  glGetShaderiv(enigma::shaders[id]->shader, GL_COMPILE_STATUS, &compiled);
  if (compiled){
    return true;
  } else {
    DEBUG_MESSAGE("Shader[" + std::to_string(id) + "] " + 
      (enigma::shaders[id]->type == sh_vertex?"Vertex shader":"Pixel shader") + " - Compilation failed - Info log: " + 
      glsl_shader_get_infolog(id), MESSAGE_TYPE::M_ERROR);
    return false;
  }
}

bool glsl_shader_get_compiled(int id) {
  GLint compiled;
  glGetShaderiv(enigma::shaders[id]->shader, GL_COMPILE_STATUS, &compiled);
  if (compiled){
    return true;
  } else {
    return false;
  }
}

void glsl_shader_free(int id)
{
  delete enigma::shaders[id];
}

int glsl_program_create()
{
  unsigned int id = enigma::shaderprograms.size();
  enigma::shaderprograms.push_back(new enigma::ShaderProgram());
  return id;
}

bool glsl_program_link(int id)
{
  glLinkProgram(enigma::shaderprograms[id]->shaderprogram);
  GLint linked;
  glGetProgramiv(enigma::shaderprograms[id]->shaderprogram, GL_LINK_STATUS, &linked);

	if (linked){
    enigma::getUniforms(id);
    enigma::getAttributes(id);
    enigma::getDefaultUniforms(id);
    enigma::getDefaultAttributes(id);
    return true;
  } else {
    DEBUG_MESSAGE("Shader program[" + std::to_string(id) + "] - Linking failed - Info log: " + 
      glsl_program_get_infolog(id), MESSAGE_TYPE::M_ERROR);
    return false;
  }
}

bool glsl_program_validate(int id)
{
  glValidateProgram(enigma::shaderprograms[id]->shaderprogram);
  GLint validated;
  glGetProgramiv(enigma::shaderprograms[id]->shaderprogram, GL_VALIDATE_STATUS, &validated);

  if (validated){
    return true;
  } else {
    DEBUG_MESSAGE("Shader program[" + std::to_string(id) + "] - Validation failed - Info log: " + 
      glsl_program_get_infolog(id), MESSAGE_TYPE::M_ERROR);
    return false;
  }
}

void glsl_program_attach(int id, int sid)
{
  glAttachShader(enigma::shaderprograms[id]->shaderprogram, enigma::shaders[sid]->shader);
}

void glsl_program_detach(int id, int sid)
{
  glDetachShader(enigma::shaderprograms[id]->shaderprogram, enigma::shaders[sid]->shader);
}

void glsl_program_set(int id)
{
  if (enigma::bound_shader != unsigned(id)) {
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    enigma::bound_shader = id;
    glUseProgram(enigma::shaderprograms[id]->shaderprogram);
  }
}

int glsl_program_get()
{
  return enigma::bound_shader;
}

void glsl_program_reset()
{
    //if (enigma::bound_shader != enigma::main_shader){ //This doesn't work because enigma::bound_shader is the same as enigma::main_shader at start
        enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
        enigma::bound_shader = enigma::main_shader;
        glUseProgram(enigma::shaderprograms[enigma::main_shader]->shaderprogram);
    //}
}

void glsl_program_default_set(int id){
  enigma::main_shader = id;
}

void glsl_program_default_reset(){
  enigma::main_shader = enigma::default_shader;
}

void glsl_program_free(int id)
{
  delete enigma::shaderprograms[id];
  enigma::shaderprograms[id] = nullptr;
}

void glsl_program_set_name(int id, string name){
  enigma::shaderprograms[id]->name = name;
}

int glsl_get_uniform_location(int program, string name) {
	//int uni = glGetUniformLocation(enigma::shaderprograms[program]->shaderprogram, name.c_str());
	std::unordered_map<string,GLint>::iterator it = enigma::shaderprograms[program]->uniform_names.find(name);
  if (it == enigma::shaderprograms[program]->uniform_names.end()){
    #ifdef DEBUG_MODE
      if (enigma::shaderprograms[program]->name == ""){
        DEBUG_MESSAGE("Program[" + std::to_string(program) + "] - Uniform " + name + " not found!", MESSAGE_TYPE::M_ERROR);
      }else{
       DEBUG_MESSAGE("Program[" + enigma::shaderprograms[program]->name + " = " + std::to_string(program) + "] - Uniform " + name + " not found!", MESSAGE_TYPE::M_ERROR);
      }
    #endif
    return -1;
  }else{
    return it->second;
  }
}

void glsl_uniformf(int location, float v0) {
  get_uniform(it,location,1);
  if (it->second.data[0].f != v0){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform1f(location, v0);
    it->second.data[0].f = v0;
  }
}

void glsl_uniformf(int location, float v0, float v1) {
  get_uniform(it,location,2);
  if (it->second.data[0].f != v0 || it->second.data[1].f != v1){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform2f(location, v0, v1);
    it->second.data[0].f = v0, it->second.data[1].f = v1;
  }
}

void glsl_uniformf(int location, float v0, float v1, float v2) {
  get_uniform(it,location,3);
  if (it->second.data[0].f != v0 || it->second.data[1].f != v1 || it->second.data[2].f != v2){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform3f(location, v0, v1, v2);
    it->second.data[0].f = v0, it->second.data[1].f = v1, it->second.data[2].f = v2;
	}
}

void glsl_uniformf(int location, float v0, float v1, float v2, float v3) {
  get_uniform(it,location,4);
  if (it->second.data[0].f != v0 || it->second.data[1].f != v1 || it->second.data[2].f != v2 || it->second.data[3].f != v3){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform4f(location, v0, v1, v2, v3);
    it->second.data[0].f = v0, it->second.data[1].f = v1, it->second.data[2].f = v2, it->second.data[3].f = v3;
	}
}

void glsl_uniformi(int location, int v0) {
  get_uniform(it,location,1);
  if (it->second.data[0].i != v0){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform1i(location, v0);
    it->second.data[0].i = v0;
  }
}

void glsl_uniformi(int location, int v0, int v1) {
  get_uniform(it,location,2);
  if (it->second.data[0].i != v0 || it->second.data[1].i != v1){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform2i(location, v0, v1);
    it->second.data[0].i = v0, it->second.data[1].i = v1;
  }
}

void glsl_uniformi(int location, int v0, int v1, int v2) {
  get_uniform(it,location,3);
  if (it->second.data[0].i != v0 || it->second.data[1].i != v1 || it->second.data[2].i != v2){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform3i(location, v0, v1, v2);
    it->second.data[0].i = v0, it->second.data[1].i = v1, it->second.data[2].i = v2;
  }
}

void glsl_uniformi(int location, int v0, int v1, int v2, int v3) {
  get_uniform(it,location,4);
  if (it->second.data[0].i != v0 || it->second.data[1].i != v1 || it->second.data[2].i != v2 || it->second.data[3].i != v3){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform4i(location, v0, v1, v2, v3);
    it->second.data[0].i = v0, it->second.data[1].i = v1, it->second.data[2].i = v2, it->second.data[3].i = v3;
  }
}

void glsl_uniformui(int location, unsigned v0) {
  get_uniform(it,location,1);
  if (it->second.data[0].ui != v0){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform1ui(location, v0);
    it->second.data[0].ui = v0;
  }
}

void glsl_uniformui(int location, unsigned v0, unsigned v1) {
  get_uniform(it,location,2);
  if (it->second.data[0].ui != v0 || it->second.data[1].ui != v1){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform2ui(location, v0, v1);
    it->second.data[0].ui = v0, it->second.data[1].ui = v1;
  }
}

void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2) {
  get_uniform(it,location,3);
  if (it->second.data[0].ui != v0 || it->second.data[1].ui != v1 || it->second.data[2].ui != v2){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform3ui(location, v0, v1, v2);
    it->second.data[0].ui = v0, it->second.data[1].ui = v1, it->second.data[2].ui = v2;
  }
}

void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2, unsigned v3) {
  get_uniform(it,location,4);
  if (it->second.data[0].ui != v0 || it->second.data[1].ui != v1 || it->second.data[2].ui != v2 || it->second.data[3].ui != v3){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform4ui(location, v0, v1, v2, v3);
    it->second.data[0].ui = v0, it->second.data[1].ui = v1, it->second.data[2].ui = v2, it->second.data[3].ui = v3;
  }
}

////////////////////////VECTOR FUNCTIONS FOR FLOAT UNIFORMS/////////////////
void glsl_uniform1fv(int location, int size, const float *value){
  get_uniform(it,location,1);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeFComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform1fv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].f = value[i];
    }
  }
}

void glsl_uniform2fv(int location, int size, const float *value){
  get_uniform(it,location,2);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeFComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform2fv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].f = value[i];
    }
  }
}

void glsl_uniform3fv(int location, int size, const float *value){
  get_uniform(it,location,3);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeFComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform3fv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].f = value[i];
    }
  }
}

void glsl_uniform4fv(int location, int size, const float *value){
  get_uniform(it,location,4);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeFComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform4fv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].f = value[i];
    }
  }
}

////////////////////////VECTOR FUNCTIONS FOR INT UNIFORMS/////////////////
void glsl_uniform1iv(int location, int size, const int *value){
  get_uniform(it,location,1);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform1iv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].i = value[i];
    }
  }
}

void glsl_uniform2iv(int location, int size, const int *value){
  get_uniform(it,location,2);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform2iv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].i = value[i];
    }
  }
}

void glsl_uniform3iv(int location, int size, const int *value){
  get_uniform(it,location,3);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform3iv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].i = value[i];
    }
  }
}

void glsl_uniform4iv(int location, int size, const int *value){
  get_uniform(it,location,4);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform4iv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].i = value[i];
    }
  }
}

////////////////////////VECTOR FUNCTIONS FOR UNSIGNED INT UNIFORMS/////////////////
void glsl_uniform1uiv(int location, int size, const unsigned int *value){
  get_uniform(it,location,1);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeUIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform1uiv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].ui = value[i];
    }
  }
}

void glsl_uniform2uiv(int location, int size, const unsigned int *value){
  get_uniform(it,location,2);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeUIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform2uiv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].ui = value[i];
    }
  }
}

void glsl_uniform3uiv(int location, int size, const unsigned int *value){
  get_uniform(it,location,3);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeUIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform3uiv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].ui = value[i];
    }
  }
}

void glsl_uniform4uiv(int location, int size, const unsigned int *value){
  get_uniform(it,location,4);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeUIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform4uiv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].ui = value[i];
    }
  }
}

////////////////////////MATRIX FUNCTIONS FOR FLOAT UNIFORMS/////////////////
void glsl_uniform_matrix2fv(int location, int size, const float *matrix){
  get_uniform(it,location,4);
  if (std::equal(it->second.data.begin(), it->second.data.end(), matrix, enigma::UATypeFComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniformMatrix2fv(location, size, true, matrix);
    memcpy(&it->second.data[0], &matrix[0], it->second.data.size() * sizeof(enigma::UAType));
  }
}

void glsl_uniform_matrix3fv(int location, int size, const float *matrix){
  get_uniform(it,location,9);
  if (std::equal(it->second.data.begin(), it->second.data.end(), matrix, enigma::UATypeFComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniformMatrix3fv(location, size, true, matrix);
    memcpy(&it->second.data[0], &matrix[0], it->second.data.size() * sizeof(enigma::UAType));
  }
}

void glsl_uniform_matrix4fv(int location, int size, const float *matrix){
  get_uniform(it,location,16);
  if (std::equal(it->second.data.begin(), it->second.data.end(), matrix, enigma::UATypeFComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniformMatrix4fv(location, size, true, matrix);
    memcpy(&it->second.data[0], &matrix[0], it->second.data.size() * sizeof(enigma::UAType));
  }
}

//Attributes
int glsl_get_attribute_location(int program, string name) {
  get_program(prog, program, -1);
  std::unordered_map<string,GLint>::iterator it = prog->attribute_names.find(name);
  if (it == prog->attribute_names.end()){
    #ifdef DEBUG_MODE
      if (prog->name == ""){
        DEBUG_MESSAGE("Program[" + std::to_string(program) + "] - Attribute " + name + "not found!", MESSAGE_TYPE::M_ERROR);
      }else{
        DEBUG_MESSAGE("Program[" + prog->name + " =  " + std::to_string(program) + "] - Attribute " + name + " not found!", MESSAGE_TYPE::M_ERROR);
      }
    #endif
    return -1;
  }else{
    return it->second;
  }
}

void glsl_attribute_enable_all(bool enable){
  for ( auto &it : enigma::shaderprograms[enigma::bound_shader]->attributes ){
    if (enable != it.second.enabled){
      enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
      if (enable == true){
        glEnableVertexAttribArray( it.second.location );
      }else{
        glDisableVertexAttribArray( it.second.location );
      }
      it.second.enabled = enable;
    }
  }
}

void glsl_attribute_enable(int location, bool enable){
  get_attribute(it,location);
  if (enable != it->second.enabled){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    if (enable == true){
      glEnableVertexAttribArray(location);
    }else{
      glDisableVertexAttribArray(location);
    }
    it->second.enabled = enable;
  }
}

void glsl_attribute_set(int location, int size, int type, bool normalize, int stride, unsigned offset){
  get_attribute(it,location);
  //if (/*it->second.enabled == true*/ (it->second.vao != enigma::bound_vbo || it->second.datatype != type || it->second.datasize != size || it->second.normalize != normalize || it->second.stride != stride || it->second.offset != offset)){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glVertexAttribPointer(location, size, type, normalize, stride, (const GLvoid*)(intptr_t)offset);
    it->second.datatype = type;
    it->second.datasize = size;
    it->second.normalize = normalize;
    it->second.stride = stride;
    it->second.offset = offset;
    it->second.vao = enigma::bound_vbo;
  //}
}

}

//Internal functions - they do less error checking and don't call batch flush to stop recursion in GL3ModelStruct::Draw
namespace enigma
{
  void glsl_uniform_matrix3fv_internal(int location, int size, const float *matrix){
    get_uniform(it,location,9);
    if (std::equal(it->second.data.begin(), it->second.data.end(), matrix, enigma::UATypeFComp) == false){
      glUniformMatrix3fv(location, size, true, matrix);
      memcpy(&it->second.data[0], &matrix[0], it->second.data.size() * sizeof(enigma::UAType));
    }
  }

  void glsl_uniform_matrix4fv_internal(int location, int size, const float *matrix){
    get_uniform(it,location,16);
    if (std::equal(it->second.data.begin(), it->second.data.end(), matrix, enigma::UATypeFComp) == false){
      glUniformMatrix4fv(location, size, true, matrix);
      memcpy(&it->second.data[0], &matrix[0], it->second.data.size() * sizeof(enigma::UAType));
    }
  }

  void glsl_uniformi_internal(int location, int v0) {
    get_uniform(it,location,1);
    if (it->second.data[0].i != v0){
      glUniform1i(location, v0);
      it->second.data[0].i = v0;
    }
  }

  void glsl_uniformf_internal(int location, float v0, float v1, float v2, float v3) {
    get_uniform(it,location,4);
    if (it->second.data[0].f != v0 || it->second.data[1].f != v1 || it->second.data[2].f != v2 || it->second.data[3].f != v3){
      glUniform4f(location, v0, v1, v2, v3);
      it->second.data[0].f = v0, it->second.data[1].f = v1, it->second.data[2].f = v2, it->second.data[3].f = v3;
    }
  }

  void glsl_attribute_enable_all_internal(bool enable){
    for ( auto &it : enigma::shaderprograms[enigma::bound_shader]->attributes ){
      if (enable != it.second.enabled){
        if (enable == true){
          glEnableVertexAttribArray( it.second.location );
        }else{
          glDisableVertexAttribArray( it.second.location );
        }
        it.second.enabled = enable;
      }
    }
  }

  void glsl_attribute_enable_internal(int location, bool enable){
    get_attribute(it,location);
    if (enable != it->second.enabled){
      if (enable == true){
        glEnableVertexAttribArray(location);
      }else{
        glDisableVertexAttribArray(location);
      }
      it->second.enabled = enable;
    }
  }

  void glsl_attribute_set_internal(int location, int size, int type, bool normalize, int stride, unsigned offset){
    get_attribute(it,location);
    //if (/*it->second.enabled == true*/ (it->second.vao != enigma::bound_vbo || it->second.datatype != type || it->second.datasize != size || it->second.normalize != normalize || it->second.stride != stride || it->second.offset != offset)){
      glVertexAttribPointer(location, size, type, normalize, stride, (const GLvoid*)(intptr_t)offset);
      it->second.datatype = type;
      it->second.datasize = size;
      it->second.normalize = normalize;
      it->second.stride = stride;
      it->second.offset = offset;
      it->second.vao = enigma::bound_vbo;
    //}
  }
}
