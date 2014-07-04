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

namespace enigma_user{
	extern string shader_get_name(int i);
}

namespace enigma
{
  unsigned bound_texture=0;
  unsigned default_shader;
  unsigned main_shader;
  unsigned bound_shader;
  unsigned char currentcolor[4] = {0,0,0,255};
  int currentblendmode[2] = {0,0};
  int currentblendtype = 0;
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

            int vshader_id = glsl_shader_create(enigma_user::sh_vertex);
            glsl_shader_load_string(vshader_id, shaderstruct->vertex);

            int fshader_id = glsl_shader_create(enigma_user::sh_fragment);
            glsl_shader_load_string(fshader_id, shaderstruct->fragment);

            int prog_id = glsl_program_create();
			glsl_program_set_name(prog_id, enigma_user::shader_get_name(i));

            if (shaderstruct->precompile) {
                glsl_shader_compile(vshader_id);
                glsl_shader_compile(fshader_id);
            }

            glsl_program_attach(prog_id, vshader_id);
            glsl_program_attach(prog_id, fshader_id);
            glsl_program_link(prog_id);
            glsl_program_validate(prog_id);
        }

        //ADD DEFAULT SHADER (emulates FFP)
        int vshader_id = glsl_shader_create(enigma_user::sh_vertex);
        glsl_shader_load_string(vshader_id, getDefaultVertexShader());

        int fshader_id = glsl_shader_create(enigma_user::sh_fragment);
        glsl_shader_load_string(fshader_id, getDefaultFragmentShader());

        int prog_id = glsl_program_create();

        glsl_shader_compile(vshader_id);
        glsl_shader_compile(fshader_id);
        glsl_program_attach(prog_id, vshader_id);
        glsl_program_attach(prog_id, fshader_id);
        glsl_program_link(prog_id);
        glsl_program_validate(prog_id);

        getUniforms(prog_id);
        getAttributes(prog_id);
        getDefaultUniforms(prog_id);
        getDefaultAttributes(prog_id);

        default_shader = prog_id; //Default shader for FFP
        main_shader = default_shader; //Main shader used to override the default one

        glsl_program_reset(); //Set the default program
        //END DEFAULT SHADER

        glViewport(0,0,(int)room_width,(int)room_height);
        d3d_set_projection_ortho(0,(int)room_width,0,(int)room_height, 0);
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

