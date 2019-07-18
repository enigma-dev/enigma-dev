#include "Platforms/General/PFmain.h"
#include "Widget_Systems/widgets_mandatory.h"

namespace enigma_user {

void execute_shell(std::string operation, std::string fname, std::string args) {
  if (system(NULL)) {
    system((fname + args + " &").c_str());
  } else {
    DEBUG_MESSAGE("execute_shell cannot be used as there is no command processor!", MESSAGE_TYPE::M_ERROR);
    return;
  }
}

void execute_shell(std::string fname, std::string args) { execute_shell("", fname, args); }

void execute_program(std::string operation, std::string fname, std::string args, bool wait) {
  if (system(NULL)) {
    system((fname + args + (wait ? " &" : "")).c_str());
  } else {
    DEBUG_MESSAGE("execute_program cannot be used as there is no command processor!", MESSAGE_TYPE::M_ERROR);
    return;
  }
}

std::string execute_shell_for_output(const std::string &command) {
  std::string res;
  char buffer[BUFSIZ];
  FILE *pf = popen(command.c_str(), "r");
  while (!feof(pf)) {
    res.append(buffer, fread(&buffer, sizeof(char), BUFSIZ, pf));
  }
  pclose(pf);
  return res;
}

void execute_program(std::string fname, std::string args, bool wait) { execute_program("", fname, args, wait); }

void url_open(std::string url, std::string target, std::string options) {
  execute_program("xdg-open", url, false);
}

void url_open_ext(std::string url, std::string target) { url_open(url, target); }

void url_open_full(std::string url, std::string target, std::string options) { url_open(url, target, options); }

void action_webpage(const std::string& url) { url_open(url); }

} // namespace enigma_user
