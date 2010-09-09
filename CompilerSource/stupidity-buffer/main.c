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
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

string fixdrive(string p)
{
  if (p[0] == '\\' or p[0]=='/')
    return p;
  
  size_t bs = GetCurrentDirectory(0,NULL);
  
  char buf[bs];
  GetCurrentDirectory(bs,buf);
  
  if (((*buf >= 'A' and *buf <= 'Z') or (*buf >= '0' and *buf <= '9')) and buf[1] == ':')
    return string(buf,2) + p;
  return p;
}

int better_system(const char* jname, const char* param)
{
  DWORD exit_status;
  
  STARTUPINFO StartupInfo;
  PROCESS_INFORMATION ProcessInformation;
  
  ZeroMemory(&StartupInfo, sizeof(StartupInfo));
  StartupInfo.cb = sizeof(StartupInfo);
  
  ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));
  
  char buf[2048];
  sprintf(buf, "\"%s\" %s", jname, param);
  if (CreateProcess(jname,(CHAR*)buf,NULL,NULL,TRUE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartupInfo,&ProcessInformation ))
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

int main()
{
  int a;
  puts("Scouring for Make");
  const char *mpath = "make";
  a = better_system(mpath, "--ver");
  if (a) a = better_system(mpath = "mingw32-make", "--ver");
  if (a) a = better_system(mpath = "\\MinGW\\bin\\make.exe", "--ver");
  if (a) a = better_system(mpath = "\\MinGW\\bin\\mingw32-make.exe", "--ver");
  if (a) a = better_system(mpath = "C:\\MinGW\\bin\\mingw32-make.exe", "--ver");
  if (a) a = better_system(mpath = "C:\\MinGW\\bin\\make.exe", "--ver");
  /*
    if (a) a = better_system(mpath = "\\Program Files\\CodeBlocks\\MinGW\\bin\\mingw32-make.exe", "--ver");
    if (a) a = better_system(mpath = "\\Program Files (x86)\\CodeBlocks\\MinGW\\bin\\mingw32-make.exe", "--ver");
    if (a) a = better_system(mpath = "\\Program Files\\CodeBlocks\\MinGW\\bin\\make.exe", "--ver");
    if (a) a = better_system(mpath = "\\Program Files (x86)\\CodeBlocks\\MinGW\\bin\\make.exe", "--ver");
    if (a) a = better_system(mpath = "C:\\Program Files\\CodeBlocks\\MinGW\\bin\\mingw32-make.exe", "--ver");
    if (a) a = better_system(mpath = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\bin\\mingw32-make.exe", "--ver");
    if (a) a = better_system(mpath = "C:\\Program Files\\CodeBlocks\\MinGW\\bin\\make.exe", "--ver");
    if (a) a = better_system(mpath = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\bin\\make.exe", "--ver");
  */
  if (a) // If we didn't find it
  {
    FILE *tf = fopen("winmake.txt","rb"); // Config file
    if (tf) { // If we've been down this road before
      puts("I hope this file is accurate.");
      fclose(tf);
    }
    else // First time run
    {
      if (MessageBox(NULL,"MinGW was not detected on this system. Would you like to install it now? "
                          "If you are certain you have a stable version, you can decline now and help ENIGMA find said "
                          "version later.\n\nIf you are clueless, press \"Yes\".", "Welcome to ENIGMA!", MB_YESNO) == IDYES)
      {
        if (MessageBox(NULL,"ENIGMA will now run the MinGW installer. It should default to C:\\MinGW, or whatever your main drive is in place of C:\\. "
                            "Such is the recommended location (it's easiest to find).\n\nWhen prompted, please check that you would like three items:\n"
                            " = The GCC C Compiler (If it's on the list, which it probably won't be)\n"
                            " = The G++ C++ Compiler (Make sure this is checked! It's probably called just \"g++ compiler\")\n"
                            " = The GDB GNU Debugging Program (If this isn't on the list, don't worry)", "MinGW Install", MB_OKCANCEL) == IDCANCEL)
        {
          MessageBox(NULL, "Don't you cancel on me. <__<\"\nAll you had to do was run through the installer.\n\n*sigh*, We'll see how this works for you, then I'll ask you again next time.", "ENIGMA", MB_OK);
          goto confused_cancel;
        }
        else
        {
          int mgi = better_system("Autoconf\\MinGW.exe","");
          if (!mgi) puts("Thanks for installing!");
          else {
            puts("Somehow, I don't think you installed it.\nContinuing anyway... Will check again next run.");
            goto confused_cancel;
          }
        }
      }
      if (tf = fopen("winmake.txt","wb")) { fputs(mpath, tf); fclose(tf); }
      else puts("\nI believe there's something wrong with your system. Ignoring and continuing...\n");
      confused_cancel: ;
    }
  }
  else { 
    printf("Make detected. Accessile from `%s`. Informing the Environment.\n\n", mpath);
    if (FILE *tf = fopen("winmake.txt","wb")) { fputs(fixdrive(mpath).c_str(), tf); fclose(tf); }
  }
  
  puts("Scouring for Java");
  const char *jpath = "java";
  
  char buf[MAX_PATH];
  DWORD WINAPI GetEnvironmentVariable("programfiles", buf, MAX_PATH);
  string pfp = buf; pfp += "\\Java\\jre6\\bin\\java.exe";
  DWORD WINAPI GetEnvironmentVariable("programfiles(x86)", buf, MAX_PATH);
  string pfx86p = buf; pfx86p += "\\Java\\jre6\\bin\\java.exe";
  
  a = better_system(jpath, "-version");
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
  if (!a)
  {
    printf("Calling `%s -jar l*.jar`\n\n",jpath);
    better_system(jpath,"-jar l*.jar");
  }
  else
    puts("Could not find Java.exe. Please install Sun's Java runtime environment so LGM can run.\r\nhttp://www.java.com/en/download/manual.jsp\r\n\r\nIf you already have Java, You could try adding it to your PATH variable.");
  system("pause");
  return 0;
}
