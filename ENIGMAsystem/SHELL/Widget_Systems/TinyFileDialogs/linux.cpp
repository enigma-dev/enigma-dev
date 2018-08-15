#include <string>

using enigma_user::string_replace_all;
using std::string;

namespace enigms
{
  string tfd_add_escaping(string str)
  {
    return string_replace_all(str, "\"", "\\\"");
  }
}
