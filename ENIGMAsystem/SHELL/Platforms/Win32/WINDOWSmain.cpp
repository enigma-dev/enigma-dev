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
#include <time.h>
#include <string>
using std::string;

#include "WINDOWScallback.h"
#include "../../Universal_System/var4.h"
#include "../../Universal_System/roomsystem.h"
#include "WINDOWSwindow.h"

namespace enigma //TODO: Find where this belongs
{
    HWND hWndParent;
    HWND hWnd;
    LRESULT CALLBACK WndProc (HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
    HDC window_hDC;

    int windowcolor; double viewscale; bool windowIsTop;

    void EnableDrawing (HGLRC *hRC);
    void DisableDrawing (HWND hWnd, HDC hDC, HGLRC hRC);

    #ifdef ENIGMA_GS_OPENGL
    void EnableDrawing (HGLRC *hRC);
    void DisableDrawing (HWND hWnd, HDC hDC, HGLRC hRC);
    #endif
}

namespace enigma {
  int initialize_everything();
  int ENIGMA_events();
} // TODO: synchronize with XLib by moving these declarations to a platform_includes header in the root.

extern double fps;
namespace enigma {
  clock_t lc;
  void sleep_for_framerate(int rs)
  {
    clock_t nc = clock();
    int sdur = 1000/rs - 1 - (nc - lc)*1000 / CLOCKS_PER_SEC;
    if (sdur > 0)
    {
        Sleep(sdur);
        fps = room_speed;
    }
    else
    {
        fps = CLOCKS_PER_SEC / (nc - lc);
        if(fps > room_speed){ fps = room_speed; }
    }
    lc = nc;
  }
}

int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iCmdShow)
{
    int wid=(int)room_width, hgt=(int)room_height;

    //Create the window
        WNDCLASS wcontainer,wmain;
        HGLRC hRC;
        MSG msg;

        //Register window class
        wcontainer.style = CS_OWNDC;
        wcontainer.lpfnWndProc = enigma::WndProc;
        wcontainer.cbClsExtra = 0;
        wcontainer.cbWndExtra = 0;
        wcontainer.hInstance = hInstance;
        wcontainer.hIcon = LoadIcon (NULL, IDI_APPLICATION);
        wcontainer.hCursor = LoadCursor (NULL, IDC_ARROW);
        wcontainer.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
        wcontainer.lpszMenuName = NULL;
        wcontainer.lpszClassName = "TMain";
        RegisterClass (&wcontainer);

        //Register other window class
        wmain.style = 0;
        wmain.lpfnWndProc = enigma::WndProc;
        wmain.cbClsExtra = 0;
        wmain.cbWndExtra = 0;
        wmain.hInstance = hInstance;
        wmain.hIcon = LoadIcon (NULL, IDI_APPLICATION);
        wmain.hCursor = LoadCursor (NULL, IDC_ARROW);
        wmain.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
        wmain.lpszMenuName = NULL;
        wmain.lpszClassName = "TSub";
        RegisterClass (&wmain);


        //Create the parent window
        int screen_width = GetSystemMetrics(SM_CXSCREEN);
        int screen_height = GetSystemMetrics(SM_CYSCREEN);
        // TODO: Implement minimize button on both windows like GM
        if(resizeableWindow) {
            // TODO: Implement maximize button here like GM
            enigma::hWndParent = CreateWindow ("TMain", "ENIGMA Shell", WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE | WS_SIZEBOX, (screen_width-wid)/2, (screen_height-hgt)/2, wid, hgt,
                NULL, NULL, hInstance, NULL);
        } else {
            enigma::hWndParent = CreateWindow ("TMain", "ENIGMA Shell", WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE, (screen_width-wid)/2, (screen_height-hgt)/2, wid, hgt,
                NULL, NULL, hInstance, NULL);
        }

        //Create a child window to put into that
        enigma::hWnd = CreateWindow ("TSub", "", WS_VISIBLE | WS_CHILD,0, 0, wid, hgt,enigma::hWndParent, NULL, hInstance, NULL);


    enigma::EnableDrawing (&hRC);
    enigma::initialize_everything();

    //Main loop
        char bQuit=0;
        while (!bQuit)
        {
            if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    bQuit=1;
                    break;
                }
                else
                {
                    TranslateMessage (&msg);
                    DispatchMessage (&msg);
                }
            }
            else
            {
                enigma::ENIGMA_events();
                enigma::input_push();
            }
        }


    enigma::DisableDrawing (enigma::hWnd, enigma::window_hDC, hRC);
    DestroyWindow (enigma::hWnd);

    #if BUILDMODE
    buildmode::finishup();
    #endif

    return 0;
}
