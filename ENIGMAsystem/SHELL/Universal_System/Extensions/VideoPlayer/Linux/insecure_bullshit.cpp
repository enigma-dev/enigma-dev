#include <cstdlib>
#include <cstring>

#include "Uniersal_System/Extensions/VideoPlayer/insecure_bullshit.h"

#include "Uniersal_System/estring.h"

#include <proc/readproc.h>

using std::string;
using std::vector;
using std::to_string;

namespace enigma_insecure {

string cmd_from_pid(process_t pid) {
  string cmd;
  proc_t proc_info;
  if (!pid_exists(pid)) { return ""; }
  memset(&proc_info, 0, sizeof(proc_info));
  PROCTAB *proc = openproc(PROC_FILLCOM | PROC_PID, &pid);
  if (readproc(proc, &proc_info) != 0) {
    char **args = proc_info.cmdline;
    size_t cnta = 0;
    for (; args[cnta] != NULL && strlen(args[cnta]) != 0; cnta++);
    for (size_t i = 0; i < cnta; i++) {
      if (string_has_whitespace(args[i])) {
        cmd += "\"" + enigma_user::string_replace_all(args[i], "\"", "\\\"") + "\"";
      } else {
        cmd += args[i];
      }
      if (args[i + 1] != NULL) {
        cmd += " ";
      }
    }
  }
  closeproc(proc);
  return cmd;
}

} // namespace enigma_insecure
