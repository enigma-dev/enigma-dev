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

#include <cstdio>
#include <mutex>
#include <map>

#include "Universal_System/Extensions/VideoPlayer/insecure_bullshit.h"

#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

using std::string;
using std::to_string;

namespace {

process_t system2(const char *command, int *infp, int *outfp) {
  int p_stdin[2];
  int p_stdout[2];
  process_t pid;
  if (pipe(p_stdin) == -1)
    return -1;
  if (pipe(p_stdout) == -1) {
    close(p_stdin[0]);
    close(p_stdin[1]);
    return -1;
  }
  pid = fork();
  if (pid < 0) {
    close(p_stdin[0]);
    close(p_stdin[1]);
    close(p_stdout[0]);
    close(p_stdout[1]);
    return pid;
  } else if (pid == 0) {
    close(p_stdin[1]);
    dup2(p_stdin[0], 0);
    close(p_stdout[0]);
    dup2(p_stdout[1], 1);
    dup2(open("/dev/null", O_RDONLY), 2);
    for (int i = 3; i < 4096; i++)
      close(i);
    setsid();
    execl("/bin/sh", "/bin/sh", "-c", command, NULL);
    exit(0);
  }
  close(p_stdin[0]);
  close(p_stdout[1]);
  if (infp == NULL) {
    close(p_stdin[1]);
  } else {
    *infp = p_stdin[1];
  }
  if (outfp == NULL) {
    close(p_stdout[0]);
  } else {
    *outfp = p_stdout[0];
  }
  return pid;
}

} // anonymous namespace

namespace enigma_insecure {

static std::map<process_t, process_t> currpid;
static std::map<process_t, process_t> prevpid;

static std::map<process_t, string> currout;
static std::map<process_t, string> prevout;

std::mutex currout_mutex;

process_t process_execute(process_t ind, string command) {
  process_t pid = 0;
  string output;
  int outfp = 0; 
  ssize_t nRead = 0;
  pid = system2(command.c_str(), NULL, &outfp);
  currpid.insert(std::make_pair(ind, pid));
  char buffer[BUFSIZ];
  while ((nRead = read(outfp, buffer, BUFSIZ)) > 0) {
    buffer[nRead] = '\0';
    output.append(buffer, nRead);
    std::lock_guard<std::mutex> guard(currout_mutex);
    currout[ind] = output;
  }
  // file paths need to return with no trailing newlines
  while (output.back() == '\r' || output.back() == '\n')
    output.pop_back();
  prevpid.insert(std::make_pair(ind, pid));
  prevout.insert(std::make_pair(ind, output));
  return pid;
}

process_t process_current(process_t ind) {
  process_t pid = currpid.find(ind)->second;
  // check if OS didn't skip the "/bin/sh" process id
  // cmndline will say sh regardless of default shell
  // /proc/<pid>/exe should say default shell instead
  if (cmd_from_pid(pid).substr(0, 8) == "/bin/sh ") {
    // get child process id of /bin/sh
    string cpid = pids_from_ppid(pid);
    return (cpid.empty() || !pid) ? pid : stoul(cpid, nullptr, 10);
  }
  return pid;
}

process_t process_previous(process_t ind) {
  process_t pid = prevpid.find(ind)->second;
  // check if OS didn't skip the "/bin/sh" process id
  // cmndline will say sh regardless of default shell
  // /proc/<pid>/exe should say default shell instead
  if (cmd_from_pid(pid).substr(0, 8) == "/bin/sh ") {
    // get child process id of /bin/sh
    string cpid = pids_from_ppid(pid);
    return (cpid.empty() || !pid) ? pid : stoul(cpid, nullptr, 10);
  }
  return pid;
}

string process_output(process_t ind) {
  std::lock_guard<std::mutex> guard(currout_mutex);
  return currout[ind];
}

string process_evaluate(process_t ind) {
  return prevout.find(ind)->second;
}

void process_clear_pid(process_t ind) {
  currpid.erase(ind);
  prevpid.erase(ind);
}

void process_clear_out(process_t ind) {
  std::lock_guard<std::mutex> guard(currout_mutex);
  currout.erase(ind);
  prevout.erase(ind);
}

bool pid_exists(process_t pid) {
  return (kill(pid, 0) == 0);
}

bool pid_kill(process_t pid) {
  return (kill(pid, SIGKILL) == 0);
}

string echo(process_t ind, string expression) {
  process_execute(ind, "echo " + expression);
  return process_evaluate(ind);
}

} // namepace enigma_insecure
