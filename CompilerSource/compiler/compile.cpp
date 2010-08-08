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

#define idpr(x,y) \
  ide_dia_progress_text(x); \
  ide_dia_progress(y);

#include <string>
#include <iostream>
#include <fstream>
#include "../backend/ideprint.h"

using namespace std;

#include "../backend/JavaCallbacks.h"
#include "../externs/externs.h"
#include "../syntax/syncheck.h"
#include "../parser/parser.h"
#include "compile_includes.h"
#include "compile_common.h"

#include "../settings-parse/crawler.h"

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
enum { emode_run, emode_debug, emode_build, emode_compile, emode_rebuild };
dllexport int compileEGMf(EnigmaStruct *es, const char* filename, int mode)
{
  cout << "Initializing dialog boxes" << endl;
    ide_dia_clear();
    ide_dia_open();
  cout << "Initialized." << endl;
  
  edbg << "Building for mode (" << mode << ")" << flushl;
  
  // CLean up from any previous executions.
  edbg << "Cleaning up from previous executions" << flushl;
    parsed_objects.clear(); //Make sure we don't dump in any old object code...
    shared_locals_clear();  //Forget inherited locals, we'll reparse them
    event_info_clear();     //Forget event definitions, we'll re-get them
  
  // Re-establish ourself
    // Read the locals that will be included with each instance
    if (shared_locals_load() != 0) {
      user << "Failed to determine locals; couldn't determine bottom tier: is ENIGMA configured correctly?";
      idpr("ENIGMA Misconfiguration",-1); return E_ERROR_LOAD_LOCALS;
    }
    edbg << "Grabbing locals" << flushl;  
      string high_level_shared_locals = extensions::compile_local_string();
  
  //Read the types of events
  event_parse_resourcefile();
  
  // Pick apart the sent resources
  edbg << "Location in memory of structure: " << (void*)es << flushl;
  if (es == NULL) {
    idpr("Java ENIGMA plugin dropped its ass.",-1);
    return E_ERROR_PLUGIN_FUCKED_UP;
  }
  
  edbg << "File version: " << es->fileVersion << flushl << flushl;
  if (es->fileVersion != 800)
    edbg << "Error: Incorrect version. File is too " << ((es->fileVersion > 800)?"new":"old") << " for this compiler.";
  
  
  
  
  /**** Segment One: This segment of the compile process is responsible for
  * @ * translating the code into C++. Basically, anything essential to the
  *//// compilation of said code is dealt with during this segment.
  
  ///The segment begins by adding resource names to the collection of variables that should not be automatically re-scoped.  
  
  
  //First, we make a space to put our globals.
    globals_scope = scope_get_using(&global_scope);
    extiter gso = globals_scope->members.find("ENIGMA Resources"); if (gso != globals_scope->members.end()) delete gso->second;
    globals_scope = globals_scope->members["ENIGMA Resources"] = new externs("ENIGMA Resources",globals_scope,NULL,EXTFLAG_NAMESPACE);
  
  idpr("Copying resources",1);
  
  //Next, add the resource names to that list
  edbg << "COPYING SOME F*CKING RESOURCES:" << flushl;
  
  edbg << "Copying sprite names [" << es->spriteCount << "]" << flushl;
  for (int i = 0; i < es->spriteCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->sprites[i].name);
    
  edbg << "Copying sound names [" << es->soundCount << "]" << flushl;
  for (int i = 0; i < es->soundCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->sounds[i].name);
    
  edbg << "Copying background names [" << es->backgroundCount << "]" << flushl;
  for (int i = 0; i < es->backgroundCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->backgrounds[i].name);
    
  edbg << "Copying path names [kidding, these are totally not implemented :P] [" << es->pathCount << "]" << flushl;
  for (int i = 0; i < es->pathCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->paths[i].name);
    
  edbg << "Copying script names [" << es->scriptCount << "]" << flushl;
  for (int i = 0; i < es->scriptCount; i++)
    quickmember_script(globals_scope,es->scripts[i].name);
    
  edbg << "Copying font names [kidding, these are totally not implemented :P] [" << es->fontCount << "]" << flushl;
  for (int i = 0; i < es->fontCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->fonts[i].name);
    
  edbg << "Copying timeline names [kidding, these are totally not implemented :P] [" << es->timelineCount << "]" << flushl;
  for (int i = 0; i < es->timelineCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->timelines[i].name);
  
  edbg << "Copying object names [" << es->gmObjectCount << "]" << flushl;
  for (int i = 0; i < es->gmObjectCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->gmObjects[i].name);
  
  edbg << "Copying room names [" << es->roomCount << "]" << flushl;
  for (int i = 0; i < es->roomCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->rooms[i].name);
  
  
  
  /// Next we do a simple parse of the code, scouting for some variable names and adding semicolons.
  
  idpr("Checking Syntax and performing Preliminary Parsing",2);
  
  edbg << "SYNTAX CHECKING AND PRIMARY PARSING:" << flushl;
  
  edbg << es->scriptCount << " Scripts:" << flushl;
  parsed_script *scripts[es->scriptCount];
  
  scr_lookup.clear();
  used_funcs::zero();
  
  int res;
  #define irrr() if (res) { idpr("Error occurred; see scrollback for details.",-1); return res; }
  
  res = compile_parseAndLink(es,scripts);
  irrr();
  
  
  //Export resources to each file.
  
  ofstream wto;
  idpr("Outputting Resources in Various Places...",10);
  
  // FIRST FILE
  // Modes and settings.
  
  edbg << "Writing modes and settings" << flushl;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/GAME_SETTINGS.h",ios_base::out);
    wto << license;
    wto << "#define ASSUMEZERO 0\n";
    wto << "#define PRIMBUFFER 0\n";
    wto << "#define PRIMDEPTH2 6\n";
    wto << "#define AUTOLOCALS 0\n";
    wto << "#define MODE3DVARS 0\n";
    wto << "void ABORT_ON_ALL_ERRORS() { " << (false?"exit(0);":"") << " }\n";
    wto << '\n';
  wto.close();
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h",ios_base::out);
    wto << license;
    wto << "#define BUILDMODE " << 0 << "\n";
    wto << "#define DEBUGMODE " << 0 << "\n";
    wto << '\n';
  wto.close();
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h",ios_base::out);
    wto << high_level_shared_locals;
  wto.close();
  
  
  //NEXT FILE ----------------------------------------
  //Object switch: A listing of all object IDs and the code to allocate them.
  edbg << "Writing object switch" << flushl;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h",ios_base::out);
    wto << license;
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    {
      wto << "case " << i->second->id << ":\n";
      wto << "    new enigma::OBJ_" << i->second->name <<"(x,y,idn);\n";
      wto << "  break;\n";
    }
    wto << '\n';
  wto.close();
  
  
  //NEXT FILE ----------------------------------------
  //Resource names: Defines integer constants for all resources.
  int max;
  edbg << "Writing resource names and maxima" << flushl;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h",ios_base::out);
    wto << license;
    
    wto << "enum //object names\n{\n";
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
      wto << "  " << i->second->name << " = " << i->first << ",\n"; 
    wto << "};\n\n";
    
    max = 0;
    wto << "enum //sprite names\n{\n";
    for (int i = 0; i < es->spriteCount; i++) {
      if (es->sprites[i].id >= max) max = es->sprites[i].id + 1;
      wto << "  " << es->sprites[i].name << " = " << es->sprites[i].id << ",\n"; 
    } wto << "};\nnamespace enigma { size_t sprite_idmax = " << max << "; }\n\n";
    
    max = 0;
    wto << "enum //sound names\n{\n";
    for (int i = 0; i < es->soundCount; i++) {
      if (es->sounds[i].id >= max) max = es->sounds[i].id + 1;
      wto << "  " << es->sounds[i].name << " = " << es->sounds[i].id << ",\n"; 
    } wto << "};\nnamespace enigma { size_t sound_idmax = " <<max << "; }\n\n";
    
    wto << "enum //room names\n{\n";
    for (int i = 0; i < es->roomCount; i++)
      wto << "  " << es->rooms[i].name << " = " << es->rooms[i].id << ",\n"; 
    wto << "};\n\n";
  wto.close();
  
  idpr("Performing Secondary Parsing and Writing Globals",25);
  
  // Defragged events must be written before object data, or object data cannot determine which events were used.
  edbg << "Writing events" << flushl;
  res = compile_writeDefraggedEvents(es);
  irrr();
  
  parsed_object EGMglobal;
  
  edbg << "Linking globals" << flushl;
  res = link_globals(&EGMglobal,es,scripts);
  irrr();
  
  edbg << "Writing object data" << flushl;
  res = compile_writeObjectData(es,&EGMglobal);
  irrr();
  
  res = compile_writeRoomData(es);
  irrr();
  
  res = compile_writeRoomData(es);
  irrr();
  
  
  
  // Write the global variables to their own file to be included before any of the objects
  res = compile_writeGlobals(es,&EGMglobal);
  irrr();
  
  
  
  /**  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment two: Now that the game has been exported as C++ and raw
    resources, our task is to compile the game itself via GNU Make.
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
  
  idpr("Starting compile (This may take a while...)", 30);
  
  string gflags = "-O3 -s";
  
  #if   TARGET_PLATFORM_ID == OS_WINDOWS
    string glinks = "-lopengl32 '../additional/zlib/libzlib.a' -lcomdlg32 -lgdi32";
    string graphics = "OpenGL";
    string platform = "windows";
  #else
    string glinks = "-lGL -lz -lopenal";
    string graphics = "OpenGL";
    string platform = "xlib";
  #endif
  
  string make = "Game ";
  make += "GMODE=Run ";
  make += "GFLAGS=\"" + gflags   + "\" ";
  make += "GLINKS=\"" + glinks   + "\" ";
  make += "GRAPHICS=" + graphics + " ";
  make += "PLATFORM=" + platform + " ";
  
  edbg << "Running make from `" << MAKE_location << "'" << flushl;
  edbg << "Full command line: " << MAKE_location << " " << make << flushl;
  int makeres = better_system(MAKE_location,make);
  if (makeres) {
    user << "----Make returned error " << makeres << "----------------------------------\n";
    idpr("Compile failed at C++ level.",-1); return E_ERROR_BUILD;
  }
  user << "+++++Make completed successfully.++++++++++++++++++++++++++++++++++++\n";
  
  
  
  
  
  /**  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment three: Add resources into the game executable. They are
    literally tacked on to the end of the file for now. They should
    have an option in the config file to pass them to some resource
    linker sometime in the future.
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
  
  idpr("Adding resources...",90);
  
  FILE *gameModule = fopen("ENIGMAsystem/SHELL/game.exe","ab");
  if (!gameModule) {
    user << "Failed to append resources to the game. Did compile actually succeed?" << flushl;
    idpr("Failed to add resources.",-1); return 12;
  }
  
  fseek(gameModule,0,SEEK_END); //necessary on Windows for no reason.
  int resourceblock_start = ftell(gameModule);
  
  if (resourceblock_start < 128) {
    user << "Compiled game is clearly not a working module; cannot continue" << flushl;
    idpr("Failed to add resources.",-1); return 13;
  }
  
  
  // Start by setting off our location with a DWord of NULLs
  fwrite("\0\0\0",1,4,gameModule);
  
  idpr("Adding Sprites",90);
  
  res = module_write_sprites(es, gameModule);
  irrr();
  
  edbg << "Finalized sprites." << flushl;
  idpr("Adding Sounds",93);
  
  module_write_sounds(es,gameModule);
  
  // Tell where the resources start
  fwrite("\0\0\0\0res0",8,1,gameModule);
  fwrite(&resourceblock_start,4,1,gameModule);
  
  //Close the game module; we're done adding resources
  idpr("Closing game module and running if requested.",99);
  edbg << "Closing game module and running if requested." << flushl;
  fclose(gameModule);
  
  if (mode == emode_run or mode == emode_build or true)
  {
    int gameres = better_system("ENIGMAsystem/SHELL/game.exe","");
    user << "Game returned " << gameres << "\n";
  }
  
  idpr("Done.", 100);
  return 0;
};
