/** Copyright (C) 2011, 2017 Josh Ventura
*** Copyright (C) 2014, 2017 Robert B. Colton
*** Copyright (C) 2018 Samuel Venable
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

#include <windows.h>
#include <shlwapi.h> //for Shell API
#include <shlobj.h> //for Shell API
#include <richedit.h>
#include <stdio.h>
#include <string>
#include <commdlg.h>
#include <comutil.h>
#include <wchar.h>
#include <dlgs.h>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;
#include "Platforms/Win32/WINDOWSmain.h"
#include "Platforms/General/PFwindow.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/estring.h"
#include "GameSettings.h"

using enigma_user::string_replace_all;
using enigma_user::window_get_caption;

#include "../General/WSdialogs.h"
#include "resources.h"

#include "Graphics_Systems/General/GScolor_macros.h"

static string gs_cap;
static string gs_def;
static string gs_message;
static string gs_username;
static string gs_password;
static bool   gs_form_canceled;
static string gs_str_submitted;
static string gs_but1, gs_but2, gs_but3;

#ifdef DEBUG_MODE
#include "Universal_System/var4.h"
#include "Universal_System/resource_data.h"
#include "Universal_System/object.h"
#include "Universal_System/debugscope.h"
#endif

void show_error(string errortext, const bool fatal) {
  #ifdef DEBUG_MODE
    errortext += enigma::debug_scope::GetErrors();
  #else
    errortext = "Error in some event or another for some object: \r\n" + errortext;
  #endif

  string strStr = errortext;
  string strWindowCaption = "Error";

  if (strStr != "")
    strStr = strStr + "\n\n";

  if (!fatal)
    strStr = strStr + "Do you want to abort the application?";
  else
    strStr = strStr + "Click 'OK' to abort the application.";

  tstring tstrStr = widen(strStr);
  tstring tstrWindowCaption = widen(strWindowCaption);

  double result;

  DWORD flags = MB_ICONERROR | MB_DEFBUTTON1 | MB_APPLMODAL;
  result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), flags | (fatal ? MB_OK : MB_YESNO));
  printf(fatal ? "FATAL ERROR: %s\n" : "ERROR: %s\n", errortext.c_str());

  if (result == IDOK || result == IDYES)
    exit(0);
}

namespace enigma {
  extern HINSTANCE hInstance;
  extern HWND hWnd;
  HWND infore;
}

static INT_PTR CALLBACK ShowInfoProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_KEYUP) {
    switch (wParam) {
      case VK_ESCAPE:
          PostMessage(hwndDlg, WM_CLOSE, 0, 0);
        return TRUE;
    }
  }

  if (uMsg == WM_SIZE) {
    RECT rectParent;
    GetClientRect(hwndDlg, &rectParent);
    MoveWindow(enigma::infore, rectParent.top, rectParent.left, rectParent.right, rectParent.bottom, TRUE);
  }

  return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
}

/* < Used by GetDirectoryProc, InputBoxProc, and GetColorProc > */
#define MONITOR_CENTER     0x0001

void CenterRectToMonitor(LPRECT prc, UINT flags)
{
  HMONITOR hMonitor;
  MONITORINFO mi;
  RECT        rc;
  int         w = prc->right - prc->left;
  int         h = prc->bottom - prc->top;

  hMonitor = MonitorFromRect(prc, MONITOR_DEFAULTTONEAREST);

  mi.cbSize = sizeof(mi);
  GetMonitorInfo(hMonitor, &mi);
  rc = mi.rcMonitor;

  if (flags & MONITOR_CENTER)
  {
    prc->left = rc.left + (rc.right - rc.left - w) / 2;
    prc->top = rc.top + (rc.bottom - rc.top - h) / 2;
    prc->right = prc->left + w;
    prc->bottom = prc->top + h;
  }
  else
  {
    prc->left = rc.left + (rc.right - rc.left - w) / 2;
    prc->top = rc.top + (rc.bottom - rc.top - h) / 3;
    prc->right = prc->left + w;
    prc->bottom = prc->top + h;
  }
}

