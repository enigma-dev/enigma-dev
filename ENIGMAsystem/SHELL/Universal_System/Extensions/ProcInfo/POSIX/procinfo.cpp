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

#include "../procinfo.h"
#include <sys/wait.h>
#include <signal.h>
#include <cstdio>

using std::string;

namespace procinfo {

static process_t prevpid;
static string prevout;

process_t process_execute(string command) {
  pid_t pid;
  string output;
  char buffer[BUFSIZ];
  FILE *pf = popen(command.c_str(), "r");
  while (!feof(pf)) {
    output.append(buffer, fread(&buffer, sizeof(char), BUFSIZ, pf));
    pid = wait(NULL);
  }
  prevpid = pid;
  pclose(pf);
  while (output.back() == '\r' || output.back() == '\n')
    output.pop_back();
  prevout = output;
  return pid;
}

process_t process_previous() {
  return prevpid;
}

string process_evaluate() {
  return prevout;
}

void process_clear_pid() {
  prevpid = 0;
}

void process_clear_out() {
  prevout = "";
}

bool pid_exists(process_t pid) {
  return (kill(pid, 0) == 0);
}

} // namepace procinfo

