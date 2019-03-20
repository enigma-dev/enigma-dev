#include "search.h"

#include <cstdlib> // std::getenv 

namespace enigma {

void init_font_search_dirs() {
  fontSearchPaths = {
    std::string(std::getenv("HOME")) + "/.local/share/fonts",
    std::string(std::getenv("HOME")) + "/.fonts"
    "/usr/share/fonts/TTF/",
    "/usr/share/fonts/truetype/"
    "/usr/share/fonts/truetype/",
    "/usr/X11R6/lib/X11/fonts/ttfonts/",
    "/usr/X11R6/lib/X11/fonts/",
  };
}

}
