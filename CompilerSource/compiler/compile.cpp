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

#include "../OS_Switchboard.h" //Tell us where the hell we are
#include "../backend/EnigmaStruct.h" //LateralGM interface structures

#include "../general/darray.h"

#ifdef _WIN32
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif


#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#define flushl (fflush(stdout), "\n")
#define flushs (fflush(stdout), " ")

#include "../externs/externs.h"
#include "../syntax/syncheck.h"
#include "../parser/parser.h"
#include "compile_includes.h"
#include "compile_common.h"

#include "components/components.h"
#include "../gcc_interface/gcc_backend.h"

#include "../general/bettersystem.h"
#include "event_reader/event_parser.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}

void clear_ide_editables()
{
  ofstream wto;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/GAME_SETTINGS.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_globals.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_eventpointers.h"); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_wildclass.cpp"); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_wildclass.h"); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_evparent.h"); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_events.cpp"); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_events.h"); wto.close();
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomcreates.h",ios_base::out);
    wto << "int instdata[] = { 0 }; //Empty until game is built via ENIGMA\n";
    //wto << "enigma::room_max = 1;\nenigma::maxid = 100001;\n";
  wto.close();
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h",ios_base::out);
    wto <<
    "var  alarm;\nvar  image_single;\nvar  image_speed;\nvar  path_endaction;\nvar  path_index;\nvar  path_orientation;\n"
    "var  path_position;\nvar  path_positionprevious;\nvar  path_scale;\nvar  path_speed;\nvar  timeline_index;\n"
    "var  timeline_position;\nvar  timeline_speed;\n";
  wto.close();

//FIXME: Accessors are required for sprite_width and height, as well as all bbox_ variables
  
  wto.open("ENIGMAsystem/SHELL/API_Switchboard.h",ios_base::out);
    wto << license;
    wto << "#define ENIGMA_GS_OPENGL 1\n#define " << TARGET_PLATFORM_NAME << " 1\n";
  wto.close();
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/LIBINCLUDE.h");
    wto << license;
    wto << "/*************************************************************\nOptionally included libraries\n****************************/\n";
    wto << "#define STRINGLIB 1\n#define COLORSLIB 1\n#define STDRAWLIB 1\n#define PRIMTVLIB 1\n#define WINDOWLIB 1\n#define FONTPOLYS 1\n"
           "#define STDDRWLIB 1\n#define GMSURFACE 0\n#define BLENDMODE 1\n#define COLLIGMA  0\n";
    wto << "/***************\nEnd optional libs\n ***************/\n";
  wto.close();
  
}

