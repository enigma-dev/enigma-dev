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

#include "../General/OpenGLHeaders.h"
#include "../General/GStextures.h"
#include "GL3shader.h"
#include "GLSLshader.h"
#include <math.h>

#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */

#include <iostream>
#include <fstream>
using namespace std;

#include <vector>
using std::vector;

extern GLenum shadertypes[5] = {
  GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER
};

vector<enigma::Shader*> shaders(0);
vector<enigma::ShaderProgram*> shaderprograms(0);

namespace enigma
{
    extern ShaderProgram* default_shader;
    string getVertexShaderPrefix(){
        return "#version 140\n"
                "#define MATRIX_VIEW                                    0\n"
                "#define MATRIX_PROJECTION                              1\n"
                "#define MATRIX_WORLD                                   2\n"
                "#define MATRIX_WORLD_VIEW                              3\n"
                "#define MATRIX_WORLD_VIEW_PROJECTION                   4\n"
                "#define MATRICES_MAX                                   5\n"

                "uniform mat4 transform_matrix[MATRICES_MAX];           \n"
                "#define gm_Matrices transform_matrix \n"
                "#define modelMatrix transform_matrix[MATRIX_WORLD] \n"
                "#define modelViewMatrix transform_matrix[MATRIX_WORLD_VIEW] \n"
                "#define projectionMatrix transform_matrix[MATRIX_PROJECTION] \n"
                "#define viewMatrix transform_matrix[MATRIX_VIEW] \n"
                "#define modelViewProjectionMatrix transform_matrix[MATRIX_WORLD_VIEW_PROJECTION] \n"

                "uniform mat3 normalMatrix;     \n"
                "uniform bool en_LightingEnabled;\n"
                "uniform bool en_VS_FogEnabled;\n"
                "uniform float en_FogStart;\n"
                "uniform float en_RcpFogRange;\n"

                "#define MAX_LIGHTS   8\n"

                "uniform vec4   en_AmbientColor;                              // rgba=color\n"
                //"uniform vec4   en_Lights_Direction[MAX_LIGHTS];              // normalised direction\n"
                //"uniform vec4   en_Lights_PositionRange[MAX_LIGHTS];          // X,Y,Z position,  W range\n"
                //"uniform vec4   en_Lights_Color[MAX_LIGHTS];                  // rgba=color\n"

                "in vec3 in_Position;                 // (x,y,z)\n"
                "in vec3 in_Normal;                  // (x,y,z)\n"
                "in vec4 in_Color;                    // (r,g,b,a)\n"
                "in vec2 in_TextureCoord;             // (u,v)\n";
    }
    string getFragmentShaderPrefix(){
        return "#version 140\n"
                "uniform sampler2D TexSampler;\n"
                "uniform bool en_TexturingEnabled;\n"
                "uniform bool en_ColorEnabled;\n"
                "uniform vec4 en_bound_color;\n";
    }
    string getDefaultVertexShader(){
        return  "out vec2 v_TextureCoord;\n"
                "out vec4 v_Color;\n"
                "out vec3 v_lightIntensity;\n"

                "struct LightInfo {\n"
                  "vec4 Position; // Light position in eye coords\n"
                  "vec4 La; // Ambient light intensity\n"
                  "vec4 Ld; // Diffuse light intensity\n"
                  "vec4 Ls; // Specular light intensity\n"
                  //"bool type; //Type - directional or point"
                "};\n"
                "uniform LightInfo Light[MAX_LIGHTS];\n"

                "struct MaterialInfo {"
                    "vec4 Ka; // Ambient reflectivity\n"
                    "vec4 Kd; // Diffuse reflectivity\n"
                    "vec4 Ks; // Specular reflectivity\n"
                    "float Shininess; // Specular shininess factor\n"
                "};\n"
                "uniform MaterialInfo Material;\n"

                "void getEyeSpace( out vec3 norm, out vec4 position )\n"
                "{\n"
                    "norm = normalize( normalMatrix * in_Normal );\n"
                    "position = modelViewMatrix * vec4(in_Position,1.0);\n"
                "}\n"

                "vec4 phongModel( vec4 position, vec3 norm, bool type )\n"
                "{\n"
                  "vec3 s;\n"
                  "if (type == true){ //Directional light\n"
                    "s = Light[0].Position.xyz;\n"
                  "}else{ //Point light\n"
                    "s = normalize(vec3((Light[0].Position - position).xyz));\n"
                  "}\n"
                  "vec3 v = normalize(-position.xyz);\n"
                  "vec3 r = reflect( -s, norm );\n"
                  "vec4 ambient = Light[0].La * Material.Ka;\n"
                  "float sDotN = max( dot(s,norm), 0.0 );\n"
                  "vec4 diffuse = Light[0].Ld * Material.Kd * sDotN;\n"
                  "vec4 spec = vec4(0.0);\n"
                  "if( sDotN > 0.0 )\n"
                      "spec = Light[0].Ls * Material.Ks * pow( max( dot(r,v), 0.0 ), Material.Shininess );\n"
                  "return ambient + diffuse + spec;\n"
                "}\n"

                "void main()\n"
                "{\n"
                    "if (en_LightingEnabled == true){\n"
                        "vec3 eyeNorm;\n"
                        "vec4 eyePosition;\n"
                        "getEyeSpace(eyeNorm, eyePosition);\n"
                        "v_Color = en_AmbientColor + phongModel( eyePosition, eyeNorm, false ) * in_Color;\n"
                    "}else{\n"
                        "v_Color = in_Color;\n"
                    "}\n"
                    "gl_Position = modelViewProjectionMatrix * vec4( in_Position.xyz, 1.0);\n"

                    "v_TextureCoord = in_TextureCoord;\n"
                "}\n";
    }
    string getDefaultFragmentShader(){
        return  "in vec2 v_TextureCoord;\n"
                "in vec4 v_Color;\n"
                "out vec4 out_FragColor;\n"

                "void main()\n"
                "{\n"
                    //"vec3 normal = normalize(v_Normal);\n"
                    //"vec4 LightColor = CalculateLighting(normal);\n"
                    "vec4 TexColor;"
                    "if (en_TexturingEnabled == true && en_ColorEnabled == true){\n"
                        "TexColor = texture2D( TexSampler, v_TextureCoord.st ) * v_Color;\n"
                    "}else if (en_ColorEnabled == true){\n"
                        "TexColor = v_Color;\n"
                    "}else if (en_TexturingEnabled == true){\n"
                        "TexColor = texture2D( TexSampler, v_TextureCoord.st );\n"
                    "}else{\n"
                        "TexColor = en_bound_color;\n"
                    "}\n"
                    "out_FragColor = TexColor;"// * LightColor;"
                "}\n";
    }
}

