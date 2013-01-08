/**
  @file  compile.cpp
  @brief Implements the basic structure behind the compilation sequence.
  
  @section License
    Copyright (C) 2011-2013 Josh Ventura
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

#include "OS_Switchboard.h" //Tell us where the hell we are
#include "backend/EnigmaStruct.h" //LateralGM interface structures

#include "general/darray.h"

 #include <cstdio>

#ifdef _WIN32
 #define dllexport extern "C" __declspec(dllexport)
 #include <windows.h>
 #define sleep Sleep
#else
 #define dllexport extern "C"
 #include <unistd.h>
 #define sleep(x) usleep(x * 1000)
#endif

#define idpr(x,y) \
  ide_dia_progress_text(x); \
  ide_dia_progress(y);

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "backend/ideprint.h"

using namespace std;

#include "compile.h"

#include "backend/JavaCallbacks.h"
#include "syntax/syncheck.h"
#include "parser/parser.h"
#include "compile_includes.h"
#include "compile_common.h"

#include "settings-parse/crawler.h"
#include "settings-parse/eyaml.h"

#include "components/components.h"
#include "gcc_interface/gcc_backend.h"

#include "general/bettersystem.h"
#include "event_reader/event_parser.h"

#include "languages/lang_CPP.h"

#include "compiler/jdi_utility.h"

#ifdef WRITE_UNIMPLEMENTED_TXT
std::map <string, char> unimplemented_function_list;
#endif

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}
inline void writef(float x, FILE *f) {
  fwrite(&x,4,1,f);
}

#include <general/estring.h>
#include <parser/parser_components.h>

string string_replace_all(string str,string substr,string nstr)
{
  pt pos=0;
  while ((pos=str.find(substr,pos)) != string::npos)
  {
    str.replace(pos,substr.length(),nstr);
    pos+=nstr.length();
  }
  return str;
}

string toUpper(string x) {
  string res = x;
  for (size_t i = 0; i < res.length(); i++)
    res[i] = res[i] >= 'a' and res[i] <= 'z' ? res[i] + 'A' - 'a' : res[i];
  return res;
}

#include "System/builtins.h"

// modes: 0=run, 1=debug, 2=design, 3=compile

dllexport int compileEGMf(EnigmaStruct *es, const char* exe_filename, int mode);
dllexport int compileEGMf(EnigmaStruct *es, const char* exe_filename, int mode) {
  return ::compile(es, exe_filename, mode);
}

int compile(EnigmaStruct *es, const char* exe_filename, int mode)
{
  cout << "Initializing dialog boxes" << endl;
    ide_dia_clear();
    ide_dia_open();
  cout << "Initialized." << endl;

  if (mode == emode_rebuild) {
    idpr("Rebuilding.", 0);
    return current_language->rebuild();
    idpr("Done.", 100);
  }

  edbg << "Building for mode (" << mode << ")" << flushl;
  compile_context ctex; // This baby holds everything about our compile process
   
  // Re-establish ourself
  // Read the global locals: locals that will be included with each instance
  {
    // FIXME: Extensions already read?
    
    /*vector<string> extnp;
    for (int i = 0; i < es->extensionCount; i++) {
      cout << "Adding extension " << flushl << "extension " << flushl << es->extensions[i].path << flushl << ":" << endl << es->extensions[i].name << flushl;
      extnp.push_back(string(es->extensions[i].path) + es->extensions[i].name);
    }*/
    
    edbg << "Loading shared locals from extensions list" << flushl;
    if (current_language->load_shared_locals() != 0) {
      user << "Failed to determine locals; couldn't determine bottom tier: is ENIGMA configured correctly?";
      idpr("ENIGMA Misconfiguration",-1); return E_ERROR_LOAD_LOCALS;
    }
  }

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
    edbg << "Incorrect version. File is too " << ((es->fileVersion > 800)?"new":"old") << " for this compiler. Continuing anyway, because this number is always wrong.";


  /* *\\\  Segment One: This segment of the compile process is responsible for
  *  *||| translating the code into C++. Basically, anything essential to the
  \* *///  compilation of said code is dealt with during this segment.

  ///The segment begins by adding resource names to the collection of variables that should not be automatically re-scoped.


  //First, we make a space to put our globals.
  jdi::using_scope globals_scope("<ENIGMA Resources>", main_context->get_global());

  idpr("Copying resources",1);

  //Next, add the resource names to that list
  edbg << "COPYING SOME F*CKING RESOURCES:" << flushl;

  edbg << "Copying sprite names [" << es->spriteCount << "]" << flushl;
  for (int i = 0; i < es->spriteCount; i++) {
    cout << "Name on this side: " << globals_scope.name << endl;
    cout << "Name on this side2: " << ((jdi::definition_scope*)&globals_scope)->name << endl;
    cout << "Pointer on this side: " << (&globals_scope) << endl;
    cout << "Address on this side: " << ((jdi::definition_scope*)&globals_scope) << endl;
    cout << "Char on this side: " << (*(char**)&globals_scope.name) << endl;
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->sprites[i].name);
  }

  edbg << "Copying sound names [" << es->soundCount << "]" << flushl;
  for (int i = 0; i < es->soundCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->sounds[i].name);

  edbg << "Copying background names [" << es->backgroundCount << "]" << flushl;
  for (int i = 0; i < es->backgroundCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->backgrounds[i].name);

  edbg << "Copying path names [" << es->pathCount << "]" << flushl;
  for (int i = 0; i < es->pathCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->paths[i].name);

  edbg << "Copying script names [" << es->scriptCount << "]" << flushl;
  for (int i = 0; i < es->scriptCount; i++)
    quickmember_script(&globals_scope,es->scripts[i].name);

  edbg << "Copying font names [" << es->fontCount << "]" << flushl;
  for (int i = 0; i < es->fontCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->fonts[i].name);

  edbg << "Copying timeline names [kidding, these are totally not implemented :P] [" << es->timelineCount << "]" << flushl;
  for (int i = 0; i < es->timelineCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->timelines[i].name);

  edbg << "Copying object names [" << es->gmObjectCount << "]" << flushl;
  for (int i = 0; i < es->gmObjectCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->gmObjects[i].name);

  edbg << "Copying room names [" << es->roomCount << "]" << flushl;
  for (int i = 0; i < es->roomCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->rooms[i].name);



  // Next we do a simple parse of the code, scouting for some variable names and adding semicolons.

  idpr("Checking Syntax and performing Preliminary Parsing",2);
  edbg << "SYNTAX CHECKING AND PRIMARY PARSING:" << flushl;
  edbg << es->scriptCount << " Scripts:" << flushl;

  int res;
  #define irrr() if (res) { idpr("Error occurred; see scrollback for details.",-1); return res; }
  res = compile_parseAndLink(ctex);
  irrr();
  
  ofstream wto; // NEWPARSER: FIXME: Remove this! The compiler abstraction's job is not to write to files.
  idpr("Outputting Resources in Various Places...",10);

  // FIRST FILE
  // Modes and settings.

  edbg << "Writing modes and settings" << flushl;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/GAME_SETTINGS.h",ios_base::out);
    wto << lang_CPP::gen_license;
    wto << "#define ASSUMEZERO 0\n";
    wto << "void ABORT_ON_ALL_ERRORS() { " << (false?"game_end();":"") << " }\n";
    wto << '\n';
  wto.close();

  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h",ios_base::out);
    wto << lang_CPP::gen_license;
    wto << "#define BUILDMODE " << 0 << "\n";
    wto << "#define DEBUGMODE " << 0 << "\n";
    wto << '\n';
  wto.close();

  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h",ios_base::out);
  wto.close();


  //NEXT FILE ----------------------------------------
  //Object switch: A listing of all object IDs and the code to allocate them.
  edbg << "Writing object switch" << flushl;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h",ios_base::out);
    wto << lang_CPP::gen_license;
    wto << "#ifndef NEW_OBJ_PREFIX\n#  define NEW_OBJ_PREFIX\n#endif\n\n";
    for (po_i i = ctex.parsed_objects.begin(); i != ctex.parsed_objects.end(); i++)
    {
      wto << "case " << i->second->properties->id << ":\n";
      wto << "    NEW_OBJ_PREFIX new enigma::" << i->second->class_name <<"(x,y,idn);\n";
      wto << "  break;\n";
    }
    wto << "\n\n#undef NEW_OBJ_PREFIX\n";
  wto.close();


  //NEXT FILE ----------------------------------------
  //Resource names: Defines integer constants for all resources.
  edbg << "Writing resource names and maxima" << flushl;
  current_language->compile_write_resource_names(ctex);
  
  idpr("Performing Secondary Parsing and Writing Globals",25);

  // Defragged events must be written before object data, or object data cannot determine which events were used.
  edbg << "Writing events" << flushl;
  res = current_language->compile_writeDefraggedEvents(ctex);
  irrr();

  /*edbg << "Linking globals and running Secondary Parse Passes" << flushl;
  res = compile_parseSecondary(ctex);
  irrr();*/
  
  edbg << "Writing object data" << flushl;
  res = current_language->compile_writeObjectData(ctex);
  irrr();

  edbg << "Writing local accessors" << flushl;
  res = current_language->compile_writeObjAccess(ctex);
  irrr();

  edbg << "Writing font data" << flushl;
  res = current_language->compile_writeFontInfo(ctex);
  irrr();

  edbg << "Writing room data" << flushl;
  res = current_language->compile_writeRoomData(ctex);
  irrr();
  
  // Write the global variables to their own file to be included before any of the objects
  res = current_language->compile_writeGlobals(ctex);
  irrr();


  // Now we write any additional templates requested by the window system.
  // compile_handle_templates(es);

