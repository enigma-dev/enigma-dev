#include <string>

namespace enigma {
std::string font_lookup(std::string name, bool bold, bool italic, unsigned char** buffer, size_t &buffer_size);
}