unsigned long getFileLength(ifstream& file)
{
    if(!file.good()) return 0;

    //unsigned long pos=file.tellg();
    file.seekg(0,ios::end);
    unsigned long len = file.tellg();
    file.seekg(ios::beg);

    return len;
}

namespace enigma_user
{

int glsl_shader_create(int type)
{
  unsigned int id = shaders.size();
  shaders.push_back(new enigma::Shader(type));
  return id;
}

int glsl_shader_load(int id, string fname)
{
  ifstream file;
  file.open(fname.c_str(), ios::in); // opens as ASCII
  if (!file.is_open()) return 1; // Error: File could not be oppenned

  unsigned long len = getFileLength(file);
  if (len == 0) return 2;   // Error: Empty File

  GLchar* ShaderSource;
  ShaderSource = (GLchar*) new char[len+1];

  ShaderSource[len] = 0;

  unsigned int i=0;
  while (file.good())
  {
    ShaderSource[i] = file.get();       // get character from file.
    if (!file.eof())
    i++;
  }

  ShaderSource[i] = 0;  // 0-terminate it at the correct position

  glShaderSource(shaders[id]->shader, 1, (const GLchar**)&ShaderSource, NULL);

  file.close();
  return 3; // No Error
}

bool glsl_shader_compile(int id)
{
  glCompileShader(shaders[id]->shader);

  GLint blen = 0;
  GLsizei slen = 0;

  glGetShaderiv(shaders[id]->shader, GL_INFO_LOG_LENGTH , &blen);

  if (blen > 1)
  {
    GLchar* compiler_log = (GLchar*)malloc(blen);

    glGetInfoLogARB(shaders[id]->shader, blen, &slen, compiler_log);
    shaders[id]->log = (string)compiler_log;
  } else {
    shaders[id]->log = "Shader compile log empty";
  }

  GLint compiled;
  glGetProgramiv(shaders[id]->shader, GL_COMPILE_STATUS, &compiled);
  if (compiled)
  {
    return true;
  } else {
    return false;
  }
}

bool glsl_shader_get_compiled(int id) {
  GLint compiled;
  glGetProgramiv(shaders[id]->shader, GL_COMPILE_STATUS, &compiled);
  if (compiled)
  {
    return true;
  } else {
    return false;
  }
}

string glsl_shader_get_infolog(int id)
{
  return shaders[id]->log;
}

void glsl_shader_free(int id)
{
  delete shaders[id];
}

int glsl_program_create()
{
  unsigned int id = shaderprograms.size();
  shaderprograms.push_back(new enigma::ShaderProgram());
  return id;
}

bool glsl_program_link(int id)
{
//glBindFragDataLocation(shaderprograms[id]->shaderprogram, 0, "fragColor");

  glLinkProgram(shaderprograms[id]->shaderprogram);
  GLint linked;
  glGetProgramiv(shaderprograms[id]->shaderprogram, GL_LINK_STATUS, &linked);
  if (linked)
  {
    return true;
  } else {
    return false;
  }
}

bool glsl_program_validate(int id)
{
  glValidateProgram(shaderprograms[id]->shaderprogram);
  GLint validated;
  glGetProgramiv(shaderprograms[id]->shaderprogram, GL_VALIDATE_STATUS, &validated);
  if (validated)
  {
    return true;
  } else {
    return false;
  }
}

void glsl_program_attach(int id, int sid)
{
  glAttachShader(shaderprograms[id]->shaderprogram, shaders[sid]->shader);
}

void glsl_program_detach(int id, int sid)
{
  glDetachShader(shaderprograms[id]->shaderprogram, shaders[sid]->shader);
}

void glsl_program_set(int id)
{
  glUseProgram(shaderprograms[id]->shaderprogram);
}

void glsl_program_reset()
{
  //NOTE: Texture must be reset first so the Global VBO can draw and let people use shaders on text.
  texture_reset();
  glUseProgram(enigma::default_shader->shaderprogram);
}

void glsl_program_free(int id)
{
  delete shaderprograms[id];
}

int glsl_get_uniform_location(int program, string name) {
	int uni = glGetUniformLocation(shaderprograms[program]->shaderprogram, name.c_str());
    if (uni == -1){
        printf("Error: Uniform %s not found!\n", name.c_str());
    }
    return uni;
}

void glsl_uniformf(int location, float v0) {
	glUniform1f(location, v0);
}

void glsl_uniformf(int location, float v0, float v1) {
	glUniform2f(location, v0, v1);
}

void glsl_uniformf(int location, float v0, float v1, float v2) {
	glUniform3f(location, v0, v1, v2);
}

void glsl_uniformf(int location, float v0, float v1, float v2, float v3) {
	glUniform4f(location, v0, v1, v2, v3);
}

void glsl_uniformi(int location, int v0) {
	glUniform1i(location, v0);
}

void glsl_uniformi(int location, int v0, int v1) {
	glUniform2i(location, v0, v1);
}

void glsl_uniformi(int location, int v0, int v1, int v2) {
	glUniform3i(location, v0, v1, v2);
}

void glsl_uniformi(int location, int v0, int v1, int v2, int v3) {
	glUniform4i(location, v0, v1, v2, v3);
}

void glsl_uniformui(int location, unsigned v0) {
	glUniform1ui(location, v0);
}

void glsl_uniformui(int location, unsigned v0, unsigned v1) {
	glUniform2ui(location, v0, v1);
}

void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2) {
	glUniform3ui(location, v0, v1, v2);
}

void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2, unsigned v3) {
	glUniform4ui(location, v0, v1, v2, v3);
}

}

