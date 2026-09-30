/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008, 2018 Josh Ventura                                       **
**  Copyright (C) 2014 Seth N. Hetu                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "settings.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <iterator>
#include <vector>

#include "libpng-util.h"

using namespace std;

#include "general/estring.h"

#include "backend/GameData.h"
#include "compiler/compile_common.h"

#include "languages/lang_CPP.h"

int global_script_argument_count = 0;

static string esc(std::string_view str) {
  string res;
  res.reserve(str.length());
  for (size_t i = 0; i < str.length(); ++i) {
    char c = str[i];
    if (c == '\n') { res += "\\n"; continue; }
    if (c == '\r') { res += "\\r"; continue; }
    if (c == '\\') { res += "\\\\"; continue; }
    if (c == '\"') { res += "\\\""; continue; }
    res.append(1, c);
  }
  return res;
}

// Constant values are GML; rewrite the literal forms C++ spells differently.
static string constant_value(const string &value) {
  const char q = value.empty() ? 0 : value.front();
  if ((q == '"' || q == '\'') && value.size() >= 2 && value.find(q, 1) == value.size() - 1)
    return "std::string{\"" + esc(std::string_view(value).substr(1, value.size() - 2)) + "\"}";
  if (q == '$' && value.size() > 1 &&
      value.find_first_not_of("0123456789abcdefABCDEF", 1) == string::npos)
    return "0x" + value.substr(1);
  return value;
}

