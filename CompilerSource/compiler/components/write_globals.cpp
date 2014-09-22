/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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

#include "makedir.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "syntax/syncheck.h"
#include "general/estring.h"
#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "compiler/compile_common.h"

#include "languages/lang_CPP.h"

int global_script_argument_count = 0;

static string esc(string str) {
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

int lang_CPP::compile_writeGlobals(EnigmaStruct* es, parsed_object* global)
{
  ofstream wto;
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_globals.h").c_str(),ios_base::out);
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
    wto << "  unsigned int game_id = " << es->gameSettings.gameId << ";" << endl;
    wto << "}" << endl <<endl;

    wto << "//Default variable type: \"undefined\" or \"real\"" <<endl;
    wto << "const int variant::default_type = " <<(es->gameSettings.treatUninitializedAs0 ? "enigma::vt_real" : "-1") <<";" <<endl <<endl;

    wto << "namespace enigma {" << endl;
    wto << "  bool interpolate_textures = " << es->gameSettings.interpolate << ";" << endl;
    wto << "  bool forceSoftwareVertexProcessing = " << es->gameSettings.forceSoftwareVertexProcessing << ";" << endl;
    wto << "  bool isSizeable = " << es->gameSettings.allowWindowResize << ";" << endl;
    wto << "  bool showBorder = " << !es->gameSettings.dontDrawBorder << ";" << endl;
    wto << "  bool showIcons = " << !es->gameSettings.dontShowButtons << ";" << endl;
    wto << "  bool freezeOnLoseFocus = " << es->gameSettings.freezeOnLoseFocus << ";" << endl;
    wto << "  bool treatCloseAsEscape = " << es->gameSettings.treatCloseAsEscape << ";" << endl;
    wto << "  bool isFullScreen = " << es->gameSettings.startFullscreen << ";" << endl;
    wto << "  string gameInfoText = \"" << esc(es->gameInfo.gameInfoStr) << "\";" << endl;
    wto << "  string gameInfoCaption = \"" << es->gameInfo.formCaption << "\";" << endl;
    wto << "  int gameInfoBackgroundColor = " << javaColor(es->gameInfo.backgroundColor) << ";" << endl;
    wto << "  int gameInfoLeft = " << es->gameInfo.left << ";" << endl;
    wto << "  int gameInfoTop = " << es->gameInfo.top << ";" << endl;
    wto << "  int gameInfoWidth = " << es->gameInfo.width << ";" << endl;
    wto << "  int gameInfoHeight = " << es->gameInfo.height << ";" << endl;
    wto << "  bool gameInfoEmbedGameWindow = " << es->gameInfo.embedGameWindow << ";" << endl;
    wto << "  bool gameInfoShowBorder = " << es->gameInfo.showBorder << ";" << endl;
    wto << "  bool gameInfoAllowResize = " << es->gameInfo.allowResize << ";" << endl;
    wto << "  bool gameInfoStayOnTop = " << es->gameInfo.stayOnTop << ";" << endl;
    wto << "  bool gameInfoPauseGame = " << es->gameInfo.pauseGame << ";" << endl;
    wto << "  int viewScale = " << es->gameSettings.scaling << ";" << endl;
    wto << "  int windowColor = " << javaColor(es->gameSettings.colorOutsideRoom) << ";" << endl;
    wto << "}" << endl;

    for (parsed_object::globit i = global->globals.begin(); i != global->globals.end(); i++)
      wto << i->second.type << " " << i->second.prefix << i->first << i->second.suffix << ";" << endl;
    //This part needs written into a global object_parent class instance elsewhere.
    //for (globit i = global->dots.begin(); i != global->globals.end(); i++)
    //  wto << i->second->type << " " << i->second->prefixes << i->second->name << i->second->suffixes << ";" << endl;
    wto << endl;

    wto << "namespace enigma" << endl << "{" << endl << "  struct ENIGMA_global_structure: object_locals" << endl << "  {" << endl;
    for (deciter i = dot_accessed_locals.begin(); i != dot_accessed_locals.end(); i++) // Dots are vars that are accessed as something.varname.
      wto << "    " << i->second.type << " " << i->second.prefix << i->first << i->second.suffix << ";" << endl;

    wto << "    ENIGMA_global_structure(const int _x, const int _y): object_locals(_x,_y) {}" << endl << "  };" << endl << "  object_basic *ENIGMA_global_instance = new ENIGMA_global_structure(global,global);" << endl << "}";
    wto << endl;
  wto.close();
  return 0;
}
