/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/Win32/WINDOWSmain.h"
#include "Platforms/General/PFwindow.h"
#include "Graphics_Systems/General/GScolors.h"

#include <windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>

namespace enigma
{

extern void (*WindowResizedCallback)();
void WindowResized() {
  // clear the window color, viewport does not need set because backbuffer was just recreated
  enigma_user::draw_clear(enigma_user::window_get_color());
}

extern HGLRC hRC;

void EnableDrawing(void*)
{
  WindowResizedCallback = &WindowResized;

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

  SetPixelFormat (enigma::window_hDC, iFormat, &pfd);
  hRC = wglCreateContext( enigma::window_hDC );
  wglMakeCurrent( enigma::window_hDC, hRC );

  GLenum err = glewInit();
  if (GLEW_OK != err)
    show_error(std::string("Failed to initialize glew for OpenGL. ") + (const char*)glewGetErrorString(err), true);

  //TODO: This never reports higher than 8, but display_aa should be 14 if 2,4,and 8 are supported and 8 only when only 8 is supported
  glGetIntegerv(GL_MAX_SAMPLES_EXT, &enigma_user::display_aa);
}

}
