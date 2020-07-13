/** Copyright (C) 2008-2017 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton
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

#include <thread>
#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include "Bridges/Win32/WINDOWShandle.h" // enigma::hWnd
#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFshell.h"
#include "Universal_System/estring.h"

using std::string;

namespace enigma_insecure {

void execute_program(string fname, string args, bool wait) {
  SHELLEXECUTEINFOW lpExecInfo;
  tstring tstr_fname = widen(fname);
  tstring tstr_args = widen(args);
  lpExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
  lpExecInfo.lpFile = tstr_fname.c_str();
  lpExecInfo.fMask = SEE_MASK_DOENVSUBST | SEE_MASK_NOCLOSEPROCESS;
  lpExecInfo.hwnd = enigma::hWnd;
  lpExecInfo.lpVerb = NULL;
  lpExecInfo.lpParameters = tstr_args.c_str();
  WCHAR cDir[MAX_PATH];
  GetCurrentDirectoryW(MAX_PATH, cDir);
  lpExecInfo.lpDirectory = cDir;
  lpExecInfo.nShow = SW_SHOW;
  lpExecInfo.hInstApp = (HINSTANCE)SE_ERR_DDEFAIL;
  ShellExecuteExW(&lpExecInfo);
  if (wait && lpExecInfo.hProcess != NULL) {
    while (DWORD eventSignalId = MsgWaitForMultipleObjects(1, &lpExecInfo.hProcess, false, INFINITE, QS_ALLEVENTS)) {
      if (eventSignalId == WAIT_OBJECT_0) break;
      enigma::handleEvents();
    }
  }
  if (lpExecInfo.hProcess != NULL)
    CloseHandle(lpExecInfo.hProcess);
}

void execute_shell(string fname, string args) {
  execute_program(fname, args, false);
}

static string thrdout;
static inline void output_thread(HANDLE handle) {
  thrdout = "";
  DWORD dwRead = 0;
  char buffer[BUFSIZ];
  while (ReadFile(handle, buffer, BUFSIZ, &dwRead, NULL) && dwRead) {
    buffer[dwRead] = '\0';
    thrdout.append(buffer, dwRead);
  }
}

string execute_shell_for_output(const string &command) {
  string output;
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
  STARTUPINFOW si = { 0 };
  si.cb = sizeof(STARTUPINFOW);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdError = hStdOutPipeWrite;
  si.hStdOutput = hStdOutPipeWrite;
  si.hStdInput = hStdInPipeRead;
  PROCESS_INFORMATION pi = { 0 };
  if (CreateProcessW(NULL, ctstr_command, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
    CloseHandle(hStdOutPipeWrite);
    CloseHandle(hStdInPipeRead);
    MSG msg;
    HANDLE waitHandles[] = { pi.hProcess, hStdOutPipeRead };
    std::thread outthrd(output_thread, hStdOutPipeRead);
    while (GetMessage(&msg, NULL, 0, 0) &&
      MsgWaitForMultipleObjects(2, waitHandles, false, INFINITE, QS_ALLEVENTS) != WAIT_OBJECT_0) {
      if (msg.message == WM_NCLBUTTONDOWN && msg.wParam == HTCLOSE ||
        msg.message == WM_NCLBUTTONUP && msg.wParam == HTCLOSE) {
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
      } else {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    outthrd.join();
    output = thrdout;
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdOutPipeRead);
    CloseHandle(hStdInPipeWrite);
  }
  return output;
}

} // namespace enigma_insecure

namespace enigma_user {

void url_open(string url) {
  tstring tstr_url = widen(url);
  ShellExecuteW(NULL, L"open", tstr_url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void action_webpage(const string &url) {
  url_open(url);
}

} // namespace enigma_user