#ifdef WRITE_UNIMPLEMENTED_TXT
    printf("write unimplemented functions %d",0);
    ofstream outputFile;
    outputFile.open("unimplementedfunctionnames.txt");

    for ( std::map< string, char, std::less< char > >::const_iterator iter = unimplemented_function_list.begin();
         iter != unimplemented_function_list.end(); ++iter )
    {
        outputFile << iter->first << '\t' << iter->second << '\n';
    }

    outputFile << endl;
    outputFile.close();
#endif



  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment two: Now that the game has been exported as C++ and raw
    resources, our task is to compile the game itself via GNU Make.
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
   
  language_adapter::resource_writer *resw = current_language->compile(ctex, exe_filename);
  if (resw)
    ;
  else {
    idpr(("Compile failed at " + current_language_name + " level.").c_str(),-1);
    return 12;
  }

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment three: Add resources into the game executable. They are
    literally tacked on to the end of the file for now. They should
    have an option in the config file to pass them to some resource
    linker sometime in the future.
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  
  idpr("Adding Sprites",90);
  res = resw->module_write_sprites(ctex);
  irrr(); idpr("Adding Sounds",92);
  res = resw->module_write_sounds(ctex);
  irrr(); idpr("Adding Backgrounds",94);
  res = resw->module_write_backgrounds(ctex);
  irrr(); idpr("Adding fonts",96);
  res = resw->module_write_fonts(ctex);
  irrr(); idpr("Adding paths",97);
  res = resw->module_write_paths(ctex);
  irrr(); idpr("Adding any extension resources",98);
  res = resw->module_write_extensions(ctex);

  // Close the game module; we're done adding resources
  idpr("Closing game module and running if requested.",99);
  res = resw->module_finalize(ctex);

  // Run the game if requested
  if (mode == emode_run or mode == emode_debug or mode == emode_design)
    current_language->run_game(ctex, resw);

  idpr("Done.", 100);
  return 0;
}
