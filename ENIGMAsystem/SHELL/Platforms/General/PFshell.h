#include <string>

namespace enigma_insecure {

void execute_shell(std::string fname, std::string args);
void execute_program(std::string fname, std::string args, bool wait);
std::string execute_shell_for_output(const std::string &command);

} // namespace enigma_insecure

