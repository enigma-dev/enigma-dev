#include <string>
#include <thread>
#include <chrono>
#include <cstdint>

#include "Platforms/platforms_mandatory.h"
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

// execute process from the shell, return process id
PROCESS ProcessExecute(std::string command) {
  return CrossProcess::ProcessExecute(command.c_str());
}

// execute process from the shell async, return process id
PROCESS ProcessExecuteAsync(std::string command) {
  return CrossProcess::ProcessExecuteAsync(command.c_str());
}

//  get whether executed process has quit based on process id
bool CompletionStatusFromExecutedProcess(PROCESS procIndex) {
  return CrossProcess::CompletionStatusFromExecutedProcess(procIndex);
}

// write to executed process standard input file descriptor based on process id
void ExecutedProcessWriteToStandardInput(PROCESS procIndex, std::string input) {
  CrossProcess::ExecutedProcessWriteToStandardInput(procIndex, input.c_str());
}

// read from executed process standard output file descriptor based on process id
std::string ExecutedProcessReadFromStandardOutput(PROCESS procIndex) {
  return CrossProcess::ExecutedProcessReadFromStandardOutput(procIndex);
}

// free executed process standard input string based on process id
void FreeExecutedProcessStandardInput(PROCESS procIndex) {
  CrossProcess::FreeExecutedProcessStandardInput(procIndex);
}

// free executed process standard ouptut string based on process id
void FreeExecutedProcessStandardOutput(PROCESS procIndex) {
  CrossProcess::FreeExecutedProcessStandardOutput(procIndex);
}

// get process id from self
PROCID ProcIdFromSelf() {
  return CrossProcess::ProcIdFromSelf();
}

// get parent process id from self
PROCID ParentProcIdFromSelf() {
  return CrossProcess::ParentProcIdFromSelf();
}

// get whether process exists based on process id
bool ProcIdExists(PROCID procId) {
  return CrossProcess::ProcIdExists(procId);
}

// kill process based on process id, return whether succeeded
bool ProcIdKill(PROCID procId) {
  return CrossProcess::ProcIdKill(procId);
}

// get executable image file path from self
std::string ExecutableFromSelf() {
  return CrossProcess::ExecutableFromSelf();
}

// get executable image file path from process id
std::string ExeFromProcId(PROCID procId) {
  return CrossProcess::ExeFromProcId(procId);
}

// get current working directory from process id
std::string CwdFromProcId(PROCID procId) {
  return CrossProcess::CwdFromProcId(procId);
}

// get process info from process id
PROCINFO ProcInfoFromProcId(PROCID procId) {
  return CrossProcess::ProcInfoFromProcId((PROCID)procId);
}

// free process info data from memory
void FreeProcInfo(PROCINFO procInfo) {
  CrossProcess::FreeProcInfo(procInfo);
}

// create a list of all process id's
PROCLIST ProcListCreate() {
  return CrossProcess::ProcListCreate();
}

// get process id from process list at index
PROCID ProcessId(PROCLIST procList, i) {
  return CrossProcess::ProcessId(procList, i);
}

// get amount of process id's in process list
int ProcessIdLength(PROCLIST procList) {
  return CrossProcess::ProcessIdLength(procList);
}

// free list of process id's from memory
void FreeProcList(PROCLIST procList) {
  CrossProcess::FreeProcList(procList);
}

// get executable image file path from process info data
std::string ExecutableImageFilePath(PROCINFO procInfo) {
  return CrossProcess::ExecutableImageFilePath(procInfo);
}

// get current working directory ffrom process info data
std::string CurrentWorkingDirectory(PROCINFO procInfo) {
  return CrossProcess::CurrentWorkingDirectory(procInfo);
}

// get parent processs id from process info data
PROCID ParentProcessId(PROCINFO procInfo) {
  return CrossProcess::ParentProcessId(procInfo);
}

// get child process id from process info data at index
PROCID ChildProcessId(PROCINFO procInfo, int i) {
  return CrossProcess::ChildProcessId(procInfo, i);
}

// get amount of child processes from process info data
int ChildProcessIdLength(PROCINFO procInfo) {
  return CrossProcess::ChildProcessIdLength(procInfo);
}

// get command line argument from process info data at index
std::string CommandLine(PROCINFO procInfo, int i) {
  return CrossProcess::CommandLine(procInfo, i);
}

// get amount of command line arguments from process info data
int CommandLineLength(PROCINFO procInfo) {
  return CrossProcess::CommandLineLength(procInfo);
}

// get environment variable (NAME=VALUE) from process info at index
std::string Environment(PROCINFO procInfo, int i) {
  return CrossProcess::Environment(procInfo, i);
}

// get amount of anvironment variables from process info at index
int EnvironmentLength(PROCINFO procInfo) {
  return CrossProcess::EnvironmentLength(procInfo);
}

// get current working directory
std::string DirectoryGetCurrentWorking() {
  return CrossProcess::DirectoryGetCurrentWorking();
}

// set current working directory based on a given dname
double DirectorySetCurrentWorking(std::string dname) {
  return CrossProcess::DirectorySetCurrentWorking(dname.c_str());
}

// get the environment variable of the given name
std::string EnvironmentGetVariable(std::string name) {
  return CrossProcess::EnvironmentGetVariable(name.c_str());
}

// set the environment variable with the given name and value
bool EnvironmentSetVariable(std::string name, std::string value) {
  return CrossProcess::EnvironmentSetVariable(name.c_str(), value.c_str());
}

// unset the environment variable with the given name
bool EnvironmentUnsetVariable(std::string name) {
  return CrossProcess::EnvironmentUnsetVariable(name.c_str());
}

#if defined(XPROCESS_GUIWINDOW_IMPL)
// get owned window id string from process info at index
wid_t OwnedWindowId(PROCINFO procInfo, int i) {
  return CrossProcess::OwnedWindowId(procInfo, i);
}

// get amount of owned window id's from process info at index
int OwnedWindowIdLength(PROCINFO procInfo) {
  return CrossProcess::OwnedWindowIdLength(procInfo);
}

// get whether a process exists based on one of its window id's
bool WindowIdExists(wid_t winId) {
  return CrossProcess::WindowIdExists((WINDOWID)winId.c_str());
}

// kill a process based on one of its window id's, return whether succeeded
bool WindowIdKill(wid_t winId) {
  return CrossProcess::WindowIdKill((WINDOWID)winId.c_str());
}

// return a window id from native window handle
wid_t WindowIdFromNativeWindow(window_t window) {
  return CrossProcess::WindowIdFromNativeWindow((window_t)(void *)window);
} 
#endif

} // namespace enigma_user
