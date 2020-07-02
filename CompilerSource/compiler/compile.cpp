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

#include "strings_util.h"
#include "OS_Switchboard.h" //Tell us where the hell we are
#include "backend/GameData.h"
#include "settings.h"
#include "darray.h"
#include "treenode.pb.h"

#include <cstdio>

#if CURRENT_PLATFORM_ID == OS_WINDOWS
 #define DLLEXPORT extern "C" __declspec(dllexport)
 #include <windows.h>
 #define sleep Sleep
#else
 #define DLLEXPORT extern "C"
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

#ifdef WRITE_UNIMPLEMENTED_TXT
std::map <string, char> unimplemented_function_list;
#endif

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}
inline void writef(float x, FILE *f) {
  fwrite(&x,4,1,f);
}

inline void write_desktop_entry(const std::filesystem::path& fname, const GameData& game) {
  std::ofstream wto;
  const buffers::resources::General &gameSet = game.settings.general();

  wto.open(fname/".desktop");
  wto << "[Desktop Entry]\n";
  wto << "Type=Application\n";
  wto << "Version="
        << gameSet.version_major()   << "."
        << gameSet.version_minor()   << "."
        << gameSet.version_release() << "."
        << gameSet.version_build()   << "\n";
  wto << "Name=" << fname.u8string() << "\n";
  wto << "Comment=" << gameSet.description() << "\n";
  wto << "Path=.\n";
  wto << "Exec=./" << fname.u8string() << "\n";
  //NOTE: Due to security concerns linux doesn't allow releative paths for icons
  // hardcoding icon because relative paths search /usr/share/icons and full paths aren't portable
  wto << "Icon=applications-games-symbolic.svg\n";
  wto << "Terminal=false\n"; //TODO: Add setting for this in ide
  wto << "Categories=Game;\n";
  wto << "MimeType=application/octet-stream;\n";
  wto.close();

  #if CURRENT_PLATFORM_ID != OS_WINDOWS
  chmod((fname/".desktop").u8string().c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
  #endif
}

inline void write_exe_info(const std::filesystem::path& codegen_directory, const GameData &game) {
  std::ofstream wto;
  const buffers::resources::General &gameSet = game.settings.general();
  const string &gloss_version = game.settings.info().version();

  wto.open((codegen_directory/"Preprocessor_Environment_Editable/Resources.rc").u8string().c_str(),ios_base::out);
  wto << license;
  wto << "#include <windows.h>\n";
  if (!gameSet.game_icon().empty()) {
    wto << "IDI_MAIN_ICON ICON          \""
        << string_replace_all(gameSet.game_icon(),"\\","/")  << "\"\n";
  }
  wto << "VS_VERSION_INFO VERSIONINFO\n";
  for (const char *v : vector<const char*>{"FILEVERSION ", "PRODUCTVERSION "}) {
    wto << v
        << gameSet.version_major() << ","
        << gameSet.version_minor() << ","
        << gameSet.version_release() << ","
        << gameSet.version_build() << "\n";
  }
  wto << "BEGIN\n" << "BLOCK \"StringFileInfo\"\n"
      << "BEGIN\n" << "BLOCK \"040904E4\"\n" << "BEGIN\n"
      << "VALUE \"CompanyName\",         \"" << gameSet.company() << "\"\n"
      << "VALUE \"FileDescription\",     \"" << gameSet.description() << "\"\n"
      << "VALUE \"FileVersion\",         \"" << gloss_version << "\\0\"\n"
      << "VALUE \"ProductName\",         \"" << gameSet.product() << "\"\n"
      << "VALUE \"ProductVersion\",      \"" << gloss_version << "\\0\"\n"
      << "VALUE \"LegalCopyright\",      \"" << gameSet.copyright() << "\"\n"
      << "VALUE \"OriginalFilename\",    \"" << string_replace_all(game.filename,"\\","/") << "\"\n"
      << "END\nEND\nBLOCK \"VarFileInfo\"\nBEGIN\n"
      << "VALUE \"Translation\", 0x409, 1252\n"
      << "END\nEND";
  wto.close();
}

#include "System/builtins.h"

DLLEXPORT int compileEGMf(deprecated::JavaStruct::EnigmaStruct *es, const char* exe_filename, int mode) {
  return current_language->compile(GameData(es), exe_filename, mode);
}

DLLEXPORT int compileProto(const buffers::Project *proj, const char* exe_filename, int mode) {
  GameData gameData(*proj);
  int error = FlattenProto(*proj, &gameData);
  if (error) return error;
  return current_language->compile(gameData, exe_filename, mode);
}

static bool run_game = true;
DLLEXPORT void ide_handles_game_launch() { run_game = false; }

static bool redirect_make = true;
DLLEXPORT void log_make_to_console() { redirect_make = false; }

template<typename T> void write_resource_meta(ofstream &wto, const char *kind, vector<T> resources, bool gen_names = true) {
  int max = 0;
  stringstream swb;  // switch body
  wto << "namespace enigma_user {\n"
         "  enum {  // " << kind << " names\n\n";
  for (const T &res : resources) {
    if (res.id() >= max) max = res.id() + 1;
    wto << "    " << res.name << " = " << res.id() << ",\n";
    swb << "      case " << res.id() << ": return \""  << res.name << "\";\n";
  }
  wto << "  };\n\n";
  if (gen_names) {
    wto << "  string " << kind << "_get_name(int i) {\n"
           "    switch (i) {\n";
    wto << swb.str() << "      default: return \"<undefined>\";\n";
    wto << "    }\n"
           "  }\n";
  }
  wto << "}\n";
  wto << "namespace enigma { size_t " << kind << "_idmax = " << max << "; }\n\n";
}   
 
void wite_asset_enum(const std::filesystem::path& fName) {
  std::ofstream wto;
  wto.open(fName.u8string().c_str());
  
  wto<< "#ifndef ASSET_ENUM_H\n#define ASSET_ENUM_H\n\n";
  
  wto << "namespace enigma_user {\n\nenum AssetType : int {\n";
  
  // "unknown" / "any" need to added manually
  wto << "  asset_any = -2,\n";
  wto << "  asset_unknown = -1,\n";
  
  buffers::TreeNode tn;
  google::protobuf::Message *m = &tn;
  const google::protobuf::Descriptor *desc = m->GetDescriptor();
  for (int i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor *field = desc->field(i);
    if (field->containing_oneof() && field->cpp_type() == google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE)
      // NOTE: -1 because protobutt doesn't allow index 0 and we're trying to match GM's values 
      wto << "  asset_" << field->name() << " = " << field->number()-1 <<  "," << std::endl;
  }
  
  // This one doesn't really exist but added for GM compatibility
  wto << "  asset_tileset = asset_background\n";
  wto << "};\n\n}\n\n";
  
  wto << "namespace enigma {\n\nstatic const enigma_user::AssetType assetTypes[] = {\n";
  
  for (int i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor *field = desc->field(i);
    if (field->containing_oneof() && field->cpp_type() == google::protobuf::FieldDescriptor::CppType::CPPTYPE_MESSAGE)
      wto << "  enigma_user::asset_" << field->name() << "," << std::endl;
  }
  
  wto << "};\n\n}\n\n#endif\n";
  
  wto.close();
}
    
template<typename T> void write_asset_map(std::string& str, vector<T> resources, const std::string& type) {
  str += "\n{ enigma_user::" + type + ",\n  {\n";
  for (const T &res : resources) {
    str += "    { \"" + res.name  + "\", " + std::to_string(res.id()) + " },\n";
  }
  
  if (resources.size() > 0) { str.pop_back(); str.back() = '\n'; }
  
  str += "  }\n},\n";
}

static bool ends_with(std::string const &fullString, std::string const &ending) {
    if (fullString.length() < ending.length())
      return false;

    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
}

int lang_CPP::compile(const GameData &game, const char* exe_filename, int mode) {
  std::filesystem::path exename;
  if (exe_filename) {
    exename = exe_filename;
    const std::filesystem::path buildext = compilerInfo.exe_vars["BUILD-EXTENSION"];
    if (!ends_with(exename.u8string(), buildext.u8string())) {
      exename += buildext;
      exe_filename = exename.u8string().c_str();
    }
  }

  cout << "Initializing dialog boxes" << endl;
  // reset this as IDE will soon enable stop button
  build_stopping = false;
  ide_dia_clear(); // <- stop button usually enabled IDE side
  ide_dia_open();
  cout << "Initialized." << endl;

  CompileState state;

  // replace any spaces in ey name because make is trash
  string name = string_replace_all(compilerInfo.name, " ", "_");
  std::filesystem::path compilepath = std::filesystem::path(CURRENT_PLATFORM_NAME)/compilerInfo.target_platform/name;

  if (mode == emode_rebuild)
  {
    edbg << "Cleaning..." << flushl;

  	string make = compilerInfo.make_vars["MAKEFLAGS"];
    make += " -C \"" + unixfy_path(enigma_root) + "\"";
    make += " clean-game ";
  	make += "COMPILEPATH=\"" + unixfy_path(compilepath) + "\" ";
  	make += "WORKDIR=\"" + unixfy_path(eobjs_directory) + "\" ";
    make += "CODEGEN=\"" + unixfy_path(codegen_directory) + "\" ";

  	edbg << "Full command line: " << compilerInfo.MAKE_location << " " << make << flushl;
    e_execs(compilerInfo.MAKE_location,make);

    edbg << "Done.\n" << flushl;
  	idpr("Done.", 100);
  	return 0;
  }
  edbg << "Building for mode (" << mode << ")" << flushl;

  // Clean up from any previous executions.
  edbg << "Cleaning up from previous executions" << flushl;
  event_info_clear();     //Forget event definitions, we'll re-get them
  edbg << " - Cleared event info." << flushl;

  // Re-establish ourself
  // Read the global locals: locals that will be included with each instance
  {
    set<string> exts_new_parse, exts_last_parse;
    for (const auto &ext : game.extensions) {
      exts_new_parse.insert(ext.path + "/" + ext.name);
    }
    for (const string &ext : requested_extensions_last_parse) {
      exts_last_parse.insert(ext);
    }
    edbg << "Loading shared locals from extensions list" << flushl;
    if (exts_new_parse != exts_last_parse) {
      user << "The IDE didn't tell ENIGMA what extensions "
              "were selected before requesting a build.";

      cout << "Extensions I have loaded:" << endl;
      for (const string &e : exts_last_parse) cout << "- " << e << endl;
      cout << "Extensions I should have loaded:" << endl;
      for (const string &e : exts_new_parse) cout << "- " << e << endl;

      //idpr("ENIGMA Misconfiguration",-1); return E_ERROR_LOAD_LOCALS;
      user << "...Continuing anyway..." << flushl;
    }
  }

  //Read the types of events
  event_parse_resourcefile();

  /**** Segment One: This segment of the compile process is responsible for
  * @ * translating the code into C++. Basically, anything essential to the
  *//// compilation of said code is dealt with during this segment.

  // The segment begins by adding resource names to the collection of variables
  // that should not be automatically re-scoped.


  // First, we make a space to put our globals.
  jdi::using_scope globals_scope("<ENIGMA Resources>", namespace_enigma_user);

  idpr("Copying resources",1);

  //Next, add the resource names to that list
  edbg << "Copying resources:" << flushl;

  edbg << "Copying sprite names [" << game.sprites.size() << "]" << flushl;
  for (size_t i = 0; i < game.sprites.size(); i++)
    current_language->quickmember_integer(&globals_scope, game.sprites[i].name);

  edbg << "Copying sound names [" << game.sounds.size() << "]" << flushl;
  for (size_t i = 0; i < game.sounds.size(); i++)
    current_language->quickmember_integer(&globals_scope, game.sounds[i].name);

  edbg << "Copying background names [" << game.backgrounds.size() << "]" << flushl;
  for (size_t i = 0; i < game.backgrounds.size(); i++)
    current_language->quickmember_integer(&globals_scope, game.backgrounds[i].name);

  edbg << "Copying path names [" << game.paths.size() << "]" << flushl;
  for (size_t i = 0; i < game.paths.size(); i++)
    current_language->quickmember_integer(&globals_scope, game.paths[i].name);

  edbg << "Copying script names [" << game.scripts.size() << "]" << flushl;
  for (size_t i = 0; i < game.scripts.size(); i++)
    current_language->quickmember_script(&globals_scope,game.scripts[i].name);

  edbg << "Copying shader names [" << game.shaders.size() << "]" << flushl;
  for (size_t i = 0; i < game.shaders.size(); i++)
    current_language->quickmember_integer(&globals_scope, game.shaders[i].name);

  edbg << "Copying font names [" << game.fonts.size() << "]" << flushl;
  for (size_t i = 0; i < game.fonts.size(); i++)
    current_language->quickmember_integer(&globals_scope, game.fonts[i].name);

  edbg << "Copying timeline names [" << game.timelines.size() << "]" << flushl;
  for (size_t i = 0; i < game.timelines.size(); i++)
    current_language->quickmember_integer(&globals_scope, game.timelines[i].name);

  edbg << "Copying object names [" << game.objects.size() << "]" << flushl;
  for (size_t i = 0; i < game.objects.size(); i++)
    current_language->quickmember_integer(&globals_scope, game.objects[i].name);

  edbg << "Copying room names [" << game.rooms.size() << "]" << flushl;
  for (size_t i = 0; i < game.rooms.size(); i++)
    current_language->quickmember_integer(&globals_scope, game.rooms[i].name);

  edbg << "Copying constant names [" << game.constants.size() << "]" << flushl;
  for (size_t i = 0; i < game.constants.size(); i++)
    current_language->quickmember_integer(&globals_scope, game.constants[i].name);


  /// Next we do a simple parse of the code, scouting for some variable names and adding semicolons.

  idpr("Checking Syntax and performing Preliminary Parsing",2);

  edbg << "SYNTAX CHECKING AND PRIMARY PARSING:" << flushl;

  edbg << game.scripts.size() << " Scripts:" << flushl;

  used_funcs::zero();

  int res;
  #define irrr() if (res) { idpr("Error occurred; see scrollback for details.",-1); return res; }

  //The parser (and, to some extent, the compiler) needs knowledge of script names for various optimizations.
  std::set<std::string> script_names;
  for (size_t i = 0; i < game.scripts.size(); i++)
    script_names.insert(game.scripts[i].name);

  res = current_language->compile_parseAndLink(game, state);
  irrr();


  //Export resources to each file.

  ofstream wto;
  idpr("Outputting Resources in Various Places...",10);

  // FIRST FILE
  // Modes, settings and executable information.

  idpr("Adding resources...",90);
  std::filesystem::path desstr = "./ENIGMAsystem/SHELL/design_game" + compilerInfo.exe_vars["BUILD-EXTENSION"];
  std::filesystem::path gameFname = mode == emode_design ? desstr.u8string().c_str() : (desstr = exe_filename, exe_filename); // We will be using this first to write, then to run

  edbg << "Writing executable information and resources." << flushl;
  if (compilerInfo.target_platform == "Windows")
    write_exe_info(codegen_directory, game);
  else if (compilerInfo.target_platform == "Linux" && mode == emode_compile)
    write_desktop_entry(gameFname, game);

  edbg << "Writing modes and settings" << flushl;
  wto.open((codegen_directory/"Preprocessor_Environment_Editable/GAME_SETTINGS.h").u8string().c_str(),ios_base::out);
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

  wto.open((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h").u8string().c_str(),ios_base::out);
  wto << license;
  wto << "#define BUILDMODE " << 0 << "\n";
  wto << "#define DEBUGMODE " << 0 << "\n";
  wto << '\n';
  wto.close();

  wto.open((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h").u8string().c_str(),ios_base::out);
  wto.close();

  //NEXT FILE ----------------------------------------
  //Object switch: A listing of all object IDs and the code to allocate them.
  edbg << "Writing object switch" << flushl;
  wto.open((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h").u8string().c_str(),ios_base::out);
    wto << license;
    wto << "#ifndef NEW_OBJ_PREFIX\n#  define NEW_OBJ_PREFIX\n#endif\n\n";
    for (auto *obj : state.parsed_objects) {
      wto << "case " << obj->id << ":\n";
      wto << "    NEW_OBJ_PREFIX new enigma::OBJ_" << obj->name <<"(x,y,idn);\n";
      wto << "  break;\n";
    }
    wto << "\n\n#undef NEW_OBJ_PREFIX\n";
  wto.close();


  //NEXT FILE ----------------------------------------
  //Resource names: Defines integer constants for all resources.
  edbg << "Writing resource names and maxima" << flushl;
  wto.open((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h").u8string().c_str(),ios_base::out);
  wto << license;

  wto << "namespace enigma {\n";
  std::string res_in = (compilerInfo.exe_vars["RESOURCES_IN"] != "") ? "RESOURCES_IN" : "RESOURCES";
  wto << "const char *resource_file_path=\"" << compilerInfo.exe_vars[res_in] << "\";\n";
  wto << "}\n";

  write_resource_meta(wto,     "object", game.objects);
  write_resource_meta(wto,     "sprite", game.sprites);
  write_resource_meta(wto, "background", game.backgrounds);
  write_resource_meta(wto,       "font", game.fonts);
  write_resource_meta(wto,   "timeline", game.timelines);
  write_resource_meta(wto,       "path", game.paths);
  write_resource_meta(wto,      "sound", game.sounds);
  write_resource_meta(wto,     "script", game.scripts);
  write_resource_meta(wto,     "shader", game.shaders);
  write_resource_meta(wto,       "room", game.rooms, false);
  
  // asset_get_index/type map
  wite_asset_enum(codegen_directory/"AssetEnum.h");
  
  wto << "#include \"AssetEnum.h\"\n";
  wto << "namespace enigma {\n\n";
  wto << "std::map<enigma_user::AssetType, std::map<std::string, int>> assetMap = {\n";
  
  std::string assets;
  write_asset_map(assets, game.objects,     "asset_object");
  write_asset_map(assets, game.sprites,     "asset_sprite");
  write_asset_map(assets, game.backgrounds, "asset_background");
  write_asset_map(assets, game.fonts,       "asset_font");
  write_asset_map(assets, game.timelines,   "asset_timeline");
  write_asset_map(assets, game.paths,       "asset_path");
  write_asset_map(assets, game.sounds,      "asset_sound");
  write_asset_map(assets, game.scripts,     "asset_script");
  write_asset_map(assets, game.shaders,     "asset_shader");
  write_asset_map(assets, game.rooms,       "asset_room");
  while (!assets.empty() && (assets.back() == ',' || assets.back() == '\n')) {
    assets.pop_back();
  }
  
  wto << assets;
  
  wto << "\n};\n";
  wto << "\n\n}\n";
  wto.close();


  //NEXT FILE ----------------------------------------
  //Timelines: Defines "moment" lookup structures for timelines.
  edbg << "Writing timeline control information" << flushl;
  wto.open((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_timelines.h").u8string().c_str(),ios_base::out);
  {
    wto << license;
    wto <<"namespace enigma {\n\n";

    //Each timeline has a lookup structure (in this case, a map) which allows easy forward/backward lookup.
    //This is currently constructed rather manually; there are probably more efficient
    // construction techniques, but none come to mind.
    wto <<"void timeline_system_initialize() {\n";
    wto <<"  std::vector< std::map<int, int> >& res = object_timelines::timeline_moments_maps;\n";
    wto <<"  res.reserve(" <<game.timelines.size() <<");\n";
    wto <<"  std::map<int, int> curr;\n\n";
    for (size_t i=0; i<game.timelines.size(); i++) {
      wto <<"  curr.clear();\n";
      for (int j = 0; j < game.timelines[i]->moments().size(); j++) {
        wto << "  curr[" << game.timelines[i]->moments()[j].step()
                         << "] = " << j <<";\n";
      }
      wto <<"  res.push_back(curr);\n\n";
    }
    wto <<"}\n\n";

    wto <<"}\n"; //namespace
  }
  wto.close();


  idpr("Performing Secondary Parsing and Writing Globals",25);

  edbg << "Linking globals and ambiguous variables" << flushl;
  res = current_language->link_globals(game, state);
  res = current_language->link_ambiguous(game, state);
  irrr();

  edbg << "Running Secondary Parse Passes" << flushl;
  res = current_language->compile_parseSecondary(state);

  edbg << "Writing events" << flushl;
  res = current_language->compile_writeDefraggedEvents(game, state.parsed_objects);
  irrr();

  edbg << "Writing object data" << flushl;
  res = current_language->compile_writeObjectData(game, state, mode);
  irrr();

  edbg << "Writing local accessors" << flushl;
  res = current_language->compile_writeObjAccess(
            state.parsed_objects, state.dot_accessed_locals, &state.global_object,
            game.settings.compiler().treat_uninitialized_vars_as_zero());
  irrr();

  edbg << "Writing font data" << flushl;
  res = current_language->compile_writeFontInfo(game);
  irrr();

  edbg << "Writing room data" << flushl;
  res = current_language->compile_writeRoomData(game, state.parsed_rooms, &state.global_object, mode);
  irrr();

  edbg << "Writing shader data" << flushl;
  res = current_language->compile_writeShaderData(game, &state.global_object);
  irrr();


  // Write the global variables to their own file to be included before any of the objects
  res = current_language->compile_writeGlobals(game, &state.global_object, state.dot_accessed_locals);
  irrr();

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

  if (codegen_only) {
    edbg << "The \"codegen-only\" flag was passed. Skipping compile and exiting," << flushl;
    return 0;
  }


  /**  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment two: Now that the game has been exported as C++ and raw
    resources, our task is to compile the game itself via GNU Make.
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

  idpr("Starting compile (This may take a while...)", 30);

  string make = compilerInfo.make_vars["MAKEFLAGS"];

  make += "-C \"" + unixfy_path(enigma_root) + "\" ";
  make += "Game ";
  make += "WORKDIR=\"" + unixfy_path(eobjs_directory) + "\" ";
  make += "CODEGEN=\"" + unixfy_path(codegen_directory) + "\" ";
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

  make += "COMPILEPATH=\"" + unixfy_path(compilepath) + "\" ";

  string extstr = "EXTENSIONS=\"";
  for (unsigned i = 0; i < parsed_extensions.size(); i++)
  	extstr += " " + parsed_extensions[i].pathname;
  make += extstr + "\"";

  string mfgfn = gameFname.u8string();
  for (size_t i = 0; i < mfgfn.length(); i++)
    if (mfgfn[i] == '\\') mfgfn[i] = '/';
  make += string(" OUTPUTNAME=\"") + mfgfn + "\" ";

  edbg << "Running make from `" << compilerInfo.MAKE_location << "'" << flushl;
  edbg << "Full command line: " << compilerInfo.MAKE_location << " " << make << flushl;

  string flags = "";

  if (redirect_make) {

    std::string dirs = "CODEGEN=" + codegen_directory.u8string() + " ";
    dirs += "WORKDIR=" + eobjs_directory.u8string() + " ";
    e_execs("make", dirs, "required-directories");

    // Pick a file and flush it
    const std::filesystem::path redirfile = (eobjs_directory/"enigma_compile.log");
    fclose(fopen(redirfile.u8string().c_str(),"wb"));

    // Redirect it
    ide_output_redirect_file(redirfile.u8string().c_str()); //TODO: If you pass this function the address it will screw up the value; most likely a JNA/Plugin bug.

    flags += "&> \"" + redirfile.u8string() + "\"";
  }

  int makeres = e_execs(compilerInfo.MAKE_location, make, flags);
  if (build_stopping) { build_stopping = false; return 0; }

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
  std::filesystem::path resfile = compilerInfo.exe_vars["RESOURCES"];
  cout << "`" << resfile.u8string() << "` == '$exe': " << (resfile == "$exe"?"true":"FALSE") << endl;
  if (resfile == "$exe")
  {
    gameModule = fopen(gameFname.u8string().c_str(),"ab");
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
    auto resname = resfile.u8string();
    for (size_t p = resname.find("$exe"); p != string::npos; p = resname.find("$game"))
      resname.replace(p,4,gameFname.u8string());
    gameModule = fopen(resname.c_str(),"wb");
    if (!gameModule) {
      user << "Failed to write resources to compiler-specified file, `" << resname << "`. Write permissions to valid path?" << flushl;
      idpr("Failed to write resources.",-1); return 12;
    }
  }

  // Start by setting off our location with a DWord of NULLs
  fwrite("\0\0\0",1,4,gameModule);

  idpr("Adding Sprites",90);

  res = current_language->module_write_sprites(game, gameModule);
  irrr();

  edbg << "Finalized sprites." << flushl;
  idpr("Adding Sounds",93);

  current_language->module_write_sounds(game, gameModule);

  current_language->module_write_backgrounds(game, gameModule);

  current_language->module_write_fonts(game, gameModule);

  current_language->module_write_paths(game, gameModule);

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
    string newdir = game.filename.empty() ? exe_filename : game.filename;
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
    rprog = string_replace_all(rprog,"$game",gameFname.u8string());
    rparam = string_replace_all(rparam,"$game",gameFname.u8string());
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
