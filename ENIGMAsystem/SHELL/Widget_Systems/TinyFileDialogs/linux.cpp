#include "unix.h"
#include "Universal_System/estring.h"
#include <string>

using std::string;

using enigma_user::string_replace_all;

namespace enigma
{
  string tfd_add_escaping(string str)
  {
    string result;
    result = string_replace_all(str, "\"", "\\\"");
    result += string_replace_all(str, "_", "__");
    return result;
  }
}
