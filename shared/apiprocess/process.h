/*

 MIT License
 
 Copyright © 2021 Samuel Venable
 Copyright © 2021 Lars Nilsson
 
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

#pragma once
#if defined(PROCESS_GUIWINDOW_IMPL)
#if defined(_WIN32)
#include <windows.h>
#else
#if (defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL)
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include <Cocoa/Cocoa.h>
#elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__OpenBSD__)) || defined(PROCESS_XQUARTZ_IMPL)
#include <X11/Xlib.h>
#endif
#endif
#endif

namespace ngs::proc {

  #if !defined(_WIN32)
  typedef int PROCID;
  #else
  typedef unsigned long PROCID;
  #endif
  typedef PROCID LOCALPROCID;
  #if defined(PROCESS_GUIWINDOW_IMPL)
  #if defined(_WIN32)
  typedef HWND WINDOW;
  #elif (defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL)
  typedef NSWindow *WINDOW;
  #elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__OpenBSD__)) || defined(PROCESS_XQUARTZ_IMPL)
  typedef Window WINDOW;
  #endif
  typedef char *WINDOWID;
  #endif
  typedef int  PROCLIST;
  typedef int  PROCINFO;
  typedef long KINFOFLAGS;

  // Specific Process Info
  #define KINFO_EXEP 0x00800000
  #define KINFO_CWDP 0x00200000
  #define KINFO_PPID 0x40000000
  #define KINFO_CPID 0x02000000
  #define KINFO_ARGV 0x04000000
  #define KINFO_ENVV 0x08000000
  #if defined(PROCESS_GUIWINDOW_IMPL)
  #define KINFO_OWID 0x00400000
  #endif

  void proc_id_enumerate(PROCID **proc_id, int *size);
  void free_proc_id(PROCID *proc_id);
  void proc_id_from_self(PROCID *proc_id);
  PROCID proc_id_from_self();
  void parent_proc_id_from_self(PROCID *parent_proc_id);
  PROCID parent_proc_id_from_self();
  bool proc_id_exists(PROCID proc_id);
  bool proc_id_kill(PROCID proc_id);
  const char *executable_from_self();
  void parent_proc_id_from_proc_id(PROCID proc_id, PROCID *parent_proc_id);
  void proc_id_from_parent_proc_id(PROCID parent_proc_id, PROCID **proc_id, int *size);
  const char *exe_from_proc_id(PROCID proc_id);
  void exe_from_proc_id(PROCID proc_id, char **buffer);
  const char *directory_get_current_working();
  bool directory_set_current_working(const char *dname);
  const char *cwd_from_proc_id(PROCID proc_id);
  void cwd_from_proc_id(PROCID proc_id, char **buffer);
  void free_cmdline(char **buffer);
  void cmdline_from_proc_id(PROCID proc_id, char ***buffer, int *size);
  const char *environment_get_variable(const char *name);
  bool environment_get_variable_exists(const char *name);
  bool environment_set_variable(const char *name, const char *value);
  bool environment_unset_variable(const char *name);
  void free_environ(char **buffer);
  void environ_from_proc_id(PROCID proc_id, char ***buffer, int *size);
  void environ_from_proc_id_ex(PROCID proc_id, const char *name, char **value);
  const char *environ_from_proc_id_ex(PROCID proc_id, const char *name);
  bool environ_from_proc_id_ex_exists(PROCID proc_id, const char *name);
  const char *directory_get_temporary_path();
  PROCINFO proc_info_from_proc_id(PROCID proc_id);
  PROCINFO proc_info_from_proc_id_ex(PROCID proc_id, KINFOFLAGS kinfo_flags);
  void free_proc_info(PROCINFO proc_info);
  PROCLIST proc_list_create();
  PROCID process_id(PROCLIST proc_list, int i);
  int process_id_length(PROCLIST proc_list);
  void free_proc_list(PROCINFO proc_info);
  #if defined(PROCESS_GUIWINDOW_IMPL)
  WINDOWID window_id_from_native_window(WINDOW window);
  WINDOW native_window_from_window_id(WINDOWID winid);
  void window_id_enumerate(WINDOWID **win_id, int *size);
  void proc_id_from_window_id(WINDOWID win_id, PROCID *proc_id);
  void window_id_from_proc_id(PROCID proc_id, WINDOWID **win_id, int *size);
  void free_window_id(WINDOWID *win_id);
  bool window_id_exists(WINDOWID win_id);
  bool window_id_kill(WINDOWID win_id);
  #endif

  char *executable_image_file_path(PROCINFO proc_info);
  char *current_working_directory(PROCINFO proc_info);
  PROCID parent_process_id(PROCINFO proc_info);
  PROCID *child_process_id(PROCINFO proc_info);
  PROCID child_process_id(PROCINFO proc_info, int i);
  int child_process_id_length(PROCINFO proc_info);
  char **commandline(PROCINFO proc_info);
  char *commandline(PROCINFO proc_info, int i);
  int commandline_length(PROCINFO proc_info);
  char **environment(PROCINFO proc_info);
  char *environment(PROCINFO proc_info, int i);
  int environment_length(PROCINFO proc_info);
  #if defined(PROCESS_GUIWINDOW_IMPL)
  WINDOWID *owned_window_id(PROCINFO proc_info);
  WINDOWID owned_window_id(PROCINFO proc_info, int i);
  int owned_window_id_length(PROCINFO proc_info);
  #endif

  LOCALPROCID process_execute(const char *command);
  LOCALPROCID process_execute_async(const char *command);
  void executed_process_write_to_standard_input(LOCALPROCID proc_index, const char *input);
  const char *executed_process_read_from_standard_output(LOCALPROCID proc_index);
  void free_executed_process_standard_input(LOCALPROCID proc_index);
  void free_executed_process_standard_output(LOCALPROCID proc_index);
  bool completion_status_from_executed_process(LOCALPROCID proc_index);
  const char *current_process_read_from_standard_input();

} // namespace ngs::proc
