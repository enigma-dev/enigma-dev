/** Copyright (C) 2008-2013 Josh Ventura
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
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

#include "settings-parse/eyaml.h"

string fixdrive(string p)
{
  if (p[0] != '\\' and p[0] != '/')
    return p;

  char buf[3];
  GetCurrentDirectory(3,buf);

  if (((*buf >= 'A' and *buf <= 'Z') or (*buf >= 'a' and *buf <= 'z')) and buf[1] == ':')
    return string(buf,2) + p;
  return p;
}

int better_system(const char* jname, const char* param, const char *direc = NULL)
{
  DWORD exit_status;

  if (direc)
    SetCurrentDirectory(direc);

  STARTUPINFO StartupInfo;
  PROCESS_INFORMATION ProcessInformation;

  ZeroMemory(&StartupInfo, sizeof(StartupInfo));
  StartupInfo.cb = sizeof(StartupInfo);

  ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));

  char buf[2048];
  sprintf(buf, "\"%s\" %s", jname, param);
  if (CreateProcess(0,(CHAR*)buf,NULL,NULL,TRUE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartupInfo,&ProcessInformation ))
  {
    WaitForSingleObject(ProcessInformation.hProcess, INFINITE);
    GetExitCodeProcess(ProcessInformation.hProcess, &exit_status);
    CloseHandle(ProcessInformation.hProcess);
    CloseHandle(ProcessInformation.hThread);
  }
  else
    return -1;
  return exit_status;
}

typedef const char* const EMessage;
EMessage
 java_not_found =
  "Could not find Java.exe. Please install Sun's Java runtime environment so LGM can run.\r\n"
  "http://www.java.com/en/download/manual.jsp\r\n\r\n"
  "If you already have Java, and believe you have received this message in error, you could "
  "try adding it to your system PATH variable.";

#define fixFont(hwnd) SendMessage(hwnd,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),0);

string expand_message(string msg, string arg1)
{
  size_t p = msg.find("%s");
  if (p != string::npos)
    msg.replace(p,2,arg1);
  return msg;
}

char drive_letter[4] = { '\\', 0, 0, 0 };
static int keepgoing; HWND dlb = NULL, cbb = NULL;
LRESULT CALLBACK getproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

  /*  Switch according to what type of message we have received  */
        printf("Message %d %s %d\n",msg,msg==WM_COMMAND?"==":"!=",WM_COMMAND);
  switch (msg) {
    case WM_COMMAND:
        puts("Notified.");
        if (HIWORD(wParam) == BN_CLICKED)
        {
          if (LOWORD(wParam) == 4)
          {
            int sel = ComboBox_GetCurSel(cbb);
            if (sel) ComboBox_GetText(cbb,drive_letter,4);
            else drive_letter[0] = '\\', drive_letter[1] = drive_letter[2] = drive_letter[3] = 0;
          }
          keepgoing = 0;
          DestroyWindow(dlb);
          break;
        }
      break;
    case WM_CLOSE:
      keepgoing = 0;
      break;
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}
void get_new_drive_letter()
{
  WNDCLASSEX wndclass;
    wndclass.cbSize         = sizeof(wndclass);
    wndclass.style          = 0;
    wndclass.lpfnWndProc    = getproc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = NULL;
    wndclass.hIcon          = LoadIcon(NULL, IDI_QUESTION);
    wndclass.hIconSm        = LoadIcon(NULL, IDI_QUESTION);
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH)COLOR_WINDOW;
    wndclass.lpszClassName  = "DrivePicker";
    wndclass.lpszMenuName   = NULL;

  if (!RegisterClassEx(&wndclass))
    return;

  const int WIDTH = 320, HEIGHT = 96;

  RECT n; n.top = 0, n.bottom = HEIGHT, n.left = 0, n.right = WIDTH;
  AdjustWindowRect(&n,WS_BORDER|WS_CAPTION,false);
  dlb = CreateWindow("DrivePicker", "Drive Selection",
      WS_OVERLAPPED,
      CW_USEDEFAULT, CW_USEDEFAULT,
      n.right - n.left, n.bottom - n.top,
      NULL, NULL, NULL , NULL);

  SIZE tsz; int y = 4;
  LPCSTR tt = "Please select a drive onto which ENIGMA will install MinGW:";
  HWND tr = CreateWindow("STATIC",tt,WS_CHILD,4,y,WIDTH-8,32,dlb,(HMENU)(1),NULL,NULL);
  fixFont(tr); ShowWindow(tr,SW_SHOW); HDC dc = GetDC(tr); GetTextExtentPoint32(dc,tt,strlen(tt),&tsz); ReleaseDC(tr,dc);
  SetWindowPos(tr,NULL,0,0,WIDTH-8,tsz.cy+4,SWP_NOMOVE|SWP_NOZORDER); y += 4 + tsz.cy;

  cbb = CreateWindow("COMBOBOX","Drive",WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SORT,4,y,WIDTH-8,320,dlb,(HMENU)(2),NULL,NULL);
  fixFont(cbb); ShowWindow(cbb,SW_SHOW);
  y += 24 + 4;

  HWND bc = CreateWindow("BUTTON","Cancel",WS_CHILD | BS_DEFPUSHBUTTON,4,y,64,24,dlb,(HMENU)(3),NULL,NULL);
  fixFont(bc); ShowWindow(bc,SW_SHOW);
  HWND bk = CreateWindow("BUTTON","OK",WS_CHILD | BS_DEFPUSHBUTTON,WIDTH-8-64,y,64,24,dlb,(HMENU)(4),NULL,NULL);
  fixFont(bk); ShowWindow(bk,SW_SHOW);
  y += 24 + 4;

  n.top = 0, n.bottom = y, n.left = 0, n.right = WIDTH;
  AdjustWindowRect(&n,WS_BORDER|WS_CAPTION,false);
  SetWindowPos(dlb,NULL,0,0,n.right - n.left, n.bottom - n.top,SWP_NOMOVE|SWP_NOZORDER);

  ComboBox_AddString(cbb,"\\ (Whatever drive ENIGMA is installed on)");
  DWORD drives = GetLogicalDrives();
  char drivename[4] = "A:\\";
  for (int i = 0; i < 26; i++)
    if (drives & (1 << i))
      drivename[0] = 'A' + i, ComboBox_AddString(cbb,drivename);
  ShowWindow(dlb, SW_SHOW);
  UpdateWindow(dlb);

  MSG msg; keepgoing = 1;
  while (keepgoing and GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);    /*  for certain keyboard messages  */
    DispatchMessage(&msg);     /*  send message to WndProc        */
  }
}

