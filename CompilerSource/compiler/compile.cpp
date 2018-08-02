/** Copyright (C) 2011 Josh Ventura
*** Copyright (C) 2014 Seth N. Hetu
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "makedir.h"
#include "OS_Switchboard.h" //Tell us where the hell we are
#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "settings.h"

#include "general/darray.h"

#include <cstdio>

#if CURRENT_PLATFORM_ID == OS_WINDOWS
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

#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
//#include <dirent.h>
#include <sys/stat.h>
#include "backend/ideprint.h"

using namespace std;

#include "backend/JavaCallbacks.h"
#include "syntax/syncheck.h"
#include "parser/parser.h"
#include "compile_includes.h"
#include "compile_common.h"


#include "settings-parse/crawler.h"

#include "components/components.h"

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

inline string string_replace_all(string str,string substr,string nstr)
{
  pt pos=0;
  while ((pos=str.find(substr,pos)) != string::npos)
  {
    str.replace(pos,substr.length(),nstr);
    pos+=nstr.length();
  }
  return str;
}

inline void write_desktop_entry(const std::string fPath, const GameSettings& gameSet) {
  std::ofstream wto;
  std::string fName = fPath.substr(fPath.find_last_of("/\\") + 1);

  wto.open(fPath + ".desktop");
  wto << "[Desktop Entry]\n";
  wto << "Type=Application\n";
  wto << "Version=" << gameSet.versionMajor << "." << gameSet.versionMinor << "." << gameSet.versionRelease << "." << gameSet.versionBuild << "\n";
  wto << "Name=" << fName << "\n";
  wto << "Comment=" << gameSet.description << "\n";
  wto << "Path=.\n";
  wto << "Exec=./" << fName << "\n";
  //NOTE: Due to security concerns linux doesn't allow releative paths for icons
  // hardcoding icon because relative paths search /usr/share/icons and full paths aren't portable
  wto << "Icon=applications-games-symbolic.svg\n";
  wto << "Terminal=false\n"; //TODO: Add setting for this in ide
  wto << "Categories=Game;\n";
  wto << "MimeType=application/octet-stream;\n";
  wto.close();

  #if CURRENT_PLATFORM_ID != OS_WINDOWS
  chmod((fPath + ".desktop").c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
  #endif
}

inline void write_exe_info(const std::string codegen_directory, const EnigmaStruct *es) {
  std::ofstream wto;
  GameSettings gameSet = es->gameSettings;

  wto.open((codegen_directory + "Preprocessor_Environment_Editable/Resources.rc").c_str(),ios_base::out);
  wto << license;
  wto << "#include <windows.h>\n";
  if (gameSet.gameIcon != NULL && strlen(gameSet.gameIcon) > 0) {
    wto << "IDI_MAIN_ICON ICON          \"" << string_replace_all(gameSet.gameIcon,"\\","/")  << "\"\n";
  }
  wto << "VS_VERSION_INFO VERSIONINFO\n";
  wto << "FILEVERSION " << gameSet.versionMajor << "," << gameSet.versionMinor << "," << gameSet.versionRelease << "," << gameSet.versionBuild << "\n";
  wto << "PRODUCTVERSION " << gameSet.versionMajor << "," << gameSet.versionMinor << "," << gameSet.versionRelease << "," << gameSet.versionBuild << "\n";
  wto << "BEGIN\n" << "BLOCK \"StringFileInfo\"\n" << "BEGIN\n" << "BLOCK \"040904E4\"\n" << "BEGIN\n";
  wto << "VALUE \"CompanyName\",         \"" << gameSet.company << "\"\n";
  wto << "VALUE \"FileDescription\",     \"" << gameSet.description << "\"\n";
  wto << "VALUE \"FileVersion\",         \"" << gameSet.version << "\\0\"\n";
  wto << "VALUE \"ProductName\",         \"" << gameSet.product << "\"\n";
  wto << "VALUE \"ProductVersion\",      \"" << gameSet.version << "\\0\"\n";
  wto << "VALUE \"LegalCopyright\",      \"" << gameSet.copyright << "\"\n";
  if (es->filename != NULL && strlen(es->filename) > 0) {
    wto << "VALUE \"OriginalFilename\",         \"" << string_replace_all(es->filename,"\\","/") << "\"\n";
  } else {
    wto << "VALUE \"OriginalFilename\",         \"\"\n";
  }
  wto << "END\nEND\nBLOCK \"VarFileInfo\"\nBEGIN\n";
  wto << "VALUE \"Translation\", 0x409, 1252\n";
  wto << "END\nEND";
  wto.close();
}

#include "System/builtins.h"

dllexport int compileEGMf(EnigmaStruct *es, const char* exe_filename, int mode) {
  return current_language->compile(es, exe_filename, mode);
}

static bool run_game = true;
dllexport void ide_handles_game_launch() { run_game = false; }

static bool redirect_make = true;
dllexport void log_make_to_console() { redirect_make = false; }

int lang_CPP::compile(EnigmaStruct *es, const char* exe_filename, int mode)
{

  cout << "Initializing dialog boxes" << endl;
  ide_dia_clear();
  ide_dia_open();
  cout << "Initialized." << endl;

  // replace any spaces in ey name because make is trash
  string name = string_replace_all(compilerInfo.name, " ", "_");
  string compilepath = CURRENT_PLATFORM_NAME "/" + compilerInfo.target_platform + "/" + name;

  if (mode == emode_rebuild)
  {
    edbg << "Cleaning..." << flushl;

  	string make = compilerInfo.make_vars["MAKEFLAGS"];
    make += " clean-game ";
  	make += "COMPILEPATH=\"" + compilepath + "\" ";
  	make += "WORKDIR=\"" + eobjs_directory + "\" ";
    make += "CODEGEN=\"" + codegen_directory + "\" ";

  	edbg << "Full command line: " << compilerInfo.MAKE_location << " " << make << flushl;
    e_execs(compilerInfo.MAKE_location,make);

    edbg << "Done.\n" << flushl;
  	idpr("Done.", 100);
  	return 0;
  }
  edbg << "Building for mode (" << mode << ")" << flushl;

  // Clean up from any previous executions.
  edbg << "Cleaning up from previous executions" << flushl;
  parsed_objects.clear(); //Make sure we don't dump in any old object code...
  edbg << " - Cleared parsed objects" << flushl;
  parsed_rooms.clear();   //Or that we dump any room code, for that matter...
  edbg << " - Cleared room entries" << flushl;
  shared_locals_clear();  //Forget inherited locals, we'll reparse them
  edbg << " - Cleared shared locals list" << flushl;
  event_info_clear();     //Forget event definitions, we'll re-get them
  edbg << " - Cleared event info" << flushl;

  // Re-establish ourself
  // Read the global locals: locals that will be included with each instance
  {
    vector<string> extnp;
    for (int i = 0; i < es->extensionCount; i++) {
      cout << "Adding extension " << flushl << "extension " << flushl << es->extensions[i].path << flushl << ":" << endl << es->extensions[i].name << flushl;
      extnp.push_back(string(es->extensions[i].path) + es->extensions[i].name);
    }
    edbg << "Loading shared locals from extensions list" << flushl;
    if (shared_locals_load(extnp) != 0) {
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


  /**** Segment One: This segment of the compile process is responsible for
  * @ * translating the code into C++. Basically, anything essential to the
  *//// compilation of said code is dealt with during this segment.

  ///The segment begins by adding resource names to the collection of variables that should not be automatically re-scoped.


  //First, we make a space to put our globals.
  jdi::using_scope globals_scope("<ENIGMA Resources>", main_context->get_global());

  idpr("Copying resources",1);

  //Next, add the resource names to that list
  edbg << "Copying resources:" << flushl;

  edbg << "Copying sprite names [" << es->spriteCount << "]" << flushl;
  for (int i = 0; i < es->spriteCount; i++) {
    cout << "Name on this side: " << globals_scope.name << endl;
    cout << "Name on this side2: " << ((jdi::definition_scope*)&globals_scope)->name << endl;
    cout << "Pointer on this side: " << (&globals_scope) << endl;
    cout << "Address on this side: " << ((jdi::definition_scope*)&globals_scope) << endl;

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

  edbg << "Copying shader names [" << es->shaderCount << "]" << flushl;
  for (int i = 0; i < es->shaderCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->shaders[i].name);

  edbg << "Copying font names [" << es->fontCount << "]" << flushl;
  for (int i = 0; i < es->fontCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->fonts[i].name);

  edbg << "Copying timeline names [" << es->timelineCount << "]" << flushl;
  for (int i = 0; i < es->timelineCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->timelines[i].name);

  edbg << "Copying object names [" << es->gmObjectCount << "]" << flushl;
  for (int i = 0; i < es->gmObjectCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->gmObjects[i].name);

  edbg << "Copying room names [" << es->roomCount << "]" << flushl;
  for (int i = 0; i < es->roomCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->rooms[i].name);

  edbg << "Copying constant names [" << es->constantCount << "]" << flushl;
  for (int i = 0; i < es->constantCount; i++)
    quickmember_variable(&globals_scope,jdi::builtin_type__int,es->constants[i].name);


  /// Next we do a simple parse of the code, scouting for some variable names and adding semicolons.

  idpr("Checking Syntax and performing Preliminary Parsing",2);

  edbg << "SYNTAX CHECKING AND PRIMARY PARSING:" << flushl;

  edbg << es->scriptCount << " Scripts:" << flushl;
  parsed_script *parsed_scripts[es->scriptCount];

  //parsed timelines involve N timelines with M moments each. So we just store them in a large vector rather than a messy 2-D array of pointers.
  vector<parsed_script*> parsed_tlines;

  scr_lookup.clear();
  used_funcs::zero();

  int res;
  #define irrr() if (res) { idpr("Error occurred; see scrollback for details.",-1); return res; }

  //The parser (and, to some extent, the compiler) needs knowledge of script names for various optimizations.
  std::set<std::string> script_names;
  for (int i = 0; i < es->scriptCount; i++)
    script_names.insert(es->scripts[i].name);

  res = current_language->compile_parseAndLink(es,parsed_scripts, parsed_tlines, script_names);
  irrr();


  //Export resources to each file.

  ofstream wto;
  idpr("Outputting Resources in Various Places...",10);

  // FIRST FILE
  // Modes, settings and executable information.

  idpr("Adding resources...",90);
  string desstr = "./ENIGMAsystem/SHELL/design_game" + compilerInfo.exe_vars["BUILD-EXTENSION"];
  string gameFname = mode == emode_design ? desstr.c_str() : (desstr = exe_filename, exe_filename); // We will be using this first to write, then to run

  edbg << "Writing executable information and resources." << flushl;
  if (compilerInfo.target_platform == "Windows")
    write_exe_info(codegen_directory, es);
  else if (compilerInfo.target_platform == "Linux" && mode == emode_compile)
    write_desktop_entry(gameFname, es->gameSettings);

  edbg << "Writing modes and settings" << flushl;
  wto.open((codegen_directory + "Preprocessor_Environment_Editable/GAME_SETTINGS.h").c_str(),ios_base::out);
  wto << license;
  wto << "#define ASSUMEZERO 0\n";
  wto << "#define PRIMBUFFER 0\n";
  wto << "#define PRIMDEPTH2 6\n";
  wto << "#define AUTOLOCALS 0\n";
  wto << "#define MODE3DVARS 0\n";
  wto << "#define GM_COMPATIBILITY_VERSION " << setting::compliance_mode << "\n";
  wto << "void ABORT_ON_ALL_ERRORS() { " << (false?"game_end();":"") << " }\n";
  wto << '\n';
  wto.close();

  wto.open((codegen_directory + "Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h").c_str(),ios_base::out);
  wto << license;
  wto << "#define BUILDMODE " << 0 << "\n";
  wto << "#define DEBUGMODE " << 0 << "\n";
  wto << '\n';
  wto.close();

  wto.open((codegen_directory + "Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h").c_str(),ios_base::out);
  wto.close();

  //NEXT FILE ----------------------------------------
  //Object switch: A listing of all object IDs and the code to allocate them.
  edbg << "Writing object switch" << flushl;
  wto.open((codegen_directory + "Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h").c_str(),ios_base::out);
    wto << license;
    wto << "#ifndef NEW_OBJ_PREFIX\n#  define NEW_OBJ_PREFIX\n#endif\n\n";
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    {
      wto << "case " << i->second->id << ":\n";
      wto << "    NEW_OBJ_PREFIX new enigma::OBJ_" << i->second->name <<"(x,y,idn);\n";
      wto << "  break;\n";
    }
    wto << "\n\n#undef NEW_OBJ_PREFIX\n";
  wto.close();


  //NEXT FILE ----------------------------------------
  //Resource names: Defines integer constants for all resources.
  int max;
  edbg << "Writing resource names and maxima" << flushl;
  wto.open((codegen_directory + "Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h").c_str(),ios_base::out);
  wto << license;

  wto << "namespace enigma_user {\n";
  wto << "const char *resource_file_path=\"" << compilerInfo.exe_vars["RESOURCES"] << "\";\n";
  wto << "}\n";

  stringstream ss;

    max = 0;
    wto << "namespace enigma_user {\nenum //object names\n{\n";
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++) {
      if (i->first >= max) max = i->first + 1;
      wto << "  " << i->second->name << " = " << i->first << ",\n";
      ss << "    case " << i->first << ": return \"" << i->second->name << "\"; break;\n";
    } wto << "};\n}\nnamespace enigma { size_t object_idmax = " << max << "; }\n\n";

    wto << "namespace enigma_user {\nstring object_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //sprite names\n{\n";
    for (int i = 0; i < es->spriteCount; i++) {
      if (es->sprites[i].id >= max) max = es->sprites[i].id + 1;
      wto << "  " << es->sprites[i].name << " = " << es->sprites[i].id << ",\n";
      ss << "    case " << es->sprites[i].id << ": return \"" << es->sprites[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t sprite_idmax = " << max << "; }\n\n";

     wto << "namespace enigma_user {\nstring sprite_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //background names\n{\n";
    for (int i = 0; i < es->backgroundCount; i++) {
      if (es->backgrounds[i].id >= max) max = es->backgrounds[i].id + 1;
      wto << "  " << es->backgrounds[i].name << " = " << es->backgrounds[i].id << ",\n";
      ss << "    case " << es->backgrounds[i].id << ": return \"" << es->backgrounds[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t background_idmax = " << max << "; }\n\n";

     wto << "namespace enigma_user {\nstring background_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //font names\n{\n";
    for (int i = 0; i < es->fontCount; i++) {
      if (es->fonts[i].id >= max) max = es->fonts[i].id + 1;
      wto << "  " << es->fonts[i].name << " = " << es->fonts[i].id << ",\n";
      ss << "    case " << es->fonts[i].id << ": return \"" << es->fonts[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t font_idmax = " << max << "; }\n\n";

     wto << "namespace enigma_user {\nstring font_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
	wto << "namespace enigma_user {\nenum //timeline names\n{\n";
	for (int i = 0; i < es->timelineCount; i++) {
	    if (es->timelines[i].id >= max) max = es->timelines[i].id + 1;
        wto << "  " << es->timelines[i].name << " = " << es->timelines[i].id << ",\n";
        ss << "    case " << es->timelines[i].id << ": return \"" << es->timelines[i].name << "\"; break;\n";
	} wto << "};}\nnamespace enigma { size_t timeline_idmax = " << max << "; }\n\n";

wto << "namespace enigma_user {\nstring timeline_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
	wto << "namespace enigma_user {\nenum //path names\n{\n";
	for (int i = 0; i < es->pathCount; i++) {
	    if (es->paths[i].id >= max) max = es->paths[i].id + 1;
        wto << "  " << es->paths[i].name << " = " << es->paths[i].id << ",\n";
        ss << "    case " << es->paths[i].id << ": return \"" << es->paths[i].name << "\"; break;\n";
	} wto << "};}\nnamespace enigma { size_t path_idmax = " << max << "; }\n\n";

wto << "namespace enigma_user {\nstring path_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //sound names\n{\n";
    for (int i = 0; i < es->soundCount; i++) {
      if (es->sounds[i].id >= max) max = es->sounds[i].id + 1;
      wto << "  " << es->sounds[i].name << " = " << es->sounds[i].id << ",\n";
      ss << "    case " << es->sounds[i].id << ": return \"" << es->sounds[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t sound_idmax = " <<max << "; }\n\n";

wto << "namespace enigma_user {\nstring sound_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //script names\n{\n";
    for (int i = 0; i < es->scriptCount; i++) {
      if (es->scripts[i].id >= max) max = es->scripts[i].id + 1;
      wto << "  " << es->scripts[i].name << " = " << es->scripts[i].id << ",\n";
      ss << "    case " << es->scripts[i].id << ": return \"" << es->scripts[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t script_idmax = " <<max << "; }\n\n";

wto << "namespace enigma_user {\nstring script_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //shader names\n{\n";
    for (int i = 0; i < es->shaderCount; i++) {
      if (es->shaders[i].id >= max) max = es->shaders[i].id + 1;
      wto << "  " << es->shaders[i].name << " = " << es->shaders[i].id << ",\n";
      ss << "    case " << es->shaders[i].id << ": return \"" << es->shaders[i].name << "\"; break;\n";
    } wto << "};}\nnamespace enigma { size_t shader_idmax = " <<max << "; }\n\n";

wto << "namespace enigma_user {\nstring shader_get_name(int i) {\n switch (i) {\n";
     wto << ss.str() << " default: return \"<undefined>\";}};}\n\n";
     ss.str( "" );

    max = 0;
    wto << "namespace enigma_user {\nenum //room names\n{\n";
    for (int i = 0; i < es->roomCount; i++) {
      if (es->rooms[i].id >= max) max = es->rooms[i].id + 1;
      wto << "  " << es->rooms[i].name << " = " << es->rooms[i].id << ",\n";
    }
    wto << "};}\nnamespace enigma { size_t room_idmax = " <<max << "; }\n\n";
  wto.close();


  //NEXT FILE ----------------------------------------
  //Timelines: Defines "moment" lookup structures for timelines.
  edbg << "Writing timeline control information" << flushl;
  wto.open((codegen_directory + "Preprocessor_Environment_Editable/IDE_EDIT_timelines.h").c_str(),ios_base::out);
  {
    wto << license;
    wto <<"namespace enigma {\n\n";

    //Each timeline has a lookup structure (in this case, a map) which allows easy forward/backward lookup.
    //This is currently constructed rather manually; there are probably more efficient
    // construction techniques, but none come to mind.
    wto <<"void timeline_system_initialize() {\n";
    wto <<"  std::vector< std::map<int, int> >& res = object_timelines::timeline_moments_maps;\n";
    wto <<"  res.reserve(" <<es->timelineCount <<");\n";
    wto <<"  std::map<int, int> curr;\n\n";
    for (int i=0; i<es->timelineCount; i++) {
      wto <<"  curr.clear();\n";
      for (int j=0; j<es->timelines[i].momentCount; j++) {
        wto <<"  curr[" <<es->timelines[i].moments[j].stepNo <<"] = " <<j <<";\n";
      }
      wto <<"  res.push_back(curr);\n\n";
    }
    wto <<"}\n\n";

    wto <<"}\n"; //namespace
  }
  wto.close();


  idpr("Performing Secondary Parsing and Writing Globals",25);

  parsed_object EGMglobal;

  edbg << "Linking globals and ambiguous variables" << flushl;
  res = current_language->link_globals(&EGMglobal,es,parsed_scripts, parsed_tlines);
  res = current_language->link_ambiguous(&EGMglobal,es,parsed_scripts, parsed_tlines);
  irrr();

  edbg << "Running Secondary Parse Passes" << flushl;
  res = current_language->compile_parseSecondary(parsed_objects,parsed_scripts,es->scriptCount, parsed_tlines, parsed_rooms,&EGMglobal, script_names);

  edbg << "Writing events" << flushl;
  res = current_language->compile_writeDefraggedEvents(es);
  irrr();

  edbg << "Writing object data" << flushl;
  res = current_language->compile_writeObjectData(es,&EGMglobal,mode);
  irrr();

  edbg << "Writing local accessors" << flushl;
  res = current_language->compile_writeObjAccess(parsed_objects, &EGMglobal, es->gameSettings.treatUninitializedAs0);
  irrr();

  edbg << "Writing font data" << flushl;
  res = current_language->compile_writeFontInfo(es);
  irrr();

  edbg << "Writing room data" << flushl;
  res = current_language->compile_writeRoomData(es,&EGMglobal,mode);
  irrr();

  edbg << "Writing shader data" << flushl;
  res = current_language->compile_writeShaderData(es,&EGMglobal);
  irrr();


  // Write the global variables to their own file to be included before any of the objects
  res = current_language->compile_writeGlobals(es,&EGMglobal);
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



  /**  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment two: Now that the game has been exported as C++ and raw
    resources, our task is to compile the game itself via GNU Make.
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

  idpr("Starting compile (This may take a while...)", 30);

  string make = compilerInfo.make_vars["MAKEFLAGS"];

  make += "Game ";
  make += "WORKDIR=\"" + eobjs_directory + "\" ";
  make += "CODEGEN=\"" + codegen_directory + "\" ";
  make += mode == emode_debug? "GMODE=\"Debug\"" : mode == emode_design? "GMODE=\"Design\"" : mode == emode_compile?"GMODE=\"Compile\"" : "GMODE=\"Run\"";
  make += " ";
  make += "GRAPHICS=\"" + extensions::targetAPI.graphicsSys + "\" ";
  make += "AUDIO=\"" + extensions::targetAPI.audioSys + "\" ";
  make += "COLLISION=\"" + extensions::targetAPI.collisionSys + "\" ";
  make += "WIDGETS=\""  + extensions::targetAPI.widgetSys + "\" ";
  make += "NETWORKING=\""  + extensions::targetAPI.networkSys + "\" ";
  make += "PLATFORM=\"" + extensions::targetAPI.windowSys + "\" ";
  make += "TARGET-PLATFORM=\"" + compilerInfo.target_platform + "\" ";

  for (const auto& key : compilerInfo.make_vars) {
    if (key.second != "")
      make += key.first + "=\"" + key.second + "\" ";
  }

  make += "COMPILEPATH=\"" + compilepath + "\" ";

  string extstr = "EXTENSIONS=\"";
  for (unsigned i = 0; i < parsed_extensions.size(); i++)
  	extstr += " " + parsed_extensions[i].pathname;
  make += extstr + "\"";

  string mfgfn = gameFname;
  for (size_t i = 0; i < mfgfn.length(); i++)
    if (mfgfn[i] == '\\') mfgfn[i] = '/';
  make += string(" OUTPUTNAME=\"") + mfgfn + "\" ";

  edbg << "Running make from `" << compilerInfo.MAKE_location << "'" << flushl;
  edbg << "Full command line: " << compilerInfo.MAKE_location << " " << make << flushl;

//  #if CURRENT_PLATFORM_ID == OS_MACOSX
//  int makeres = better_system("cd ","/MacOS/");
//  int makeres = better_system(MAKE_location,"MacOS");

  string flags = "";

  if (redirect_make) {

    std::string dirs = "CODEGEN=" + codegen_directory + " ";
    dirs += "WORKDIR=" + eobjs_directory + " ";
    e_execs("make", dirs, "required-directories");

    // Pick a file and flush it
    const string redirfile = (eobjs_directory + "enigma_compile.log");
    fclose(fopen(redirfile.c_str(),"wb"));

    // Redirect it
    ide_output_redirect_file(redirfile.c_str()); //TODO: If you pass this function the address it will screw up the value; most likely a JNA/Plugin bug.

    flags += "&> \"" + redirfile + "\"";
  }

  int makeres = e_execs(compilerInfo.MAKE_location, make, flags);

  // Stop redirecting GCC output
  if (redirect_make)
    ide_output_redirect_reset();

  if (makeres) {
    idpr("Compile failed at C++ level.",-1);
    return E_ERROR_BUILD;
  }
  user << "******** Make Completed Successfully ******** \n";

  /**  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment three: Add resources into the game executable. They are
    literally tacked on to the end of the file for now. They should
    have an option in the config file to pass them to some resource
    linker sometime in the future.
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

  #ifdef OS_ANDROID
    "Platforms/Android/EnigmaAndroidGame/libs/armeabi/libndkEnigmaGame.so";
  #endif

  FILE *gameModule;
  int resourceblock_start = 0;
  std::string resfile = compilerInfo.exe_vars["RESOURCES"];
  cout << "`" << resfile << "` == '$exe': " << (resfile == "$exe"?"true":"FALSE") << endl;
  if (resfile == "$exe")
  {
    gameModule = fopen(gameFname.c_str(),"ab");
    if (!gameModule) {
      user << "Failed to append resources to the game. Did compile actually succeed?" << flushl;
      idpr("Failed to add resources.",-1); return 12;
    }

    fseek(gameModule,0,SEEK_END); //necessary on Windows for no reason.
    resourceblock_start = ftell(gameModule);

    if (resourceblock_start < 128) {
      user << "Compiled game is clearly not a working module; cannot continue" << flushl;
      idpr("Failed to add resources.",-1); return 13;
    }
  }
  else
  {
    string resname = resfile;
    for (size_t p = resname.find("$exe"); p != string::npos; p = resname.find("$game"))
      resname.replace(p,4,gameFname);
    gameModule = fopen(resname.c_str(),"wb");
    if (!gameModule) {
      user << "Failed to write resources to compiler-specified file, `" << resname << "`. Write permissions to valid path?" << flushl;
      idpr("Failed to write resources.",-1); return 12;
    }
  }

  // Start by setting off our location with a DWord of NULLs
  fwrite("\0\0\0",1,4,gameModule);

  idpr("Adding Sprites",90);

  res = current_language->module_write_sprites(es, gameModule);
  irrr();

  edbg << "Finalized sprites." << flushl;
  idpr("Adding Sounds",93);

  current_language->module_write_sounds(es,gameModule);

  current_language->module_write_backgrounds(es,gameModule);

  current_language->module_write_fonts(es,gameModule);

  current_language->module_write_paths(es,gameModule);

  // Tell where the resources start
  fwrite("\0\0\0\0res0",8,1,gameModule);
  fwrite(&resourceblock_start,4,1,gameModule);

  // Close the game module; we're done adding resources
  idpr("Closing game module and running if requested.",99);
  edbg << "Closing game module and running if requested." << flushl;
  fclose(gameModule);

  // Run the game if requested
  if (run_game && (mode == emode_run or mode == emode_debug or mode == emode_design))
  {
    // The games working directory, in run/debug it is the GMK/GMX location where the IDE is working with the project,
    // in compile mode it is the same as program_directory, or where the (*.exe executable) is located.
    // The working_directory global is set in the main() of each platform using the platform specific function.
    // This the exact behaviour of GM8.1
    std::vector<char> prevdir(size_t(4096));
    string newdir = (es->filename != NULL && strlen(es->filename) > 0) ? string(es->filename) : string( exe_filename );
    #if CURRENT_PLATFORM_ID == OS_WINDOWS
      if (newdir[0] == '/' || newdir[0] == '\\') {
        newdir = newdir.substr(1, newdir.size());
      }
    #endif
    newdir = newdir.substr( 0, newdir.find_last_of( "\\/" ));

    #if CURRENT_PLATFORM_ID == OS_WINDOWS
    GetCurrentDirectory( 4096, prevdir.data() );
    SetCurrentDirectory(newdir.c_str());
    #else
    getcwd (prevdir.data(), 4096);
    chdir(newdir.c_str());
    #endif

    string rprog = compilerInfo.exe_vars["RUN-PROGRAM"], rparam = compilerInfo.exe_vars["RUN-PARAMS"];
    rprog = string_replace_all(rprog,"$game",gameFname);
    rparam = string_replace_all(rparam,"$game",gameFname);
    user << "Running \"" << rprog << "\" " << rparam << flushl;
    int gameres = e_execs(rprog, rparam);
    user << "\n\nGame returned " << gameres << "\n";

    // Restore the compilers original working directory.
    #if CURRENT_PLATFORM_ID == OS_WINDOWS
    SetCurrentDirectory(prevdir.data());
    #else
    chdir(prevdir.data());
    #endif
  }

  idpr("Done.", 100);
  return 0;
}
