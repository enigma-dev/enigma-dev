#include "search.h"

#include <cstdlib> // std::getenv 

namespace enigma {

void init_font_search_dirs() {
  fontSearchPaths = {
    std::string(std::getenv("windir")) + "/Fonts";
    std::string(std::getenv("windir")) + "/boot/Fonts";
  };
}

}
