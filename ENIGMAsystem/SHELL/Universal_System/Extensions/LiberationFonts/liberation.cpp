#include "Universal_System/Extensions/ttf/provider.h"

namespace enigma {

void font_add_lib_font(std::string name, bool bold, bool italic, std::string& out_name) {
  const std::string base_directory = "C:/Users/Owner/Desktop/enigma-dev/ENIGMAsystem/SHELL/Universal_System/Extensions/LiberationFonts/ttfs/";
  std::string file_name = "LiberationSans"; // map the fucking name here
  std::string suffix = "Regular";
  if (bold && italic) {
    suffix = "BoldItalic";
  } else if (bold) {
    suffix = "Bold";
  } else if (italic) {
    suffix = "Italic";
  }

  out_name = base_directory + file_name + "-" + suffix + ".ttf";
}

void extension_liberationfonts_init() {
  font_add_file_loader(font_add_lib_font);
}

} // namespace
