/*

 MIT License
 
 Copyright © 2022 Samuel Venable
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
*/

#include <string>
#include <thread>
#include <chrono>

#include <cstdint>
#include <cstdlib>

#include "shell.h"
#include "strings_util.h"

#include "Platforms/General/PFmain.h"
#include "Widget_Systems/widgets_mandatory.h"

#if (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
#include <X11/Xlib.h>
#endif

using std::string;

namespace enigma_user {

CPROCID ProcessExecute(string command) {
  return process_execute(command.c_str());
}

CPROCID ProcessExecuteAsync(string command) {
  return process_execute_async(command.c_str());
}

bool CompletionStatusFromExecutedProcess(CPROCID procIndex) {
  return completion_status_from_executed_process(procIndex);
}

void ExecutedProcessWriteToStandardInput(CPROCID procIndex, string input) {
  executed_process_write_to_standard_input(procIndex, input.c_str());
}

std::string CurrentProcessReadFromStandardInput() {
  return current_process_read_from_standard_input();
}

string ExecutedProcessReadFromStandardOutput(CPROCID procIndex) {
  return executed_process_read_from_standard_output(procIndex);
}

void FreeExecutedProcessStandardInput(CPROCID procIndex) {
  free_executed_process_standard_input(procIndex);
}

void FreeExecutedProcessStandardOutput(CPROCID procIndex) {
  free_executed_process_standard_output(procIndex);
}

XPROCID ProcIdFromSelf() {
  return proc_id_from_self();
}

XPROCID ParentProcIdFromSelf() {
  return parent_proc_id_from_self();
}

bool ProcIdExists(XPROCID procId) {
  return proc_id_exists(procId);
}

bool ProcIdSuspend(XPROCID procId) {
  return proc_id_suspend(procId);
}

bool ProcIdResume(XPROCID procId) {
  return proc_id_resume(procId);
}

bool ProcIdKill(XPROCID procId) {
  return proc_id_kill(procId);
}

string ExecutableFromSelf() {
  return executable_from_self();
}

string ExeFromProcId(XPROCID procId) {
  return exe_from_proc_id(procId);
}

string CwdFromProcId(XPROCID procId) {
  return cwd_from_proc_id(procId);
}

PROCINFO ProcInfoFromProcId(XPROCID procId) {
  return proc_info_from_proc_id(procId);
}

PROCINFO ProcInfoFromProcIdEx(XPROCID procId, KINFOFLAGS kInfoFlags) {
  return proc_info_from_proc_id_ex(procId, kInfoFlags);
}

void FreeProcInfo(PROCINFO procInfo) {
  free_proc_info(procInfo);
}

PROCLIST ProcListCreate() {
  return proc_list_create();
}

XPROCID ProcessId(PROCLIST procList, int i) {
  return process_id(procList, i);
}

int ProcessIdLength(PROCLIST procList) {
  return process_id_length(procList);
}

void FreeProcList(PROCLIST procList) {
  free_proc_list(procList);
}

string ExecutableImageFilePath(PROCINFO procInfo) {
  return executable_image_file_path(procInfo);
}

string CurrentWorkingDirectory(PROCINFO procInfo) {
  return current_working_directory(procInfo);
}

XPROCID ParentProcessId(PROCINFO procInfo) {
  return parent_process_id(procInfo);
}

XPROCID ChildProcessId(PROCINFO procInfo, int i) {
  return child_process_id(procInfo, i);
}

int ChildProcessIdLength(PROCINFO procInfo) {
  return child_process_id_length(procInfo);
}

string CommandLine(PROCINFO procInfo, int i) {
  return commandline(procInfo, i);
}

int CommandLineLength(PROCINFO procInfo) {
  return commandline_length(procInfo);
}

string Environment(PROCINFO procInfo, int i) {
  return environment(procInfo, i);
}

int EnvironmentLength(PROCINFO procInfo) {
  return environment_length(procInfo);
}

string DirectoryGetCurrentWorking() {
  return directory_get_current_working();
}

bool DirectorySetCurrentWorking(string dname) {
  return directory_set_current_working(dname.c_str());
}

string EnvironmentGetVariable(string name) {
  return environment_get_variable(name.c_str());
}

bool EnvironmentGetVariableExists(string name) {
  return environment_get_variable_exists(name.c_str());
}

bool EnvironmentSetVariable(string name, string value) {
  return environment_set_variable(name.c_str(), value.c_str());
}

bool EnvironmentUnsetVariable(string name) {
  return environment_unset_variable(name.c_str());
}

string DirectoryGetTemporaryPath() {
  return directory_get_temporary_path();
}

void execute_program(string fname, string args, bool wait) {
  XPROCID pid = 0;
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
  XPROCID pid = ProcessExecute(command.c_str());
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

WINDOWID WindowIdFromNativeWindow(WINDOW window) {
  return window_id_from_native_window(window);
}

WINDOW NativeWindowFromWindowId(WINDOWID winid) {
  return native_window_from_window_id(win_id);
}

bool WindowIdExists(WINDOWID winId) {
  return window_id_exists(winId);
}

bool WindowIdSuspend(WINDOWID winId) {
  return window_id_suspend(winId);
}

bool WindowIdResume(WINDOWID winId) {
  return window_id_resume(winId);
}

bool WindowidKill(WINDOWID winId) {
  return window_id_kill(winId);
}

char *OwnedWindowId(PROCINFO procInfo, int i) {
  return (char *)owned_window_id(procInfo, i);
}

int OwnedWindowIdLength(PROCINFO procInfo) {
  return owned_window_id_length(procInfo);
}

} // namespace enigma_user
