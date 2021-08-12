#include <string>

namespace enigma {

const char *DirectoryGetTemporaryPath();

} // namespace enigma

namespace enigma_user {

std::string get_temp_directory();

#define temp_directory get_temp_directory()

} // namespace enigma_user