void CenterWindowToMonitor(HWND hwnd, UINT flags)
{
  RECT rc;
  GetWindowRect(hwnd, &rc);
  CenterRectToMonitor(&rc, flags);
  SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
/* < / Used by GetDirectoryProc, InputBoxProc, and GetColorProc > */

/* < Used by GetDirectoryProc and InputBoxProc > */
int ScaleToDpiPercentX(double PixelX)
{
  HDC screen = GetDC(NULL);
  double hPixelsPerInch = GetDeviceCaps(screen, LOGPIXELSX);
  double result = PixelX * (hPixelsPerInch / 96);
  ReleaseDC(NULL, screen);

  return (int)result;
}

int ScaleToDpiPercentY(double PixelY)
{
  HDC screen = GetDC(NULL);
  double vPixelsPerInch = GetDeviceCaps(screen, LOGPIXELSY);
  double result = PixelY * (vPixelsPerInch / 96);
  ReleaseDC(NULL, screen);

  return (int)result;
}
/* < / Used by GetDirectoryProc and InputBoxProc > */

/* < Used by InputBoxProc > */
void ClientResize(HWND hWnd, int nWidth, int nHeight) {
  RECT rcClient, rcWind;
  POINT ptDiff;
  GetClientRect(hWnd, &rcClient);
  GetWindowRect(hWnd, &rcWind);
  ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
  ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
  MoveWindow(hWnd, rcWind.left, rcWind.top, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
}

WCHAR wstrPromptStr[4096];
WCHAR wstrTextEntry[MAX_PATH + 1];
bool HideInput = 0;
/* < / Used by InputBoxProc > */

/* < Used by get_string, get_password, get_integer, and get_passcode > */
LRESULT CALLBACK InputBoxProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam) {
  switch (Msg) {
  case WM_INITDIALOG: {
      SetWindowLongPtr(hWndDlg, GWL_EXSTYLE, GetWindowLongPtr(hWndDlg, GWL_EXSTYLE) | WS_EX_DLGMODALFRAME);
      SetWindowPos(hWndDlg, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
      ClientResize(hWndDlg, ScaleToDpiPercentX(357), ScaleToDpiPercentY(128));
      RECT rect;
      GetWindowRect(hWndDlg, &rect);
      MoveWindow(hWndDlg, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
      CenterWindowToMonitor(hWndDlg, 0);
      MoveWindow(GetDlgItem(hWndDlg, IDOK), ScaleToDpiPercentX(272), ScaleToDpiPercentY(10), ScaleToDpiPercentX(75), ScaleToDpiPercentY(23), TRUE);
      MoveWindow(GetDlgItem(hWndDlg, IDCANCEL), ScaleToDpiPercentX(272), ScaleToDpiPercentY(39), ScaleToDpiPercentX(75), ScaleToDpiPercentY(23), TRUE);
      MoveWindow(GetDlgItem(hWndDlg, IDC_EDIT), ScaleToDpiPercentX(11), ScaleToDpiPercentY(94), ScaleToDpiPercentX(336), ScaleToDpiPercentY(23), TRUE);
      MoveWindow(GetDlgItem(hWndDlg, IDC_PROMPT), ScaleToDpiPercentX(11), ScaleToDpiPercentY(11), ScaleToDpiPercentX(252), ScaleToDpiPercentY(66), TRUE);
      SetDlgItemTextW(hWndDlg, IDC_PROMPT, wstrPromptStr);
      SetDlgItemTextW(hWndDlg, IDC_EDIT, wstrTextEntry);
      WCHAR wstrWindowCaption[MAX_PATH + 1];
      tstring tstrWindowCaption = widen(gs_cap);
      wcsncpy(wstrWindowCaption, tstrWindowCaption.c_str(), MAX_PATH + 1);
      SetWindowTextW(hWndDlg, wstrWindowCaption);
      if (HideInput == 1)
        SendDlgItemMessage(hWndDlg, IDC_EDIT, EM_SETPASSWORDCHAR, '*', 0);
      SendDlgItemMessage(hWndDlg, IDC_EDIT, EM_SETSEL, '*', 0);
      SendDlgItemMessage(hWndDlg, IDC_EDIT, WM_SETFOCUS, 0, 0);
      return TRUE;
    }
  break;

  case WM_DPICHANGED: {
    ClientResize(hWndDlg, ScaleToDpiPercentX(357), ScaleToDpiPercentY(128));
    RECT rect;
    GetWindowRect(hWndDlg, &rect);
    MoveWindow(hWndDlg, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    MoveWindow(GetDlgItem(hWndDlg, IDOK), ScaleToDpiPercentX(272), ScaleToDpiPercentY(10), ScaleToDpiPercentX(75), ScaleToDpiPercentY(23), TRUE);
    MoveWindow(GetDlgItem(hWndDlg, IDCANCEL), ScaleToDpiPercentX(272), ScaleToDpiPercentY(39), ScaleToDpiPercentX(75), ScaleToDpiPercentY(23), TRUE);
    MoveWindow(GetDlgItem(hWndDlg, IDC_EDIT), ScaleToDpiPercentX(11), ScaleToDpiPercentY(94), ScaleToDpiPercentX(336), ScaleToDpiPercentY(23), TRUE);
    MoveWindow(GetDlgItem(hWndDlg, IDC_PROMPT), ScaleToDpiPercentX(11), ScaleToDpiPercentY(11), ScaleToDpiPercentX(252), ScaleToDpiPercentY(66), TRUE);
  }

  case WM_COMMAND:
    switch (wParam) {
    case IDOK:
      GetDlgItemTextW(hWndDlg, IDC_EDIT, wstrTextEntry, MAX_PATH + 1);
      gs_form_canceled = 0;
      EndDialog(hWndDlg, 0);
      return TRUE;
    case IDCANCEL:
      tstring tstrEmpty = widen("");
      wcsncpy(wstrTextEntry, tstrEmpty.c_str(), MAX_PATH + 1);
      gs_form_canceled = 1;
      EndDialog(hWndDlg, 0);
      return TRUE;
    }
    break;
  }

  return FALSE;
}
/* < / Used by get_string, get_password, get_integer, and get_passcode > */

static INT_PTR CALLBACK GetLoginProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_INITDIALOG) {
    SetWindowText(hwndDlg, gs_cap.c_str());
    SetDlgItemText(hwndDlg, 14, gs_username.c_str());
    SetDlgItemText(hwndDlg, 15, gs_password.c_str());
  }

  if (uMsg == WM_COMMAND) {
    if (wParam == 2 || wParam == 11) {
      gs_str_submitted = "";
      gs_form_canceled = 1;
      EndDialog(hwndDlg, 1);
    } else if (wParam == 10) {
      char strget[1024];
      GetDlgItemText(hwndDlg, 14, strget, 1024);
      gs_str_submitted = strget;
      GetDlgItemText(hwndDlg, 15, strget, 1024);
      gs_str_submitted += string(1, 0) + string(strget);
      gs_form_canceled = 0;
      EndDialog(hwndDlg, 2);
    }
  }

  return 0;
}

static INT_PTR CALLBACK ShowMessageExtProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_INITDIALOG) {
    tstring tstr_cap = widen(gs_cap);
    tstring tstr_msg = widen(gs_message);
    tstring tstr_but1 = widen(gs_but1);
    tstring tstr_but2 = widen(gs_but2);
    tstring tstr_but3 = widen(gs_but1);
    SetWindowTextW(hwndDlg, tstr_cap.c_str());
    SetDlgItemTextW(hwndDlg, 10, tstr_msg.c_str());
    SetDlgItemTextW(hwndDlg, 11, tstr_but1.c_str());
    SetDlgItemTextW(hwndDlg, 12, tstr_but2.c_str());
    SetDlgItemTextW(hwndDlg, 13, tstr_but3.c_str());
  }

  if (uMsg == WM_COMMAND) {
    if (wParam == 2) {
      gs_str_submitted = "";
      gs_form_canceled = 1;
      EndDialog(hwndDlg, 0);
    } else if (wParam == 11) {
      gs_str_submitted = "";
      gs_form_canceled = 0;
      EndDialog(hwndDlg, 1);
    } else if (wParam == 12) {
      gs_str_submitted = "";
      gs_form_canceled = 0;
      EndDialog(hwndDlg, 2);
    } else if (wParam == 13) {
      gs_str_submitted = "";
      gs_form_canceled = 0;
      EndDialog(hwndDlg, 3);
    }
  }

  if (uMsg == WM_KEYUP) {
    switch (wParam) {
      case VK_ESCAPE:
          gs_str_submitted = "";
          gs_form_canceled = 1;
          EndDialog(hwndDlg, 0);
        break;
    }
  }

  return 0;
}

