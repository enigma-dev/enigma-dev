/*

 MIT License
 
 Copyright © 2020 Samuel Venable
 
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
#include <vector>

// individual platforms need their platform-specific
// window types casted to an unsigned long long type
#define window_t unsigned long long

// we don't really care about the process swapper so
// if (pid == 0) an error occured with some function
#define process_t unsigned long

// an unsigned long long window identifier placed in
// quotes for quick concatenation in shell scripting
#define wid_t std::string

bool string_has_whitespace(std::string str);
size_t string_count_equalssigns(std::string str);
std::string string_replace_all(std::string str, std::string substr, std::string nstr);
std::vector<std::string> string_split_by_first_equalssign(std::string str);
std::vector<std::string> string_split(std::string str, char delimiter);

namespace procinfo {

// execute process, returns process id
process_t process_execute(std::string command);

// execute process outside main thread
void process_execute_async(std::string command);

// return previous executed process id
process_t process_previous();

// evaluate last process output string
std::string process_evaluate();

// clears previous process id executed
void process_clear_pid();

// clears previous process output text
void process_clear_out();

// get process id from current process
process_t pid_from_self();

// parent process id from this process
process_t ppid_from_self();

// get executable path from process id
std::string path_from_pid(process_t pid);

// get exe parent path from process id
std::string dir_from_pid(process_t pid);

// get executable name from process id
std::string name_from_pid(process_t pid);

// return command line from process id
std::string cmd_from_pid(process_t pid);

// return environ vars from process id
std::string env_from_pid(process_t pid);

// check for existence from process id
bool pid_exists(process_t pid);

// check existence for given window id
bool wid_exists(wid_t wid);

// kill an application from process id
bool pid_kill(process_t pid);

// return window handle from window id
window_t window_from_wid(wid_t wid);

// return window id from window handle
wid_t wid_from_window(window_t window);

// get owner process id from window id
process_t pid_from_wid(wid_t wid);

// return strings for every process id
std::string pids_enum(bool trim_dir, bool trim_empty);

// get parent process id of process id
process_t ppid_from_pid(process_t pid);

// get list of process ids from parent
std::string pids_from_ppid(process_t ppid);

// get list of window ids from process
std::string wids_from_pid(process_t pid);

// get window id from frontmost window
wid_t wid_from_top();

// get process id from topmost process
process_t pid_from_top();

// bring window id to frontmost window
void wid_to_top(wid_t wid);

// add a parent window id to window id
void wid_set_pwid(wid_t wid, wid_t pwid);

// return output of a given expression
std::string echo(std::string expression);

} // namespace procinfo

namespace enigma_user {

using procinfo::process_execute;
using procinfo::process_execute_async;
using procinfo::process_previous;
using procinfo::process_evaluate;
using procinfo::process_clear_pid;
using procinfo::process_clear_out;
using procinfo::pid_from_self;
using procinfo::ppid_from_self;
using procinfo::path_from_pid;
using procinfo::dir_from_pid;
using procinfo::name_from_pid;
using procinfo::cmd_from_pid;
using procinfo::env_from_pid;
using procinfo::pid_exists;
using procinfo::wid_exists;
using procinfo::pid_kill;
using procinfo::window_from_wid;
using procinfo::wid_from_window;
using procinfo::pid_from_wid;
using procinfo::pids_enum;
using procinfo::ppid_from_pid;
using procinfo::pids_from_ppid;
using procinfo::wids_from_pid;
using procinfo::wid_from_top;
using procinfo::pid_from_top;
using procinfo::wid_to_top;
using procinfo::wid_set_pwid;
using procinfo::echo;

} // namespace enigma_user

