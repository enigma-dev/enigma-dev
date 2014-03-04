/**
  @file  write_globals.cpp
  @brief Implements the method in the C++ plugin that writes all global variables
         and constants to the engine.
  
  @section License
    Copyright (C) 2008-2014 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

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

/** @section impl_cpp Implementation (C++)
*** So what we're going to do is not trivial, but not killer. We're going to sort globals by
*** dependencies on other globals in their initializers. If these dependencies are circular,
*** we bitch up a mighty storm. Otherwise, we dump them to the file in whatever order makes
*** the compiler happy.
*** 
*** From there, we also create a "global" object, representing the special EDL object `global'.
*** This object contains a copy of all dot-accessed variables which could not be resolved to a
*** specific instance, as well as all variables explicitly accessed as `global.variable'.
**/
int lang_CPP::compile_write_globals(compile_context &ctex)
{
  ofstream wto;
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_globals.h").c_str(),ios_base::out);
    wto << gen_license;

    global_script_argument_count = 16; // Write all 16 arguments
    if (global_script_argument_count) {
      wto << "// Script arguments\n";
      wto << "variant argument0 = 0";
      for (int i = 1; i < global_script_argument_count; i++)
        wto << ", argument" << i << " = 0";
      wto << ";\n\n";
    }
    
    wto << "namespace enigma_user { " << endl;
    wto << "  unsigned int game_id = " << ctex.es->gameSettings.gameId << ";" << endl;
    wto << "}" << endl <<endl;

    wto << "//Default variable type: \"undefined\" or \"real\"" <<endl;
    wto << "const int variant::default_type = " <<(ctex.es->gameSettings.treatUninitializedAs0 ? "enigma::vt_real" : "-1") << ";" << endl << endl;

    wto << "namespace enigma {" << endl;
    wto << "  bool interpolate_textures = " << ctex.es->gameSettings.interpolate << ";" << endl;
    wto << "  bool forceSoftwareVertexProcessing = " << ctex.es->gameSettings.forceSoftwareVertexProcessing << ";" << endl;
    wto << "  bool isSizeable = " <<  ctex.es->gameSettings.allowWindowResize << ";" << endl;
    wto << "  bool showBorder = " << !ctex.es->gameSettings.dontDrawBorder    << ";" << endl;
    wto << "  bool showIcons = "  << !ctex.es->gameSettings.dontShowButtons   << ";" << endl;
    wto << "  bool freezeOnLoseFocus = " << ctex.es->gameSettings.freezeOnLoseFocus << ";" << endl;
    wto << "  bool treatCloseAsEscape = " << ctex.es->gameSettings.treatCloseAsEscape << ";" << endl;
    wto << "  bool isFullScreen = " << ctex.es->gameSettings.startFullscreen << ";" << endl;
    wto << "  int viewScale = " << ctex.es->gameSettings.scaling << ";" << endl;
    wto << "}" << endl;

    // TODO: NEWPARSER: Actual global iteration here.
    //for (parsed_script::globit i = ctex.global->globals.begin(); i != ctex.global->globals.end(); i++)
    //  wto << i->second.type << " " << i->second.prefix << i->first << i->second.suffix << ";" << endl;
    
    //This part needs written into a global object_parent class instance elsewhere.
    //for (globit i = global->dots.begin(); i != global->globals.end(); i++)
    //  wto << i->second->type << " " << i->second->prefixes << i->second->name << i->second->suffixes << ";" << endl;
    
    wto << endl;
    wto << "namespace enigma" << endl
        << "{" << endl
        << "  struct ENIGMA_global_structure: object_locals" << endl
        << "  {" << endl;
    
    // NEWPARSER: TODO: FIXME: WRITE DOT ACCESSED LOCALS INTO THE GLOBAL SCOPE HERE

    wto << "    ENIGMA_global_structure(const int _x, const int _y): object_locals(_x,_y) {}" << endl
        << "  };" << endl
        << "  object_basic *ENIGMA_global_instance = new ENIGMA_global_structure(global,global);" << endl
        << "}" << endl;
  
  wto << endl; // Two newlines is better than zero.
  wto.close(); // We're done here.
  return 0;
}