/* < Used by GetDirectoryProc > */
WCHAR *LowerCaseToActualPathName(WCHAR *wstr_dname) {
  LPITEMIDLIST pstr_dname;
  SHParseDisplayName(wstr_dname, 0, &pstr_dname, 0, 0);

  static WCHAR wstr_result[MAX_PATH + 1];
  SHGetPathFromIDListW(pstr_dname, wstr_result);
  return wstr_result;
}

WCHAR wstr_dname[MAX_PATH + 1];
WCHAR wstr_stc1[MAX_PATH + 1];
static string DlgItemText;
tstring ActualPath;
/* < / Used by GetDirectoryProc > */

UINT APIENTRY GetDirectoryProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_INITDIALOG: {
      ClientResize(hWnd, ScaleToDpiPercentX(424), ScaleToDpiPercentY(255));
      RECT rect;
      GetWindowRect(hWnd, &rect);
      MoveWindow(hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
      CenterWindowToMonitor(hWnd, MONITOR_CENTER);
      MoveWindow(GetDlgItem(hWnd, IDOK), ScaleToDpiPercentX(256), ScaleToDpiPercentY(224), ScaleToDpiPercentX(77), ScaleToDpiPercentY(27), TRUE);
      MoveWindow(GetDlgItem(hWnd, IDCANCEL), ScaleToDpiPercentX(340), ScaleToDpiPercentY(224), ScaleToDpiPercentX(77), ScaleToDpiPercentY(27), TRUE);
      MoveWindow(GetDlgItem(hWnd, stc3), ScaleToDpiPercentX(232), ScaleToDpiPercentY(56), ScaleToDpiPercentX(72), ScaleToDpiPercentY(16), TRUE);
      MoveWindow(GetDlgItem(hWnd, IDC_LABEL), ScaleToDpiPercentX(8), ScaleToDpiPercentY(56), ScaleToDpiPercentX(72), ScaleToDpiPercentY(16), TRUE);
      MoveWindow(GetDlgItem(hWnd, stc2), ScaleToDpiPercentX(8), ScaleToDpiPercentY(8), ScaleToDpiPercentX(93), ScaleToDpiPercentY(16), TRUE);
      MoveWindow(GetDlgItem(hWnd, stc4), ScaleToDpiPercentX(232), ScaleToDpiPercentY(176), ScaleToDpiPercentX(50), ScaleToDpiPercentY(16), TRUE);
      MoveWindow(GetDlgItem(hWnd, stc1), ScaleToDpiPercentX(8), ScaleToDpiPercentY(24), ScaleToDpiPercentX(409 * 100), ScaleToDpiPercentY(16), TRUE);
      MoveWindow(GetDlgItem(hWnd, IDC_DNAME), ScaleToDpiPercentX(8), ScaleToDpiPercentY(24), ScaleToDpiPercentX(409), ScaleToDpiPercentY(16), TRUE);
      MoveWindow(GetDlgItem(hWnd, lst1), ScaleToDpiPercentX(232), ScaleToDpiPercentY(72), ScaleToDpiPercentX(185), ScaleToDpiPercentY(93), TRUE);
      MoveWindow(GetDlgItem(hWnd, lst2), ScaleToDpiPercentX(8), ScaleToDpiPercentY(72), ScaleToDpiPercentX(213), ScaleToDpiPercentY(123), TRUE);
      MoveWindow(GetDlgItem(hWnd, cmb2), ScaleToDpiPercentX(232), ScaleToDpiPercentY(192), ScaleToDpiPercentX(185), ScaleToDpiPercentY(332), TRUE);
      ShowWindow(GetDlgItem(hWnd, stc1), SW_HIDE);
      DlgDirListW(hWnd, wstr_dname, lst1, stc1, DDL_ARCHIVE | DDL_READWRITE | DDL_READONLY);
      GetDlgItemTextW(hWnd, stc1, wstr_stc1, MAX_PATH + 1);
      static string DlgItemText;
      DlgItemText = shorten(LowerCaseToActualPathName(wstr_stc1));
      ActualPath = widen(string_replace_all(DlgItemText + "\\", "\\\\", "\\"));
      SetDlgItemTextW(hWnd, IDC_DNAME, ActualPath.c_str());
      PostMessageW(hWnd, WM_SETFOCUS, 0, 0);
    }
  break;

  case WM_DPICHANGED: {
    ClientResize(hWnd, ScaleToDpiPercentX(424), ScaleToDpiPercentY(255));
    RECT rect;
    GetWindowRect(hWnd, &rect);
    MoveWindow(hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    MoveWindow(GetDlgItem(hWnd, IDOK), ScaleToDpiPercentX(256), ScaleToDpiPercentY(224), ScaleToDpiPercentX(77), ScaleToDpiPercentY(27), TRUE);
    MoveWindow(GetDlgItem(hWnd, IDCANCEL), ScaleToDpiPercentX(340), ScaleToDpiPercentY(224), ScaleToDpiPercentX(77), ScaleToDpiPercentY(27), TRUE);
    MoveWindow(GetDlgItem(hWnd, stc3), ScaleToDpiPercentX(232), ScaleToDpiPercentY(56), ScaleToDpiPercentX(72), ScaleToDpiPercentY(16), TRUE);
    MoveWindow(GetDlgItem(hWnd, IDC_LABEL), ScaleToDpiPercentX(8), ScaleToDpiPercentY(56), ScaleToDpiPercentX(72), ScaleToDpiPercentY(16), TRUE);
    MoveWindow(GetDlgItem(hWnd, stc2), ScaleToDpiPercentX(8), ScaleToDpiPercentY(8), ScaleToDpiPercentX(93), ScaleToDpiPercentY(16), TRUE);
    MoveWindow(GetDlgItem(hWnd, stc4), ScaleToDpiPercentX(232), ScaleToDpiPercentY(176), ScaleToDpiPercentX(50), ScaleToDpiPercentY(16), TRUE);
    MoveWindow(GetDlgItem(hWnd, stc1), ScaleToDpiPercentX(8), ScaleToDpiPercentY(24), ScaleToDpiPercentX(409 * 100), ScaleToDpiPercentY(16), TRUE);
    MoveWindow(GetDlgItem(hWnd, IDC_DNAME), ScaleToDpiPercentX(8), ScaleToDpiPercentY(24), ScaleToDpiPercentX(409), ScaleToDpiPercentY(16), TRUE);
    MoveWindow(GetDlgItem(hWnd, lst1), ScaleToDpiPercentX(232), ScaleToDpiPercentY(72), ScaleToDpiPercentX(185), ScaleToDpiPercentY(93), TRUE);
    MoveWindow(GetDlgItem(hWnd, lst2), ScaleToDpiPercentX(8), ScaleToDpiPercentY(72), ScaleToDpiPercentX(213), ScaleToDpiPercentY(123), TRUE);
    MoveWindow(GetDlgItem(hWnd, cmb2), ScaleToDpiPercentX(232), ScaleToDpiPercentY(192), ScaleToDpiPercentX(185), ScaleToDpiPercentY(332), TRUE);
  }

  case WM_CREATE: {
      SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLongPtr(hWnd, GWL_STYLE) | DS_FIXEDSYS);
      SetWindowPos(hWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
    }

  case WM_PAINT: {
      GetDlgItemTextW(hWnd, stc1, wstr_stc1, MAX_PATH + 1);
      static string DlgItemText;
      DlgItemText = shorten(LowerCaseToActualPathName(wstr_stc1));
      ActualPath = widen(string_replace_all(DlgItemText + "\\", "\\\\", "\\"));
      SetDlgItemTextW(hWnd, IDC_DNAME, ActualPath.c_str());
    }
  break;

  case WM_COMMAND: {
      if (HIWORD(wParam) == LBN_DBLCLK && LOWORD(wParam) == lst1)
        return TRUE;
      else if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDOK) {
        wcsncpy(wstr_dname, ActualPath.c_str(), MAX_PATH + 1);
        PostMessageW(hWnd, WM_COMMAND, IDABORT, 0);
        return TRUE;
      } else if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDCANCEL) {
        tstring tstr_dname = widen("");
        wcsncpy(wstr_dname, tstr_dname.c_str(), MAX_PATH + 1);
        PostMessageW(hWnd, WM_COMMAND, IDABORT, 0);
        return TRUE;
      }
    }
  break;

  case WM_CLOSE: {
      tstring tstr_dname = widen("");
      wcsncpy(wstr_dname, tstr_dname.c_str(), MAX_PATH + 1);
      PostMessageW(hWnd, WM_COMMAND, IDABORT, 0);
      return TRUE;
    }
  break;
  }

  return FALSE;
}

