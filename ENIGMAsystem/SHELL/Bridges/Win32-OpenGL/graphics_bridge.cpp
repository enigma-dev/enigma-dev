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

#include "OpenGLHeaders.h"
#include "Graphics_Systems/OpenGL/GLversion.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Bridges/OpenGL/GLload.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/Win32/WINDOWSmain.h"
#include "Platforms/General/PFwindow.h"

#include <string>
#include <GL/wglew.h>
#include <windows.h>

namespace enigma {

extern HGLRC hRC;

extern void (*WindowResizedCallback)();
void WindowResized() {
  // clear the window color, viewport does not need set because backbuffer was just recreated
  enigma_user::draw_clear(enigma_user::window_get_color());
}

void EnableDrawing(void*)
{
  WindowResizedCallback = &WindowResized;
  /**
   * Edited by Cool Breeze on 16th October 2013
   * + Updated the Pixel Format to support 24-bitdepth buffers
   * + Correctly create a GL 3.x compliant context
   */
  HGLRC LegacyRC;
  PIXELFORMATDESCRIPTOR pfd;
  int iFormat;

  enigma::window_hDC = GetDC (hWnd);
  ZeroMemory (&pfd, sizeof (pfd));
  pfd.nSize = sizeof (pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 24;
  pfd.cStencilBits = 8;
  pfd.iLayerType = PFD_MAIN_PLANE;
  iFormat = ChoosePixelFormat (enigma::window_hDC, &pfd);

  if (iFormat==0) { show_error("Failed to set the format of the OpenGL graphics device.",1); }

  SetPixelFormat ( enigma::window_hDC, iFormat, &pfd );
  LegacyRC = wglCreateContext( enigma::window_hDC );
  wglMakeCurrent( enigma::window_hDC, LegacyRC );

  gl_load_exts();

  const bool gl_core = (graphics_opengl_profile==gl_profile_core);

  if (gl_core && wglewIsSupported("WGL_ARB_create_context"))
  {
    // -- Define an array of Context Attributes
    int attribs[] =
    {
      WGL_CONTEXT_PROFILE_MASK_ARB,gl_core?WGL_CONTEXT_CORE_PROFILE_BIT_ARB:WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
      #ifdef DEBUG_MODE
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
      #endif
      0
    };

    hRC = wglCreateContextAttribsARB( enigma::window_hDC,0, attribs );
    wglMakeCurrent( NULL,NULL );
    wglDeleteContext( LegacyRC );
    wglMakeCurrent(enigma::window_hDC, hRC );
  } else { // unable to get a core context, use the legacy context
    hRC = LegacyRC;
  }
}

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
