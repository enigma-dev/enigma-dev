#include "PFshell.h"
#include <string>

using string;

static bool secure_mode = true;
static bool permission_requested = false;

static void request_permission() {
  if (!permission_requested) {
    if (enigma_user::show_question("This application is requesting permission to use the command line.\
Be aware this can be security risk and it is advised to only allow this from application publishers\
that can trusted.\n\nAllow the application to utilize such functionailty?\n\nThis might be\
required for the application to behave as expected.")) {
      secure_mode  = false; 
    }
      permission_requested = true;
  }
}

namespace enigma_user {

void execute_shell(string fname, string args) {
  request_permission();
  if (!secure_mode) enigma_insecure::execute_shell(fname, args);
}

void execute_program(string fname, string args, bool wait) {
  request_permission();
  if (!secure_mode) enigma_insecure::execute_program(fname, args, wait);
}

string execute_shell_for_output(const string &command) {
  request_permission();
  if (!secure_mode) return enigma_insecure::execute_shell_for_output(command);
  else return "";
}

}