// The game icon (.ico) as _NET_WM_ICON data: per image, width, height, then
// ARGB pixels top-down. Handles PNG entries and 1/4/8/24/32-bit DIBs with
// their AND mask. Empty when there is no icon or it can't be read.
static std::vector<uint32_t> ico_to_argb(const string &path) {
  std::vector<uint32_t> out;
  ifstream f(path, ios::binary);
  const string d((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
  auto u8 = [&](size_t p) -> uint32_t { return p < d.size() ? (unsigned char)d[p] : 0; };
  auto u16 = [&](size_t p) { return u8(p) | u8(p + 1) << 8; };
  auto u32 = [&](size_t p) { return u16(p) | u16(p + 2) << 16; };
  if (d.size() < 6 || u16(0) != 0 || u16(2) != 1) return out;
  for (uint32_t e = 0, count = u16(4); e < count; e++) {
    const size_t off = u32(6 + 16 * e + 12), size = u32(6 + 16 * e + 8);
    if (off >= d.size() || size > d.size() - off) continue;
    if (d.compare(off, 8, "\x89PNG\r\n\x1a\n") == 0) {
      const auto tmp = std::filesystem::temp_directory_path() / "enigma_game_icon.png";
      ofstream(tmp, ios::binary).write(d.data() + off, size);
      unsigned char *px = nullptr;
      unsigned w = 0, h = 0;
      if (libpng_decode32_file(&px, &w, &h, tmp.u8string().c_str()) == 0 && px) {
        out.push_back(w), out.push_back(h);
        for (size_t i = 0; i < size_t(w) * h; i++)
          out.push_back(uint32_t(px[4 * i + 3]) << 24 | px[4 * i] << 16 | px[4 * i + 1] << 8 | px[4 * i + 2]);
      }
      delete[] px;
      std::filesystem::remove(tmp);
      continue;
    }
    // BITMAPINFOHEADER; the height counts the colour rows and the AND mask.
    const uint32_t hs = u32(off), w = u32(off + 4), h = u32(off + 8) / 2, bpp = u16(off + 14);
    if (hs < 40 || !w || !h || w > 1024 || h > 1024 || (bpp != 1 && bpp != 4 && bpp != 8 && bpp != 24 && bpp != 32))
      continue;
    const size_t colours = bpp <= 8 ? (u32(off + 32) ? u32(off + 32) : 1u << bpp) : 0;
    const size_t pal = off + hs, xor_at = pal + 4 * colours, stride = (w * bpp + 31) / 32 * 4;
    const size_t and_at = xor_at + stride * h, and_stride = (w + 31) / 32 * 4;
    if (and_at + and_stride * h > d.size()) continue;
    bool has_alpha = false;
    if (bpp == 32)
      for (size_t i = 0; i < size_t(w) * h && !has_alpha; i++) has_alpha = u8(xor_at + 4 * i + 3);
    out.push_back(w), out.push_back(h);
    for (uint32_t y = 0; y < h; y++) {
      const size_t row = xor_at + stride * (h - 1 - y), mrow = and_at + and_stride * (h - 1 - y);
      for (uint32_t x = 0; x < w; x++) {
        uint32_t b, g, r, a = (u8(mrow + x / 8) >> (7 - x % 8) & 1) ? 0 : 255;
        if (bpp <= 8) {
          const uint32_t idx = u8(row + x * bpp / 8) >> (8 - bpp - x * bpp % 8) & ((1u << bpp) - 1);
          b = u8(pal + 4 * idx), g = u8(pal + 4 * idx + 1), r = u8(pal + 4 * idx + 2);
        } else {
          const size_t p = row + x * (bpp / 8);
          b = u8(p), g = u8(p + 1), r = u8(p + 2);
          if (has_alpha) a = u8(p + 3);
        }
        out.push_back(a << 24 | r << 16 | g << 8 | b);
      }
    }
  }
  return out;
}

int lang_CPP::compile_writeGlobals(const GameData &game,
                                   const ParsedScope* global,
                                   const DotLocalMap &dot_accessed_locals) {
  ofstream wto;
  wto.open((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_globals.h").u8string().c_str(),ios_base::out);
  wto << license;

  global_script_argument_count=16; //write all 16 arguments
  if (global_script_argument_count) {
    wto << "// Script arguments\n";
    wto << "variant argument0 = 0";
    for (int i = 1; i < global_script_argument_count; i++)
      wto << ", argument" << i << " = 0";
    wto << ";\n\n";
  }

  wto << "namespace enigma_user { " << endl;
  //wto << "  string working_directory = \"\";" << endl; // moved over to PFmain.h
  wto << "  unsigned int game_id = " << game.settings.general().game_id() << ";"
      << endl;
  wto << "}" << endl <<endl;

  wto << "namespace enigma_user {" << endl;
  for (size_t i = 0; i < game.constants.size(); i++) {
    const GameData::Constant &con = game.constants[i];
    wto << "  #define " << con.name << " (" << constant_value(con.value) <<")" << endl;
  }
  wto << "}" << endl;

  const auto &csets = game.settings.compiler();
  const auto &gsets = game.settings.graphics();
  const auto &wsets = game.settings.windowing();
  const auto &gameInfo = game.gameInfo;

  wto << "//Default variable type: \"undefined\" or \"real\"" << endl;
  wto << "const int variant::default_type = "
      << (csets.treat_uninitialized_vars_as_zero()
              ? "ty_real" : "ty_undefined") << ";"
      << endl << endl;

  wto << "namespace enigma {" << endl;
  wto << "  bool interpolate_textures = " << gsets.interpolate_textures() << ";" << endl;
  wto << "  bool forceSoftwareVertexProcessing = " << gsets.force_software_vertex_processing() << ";" << endl;
  wto << "  bool isSizeable = "         << wsets.is_sizeable() << ";" << endl;
  wto << "  bool showBorder = "         << wsets.show_border() << ";" << endl;
  wto << "  bool showIcons = "          << wsets.show_icons() << ";" << endl;
  wto << "  bool freezeOnLoseFocus = "  << wsets.freeze_on_lose_focus() << ";" << endl;
  wto << "  bool treatCloseAsEscape = " << wsets.treat_close_as_escape() << ";" << endl;
  wto << "  bool gml_hash_newlines = " << !compatibility_opts_.use_cpp_escapes << ";" << endl;
  wto << "  bool isFullScreen = " << wsets.start_in_fullscreen() << ";" << endl;
  wto << "  int viewScale = " << gsets.view_scale() << ";" << endl;
  wto << "  int windowColor = " << gsets.color_outside_room_region() << ";" << endl;
  {
    const std::vector<uint32_t> icon = ico_to_argb(string(game.settings.general().game_icon()));
    wto << "  extern const unsigned long game_icon[] = {";
    for (size_t i = 0; i < icon.size(); i++) {
      if (i) wto << ",";
      if (i % 16 == 0) wto << "\n    ";
      wto << icon[i];
    }
    if (icon.empty()) wto << "0";
    wto << "};" << endl;
    wto << "  extern const unsigned game_icon_size = " << icon.size() << ";" << endl;
  }

  wto << "  string gameInfoText = \"" << esc(gameInfo.text()) << "\";" << endl;
  wto << "  string gameInfoCaption = \"" << gameInfo.form_caption() << "\";" << endl;
  wto << "  int gameInfoBackgroundColor = " << gameInfo.background_color() << ";" << endl;
  wto << "  int gameInfoLeft = " << gameInfo.left() << ";" << endl;
  wto << "  int gameInfoTop = " << gameInfo.top() << ";" << endl;
  wto << "  int gameInfoWidth = " << gameInfo.right() - gameInfo.left() << ";" << endl;
  wto << "  int gameInfoHeight = " << gameInfo.bottom() - gameInfo.top() << ";" << endl;
  wto << "  bool gameInfoEmbedGameWindow = " << gameInfo.embed_game_window() << ";" << endl;
  wto << "  bool gameInfoShowBorder = " << gameInfo.show_border() << ";" << endl;
  wto << "  bool gameInfoAllowResize = " << gameInfo.allow_resize() << ";" << endl;
  wto << "  bool gameInfoStayOnTop = " << gameInfo.stay_on_top() << ";" << endl;
  wto << "  bool gameInfoPauseGame = " << gameInfo.pause_game() << ";" << endl;
  wto << "}" << endl;

  for (parsed_object::cglobit i = global->globals.begin(); i != global->globals.end(); i++)
    wto << i->second.type << " " << i->second.prefix << i->first << i->second.suffix << ";" << endl;
  //This part needs written into a global object_parent class instance elsewhere.
  //for (globit i = global->dots.begin(); i != global->globals.end(); i++)
  //  wto << i->second->type << " " << i->second->prefixes << i->second->name << i->second->suffixes << ";" << endl;
  wto << endl;

  wto << "namespace enigma" << endl << "{" << endl << "  struct ENIGMA_global_structure: object_locals" << endl << "  {" << endl;
  for (decciter i = dot_accessed_locals.begin(); i != dot_accessed_locals.end(); i++) // Dots are vars that are accessed as something.varname.
    wto << "    " << i->second.type << " " << i->second.prefix << i->first << i->second.suffix << ";" << endl;

  wto << "    ENIGMA_global_structure(const int _x, const int _y): object_locals(_x,_y) {}" << endl << "  };" << endl << "  object_basic *ENIGMA_global_instance = new ENIGMA_global_structure(global,global);" << endl << "}";
  wto << endl;
  wto.close();
  return 0;
}
