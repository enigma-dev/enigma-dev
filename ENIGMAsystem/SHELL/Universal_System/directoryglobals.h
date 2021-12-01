#include <string>

#include "estring.h"

#include "../../../shared/apiprocess/process.h"
#include "../../../shared/tempdir/tempdir.h"

namespace enigma_user {

std::string get_working_directory();
std::string get_program_directory();
std::string get_program_filename();
std::string get_program_pathname();

#define working_directory get_working_directory()
#define program_directory get_program_directory()
#define program_filename get_program_filename()
#define program_pathname get_program_pathname()
#define temp_directory get_temp_directory()

} // namespace enigma_user
