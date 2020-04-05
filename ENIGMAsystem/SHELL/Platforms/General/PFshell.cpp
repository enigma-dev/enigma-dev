#include "PFshell.h"

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

void execute_shell(std::string fname, std::string args) {
  request_permission();
  if (!secure_mode) enigma_insecure::execute_shell(fname, args);
}

void execute_program(std::string fname, std::string args, bool wait) {
  request_permission();
  if (!secure_mode) enigma_insecure::execute_shell(fname, args, wait);
}

std::string execute_shell_for_output(const std::string &command) {
  request_permission();
  if (!secure_mode) return enigma_insecure::execute_shell_for_output(command);
  else return "";
}

}
