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

// individual platforms need their platform-specific
// window types casted to an unsigned long long type
typedef unsigned long long window_t;

#ifdef _WiN32
#include <windows.h>
typedef DWORD process_t;
#else
#include <sys/types.h>
typedef pid_t process_t;
#endif
#include <string>
typedef std::string wid_t;

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

// check for existence from process id
bool pid_exists(process_t pid);

// check existence for given window id
bool wid_exists(wid_t wid);

// return window handle from window id
window_t window_from_wid(wid_t wid);

// return window id from window handle
wid_t wid_from_window(window_t window);

// get owner process id from window id
process_t pid_from_wid(wid_t wid);

// get parent process id of process id
process_t ppid_from_pid(process_t pid);

// get window id from frontmost window
wid_t wid_from_top();

// get process id from topmost process
process_t pid_from_top();

// bring window id to frontmost window
void wid_to_top(wid_t wid);

// add a parent window id to window id
void wid_set_pwid(wid_t wid, wid_t pwid);

} // namespace procinfo

namespace enigma_user {

process_t process_execute(std::string command);
void process_execute_async(std::string command);

process_t process_previous();
std::string process_evaluate();
void process_clear_pid();
void process_clear_out();

process_t pid_from_self();
process_t ppid_from_self();

std::string path_from_pid(process_t pid);
std::string dir_from_pid(process_t pid);
std::string name_from_pid(process_t pid);

bool pid_exists(process_t pid);
bool wid_exists(wid_t wid);

window_t window_from_wid(wid_t wid);
wid_t wid_from_window(window_t window);

process_t pid_from_wid(wid_t wid);
process_t ppid_from_pid(process_t pid);

wid_t wid_from_top();
process_t pid_from_top();
void wid_to_top(wid_t wid);
void wid_set_pwid(wid_t wid, wid_t pwid);

} // namespace enigma_user