static INT CALLBACK GetDirectoryAltProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData) {
  if (uMsg == BFFM_INITIALIZED) {
    tstring tstr_cap = widen(gs_cap);
    SetWindowTextW(hwnd, tstr_cap.c_str());
  }

  return 0;
}

UINT_PTR CALLBACK GetColorProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  if (uiMsg == WM_INITDIALOG) {
    CenterWindowToMonitor(hdlg, 0);
    tstring tstr_cap = widen(gs_cap);
    SetWindowTextW(hdlg, tstr_cap.c_str());
    PostMessageW(hdlg, WM_SETFOCUS, 0, 0);
  }

  return FALSE;
}

namespace enigma_user {

void message_alpha(double alpha) {

}

void message_background(int back) {

}

void message_button(int spr) {

}

void message_button_font(string name, int size, int color, int style) {

}

void message_caption(bool show, string str) {
  gs_cap = str;
}

void message_input_color(int col) {

}

void message_input_font(string name, int size, int color, int style) {

}

void message_mouse_color(int col) {

}

void message_position(int x, int y) {

}

void message_size(int w, int h) {

}

void message_text_font(string name, int size, int color, int style) {

}

void message_text_charset(int type, int charset) {

}

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) {
  LoadLibrary(TEXT("Riched32.dll"));

  // Center Information Window to the Middle of the Screen
  if (left < 0) {
    left = (GetSystemMetrics(SM_CXSCREEN) - width)/2;
  }
  if (top < 0) {
    top = (GetSystemMetrics(SM_CYSCREEN) - height)/2;
  }

  HWND parent;
  if (embedGameWindow) {
    parent = enigma::hWnd;
  } else {
    WNDCLASS wc = {CS_VREDRAW|CS_HREDRAW,(WNDPROC)ShowInfoProc,0,0,enigma::hInstance,0,
      0,GetSysColorBrush(COLOR_WINDOW),0,"infodialog"};
    RegisterClass(&wc);

    DWORD flags = WS_VISIBLE|WS_POPUP|WS_SYSMENU|WS_TABSTOP|WS_CLIPCHILDREN; // DS_3DLOOK|DS_CENTER|DS_FIXEDSYS
    if (showBorder) {
      flags |= WS_BORDER | WS_DLGFRAME | WS_CAPTION;
    }
    if (allowResize) {
      flags |= WS_SIZEBOX;
    }
    if (stayOnTop) {
      flags |= DS_MODALFRAME; // Same as WS_EX_TOPMOST
    }

    parent = CreateWindow("infodialog", TEXT(caption.c_str()),
      flags, left, top, width, height, enigma::hWnd, 0, enigma::hInstance, 0);

    if (showBorder) {
      // Set Window Information Icon
      HICON hIcon = LoadIcon(enigma::hInstance, "infoicon");
      if (hIcon) {
        SendMessage(parent, WM_SETICON, ICON_SMALL,(LPARAM)hIcon);
        SendMessage(parent, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
      }
    }
  }

  enigma::infore=CreateWindowEx(WS_EX_TOPMOST,"RICHEDIT",TEXT("information text"),
    ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN | ES_READONLY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP,
    0, 0, width, height, parent, 0, enigma::hInstance, 0);

  // Size the RTF Component to the Window
  RECT rectParent;
  GetClientRect(parent, &rectParent);
  MoveWindow(enigma::infore, rectParent.top, rectParent.left, rectParent.right, rectParent.bottom, TRUE);

  // Set RTF Editor Background Color
  SendMessage(enigma::infore, EM_SETBKGNDCOLOR, (WPARAM)0, (LPARAM)RGB(COL_GET_R(bgcolor), COL_GET_G(bgcolor), COL_GET_B(bgcolor)));

  // Set RTF Information Text
  SETTEXTEX se;
  se.codepage = CP_ACP;
  se.flags = ST_DEFAULT;
  SendMessage(enigma::infore, EM_SETTEXTEX, (WPARAM)&se, (LPARAM)info.c_str());

  //TODO: Figure out how to block if we need to pause the game, otherwise ShowWindowAsync
  ShowWindow(parent, SW_SHOWDEFAULT);
  if (!embedGameWindow) {
    SetFocus(enigma::infore);
  }

  /*
  MSG msg;
  BOOL bRet;

  bool bQuit = false;
  while (!bQuit)
  {
    bRet = PeekMessage(&msg, main, 0, 0, PM_REMOVE);
    if (bRet == -1) {
      // handle the error and possibly exit
    } else if (msg.message == WM_CLOSE) {
      bQuit = true;
    } else {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }*/

  /* Round two...
  MSG msg;
  BOOL bRet;

  bool bQuit = false;
  while (!bQuit)
  {
    // Check RTF Control Messages
    bRet = PeekMessage(&msg, infore, 0, 0, PM_REMOVE);
    if (bRet == -1) {
      // handle the error and possibly exit
      PostMessage(embedGameWindow ? infore : main, WM_CLOSE, 0, 0);
      bQuit = true;
    } else {
      if (msg.message == WM_KEYUP) {
        switch(msg.wParam)
        {
          case VK_ESCAPE:
          PostMessage(embedGameWindow ? infore : main, WM_CLOSE, 0, 0);
          bQuit = true;
          break;
        }
      } else {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }

    // If game information was showed in a separate window, then handle the messages for sizing and stuff
    if (!embedGameWindow) {
      bRet = PeekMessage(&msg, main, 0, 0, PM_REMOVE);
      if (bRet == -1) {
        // handle the error and possibly exit
        PostMessage(main, WM_CLOSE, 0, 0);
        bQuit = true;
      } else {
        if (msg.message == WM_KEYUP) {
          switch(msg.wParam)
            {
            case VK_ESCAPE:
            PostMessage(main, WM_CLOSE, 0, 0);
            bQuit = true;
            break;
            }
        } else if (msg.message == WM_SIZE) {
          RECT rectParent;
          GetClientRect(main, &rectParent);
          MoveWindow(infore, rectParent.top, rectParent.left, rectParent.right, rectParent.bottom, TRUE);
        } else {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      }
    }


  }
  */
}

int show_message(const string &str) {
  //NOTE: This will not work with a fullscreen application, it is an issue with Windows
  //this could be why GM8.1, unlike Studio, did not use native dialogs and custom
  //rendered its own message boxes like most game engines.
  //In Studio this function will cause the window to be minimized and the message shown, fullscreen will not be restored.
  //A possible alternative is fake fullscreen for Win32, but who knows if we have to do that on XLIB or anywhere else.

  tstring tstrStr = widen(str);
  tstring tstrWindowCaption = widen(window_get_caption());

  MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_OK | MB_ICONINFORMATION | MB_DEFBUTTON1 | MB_APPLMODAL);

  return 0;
}

