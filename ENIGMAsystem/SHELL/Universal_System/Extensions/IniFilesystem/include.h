#include "PFini.h"

//Additional functionality.
namespace enigma_user
{
std::string ini_full_file_text();
void ini_set_comment_char(char ch);  //Will only apply to the next ini_open() (we could also make it part of ini_open()).
}
