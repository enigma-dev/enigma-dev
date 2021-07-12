/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development environment.                  **
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
#include <climits>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "bettersystem.h"
#include "OS_Switchboard.h"
#include "general/parse_basics_old.h"
#include "frontend.h"

#if CURRENT_PLATFORM_ID == OS_FREEBSD || CURRENT_PLATFORM_ID == OS_DRAGONFLY
    #include <sys/user.h>
    #include <libutil.h>
    #if CURRENT_PLATFORM_ID == OS_DRAGONFLY
        #include <sys/param.h>
        #include <sys/sysctl.h>
        #include <kvm.h>
    #endif
#endif

using std::string;

#if CURRENT_PLATFORM_ID == OS_DRAGONFLY
kvm_t *kd = nullptr;
#endif

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
  #define cut_termining_string(x) ((x)-1)
#endif
inline string cutout_block(const char* source, pt& pos, bool& qed)
{
  pt spos = pos;
  qed = false;
  string ret;

  if (source[pos] == '"' and (qed = true))
  { DoubleQuoteInBlock:
    while (source[++pos] and source[pos] != '"')
      if (source[pos] == '\\') pos++;
    ret += string(source + cut_beginning_string(spos), cut_termining_string(pos-spos));
    pos++;
  }
  else if (source[pos] == '\'' and (qed = true))
  { SingleQuoteInBlock:
    while (source[++pos] and source[pos] != '\'')
      if (source[pos] == '\\') pos++;
    ret += string(source + cut_beginning_string(spos), cut_termining_string(pos-spos));
    pos++;
  }
  else
  {
    while (source[pos] and !is_useless(source[pos]))
      if (source[++pos] == '"') {
        ret = string(source + spos, pos - spos); spos=pos;
        goto DoubleQuoteInBlock;
      }
      else if (source[pos] == '\'') {
        ret = string(source + spos, pos - spos); spos=pos;
        goto SingleQuoteInBlock;
      }
    ret = string(source + spos, pos - spos);
  }

  while (is_useless(source[pos])) pos++;

  return ret;
}

