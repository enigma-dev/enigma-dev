/** Copyright (C) 2008-2017 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton
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

#define NOMINMAX // for windows.h because we use std::min/max
#include "WINDOWSmain.h"

#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/platforms_mandatory.h"

#include "Universal_System/mathnc.h" // enigma_user::clamp
#include "Universal_System/estring.h"
#include "Universal_System/joysticks.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/var4.h"

#include <mmsystem.h>
#include <thread>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>
using std::string;
using std::vector;

using enigma_user::file_exists;
using enigma_user::directory_exists;

namespace enigma_user {

const int os_type = os_windows;
}  // namespace enigma_user

namespace enigma
{

HINSTANCE hInstance;
HWND hWnd;
HDC window_hDC;
HANDLE mainthread;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void (*touch_extension_register)(HWND hWnd);

void Sleep(int ms) { ::Sleep(ms); }

void initInput(){};

} // namespace enigma

static inline string add_slash(const string& dir) {
  if (dir.empty() || dir.back() != '\\') return dir + '\\';
  return dir;
}

namespace enigma {

static LONG_PTR ComputeInitialWindowStyle() {
  LONG_PTR newlong = 0;

  if (showIcons) newlong |= WS_SYSMENU;

  if (isFullScreen) {
    newlong |= WS_POPUP;
  } else if (enigma::showBorder) {
    newlong |= WS_CAPTION | WS_MINIMIZEBOX;
    if (isSizeable) newlong |= WS_SIZEBOX | WS_MAXIMIZEBOX;
  }

  // these two flags are necessary for extensions like Ultimate3D and GMOgre to render on top of the window
  return newlong | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
}

WNDCLASS wcontainer;
HGLRC hRC;
MSG msg;

bool initGameWindow() {
  int wid = (int)enigma_user::room_width, hgt = (int)enigma_user::room_height;
  if (!wid || !hgt) wid = 640, hgt = 480;
  enigma::mainthread = GetCurrentThread();

  //Register window class
  wcontainer.style = CS_OWNDC;
  wcontainer.lpfnWndProc = enigma::WndProc;
  wcontainer.cbClsExtra = 0;
  wcontainer.cbWndExtra = 0;
  wcontainer.hInstance = hInstance;
  wcontainer.hIcon = LoadIcon(hInstance, "IDI_MAIN_ICON");
  wcontainer.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcontainer.hbrBackground = (HBRUSH)CreateSolidBrush(enigma::windowColor);
  wcontainer.lpszMenuName = NULL;
  wcontainer.lpszClassName = "EnigmaDevGameMainWindow";
  RegisterClass(&wcontainer);

  //Create the parent window
  int screen_width = GetSystemMetrics(SM_CXSCREEN);
  int screen_height = GetSystemMetrics(SM_CYSCREEN);
  // By default if the room is too big instead of creating a gigantic ass window
  // make it not bigger than the screen to full screen it, this is what 8.1 and Studio
  // do, if the user wants to manually override this they can using
  // views/screen_set_viewport or window_set_size/window_set_region_size
  // We won't limit those functions like GM, just the default.
  if (wid > screen_width) wid = screen_width;
  if (hgt > screen_height) hgt = screen_height;
  // Create the window initially without the WS_VISIBLE flag until we've loaded all of the resources.
  // This will be handled by game_start in roomsystem.cpp where window_default(true) sets the initial
  // fullscreen state of the window before showing it.
  enigma::hWnd = CreateWindow("EnigmaDevGameMainWindow", "", ComputeInitialWindowStyle(), (screen_width - wid) / 2,
                              (screen_height - hgt) / 2, wid, hgt, NULL, NULL, hInstance, NULL);

  if (enigma::touch_extension_register != NULL) {
    enigma::touch_extension_register(enigma::hWnd);
  }

  return true;
}

int handleEvents() {
  if (enigma::game_isending) PostQuitMessage(enigma::game_return);

  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      enigma::game_return = msg.wParam;
      return 1;
    } else {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return 0;
}

void handleInput() {
  joystick_update(); 
  input_push(); 
}

void destroyWindow() { DestroyWindow(enigma::hWnd); }

}  // namespace enigma

namespace enigma_user {

unsigned long long disk_size(std::string drive) {
  DWORD sectorsPerCluster, bytesPerSector, totalClusters, freeClusters;

  tstring tstr_drive = widen(drive);
  tstring tstr_colonslash = widen(":\\");
  tstring tstr_empty = widen("");

  if (tstr_drive.length() == 1) tstr_drive += tstr_colonslash;

  if (!GetDiskFreeSpaceW((tstr_drive == tstr_empty) ? NULL : tstr_drive.c_str(), &sectorsPerCluster, &bytesPerSector,
                         &freeClusters, &totalClusters))
    return 0;

  return (unsigned long long)(totalClusters * sectorsPerCluster) * (unsigned long long)bytesPerSector;
}

unsigned long long disk_free(std::string drive) {
  DWORD sectorsPerCluster, bytesPerSector, totalClusters, freeClusters;

  tstring tstr_drive = widen(drive);
  tstring tstr_colonslash = widen(":\\");
  tstring tstr_empty = widen("");

  if (tstr_drive.length() == 1) tstr_drive += tstr_colonslash;

  if (!GetDiskFreeSpaceW((tstr_drive == tstr_empty) ? NULL : tstr_drive.c_str(), &sectorsPerCluster, &bytesPerSector,
                         &freeClusters, &totalClusters))
    return 0;

  return ((unsigned long long)(totalClusters * sectorsPerCluster) * (unsigned long long)bytesPerSector) -
         ((unsigned long long)(freeClusters * sectorsPerCluster) * (unsigned long long)bytesPerSector);
}

void set_program_priority(int value) {
  // Need to add PROCESS_SET_INFORMATION permission to thread's access rights, not sure how

  DWORD priorityValue = NORMAL_PRIORITY_CLASS;
  if (value == -1 || value == -2)
    priorityValue = BELOW_NORMAL_PRIORITY_CLASS;
  else if (value == -3)
    priorityValue = IDLE_PRIORITY_CLASS;
  else if (value == 1)
    priorityValue = ABOVE_NORMAL_PRIORITY_CLASS;
  else if (value == 2)
    priorityValue = HIGH_PRIORITY_CLASS;
  else if (value == 3)
    priorityValue = REALTIME_PRIORITY_CLASS;

  SetPriorityClass(GetCurrentThread(), priorityValue);
}

// converts a relative path to absolute if the path exists
std::string filename_absolute(std::string fname) {
  if (string_replace_all(fname, " ", "") == "") fname = ".";
  wchar_t rpath[MAX_PATH];
  tstring tstr_fname = widen(fname);
  tstring result(rpath, GetFullPathNameW(tstr_fname.c_str(), MAX_PATH, rpath, NULL));
  if (directory_exists(shorten(result))) return add_slash(shorten(result));
  if (file_exists(shorten(result))) return shorten(result);
  return "";
}

std::string filename_join(std::string prefix, std::string suffix) {
  return add_slash(prefix) + suffix;
}

}  // namespace enigma_user

int main(int argc, char *argv[]) {
  // DO NOT move this out of WinMain like a fucking idiot
  // we are setting the hInstance in the enigma namespace
  // to the local one passed as a parameter here so that
  // we can use it for the CreateWindow call and icon
  enigma::hInstance = GetModuleHandle(0);

  int cnt = 0;
  std::vector<const char*> vec;
  std::vector<string> shortened;
  if (wchar_t **_vec = CommandLineToArgvW(GetCommandLineW(), &cnt)) {
    for (int i = 0; i < cnt; ++i) {
      shortened.push_back(shorten(_vec[i]));
      vec.push_back(shortened[i].c_str());
    }
    LocalFree(_vec);
  }

  //Main loop
  return enigma::enigma_main(cnt, const_cast<char**>(vec.data()));
}
