/** Copyright (C) 2014 Harijs Grinbergs
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

#include "Graphics_Systems/OpenGL-Common/version.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/OpenGL-Desktop/enums.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSd3d.h"

#include <string>
#include <cstring>

namespace enigma {
  const gl_profile_type graphics_opengl_profile = gl_profile_core;
  const unsigned int gl_major = 3, gl_minor = 3;
  const std::string shader_header = "#version 330\n";

  bool gl_extension_supported(std::string extension) {
    GLint n, i;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    for (i = 0; i < n; ++i) {
      if (std::strstr((char*)glGetStringi(GL_EXTENSIONS, i),extension.c_str())!=NULL) return true;
    }
    return false;
  }

  void init_vao() {
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
  }

  void graphics_flush_ext() {
    glPolygonMode(GL_FRONT_AND_BACK, fillmodes[drawFillMode]);
    (msaaEnabled?glEnable:glDisable)(GL_MULTISAMPLE);
    (d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
    (d3dClipPlane?glEnable:glDisable)(GL_CLIP_DISTANCE0);
    glPointSize(drawPointSize);
  }
}
