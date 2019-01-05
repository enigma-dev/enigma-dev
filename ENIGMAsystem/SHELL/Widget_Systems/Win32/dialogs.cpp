/** Copyright (C) 2011, 2017 Josh Ventura
*** Copyright (C) 2014, 2017 Robert B. Colton
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
#include <stdio.h>
#include <string>

using namespace std;
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/estring.h"
#include "GameSettings.h"

#include "../General/WSdialogs.h"

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

void show_error(string errortext, const bool fatal)
{
  #ifdef DEBUG_MODE
    errortext += enigma::debug_scope::GetErrors();
  #else
  errortext = "Error in some event or another for some object: \r\n" + errortext;
  #endif

  if (MessageBox(NULL,errortext.c_str(),"Error",MB_ABORTRETRYIGNORE | MB_ICONERROR)==IDABORT)
    exit(0);

  if (fatal)
    printf("FATAL ERROR: %s\n",errortext.c_str()),
    exit(0);
  else
    printf("ERROR: %s\n",errortext.c_str());

  //ABORT_ON_ALL_ERRORS();
}

namespace enigma {
  extern HINSTANCE hInstance;
  extern HWND hWnd;
  HWND infore;
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
    SetWindowText(hwndDlg, gs_cap.c_str());
    SetDlgItemText(hwndDlg, 12, gs_def.c_str());
    SetDlgItemText(hwndDlg, 13, gs_message.c_str());
    return true;
  }

  if (uMsg == WM_COMMAND) {
    if (wParam == 2 || wParam == 11) {
      gs_str_submitted = "";
      gs_form_canceled = 1;
      EndDialog(hwndDlg, 1);
    } else if (wParam == 10) {
      char strget[1024];
      GetDlgItemText(hwndDlg, 12, strget, 1024);
      gs_str_submitted = strget;
      gs_form_canceled = 0;
      EndDialog(hwndDlg, 2);
    }
  }

  return 0;
}

static INT_PTR CALLBACK GetLoginProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG) {
    SetWindowText(hwndDlg, gs_cap.c_str());
    SetDlgItemText(hwndDlg, 14, gs_username.c_str());
    SetDlgItemText(hwndDlg, 15, gs_password.c_str());
    return true;
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

static INT_PTR CALLBACK ShowMessageExtProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG) {
    SetWindowText(hwndDlg,gs_cap.c_str());
    SetDlgItemText(hwndDlg, 10, gs_message.c_str());
    SetDlgItemText(hwndDlg, 11, gs_but1.c_str());
    SetDlgItemText(hwndDlg, 12, gs_but2.c_str());
    SetDlgItemText(hwndDlg, 13, gs_but3.c_str());
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

static int CALLBACK GetDirectoryAltProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
  if (uMsg == BFFM_INITIALIZED)
    SetWindowText(hwnd, gs_cap.c_str());

  return 0;
}

namespace enigma_user {

extern string window_get_caption();

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

int show_message(const string &str)
{
  //NOTE: This will not work with a fullscreen application, it is an issue with Windows
  //this could be why GM8.1, unlike Studio, did not use native dialogs and custom
  //rendered its own message boxes like most game engines.
  //In Studio this function will cause the window to be minimized and the message shown, fullscreen will not be restored.
  //A possible alternative is fake fullscreen for Win32, but who knows if we have to do that on XLIB or anywhere else.

  tstring message = widen(str);
  tstring caption = widen(window_get_caption());

  MessageBoxW(enigma::hWnd, message.c_str(), caption.c_str(), MB_OK);

  return 0;
}

int show_message_ext(string msg, string but1, string but2, string but3)
{
  gs_cap = window_get_caption(); gs_message = msg;
  gs_but1 = but1; gs_but2 = but2; gs_but3 = but3;

  return DialogBox(enigma::hInstance, "showmessageext", enigma::hWnd, ShowMessageExtProc);
}

bool show_question(string str)
{
  return (MessageBox(enigma::hWnd, str.c_str(), window_get_caption().c_str(), MB_YESNO) == IDYES);
}

string get_login(string username, string password, string cap)
{
  gs_cap = cap; gs_username = username; gs_password = password;
  DialogBox(enigma::hInstance, "getlogindialog", enigma::hWnd, GetLoginProc);

  return gs_str_submitted;
}

string get_string(string message,string def,string cap)
{
  gs_cap = cap; gs_message = message; gs_def = def;
  DialogBox(enigma::hInstance, "getstringdialog", enigma::hWnd, GetStrProc);

  return gs_str_submitted;
}

int get_integer(string message, string def, string cap)
{
  gs_cap = cap; gs_message = message; gs_def = def;
  DialogBox(enigma::hInstance, "getstringdialog", enigma::hWnd, GetStrProc);
  if (gs_str_submitted == "") return 0;
  puts(gs_str_submitted.c_str());

  return atol(gs_str_submitted.c_str());
}

double get_number(string message,string def,string cap)
{
  gs_cap = cap; gs_message = message; gs_def = def;
  DialogBox(enigma::hInstance, "getstringdialog", enigma::hWnd, GetStrProc);
  if (gs_str_submitted == "") return 0;
  puts(gs_str_submitted.c_str());

  return atof(gs_str_submitted.c_str());
}

bool get_string_canceled() {
  return gs_form_canceled;
}

string get_open_filename(string filter,string filename,string caption)
{
  filter.append("||");
  const unsigned int l = filter.length();
  for (unsigned int i = 0; i < l; i++)
    if (filter[i] == '|') filter[i] = 0;

  char fn[MAX_PATH];
  strcpy(fn, filename.c_str());

  OPENFILENAME ofn;
  ofn.lStructSize=sizeof(ofn); ofn.hwndOwner=enigma::hWnd; ofn.hInstance=NULL;
  ofn.lpstrFilter=filter.c_str(); ofn.lpstrCustomFilter=NULL;
  ofn.nMaxCustFilter=0; ofn.nFilterIndex=0;
  ofn.lpstrFile=fn; ofn.nMaxFile=MAX_PATH;
  ofn.lpstrFileTitle=NULL; ofn.nMaxFileTitle=0;
  ofn.lpstrInitialDir=NULL; ofn.lpstrTitle=caption.length()?caption.c_str():NULL;
  ofn.Flags=OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_NOCHANGEDIR;
  ofn.nFileOffset=0; ofn.nFileExtension=0;
  ofn.lpstrDefExt=NULL; ofn.lCustData=0;
  ofn.lpfnHook=NULL; ofn.lpTemplateName=0;

  bool ret = GetOpenFileName(&ofn);
  return ret == 0 ? "-1" : fn;
}

string get_save_filename(string filter, string filename, string caption)
{
  filter.append("||");
  const unsigned int l = filter.length();
  for (unsigned int i = 0;i < l; i++)
    if (filter[i] == '|') filter[i] = 0;

  char fn[MAX_PATH];
  strcpy(fn, filename.c_str());

  OPENFILENAME ofn;
  ofn.lStructSize=sizeof(ofn); ofn.hwndOwner=enigma::hWnd; ofn.hInstance=NULL;
  ofn.lpstrFilter=filter.c_str(); ofn.lpstrCustomFilter=NULL;
  ofn.nMaxCustFilter=0; ofn.nFilterIndex=0;
  ofn.lpstrFile=fn; ofn.nMaxFile=MAX_PATH;
  ofn.lpstrFileTitle=NULL; ofn.nMaxFileTitle=0;
  ofn.lpstrInitialDir=NULL; ofn.lpstrTitle=caption.length()?caption.c_str():NULL;
  ofn.Flags=OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR;
  ofn.nFileOffset=0; ofn.nFileExtension=0;
  ofn.lpstrDefExt=NULL; ofn.lCustData=0;
  ofn.lpfnHook=NULL; ofn.lpTemplateName=0;

  bool ret = GetSaveFileName(&ofn);
  return ret == 0 ? "-1" : fn;
}

int get_color(int defcolor, bool advanced)
{
  COLORREF defc = (int)defcolor;
  static COLORREF custcs[16];

  CHOOSECOLOR gcol;
  gcol.lStructSize = sizeof(CHOOSECOLOR);
  gcol.hwndOwner = enigma::hWnd;
  gcol.rgbResult = defc;
  gcol.lpCustColors = custcs;
  if (advanced) {
    gcol.Flags = CC_FULLOPEN | CC_RGBINIT;
  } else {
    gcol.Flags = CC_RGBINIT;
  }
  gcol.lpTemplateName = "";

  if (ChooseColor(&gcol))
    return (int)gcol.rgbResult;
  else return defc;
}

string get_directory(string dname, string caption)
{
  //NOTE: This uses the Windows Vista or later file chooser, which is different than the one used by GM8 and lower
  //because I could not find out which one it uses, since IFileDialog is used by both wxWidgets and QtFramework
  //and there doesn't appear to be a standard file picker for XP or lower in the Windows API except SHBrowseForFolder that is
  //used by Game Maker for get_directory_alt
  IFileDialog* fileDialog;
  CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&fileDialog));

  DWORD options;
  fileDialog->GetOptions(&options);
  options &= ~FOS_FILEMUSTEXIST;
  options &= ~FOS_PATHMUSTEXIST;
  fileDialog->SetOptions(options | FOS_PICKFOLDERS);
  //TODO: Set default directory to dname
  //fileDialog->SetDefaultFolder(std::wstring(dname.begin(), dname.end()).c_str());
  fileDialog->SetTitle(std::wstring(caption.begin(), caption.end()).c_str());

  fileDialog->Show(enigma::hWnd);

  string res = "";
  IShellItem *psi;

  if (SUCCEEDED(fileDialog->GetResult(&psi))) {
    LPWSTR wideres;
    psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &wideres);
    psi->Release();

    std::wstring wstr = wideres;
    res = string(wstr.begin(), wstr.end());
  }

  return res;
}

string get_directory_alt(string message, string root, bool modern, string caption) {
  //standard use of the Shell API to browse for folders
  bool f_selected = false;

  char szDir [MAX_PATH];
  BROWSEINFO bi;
  LPITEMIDLIST pidl;
  LPMALLOC pMalloc;

  if (SUCCEEDED (::SHGetMalloc (&pMalloc)))
  {
    ::ZeroMemory (&bi,sizeof(bi));

    bi.lpszTitle = message.c_str();
    bi.hwndOwner = enigma::hWnd;
    bi.pszDisplayName = 0;
    bi.pidlRoot = 0;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
    if (modern) {
    bi.ulFlags |= BIF_EDITBOX | BIF_NEWDIALOGSTYLE;
    }
    gs_cap = caption;
    bi.lpfn =  GetDirectoryAltProc;      //callback to set window caption

    pidl = ::SHBrowseForFolder(&bi);
    if (pidl) {
      if (::SHGetPathFromIDList(pidl, szDir)) {
        f_selected = true;
      }

      pMalloc->Free(pidl);
      pMalloc->Release();
    }
  }

  if (f_selected) {
    return szDir;
  } else {
    return "";
  }
}

}
