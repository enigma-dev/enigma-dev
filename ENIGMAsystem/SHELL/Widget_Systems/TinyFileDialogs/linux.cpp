#include "Universal_System/estring.h"
#include <string>

using std::string;

using enigma_user::string_replace_all;

namespace enigms
{
  string tfd_add_escaping(string str)
  {
    return string_replace_all(str, "\"", "\\\"");
  }
}
