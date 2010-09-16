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

#include <string>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "../OS_Switchboard.h"

#if CURRENT_PLATFORM_ID == OS_WINDOWS
  #include <windows.h>
#endif

  #if CURRENT_PLATFORM_ID == OS_WINDOWS
    typedef DWORD sys_result_type;
  #else
    typedef int sys_result_type;
  #endif

int better_system(string program,string arguments, string redirchar = "", const char* redirf = NULL)
{
  sys_result_type exit_status = sys_result_type(-1);
  
  #if CURRENT_PLATFORM_ID == OS_WINDOWS
    STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcessInformation;
    
    ZeroMemory(&StartupInfo, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);
    
    ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));
    
    SECURITY_ATTRIBUTES inheritibility;
        inheritibility.nLength = sizeof(inheritibility);
        inheritibility.lpSecurityDescriptor = NULL;
        inheritibility.bInheritHandle = TRUE;
    
    // Output redirection
    HANDLE of = NULL;
    if (redirf) 
    {
      of = CreateFile(redirf, FILE_WRITE_DATA, FILE_SHARE_READ|FILE_SHARE_WRITE, &inheritibility, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      if (of != NULL)
      {
        StartupInfo.dwFlags = STARTF_USESTDHANDLES;
        StartupInfo.hStdInput = GetStdHandle((DWORD)-10);
        StartupInfo.hStdOutput = GetStdHandle((DWORD)-11);
        StartupInfo.hStdError = GetStdHandle((DWORD)-12);
        
        if (!StartupInfo.hStdInput or StartupInfo.hStdInput == INVALID_HANDLE_VALUE)
        {
          HANDLE conin = CreateFile("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, FALSE, OPEN_EXISTING, 0, NULL);
            if (!conin || conin == INVALID_HANDLE_VALUE) { printf("CreateFile(CONIN$) failed (Error%d)\n", (int)GetLastError()); }
          StartupInfo.hStdInput = conin;
        }
        
        if (!StartupInfo.hStdOutput or StartupInfo.hStdOutput == INVALID_HANDLE_VALUE or !StartupInfo.hStdError or StartupInfo.hStdError == INVALID_HANDLE_VALUE)
        {
          HANDLE conout = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, FALSE, OPEN_EXISTING, 0, NULL);
            if (!conout || conout == INVALID_HANDLE_VALUE) { printf("CreateFile(CONOUT$) failed (Error %d)\n", (int)GetLastError()); }
            
          if (!StartupInfo.hStdOutput or StartupInfo.hStdOutput == INVALID_HANDLE_VALUE)  StartupInfo.hStdInput = conout;
          if (!StartupInfo.hStdError  or StartupInfo.hStdError  == INVALID_HANDLE_VALUE)  StartupInfo.hStdError = conout;
        }
        
        if (redirchar == ">" or redirchar == "1>" or redirchar == "&>") StartupInfo.hStdOutput = of;
        if (redirchar == "2>" or redirchar == "&>") StartupInfo.hStdError = of;
      }
    }
    
    if (CreateProcess(program.c_str(),(CHAR*)("\"" + program + "\" " + arguments).c_str(),NULL,&inheritibility,TRUE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartupInfo,&ProcessInformation ))
    {
      WaitForSingleObject(ProcessInformation.hProcess, INFINITE);
      GetExitCodeProcess(ProcessInformation.hProcess, &exit_status);
      CloseHandle(ProcessInformation.hProcess);
      CloseHandle(ProcessInformation.hThread);
    }
    else
    {
      cout << "ENIGMA: Failed to create process `" << program << "'.\n";
      exit_status = DWORD(-1);
    }
    
    if (of) CloseHandle(of);
  #else
    string rd = redirf ? ((redirchar == "&>") ? " >" + string(redirf) + " 2>&1" : " " + redirchar + " " + string(redirf)) : "";
    string cmd = program + " " + arguments + rd;
    cout << cmd << endl;
    exit_status = system(cmd.c_str());
    cout << flush;
  #endif
  
  return exit_status;
}