// modes: 0=run, 1=debug, 2=build, 3=compile
enum { emode_run, emode_debug, emode_build, emode_compile };
dllexport int compileEGMf(EnigmaStruct *es, const char* filename, int mode)
{
  // CLean up from any previous executions.
    parsed_objects.clear(); //Make sure we don't dump in any old object code...
    shared_locals_clear();  //Forget inherited locals, we'll reparse them
    event_info_clear();     //Forget event definitions, we'll re-get them
  
  // Re-establish ourself
    // Read the locals that will be included with each instance
    if (shared_locals_load() != 0) {
      cout << "Failed to determine locals; couldn't determine bottom tier: is ENIGMA configured correctly?";
      return E_ERROR_LOAD_LOCALS;
    }
    
    //Read the types of events
    event_parse_resourcefile();
  
  // Pick apart the sent resources
  cout << "Location in memory of structure: " << es << flushl;
  if (es == NULL)
    return -1;//E_ERROR_PLUGIN_FUCKED_UP;
  
  cout << "File version: " << es->fileVersion << endl << flushl;
  if (es->fileVersion != 600)
    cout << "Error: Incorrect version. File is too " << ((es->fileVersion > 600)?"new":"old") << " for this compiler.";
  
  /**  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment One: This segment of the compile process is responsible for
    translating the code into C++. Basically, anything essential to the
    compilation of said code is dealt with during this segment.
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
  
  ///The segment begins by adding resource names to the collection of variables that should not be automatically re-scoped.  
  
  //First, we make a space to put our globals.
    globals_scope = scope_get_using(&global_scope);
    globals_scope = globals_scope->members["ENIGMA Resources"] = new externs("ENIGMA Resources",globals_scope,NULL,EXTFLAG_NAMESPACE);
  
  
  
  //Next, add the resource names to that list
  cout << "COPYING SOME FUCKING RESOURCES:" << flushl;
  
  cout << "Copying sprite names [" << es->spriteCount << "]" << flushl;
  for (int i = 0; i < es->spriteCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->sprites[i].name);
    
  cout << "Copying sound names [" << es->soundCount << "]" << flushl;
  for (int i = 0; i < es->soundCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->sounds[i].name);
    
  cout << "Copying background names [" << es->backgroundCount << "]" << flushl;
  for (int i = 0; i < es->backgroundCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->backgrounds[i].name);
    
  cout << "Copying path names [kidding, these are totally not implemented :P] [" << es->pathCount << "]" << flushl;
  for (int i = 0; i < es->pathCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->paths[i].name);
    
  cout << "Copying script names [" << es->scriptCount << "]" << flushl;
  for (int i = 0; i < es->scriptCount; i++)
    quickmember_script(globals_scope,es->scripts[i].name);
    
  cout << "Copying font names [kidding, these are totally not implemented :P] [" << es->fontCount << "]" << flushl;
  for (int i = 0; i < es->fontCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->fonts[i].name);
    
  cout << "Copying timeline names [kidding, these are totally not implemented :P] [" << es->timelineCount << "]" << flushl;
  for (int i = 0; i < es->timelineCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->timelines[i].name);
  
  cout << "Copying object names [" << es->gmObjectCount << "]" << flushl;
  for (int i = 0; i < es->gmObjectCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->gmObjects[i].name);
  
  cout << "Copying room names [" << es->roomCount << "]" << flushl;
  for (int i = 0; i < es->roomCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->rooms[i].name);
  
  
  
  ///Next we do a simple parse of the code, scouting for some variable names and adding semicolons.
  cout << "SYNTAX CHECKING AND PRIMARY PARSING:" << flushl;
  
  cout << es->scriptCount << " Scripts:" << endl;
  parsed_script *scripts[es->scriptCount];
  
  scr_lookup.clear();
  used_funcs::zero();
  
  int res;
  
  res = compile_parseAndLink(es,scripts);
  if (res) return res;
  
  //Export resources to each file.
  
  ofstream wto;
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/GAME_SETTINGS.h",ios_base::out);
    wto << license;
    wto << "#define SHOWERRORS 1\n";
    wto << "#define ABORTONALL 0\n";
    wto << "#define ASSUMEZERO 0\n";
    wto << "#define PRIMBUFFER 0\n";
    wto << "#define PRIMDEPTH2 6\n";
    wto << "#define AUTOLOCALS 0\n";
    wto << "#define MODE3DVARS 0\n";
    wto << "void ABORT_ON_FATAL_ERRORS() { " << (false?"exit(0);":"") << " }\n";
    wto << '\n';
  wto.close();
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h",ios_base::out);
    wto << license;
    wto << "#define BUILDMODE " << 0 << "\n";
    wto << "#define DEBUGMODE " << 0 << "\n";
    wto << '\n';
  wto.close();
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h",ios_base::out);
    wto << license;
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    {
      wto << "case " << i->second->id << ":\n";
      wto << "    enigma::instance_list[idn] = new enigma::" << i->second->name <<";\n";
      wto << "  break;\n";
    }
    wto << '\n';
  wto.close();
  
  
  //NEXT FILE ----------------------------------------
  //Object switch: A listing of all object IDs and the code to allocate them.
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h",ios_base::out);
    wto << license;
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    {
      wto << "case " << i->second->id << ":\n";
      wto << "    enigma::instance_list[idn] = new enigma::OBJ_" << i->second->name <<"(x,y,idn);\n";
      wto << "  break;\n";
    }
    wto << '\n';
  wto.close();
  
  
  //NEXT FILE ----------------------------------------
  //Resource names: Defines integer constants for all resources.
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h",ios_base::out);
    wto << license;
    
    wto << "enum //object names\n{\n";
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
      wto << "  " << i->second->name << " = " << i->first << ",\n"; 
    wto << "};\n\n";
    
    wto << "enum //sprite names\n{\n";
    for (int i = 0; i < es->spriteCount; i++)
      wto << "  " << es->sprites[i].name << " = " << es->sprites[i].id << ",\n"; 
    wto << "};\n\n";
    
    wto << "enum //sound names\n{\n";
    for (int i = 0; i < es->soundCount; i++)
      wto << "  " << es->sounds[i].name << " = " << es->sounds[i].id << ",\n"; 
    wto << "};\n\n";
    
    wto << "enum //room names\n{\n";
    for (int i = 0; i < es->roomCount; i++)
      wto << "  " << es->rooms[i].name << " = " << es->rooms[i].id << ",\n"; 
    wto << "};\n\n";
  wto.close();
  
  parsed_object EGMglobal;
  
  link_globals(&EGMglobal,es,scripts);
  if (res) return res;
  
  res = compile_writeGlobals(es,&EGMglobal);
  if (res) return res;
  
  res = compile_writeObjectData(es,&EGMglobal);
  if (res) return res;
  
  res = compile_writeRoomData(es);
  if (res) return res;
  
  res = compile_writeRoomData(es);
  if (res) return res;
  
  res = compile_writeDefraggedEvents(es);
  if (res) return res;
  
  
  
  /**  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment two: Now that the game has been exported as C++ and raw
    resources, our task is to compile the game itself via GNU Make.
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
  
  string gflags = "";
  #if   TARGET_PLATFORM_ID == OS_WINDOWS
    string glinks = "-lopengl32 '../additional/zlib/libzlib.a' -lcomdlg32 -lgdi32";
    string graphics = "OpenGL";
    string platform = "windows";
  #else
    string glinks = "-lGL -lz";
    string graphics = "OpenGL";
    string platform = "xlib";
  #endif
  
  string make = "Game ";
  make += "GMODE=Run ";
  make += "GFLAGS=\"" + gflags   + "\" ";
  make += "GLINKS=\"" + glinks   + "\" ";
  make += "GRAPHICS=" + graphics + " ";
  make += "PLATFORM=" + platform + " ";
  
  cout << "Running make from `" << MAKE_location << "'" << flushl;
  cout << "Full command line: " << MAKE_location << " " << make << flushl;
  int makeres = better_system(MAKE_location,make);
  if (makeres) {
    cout << "----Make returned error " << makeres << "----------------------------------\n";
    return E_ERROR_BUILD;
  }
  cout << "+++++Make completed successfully.++++++++++++++++++++++++++++++++++++\n";
  
  
  
  
  
  /**  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment three: Add resources into the game executable. They are
    literally tacked on to the end of the file for now. They should
    have an option in the config file to pass them to some resource
    linker sometime in the future.
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
  
  FILE *gameModule = fopen("ENIGMAsystem/SHELL/game.exe","ab");
  if (!gameModule) {
    cout << "Failed to append resources to the game. Did compile actually succeed?" << flushl;
    return 12;
  }
  
  fseek(gameModule,0,SEEK_END); //necessary on Windows for no reason.
  int resourceblock_start = ftell(gameModule);
  
  if (resourceblock_start < 128) {
    cout << "Compiled game is clearly not a working module; cannot continue" << flushl;
    return 13;
  }
  
  
  // Start by setting off our location with a DWord of NULLs
  fwrite("\0\0\0",1,4,gameModule);
  
  res = module_write_sprites(es, gameModule);
  if (res) return res;
  
  
  // Tell where the sprites are
  fwrite("\0\0\0\0sprn",8,1,gameModule);
  fwrite(&resourceblock_start,4,1,gameModule);
  
  // Debug print; we're done
  cout << "Finalized sprites." << flushl;
  
  cout << es->soundCount << " Sounds:" << endl;
  for (int i = 0; i < es->soundCount; i++) {
    cout << " " << es->sounds[i].name << endl;
    fflush(stdout);
  }
  
  
  //Close the game module; we're done adding resources
  cout << "Closing game module and running if requested." << flushl;
  fclose(gameModule);
  
  if (mode == emode_run or mode == emode_build or true)
  {
    int gameres = better_system("ENIGMAsystem/SHELL/game.exe","");
    cout << "Game returned " << gameres << "\n";
  }
  
  
  return 0;
};
