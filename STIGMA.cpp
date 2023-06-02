#include <algorithm>

#include "shared/apifilesystem/filesystem.hpp"
#include "shared/apiprocess/process.hpp"

int main() {  
  ngs::fs::directory_set_current_working(ngs::fs::executable_get_directory());
  std::string dir = ngs::fs::executable_get_directory();
  std::replace(dir.begin(), dir.end(), '\\', '/');
  if (ngs::fs::file_exists(ngs::fs::executable_get_directory() +  + "LateralGM-Windows-i686.exe")) {
    ngs::ps::spawn_child_proc_id("cmd /c start \"\" ..\\msys64\\msys2_shell.cmd -defterm -mingw32 -no-start -here -lc \"" + dir +  + "LateralGM-Windows-i686.exe\"", false);
  } else if (ngs::fs::file_exists(ngs::fs::executable_get_directory() +  + "LateralGM-Windows-x86_64.exe")) {
    ngs::ps::spawn_child_proc_id("cmd /c start \"\"  ..\\msys64\\msys2_shell.cmd -defterm -mingw64 -no-start -here -lc \"" + dir +  + "LateralGM-Windows-x86_64.exe\"", false);
  }
  return 0;
}
