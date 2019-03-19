
#include <functional>

namespace enigma {

using FontLoadFileFunction = std::function<void(std::string, bool, bool, std::string&)>;
using FontLoadMemoryFunction = std::function<void(std::string, bool, bool, unsigned char** buffer, size_t &buffer_size)>;

void font_add_file_loader(FontLoadFileFunction fnc);
void font_add_memory_loader(FontLoadMemoryFunction fnc);

} // namespace enigma