string dirpart(string fqp)
{
  size_t lp = fqp.find_last_of("/\\");
  if (lp == string::npos) return "";
  return fqp.erase(lp+1);
}
string filepart(string fqp)
{
  size_t lp = fqp.find_last_of("/\\");
  if (lp == string::npos) return fqp;
  return fqp.substr(lp+1);
}

int main()
{
 /* //Set Env Paths
  char* pPath = getenv("PATH");
  TCHAR cPath[MAX_PATH];
  GetCurrentDirectory(MAX_PATH,cPath);
  stringstream envSS;
  envSS << "PATH=" << pPath << cPath << "\\mingw32\\bin;" << cPath << "\\git\\bin;";
  const string& tmp = envSS.str();
  const char* envPath = tmp.c_str();
  puts(envPath);
  putenv(envPath);*/

  //Look for java
  const char *jpath = "java";

  puts("Scouring for Java");
  {

    char buf[MAX_PATH];
    GetEnvironmentVariable("programfiles", buf, MAX_PATH);
    string pfp = buf; pfp += "\\Java\\jre6\\bin\\java.exe";
    GetEnvironmentVariable("programfiles(x86)", buf, MAX_PATH);
    string pfx86p = buf; pfx86p += "\\Java\\jre6\\bin\\java.exe";

    int a = better_system(jpath, "-version");
    if (a)
    {
      a = better_system(jpath = pfp.c_str(), "-version"); // This should hopefully take care of most of it
      if (a)
      {
        a = better_system(jpath = pfx86p.c_str(), "-version"); //One would think this would take care of the rest
        if (a)
        {
          a = better_system(jpath = "\\Program Files\\Java\\jre6\\bin\\java.exe", "-version");
          if (a)
          {
            a = better_system(jpath = "\\Program Files (x86)\\Java\\jre6\\bin\\java.exe", "-version");
            if (a)
            {
              a = better_system(jpath = "C:\\Program Files\\Java\\jre6\\bin\\java.exe", "-version"); //At this point, they're probably running something that uses C:.
              if (a)
                a = better_system(jpath = "C:\\Program Files (x86)\\Java\\jre6\\bin\\java.exe", "-version"); //What a fucked up configuration. *cough* dazappa *cough*
            }
          }
        }
      }
    }
    if (a)
    {
      puts(java_not_found);
      MessageBox(NULL, java_not_found , "Java Problem", MB_OK);
      system("pause");
      return 0;
    }
  }

  //if init script exists; run it then delete it
  const char *initpath = "init";
  const char *winpatch = "WinPatch.zip";

  GetFileAttributes(initpath);
  if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(initpath) && GetLastError()==ERROR_FILE_NOT_FOUND)  //If init script not found
  {
  }
  else
  {
      puts("Downloading The ENIGMA Repo, please wait...");
      SHELLEXECUTEINFO lpExecInfo;
      lpExecInfo.cbSize  = sizeof(SHELLEXECUTEINFO);
      lpExecInfo.lpFile = "git-bash.bat";
      lpExecInfo.fMask = SEE_MASK_DOENVSUBST|SEE_MASK_NOCLOSEPROCESS;
      lpExecInfo.hwnd = NULL;
      lpExecInfo.lpVerb = "open";
      lpExecInfo.lpParameters = "./init";
      lpExecInfo.lpDirectory = NULL;
      lpExecInfo.nShow = SW_SHOW;
      lpExecInfo.hInstApp = (HINSTANCE) SE_ERR_DDEFAIL ;   //WINSHELLAPI BOOL WINAPI result;
      ShellExecuteEx(&lpExecInfo);

      //wait until a file is finished printing
      if (lpExecInfo.hProcess != NULL)
      {
        ::WaitForSingleObject(lpExecInfo.hProcess, INFINITE);
        ::CloseHandle(lpExecInfo.hProcess);
      }

      DeleteFileA(winpatch);
      DeleteFileA(initpath);
  }

  //Run Lateral GM

  puts("Calling LGM");
  SHELLEXECUTEINFO lpExecInfo;
  lpExecInfo.cbSize  = sizeof(SHELLEXECUTEINFO);
  lpExecInfo.lpFile = "git-bash.bat";
  lpExecInfo.fMask = SEE_MASK_DOENVSUBST|SEE_MASK_NOCLOSEPROCESS;
  lpExecInfo.hwnd = NULL;
  lpExecInfo.lpVerb = "open";
  lpExecInfo.lpParameters = "./run";
  lpExecInfo.lpDirectory = NULL;
  lpExecInfo.nShow = SW_HIDE;
  lpExecInfo.hInstApp = (HINSTANCE) SE_ERR_DDEFAIL ;   //WINSHELLAPI BOOL WINAPI result;
  ShellExecuteEx(&lpExecInfo);

/*  const char *lgmdir = "enigma-dev\\";

  printf("Calling `%s -jar l*.jar`\n\n",jpath);
  clock_t x = clock();
  int res = better_system(jpath,"-jar l*.jar &> enigma_log.log",lgmdir);
  if (res and (clock() - x)/CLOCKS_PER_SEC <= 3)
  {
      printf("Failing that, calling `%s -jar lgm16b4.jar`\n\n",jpath);
      res = better_system(jpath,"-jar lgm16b4.jar &> enigma_log.log",lgmdir);
      if (res and (clock() - x)/CLOCKS_PER_SEC <= 5)
          printf("Java error. Please make sure Java is actually installed and that LGM exists.\r\n\r\n");
  }*/

  //system("pause");
  return 0;
}

static inline bool CopyFile2(const char* x, const char* y)
{
  printf("      => Copy `%s`\n",x);
  return CopyFile(x,y,FALSE);
}