#include "OpenGLHeaders.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <string>
#include <sstream>

namespace enigma {

void openglCallbackFunction(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                                     const GLchar* message, const void* userParam) {
                                     
  MESSAGE_TYPE errType = MESSAGE_TYPE::M_ERROR;
  
  std::stringstream error;
  error << "\n---------------------opengl-callback-start------------\n";
  error << "message: " << message << "\n";
  error << "type: ";

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      error << "ERROR";
      break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      errType = MESSAGE_TYPE::M_WARNING;
      error << "DEPRECATED_BEHAVIOR";
      break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      error << "UNDEFINED_BEHAVIOR";
      break;

    case GL_DEBUG_TYPE_PORTABILITY:
      errType = MESSAGE_TYPE::M_WARNING;
      error << "PORTABILITY";
      break;

    case GL_DEBUG_TYPE_PERFORMANCE:
      errType = MESSAGE_TYPE::M_WARNING;
      error << "PERFORMANCE";
      break;

    case GL_DEBUG_TYPE_OTHER:
      errType = MESSAGE_TYPE::M_INFO;
      error << "OTHER";
      break;
  }

  error << "\n";

  error << "id: " << std::to_string(id) << "\n";
  error << "severity: ";

  switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
      error << "LOW";
      break;

    case GL_DEBUG_SEVERITY_MEDIUM:
      error << "MEDIUM";
      break;

    case GL_DEBUG_SEVERITY_HIGH:
      error << "HIGH";
      break;
  }

  error << "\n";
  error << "---------------------opengl-callback-end--------------\n";
  DEBUG_MESSAGE(error.str(), errType);
}

void register_gl_debug_callback() {
  if (glDebugMessageCallback) {
    DEBUG_MESSAGE("Registered OpenGL debug callback", MESSAGE_TYPE::M_INFO);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback((GLDEBUGPROC)&openglCallbackFunction, nullptr);
    unsigned int unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
  } else DEBUG_MESSAGE("glDebugMessageCallback not available", MESSAGE_TYPE::M_INFO);
}

} // namespace enigma
