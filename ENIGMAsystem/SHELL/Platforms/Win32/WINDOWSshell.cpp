#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include "Bridges/Win32/WINDOWShandle.h" // enigma::hWnd
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

void execute_shell(string fname, string args) {
  execute_program(fname, args, false);
}

string execute_shell_for_output(const string &command) {
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
    char buffer[4096] = { };
    DWORD dwRead = 0;
    ok = ReadFile(hStdOutPipeRead, buffer, 4095, &dwRead, NULL);
    while (ok == TRUE) {
      buffer[dwRead] = 0;
      ok = ReadFile(hStdOutPipeRead, buffer, 4095, &dwRead, NULL);
    }
    CloseHandle(hStdOutPipeRead);
    CloseHandle(hStdInPipeWrite);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return shorten(widen(buffer));
  }
  return "";
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