int show_message_ext(string msg, string but1, string but2, string but3) {
  gs_cap = window_get_caption();
  gs_message = msg;
  gs_but1 = but1; gs_but2 = but2; gs_but3 = but3;

  return DialogBox(enigma::hInstance,"showmessageext",enigma::hWnd,ShowMessageExtProc);
}

bool show_question(string str) {
  tstring tstrStr = widen(str);
  tstring tstrWindowCaption = widen(window_get_caption());

  double result;
  result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1 | MB_APPLMODAL);

  return (result == IDYES);
}

string get_login(string username, string password, string cap) {
  gs_cap = cap; gs_username = username; gs_password = password;
  DialogBox(enigma::hInstance,"getlogindialog",enigma::hWnd,GetLoginProc);

  return gs_str_submitted;
}

string get_string(string str, string def, string title) {
  tstring tstrStr = widen(str);
  tstring tstrDef = widen(def);
  if (title == "") title = window_get_caption();
  gs_cap = title;

  wcsncpy(wstrPromptStr, tstrStr.c_str(), 4096);
  wcsncpy(wstrTextEntry, tstrDef.c_str(), MAX_PATH + 1);

  HideInput = 0;
  DialogBoxW(enigma::hInstance, MAKEINTRESOURCEW(IDD_INPUTBOX), enigma::hWnd, reinterpret_cast<DLGPROC>(InputBoxProc));

  static string strResult;
  strResult = shorten(wstrTextEntry);
  return strResult;
}

