#include <string>

using std::string;

namespace enigma_insecure {

void execute_shell(string fname, string args);
void execute_program(string fname, string args, bool wait);
string execute_shell_for_output(const string &command);

} // namespace enigma_insecure

