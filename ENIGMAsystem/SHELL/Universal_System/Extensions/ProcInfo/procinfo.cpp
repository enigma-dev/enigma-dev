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

#include "procinfo.h"
#include <string>

using std::string;

namespace enigma_user {

process_t process_execute(string command) {
  return procinfo::process_execute(command);
}

void process_execute_async(string command) {
  procinfo::process_execute_async(command);
}

process_t process_previous() {
  return procinfo::process_previous();
}

string process_evaluate() {
  return procinfo::process_evaluate();
}

void process_clear_pid() {
  procinfo::process_clear_pid();
}

void process_clear_out() {
  procinfo::process_clear_out();
}

process_t pid_from_self() {
  return procinfo::pid_from_self();
}

process_t ppid_from_self() {
  return procinfo::ppid_from_self();
}

string path_from_pid(process_t pid) {
  return procinfo::path_from_pid(pid);
}

string dir_from_pid(process_t pid) {
  return procinfo::dir_from_pid(pid);
}

string name_from_pid(process_t pid) {
  return procinfo::name_from_pid(pid);
}

bool pid_exists(process_t pid) {
  return procinfo::pid_exists(pid);
}

bool wid_exists(wid_t wid) {
  return procinfo::wid_exists(wid);
}

window_t window_from_wid(wid_t wid) {
  return procinfo::window_from_wid(wid);
}

wid_t wid_from_window(window_t window) {
  return procinfo::wid_from_window(window);
}

process_t pid_from_wid(wid_t wid) {
  return procinfo::pid_from_wid(wid);
}

process_t ppid_from_pid(process_t pid) {
  return procinfo::ppid_from_pid(pid);
}

wid_t wid_from_top() {
  return procinfo::wid_from_top();
}

process_t pid_from_top() {
  return procinfo::pid_from_top();
}

void wid_to_top(wid_t wid) {
  procinfo::wid_to_top(wid);
}

void wid_set_pwid(wid_t wid, wid_t pwid) {
  procinfo::wid_set_pwid(wid, pwid);
}

} // namespace enigma_user

