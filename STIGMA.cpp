#include <algorithm>
#include <thread>
#include <chrono>

#inlcude <cstdlib>
#include <cstdio>

#include "shared/apifilesystem/filesystem.hpp"
#include "shared/apiprocess/process.hpp"

int main() {
  unsigned long pid = 0;
  ngs::fs::directory_set_current_working(ngs::fs::executable_get_directory());
  std::string dir = ngs::fs::executable_get_directory();
  std::replace(dir.begin(), dir.end(), '\\', '/');
  if (ngs::fs::file_exists(ngs::fs::executable_get_directory() + "LateralGM-Windows-i686.exe")) {
    pid = ngs::ps::spawn_child_proc_id("cmd /c start \"\" ..\\msys64\\msys2_shell.cmd -defterm -mingw32 -no-start -here -lc \"" + dir + "LateralGM-Windows-i686.exe\"", false);
    ngs::ps::proc_id_kill(pid);
  } else if (ngs::fs::file_exists(ngs::fs::executable_get_directory() + "LateralGM-Windows-x86_64.exe")) {
    pid = ngs::ps::spawn_child_proc_id("cmd /c start \"\"  ..\\msys64\\msys2_shell.cmd -defterm -mingw64 -no-start -here -lc \"" + dir + "LateralGM-Windows-x86_64.exe\"", false);
  }
  if (pid) {
    while (!ngs::ps::child_proc_id_is_complete(pid)) {
      printf("%s", ngs::ps::read_from_stdout_for_child_proc_id(pid));
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      system("cls");
    }
    ngs::ps::free_stdout_for_child_proc_id(pid);
    ngs::ps::free_stdin_for_child_proc_id(pid);
  }
  return 0;
}
