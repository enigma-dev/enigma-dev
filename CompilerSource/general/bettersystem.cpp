/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Cenvironment.                  **
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
**  applications created by its users, or damages caused by theCenvironment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>

using namespace std;

#include "bettersystem.h"
#include "../OS_Switchboard.h"
#include "../general/parse_basics.h"


inline char* scopy(string& str)
{
  char *np = (char*)malloc(str.length()+1);
  memcpy(np,str.c_str(),str.length()+1);
  return np;
}
#if CURRENT_PLATFORM_ID == OS_WINDOWS
  #define cut_beginning_string(x) (x)
  #define cut_termining_string(x) ((x)+1)
#else
  #define cut_beginning_string(x) ((x)+1)
  #define cut_termining_string(x) (x)
#endif
inline string cutout_block(const char* source, pt& pos, bool& qed)
{
  pt spos = pos;
  qed = false;
  string ret;
  
  if (source[pos] == '"' and (qed = true))
  {
    while (source[++pos] and source[pos] != '"')
      if (source[pos] == '\\') pos++;
    ret = string(source + cut_beginning_string(spos), cut_termining_string(pos-spos));
  }
  else if (source[pos] == '\'' and (qed = true))
  {
    while (source[++pos] and source[pos] != '\'')
      if (source[pos] == '\\') pos++;
    ret = string(source + cut_beginning_string(spos), cut_termining_string(pos-spos));
  }
  else
  {
    while (source[pos] and !is_useless(source[pos]))
      if (source[++pos] == '"')
        while (source[++pos] and source[pos] != '"')
           if (source[pos] == '\\') pos++; else;
      else if (source[pos] == '\'')
        while (source[++pos] and source[pos] != '\'')
           if (source[pos] == '\\') pos++; else;
    ret = string(source + spos, pos - spos);
  }
  
  while (is_useless(source[pos])) pos++;
  
  return ret;
}

