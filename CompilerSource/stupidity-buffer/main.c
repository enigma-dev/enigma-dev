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
  
  if (CreateProcess(jname,(CHAR*)param,NULL,NULL,TRUE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartupInfo,&ProcessInformation ))
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
  char buf[2048];
  puts("Scouring for Java");
  const char *jpath = "java";
    sprintf(buf, "\"%s\" -version", jpath);
  int a = better_system(jpath, buf);
  if (a)
  {
    jpath = "\\Program Files (x86)\\Java\\jre6\\bin\\java.exe";
    sprintf(buf, "\"%s\" -version", jpath);
      a = better_system(jpath, buf);
    if (a)
    {
      jpath = "\\Program Files\\Java\\jre6\\bin\\java.exe";
      sprintf(buf, "\"%s\" -version", jpath);
        a = better_system(jpath, buf);
      if (a)
      {
        jpath = "C:\\Program Files\\Java\\jre6\\bin\\java.exe"; //At this point, they're probably running something that uses C:.
        sprintf(buf, "\"%s\" -version", jpath);
          a = better_system(jpath, buf);
      }
    }
  }
  if (!a)
  {
    sprintf(buf, "\"%s\" -jar l*.jar", jpath);
    printf("Calling `%s`\n\n",buf);
    better_system(jpath,buf);
  }
  else
    puts("Could not find Java.exe. You could try adding it to your PATH variable.");
  system("pause");
  return 0;
}
