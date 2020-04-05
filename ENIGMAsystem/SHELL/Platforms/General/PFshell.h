#include <string>

using string;

namespace enigma_insecure {

void execute_shell(string fname, string args, bool wait);
void execute_program(string fname, string args, bool wait);
string execute_shell_for_output(const string &command);

}

namespace enigma_user {

void execute_shell(string fname, string args);
void execute_program(string fname, string args, bool wait);
string execute_shell_for_output(const string &command);
void url_open(string url);
void action_webpage(const string &url);

}
