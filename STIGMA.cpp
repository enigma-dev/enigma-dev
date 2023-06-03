#include <algorithm>
#include <thread>
#include <chrono>
#include <string>

#include <cstdlib>
#include <cstdio>

#include "shared/apifilesystem/filesystem.hpp"
#include "shared/apiprocess/process.hpp"

#include <windows.h>

int main() {
  auto InstDir = []() {
    char buf[MAX_PATH]; 
    DWORD sz = sizeof(buf);
    if (RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\STIGMA", "InstDir", RRF_RT_REG_SZ, nullptr, &buf, &sz) == ERROR_SUCCESS) {
      return std::string(buf);
    }
    return std::string("");
  };
  unsigned long pid = 0;
  ngs::fs::directory_set_current_working(ngs::fs::executable_get_directory());
  std::string dir = ngs::fs::executable_get_directory();
  std::replace(dir.begin(), dir.end(), '\\', '/');
  ngs::fs::environment_set_variable("PATH", (InstDir() + "\\msys64;" + InstDir() + "\\msys64\\mingw64\\bin;" + InstDir() + "\\msys64\\usr\\local\\bin;" + InstDir() + "\\msys64\\usr\\bin;%PATH%").c_str());
  if (ngs::fs::file_exists(ngs::fs::executable_get_directory() + "LateralGM-Windows-i686.exe")) {
    pid = ngs::ps::spawn_child_proc_id("\"" + dir + "LateralGM-Windows-i686.exe\"", false);
    ngs::ps::proc_id_kill(pid);
  } else if (ngs::fs::file_exists(ngs::fs::executable_get_directory() + "LateralGM-Windows-x86_64.exe")) {
    pid = ngs::ps::spawn_child_proc_id("\"" + dir + "LateralGM-Windows-x86_64.exe\"", false);
  }
  if (pid) {
    while (!ngs::ps::child_proc_id_is_complete(pid)) {
      printf("%s", ngs::ps::read_from_stdout_for_child_proc_id(pid));
      std::this_thread::sleep_for(std::chrono::seconds(1));
      system("cls");
    }
    ngs::ps::free_stdout_for_child_proc_id(pid);
    ngs::ps::free_stdin_for_child_proc_id(pid);
  }
  return 0;
}
