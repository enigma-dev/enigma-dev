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

#include "OPENGL3Std.h"
#include "GL3shader.h"
#include "GLSLshader.h"

#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"

#include "Universal_System/shaderstruct.h"
#include "Universal_System/var4.h"

#include <iostream>
#include <string>
#include <stdlib.h>     /* malloc, free, rand */

using namespace std;

namespace enigma_user {

extern string shader_get_name(int i);

} // namespace enigma_user

namespace enigma {
  unsigned default_shader;
  unsigned main_shader;
  unsigned bound_shader;
  unsigned bound_vbo = -1; //This means it's max-1, just so it wouldn't randomly be 0 at first render call.
  unsigned bound_vboi = -1; //This means it's max-1

  void graphicssystem_initialize()
  {
    glEnable(GL_SCISSOR_TEST); // constrain clear to viewport like D3D9
    glDepthFunc(GL_LEQUAL); // to match GM8's D3D8 default

    init_shaders();
    // read shaders into graphics system structure and compile and link them if needed
    for (size_t i = 0; i < shader_idmax; ++i) {
      ShaderStruct* shaderstruct = shaderdata[i];
      //TODO(harijs): If precompile == false or ID's are not defragged, there is a segfault because we try to access invalid position in shader vector
      if (shaderstruct->precompile == false) { continue; }

      int vshader_id = enigma_user::glsl_shader_create(enigma_user::sh_vertex);
      enigma_user::glsl_shader_load_string(vshader_id, shaderstruct->vertex);

      int fshader_id = enigma_user::glsl_shader_create(enigma_user::sh_fragment);
      enigma_user::glsl_shader_load_string(fshader_id, shaderstruct->fragment);

      int prog_id = enigma_user::glsl_program_create();
      enigma_user::glsl_program_set_name(prog_id, enigma_user::shader_get_name(i));

      enigma_user::glsl_shader_compile(vshader_id);
      enigma_user::glsl_shader_compile(fshader_id);

      enigma_user::glsl_program_attach(prog_id, vshader_id);
      enigma_user::glsl_program_attach(prog_id, fshader_id);
      enigma_user::glsl_program_link(prog_id);
      enigma_user::glsl_program_validate(prog_id);
    }

    //ADD DEFAULT SHADER (emulates FFP)
    int vshader_id = enigma_user::glsl_shader_create(enigma_user::sh_vertex);
    enigma_user::glsl_shader_load_string(vshader_id, getDefaultVertexShader());

    int fshader_id = enigma_user::glsl_shader_create(enigma_user::sh_fragment);
    enigma_user::glsl_shader_load_string(fshader_id, getDefaultFragmentShader());

    int prog_id = enigma_user::glsl_program_create();

    enigma_user::glsl_shader_compile(vshader_id);
    enigma_user::glsl_shader_compile(fshader_id);
    enigma_user::glsl_program_attach(prog_id, vshader_id);
    enigma_user::glsl_program_attach(prog_id, fshader_id);
    enigma_user::glsl_program_link(prog_id);
    enigma_user::glsl_program_validate(prog_id);
    enigma_user::glsl_program_set_name(prog_id, "DEFAULT_SHADER");

    default_shader = prog_id; //Default shader for FFP
    main_shader = default_shader; //Main shader used to override the default one

    enigma_user::glsl_program_reset(); //Set the default program
    //END DEFAULT SHADER

    //In GL3.3 Core VAO is mandatory. So we create one and never change it
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
  }
}
