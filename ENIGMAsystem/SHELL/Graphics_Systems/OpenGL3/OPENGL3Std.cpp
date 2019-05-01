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

#include "Graphics_Systems/OpenGL/GLversion.h"
#include "Graphics_Systems/OpenGL/GLscreen.h"
#include "OpenGLHeaders.h"

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h"
#endif

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
  const gl_profile_type graphics_opengl_profile = gl_profile_core;

  unsigned default_shader;
  unsigned main_shader;
  unsigned bound_shader;
  unsigned bound_vbo = -1; //This means it's max-1, just so it wouldn't randomly be 0 at first render call.
  unsigned bound_vboi = -1; //This means it's max-1

#ifdef DEBUG_MODE
  //Based on code from Cort Stratton (http://www.altdev.co/2011/06/23/improving-opengl-error-messages/)
  void FormatDebugOutputARB(char outStr[], size_t outStrSize, GLenum source, GLenum type, GLuint id, GLenum severity, const char *msg) {
    char sourceStr[32]; const char *sourceFmt = "UNDEFINED(0x%04X)";
    switch(source) {
      case GL_DEBUG_SOURCE_API_ARB: sourceFmt = "API"; break;
      case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: sourceFmt = "WINDOW_SYSTEM"; break;
      case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: sourceFmt = "SHADER_COMPILER"; break;
      case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: sourceFmt = "THIRD_PARTY"; break;
      case GL_DEBUG_SOURCE_APPLICATION_ARB: sourceFmt = "APPLICATION"; break;
      case GL_DEBUG_SOURCE_OTHER_ARB: sourceFmt = "OTHER"; break;
    }
    snprintf(sourceStr, 32, sourceFmt, source);
    char typeStr[32];
    const char *typeFmt = "UNDEFINED(0x%04X)";
    switch(type) {
      case GL_DEBUG_TYPE_ERROR_ARB: typeFmt = "ERROR"; break;
      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: typeFmt = "DEPRECATED_BEHAVIOR"; break;
      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: typeFmt = "UNDEFINED_BEHAVIOR"; break;
      case GL_DEBUG_TYPE_PORTABILITY_ARB: typeFmt = "PORTABILITY"; break;
      case GL_DEBUG_TYPE_PERFORMANCE_ARB: typeFmt = "PERFORMANCE"; break;
      case GL_DEBUG_TYPE_OTHER_ARB: typeFmt = "OTHER"; break;
    }
    snprintf(typeStr, 32, typeFmt, type);
    char severityStr[32];
    const char *severityFmt = "UNDEFINED(%i)";
    switch(severity) {
      case GL_DEBUG_SEVERITY_HIGH_ARB: severityFmt = "HIGH"; break;
      case GL_DEBUG_SEVERITY_MEDIUM_ARB: severityFmt = "MEDIUM"; break;
      case GL_DEBUG_SEVERITY_LOW_ARB: severityFmt = "LOW"; break;
    }
    snprintf(severityStr, 32, severityFmt, severity);
    snprintf(outStr, outStrSize, "OpenGL: %s [source=%s type=%s severity=%s id=%d]", msg, sourceStr, typeStr, severityStr, id);
  }

  void DebugCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid* userParam) {
    char finalMessage[256];
    FormatDebugOutputARB(finalMessage, 256, source, type, id, severity, message);
    printf("%s\n", finalMessage);
    show_error(toString(finalMessage), false);
  }
#endif

  void graphicssystem_initialize()
  {
    #ifdef DEBUG_MODE
    glDebugMessageCallbackARB((GLDEBUGPROCARB)&DebugCallbackARB, 0);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

    GLuint other_ids[] = { 131185 };
    glDebugMessageControlARB(GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_OTHER_ARB, GL_DONT_CARE, 1, other_ids, GL_FALSE); //Disable some notifications shown below:
    //OpenGL: Buffer detailed info: Buffer object 1 (bound to GL_ELEMENT_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations. [source=API type=OTHER severity=UNDEFINED (33387) id=131185]
    GLuint performance_ids[] = { 131218, 2 };
    glDebugMessageControlARB(GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_PERFORMANCE_ARB, GL_DONT_CARE, 2, performance_ids, GL_FALSE); //Disable some notifications shown below:
    //OpenGL: Program/shader state performance warning: Vertex shader in program 9 is being recompiled based on GL state. [source=API type=PERFORMANCE severity=MEDIUM id=131218] - This is NVidia only and doesn't tell much
    #endif

    gl_screen_init();

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
