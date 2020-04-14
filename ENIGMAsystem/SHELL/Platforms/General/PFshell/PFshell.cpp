#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFshell.h"
#include "Widget_Systems/widgets_mandatory.h"

using std::string;

static bool secure_mode = true;
static bool permission_requested = false;

static void request_permission() {
  if (!permission_requested) {
    if (enigma_user::show_question("This application is requesting permission to use the command line. \
This is a security risk and it is advised to only allow it from applications whose publishers \
can be trusted.\n\nAllow the application to use such functionailty?")) {
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

} // namspace enigma_user
