/** Copyright (C) 2008-2017 Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
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

#include <map>
#include <string>
using std::string;
using std::map;
#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

//#include <winuser.h> // includes windows.h

#include "Platforms/General/PFmain.h" // for keyboard_string
#include "Platforms/General/PFwindow.h" // For those damn vk_ constants.
#include "Universal_System/Instances/instance_system.h"
#include "Universal_System/Instances/instance.h"

#include "Platforms/platforms_mandatory.h"

#ifndef WM_MOUSEHWHEEL
  #define WM_MOUSEHWHEEL 0x020E
#endif

namespace enigma_user {
void draw_clear(int col);
void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height);
} // namespace enigma_user

namespace enigma
{
  extern HWND hWnd;
  extern HDC window_hDC;

  using enigma_user::keyboard_key;
  using enigma_user::keyboard_lastkey;
  using enigma_user::keyboard_lastchar;
  using enigma_user::keyboard_string;

  extern char mousestatus[3],last_mousestatus[3],keybdstatus[256],last_keybdstatus[256];
  extern int windowX, windowY, windowColor;
  extern HCURSOR currentCursor;

  static RECT tempWindow;
  static short hdeltadelta = 0, vdeltadelta = 0;
  static int tempLeft = 0, tempTop = 0, tempRight = 0, tempBottom = 0, tempWidth, tempHeight;

  LRESULT (CALLBACK *touch_extension_callback)(HWND hWndParameter, UINT message, WPARAM wParam, LPARAM lParam);
  void (*WindowResizedCallback)();

  LRESULT CALLBACK WndProc (HWND hWndParameter, UINT message,WPARAM wParam, LPARAM lParam)
  {
    switch (message)
    {
      case WM_SHOWWINDOW:
        enigma::windowWidth = enigma_user::window_get_width();
        enigma::windowHeight = enigma_user::window_get_height();
        enigma::compute_window_scaling();
        return 0;
      case WM_CLOSE:
        instance_event_iterator = &dummy_event_iterator;
        for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
        {
          it->myevent_closebutton();
        }
        // Game Maker actually checks this first I am making the decision to check if after, since that is how it is expected to work
        // so the user can execute something before the escape is processed, no sense in an override if user is going to call game_end() anyway.
        // - Robert
        if (treatCloseAsEscape) {
          PostQuitMessage(game_return);
        }
        return 0;

      case WM_SETFOCUS:
        input_initialize();
        game_window_focused = true;
        pausedSteps = 0;
        return 0;

      case WM_KILLFOCUS:
        for (int i = 0; i < 255; i++)
        {
            last_keybdstatus[i] = keybdstatus[i];
            keybdstatus[i] = 0;
        }
        for(int i=0; i < 3; i++)
        {
            last_mousestatus[i] = mousestatus[i];
            mousestatus[i] = 0;
        }
        game_window_focused = false;
        return 0;

      case WM_SIZE:
        // make sure window resized is only processed once per resize because there could possibly be child windows and handles, especially with widgets
        if (hWndParameter == hWnd) {
          if (WindowResizedCallback != NULL) {
            WindowResizedCallback();
            windowWidth = enigma_user::window_get_width();
            windowHeight = enigma_user::window_get_height();
            enigma::compute_window_scaling();
          }
          instance_event_iterator = &dummy_event_iterator;
          for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
          {
            ((object_graphics*)*it)->myevent_drawresize();
          }
        } else {
          DefWindowProc(hWndParameter, message, wParam, lParam);
        }
        return 0;

      case WM_ENTERSIZEMOVE:
        GetWindowRect(hWndParameter,&tempWindow);
        tempLeft = tempWindow.left;
        tempTop = tempWindow.top;
        tempRight = tempWindow.right;
        tempBottom = tempWindow.bottom;
        return 0;

      case WM_EXITSIZEMOVE:
        GetWindowRect(hWndParameter,&tempWindow);
        tempWidth = windowWidth + (tempWindow.right - tempWindow.left) - (tempRight - tempLeft);
        tempHeight = windowHeight + (tempWindow.bottom - tempWindow.top) - (tempBottom - tempTop);

        windowX += tempWindow.left - tempLeft;
        windowY += tempWindow.top - tempTop;
        windowWidth = tempWidth;
        windowHeight = tempHeight;
        enigma::windowWidth = enigma_user::window_get_width();
        enigma::windowHeight = enigma_user::window_get_height();
        enigma::compute_window_scaling();
        return 0;

      case WM_GETMINMAXINFO: {
        if (viewScale > 0) { //Fixed Scale, this is GM8.1 behaviour
          RECT c;
          c.left = 0; c.top = 0; c.right = scaledWidth; c.bottom = scaledHeight;
          AdjustWindowRect(&c, GetWindowLongPtr(enigma::hWnd, GWL_EXSTYLE), false);

          LPMINMAXINFO lpMinMaxInfo = (LPMINMAXINFO) lParam;
          lpMinMaxInfo->ptMinTrackSize.x = c.right-c.left;
          lpMinMaxInfo->ptMinTrackSize.y = c.bottom-c.top;
        }
        break;
      }

      case WM_SETCURSOR:
        // Set the user cursor if the mouse is in the client area of the window, otherwise let Windows handle setting the cursor
        // since it knows how to set the gripper cursor for window resizing. This is exactly how GM handles it.
        if (LOWORD(lParam) == HTCLIENT) {
          SetCursor(currentCursor);
          return TRUE;
        }
        break;
      case WM_CHAR:
        keyboard_lastchar = string(1,wParam);
        if (keyboard_lastkey == enigma_user::vk_backspace) {
          if (!keyboard_string.empty()) keyboard_string.pop_back();
        } else {
          keyboard_string += keyboard_lastchar;
        }
        return 0;

      case WM_KEYDOWN: {
        int key = enigma_user::keyboard_get_map(wParam);
        keyboard_lastkey = key;
        keyboard_key = key;
        last_keybdstatus[key]=keybdstatus[key];
        keybdstatus[key]=1;
        return 0;
      }
      case WM_KEYUP: {
        int key = enigma_user::keyboard_get_map(wParam);
        keyboard_key = 0;
        last_keybdstatus[key]=keybdstatus[key];
        keybdstatus[key]=0;
        return 0;
      }
      case WM_SYSKEYDOWN: {
        int key = enigma_user::keyboard_get_map(wParam);
        keyboard_lastkey = key;
        keyboard_key = key;
        last_keybdstatus[key]=keybdstatus[key];
        keybdstatus[key]=1;
        if (key!=18)
        {
          if ((lParam&(1<<29))>0)
               last_keybdstatus[18]=keybdstatus[18], keybdstatus[18]=1;
          else last_keybdstatus[18]=keybdstatus[18], keybdstatus[18]=0;
        }
        return 0;
      }
      case WM_SYSKEYUP: {
        int key = enigma_user::keyboard_get_map(wParam);
        keyboard_key = 0;
        last_keybdstatus[key]=keybdstatus[key];
        keybdstatus[key]=0;
        if (key!=(unsigned int)18)
        {
          if ((lParam&(1<<29))>0)
               last_keybdstatus[18]=keybdstatus[18], keybdstatus[18]=0;
          else last_keybdstatus[18]=keybdstatus[18], keybdstatus[18]=1;
        }
        return 0;
      }
      case WM_MOUSEWHEEL:
         vdeltadelta += (int)HIWORD(wParam);
         enigma_user::mouse_vscrolls += vdeltadelta / WHEEL_DELTA;
         vdeltadelta %= WHEEL_DELTA;
         return 0;

      case WM_MOUSEHWHEEL:
         hdeltadelta += (int)HIWORD(wParam);
         enigma_user::mouse_hscrolls += hdeltadelta / WHEEL_DELTA;
         hdeltadelta %= WHEEL_DELTA;
         return 0;

      case WM_LBUTTONUP:   { if (!wParam) ReleaseCapture(); mousestatus[0]=0; return 0; }
      case WM_LBUTTONDOWN: SetCapture(hWnd); mousestatus[0]=1; return 0;
      case WM_RBUTTONUP:   { if (!wParam) ReleaseCapture(); mousestatus[1]=0; return 0; }
      case WM_RBUTTONDOWN: SetCapture(hWnd); mousestatus[1]=1; return 0;
      case WM_MBUTTONUP:   { if (!wParam) ReleaseCapture(); mousestatus[2]=0; return 0; }
      case WM_MBUTTONDOWN: SetCapture(hWnd); mousestatus[2]=1; return 0;

      case WM_ERASEBKGND:
        RECT rc;
        GetClientRect(hWnd, &rc);
        FillRect((HDC) wParam, &rc, CreateSolidBrush(windowColor));
        return 1L;

      case WM_SYSCOMMAND: {
        if (wParam == SC_MAXIMIZE) {
          ShowWindow(hWnd, SW_MAXIMIZE);
          enigma::windowWidth = enigma_user::window_get_width();
          enigma::windowHeight = enigma_user::window_get_height();
          enigma::compute_window_scaling();
        }
        break;
      }
    }
    return DefWindowProc (hWndParameter, message, wParam, lParam);
  }
}
