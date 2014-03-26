/** Copyright (C) 2011 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton
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

#include <string>
#include <windows.h>
#include <richedit.h>
#include <stdio.h>
using namespace std;
#include "Widget_Systems/widgets_mandatory.h"
#include "GameSettings.h"

#include "../General/WSdialogs.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00)>>8)
#define __GETB(x) ((x & 0xFF0000)>>16)

static string gs_cap;
static string gs_def;
static string gs_message;
static string gs_username;
static string gs_password;
static bool   gs_form_canceled;
static string gs_str_submitted;

void show_error(string errortext,const bool fatal)
{
  if (MessageBox(NULL,("Error in some event or another for some object: \r\n"+errortext).c_str(),"Error",MB_ABORTRETRYIGNORE | MB_ICONERROR)==IDABORT)
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
  extern string gameInfoText, gameInfoCaption;
  extern int gameInfoBackgroundColor, gameInfoLeft, gameInfoTop, gameInfoWidth, gameInfoHeight;
  extern bool gameInfoEmbedGameWindow, gameInfoShowBorder, gameInfoAllowResize, gameInfoStayOnTop, gameInfoPauseGame;
  HWND infore;
}

static INT_PTR CALLBACK ShowInfoProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg==WM_KEYUP)
  {
	switch(wParam)
	  {
	  case VK_ESCAPE:
		PostMessage(hwndDlg, WM_CLOSE, 0, 0);
		return TRUE;
	  }
  }
  if (uMsg==WM_SIZE) {
	RECT rectParent;
	GetClientRect(hwndDlg, &rectParent);
	MoveWindow(enigma::infore, rectParent.top, rectParent.left, rectParent.right, rectParent.bottom, TRUE); 
  }
  return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
}

static INT_PTR CALLBACK GetStrProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
  if (uMsg==WM_INITDIALOG)
  {
    SetDlgItemText(hwndDlg,1,gs_cap.c_str());
    SetDlgItemText(hwndDlg,12,gs_def.c_str());
    SetDlgItemText(hwndDlg,13,gs_message.c_str());
  }
  if (uMsg==WM_COMMAND)
  {
    if (wParam==2 || wParam==11)
    {
      gs_str_submitted="";
      gs_form_canceled=1;
      EndDialog(hwndDlg,1);
    }
    else if (wParam==10)
    {
      char strget[1024];
      GetDlgItemText(hwndDlg,12,strget,1024);
      gs_str_submitted=strget;
      gs_form_canceled=0;
      EndDialog(hwndDlg,2);
    }
  }
  return 0;
}

static INT_PTR CALLBACK GetLoginProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
  if (uMsg==WM_INITDIALOG)
  {
    SetDlgItemText(hwndDlg,1,gs_cap.c_str());
    SetDlgItemText(hwndDlg,14,gs_username.c_str());
    SetDlgItemText(hwndDlg,15,gs_password.c_str());
  }
  if (uMsg==WM_COMMAND)
  {
    if (wParam==2 || wParam==11)
    {
      gs_str_submitted="";
      gs_form_canceled=1;
      EndDialog(hwndDlg,1);
    }
    else if (wParam==10)
    {
      char strget[1024];
      GetDlgItemText(hwndDlg,14,strget,1024);
	  gs_str_submitted = strget;
      GetDlgItemText(hwndDlg,15,strget,1024);
      gs_str_submitted += string("|") + string(strget);
      gs_form_canceled=0;
      EndDialog(hwndDlg,2);
    }
  }
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
	
	WNDCLASS wc = {CS_VREDRAW|CS_HREDRAW,(WNDPROC)ShowInfoProc,0,0,enigma::hInstance,0,
		0,GetSysColorBrush(COLOR_WINDOW),0,"infodialog"};
	RegisterClass(&wc);
	
	DWORD flags = WS_VISIBLE|WS_POPUP|WS_SYSMENU|WS_TABSTOP|WS_CLIPCHILDREN; // DS_3DLOOK|DS_CENTER|DS_FIXEDSYS
	if (showBorder) {
		flags |= WS_BORDER | WS_DLGFRAME | WS_CAPTION;
	}
	if (stayOnTop) {
		flags |= DS_MODALFRAME; // Same as WS_EX_TOPMOST
	}
	if (allowResize) {
		flags |= WS_SIZEBOX;
	}
	
	// Center Information Window to the Middle of the Screen
	if (left < 0) {
		left = (GetSystemMetrics(SM_CXSCREEN) - width)/2;
	}
	if (top < 0) {
		top = (GetSystemMetrics(SM_CYSCREEN) - height)/2;
	}
	
	HWND main=CreateWindowA("infodialog", TEXT(caption.c_str()),
		flags, left, top, width, height, enigma::hWnd, 0, enigma::hInstance, 0);
		
	if (showBorder) {
		// Set Window Information Icon
		HICON hIcon = LoadIcon(enigma::hInstance, MAKEINTRESOURCE(3));
		if (hIcon) {
			SendMessage(main, WM_SETICON, ICON_SMALL,(LPARAM)hIcon);
			SendMessage(main, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		}
	}
		
	//TODO: Implement embedding to the game window
	enigma::infore=CreateWindowA("RICHEDIT",TEXT("information text"),
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN | ES_READONLY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP,
		0,0,width,height,main,0,enigma::hInstance,0);
		
	// Size the RTF Component to the Window
	RECT rectParent;
	GetClientRect(main, &rectParent);
	MoveWindow(enigma::infore, rectParent.top, rectParent.left, rectParent.right, rectParent.bottom, TRUE); 
	
	// Set RTF Editor Background Color
	SendMessage(enigma::infore, EM_SETBKGNDCOLOR, (WPARAM)0, (LPARAM)RGB(__GETR(bgcolor), __GETG(bgcolor), __GETB(bgcolor)));
	
	// Set RTF Information Text
	SETTEXTEX se;
	se.codepage = CP_ACP;
	se.flags = ST_DEFAULT;
	SendMessage(enigma::infore, EM_SETTEXTEX, (WPARAM)&se, (LPARAM)info.c_str());
		
	//TODO: Figure out how to block if we need to pause the game, otherwise ShowWindowAsync
	ShowWindow(main,SW_SHOWDEFAULT);
	
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
}

void show_info() {
  show_info(enigma::gameInfoText, enigma::gameInfoBackgroundColor, enigma::gameInfoLeft, enigma::gameInfoTop, enigma::gameInfoWidth, enigma::gameInfoHeight, enigma::gameInfoEmbedGameWindow, enigma::gameInfoShowBorder,
	enigma::gameInfoAllowResize, enigma::gameInfoStayOnTop, enigma::gameInfoPauseGame, enigma::gameInfoCaption);
}

int show_message(string str)
{
    MessageBox(enigma::hWnd, str.c_str(), window_get_caption().c_str(), MB_OK);
    return 0;
}

// TODO There's no easy way to do this. Creating a custom form is the only
// solution I could find.
int show_message_ext(string msg, string but1, string but2, string but3)
{
    return 1;
}

bool show_question(string str)
{
    if(MessageBox(enigma::hWnd, str.c_str(), window_get_caption().c_str(), MB_YESNO) == IDYES)
    {
        return true;
    }
    return false;
}

string get_login(string username, string password, string cap)
{
  gs_cap = cap;
  gs_username = username; gs_password = password;
  DialogBox(enigma::hInstance,"getlogindialog",enigma::hWnd,GetLoginProc);
  return gs_str_submitted;
}

string get_string(string message,string def,string cap)
{
  gs_cap = cap;
  gs_message=message; gs_def=def;
  DialogBox(enigma::hInstance,"getstringdialog",enigma::hWnd,GetStrProc);
  return gs_str_submitted;
}

int get_integer(string message,string def,string cap)
{
  gs_cap = cap;
  gs_message=message; gs_def=def;
  DialogBox(enigma::hInstance,"getstringdialog",enigma::hWnd,GetStrProc);
  if (gs_str_submitted == "") return 0;
  puts(gs_str_submitted.c_str());
  return atol(gs_str_submitted.c_str());
}

double get_number(string message,string def,string cap)
{
  gs_cap = cap;
  gs_message=message; gs_def=def;
  DialogBox(enigma::hInstance,"getstringdialog",enigma::hWnd,GetStrProc);
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
  const unsigned int l=filter.length();
  for (unsigned int i=0;i<l;i++)
    if (filter[i]=='|') filter[i]=0;

  char fn[MAX_PATH];
  strcpy(fn,filename.c_str());

  OPENFILENAME ofn;
  ofn.lStructSize=sizeof(ofn); ofn.hwndOwner=enigma::hWnd; ofn.hInstance=NULL;
  ofn.lpstrFilter=filter.c_str(); ofn.lpstrCustomFilter=NULL;
  ofn.nMaxCustFilter=0; ofn.nFilterIndex=0;
  ofn.lpstrFile=fn; ofn.nMaxFile=MAX_PATH;
  ofn.lpstrFileTitle=NULL; ofn.nMaxFileTitle=0;
  ofn.lpstrInitialDir=NULL; ofn.lpstrTitle=caption.length()?caption.c_str():NULL;
  ofn.Flags=OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
  ofn.nFileOffset=0; ofn.nFileExtension=0;
  ofn.lpstrDefExt=NULL; ofn.lCustData=0;
  ofn.lpfnHook=NULL; ofn.lpTemplateName=0;

  bool ret=GetOpenFileName(&ofn);
  return ret==0?"-1":fn;
}

string get_save_filename(string filter, string filename, string caption)
{
  filter.append("||");
  const unsigned int l=filter.length();
  for (unsigned int i=0;i<l;i++)
    if (filter[i]=='|') filter[i]=0;

  char fn[MAX_PATH];
  strcpy(fn,filename.c_str());

  OPENFILENAME ofn;
  ofn.lStructSize=sizeof(ofn); ofn.hwndOwner=enigma::hWnd; ofn.hInstance=NULL;
  ofn.lpstrFilter=filter.c_str(); ofn.lpstrCustomFilter=NULL;
  ofn.nMaxCustFilter=0; ofn.nFilterIndex=0;
  ofn.lpstrFile=fn; ofn.nMaxFile=MAX_PATH;
  ofn.lpstrFileTitle=NULL; ofn.nMaxFileTitle=0;
  ofn.lpstrInitialDir=NULL; ofn.lpstrTitle=caption.length()?caption.c_str():NULL;
  ofn.Flags=OFN_PATHMUSTEXIST;
  ofn.nFileOffset=0; ofn.nFileExtension=0;
  ofn.lpstrDefExt=NULL; ofn.lCustData=0;
  ofn.lpfnHook=NULL; ofn.lpTemplateName=0;

  bool ret=GetSaveFileName(&ofn);
  return ret==0?"-1":fn;
}

int get_color(int defcolor, bool advanced)
{
    COLORREF defc=(int)defcolor;
    static COLORREF custcs[16];

    CHOOSECOLOR gcol;
    gcol.lStructSize=sizeof(CHOOSECOLOR);
    gcol.hwndOwner=enigma::hWnd;
    gcol.rgbResult=defc;
    gcol.lpCustColors=custcs;
	if (advanced) {
		gcol.Flags= CC_FULLOPEN | CC_RGBINIT;
	} else {
		gcol.Flags= CC_RGBINIT;
	}
    gcol.lpTemplateName="";

    if (ChooseColor(&gcol))
      return (int)gcol.rgbResult;
    else return defc;
}

}