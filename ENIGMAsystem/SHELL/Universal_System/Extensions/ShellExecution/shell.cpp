#include <string>
#include <thread>
#include <chrono>

#include <cstdint>
#include <cstdlib>

#include "shell.h"
#include "strings_util.h"

#include "Widget_Systems/widgets_mandatory.h"

#if defined(_WIN32) 
#include <windows.h>
#elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
#include <X11/Xlib.h>
#endif

using std::string;

namespace enigma_user {

// execute process from the shell, return ngs::proc::process id
LOCALPROCID ProcessExecute(string command) {
  return ngs::proc::process_execute(command.c_str());
}

// execute process from the shell async, return ngs::proc::process id
LOCALPROCID ProcessExecuteAsync(string command) {
  return ngs::proc::process_execute_async(command.c_str());
}

// embed window identifier into parent window identifier
void WindowIdSetParentWindowId(wid_t windowId, wid_t parentWindowId) {
  #if defined(_WIN32)
  HWND child  = (HWND)(void *)(uintptr_t)strtoull(windowId.c_str(), nullptr, 10);
  HWND parent = (HWND)(void *)(uintptr_t)strtoull(parentWindowId.c_str(), nullptr, 10);
  SetWindowLongPtr(child, GWL_STYLE, (GetWindowLongPtr(child, GWL_STYLE) | WS_CHILD) & ~(WS_CAPTION | WS_BORDER | WS_SIZEBOX));
  SetWindowLongPtr(child, GWL_EXSTYLE, GetWindowLongPtr(child, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
  SetWindowLongPtr(parent, GWL_STYLE, GetWindowLongPtr(parent, GWL_STYLE) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
  SetParent(child, parent); SetWindowPos(child, HWND_TOP, 0, 0, 0, 0, SWP_NOOWNERZORDER | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
  RECT rect; GetClientRect(parent, &rect); MoveWindow(child, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
  ShowWindow(child, SW_MAXIMIZE); ShowWindow(parent, SW_RESTORE);
  GetWindowRect(parent, &rect); MoveWindow(parent, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top - 1, true);
  GetWindowRect(parent, &rect); MoveWindow(parent, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top + 1, true);
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
  RECT rect; GetClientRect(parent, &rect); MoveWindow(child, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
  SetWindowLongPtr(child, GWL_STYLE, (GetWindowLongPtr(child, GWL_STYLE) | WS_CHILD) & ~(WS_CAPTION | WS_BORDER | WS_SIZEBOX));
  SetWindowLongPtr(child, GWL_EXSTYLE, GetWindowLongPtr(child, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
  #elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
  Window child  = (Window)(uintptr_t)strtoull(windowId.c_str(), nullptr, 10);
  Window parent = (Window)(uintptr_t)strtoull(parentWindowId.c_str(), nullptr, 10);
  Display *display = XOpenDisplay(nullptr); XMapWindow(display, child);
  Window r = 0; int x = 0, y = 0;
  unsigned w = 0, h = 0, b = 0, d = 0;
  XGetGeometry(display, parent, &r, &x, &y, &w, &h, &b, &d);
  XMoveResizeWindow(display, child, 0, 0, w, h);
  XCloseDisplay(display);
  #else
  DEBUG_MESSAGE("Unsupported platform for function!", MESSAGE_TYPE::M_INFO);
  #endif
}

//  get whether executed process has quit based on process id
bool CompletionStatusFromExecutedProcess(LOCALPROCID procIndex) {
  return ngs::proc::completion_status_from_executed_process(procIndex);
}

// write to executed process standard input file descriptor based on process id
void ExecutedProcessWriteToStandardInput(LOCALPROCID procIndex, string input) {
  ngs::proc::executed_process_write_to_standard_input(procIndex, input.c_str());
}

// read from current process standard input
std::string CurrentProcessReadFromStandardInput() {
  return ngs::proc::current_process_read_from_standard_input();
}

// read from executed process standard output file descriptor based on process id
string ExecutedProcessReadFromStandardOutput(LOCALPROCID procIndex) {
  return ngs::proc::executed_process_read_from_standard_output(procIndex);
}

// free executed process standard input string based on process id
void FreeExecutedProcessStandardInput(LOCALPROCID procIndex) {
  ngs::proc::free_executed_process_standard_input(procIndex);
}

// free executed process standard ouptut string based on process id
void FreeExecutedProcessStandardOutput(LOCALPROCID procIndex) {
  ngs::proc::free_executed_process_standard_output(procIndex);
}

// get process id from self
PROCID ProcIdFromSelf() {
  return ngs::proc::proc_id_from_self();
}

// get parent process id from self
PROCID ParentProcIdFromSelf() {
  return ngs::proc::parent_proc_id_from_self();
}

// get whether process exists based on process id
bool ProcIdExists(PROCID procId) {
  return ngs::proc::proc_id_exists(procId);
}

// kill process based on process id, return ngs::proc::whether succeeded
bool ProcIdKill(PROCID procId) {
  return ngs::proc::proc_id_kill(procId);
}

// get executable image file path from self
string ExecutableFromSelf() {
  return ngs::proc::executable_from_self();
}

// get executable image file path from process id
string ExeFromProcId(PROCID procId) {
  return ngs::proc::exe_from_proc_id(procId);
}

// get current working directory from process id
string CwdFromProcId(PROCID procId) {
  return ngs::proc::cwd_from_proc_id(procId);
}

// get process info from process id
PROCINFO ProcInfoFromProcId(PROCID procId) {
  return ngs::proc::proc_info_from_proc_id(procId);
}

// get specific process info from process id
PROCINFO ProcInfoFromProcIdEx(PROCID procId, KINFOFLAGS kInfoFlags) {
  return ngs::proc::proc_info_from_proc_id_ex(procId, kInfoFlags);
}

// free process info data from memory
void FreeProcInfo(PROCINFO procInfo) {
  ngs::proc::free_proc_info(procInfo);
}

// create a list of all process id's
PROCLIST ProcListCreate() {
  return ngs::proc::proc_list_create();
}

// get process id from process list at index
PROCID ProcessId(PROCLIST procList, int i) {
  return ngs::proc::process_id(procList, i);
}

// get amount of process id's in process list
int ProcessIdLength(PROCLIST procList) {
  return ngs::proc::process_id_length(procList);
}

// free list of process id's from memory
void FreeProcList(PROCLIST procList) {
  ngs::proc::free_proc_list(procList);
}

// get executable image file path from process info data
string ExecutableImageFilePath(PROCINFO procInfo) {
  return ngs::proc::executable_image_file_path(procInfo);
}

// get current working directory ffrom process info data
string CurrentWorkingDirectory(PROCINFO procInfo) {
  return ngs::proc::current_working_directory(procInfo);
}

// get parent processs id from process info data
PROCID ParentProcessId(PROCINFO procInfo) {
  return ngs::proc::parent_process_id(procInfo);
}

// get child process id from process info data at index
PROCID ChildProcessId(PROCINFO procInfo, int i) {
  return ngs::proc::child_process_id(procInfo, i);
}

// get amount of child processes from process info data
int ChildProcessIdLength(PROCINFO procInfo) {
  return ngs::proc::child_process_id_length(procInfo);
}

// get command line argument from process info data at index
string CommandLine(PROCINFO procInfo, int i) {
  return ngs::proc::commandline(procInfo, i);
}

// get amount of command line arguments from process info data
int CommandLineLength(PROCINFO procInfo) {
  return ngs::proc::commandline_length(procInfo);
}

// get environment variable (NAME=VALUE) from process info at index
string Environment(PROCINFO procInfo, int i) {
  return ngs::proc::environment(procInfo, i);
}

// get amount of anvironment variables from process info at index
int EnvironmentLength(PROCINFO procInfo) {
  return ngs::proc::environment_length(procInfo);
}

// get current working directory
string DirectoryGetCurrentWorking() {
  return ngs::proc::directory_get_current_working();
}

// set current working directory based on a given dname
bool DirectorySetCurrentWorking(string dname) {
  return ngs::proc::directory_set_current_working(dname.c_str());
}

// get the environment variable of the given name
string EnvironmentGetVariable(string name) {
  return ngs::proc::environment_get_variable(name.c_str());
}

// get whether the environment variable of the given name exists
bool EnvironmentGetVariableExists(string name) {
  return ngs::proc::environment_get_variable_exists(name.c_str());
}

// set the environment variable with the given name and value
bool EnvironmentSetVariable(string name, string value) {
  return ngs::proc::environment_set_variable(name.c_str(), value.c_str());
}

// unset the environment variable with the given name
bool EnvironmentUnsetVariable(string name) {
  return ngs::proc::environment_unset_variable(name.c_str());
}

// get temporary directory path
string DirectoryGetTemporaryPath() {
  return ngs::proc::directory_get_temporary_path();
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
