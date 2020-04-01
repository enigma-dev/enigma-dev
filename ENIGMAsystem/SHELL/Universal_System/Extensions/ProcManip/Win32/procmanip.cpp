#include <windows.h>
#include <tlhelp32.h>
#include <string>

using std::string;

extern "C" unsigned long cocoa_pid_from_wid(unsigned long wid);

static inline void path_and_ppid_from_pid(pid_t *ppid, char **path) {
  HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = { 0 };
  pe.dwSize = sizeof(PROCESSENTRY32);
  if(Process32First(h, &pe)) {
    do {
      if (pe.th32ProcessID == pid) {
        *(ppid) = pe.th32ParentProcessID;
        *(path) = pe.szExeFile;
      }
    } while(Process32Next(h, &pe));
  }
  CloseHandle(h);
}

namespace enigma_user {

string path_from_pid(pid_t pid) {
  pid_t ppid; char *path;
  path_and_ppid_from_pid(&ppid, &path);
  return path;
}

pid_t ppid_from_pid(pid_t pid) {
  pid_t ppid; char *path;
  path_and_ppid_from_pid(&ppid, &path);
  return ppid;
}

pid_t pid_from_wid(string wid) {
  DWORD dwProcessId;
  unsigned long long ull_wid = stoull(wid, nullptr, 10);
  void *voidp_wid = reinterpret_cast<void *>(ull_wid);
  HWND hwnd_wid = reinterpret_cast<HWND>(voidp_wid);
  GetWindowThreadProcessId(hwnd_wid, &dwProcessId);
  unsigned long ul_pid = reinterpret_cast<unsigned long>(dwProcessId);
  return reinterpret_cast<pid_t>(ul_pid);
}

}
