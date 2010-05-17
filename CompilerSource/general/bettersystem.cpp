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

#if TARGET_PLATFORM_ID == OS_WINDOWS
  #include <windows.h>
#endif

#include "../OS_Switchboard.h"

  #if TARGET_PLATFORM_ID == OS_WINDOWS
    typedef DWORD sys_result_type;
  #else
    typedef int sys_result_type;
  #endif


int better_system(string program,string arguments)
{
  sys_result_type exit_status = sys_result_type(-1);
  
  #if TARGET_PLATFORM_ID == OS_WINDOWS
    STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcessInformation;
    
    ZeroMemory(&StartupInfo, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);
    
    ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));
    
    if (CreateProcess(NULL,(CHAR*)(program + " " + arguments).c_str(),NULL,NULL,TRUE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartupInfo,&ProcessInformation ))
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
    
    return exit_status;
  #else
    return system((program + " " + arguments).c_str());
  #endif
}
