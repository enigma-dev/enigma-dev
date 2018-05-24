#include "Utility.h"

#include "Platforms/General/PFmain.h"

#include <sys/resource.h>  //setpriority
#include <sys/types.h>     //getpid
#include <unistd.h>        //usleep

namespace enigma {

std::string *parameters;
int parameterc;

void Sleep(int ms) {
  if (ms >= 1000) enigma_user::sleep(ms / 1000);
  if (ms > 0) usleep(ms % 1000 * 1000);
}

void windowsystem_write_exename(char* x) {
  unsigned irx = 0;
  if (enigma::parameterc)
    for (irx = 0; enigma::parameters[0][irx] != 0; irx++) x[irx] = enigma::parameters[0][irx];
  x[irx] = 0;
}

}  //namespace enigma

namespace enigma_user {
void sleep(int ms) { enigma::Sleep(ms); }

std::string parameter_string(int num) { return num < enigma::parameterc ? enigma::parameters[num] : ""; }

int parameter_count() { return enigma::parameterc; }

void execute_shell(std::string operation, std::string fname, std::string args) {
  if (system(NULL)) {
    system((fname + args + " &").c_str());
  } else {
    printf("execute_shell cannot be used as there is no command processor!");
    return;
  }
}

void execute_shell(std::string fname, std::string args) { execute_shell("", fname, args); }

void execute_program(std::string operation, std::string fname, std::string args, bool wait) {
  if (system(NULL)) {
    system((fname + args + (wait ? " &" : "")).c_str());
  } else {
    printf("execute_program cannot be used as there is no command processor!");
    return;
  }
}

void execute_program(std::string fname, std::string args, bool wait) { execute_program("", fname, args, wait); }

void url_open(std::string url, std::string target, std::string options) {
  if (!fork()) {
    execlp("xdg-open", "xdg-open", url.c_str(), NULL);
    exit(0);
  }
}

void url_open_ext(std::string url, std::string target) { url_open(url, target); }

void url_open_full(std::string url, std::string target, std::string options) { url_open(url, target, options); }

void action_webpage(const std::string &url) { url_open(url); }

std::string environment_get_variable(std::string name) {
  char *ev = getenv(name.c_str());
  return ev ? ev : "";
}

void set_program_priority(int value) { setpriority(PRIO_PROCESS, getpid(), value); }
}  //namespace enigma_user