string get_password(string str, string def, string title) {
  tstring tstrStr = widen(str);
  tstring tstrDef = widen(def);
  if (title == "") title = window_get_caption();
  gs_cap = title;

  wcsncpy(wstrPromptStr, tstrStr.c_str(), 4096);
  wcsncpy(wstrTextEntry, tstrDef.c_str(), MAX_PATH + 1);

  HideInput = 1;
  DialogBoxW(enigma::hInstance, MAKEINTRESOURCEW(IDD_INPUTBOX), enigma::hWnd, reinterpret_cast<DLGPROC>(InputBoxProc));

  static string strResult;
  strResult = shorten(wstrTextEntry);
  return strResult;
}

double get_integer(string str, double def, string title) {
  std::ostringstream defInteger;
  defInteger << def;
  string strDef = defInteger.str();

  tstring tstrStr = widen(str);
  tstring tstrDef = widen(strDef);
  if (title == "") title = window_get_caption();
  gs_cap = title;

  wcsncpy(wstrPromptStr, tstrStr.c_str(), 4096);
  wcsncpy(wstrTextEntry, tstrDef.c_str(), MAX_PATH + 1);

  HideInput = 0;
  DialogBoxW(enigma::hInstance, MAKEINTRESOURCEW(IDD_INPUTBOX), enigma::hWnd, reinterpret_cast<DLGPROC>(InputBoxProc));

  static string strResult;
  strResult = shorten(wstrTextEntry);
  char *cstrResult = (char *)strResult.c_str();

  return cstrResult ? strtod(cstrResult, NULL) : 0;
}

