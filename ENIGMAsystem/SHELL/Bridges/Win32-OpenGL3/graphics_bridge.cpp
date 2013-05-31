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

#include <string>
#include <windows.h>
using namespace std;

#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/Win32/WINDOWSmain.h"
#include "Platforms/Win32/WINDOWSwindow.h"

namespace enigma
{
    void EnableDrawing (HGLRC *hRC)
    {
        PIXELFORMATDESCRIPTOR pfd;
        int iFormat;

        enigma::window_hDC = GetDC (hWnd);
        ZeroMemory (&pfd, sizeof (pfd));
        pfd.nSize = sizeof (pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 16;
        pfd.iLayerType = PFD_MAIN_PLANE;
        iFormat = ChoosePixelFormat (enigma::window_hDC, &pfd);

        if (iFormat==0) { show_error("Total failure. Abort.",1); }

        SetPixelFormat (enigma::window_hDC, iFormat, &pfd);
        *hRC = wglCreateContext( enigma::window_hDC );
        wglMakeCurrent( enigma::window_hDC, *hRC );
    }

    void DisableDrawing (HWND hWnd, HDC hDC, HGLRC hRC)
    {
        wglMakeCurrent (NULL, NULL);
        wglDeleteContext (hRC);
        ReleaseDC (hWnd, hDC);
    }
}

namespace enigma_user
{

void set_synchronization(bool enable) //TODO: Needs to be rewritten
{
 /*   typedef BOOL (APIENTRY *fp)( int );
    fp f = 0;
    const char *extensions = (char*)glGetString(GL_EXTENSIONS);
    if (strstr(extensions, "WGL_EXT_swap_control") and (f = (fp)wglGetProcAddress( "wglSwapIntervalEXT" )))
        f(enable);*/
}  

}

