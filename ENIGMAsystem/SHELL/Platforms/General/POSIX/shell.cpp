#include "Platforms/General/PFshell.h"
#include "Widget_Systems/widgets_mandatory.h"

namespace enigma_insecure {
  void execute_program(string fname, string args, bool wait) {
    if (system(NULL)) {
      system(("\"" + fname + "\" " + args + (wait ? " &" : "")).c_str());
    } else {
      DEBUG_MESSAGE("shell execution cannot be used as there is no command processor!", MESSAGE_TYPE::M_ERROR);
      return;
    }
  }
  
  void execute_shell(string fname, string args) {
    execute_program(fname, args, false);
  }
  
  string execute_shell_for_output(const string &command) {
    char buffer[BUFSIZ]; string res;
    FILE *pf = popen(command.c_str(), "r");
    while (!feof(pf)) {
      res.append(buffer, fread(&buffer, sizeof(char), BUFSIZ, pf));
    }
    pclose(pf);
    return res;
  }
} // namsepace enigma_insecure

namespace enigma_user {

void execute_program(string fname, string args, bool wait) {
  execute_program(fname, args, wait);
}

void execute_shell(string fname, string args) {
  execute_shell(fname, args);
}

string execute_shell_for_output(const string &command) {
  return execute_shell_for_output(command);
}

void url_open(string url) {
  execute_program("xdg-open", url, false);
}

void action_webpage(const string& url) {
  url_open(url);
}

} // namespace enigma_user