double get_passcode(string str, double def, string title) {
  std::ostringstream defInteger;
  defInteger << def;
  string strDef = defInteger.str();

  tstring tstrStr = widen(str);
  tstring tstrDef = widen(strDef);
  if (title == "") title = window_get_caption();
  gs_cap = title;

  wcsncpy(wstrPromptStr, tstrStr.c_str(), 4096);
  wcsncpy(wstrTextEntry, tstrDef.c_str(), MAX_PATH + 1);

  HideInput = 1;
  DialogBoxW(enigma::hInstance, MAKEINTRESOURCEW(IDD_INPUTBOX), enigma::hWnd, reinterpret_cast<DLGPROC>(InputBoxProc));

  static string strResult;
  strResult = shorten(wstrTextEntry);
  char *cstrResult = (char *)strResult.c_str();

  return cstrResult ? strtod(cstrResult, NULL) : 0;
}

bool get_string_canceled() {
  return gs_form_canceled;
}

namespace {
  string ofn_init(bool open, LPWSTR fname, LPCWSTR filter, LPCWSTR dir, LPCWSTR title, DWORD flags) {
    OPENFILENAMEW ofn;
    
    tstring tstr_empty = widen("");
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = enigma::hWnd;
    ofn.lpstrFile = fname;
    ofn.nMaxFile = MAX_PATH + 1;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 0;
    ofn.lpstrTitle = title;
    if (title == tstr_empty.c_str()) ofn.lpstrTitle = NULL;
    ofn.lpstrInitialDir = dir;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | flags;

    if (open) {
      if (GetOpenFileNameW(&ofn) != 0) {
        static string result;
        result = shorten(fname);
        return result;
      }
    } else {
      if (GetSaveFileNameW(&ofn) != 0) {
        static string result;
        result = shorten(fname);
        return result;
      }
    }

    return "";
  }
}

string get_open_filename(string filter, string fname, string title) {
  string str_filter = filter.append("||");

  tstring tstr_filter = widen(str_filter);
  replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
  tstring tstr_fname = widen(fname);
  tstring tstr_title = widen(title);
  if (title == "") title = "Open";
  gs_cap = title;

  WCHAR wstr_fname[MAX_PATH + 1];
  wcsncpy(wstr_fname, tstr_fname.c_str(), MAX_PATH + 1);

  return ofn_init(true, wstr_fname, tstr_filter.c_str(), NULL, tstr_title.c_str(), 0);
}

string get_save_filename(string filter, string fname, string title) {
  string str_filter = filter.append("||");

  tstring tstr_filter = widen(str_filter);
  replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
  tstring tstr_fname = widen(fname);
  tstring tstr_title = widen(title);
  if (title == "") title = "Save As";
  gs_cap = title;

  WCHAR wstr_fname[MAX_PATH + 1];
  wcsncpy(wstr_fname, tstr_fname.c_str(), MAX_PATH + 1);

  return ofn_init(false, wstr_fname, tstr_filter.c_str(), NULL, tstr_title.c_str(), OFN_OVERWRITEPROMPT);
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  string str_filter = filter.append("||");

  tstring tstr_filter = widen(str_filter);
  replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
  tstring tstr_fname = widen(fname);
  tstring tstr_dir = widen(dir);
  tstring tstr_title = widen(title);
  if (title == "") title = "Open";
  gs_cap = title;

  WCHAR wstr_fname[MAX_PATH + 1];
  wcsncpy(wstr_fname, tstr_fname.c_str(), MAX_PATH + 1);

  return ofn_init(true, wstr_fname, tstr_filter.c_str(), tstr_dir.c_str(), tstr_title.c_str(), 0);
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  string str_filter = filter.append("||");

  tstring tstr_filter = widen(str_filter);
  replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
  tstring tstr_fname = widen(fname);
  tstring tstr_dir = widen(dir);
  tstring tstr_title = widen(title);
  if (title == "") title = "Save As";
  gs_cap = title;

  WCHAR wstr_fname[MAX_PATH + 1];
  wcsncpy(wstr_fname, tstr_fname.c_str(), MAX_PATH + 1);

  return ofn_init(false, wstr_fname, tstr_filter.c_str(), tstr_dir.c_str(), tstr_title.c_str(), OFN_OVERWRITEPROMPT);
}

