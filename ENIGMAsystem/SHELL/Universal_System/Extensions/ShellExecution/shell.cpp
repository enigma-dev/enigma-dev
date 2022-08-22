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

// execute process from the shell, return process id
CPROCID ProcessExecute(string command) {
  return process_execute(command.c_str());
}

// execute process from the shell async, return process id
CPROCID ProcessExecuteAsync(string command) {
  return process_execute_async(command.c_str());
}

// embed window identifier into parent window identifier
void WindowIdSetParentWindowId(wid_t windowId, wid_t parentWindowId) {
  #if defined(_WIN32)
  HWND child  = (HWND)(void *)(uintptr_t)strtoull(windowId.c_str(), nullptr, 10);
  HWND parent = (HWND)(void *)(uintptr_t)strtoull(parentWindowId.c_str(), nullptr, 10);
  RECT wrect; GetWindowRect(parent, &wrect); RECT crect; GetClientRect(parent, &crect); 
  POINT lefttop     = { crect.left,  crect.top    }; ClientToScreen(parent, &lefttop);
  POINT bottomright = { crect.right, crect.bottom }; ClientToScreen(parent, &bottomright);
  MoveWindow(child, -(lefttop.x - wrect.left), -(lefttop.y - wrect.top), crect.right + bottomright.x, crect.bottom + bottomright.y, true);
  SetWindowLongPtr(child, GWL_STYLE, GetWindowLongPtr(child, GWL_STYLE) | WS_POPUP);
  SetWindowLongPtr(child, GWL_EXSTYLE, GetWindowLongPtr(child, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
  SetWindowLongPtr(parent, GWL_STYLE, GetWindowLongPtr(parent, GWL_STYLE) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS); 
  SetParent(child, parent); ShowWindow(child, SW_MAXIMIZE);
  #elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
  Window child  = (Window)(uintptr_t)strtoull(windowId.c_str(), nullptr, 10);
  Window parent = (Window)(uintptr_t)strtoull(parentWindowId.c_str(), nullptr, 10);
  Display *display = XOpenDisplay(nullptr); XMapWindow(display, child);
  Window root = 0, rparent = 0, *children = nullptr; unsigned int nchildren = 0;
  XQueryTree(display, child, &root, &rparent, &children, &nchildren);
  XReparentWindow(display, child, parent, 0, 0); if (nchildren > 0) { XFree(children); }
  XFlush(display); std::this_thread::sleep_for(std::chrono::milliseconds(5));
  while (parent != rparent) {
    XSynchronize(display, true);
    XQueryTree(display, child, &root, &rparent, &children, &nchildren);
    XReparentWindow(display, child, parent, 0, 0); if (nchildren > 0) { XFree(children); }
    XFlush(display); std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  XCloseDisplay(display);
  #else
  DEBUG_MESSAGE("Unsupported platform for function!", MESSAGE_TYPE::M_INFO);
  #endif
}

// update embed area of window identifier to fill parent window identifier client area
void WindowIdFillParentWindowId(wid_t windowId, wid_t parentWindowId) {
  if (strtoull(parentWindowId.c_str(), nullptr, 10) == 0) return;
  #if defined(_WIN32)
  HWND child  = (HWND)(void *)(uintptr_t)strtoull(windowId.c_str(), nullptr, 10);
  HWND parent = (HWND)(void *)(uintptr_t)strtoull(parentWindowId.c_str(), nullptr, 10);
  RECT crect; GetClientRect(parent, &crect); 
  MoveWindow(child, 0, 0, crect.right, crect.bottom, true);
  SetWindowLongPtr(child, GWL_STYLE, GetWindowLongPtr(child, GWL_STYLE) | WS_POPUP);
  SetWindowLongPtr(child, GWL_EXSTYLE, GetWindowLongPtr(child, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
  SetWindowLongPtr(parent, GWL_STYLE, GetWindowLongPtr(parent, GWL_STYLE) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
  #elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
  Window child  = (Window)(uintptr_t)strtoull(windowId.c_str(), nullptr, 10);
  Window parent = (Window)(uintptr_t)strtoull(parentWindowId.c_str(), nullptr, 10);
  Display *display = XOpenDisplay(nullptr); XMapWindow(display, child);
  Window r = 0; int x = 0, y = 0; unsigned w = 0, h = 0, b = 0, d = 0;
  XGetGeometry(display, parent, &r, &x, &y, &w, &h, &b, &d);
  XMoveResizeWindow(display, child, 0, 0, w, h);
  XCloseDisplay(display);
  #else
  DEBUG_MESSAGE("Unsupported platform for function!", MESSAGE_TYPE::M_INFO);
  #endif
}

//  get whether executed process has quit based on process id
bool CompletionStatusFromExecutedProcess(CPROCID procIndex) {
  return completion_status_from_executed_process(procIndex);
}

// write to executed process standard input file descriptor based on process id
void ExecutedProcessWriteToStandardInput(CPROCID procIndex, string input) {
  executed_process_write_to_standard_input(procIndex, input.c_str());
}

// read from current process standard input
std::string CurrentProcessReadFromStandardInput() {
  return current_process_read_from_standard_input();
}

// read from executed process standard output file descriptor based on process id
string ExecutedProcessReadFromStandardOutput(CPROCID procIndex) {
  return executed_process_read_from_standard_output(procIndex);
}

// free executed process standard input string based on process id
void FreeExecutedProcessStandardInput(CPROCID procIndex) {
  free_executed_process_standard_input(procIndex);
}

// free executed process standard ouptut string based on process id
void FreeExecutedProcessStandardOutput(CPROCID procIndex) {
  free_executed_process_standard_output(procIndex);
}

// get process id from self
XPROCID ProcIdFromSelf() {
  return proc_id_from_self();
}

// get parent process id from self
XPROCID ParentProcIdFromSelf() {
  return parent_proc_id_from_self();
}

// get whether process exists based on process id
bool ProcIdExists(XPROCID procId) {
  return proc_id_exists(procId);
}

// suspend process based on process id, return whether succeeded
bool ProcIdSuspend(XPROCID procId) {
  return proc_id_suspend(procId);
}

// resume process based on process id, return whether succeeded
bool ProcIdResume(XPROCID procId) {
  return proc_id_resume(procId);
}

// kill process based on process id, return whether succeeded
bool ProcIdKill(XPROCID procId) {
  return proc_id_kill(procId);
}

// get executable image file path from self
string ExecutableFromSelf() {
  return executable_from_self();
}

// get executable image file path from process id
string ExeFromProcId(XPROCID procId) {
  return exe_from_proc_id(procId);
}

// get current working directory from process id
string CwdFromProcId(XPROCID procId) {
  return cwd_from_proc_id(procId);
}

// get process info from process id
PROCINFO ProcInfoFromProcId(XPROCID procId) {
  return proc_info_from_proc_id(procId);
}

// get specific process info from process id
PROCINFO ProcInfoFromProcIdEx(XPROCID procId, KINFOFLAGS kInfoFlags) {
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
XPROCID ProcessId(PROCLIST procList, int i) {
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
XPROCID ParentProcessId(PROCINFO procInfo) {
  return parent_process_id(procInfo);
}

// get child process id from process info data at index
XPROCID ChildProcessId(PROCINFO procInfo, int i) {
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

} // namespace enigma_user
