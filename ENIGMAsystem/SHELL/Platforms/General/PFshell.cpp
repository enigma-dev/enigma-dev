#include "PFshell.sh"

static bool secure_mode = true;
static bool permission_requested = false;

using enigma_insecure::execute_shell;
using enigma_insecure::execute_shell_for_output;

static void ask_permission() {
  if (!permission_requested) {
    if (enigma_user::show_question("This application is requesting permission to use the command line.\
Be aware this can be security risk and we advise you to only allow this from application publishers\
that you know you can trust.\n\nWould you like to grant this application permission to utilize\
such functionailty?\n\nThis may or may not be required for the application to behave as expected.")) {
      secure_mode  = false; 
    }
      permission_requested = true;
  }
}
