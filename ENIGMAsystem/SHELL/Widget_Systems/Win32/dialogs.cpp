/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Josh Ventura                                             **
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
**  You should have received a copy of the GNU General Public License along     **
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
#include <stdio.h>
using namespace std;
#include "../widgets_mandatory.h"
#include "../../GameSettings.h"

#include "dialogs.h"

void show_error(string errortext,const bool fatal)
{
  if (MessageBox(NULL,("Error in some event or another for some object I'm too lazy to look up: \r\n"+errortext).c_str(),"Error",MB_ABORTRETRYIGNORE | MB_ICONERROR)==IDABORT)
    exit(0);

  if (fatal)
    printf("FATAL ERROR: %s\n",errortext.c_str()),
    exit(0);
  else
    printf("ERROR: %s\n",errortext.c_str());

  //ABORT_ON_ALL_ERRORS();
}

static string gs_cap;
static string gs_def;
static string gs_message;
static bool   gs_form_canceled;
static string gs_str_submitted;

namespace enigma {
  extern HINSTANCE hInstance;
  extern HWND hWndParent;
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

string get_string(string message,string def,string cap)
{
  gs_cap="";
  gs_message=message; gs_def=def;
  DialogBox(enigma::hInstance,"getstringdialog",enigma::hWndParent,GetStrProc);
  return gs_str_submitted;
}

int get_integer(string message,string def,string cap)
{
  gs_cap="";
  gs_message=message; gs_def=def;
  DialogBox(enigma::hInstance,"getstringdialog",enigma::hWndParent,GetStrProc);
  if (gs_str_submitted == "") return 0;
  puts(gs_str_submitted.c_str());
  return atol(gs_str_submitted.c_str());
}

double get_number(string message,string def,string cap)
{
  gs_cap = cap;
  gs_message=message; gs_def=def;
  DialogBox(enigma::hInstance,"getstringdialog",enigma::hWndParent,GetStrProc);
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
  ofn.lStructSize=sizeof(ofn); ofn.hwndOwner=enigma::hWndParent; ofn.hInstance=NULL;
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
string get_save_filename(string filter,string filename,string caption)
{
  filter.append("||");
  const unsigned int l=filter.length();
  for (unsigned int i=0;i<l;i++)
    if (filter[i]=='|') filter[i]=0;

  char fn[MAX_PATH];
  strcpy(fn,filename.c_str());

  OPENFILENAME ofn;
  ofn.lStructSize=sizeof(ofn); ofn.hwndOwner=enigma::hWndParent; ofn.hInstance=NULL;
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

int get_color(int defcolor)
{
    COLORREF defc=(int)defcolor;
    static COLORREF custcs[16];

    CHOOSECOLOR gcol;
    gcol.lStructSize=sizeof(CHOOSECOLOR);
    gcol.hwndOwner=enigma::hWndParent;
    gcol.rgbResult=defc;
    gcol.lpCustColors=custcs;
    gcol.Flags=CC_RGBINIT;
    gcol.lpTemplateName="";

    if (ChooseColor(&gcol))
      return (int)gcol.rgbResult;
    else return defc;
}
