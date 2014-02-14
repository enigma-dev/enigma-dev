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

namespace enigma
{
    vector<enigma::Shader*> shaders(0);
    vector<enigma::ShaderProgram*> shaderprograms(0);

    extern unsigned default_shader;
    extern unsigned bound_shader;
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

                "#define in_Colour in_Color \n"

                "uniform mat3 normalMatrix;     \n"
                "uniform bool en_LightingEnabled;\n"
                "uniform bool en_VS_FogEnabled;\n"
                "uniform float en_FogStart;\n"
                "uniform float en_RcpFogRange;\n"

                "#define MAX_LIGHTS   8\n"

                "uniform vec4   en_AmbientColor;                              // rgba=color\n";
                //"uniform vec4   en_Lights_Direction[MAX_LIGHTS];              // normalised direction\n"
                //"uniform vec4   en_Lights_PositionRange[MAX_LIGHTS];          // X,Y,Z position,  W range\n"
                //"uniform vec4   en_Lights_Color[MAX_LIGHTS];                  // rgba=color\n"
    }
    string getFragmentShaderPrefix(){
        return "#version 140\n"
                "uniform sampler2D en_TexSampler;\n"
                "#define gm_BaseTexture en_TexSampler\n"
                "uniform bool en_TexturingEnabled;\n"
                "uniform bool en_ColorEnabled;\n"
                "uniform vec4 en_bound_color;\n";
    }
    string getDefaultVertexShader(){
        return  "in vec3 in_Position;                 // (x,y,z)\n"
                "in vec3 in_Normal;                  // (x,y,z)\n"
                "in vec4 in_Color;                    // (r,g,b,a)\n"
                "in vec2 in_TextureCoord;             // (u,v)\n"

                "out vec2 v_TextureCoord;\n"
                "out vec4 v_Color;\n"
                "out vec3 v_lightIntensity;\n"
                "uniform int en_ActiveLights;"

                "struct LightInfo {\n"
                  "vec4 Position; // Light position in eye coords\n"
                  "vec4 La; // Ambient light intensity\n"
                  "vec4 Ld; // Diffuse light intensity\n"
                  "vec4 Ls; // Specular light intensity\n"
                  //"bool type; //Type - directional or point"
                "};\n"
                "uniform LightInfo Light[MAX_LIGHTS];\n"

                "struct MaterialInfo {\n"
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

                "vec4 phongModel( vec3 norm, vec4 position )\n"
                "{\n"
                  "vec4 total_light = vec4(0.0);"
                  "for (int index = 0; index < en_ActiveLights; ++index){\n"
                      "vec3 L;\n"
                      "if (Light[index].Position.w == 0){ //Directional light\n"
                        "L = -normalize(Light[index].Position.xyz);\n"
                      "}else{ //Point light\n"
                        "L = -normalize(vec3((Light[index].Position.xyz - position.xyz).xyz));\n"
                      "}\n"
                      "vec3 v = normalize(-position.xyz);\n"
                      "vec3 r = reflect( -L, norm );\n"
                      "vec4 ambient = Light[index].La * Material.Ka;\n"
                      "float LdotN = max( dot(norm,L), 0.0 );\n"
                      "vec4 diffuse = Light[index].Ld * Material.Kd * LdotN;\n"
                      "vec4 spec = vec4(0.0);\n"
                      "if( LdotN > 0.0 )\n"
                          "spec = Light[index].Ls * Material.Ks * pow( max( dot(r,v), 0.0 ), Material.Shininess );\n"
                      "total_light += diffuse + ambient;\n"
                  "}\n"
                  "return total_light;\n"
                "}\n"

                "void main()\n"
                "{\n"
                    "if (en_LightingEnabled == true){\n"
                        "vec3 eyeNorm;\n"
                        "vec4 eyePosition;\n"
                        "getEyeSpace(eyeNorm, eyePosition);\n"
                        "v_Color = en_AmbientColor * Material.Ka + phongModel( eyeNorm, eyePosition ) * in_Color;\n"
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
                        "TexColor = texture2D( en_TexSampler, v_TextureCoord.st ) * v_Color;\n"
                    "}else if (en_ColorEnabled == true){\n"
                        "TexColor = v_Color;\n"
                    "}else if (en_TexturingEnabled == true){\n"
                        "TexColor = texture2D( en_TexSampler, v_TextureCoord.st );\n"
                    "}else{\n"
                        "TexColor = en_bound_color;\n"
                    "}\n"
                    "out_FragColor = TexColor;\n"// * LightColor;"
                "}\n";
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

        shaderprograms[prog_id]->uni_color = enigma_user::glsl_get_uniform_location(prog_id, "en_bound_color");
        shaderprograms[prog_id]->uni_ambient_color = enigma_user::glsl_get_uniform_location(prog_id, "en_AmbientColor");
        shaderprograms[prog_id]->uni_light_active = enigma_user::glsl_get_uniform_location(prog_id, "en_ActiveLights");

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

void glsl_shader_print_infolog(int id)
{
    GLint blen = 0;
    GLsizei slen = 0;
    glGetShaderiv(enigma::shaders[id]->shader, GL_INFO_LOG_LENGTH , &blen);
    if (blen > 1)
    {
        GLchar* compiler_log = (GLchar*)malloc(blen);
        glGetShaderInfoLog(enigma::shaders[id]->shader, blen, &slen, compiler_log);
        enigma::shaders[id]->log = (string)compiler_log;
        std::cout << compiler_log << std::endl;
    } else {
        enigma::shaders[id]->log = "Shader log empty";
        std::cout << enigma::shaders[id]->log << std::endl;
    }
}

void glsl_program_print_infolog(int id)
{
    GLint blen = 0;
    GLsizei slen = 0;
    glGetProgramiv(enigma::shaderprograms[id]->shaderprogram, GL_INFO_LOG_LENGTH , &blen);
    if (blen > 1)
    {
        GLchar* compiler_log = (GLchar*)malloc(blen);
        glGetProgramInfoLog(enigma::shaderprograms[id]->shaderprogram, blen, &slen, compiler_log);
        enigma::shaderprograms[id]->log = (string)compiler_log;
        std::cout << compiler_log << std::endl;
    } else {
        enigma::shaderprograms[id]->log = "Shader program log empty";
        std::cout << enigma::shaderprograms[id]->log << std::endl;
    }
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
    //std::cout << ShaderSource << std::endl;
    glShaderSource(enigma::shaders[id]->shader, 1, &ShaderSource, NULL);
    return true; // No Error
}

bool glsl_shader_compile(int id)
{
    glCompileShader(enigma::shaders[id]->shader);

    GLint compiled;
    glGetProgramiv(enigma::shaders[id]->shader, GL_COMPILE_STATUS, &compiled);
    if (compiled)
    {
        return true;
    } else {
        std::cout << "Shader[" << id << "] - Compilation failed - Info log: " << std::endl;
        glsl_shader_print_infolog(id);
        return false;
    }
}

bool glsl_shader_get_compiled(int id) {
    GLint compiled;
    glGetProgramiv(enigma::shaders[id]->shader, GL_COMPILE_STATUS, &compiled);
    if (compiled)
    {
        return true;
    } else {
        return false;
    }
}

string glsl_shader_get_infolog(int id)
{
    return enigma::shaders[id]->log;
}

string glsl_program_get_infolog(int id)
{
    return enigma::shaderprograms[id]->log;
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
    if (linked)
    {
        return true;
    } else {
        std::cout << "Shader program[" << id << "] - Linking failed - Info log: " << std::endl;
        glsl_program_print_infolog(id);
        return false;
    }
}

bool glsl_program_validate(int id)
{
    glValidateProgram(enigma::shaderprograms[id]->shaderprogram);
    GLint validated;
    glGetProgramiv(enigma::shaderprograms[id]->shaderprogram, GL_VALIDATE_STATUS, &validated);
    if (validated)
    {
        return true;
    } else {
        std::cout << "Shader program[" << id << "] - Validation failed - Info log: " << std::endl;
        glsl_program_print_infolog(id);
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
    texture_reset();
    enigma::bound_shader = id;
    glUseProgram(enigma::shaderprograms[id]->shaderprogram);
}

void glsl_program_reset()
{
    //NOTE: Texture must be reset first so the Global VBO can draw and let people use shaders on text.
    texture_reset();
    enigma::bound_shader = enigma::default_shader;
    glUseProgram(enigma::shaderprograms[enigma::default_shader]->shaderprogram);
}

void glsl_program_free(int id)
{
    delete enigma::shaderprograms[id];
}

int glsl_get_uniform_location(int program, string name) {
	int uni = glGetUniformLocation(enigma::shaderprograms[program]->shaderprogram, name.c_str());
    if (uni == -1){
        printf("Program[%i] - Uniform %s not found!\n", program, name.c_str());
    }
    return uni;
}

int glsl_get_attribute_location(int program, string name) {
	int uni = glGetAttribLocation(enigma::shaderprograms[program]->shaderprogram, name.c_str());
    if (uni == -1){
        printf("Program[%i] - Attribute %s not found!\n", program, name.c_str());
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

