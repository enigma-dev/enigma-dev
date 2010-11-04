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

#include <windows.h>

namespace enigma
{
    extern char mousestatus[3],last_mousestatus[3],keybdstatus[256],last_keybdstatus[256];
    extern short mousewheel;

    LRESULT CALLBACK WndProc (HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
    {
      switch (message)
      {
        case WM_CREATE:
            return 0;
        case WM_CLOSE:
            PostQuitMessage (0);
            return 0;

        case WM_DESTROY:
            return 0;

        case WM_SIZE:
            return 0;

        case WM_KEYDOWN:
            last_keybdstatus[wParam]=keybdstatus[wParam];
            keybdstatus[wParam]=1;
            return 0;

        case WM_KEYUP:
            last_keybdstatus[wParam]=keybdstatus[wParam];
            keybdstatus[wParam]=0;
            return 0;

        case WM_SYSKEYDOWN:
            last_keybdstatus[wParam]=keybdstatus[wParam];
            keybdstatus[wParam]=1;
            if (wParam!=18)
            {
              if ((lParam&(1<<29))>0)
              { last_keybdstatus[18]=keybdstatus[18]; keybdstatus[18]=1; }
              else { last_keybdstatus[18]=keybdstatus[18]; keybdstatus[18]=0;}
            }
            return 0;

        case WM_SYSKEYUP:
            last_keybdstatus[wParam]=keybdstatus[wParam];
            keybdstatus[wParam]=0;
            if (wParam!=(unsigned int)18)
            {
              if ((lParam&(1<<29))>0)
              { last_keybdstatus[18]=keybdstatus[18]; keybdstatus[18]=0;}
              else { last_keybdstatus[18]=keybdstatus[18]; keybdstatus[18]=1; }
            }
            return 0;

        case WM_MOUSEWHEEL:
             mousewheel += (int)HIWORD(wParam) / 60;
             return 0;

        case WM_LBUTTONUP:   mousestatus[0]=0; return 0;
        case WM_LBUTTONDOWN: mousestatus[0]=1; return 0;
        case WM_RBUTTONUP:   mousestatus[1]=0; return 0;
        case WM_RBUTTONDOWN: mousestatus[1]=1; return 0;
        case WM_MBUTTONUP:   mousestatus[2]=0; return 0;
        case WM_MBUTTONDOWN: mousestatus[2]=1; return 0;

        default:
            return DefWindowProc (hWnd, message, wParam, lParam);
      }
    }
    
    void input_initialize()
    {
      //Clear the input arrays
      for(int i=0;i<3;i++){
        last_mousestatus[i]=0;
        mousestatus[i]=0;
      }
      for(int i=0;i<256;i++){
        last_keybdstatus[i]=0;
        keybdstatus[i]=0;
      }
    }
    
    void input_push()
    {
      for(int i=0;i<3;i++){
        last_mousestatus[i] = mousestatus[i];
      }
      for(int i=0;i<256;i++){
        last_keybdstatus[i] = keybdstatus[i];
      }
      mousewheel = 0;
    }
}
