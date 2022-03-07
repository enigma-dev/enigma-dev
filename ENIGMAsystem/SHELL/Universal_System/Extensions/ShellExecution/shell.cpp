#include <string>
#include <thread>
#include <chrono>

#include "shell.h"
#include "strings_util.h"

using std::string;

using namespace ::ngs::proc;

namespace enigma_user {

// execute process from the shell, return process id
LOCALPROCID ProcessExecute(string command) {
  return process_execute(command.c_str());
}

// execute process from the shell async, return process id
LOCALPROCID ProcessExecuteAsync(string command) {
  return process_execute_async(command.c_str());
}

//  get whether executed process has quit based on process id
bool CompletionStatusFromExecutedProcess(LOCALPROCID procIndex) {
  return completion_status_from_executed_process(procIndex);
}

// write to executed process standard input file descriptor based on process id
void ExecutedProcessWriteToStandardInput(LOCALPROCID procIndex, string input) {
  executed_process_write_to_standard_input(procIndex, input.c_str());
}

// read from current process standard input
std::string CurrentProcessReadFromStandardInput() {
  return current_process_read_from_standard_input();
}

// read from executed process standard output file descriptor based on process id
string ExecutedProcessReadFromStandardOutput(LOCALPROCID procIndex) {
  return executed_process_read_from_standard_output(procIndex);
}

// free executed process standard input string based on process id
void FreeExecutedProcessStandardInput(LOCALPROCID procIndex) {
  free_executed_process_standard_input(procIndex);
}

// free executed process standard ouptut string based on process id
void FreeExecutedProcessStandardOutput(LOCALPROCID procIndex) {
  free_executed_process_standard_output(procIndex);
}

// get process id from self
PROCID ProcIdFromSelf() {
  return proc_id_from_self();
}

// get parent process id from self
PROCID ParentProcIdFromSelf() {
  return parent_proc_id_from_self();
}

// get whether process exists based on process id
bool ProcIdExists(PROCID procId) {
  return proc_id_exists(procId);
}

// kill process based on process id, return whether succeeded
bool ProcIdKill(PROCID procId) {
  return proc_id_kill(procId);
}

// get executable image file path from self
string ExecutableFromSelf() {
  return executable_from_self();
}

// get executable image file path from process id
string ExeFromProcId(PROCID procId) {
  return exe_from_proc_id(procId);
}

// get current working directory from process id
string CwdFromProcId(PROCID procId) {
  return cwd_from_proc_id(procId);
}

// get process info from process id
PROCINFO ProcInfoFromProcId(PROCID procId) {
  return proc_info_from_proc_id(procId);
}

// get specific process info from process id
PROCINFO ProcInfoFromProcIdEx(PROCID procId, KINFOFLAGS kInfoFlags) {
  return proc_info_from_proc_id_ex(procId, kInfoFlags);
}

// free process info data from memory
void FreeProcInfo(PROCINFO procInfo) {
  free_proc_info(procInfo);
}

// create a list of all process id's
PROCLIST ProcListCreate() {
  return proc_list_create();
}

// get process id from process list at index
PROCID ProcessId(PROCLIST procList, int i) {
  return process_id(procList, i);
}

// get amount of process id's in process list
int ProcessIdLength(PROCLIST procList) {
  return process_id_length(procList);
}

// free list of process id's from memory
void FreeProcList(PROCLIST procList) {
  free_proc_list(procList);
}

// get executable image file path from process info data
string ExecutableImageFilePath(PROCINFO procInfo) {
  return executable_image_file_path(procInfo);
}

// get current working directory ffrom process info data
string CurrentWorkingDirectory(PROCINFO procInfo) {
  return current_working_directory(procInfo);
}

// get parent processs id from process info data
PROCID ParentProcessId(PROCINFO procInfo) {
  return parent_process_id(procInfo);
}

// get child process id from process info data at index
PROCID ChildProcessId(PROCINFO procInfo, int i) {
  return child_process_id(procInfo, i);
}

// get amount of child processes from process info data
int ChildProcessIdLength(PROCINFO procInfo) {
  return child_process_id_length(procInfo);
}

// get command line argument from process info data at index
string CommandLine(PROCINFO procInfo, int i) {
  return commandline(procInfo, i);
}

// get amount of command line arguments from process info data
int CommandLineLength(PROCINFO procInfo) {
  return commandline_length(procInfo);
}

// get environment variable (NAME=VALUE) from process info at index
string Environment(PROCINFO procInfo, int i) {
  return environment(procInfo, i);
}

// get amount of anvironment variables from process info at index
int EnvironmentLength(PROCINFO procInfo) {
  return environment_length(procInfo);
}

// get current working directory
string DirectoryGetCurrentWorking() {
  return directory_get_current_working();
}

// set current working directory based on a given dname
bool DirectorySetCurrentWorking(string dname) {
  return directory_set_current_working(dname.c_str());
}

// get the environment variable of the given name
string EnvironmentGetVariable(string name) {
  return environment_get_variable(name.c_str());
}

// get whether the environment variable of the given name exists
bool EnvironmentGetVariableExists(string name) {
  return environment_get_variable_exists(name.c_str());
}

// set the environment variable with the given name and value
bool EnvironmentSetVariable(string name, string value) {
  return environment_set_variable(name.c_str(), value.c_str());
}

// unset the environment variable with the given name
bool EnvironmentUnsetVariable(string name) {
  return environment_unset_variable(name.c_str());
}

// get temporary directory path
string DirectoryGetTemporaryPath() {
  return directory_get_temporary_path();
}

void execute_program(string fname, string args, bool wait) {
  PROCID pid = 0;
  string cmd = string_replace_all(fname, "\"", "\\\"");
  cmd = string_replace_all(cmd, "\\", "\\\\");
  cmd = "\"" + cmd + "\" " + args;
  if (wait) {
    pid = ProcessExecute(cmd.c_str());
  } else {
    pid = ProcessExecuteAsync(cmd.c_str());
  }
  if (pid > 0) {
    FreeExecutedProcessStandardInput(pid);
    FreeExecutedProcessStandardOutput(pid);
  }
}
  
void execute_shell(string fname, string args) {
  execute_program(fname, args, false);
}
  
string execute_shell_for_output(const string &command) {
  PROCID pid = ProcessExecute(command.c_str());
  string output;
  if (pid > 0) {
    while (!CompletionStatusFromExecutedProcess(pid)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    output = ExecutedProcessReadFromStandardOutput(pid);
    FreeExecutedProcessStandardInput(pid);
    FreeExecutedProcessStandardOutput(pid);
  }
  return output;
}

void url_open(string url) {
  string escapedfolder = url;
  escapedfolder = string_replace_all(escapedfolder, "\"", "\\\"");
  #if defined(_WIN32)
  execute_shell("cmd", "explorer \"" + escapedfolder + "\"");
  #elif defined(__APPLE__) && defined(__MACH__)
  execute_shell("open", "\"" + escapedfolder + "\"");
  #else
  execute_shell("xdg-open", "\"" + escapedfolder + "\"");
  #endif
}

void action_webpage(const string& url) {
  url_open(url);
}

} // namespace enigma_user
