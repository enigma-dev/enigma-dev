#ifndef ENIGMA_TTF_H
#define ENIGMA_TTF_H

#include <string>

namespace enigma_user {

void font_add_search_path(std::string path);
std::string font_find(std::string name, bool bold, bool italic, bool fallback = true);
int font_add(std::string name, unsigned size, bool bold = false, bool italic = false, unsigned first = 32, unsigned last = 128, bool fallback = true);

}

#endif
