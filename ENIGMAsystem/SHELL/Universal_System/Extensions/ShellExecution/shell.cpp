#include <string>
#include <thread>
#include <chrono>

#include "Platforms/General/PFmain.h"
#include "strings_util.h"

using std::string;

namespace enigma_user {

void execute_program(string fname, string args, bool wait) {
  CrossProcess::PROCID pid = 0;
  string cmd = string_replace_all(fname, "\"", "\\\"");
  cmd = string_replace_all(cmd, "\\", "\\\\");
  cmd = "\"" + cmd + "\" " + args;
  if (wait) {
    pid = CrossProcess::ProcessExecute(cmd.c_str());
  } else {
    pid = CrossProcess::ProcessExecuteAsync(cmd.c_str());
  }
  if (pid > 0) {
    CrossProcess::FreeExecutedProcessStandardInput(pid);
    CrossProcess::FreeExecutedProcessStandardOutput(pid);
  }
}
  
void execute_shell(string fname, string args) {
  execute_program(fname, args, false);
}
  
string execute_shell_for_output(const string &command) {
  CrossProcess::PROCID pid = CrossProcess::ProcessExecute(command.c_str());
  string output;
  if (pid > 0) {
    while (!CrossProcess::CompletionStatusFromExecutedProcess(pid)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    output = CrossProcess::ExecutedProcessReadFromStandardOutput(pid);
    CrossProcess::FreeExecutedProcessStandardInput(pid);
    CrossProcess::FreeExecutedProcessStandardOutput(pid);
  }
  return output;
}

void url_open(string url) {
  #if defined(_WIN32)
  std::wstring wurl = strings_util::widen(url);
  ShellExecuteW(nullptr, L"open", wurl.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
  #else
  execute_program("xdg-open", url, false);
  #endif
}

void action_webpage(const string& url) {
  url_open(url);
}

} // namespace enigma_user
