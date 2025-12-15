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
#include "general/parse_basics_old.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string_view>

using namespace std;

#include "general/estring.h"

#include "backend/GameData.h"
#include "compiler/compile_common.h"

#include "languages/lang_CPP.h"
#include "languages/clang_adapter.h"

int global_script_argument_count = 0;

static string esc(std::string_view str_view) {
  string res;
  res.reserve(str_view.length());
  for (size_t i = 0; i < str_view.length(); ++i) {
    char c = str_view[i];
    if (c == '\n') { res += "\\n"; continue; }
    if (c == '\r') { res += "\\r"; continue; }
    if (c == '\\') { res += "\\\\"; continue; }
    if (c == '\"') { res += "\\\""; continue; }
    res.append(1, c);
  }
  return res;
}

int lang_CPP::compile_writeGlobals(const GameData &game,
                                   const ParsedScope* global,
                                   const DotLocalMap &dot_accessed_locals,
                                   const ParsedObjectVec &parsed_objects) {
  // 'this' is lang_CPP*, so we can use it directly
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

  // Write standalone globals (variables NOT in dot_accessed_locals)
  // Variables in dot_accessed_locals will be written inside ENIGMA_global_structure instead
  for (parsed_object::cglobit i = global->globals.begin(); i != global->globals.end(); i++) {
    // Skip variables that are in dot_accessed_locals - they belong in ENIGMA_global_structure, not as standalone globals
    if (dot_accessed_locals.find(i->first) != dot_accessed_locals.end()) {
      continue;
    }
    wto << i->second.type << " " << i->second.prefix << i->first << i->second.suffix << ";" << endl;
  }
  //This part needs written into a global object_parent class instance elsewhere.
  //for (globit i = global->dots.begin(); i != global->globals.end(); i++)
  //  wto << i->second->type << " " << i->second->prefixes << i->second->name << i->second->suffixes << ";" << endl;
  wto << endl;

  wto << "namespace enigma" << endl << "{" << endl << "  struct ENIGMA_global_structure: object_locals" << endl << "  {" << endl;
  for (decciter i = dot_accessed_locals.begin(); i != dot_accessed_locals.end(); i++) { // Dots are vars that are accessed as something.varname.
    // Skip built-in instance variables (discovered by clang parser from C++ class hierarchy)
    // These are already part of the object tier system and should not be declared in global structure
    if (this->is_shared_local(i->first)) {
      continue;
    }
    
    // Also skip if it's a built-in constant from enigma_user namespace
    if (this->is_enigma_user_constant(i->first)) {
      // It's a built-in constant in enigma_user namespace, don't declare it in global structure
      continue;
    }
    
    // Note: Variables accessed via global.varname are added to global->globals by AddGlobal() (which then
    // gets copied to global_object.globals by link_globals()). So we can't use "in global->globals" alone
    // to determine if a variable is explicitly declared as global vs just accessed via global.varname.
    //
    // The key distinction: Variables explicitly declared as global (via compute_locals()) are added to
    // global->globals but are NOT in dot_accessed_locals (unless also accessed via global.varname).
    // Variables accessed via global.varname (via AddGlobal()) are added to both global->globals AND
    // dot_accessed_locals.
    //
    // Since we're iterating over dot_accessed_locals, all variables here are accessed via global.varname.
    // They should be in ENIGMA_global_structure, even if they're also in global->globals (from AddGlobal()).
    // Variables explicitly declared as global won't be in dot_accessed_locals (unless also accessed),
    // so they won't be in this loop. If they are also accessed, they're already global by declaration,
    // so they shouldn't be in ENIGMA_global_structure, but we can't easily distinguish that case.
    //
    // For now, include all variables in dot_accessed_locals in ENIGMA_global_structure, unless they're
    // explicitly declared as local (checked below).
    
    // Skip if this variable is explicitly declared as local in any object's LocalDeclarations()
    // Variables that are explicitly declared in objects should be local to those objects,
    // not in ENIGMA_global_structure. Only variables that are ONLY accessed via global.varname
    // (not explicitly declared) should be in ENIGMA_global_structure.
    bool explicitly_declared_as_local = false;
    bool declared_in_any_object = false;
    for (const parsed_object *obj : parsed_objects) {
      // A variable is declared in an object if it's in object->locals but NOT in object->globals
      // (meaning it was used directly, not accessed via global.varname).
      bool in_locals = (obj->locals.find(i->first) != obj->locals.end());
      bool in_globals = (obj->globals.find(i->first) != obj->globals.end());
      if (in_locals && !in_globals) {
        declared_in_any_object = true;
      }
      
      // Check if the variable is explicitly declared in LocalDeclarations()
      for (const ParsedEvent &pev : obj->all_events) {
        string addls = pev.ev_id.LocalDeclarations();
        if (addls.length() > 0) {
          // Improved check: look for "var name;" or "var name1, name2;" format
          // Also check for "local name;", "local var name;", "int name;", etc.
          size_t pos = 0;
          while ((pos = addls.find(i->first, pos)) != string::npos) {
            // Check if it's a whole word (not part of another identifier)
            bool valid_before = (pos == 0);
            if (!valid_before) {
              char before = addls[pos - 1];
              valid_before = (is_useless(before) || before == ',' || before == ';' || 
                             (pos >= 4 && addls.substr(pos - 4, 4) == "var ") ||
                             (pos >= 6 && addls.substr(pos - 6, 6) == "local ") ||
                             (pos >= 11 && addls.substr(pos - 11, 11) == "local var "));
            }
            bool valid_after = (pos + i->first.length() >= addls.length());
            if (!valid_after) {
              char after = addls[pos + i->first.length()];
              valid_after = (is_useless(after) || after == ',' || after == ';' || after == '=');
            }
            if (valid_before && valid_after) {
              explicitly_declared_as_local = true;
              break;
            }
            pos += i->first.length();
          }
          if (explicitly_declared_as_local) break;
        }
      }
      if (explicitly_declared_as_local) break;
    }
    
    // If explicitly declared as local, skip from ENIGMA_global_structure
    if (explicitly_declared_as_local) {
      continue;
    }
    
    // Skip variables that are declared in any object (in object->locals but not in object->globals).
    // Variables only accessed via global.varname are in both object->locals and object->globals,
    // so they're not declared in objects and should be in ENIGMA_global_structure.
    if (declared_in_any_object) {
      continue;
    }
    
    wto << "    " << i->second.type << " " << i->second.prefix << i->first << i->second.suffix << ";" << endl;
  }

  wto << "    ENIGMA_global_structure(const int _x, const int _y): object_locals(_x,_y) {}" << endl << "  };" << endl << "  object_basic *ENIGMA_global_instance = new ENIGMA_global_structure(global,global);" << endl << "}";
  wto << endl;
  wto.close();
  return 0;
}
