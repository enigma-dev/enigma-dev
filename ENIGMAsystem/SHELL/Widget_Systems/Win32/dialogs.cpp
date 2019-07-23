/** Copyright (C) 2011, 2017 Josh Ventura
*** Copyright (C) 2014, 2017 Robert B. Colton
*** Copyright (C) 2019 Samuel Venable
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

// Windows Vista or later for IFileDialog
#define NTDDI_VERSION NTDDI_VISTA
#define _WIN32_WINNT _WIN32_WINNT_VISTA
#include <windows.h>
#include <shobjidl.h> //for IFileDialog
#include <shlwapi.h> //for Shell API
#include <shlobj.h> //for Shell API
#include <richedit.h>
#include <cstdio>
#include <string>
#include <algorithm>

using namespace std;
#include "Widget_Systems/widgets_mandatory.h"
#include "Widget_Systems/General/WSdialogs.h"
#include "Universal_System/estring.h"
#include "GameSettings.h"

#include "Graphics_Systems/General/GScolor_macros.h"

#define MONITOR_CENTER 0x0001

static string gs_cap;
static string gs_def;
static string gs_message;
static string gs_username;
static string gs_password;
static bool   gs_form_canceled;
static string gs_str_submitted;
static string gs_but1, gs_but2, gs_but3;

// message and error captions
static string message_caption;
static tstring dialog_caption = L"";
static tstring error_caption = L"";

// show cancel button?
static bool message_cancel = false;
static bool question_cancel = false;

// get color
static string str_gctitle;
static tstring tstr_gctitle;

// file dialogs
static wchar_t wstr_filter[512];
static wchar_t wstr_fname[4096];
static tstring tstr_dir;
static tstring tstr_title;

using enigma_user::string_replace_all;

#ifdef DEBUG_MODE
#include "Universal_System/var4.h"
#include "Universal_System/Resources/resource_data.h"
#include "Universal_System/Object_Tiers/object.h"
#include "Universal_System/debugscope.h"
#endif

string message_get_caption();

static inline string add_slash(const string& dir) {
  if (dir.empty() || *dir.rbegin() != '\\') return dir + '\\';
  return dir;
}

namespace enigma {

extern HINSTANCE hInstance;
extern HWND hWnd;
HWND infore;

}

static inline string remove_slash(string dir) {
  while (!dir.empty() && (dir.back() == '\\' || dir.back() == '/'))
    dir.pop_back();
  return dir;
}

static inline string remove_trailing_zeros(double numb) {
  string strnumb = std::to_string(numb);

  while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
    strnumb.pop_back();

  return strnumb;
}

static inline void CenterRectToMonitor(LPRECT prc, UINT flags) {
  HMONITOR hMonitor;
  MONITORINFO mi;
  RECT        rc;
  int         w = prc->right - prc->left;
  int         h = prc->bottom - prc->top;

  hMonitor = MonitorFromRect(prc, MONITOR_DEFAULTTONEAREST);

  mi.cbSize = sizeof(mi);
  GetMonitorInfo(hMonitor, &mi);
  rc = mi.rcMonitor;

  if (flags & MONITOR_CENTER) {
    prc->left = rc.left + (rc.right - rc.left - w) / 2;
    prc->top = rc.top + (rc.bottom - rc.top - h) / 2;
    prc->right = prc->left + w;
    prc->bottom = prc->top + h;
  }
  else {
    prc->left = rc.left + (rc.right - rc.left - w) / 2;
    prc->top = rc.top + (rc.bottom - rc.top - h) / 3;
    prc->right = prc->left + w;
    prc->bottom = prc->top + h;
  }
}

// this is the correct way to center window on multi-monitor setups;
// GetSystemMetrics() is discouraged by Microsoft for a good reason.
static inline void CenterWindowToMonitor(HWND hwnd, UINT flags) {
  RECT rc;
  GetWindowRect(hwnd, &rc);
  CenterRectToMonitor(&rc, flags);
  SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

static tstring tstring_replace_all(tstring str, tstring substr, tstring newstr) {
  return widen(string_replace_all(shorten(str), shorten(substr), shorten(newstr)));
}

static INT_PTR CALLBACK ShowInfoProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG) {
    return true;
  }

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

static INT_PTR CALLBACK GetStrProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG) {
    CenterWindowToMonitor(hwndDlg, 0);
    tstring tstr_gs_cap = widen(gs_cap);
    tstring tstr_gs_def = widen(gs_def);
    tstring tstr_gs_message = widen(gs_message);
    SetWindowTextW(hwndDlg, tstr_gs_cap.c_str());
    SetDlgItemTextW(hwndDlg, 12, tstr_gs_def.c_str());
    SetDlgItemTextW(hwndDlg, 13, tstr_gs_message.c_str());
    return true;
  }

  if (uMsg == WM_COMMAND) {
    if (wParam == 2 || wParam == 11) {
      gs_str_submitted = "";
      gs_form_canceled = 1;
      EndDialog(hwndDlg, 1);
    } else if (wParam == 10) {
      wchar_t strget[1024];
      GetDlgItemTextW(hwndDlg, 12, strget, 1024);
      gs_str_submitted = shorten(strget);
      gs_form_canceled = 0;
      EndDialog(hwndDlg, 2);
    }
  }

  return 0;
}

static INT_PTR CALLBACK GetLoginProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG) {
    tstring tstr_gs_cap = widen(gs_cap);
    SetWindowTextW(hwndDlg, tstr_gs_cap.c_str());
    tstring tstr_gs_username = widen(gs_username);
    tstring tstr_gs_password = widen(gs_password);
    SetDlgItemTextW(hwndDlg, 14, tstr_gs_username.c_str());
    SetDlgItemTextW(hwndDlg, 15, tstr_gs_password.c_str());
    return true;
  }

  if (uMsg == WM_COMMAND) {
    if (wParam == 2 || wParam == 11) {
      gs_str_submitted = "";
      gs_form_canceled = 1;
      EndDialog(hwndDlg, 1);
    } else if (wParam == 10) {
      wchar_t strget[1024];
      GetDlgItemTextW(hwndDlg, 14, strget, 1024);
      gs_str_submitted = shorten(strget);
      GetDlgItemTextW(hwndDlg, 15, strget, 1024);
      gs_str_submitted += string(1, 0) + shorten(strget);
      gs_form_canceled = 0;
      EndDialog(hwndDlg, 2);
    }
  }

  return 0;
}

static INT_PTR CALLBACK ShowMessageExtProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG) {
    tstring tstr_gs_cap = widen(gs_cap);
    tstring tstr_gs_message = widen(gs_message);
    tstring tstr_gs_but1 = widen(gs_but1);
    tstring tstr_gs_but2 = widen(gs_but2);
    tstring tstr_gs_but3 = widen(gs_but3);
    SetWindowTextW(hwndDlg, tstr_gs_cap.c_str());
    SetDlgItemTextW(hwndDlg, 10, tstr_gs_message.c_str());
    SetDlgItemTextW(hwndDlg, 11, tstr_gs_but1.c_str());
    SetDlgItemTextW(hwndDlg, 12, tstr_gs_but2.c_str());
    SetDlgItemTextW(hwndDlg, 13, tstr_gs_but3.c_str());
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

static UINT_PTR CALLBACK GetColorProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
  if (uiMsg == WM_INITDIALOG) {
    CenterWindowToMonitor(hdlg, 0);
    if (str_gctitle != "")
      SetWindowTextW(hdlg, tstr_gctitle.c_str());
    PostMessageW(hdlg, WM_SETFOCUS, 0, 0);
  }

  return false;
}

static inline int show_message_helperfunc(const string &message) {
  tstring tstrStr = widen(message);

  wchar_t wstrWindowCaption[512];
  GetWindowTextW(enigma::hWnd, wstrWindowCaption, 512);

  if (dialog_caption != L"")
    wcsncpy_s(wstrWindowCaption, dialog_caption.c_str(), 512);

  if (message_cancel) {
    int result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), wstrWindowCaption, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1 | MB_APPLMODAL);
    if (result == IDOK) return 1; else return -1;
  }

  MessageBoxW(enigma::hWnd, tstrStr.c_str(), wstrWindowCaption, MB_OK | MB_ICONINFORMATION | MB_DEFBUTTON1 | MB_APPLMODAL);
  return 1;
}

static inline int show_question_helperfunc(string message) {
  tstring tstrStr = widen(message);

  wchar_t wstrWindowCaption[512];
  GetWindowTextW(enigma::hWnd, wstrWindowCaption, 512);

  if (dialog_caption != L"")
    wcsncpy_s(wstrWindowCaption, dialog_caption.c_str(), 512);

  int result;
  if (question_cancel) {
    result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), wstrWindowCaption, MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1 | MB_APPLMODAL);
    if (result == IDYES) return 1; else if (result == IDNO) return 0; else return -1;
  }

  result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), wstrWindowCaption, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1 | MB_APPLMODAL);
  if (result == IDYES) return 1;
  return 0;
}

static inline OPENFILENAMEW get_filename_or_filenames_helper(string filter, string fname, string dir, string title, DWORD flags) {
  OPENFILENAMEW ofn;

  filter = filter.append("||");
  fname = remove_slash(fname);

  tstring tstr_filter = widen(filter);
  tstring tstr_fname = widen(fname);
  tstr_dir = widen(dir);
  tstr_title = widen(title);

  wcsncpy_s(wstr_filter, tstr_filter.c_str(), 512);
  wcsncpy_s(wstr_fname, tstr_fname.c_str(), 4096);
  
  int i = 0;
  while(wstr_filter[i] != '\0') {
    if(wstr_filter[i] == '|') {
      wstr_filter[i] = '\0';
    }
    i += 1;
  }

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = enigma::hWnd;
  ofn.lpstrFile = wstr_fname;
  ofn.nMaxFile = 4096;
  ofn.lpstrFilter = wstr_filter;
  ofn.nMaxCustFilter = 512;
  ofn.nFilterIndex = 0;
  ofn.lpstrTitle = tstr_title.c_str();
  ofn.lpstrInitialDir = tstr_dir.c_str();
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | flags;
  
  return ofn;
}

static inline string get_open_filename_helper(string filter, string fname, string dir, string title) {
  OPENFILENAMEW ofn;
  ofn = get_filename_or_filenames_helper(filter, fname, dir, title, 0);

  if (GetOpenFileNameW(&ofn) != 0)
    return shorten(wstr_fname);

  return "";
}

static inline string get_open_filenames_helper(string filter, string fname, string dir, string title) {
  OPENFILENAMEW ofn;
  ofn = get_filename_or_filenames_helper(filter, fname, dir, title, OFN_ALLOWMULTISELECT);

  if (GetOpenFileNameW(&ofn) != 0) {
    tstring tstr_fname1 = wstr_fname;
    tstr_fname1 += '\\';

    size_t pos = 0;
    size_t prevlen = pos;
    size_t len = wcslen(wstr_fname);

    while (pos < len) {
      if (wstr_fname[len - 1] != '\n' && wstr_fname[len] == '\0')
        wstr_fname[len] = '\n';

      prevlen = len;
      len = wcslen(wstr_fname);
      pos += (len - prevlen) + 1;
    }

    tstring tstr_fname2 = wstr_fname;
    if (tstr_fname2[len - 1] == '\n') tstr_fname2[len - 1] = '\0';
    if (tstr_fname2[len - 2] == '\\') tstr_fname2[len - 2] = '\0';
    tstr_fname2 = tstring_replace_all(tstr_fname2, L"\n", L"\n" + tstr_fname1);
    size_t rm = tstr_fname2.find_first_of(L'\n');

    if (rm != string::npos)
      tstr_fname2 = tstr_fname2.substr(rm + 1, tstr_fname2.length() - (rm + 1));

    tstr_fname2.append(L"\0");
    if (tstr_fname2.length() >= 4095) {
      tstr_fname2 = tstr_fname2.substr(0, 4095);
      size_t end = tstr_fname2.find_last_of(L"\n");
      tstr_fname2 = tstr_fname2.substr(0, end);
      tstr_fname2.append(L"\0");
    }

    return string_replace_all(shorten(tstr_fname2), "\\\\", "\\");
  }

  return "";
}

static inline string get_save_filename_helper(string filter, string fname, string dir, string title) {
  OPENFILENAMEW ofn;
  ofn = get_filename_or_filenames_helper(filter, fname, dir, title, 0);

  if (GetSaveFileNameW(&ofn) != 0)
    return shorten(wstr_fname);

  return "";
}

static inline string get_directory_helper(string dname, string title) {
  IFileDialog *selectDirectory;
  CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&selectDirectory));

  DWORD options;
  selectDirectory->GetOptions(&options);
  selectDirectory->SetOptions(options | FOS_PICKFOLDERS | FOS_NOCHANGEDIR | FOS_FORCEFILESYSTEM);

  tstring tstr_dname = widen(dname);
  LPWSTR szFilePath = (wchar_t *)tstr_dname.c_str();

  IShellItem* pItem = nullptr;
  HRESULT hr = ::SHCreateItemFromParsingName(szFilePath, nullptr, IID_PPV_ARGS(&pItem));

  if (SUCCEEDED(hr)) {
    LPWSTR szName = nullptr;
    hr = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &szName);
    if (SUCCEEDED(hr)) {
      selectDirectory->SetFolder(pItem);
      ::CoTaskMemFree(szName);
    }
    pItem->Release();
  }

  if (title == "") title = "Select Directory";
  tstring tstr_capt = widen(title);

  selectDirectory->SetOkButtonLabel(L"Select");
  selectDirectory->SetTitle(tstr_capt.c_str());
  selectDirectory->Show(enigma::hWnd);

  pItem = nullptr;
  hr = selectDirectory->GetResult(&pItem);

  if (SUCCEEDED(hr)) {
    LPWSTR wstr_result;
    pItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &wstr_result);
    pItem->Release();

    return add_slash(shorten(wstr_result));
  }

  return "";
}

static inline int get_color_helper(int defcol, string title) {
  CHOOSECOLORW cc;

  COLORREF DefColor = (int)defcol;
  static COLORREF CustColors[16];

  str_gctitle = title;
  tstr_gctitle = widen(str_gctitle);

  ZeroMemory(&cc, sizeof(cc));
  cc.lStructSize = sizeof(CHOOSECOLORW);
  cc.hwndOwner = enigma::hWnd;
  cc.rgbResult = DefColor;
  cc.lpCustColors = CustColors;
  cc.Flags = CC_RGBINIT | CC_ENABLEHOOK;
  cc.lpfnHook = GetColorProc;

  if (ChooseColorW(&cc) != 0)
    return (int)cc.rgbResult;

  return -1;
}

static inline void show_debug_message_helper(string errortext, MESSAGE_TYPE type) {
  #ifdef DEBUG_MODE
  errortext += "\n\n" + enigma::debug_scope::GetErrors();
  #endif

  string strWindowCaption = "Error";

  tstring tstrStr = widen(errortext);
  tstring tstrWindowCaption = widen(strWindowCaption);

  if (error_caption != L"")
    tstrWindowCaption = error_caption;

  int result;
  result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_DEFBUTTON1 | MB_APPLMODAL);
  if (result == IDABORT || type == MESSAGE_TYPE::M_FATAL_ERROR) exit(0);

  //ABORT_ON_ALL_ERRORS();
}

static string widget = enigma_user::ws_win32;

namespace enigma_user {
    
string widget_get_system() {
  return widget;
}

void widget_set_system(string sys) {
  // place holder
}

void show_debug_message(string errortext, MESSAGE_TYPE type) {
  if (type != M_INFO && type != M_WARNING) {
    show_debug_message_helper(errortext, type);
  } else {
    #ifndef DEBUG_MODE
    fputs(errortext.c_str(), stderr);
    #endif
  }
}

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) {
  LoadLibrary(TEXT("Riched32.dll"));

  // Center Information Window to the Middle of the Screen
  if (left < 0) {
    left = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
  }
  if (top < 0) {
    top = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
  }

  HWND parent;
  if (embedGameWindow) {
    parent = enigma::hWnd;
  } else {
    WNDCLASS wc = { CS_VREDRAW | CS_HREDRAW, (WNDPROC)ShowInfoProc, 0, 0, enigma::hInstance, 0,
      0, GetSysColorBrush(COLOR_WINDOW), 0, "infodialog" };
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

int show_message(const string &message) {
  message_cancel = false;
  return show_message_helperfunc(message);
}

int show_message_cancelable(string message) {
  message_cancel = true;
  return show_message_helperfunc(message);
}

bool show_question(string message) {
  question_cancel = false;
  return (bool)show_question_helperfunc(message);
}

int show_question_cancelable(string message) {
  question_cancel = true;
  return show_question_helperfunc(message);
}

int show_attempt(string errortext) {
  string strWindowCaption = "Error";

  tstring tstrStr = widen(errortext);
  tstring tstrWindowCaption = widen(strWindowCaption);

  if (error_caption != L"")
    tstrWindowCaption = error_caption;

  int result;
  result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_RETRYCANCEL | MB_ICONERROR | MB_DEFBUTTON1 | MB_APPLMODAL);
  if (result == IDRETRY) return 0;
  return -1;
}

int show_message_ext(string message, string but1, string but2, string but3) {
  gs_cap = message_get_caption();
  gs_message = message;
  gs_but1 = but1; gs_but2 = but2; gs_but3 = but3;

  return DialogBoxW(enigma::hInstance, L"showmessageext", enigma::hWnd, ShowMessageExtProc);
}

string get_login(string username, string password) {
  gs_cap = message_get_caption(); gs_username = username; gs_password = password;
  DialogBoxW(enigma::hInstance, L"getlogindialog", enigma::hWnd, GetLoginProc);

  return gs_str_submitted;
}

string get_string(string message, string def) {
  gs_cap = message_get_caption(); gs_message = message; gs_def = def;
  DialogBoxW(enigma::hInstance, L"getstringdialog", enigma::hWnd, GetStrProc);

  return gs_str_submitted;
}

string get_password(string message, string def) {
  gs_cap = message_get_caption(); gs_message = message; gs_def = def;
  DialogBoxW(enigma::hInstance, L"getpassworddialog", enigma::hWnd, GetStrProc);

  return gs_str_submitted;
}

double get_integer(string message, double def) {
  gs_cap = message_get_caption(); gs_message = message; gs_def = remove_trailing_zeros(def);
  DialogBoxW(enigma::hInstance, L"getstringdialog", enigma::hWnd, GetStrProc);
  if (gs_str_submitted == "") return 0;
  puts(gs_str_submitted.c_str());

  return strtod(gs_str_submitted.c_str(), NULL);
}

double get_passcode(string message, double def) {
  gs_cap = message_get_caption(); gs_message = message; gs_def = remove_trailing_zeros(def);
  DialogBoxW(enigma::hInstance, L"getpassworddialog", enigma::hWnd, GetStrProc);
  if (gs_str_submitted == "") return 0;
  puts(gs_str_submitted.c_str());

  return strtod(gs_str_submitted.c_str(), NULL);
}

bool get_string_canceled() {
  return gs_form_canceled;
}

string get_open_filename(string filter, string fname) {
  return get_open_filename_helper(filter, fname, "", "");
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  return get_open_filename_helper(filter, fname, dir, title);
}

string get_open_filenames(string filter, string fname) {
  return get_open_filenames_helper(filter, fname, "", "");
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) {
  return get_open_filenames_helper(filter, fname, dir, title);
}

string get_save_filename(string filter, string fname) {
  return get_save_filename_helper(filter, fname, "", "");
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  return get_save_filename_helper(filter, fname, dir, title);
}

int get_color(int defcol) {
  return get_color_helper(defcol, "");
}

int get_color_ext(int defcol, string title) {
  return get_color_helper(defcol, title);
}

string get_directory(string dname) {
  return get_directory_helper(dname, "");
}

string get_directory_alt(string capt, string root) {
  return get_directory_helper(root, capt);
}

string message_get_caption() {
  if (dialog_caption.empty()) {
    wchar_t wstrWindowCaption[512];
    GetWindowTextW(enigma::hWnd, wstrWindowCaption, 512);
    return shorten(wstrWindowCaption);
  }

  if (error_caption.empty()) error_caption = L"Error";
  if (dialog_caption.empty() && error_caption == L"Error")
    return "";

  return shorten(dialog_caption);
}

void message_set_caption(string title) {
  if (!title.empty()) dialog_caption = widen(title);
  else dialog_caption = L"";

  if (!title.empty()) error_caption = widen(title);
  else error_caption = L"Error";
}

} // namespace enigma_user