#if CURRENT_PLATFORM_ID == OS_WINDOWS
    #include <windows.h>
	  
    int e_exec(const char* fcmd, const char* *Cenviron)
    {
      while (is_useless(*fcmd))
        fcmd++;
      if (!*fcmd)
        return 0;
      
      DWORD result;
      bool qued; pt pos = 0;
      string ename = cutout_block(fcmd, pos, qued);
      if (ename == "")
        return 0;
      
      string parameters = ename, redirout, redirerr;
      
      while (fcmd[pos])
      {
        int redirchr = 0;
        string pcur = cutout_block(fcmd, pos, qued);
        
        if (!redirchr and !qued and
        (  ((pcur[0] == '>' or pcur.substr(0,2) == "1>") and (redirchr = 1))
        or ((pcur.substr(0,2) == "2>") and (redirchr = 2))
        or ((pcur.substr(0,2) == "&>") and (redirchr = 3))
        ))
        {
          string of = (pcur[0] == '>')?
            pcur.length() == 1? cutout_block(fcmd, pos, qued) : pcur.substr(1):
            pcur.length() == 2? cutout_block(fcmd, pos, qued) : pcur.substr(2);
          if (redirchr & 1)
            redirout = of;
          if (redirchr & 2)
            redirerr = of;
        }
        else
        {
          parameters += " " + pcur;
        }
      }
      
      
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
      HANDLE handleout = NULL, handleerr = NULL;
      if (redirout != "" or redirerr != "")
      {
        if (redirout != "")
          handleout = CreateFile(redirout.c_str(), FILE_WRITE_DATA, FILE_SHARE_READ|FILE_SHARE_WRITE, &inheritibility, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (redirerr == redirout)
          handleerr = handleout;
        else
          handleerr = CreateFile(redirerr.c_str(), FILE_WRITE_DATA, FILE_SHARE_READ|FILE_SHARE_WRITE, &inheritibility, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        
        if (handleout or handleerr)
        {
          StartupInfo.dwFlags = STARTF_USESTDHANDLES;
          StartupInfo.hStdInput  = GetStdHandle((DWORD)-10);
          StartupInfo.hStdOutput = handleout? handleout : GetStdHandle((DWORD)-11);
          StartupInfo.hStdError  = handleerr? handleerr : GetStdHandle((DWORD)-12);

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
        }
      }
      
      void* Cenviron_use = NULL;
      string Cenviron_flat;
      if (Cenviron)
      {
        for (const char** i = Cenviron; *i; i++)
          Cenviron_flat.append(*i),
          Cenviron_flat.append(1,0);
        Cenviron_flat.append(1,0);
        Cenviron_use = (void*)Cenviron_flat.c_str();
      }
      
      printf("\n\n********* EXECUTE:\n%s\n\n",parameters.c_str());
      if (CreateProcess(NULL,(CHAR*)parameters.c_str(),NULL,&inheritibility,TRUE,CREATE_DEFAULT_ERROR_MODE,Cenviron_use,NULL,&StartupInfo,&ProcessInformation ))
      {
        WaitForSingleObject(ProcessInformation.hProcess, INFINITE);
        GetExitCodeProcess(ProcessInformation.hProcess, &result);
        CloseHandle(ProcessInformation.hProcess);
        CloseHandle(ProcessInformation.hThread);
      }
      else {
        printf("ENIGMA: Failed to create process `%s': error %d.\nCommand line: `%s`", ename.c_str(), (int)GetLastError(), parameters.c_str());
        return -1;
      }
      return (int)result;
    }
    
    int e_execp(const char* cmd, string path)
    {
      path.insert(0, "PATH=");
      path += ";"; path += getenv("PATH");
      const char *eCenviron[2] = { path.c_str(), NULL };
      return e_exec(cmd, eCenviron);
    }
#else
    #include <fcntl.h> 
    #include <unistd.h>
    #include <sys/wait.h>
    
    const mode_t laxpermissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    
    int e_exec(const char* fcmd, const char* *Cenviron)
    {
      while (is_useless(*fcmd))
        fcmd++;
      if (!*fcmd)
        return 0;
      
      bool qued; pt pos = 0;
      string ename = cutout_block(fcmd, pos, qued);
      if (ename == "")
        return 0;
      
      string redirout, redirerr; int argc = 0, argcmax = 16;
      char* *argv = (char**)malloc(sizeof(char*) * (argcmax+1));
      argv[0] = (char*)ename.c_str(); argc = 1;
      
      while (fcmd[pos])
      {
        int redirchr = 0;
        string pcur = cutout_block(fcmd, pos, qued);
        
        if (!redirchr and !qued and
        (  ((pcur[0] == '>' or pcur.substr(0,2) == "1>") and (redirchr = 1))
        or ((pcur.substr(0,2) == "2>") and (redirchr = 2))
        or ((pcur.substr(0,2) == "&>") and (redirchr = 3))
        ))
        {
          string of = (pcur[0] == '>')?
            pcur.length() == 1? cutout_block(fcmd, pos, qued) : pcur.substr(1):
            pcur.length() == 2? cutout_block(fcmd, pos, qued) : pcur.substr(2);
          if (redirchr & 1)
            redirout = of;
          if (redirchr & 2)
            redirerr = of;
        }
        else
        {
          char *np = scopy(pcur);
          if (argc >= argcmax)
          {
            argcmax += 16;
            if (!realloc(argv, sizeof(char*) * (argcmax+1)))
              return 0;
          }
          argv[argc++] = np;
        }
      }
      
      // Cap our parameters
      argv[argc] = NULL;
      
      int result = -1;
      pid_t fk = fork();
      if (!fk)
      {
        // Redirect STDOUT
        if (redirout == "") {
            int flags = fcntl(STDOUT_FILENO, F_GETFD);
            if (flags != -1)
              flags &= ~FD_CLOEXEC,
              fcntl(STDOUT_FILENO, F_SETFD, flags);
          }
        else {
          close(STDOUT_FILENO);
          int filedes = creat(redirout.c_str(),laxpermissions);
          dup(filedes);
          if (redirerr == redirout)
            dup2(1,2);
        }
        
        // Redirect STDERR
        if (redirerr == "") {
            int flags = fcntl(STDERR_FILENO, F_GETFD);
            if (flags != -1)
              flags &= ~FD_CLOEXEC,
              fcntl(STDERR_FILENO, F_SETFD, flags);
          }
        else if (redirerr != redirout)
          close(STDERR_FILENO),
          dup(creat(redirerr.c_str(),laxpermissions));
        execvpe(ename.c_str(), (char*const*)argv, (char*const*)Cenviron);
      }
      
      waitpid(fk,&result,0);
      for (char** i = argv+1; *i; i++)
        free(*i);
      free(argv);
      return WEXITSTATUS(result);
    }
    
    int e_execp(const char* cmd, string path)
    {
      path.insert(0, "PATH=");
      path += ":"; path += getenv("PATH");
      const char *Cenviron[2] = { path.c_str(), NULL };
      return e_exec(cmd, Cenviron);
    }
#endif

int e_execs(string cmd)                                        { return e_exec(cmd.c_str()); }
int e_execs(string cmd, string cat1)                           { return e_exec((cmd + " " + cat1).c_str()); }
int e_execs(string cmd, string cat1, string cat2)              { return e_exec((cmd + " " + cat1 + " " + cat2).c_str()); }
int e_execs(string cmd, string cat1, string cat2, string cat3) { return e_exec((cmd + " " + cat1 + " " + cat2 + " " + cat3).c_str()); }

int e_execsp(string cmd, string path)                                        { return e_execp(cmd.c_str(), path); }
int e_execsp(string cmd, string cat1, string path)                           { return e_execp((cmd + " " + cat1).c_str(), path); }
int e_execsp(string cmd, string cat1, string cat2, string path)              { return e_execp((cmd + " " + cat1 + " " + cat2).c_str(), path); }
int e_execsp(string cmd, string cat1, string cat2, string cat3, string path) { return e_execp((cmd + " " + cat1 + " " + cat2 + " " + cat3).c_str(), path); }

