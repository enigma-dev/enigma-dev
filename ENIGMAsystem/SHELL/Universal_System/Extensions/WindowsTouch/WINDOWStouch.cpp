/** Copyright (C) 2014 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY {
} without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/
#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include "WINDOWStouch.h"

namespace enigma {

  LRESULT CALLBACK windows_touch_extension_callback(HWND hWndParameter, UINT message,WPARAM wParam, LPARAM lParam) {
    switch (message) {
    
    }
    return DefWindowProc(hWndParameter, message, wParam, lParam);
  }
  
  void windows_touch_extension_register(HWND hWnd) {
    MessageBox(hWnd, "Just a test", "Hello, world!", MB_OK);
  }

  //extern LRESULT CALLBACK (*touch_extension_callback)(HWND hWndParameter, UINT message,WPARAM wParam, LPARAM lParam) = &windows_touch_extension_callback;
  //extern void (*touch_extension_register)(HWND hWnd) = &windows_touch_extension_register;
}

namespace enigma_user {

}