void myReplace(std::string& str, const std::string& oldStr, const std::string& newStr)
{
  size_t pos = 0;
  while((pos = str.find(oldStr, pos)) != std::string::npos)
  {
     str.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
}

#if CURRENT_PLATFORM_ID == OS_WINDOWS
    #define byte __windows_byte_workaround
    #include <windows.h>
    #undef byte

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
    myReplace(redirout, "\"", "");
    myReplace(redirerr, "\"", "");


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
      HANDLE handleout = INVALID_HANDLE_VALUE, handleerr = INVALID_HANDLE_VALUE;
      if (redirout != "" or redirerr != "")
      {
        if (redirout != "")
          handleout = CreateFile(redirout.c_str(), FILE_WRITE_DATA, FILE_SHARE_READ|FILE_SHARE_WRITE, &inheritibility, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (redirerr == redirout)
          handleerr = handleout;
        else
          handleerr = CreateFile(redirerr.c_str(), FILE_WRITE_DATA, FILE_SHARE_READ|FILE_SHARE_WRITE, &inheritibility, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (handleout != INVALID_HANDLE_VALUE or handleerr != INVALID_HANDLE_VALUE)
        {
          StartupInfo.dwFlags = STARTF_USESTDHANDLES;
          StartupInfo.hStdInput  = GetStdHandle((DWORD)-10);
          StartupInfo.hStdOutput = handleout != INVALID_HANDLE_VALUE? handleout : GetStdHandle((DWORD)-11);
          StartupInfo.hStdError  = handleerr != INVALID_HANDLE_VALUE? handleerr : GetStdHandle((DWORD)-12);

          if (!StartupInfo.hStdInput or StartupInfo.hStdInput == INVALID_HANDLE_VALUE)
          {
            HANDLE conin = CreateFile("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, FALSE, OPEN_EXISTING, 0, NULL);
              if (!conin || conin == INVALID_HANDLE_VALUE) { cerr << "CreateFile(CONIN$) failed (Error" << (int)GetLastError() << ")" << std::endl; }
            StartupInfo.hStdInput = conin;
          }

          if (!StartupInfo.hStdOutput or StartupInfo.hStdOutput == INVALID_HANDLE_VALUE or !StartupInfo.hStdError or StartupInfo.hStdError == INVALID_HANDLE_VALUE)
          {
            HANDLE conout = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, FALSE, OPEN_EXISTING, 0, NULL);
              if (!conout || conout == INVALID_HANDLE_VALUE) { cerr << "CreateFile(CONIN$) failed (Error" << (int)GetLastError() << ")" << std::endl; }

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
        
        Cenviron_flat.append("LC_ALL=C");
        Cenviron_flat.append(1,0); 
        
        Cenviron_flat.append(1,0);
        Cenviron_use = (void*)Cenviron_flat.c_str();
      }

      cout << "\n\n********* EXECUTE:\n" << parameters << "\n\n";

      DWORD creationFlags = CREATE_DEFAULT_ERROR_MODE;
      if (build_enable_stop)
        creationFlags |= CREATE_NEW_PROCESS_GROUP;
      if (CreateProcess(NULL,(CHAR*)parameters.c_str(),NULL,&inheritibility,TRUE,creationFlags,Cenviron_use,NULL,&StartupInfo,&ProcessInformation))
      {
        DWORD timeout = build_enable_stop ? 10 : INFINITE;
        while (WaitForSingleObject(ProcessInformation.hProcess, timeout) == WAIT_TIMEOUT) {
          if (!build_stopping) continue;
          DWORD pId = GetProcessId(ProcessInformation.hProcess);
          GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, pId);
          WaitForSingleObject(ProcessInformation.hProcess, INFINITE);
          break;
        }
        GetExitCodeProcess(ProcessInformation.hProcess, &result);
        CloseHandle(ProcessInformation.hProcess);
        CloseHandle(ProcessInformation.hThread);
        CloseHandle(handleout);
        if (handleerr != handleout) CloseHandle(handleerr);
      }
      else {
        CloseHandle(handleout);
        if (handleerr != handleout) CloseHandle(handleerr);
        cerr << "ENIGMA: Failed to create process `" << ename << "`: error " << (int)GetLastError() << ".\nCommand line: `" << parameters.c_str() << "`";
        return -1;
      }
      return (int)result;
    }

    int e_execp(const char* cmd, string path)
    {
      for (size_t pos = path.find(":"); pos != string::npos; pos = path.find(":", pos + 1))
        path.replace(pos, 1, ";");
      path.insert(0, "PATH=");
      path += ";"; path += getenv("PATH");
      const char *eCenviron[2] = { path.c_str(), NULL };
      return e_exec(cmd, eCenviron);
    }
#else
    #include <vector>
    #include <algorithm>
    #include <cstdlib>
    #include <csignal>

    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/wait.h>
    #include <sys/stat.h>
    #include <sys/types.h>

    extern char **environ;
    const mode_t laxpermissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

#if CURRENT_PLATFORM_ID == OS_MACOSX
    #include <libproc.h>
    #include <sys/proc_info.h>
    #include <crt_externs.h>
    #define environ (*_NSGetEnviron())
    extern char **environ;
#endif

    using std::vector;

#if CURRENT_PLATFORM_ID == OS_FREEBSD
    vector<pid_t> ProcIdFromParentProcId(pid_t parentProcId) {
      vector<pid_t> vec; int cntp;
      if (kinfo_proc *proc_info = kinfo_getallproc(&cntp)) {
        for (int i = 0; i < cntp; i++) {
          if (proc_info[i].ki_ppid == parentProcId) {
            vec.push_back(proc_info[i].ki_pid);
          }
        }
        free(proc_info);
      }
      return vec;
    }
#elif CURRENT_PLATFORM_ID == OS_DRAGONFLY
    vector<pid_t> ProcIdFromParentProcId(pid_t parentProcId) {
      char errbuf[_POSIX2_LINE_MAX];
      vector<pid_t> vec; kinfo_proc *proc_info = nullptr; 
      const char *nlistf, *memf; nlistf = memf = "/dev/null";
      kd = kvm_openfiles(nlistf, memf, nullptr, O_RDONLY, errbuf); if (!kd) return vec;
      int cntp = 0; if ((proc_info = kvm_getprocs(kd, KERN_PROC_ALL, 0, &cntp))) {
        for (int i = 0; i < cntp; i++) {
          if (proc_info[i].kp_pid >= 0 && proc_info[i].kp_ppid >= 0 && 
            proc_info[i].kp_ppid == parentProcId) {
            vec.push_back(proc_info[i].kp_pid);
          }
        }
        free(proc_info);
      }
      return vec;
    }
#elif CURRENT_PLATFORM_ID == OS_MACOSX
    pid_t ParentProcIdFromProcId(pid_t procId) {
      proc_bsdinfo proc_info;
      if (proc_pidinfo(procId, PROC_PIDTBSDINFO, 0, &proc_info, sizeof(proc_info)) > 0) {
        return proc_info.pbi_ppid;
      }
      return -1;
    }

    vector<pid_t> ProcIdFromParentProcId(pid_t parentProcId) {
      vector<pid_t> vec;
      int cntp = proc_listpids(PROC_ALL_PIDS, 0, nullptr, 0);
      vector<pid_t> proc_info(cntp);
      std::fill(proc_info.begin(), proc_info.end(), 0);
      proc_listpids(PROC_ALL_PIDS, 0, &proc_info[0], sizeof(pid_t) * cntp);
      for (int i = cntp; i > 0; i--) {
        if (proc_info[i] == 0) { continue; }
        if (parentProcId == ParentProcIdFromProcId(proc_info[i])) {
          vec.push_back(proc_info[i]);
        }
      }
      return vec;
    }
#endif
#if CURRENT_PLATFORM_ID == OS_FREEBSD || CURRENT_PLATFORM_ID == OS_DRAGONFLY || CURRENT_PLATFORM_ID == OS_MACOSX
    void WaitForAllChildrenToDie(pid_t pid, int *status) {
      vector<pid_t> procId = ProcIdFromParentProcId(pid);
      if (procId.size()) {
        for (int i = 0; i < procId.size(); i++) {
          if (procId[i] == 0) { break; }
          WaitForAllChildrenToDie(procId[i], status);
          waitpid(procId[i], status, 0);
        }
      }
    }
#elif CURRENT_PLATFORM_ID == OS_LINUX
    void WaitForAllChildrenToDie(pid_t pid, int *status) {
      waitpid(pid, status, __WALL);
    }
#endif

    void path_coerce(string &ename)
    {
      char* a = getenv("PATH");
      size_t len = strlen(a) + 1;
      char* b = (char*)malloc(len);
      memcpy(b,a,len);
      a = b;

      struct stat st;
      const char delim[] = ":";
      char *c = strtok(b, delim);
      while(c)
      {
        string fn = string(c) + "/" + ename;
        if (!stat(fn.c_str(), &st))
        {
          ename = fn;
          break;
        }
        c = strtok(NULL, delim);
      }

      free(a);
    }

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
      path_coerce(ename);

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
            if (!(argv = (char**)realloc(argv, sizeof(char*) * (argcmax+1))))
              return 0;
          }
          argv[argc++] = np;
        }
      }

      // Cap our parameters
      argv[argc] = NULL;

      cout << "\n\n*********** EXECUTE \n";
      for (char **i = argv; *i; i++)
        cout << "  `" << *i << "`\n";
      cout << ("\n\n");

      int result = -1;
      pid_t fk = fork();
      if (build_enable_stop)
        setpgid(0,0); // << new process group

      if (!fk)
      {
        // Redirect STDIN
        // Background process groups get SIGTTIN if
        // reading from the terminal.
        int infd = open("/dev/null", O_RDONLY);
        dup2(infd, STDIN_FILENO);

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
          if (dup(filedes) == -1) {}
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
        else if (redirerr != redirout) {
          close(STDERR_FILENO);
          if (dup(creat(redirerr.c_str(),laxpermissions)) == -1) {}
        }

        char** usenviron;
        if (Cenviron)
        {
          size_t envl = 1;
          for (char** i = environ;  *i; i++, envl++);
          for (char** i = (char**)Cenviron; *i; i++, envl++);
          usenviron = new char*[envl];
          char ** dest = usenviron;
          for (char ** i = (char**)Cenviron; *i; i++) *dest++ = *i;
          for (char ** i = environ;  *i; i++) *dest++ = *i;
          *dest = NULL;
        }
        else usenviron = environ;

        execve(ename.c_str(), (char*const*)argv, (char*const*)usenviron);
        exit(-1);
      }

      while (!waitpid(fk,&result,build_enable_stop?WNOHANG:0)) {
        if (build_stopping) {
          kill(-fk,SIGINT); // send CTRL+C to process group
          // wait for entire process group to signal,
          // important for GNU make to stop outputting
          // before run buttons are enabled again
          WaitForAllChildrenToDie(-fk, &result);
          break;
        }
        usleep(10000); // hundredth of a second
      }
      for (char** i = argv+1; *i; i++)
        free(*i);
      free(argv);
      return WEXITSTATUS(result);
    }

    int e_execp(const char* cmd, string path)
    {
      cout << "TRUE\n\n";
      path.insert(0, "PATH=");
      if (path != "PATH=") path += ":";
      path += getenv("PATH") ? : "";
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

