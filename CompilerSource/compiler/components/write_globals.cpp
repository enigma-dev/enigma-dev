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

using namespace std;

#include "syntax/syncheck.h"
#include "general/estring.h"
#include "parser/parser.h"

#include "backend/GameData.h"
#include "compiler/compile_common.h"

#include "languages/lang_CPP.h"

int global_script_argument_count = 0;

static string esc(const string &str) {
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

int lang_CPP::compile_writeGlobals(const GameData &game,
                                   const parsed_object* global,
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
    wto << "  #define " << con.name << " (" << con.value <<")" << endl;
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
  wto << "  bool isFullScreen = " << wsets.start_in_fullscreen() << ";" << endl;
  wto << "  int viewScale = " << gsets.view_scale() << ";" << endl;
  wto << "  int windowColor = " << gsets.color_outside_room_region() << ";" << endl;

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
