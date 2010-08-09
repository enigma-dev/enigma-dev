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
  if (a) puts("I hope you built ENIGMA yourself, because I sure as hell can't.");
  else { 
    a = better_system(mpath,"ENIGMA");
    if (a) { 
      puts("I found make, and I asked it to build ENIGMA for you,");
      puts("but it couldn't for some reason. I hope you did.");
      puts("Press Enter. If you enter an 'N', I'll just close");
      if (getchar() == 'N') return 0;
    }
  }
  
  puts("Scouring for Java");
  const char *jpath = "java";
  a = better_system(jpath, "-version");
  if (a)
  {
    jpath = "\\Program Files (x86)\\Java\\jre6\\bin\\java.exe";
      a = better_system(jpath, "-version");
    if (a)
    {
      jpath = "\\Program Files\\Java\\jre6\\bin\\java.exe";
        a = better_system(jpath, "-version");
      if (a)
      {
        jpath = "C:\\Program Files\\Java\\jre6\\bin\\java.exe"; //At this point, they're probably running something that uses C:.
          a = better_system(jpath, "-version");
      }
    }
  }
  if (!a)
  {
    printf("Calling `%s -jar l*.jar`\n\n",jpath);
    better_system(jpath,"-jar l*.jar");
  }
  else
    puts("Could not find Java.exe. You could try adding it to your PATH variable.");
  system("pause");
  return 0;
}
