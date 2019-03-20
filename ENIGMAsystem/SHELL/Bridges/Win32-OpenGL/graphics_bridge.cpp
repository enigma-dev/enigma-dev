/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2018 Robert B. Colton
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

#include "Platforms/Win32/WINDOWSmain.h"
#include "Platforms/General/PFwindow.h"

#include <string>
#include <GL/glew.h>
#include <GL/wglew.h>

namespace enigma {

GLuint msaa_fbo = 0;
extern HGLRC hRC;

void DisableDrawing(void*)
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(enigma::hWnd, enigma::window_hDC);
}

namespace swaphandling {
  bool has_checked_extensions = false;
  bool ext_swapcontrol_supported;

  void investigate_swapcontrol_support() {
    if (has_checked_extensions) return; // Already calculated, no need to calculate it more.

    const char *wgl_extensions = wglGetExtensionsStringARB(window_hDC);

    ext_swapcontrol_supported = strstr(wgl_extensions, "WGL_EXT_swap_control");

    has_checked_extensions = true;
  }
}

bool is_ext_swapcontrol_supported() {
  swaphandling::investigate_swapcontrol_support();
  return swaphandling::ext_swapcontrol_supported;
}

void ScreenRefresh() {
  SwapBuffers(enigma::window_hDC);
}

}

namespace enigma_user {

void display_reset(int samples, bool vsync) {
  int interval = vsync ? 1 : 0;

  if (enigma::is_ext_swapcontrol_supported()) {
    wglSwapIntervalEXT(interval);
  }

  GLint fbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo);

  GLuint ColorBufferID, DepthBufferID;

  // Cleanup the multi-sampler fbo if turning off multi-sampling
  if (samples == 0) {
    if (enigma::msaa_fbo != 0) {
      glDeleteFramebuffers(1, &enigma::msaa_fbo);
      enigma::msaa_fbo = 0;
    }
    return;
  }

  //TODO: Change the code below to fix this to size properly to views
  // If we don't already have a multi-sample fbo then create one
  if (enigma::msaa_fbo == 0) {
    glGenFramebuffersEXT(1, &enigma::msaa_fbo);
  }
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, enigma::msaa_fbo);
  // Now make a multi-sample color buffer
  glGenRenderbuffersEXT(1, &ColorBufferID);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, ColorBufferID);
  glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, samples, GL_RGBA8, window_get_region_width(), window_get_region_height());
  // We also need a depth buffer
  glGenRenderbuffersEXT(1, &DepthBufferID);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, DepthBufferID);
  glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, samples, GL_DEPTH_COMPONENT24, window_get_region_width(), window_get_region_height());
  // Attach the render buffers to the multi-sampler fbo
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, ColorBufferID);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, DepthBufferID);

}

void set_synchronization(bool enable) {
  // General notes:
  // Setting swapping on and off is platform-dependent and requires platform-specific extensions.
  // Platform-specific extensions are even more bothersome than regular extensions.
  // What functions and features to use depends on which version of OpenGL is used.
  // For more information, see the following pages:
  // http://www.opengl.org/wiki/Load_OpenGL_Functions
  // http://www.opengl.org/wiki/OpenGL_Loading_Library
  // http://www.opengl.org/wiki/Swap_Interval
  // http://en.wikipedia.org/wiki/WGL_%28software%29
  // Also note that OpenGL version >= 3.0 does not use glGetString for getting extensions.

  int interval = enable ? 1 : 0;

  if (enigma::is_ext_swapcontrol_supported()) {
    wglSwapIntervalEXT(interval);
  }
}

}
