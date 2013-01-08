/**
  @file  write_globals.cpp
  @brief Implements the method in the C++ plugin that writes all global variables
         and constants to the engine.
  
  @section License
    Copyright (C) 2008-2013 Josh Ventura
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
*** 
*** F
**/
int lang_CPP::compile_writeGlobals(compile_context &ctex)
{
  ofstream wto;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_globals.h",ios_base::out);
    wto << gen_license;

    global_script_argument_count = 16; // Write all 16 arguments
    if (global_script_argument_count) {
      wto << "// Script arguments\n";
      wto << "variant argument0 = 0";
      for (int i = 1; i < global_script_argument_count; i++)
        wto << ", argument" << i << " = 0";
      wto << ";\n\n";
    }
    
    wto << "unsigned int game_id = " << ctex.es->gameSettings.gameId << ";" << endl;

    // NEWPARSER: TODO: FIXME: WRITE GLOBALS HERE
     
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
