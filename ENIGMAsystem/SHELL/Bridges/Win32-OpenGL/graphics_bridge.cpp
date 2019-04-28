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

#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/Win32/WINDOWSmain.h"
#include "Platforms/General/PFwindow.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"

#include <string>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <windows.h>

namespace enigma {

GLuint msaa_fbo = 0;
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

  GLenum err = glewInit();
  if (GLEW_OK != err)
    show_error(std::string("Failed to initialize glew for OpenGL. ") + (const char*)glewGetErrorString(err), true);

  if (graphics_opengl_core && wglewIsSupported("WGL_ARB_create_context"))
  {
    // -- Define an array of Context Attributes
    int attribs[] =
    {
      WGL_CONTEXT_PROFILE_MASK_ARB, graphics_opengl_core?WGL_CONTEXT_CORE_PROFILE_BIT_ARB:WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
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

  //TODO: This never reports higher than 8, but display_aa should be 14 if 2,4,and 8 are supported and 8 only when only 8 is supported
  glGetIntegerv(GL_MAX_SAMPLES_EXT, &enigma_user::display_aa);
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

void init_display_aa_levels() {
  if (!wglewIsSupported("WGL_ARB_create_context")) return;
  if (!wglChoosePixelFormatARB) return;

  int pixelFormat;
  bool valid;
  UINT numFormats;
  float fAttributes[] = {0,0};
  HDC hDC = enigma::window_hDC;

  enigma_user::display_aa = 0;
  for (int i = 32; i > 1; i/=2) {
    int iAttributes[] = {
      WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
      WGL_SAMPLES_ARB, i,
      0,0,
    };
    valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
    if (!valid || !numFormats) continue;
    enigma_user::display_aa += i;
  }
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

  //wglGetPixelFormatAttribivARB(WGL_SAMPLES_ARB)

  int pixelFormat = GetPixelFormat(enigma::window_hDC);
  PIXELFORMATDESCRIPTOR pfd;
  ZeroMemory(&pfd, sizeof(pfd));
  DescribePixelFormat(enigma::window_hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  WINDOWINFO winInfo = {};
  GetWindowInfo(enigma::hWnd, &winInfo);
  DestroyWindow(enigma::hWnd);
  const RECT& rcWindow = winInfo.rcWindow;
  const HINSTANCE hInstance = GetModuleHandle(NULL);
  enigma::hWnd = CreateWindow(
    "EnigmaDevGameMainWindow", "", winInfo.dwStyle,
    rcWindow.left, rcWindow.top, rcWindow.right-rcWindow.left, rcWindow.bottom-rcWindow.top,
    NULL, NULL, hInstance, NULL
  );
  enigma::window_hDC = GetDC(enigma::hWnd);

  bool valid;
  UINT numFormats;
  float fAttributes[] = {0,0};
  HDC hDC = enigma::window_hDC;
  int iAttributes[] = {
    WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
    WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB,24,
    WGL_ALPHA_BITS_ARB,8,
    WGL_DEPTH_BITS_ARB,24,
    WGL_STENCIL_BITS_ARB,0,
    WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
    WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
    WGL_SAMPLES_ARB, samples,
    0,0
  };
  valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
  if (!valid || !numFormats) return;

  if (SetPixelFormat(hDC,pixelFormat,&pfd) == FALSE) {
    MessageBox(NULL, "bling", "ding", MB_OK);
  }

  HGLRC newrc = wglCreateContextAttribsARB(hDC, enigma::hRC, nullptr);
  wglCopyContext(enigma::hRC, newrc, GL_ALL_ATTRIB_BITS);
  enigma::DisableDrawing(nullptr);
  enigma::hRC = newrc;
  wglMakeCurrent(hDC, enigma::hRC);
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

} // namespace enigma_user
