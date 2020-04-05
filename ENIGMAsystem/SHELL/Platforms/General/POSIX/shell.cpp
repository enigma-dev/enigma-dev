#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFshell.h"
#include "Widget_Systems/widgets_mandatory.h"

namespace enigma_insecure {
  void execute_shell(std::string operation, std::string fname, std::string args, bool wait) {
    if (system(NULL)) {
      system(("\"" + fname + "\" " + args + (wait ? " &" : "")).c_str());
    } else {
      DEBUG_MESSAGE("execute_program cannot be used as there is no command processor!", MESSAGE_TYPE::M_ERROR);
      return;
    }
  }
  
  void execute_shell_for_output(const std::string &command, std::string& res) {
    char buffer[BUFSIZ];
    FILE *pf = popen(command.c_str(), "r");
    while (!feof(pf)) {
      res.append(buffer, fread(&buffer, sizeof(char), BUFSIZ, pf));
    }
    pclose(pf);
  }
}

namespace enigma_user {

void execute_program(std::string fname, std::string args, bool wait) {
  execute_shell(fname, args, wait);
}

void url_open(std::string url) {
  execute_program("xdg-open", url, false);
}

void action_webpage(const std::string& url) {
  url_open(url);
}

} // namespace enigma_user
