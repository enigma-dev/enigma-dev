/**
  @file  compile.cpp
  @brief Implements the basic structure behind the compilation sequence.
  
  @section License
    Copyright (C) 2011-2014 Josh Ventura
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
#include "OS_Switchboard.h" //Tell us where the hell we are
#include "backend/EnigmaStruct.h" //LateralGM interface structures

#include "general/darray.h"

 #include <cstdio>
 #include <cstring>

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
//#include <dirent.h>
#include <sys/stat.h>
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

dllexport int compileEGMf(EnigmaStruct *es, const char* exe_filename, int mode);
dllexport int compileEGMf(EnigmaStruct *es, const char* exe_filename, int mode) {
  compile_error_handler herr;
  compile_context ctex(es, &herr, (compile_mode)mode);
  return compile(ctex, exe_filename);
}

int compile(compile_context &ctex, const char* exe_filename)
{
  edbg << "Initializing dialog boxes" << flushl;
    ide_dia_clear();
  edbg << "Showing dialog boxes" << flushl;
    ide_dia_open();
  edbg << "Initialized." << flushl;
  
  if (ctex.mode == emode_rebuild) {
    idpr("Rebuilding.", 0);
    return current_language->rebuild();
    idpr("Done.", 100);
  }
  edbg << "Building for mode (" << ctex.mode << ")" << flushl;
  
  string working_directory;
  if (!ctex.es->filename || ctex.mode == emode_compile) {
      working_directory = ".";
  } else {
      working_directory = ctex.es->filename;
      working_directory = working_directory.substr(0, working_directory.find_last_of("/"));
      working_directory = working_directory.substr(working_directory.find("file:/",0) + 6); // FIXME: NEWCOMPILER: This is terribly broken. This breaks non-URIs and doesn't fix URIs.
      working_directory = string_replace_all(working_directory, "/", "\\\\");
      working_directory = string_replace_all(working_directory, "%20", " ");
  }
  
  // Re-establish ourself
  // Read the global locals: locals that will be included with each instance
  edbg << "Loading shared locals from extensions list" << flushl;
  if (current_language->load_shared_locals() != 0) {
    user << "Failed to determine locals; couldn't determine bottom tier: is ENIGMA configured correctly?";
    idpr("ENIGMA Misconfiguration",-1); return E_ERROR_LOAD_LOCALS;
  }

  //Read the types of events
  event_parse_resourcefile();

  // Pick apart the sent resources
  edbg << "Location in memory of structure: " << (void*)ctex.es << flushl;
  if (ctex.es == NULL) {
    idpr("Java ENIGMA plugin dropped its ass.",-1);
    return E_ERROR_PLUGIN_FUCKED_UP;
  }

  edbg << "File version: " << ctex.es->fileVersion << flushl << flushl;
  if (ctex.es->fileVersion != 800)
    edbg << "Incorrect version. File is too " << ((ctex.es->fileVersion > 800)?"new":"old") << " for this compiler. Continuing anyway, because this number is always wrong.";

  /* Segment One: This segment of the compile process is responsible for
   * translating the code into C++. Basically, anything essential to the
   * compilation of said code is dealt with during this segment.
   */

  // The segment begins by adding resource names to the collection of variables that should not be automatically re-scoped.

  // First, we make a space to put our globals.
  jdi::using_scope globals_scope("<ENIGMA Resources>", main_context->get_global());

  idpr("Copying resources",1);

  // Next, add the resource names to that list
  edbg << "Copying resources:" << flushl;

  edbg << "Copying sprite names [" << ctex.es->spriteCount << "]" << flushl;
  for (int i = 0; i < ctex.es->spriteCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int, ctex.es->sprites[i].name);

  edbg << "Copying sound names [" << ctex.es->soundCount << "]" << flushl;
  for (int i = 0; i < ctex.es->soundCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int, ctex.es->sounds[i].name);

  edbg << "Copying background names [" << ctex.es->backgroundCount << "]" << flushl;
  for (int i = 0; i < ctex.es->backgroundCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int, ctex.es->backgrounds[i].name);

  edbg << "Copying path names [" << ctex.es->pathCount << "]" << flushl;
  for (int i = 0; i < ctex.es->pathCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int, ctex.es->paths[i].name);

  edbg << "Copying script names [" << ctex.es->scriptCount << "]" << flushl;
  for (int i = 0; i < ctex.es->scriptCount; i++)
    quickmember_script(main_context, &globals_scope, ctex.es->scripts[i].name);
    
  edbg << "Copying shader names [" << ctex.es->shaderCount << "]" << flushl;
  for (int i = 0; i < ctex.es->shaderCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int, ctex.es->shaders[i].name);

  edbg << "Copying font names [" << ctex.es->fontCount << "]" << flushl;
  for (int i = 0; i < ctex.es->fontCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int, ctex.es->fonts[i].name);

  edbg << "Copying timeline names [kidding, these are totally not implemented :P] [" << ctex.es->timelineCount << "]" << flushl;
  for (int i = 0; i < ctex.es->timelineCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int, ctex.es->timelines[i].name);

  edbg << "Copying object names [" << ctex.es->gmObjectCount << "]" << flushl;
  for (int i = 0; i < ctex.es->gmObjectCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int, ctex.es->gmObjects[i].name);

  edbg << "Copying room names [" << ctex.es->roomCount << "]" << flushl;
  for (int i = 0; i < ctex.es->roomCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int, ctex.es->rooms[i].name);

  /* Next we do a simple parse of the code, scouting for some variable names and adding semicolons.
   */

  idpr("Checking Syntax and performing Preliminary Parsing",2);
  edbg << "SYNTAX CHECKING AND PRIMARY PARSING:" << flushl;
  edbg << ctex.es->scriptCount << " Scripts:" << flushl;

  int res;
  #define irrr() if (res) { idpr("Error occurred; see scrollback for details.",-1); return res; }
  res = compile_parseAndLink(ctex);
  irrr();
  
  ofstream wto; // NEWPARSER: FIXME: Remove this! The compiler abstraction's job is not to write to files.
  idpr("Outputting Resources in Various Places...",10);

  /* FIRST FILE
   * Modes, settings and executable information.
   */
  
  edbg << "Writing executable information and resources." << flushl;
  current_language->compile_write_settings(ctex);


  //NEXT FILE ----------------------------------------
  //Resource names: Defines integer constants for all resources.
  edbg << "Writing resource names and maxima" << flushl;
  current_language->compile_write_resource_names(ctex);
  
  idpr("Performing Secondary Parsing and Writing Globals",25);

  // Defragged events must be written before object data, or object data cannot determine which events were used.
  edbg << "Writing events" << flushl;
  res = current_language->compile_write_defragd_events(ctex);
  irrr();

  /*edbg << "Linking globals and running Secondary Parse Passes" << flushl;
  res = compile_parseSecondary(ctex);
  irrr();*/
  
  edbg << "Writing object data" << flushl;
  res = current_language->compile_write_object_data(ctex);
  irrr();

  edbg << "Writing local accessors" << flushl;
  res = current_language->compile_write_obj_access(ctex);
  irrr();

  edbg << "Writing font data" << flushl;
  res = current_language->compile_write_font_info(ctex);
  irrr();

  edbg << "Writing room data" << flushl;
  res = current_language->compile_write_room_data(ctex);
  irrr();
  
  edbg << "Writing shader data" << flushl;
  res = current_language->compile_write_shader_data(ctex);
  irrr();
  
  // Write the global variables to their own file to be included before any of the objects
  res = current_language->compile_write_globals(ctex);
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



  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment two: Now that the game has been exported as C++ and raw
    resources, our task is to compile the game itself via GNU Make.
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  language_adapter::resource_writer *resw = current_language->compile(ctex, exe_filename);
  if (!resw) {
    idpr(("Compile failed at " + current_language_name + " level.").c_str(),-1);
    return 12;
  }


  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment three: Add resources into the game executable. They are
    literally tacked on to the end of the file for now. They should
    have an option in the config file to pass them to some resource
    linker sometime in the future.
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  
  idpr("Adding resources...",90);
  
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
  if (ctex.mode == emode_run or ctex.mode == emode_debug or ctex.mode == emode_design)
    current_language->run_game(ctex, resw);

  idpr("Done.", 100);
  return 0;
}
