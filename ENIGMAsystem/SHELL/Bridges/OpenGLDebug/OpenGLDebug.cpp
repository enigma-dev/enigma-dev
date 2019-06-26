#include "OpenGLDebug.h"

#include <iostream>

namespace enigma {

void openglCallbackFunction(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                                     const GLchar* message, const void* userParam) {
  std::cerr << "---------------------opengl-callback-start------------" << std::endl;
  std::cerr << "message: " << message << std::endl;
  std::cerr << "type: ";

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      std::cerr << "ERROR";
      break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      std::cerr << "DEPRECATED_BEHAVIOR";
      break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      std::cerr << "UNDEFINED_BEHAVIOR";
      break;

    case GL_DEBUG_TYPE_PORTABILITY:
      std::cerr << "PORTABILITY";
      break;

    case GL_DEBUG_TYPE_PERFORMANCE:
      std::cerr << "PERFORMANCE";
      break;

    case GL_DEBUG_TYPE_OTHER:
      std::cerr << "OTHER";
      break;
  }

  std::cerr << std::endl;

  std::cerr << "id: " << id << std::endl;
  std::cerr << "severity: ";

  switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
      std::cerr << "LOW";
      break;

    case GL_DEBUG_SEVERITY_MEDIUM:
      std::cerr << "MEDIUM";
      break;

    case GL_DEBUG_SEVERITY_HIGH:
      std::cerr << "HIGH";
      break;
  }

  std::cerr << std::endl;
  std::cerr << "---------------------opengl-callback-end--------------" << std::endl;
}

void register_opengl_debug_callback() {
  if (glDebugMessageCallback) {
    std::cerr << "Registered OpenGL debug callback" << std::endl;
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    unsigned int unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
  } else std::cerr << "glDebugMessageCallback not available" << std::endl;
}

} // namespace enigma
