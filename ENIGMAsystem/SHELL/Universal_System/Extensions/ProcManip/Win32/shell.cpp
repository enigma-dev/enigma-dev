/** Copyright (C) 2008-2017 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton
*** Copyright (C) 2020 Samuel Venable
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

#include "Platforms/General/PFmain.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Bridges/Win32/WINDOWShandle.h"
#include "Universal_System/estring.h"

namespace enigma_user {

void execute_shell(std::string fname, std::string args) {
  wchar_t cDir[MAX_PATH];
  GetCurrentDirectoryW(MAX_PATH, cDir);
  tstring tstr_fname = widen(fname);
  tstring tstr_args = widen(args);
  ShellExecuteW(enigma::hWnd, NULL, tstr_fname.c_str(), tstr_args.c_str(), cDir, SW_SHOW);
}

void execute_shell(std::string operation, std::string fname, std::string args) {
  wchar_t cDir[MAX_PATH];
  GetCurrentDirectoryW(MAX_PATH, cDir);
  tstring tstr_operation = widen(operation);
  tstring tstr_fname = widen(fname);
  tstring tstr_args = widen(args);
  ShellExecuteW(enigma::hWnd, tstr_operation.c_str(), tstr_fname.c_str(), tstr_args.c_str(), cDir, SW_SHOW);
}

std::string execute_shell_for_output(const std::string &command) {
  tstring tstr_command = widen(command);
  wchar_t ctstr_command[32768];
  wcsncpy(ctstr_command, tstr_command.c_str(), 32768);
  BOOL ok = TRUE;
  HANDLE hStdInPipeRead = NULL;
  HANDLE hStdInPipeWrite = NULL;
  HANDLE hStdOutPipeRead = NULL;
  HANDLE hStdOutPipeWrite = NULL;
  SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
  ok = CreatePipe(&hStdInPipeRead, &hStdInPipeWrite, &sa, 0);
  if (ok == FALSE) return "";
  ok = CreatePipe(&hStdOutPipeRead, &hStdOutPipeWrite, &sa, 0);
  if (ok == FALSE) return "";
  STARTUPINFOW si = { };
  si.cb = sizeof(STARTUPINFOW);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdError = hStdOutPipeWrite;
  si.hStdOutput = hStdOutPipeWrite;
  si.hStdInput = hStdInPipeRead;
  PROCESS_INFORMATION pi = { };
  if (CreateProcessW(NULL, ctstr_command, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
    while (WaitForSingleObject(pi.hProcess, 5) == WAIT_TIMEOUT) {
      MSG msg;
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    CloseHandle(hStdOutPipeWrite);
    CloseHandle(hStdInPipeRead);
    char buf[4096] = { };
    DWORD dwRead = 0;
    DWORD dwAvail = 0;
    ok = ReadFile(hStdOutPipeRead, buf, 4095, &dwRead, NULL);
    string str_buf = buf; tstring output = widen(str_buf);
    while (ok == TRUE) {
      buf[dwRead] = '\0';
      OutputDebugStringW(output.c_str());
      _putws(output.c_str());
      ok = ReadFile(hStdOutPipeRead, buf, 4095, &dwRead, NULL);
      str_buf = buf; output += widen(str_buf);
    }
    CloseHandle(hStdOutPipeRead);
    CloseHandle(hStdInPipeWrite);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return shorten(output);
  }
  return "";
}

void execute_program(std::string operation, std::string fname, std::string args, bool wait) {
  SHELLEXECUTEINFOW lpExecInfo;
  tstring tstr_operation = widen(operation);
  tstring tstr_fname = widen(fname);
  tstring tstr_args = widen(args);
  lpExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
  lpExecInfo.lpFile = tstr_fname.c_str();
  lpExecInfo.fMask = SEE_MASK_DOENVSUBST | SEE_MASK_NOCLOSEPROCESS;
  lpExecInfo.hwnd = enigma::hWnd;
  lpExecInfo.lpVerb = tstr_operation.c_str();
  lpExecInfo.lpParameters = tstr_args.c_str();
  wchar_t cDir[MAX_PATH];
  GetCurrentDirectoryW(MAX_PATH, cDir);
  lpExecInfo.lpDirectory = cDir;
  lpExecInfo.nShow = SW_SHOW;
  lpExecInfo.hInstApp = (HINSTANCE)SE_ERR_DDEFAIL;  // WINSHELLAPI BOOL WINAPI result;
  ShellExecuteExW(&lpExecInfo);
  if (wait && lpExecInfo.hProcess != NULL) {
    while (WaitForSingleObject(lpExecInfo.hProcess, 5) == WAIT_TIMEOUT) {
      MSG msg;
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }
  if (lpExecInfo.hProcess != NULL)
    CloseHandle(lpExecInfo.hProcess);
}

void execute_program(std::string fname, std::string args, bool wait) {
  execute_program("open", fname, args, wait);
}

void url_open(std::string url, std::string target, std::string options) {
  tstring tstr_url = widen(url);
  tstring tstr_open = widen("open");
  ShellExecuteW(NULL, tstr_open.c_str(), tstr_url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void url_open_ext(std::string url, std::string target) {
  url_open(url, target);
}

void url_open_full(std::string url, std::string target, std::string options) {
  url_open(url, target, options);
}

void action_webpage(const std::string &url) {
  url_open(url);
}

} // namespace enigma_user
