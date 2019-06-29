#include "Graphics_Systems/OpenGL-Common/version.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/OpenGL-Desktop/enums.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSd3d.h"

namespace enigma {
  const gl_profile_type graphics_opengl_profile = gl_profile_core;
  const unsigned int gl_major = 3, gl_minor = 3; 
  const std::string shader_header = "#version 330\n";
  
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