double get_color(double defcol, bool advanced, string title) {
  CHOOSECOLOR cc;

  if (title == "") title = "Color";
  gs_cap = title;

  COLORREF DefColor = (int)defcol;
  static COLORREF CustColors[16];

  ZeroMemory(&cc, sizeof(cc));
  cc.lStructSize = sizeof(CHOOSECOLOR);
  cc.hwndOwner = enigma::hWnd;
  cc.rgbResult = DefColor;
  cc.lpCustColors = CustColors;
  cc.Flags = CC_RGBINIT | CC_ENABLEHOOK;
  if (advanced) cc.Flags |= CC_FULLOPEN;
  cc.lpfnHook = GetColorProc;

  if (ChooseColor(&cc) != 0)
    return (int)cc.rgbResult;

  return -1;
}

string get_directory(string dname, string title) {
  OPENFILENAMEW ofn;

  tstring tstr_filter = widen("*.*|*.*|");
  replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
  tstring tstr_dname = widen(dname);
  tstring tstr_title = widen(title);
  tstring tstr_empty = widen("");
  if (title == "") title = "Select Directory";
  gs_cap = title;

  if (tstr_dname == tstr_empty)
    GetCurrentDirectoryW(MAX_PATH + 1, wstr_dname);
  else
    wcsncpy(wstr_dname, tstr_dname.c_str(), MAX_PATH + 1);

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = enigma::hWnd;
  ofn.lpstrFile = NULL;
  ofn.nMaxFile = MAX_PATH + 1;
  ofn.lpstrFilter = tstr_filter.c_str();
  ofn.nFilterIndex = 0;
  ofn.lpstrTitle = tstr_title.c_str();
  ofn.lpstrInitialDir = wstr_dname;
  ofn.Flags = OFN_ENABLETEMPLATE | OFN_ENABLEHOOK | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_LONGNAMES;
  ofn.lpTemplateName = (LPCWSTR)MAKEINTRESOURCE(FILEOPENORD);
  ofn.lpfnHook = (LPOFNHOOKPROC)GetDirectoryProc;
  ofn.hInstance = enigma::hInstance;

  GetOpenFileNameW(&ofn);

  DWORD attrib = GetFileAttributesW(wstr_dname);

  if (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY)) {
    static string result;
    result = shorten(wstr_dname);
    return result;
  }

  return "";
}

string get_directory_alt(string capt, string root, bool modern, string title) {
  BROWSEINFOW bi;

  tstring tstr_capt = widen(capt);
  tstring tstr_root = widen(root);
  tstring tstr_slash = widen("\\");
  tstring tstr_empty = widen("");
  tstring tstr_zero = widen("0");
  if (title == "") title = "Browse For Folder";
  gs_cap = title;

  LPITEMIDLIST pstr_root;

  if (tstr_root == tstr_empty)
    SHParseDisplayName(tstr_zero.c_str(), 0, &pstr_root, 0, 0);
  else
    SHParseDisplayName(tstr_root.c_str(), 0, &pstr_root, 0, 0);

  WCHAR wstr_dir[MAX_PATH + 1];
  LPMALLOC pMalloc;
  gs_cap = title;

  if (SUCCEEDED(SHGetMalloc(&pMalloc))) {
    ZeroMemory(&bi, sizeof(bi));
    bi.hwndOwner = enigma::hWnd;
    bi.pidlRoot = pstr_root;
    bi.pszDisplayName = wstr_dir;
    bi.lpszTitle = tstr_capt.c_str();
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON;
    if (modern) bi.ulFlags |= BIF_EDITBOX;
    bi.lpfn = GetDirectoryAltProc;

    LPITEMIDLIST lpItem = SHBrowseForFolderW(&bi);
    if (lpItem != NULL) {
      if (SHGetPathFromIDListW(lpItem, wstr_dir) == 0)
        return "";
      else {
        static string result;
        result = string_replace_all(shorten(wstr_dir) + shorten(tstr_slash), "\\\\", "\\");
        return result;
      }

      pMalloc->Free(lpItem);
      pMalloc->Release();
    }
  }

  return "";
}

}
