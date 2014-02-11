/** Copyright (C) 2008-2014 Josh Ventura, Robert B. Colton, Harijs Grinbergs
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

#include <iostream>
#include <string>
#include <stdlib.h>     /* malloc, free, rand */

#include "Bridges/General/GL3Context.h"
#include "../General/OpenGLHeaders.h"
#include "../General/GSmatrix.h" //For d3d_set_projection_ortho
using namespace std;
#include "OPENGL3Std.h"
#include "GL3shader.h"
#include "GLSLshader.h"
#include "Universal_System/shaderstruct.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h" // Room dimensions.
#include "Graphics_Systems/graphics_mandatory.h" // Room dimensions.

ContextManager* oglmgr = NULL;

namespace enigma
{
  unsigned bound_texture=0;
  ShaderProgram* default_shader;
  unsigned char currentcolor[4] = {0,0,0,255};
  bool glew_isgo;
  bool pbo_isgo;

    void graphicssystem_initialize()
    {
        oglmgr = new ContextManager();
        GLenum err = glewInit();

        #ifdef DEBUG_MODE
        if (GLEW_OK != err)
        {
          std::cout<<"GLEW ERROR!"<<std::endl;
        }
        #endif

        //enigma::pbo_isgo=GL_ARB_pixel_buffer_object;
        using enigma_user::room_width;
        using enigma_user::room_height;

        glViewport(0,0,(int)room_width,(int)room_height);
        d3d_set_projection_ortho(0,(int)room_width,0,(int)room_height, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_BLEND);
        glEnable(GL_ALPHA_TEST);
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glAlphaFunc(GL_ALWAYS,0);

        glBindTexture(GL_TEXTURE_2D,bound_texture=0);

        init_shaders();
        // read shaders into graphics system structure and compile and link them if needed
        for (size_t i = 0; i < shader_idmax; ++i) {
            ShaderStruct* shaderstruct = shaderdata[i];

            //if (string(shaderstruct->type) != string("GLSL")) { continue; }

            Shader* vshader = new Shader(enigma_user::sh_vertex);
            shaders.push_back(vshader);
            glShaderSource(vshader->shader, 1, (const GLchar**)&shaderstruct->vertex, NULL);

            Shader* fshader = new Shader(enigma_user::sh_fragment);
            shaders.push_back(fshader);
            glShaderSource(fshader->shader, 1, (const GLchar**)&shaderstruct->fragment, NULL);

            ShaderProgram* program = new ShaderProgram();
            shaderprograms.push_back(program);

            if (shaderstruct->precompile) {
                glCompileShader(vshader->shader);
                glCompileShader(fshader->shader);

                GLint blen = 0;
                GLsizei slen = 0;

                glGetShaderiv(vshader->shader, GL_INFO_LOG_LENGTH , &blen);

                if (blen > 1)
                {
                    GLchar* compiler_log = (GLchar*)malloc(blen);

                    glGetInfoLogARB(vshader->shader, blen, &slen, compiler_log);
                    std::cout << compiler_log << std::endl;
                } else {
                    std::cout << "Vertex shader compile log empty" << std::endl;
                }

                glGetShaderiv(fshader->shader, GL_INFO_LOG_LENGTH , &blen);

                if (blen > 1)
                {
                    GLchar* compiler_log = (GLchar*)malloc(blen);

                    glGetInfoLogARB(fshader->shader, blen, &slen, compiler_log);
                    std::cout << compiler_log << std::endl;
                } else {
                    std::cout << "Fragment shader compile log empty" << std::endl;
                }

            }

            glAttachShader(program->shaderprogram, vshader->shader);
            glAttachShader(program->shaderprogram, fshader->shader);

            glLinkProgram(program->shaderprogram);
            glValidateProgram(program->shaderprogram);
        }

        //ADD DEFAULT SHADER (emulates FFP)
        Shader* vshader = new Shader(enigma_user::sh_vertex);
        shaders.push_back(vshader);
        const char *versource_str = (getVertexShaderPrefix()+getDefaultVertexShader()).c_str();
        glShaderSource(vshader->shader, 1, &versource_str, NULL);

        Shader* fshader = new Shader(enigma_user::sh_fragment);
        shaders.push_back(fshader);
        const char *fragsource_str = (getFragmentShaderPrefix()+getDefaultFragmentShader()).c_str();
        glShaderSource(fshader->shader, 1, &fragsource_str, NULL);

        ShaderProgram* program = new ShaderProgram();
        shaderprograms.push_back(program);
        int prog_id = shaderprograms.size()-1;

        glCompileShader(vshader->shader);
        glCompileShader(fshader->shader);

        GLint blen = 0;
        GLsizei slen = 0;

        glGetShaderiv(vshader->shader, GL_INFO_LOG_LENGTH , &blen);

        if (blen > 1)
        {
            GLchar* compiler_log = (GLchar*)malloc(blen);

            glGetInfoLogARB(vshader->shader, blen, &slen, compiler_log);
            std::cout << compiler_log << std::endl;
        } else {
            std::cout << "Vertex shader compile log empty" << std::endl;
        }

        glGetShaderiv(fshader->shader, GL_INFO_LOG_LENGTH , &blen);

        if (blen > 1)
        {
            GLchar* compiler_log = (GLchar*)malloc(blen);

            glGetInfoLogARB(fshader->shader, blen, &slen, compiler_log);
            std::cout << compiler_log << std::endl;
        } else {
            std::cout << "Fragment shader compile log empty" << std::endl;
        }

        glAttachShader(program->shaderprogram, vshader->shader);
        glAttachShader(program->shaderprogram, fshader->shader);

        glLinkProgram(program->shaderprogram);

        glGetProgramiv(vshader->shader, GL_INFO_LOG_LENGTH , &blen);

        if (blen > 1)
        {
            GLchar* compiler_log = (GLchar*)malloc(blen);

            glGetProgramInfoLog(vshader->shader, blen, &slen, compiler_log);
            std::cout << compiler_log << std::endl;
        } else {
            std::cout << "Program link log empty" << std::endl;
        }

        glValidateProgram(program->shaderprogram);

        glGetProgramiv(vshader->shader, GL_INFO_LOG_LENGTH , &blen);

        if (blen > 1)
        {
            GLchar* compiler_log = (GLchar*)malloc(blen);

            glGetProgramInfoLog(vshader->shader, blen, &slen, compiler_log);
            std::cout << compiler_log << std::endl;
        } else {
            std::cout << "Program validation log empty" << std::endl;
        }

        program->uni_viewMatrix = glsl_get_uniform_location(prog_id, "transform_matrix[0]");
        program->uni_projectionMatrix = glsl_get_uniform_location(prog_id, "transform_matrix[1]");
        program->uni_modelMatrix = glsl_get_uniform_location(prog_id, "transform_matrix[2]");
        program->uni_mvMatrix = glsl_get_uniform_location(prog_id, "transform_matrix[3]");
        program->uni_mvpMatrix = glsl_get_uniform_location(prog_id, "transform_matrix[4]");
        program->uni_normalMatrix = glsl_get_uniform_location(prog_id, "normalMatrix");
        program->uni_texSampler = glsl_get_uniform_location(prog_id, "TexSampler");

        program->uni_textureEnable = glsl_get_uniform_location(prog_id, "en_TexturingEnabled");
        program->uni_colorEnable = glsl_get_uniform_location(prog_id, "en_ColorEnabled");
        program->uni_lightEnable = glsl_get_uniform_location(prog_id, "en_LightingEnabled");

        program->uni_color = glsl_get_uniform_location(prog_id, "en_bound_color");
        program->uni_ambient_color = glsl_get_uniform_location(prog_id, "en_AmbientColor");
        program->uni_light_active = glsl_get_uniform_location(prog_id, "en_ActiveLights");

        char tchars[64];
        for (unsigned int i=0; i<8; ++i){
            sprintf(tchars, "Light[%d].Position", i);
            program->uni_light_position[i] = glsl_get_uniform_location(prog_id, tchars);
            sprintf(tchars, "Light[%d].La", i);
            program->uni_light_ambient[i] = glsl_get_uniform_location(prog_id, tchars);
            sprintf(tchars, "Light[%d].Ld", i);
            program->uni_light_diffuse[i] = glsl_get_uniform_location(prog_id, tchars);
            sprintf(tchars, "Light[%d].Ls", i);
            program->uni_light_specular[i] = glsl_get_uniform_location(prog_id, tchars);
        }
        program->uni_material_ambient = glsl_get_uniform_location(prog_id, "Material.Ka");
        program->uni_material_diffuse = glsl_get_uniform_location(prog_id, "Material.Kd");
        program->uni_material_specular = glsl_get_uniform_location(prog_id, "Material.Ks");
        program->uni_material_shininess = glsl_get_uniform_location(prog_id, "Material.Shininess");

        program->att_vertex = glGetAttribLocation(program->shaderprogram, "in_Position");
        program->att_color = glGetAttribLocation(program->shaderprogram, "in_Color");
        program->att_texture = glGetAttribLocation(program->shaderprogram, "in_TextureCoord");
        program->att_normal = glGetAttribLocation(program->shaderprogram, "in_Normal");

        default_shader = program;

        glsl_program_reset(); //Set the default program
        //END DEFAULT SHADER
    }
}

namespace enigma_user {
// Stolen entirely from the documentation and thrown into a switch() structure.
string draw_get_graphics_error()
{
  GLenum err = glGetError();
  switch (err)
  {
    case GL_NO_ERROR:         return "";
    case GL_INVALID_ENUM:     return "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_VALUE:    return "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_OPERATION:return "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_STACK_OVERFLOW:   return "This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_STACK_UNDERFLOW:  return "This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_OUT_OF_MEMORY:    return "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
    //case GL_TABLE_TOO_LARGE:  return "The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag.";
  }
  return "Unspecified error.";
}
}

