/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Josh Ventura                                             **
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
#include "../settings-parse/eyaml.h"

#include "components/components.h"
#include "../gcc_interface/gcc_backend.h"

#include "../general/bettersystem.h"
#include "event_reader/event_parser.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}
inline void writef(float x, FILE *f) {
  fwrite(&x,4,1,f);
}

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

string fc(const char*);
string toUpper(string x) { string res = x; for (size_t i = 0; i < res.length(); i++) res[i] = res[i] >= 'a' and res[i] <= 'z' ? res[i] + 'A' - 'a' : res[i]; return res; }
void clear_ide_editables()
{
  ofstream wto;
  string f2comp = fc("ENIGMAsystem/SHELL/API_Switchboard.h");
  string f2write = license;
    string inc = "/include.h\"\n";
    f2write += "#include \"Platforms/" + (extensions::targetAPI.windowSys)            + "/include.h\"\n"
               "#include \"Graphics_Systems/" + (extensions::targetAPI.graphicsSys)   + "/include.h\"\n"
               "#include \"Audio_Systems/" + (extensions::targetAPI.audioSys)         + "/include.h\"\n"
               "#include \"Collision_Systems/" + (extensions::targetAPI.collisionSys) + "/include.h\"\n"
               "#include \"Widget_Systems/" + (extensions::targetAPI.widgetSys)       + inc;

    const string incg = "#include \"", impl = "/implement.h\"\n";
    f2write += "\n// Extensions selected by user\n";
    for (unsigned i = 0; i < parsed_extensions.size(); i++)
    {
      ifstream ifabout((parsed_extensions[i].pathname + "/About.ey").c_str());
      ey_data about = parse_eyaml(ifabout,parsed_extensions[i].path + parsed_extensions[i].name + "/About.ey");
      f2write += incg + parsed_extensions[i].pathname + inc;
      if (parsed_extensions[i].implements != "")
        f2write += incg + parsed_extensions[i].pathname + impl;
    }

  if (f2comp != f2write)
  {
    wto.open("ENIGMAsystem/SHELL/API_Switchboard.h",ios_base::out);
      wto << f2write << endl;
    wto.close();
  }

  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/LIBINCLUDE.h");
    wto << license;
    wto << "/*************************************************************\nOptionally included libraries\n****************************/\n";
    wto << "#define STRINGLIB 1\n#define COLORSLIB 1\n#define STDRAWLIB 1\n#define PRIMTVLIB 1\n#define WINDOWLIB 1\n#define FONTPOLYS 1\n"
           "#define STDDRWLIB 1\n#define GMSURFACE 0\n#define BLENDMODE 1\n#define COLLIGMA  0\n";
    wto << "/***************\nEnd optional libs\n ***************/\n";
  wto.close();

  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/GAME_SETTINGS.h",ios_base::out);
    wto << license;
    wto << "#define ASSUMEZERO 0\n";
    wto << "#define PRIMBUFFER 0\n";
    wto << "#define PRIMDEPTH2 6\n";
    wto << "#define AUTOLOCALS 0\n";
    wto << "#define MODE3DVARS 0\n";
    wto << "void ABORT_ON_ALL_ERRORS() { }\n";
    wto << '\n';
  wto.close();
}

// modes: 0=run, 1=debug, 2=build, 3=compile
enum { emode_run, emode_debug, emode_build, emode_compile, emode_rebuild };
dllexport int compileEGMf(EnigmaStruct *es, const char* exe_filename, int mode)
{
  cout << "Initializing dialog boxes" << endl;
    ide_dia_clear();
    ide_dia_open();
  cout << "Initialized." << endl;

  if (mode == emode_rebuild)
  {
    edbg << "Rebuilding all..." << flushl;
    edbg << "Running make from `" << MAKE_location << "'" << flushl;
    edbg << "Done." << flushl;
    e_execs(MAKE_location + " clean-game eTCpath=\"" + MAKE_paths + "\"");
    return 0;
  }

  edbg << "Building for mode (" << mode << ")" << flushl;

  // CLean up from any previous executions.
  edbg << "Cleaning up from previous executions" << flushl;
    parsed_objects.clear(); //Make sure we don't dump in any old object code...
    shared_locals_clear();  //Forget inherited locals, we'll reparse them
    event_info_clear();     //Forget event definitions, we'll re-get them

  // Re-establish ourself
    // Read the global locals: locals that will be included with each instance
    {
      vector<string> extnp;
      for (int i = 0; i < es->extensionCount; i++)
        extnp.push_back(string(es->extensions[i].path) + es->extensions[i].name);
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

  edbg << "File version: " << es->fileVersion << flushl << flushl;
  if (es->fileVersion != 800)
    edbg << "Incorrect version. File is too " << ((es->fileVersion > 800)?"new":"old") << " for this compiler. Continuing anyway, because this number is always wrong.";




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

  edbg << "Copying path names [" << es->pathCount << "]" << flushl;
  for (int i = 0; i < es->pathCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->paths[i].name);

  edbg << "Copying script names [" << es->scriptCount << "]" << flushl;
  for (int i = 0; i < es->scriptCount; i++)
    quickmember_script(globals_scope,es->scripts[i].name);

  edbg << "Copying font names [" << es->fontCount << "]" << flushl;
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
  parsed_script *parsed_scripts[es->scriptCount];

  scr_lookup.clear();
  used_funcs::zero();

  int res;
  #define irrr() if (res) { idpr("Error occurred; see scrollback for details.",-1); return res; }

  res = compile_parseAndLink(es,parsed_scripts);
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

    max = 0;
    wto << "enum //object names\n{\n";
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++) {
      if (i->first >= max) max = i->first + 1;
      wto << "  " << i->second->name << " = " << i->first << ",\n";
    } wto << "};\nnamespace enigma { size_t object_idmax = " << max << "; }\n\n";

    max = 0;
    wto << "enum //sprite names\n{\n";
    for (int i = 0; i < es->spriteCount; i++) {
      if (es->sprites[i].id >= max) max = es->sprites[i].id + 1;
      wto << "  " << es->sprites[i].name << " = " << es->sprites[i].id << ",\n";
    } wto << "};\nnamespace enigma { size_t sprite_idmax = " << max << "; }\n\n";

    max = 0;
    wto << "enum //background names\n{\n";
    for (int i = 0; i < es->backgroundCount; i++) {
      if (es->backgrounds[i].id >= max) max = es->backgrounds[i].id + 1;
      wto << "  " << es->backgrounds[i].name << " = " << es->backgrounds[i].id << ",\n";
    } wto << "};\nnamespace enigma { size_t background_idmax = " << max << "; }\n\n";

    max = 0;
    wto << "enum //font names\n{\n";
    for (int i = 0; i < es->fontCount; i++) {
      if (es->fonts[i].id >= max) max = es->fonts[i].id + 1;
      wto << "  " << es->fonts[i].name << " = " << es->fonts[i].id << ",\n";
    } wto << "};\nnamespace enigma { size_t font_idmax = " << max << "; }\n\n";


    max = 0;
	wto << "enum //timeline names\n{\n";
	for (int i = 0; i < es->timelineCount; i++) {
	    if (es->timelines[i].id >= max) max = es->timelines[i].id + 1;
        wto << "  " << es->timelines[i].name << " = " << es->timelines[i].id << ",\n";
	} wto << "};\nnamespace enigma { size_t timeline_idmax = " << max << "; }\n\n";

    max = 0;
	wto << "enum //path names\n{\n";
	for (int i = 0; i < es->pathCount; i++) {
	    if (es->paths[i].id >= max) max = es->paths[i].id + 1;
        wto << "  " << es->paths[i].name << " = " << es->paths[i].id << ",\n";
	} wto << "};\nnamespace enigma { size_t path_idmax = " << max << "; }\n\n";


    max = 0;
    wto << "enum //sound names\n{\n";
    for (int i = 0; i < es->soundCount; i++) {
      if (es->sounds[i].id >= max) max = es->sounds[i].id + 1;
      wto << "  " << es->sounds[i].name << " = " << es->sounds[i].id << ",\n";
    } wto << "};\nnamespace enigma { size_t sound_idmax = " <<max << "; }\n\n";

    max = 0;
    wto << "enum //room names\n{\n";
    for (int i = 0; i < es->roomCount; i++) {
      if (es->rooms[i].id >= max) max = es->rooms[i].id + 1;
      wto << "  " << es->rooms[i].name << " = " << es->rooms[i].id << ",\n";
    }
    wto << "};\nnamespace enigma { size_t room_idmax = " <<max << "; }\n\n";
  wto.close();

  idpr("Performing Secondary Parsing and Writing Globals",25);

  // Defragged events must be written before object data, or object data cannot determine which events were used.
  edbg << "Writing events" << flushl;
  res = compile_writeDefraggedEvents(es);
  irrr();

  parsed_object EGMglobal;

  edbg << "Linking globals" << flushl;
  res = link_globals(&EGMglobal,es,parsed_scripts);
  irrr();

  edbg << "Running Secondary Parse Passes" << flushl;
  res = compile_parseSecondary(parsed_objects,parsed_scripts,es->scriptCount,&EGMglobal);

  edbg << "Writing object data" << flushl;
  res = compile_writeObjectData(es,&EGMglobal);
  irrr();

  edbg << "Writing local accessors" << flushl;
  res = compile_writeObjAccess(parsed_objects, &EGMglobal);
  irrr();

  edbg << "Writing font data" << flushl;
  res = compile_writeFontInfo(es);
  irrr();

  edbg << "Writing room data" << flushl;
  res = compile_writeRoomData(es,&EGMglobal);
  irrr();



  // Write the global variables to their own file to be included before any of the objects
  res = compile_writeGlobals(es,&EGMglobal);
  irrr();


  // Now we write any additional templates requested by the window system.
  // compile_handle_templates(es);


  /**  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Segment two: Now that the game has been exported as C++ and raw
    resources, our task is to compile the game itself via GNU Make.
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

  idpr("Adding resources...",90);
  string desstr = "./ENIGMAsystem/SHELL/design_game" + extensions::targetOS.buildext;
  string gameFname = mode == emode_build ? desstr.c_str() : (desstr = exe_filename, exe_filename); // We will be using this first to write, then to run

  idpr("Starting compile (This may take a while...)", 30);

  string gflags = "-s -O3";
  string make = "Game ";

  string glinks = extensions::targetAPI.windowLinks;
    if (extensions::targetAPI.graphicsLinks  != "") glinks += " " + extensions::targetAPI.graphicsLinks;
    if (extensions::targetAPI.audioLinks     != "") glinks += " " + extensions::targetAPI.audioLinks;
    if (extensions::targetAPI.widgetLinks    != "") glinks += " " + extensions::targetAPI.widgetLinks;
    if (extensions::targetAPI.collisionLinks != "") glinks += " " + extensions::targetAPI.collisionLinks;
    if (extensions::targetAPI.networkLinks   != "") glinks += " " + extensions::targetAPI.networkLinks;

  make += "GMODE=Run ";
  make += "GFLAGS=\"" + gflags + "\" ";
  make += "CFLAGS=\"" + TOPLEVEL_cflags + "\" ";
  make += "CPPFLAGS=\"" + TOPLEVEL_cppflags + "\" ";
  make += "GLINKS=\"" + (TOPLEVEL_links == ""? "" : TOPLEVEL_links + " ") + glinks + "\" ";
  make += "GRAPHICS=" + extensions::targetAPI.graphicsSys + " ";
  make += "AUDIO=" + extensions::targetAPI.audioSys + " ";
  make += "COLLISION=" + extensions::targetAPI.collisionSys + " ";
  make += "WIDGETS="  + extensions::targetAPI.widgetSys + " ";
  make += "PLATFORM=" + extensions::targetAPI.windowSys + " ";

  string compilepath = CURRENT_PLATFORM_NAME "/" + extensions::targetOS.identifier;
  make += "COMPILEPATH=" + compilepath + " ";

  string extstr = "EXTENSIONS=\"", extlinks = "EXTLINKS=\"";
  if (parsed_extensions.size())
  {
    string objdir = "/.eobjs/" + compilepath + "/*.o";
    extstr += parsed_extensions[0].pathname + "/Extension";
    extlinks += parsed_extensions[0].pathname + objdir;
    for (unsigned i = 1; i < parsed_extensions.size(); i++)
      extstr += " " + parsed_extensions[i].pathname + "/Extension",
      extlinks += " " + parsed_extensions[i].pathname + objdir;
  }
  make += extstr + "\" " + extlinks + "\" ";


  string mfgfn = gameFname;
  for (size_t i = 0; i < mfgfn.length(); i++)
    if (mfgfn[i] == '\\') mfgfn[i] = '/';
  make += string("OUTPUTNAME=\"") + mfgfn + "\" ";
  make += "eTCpath=\"" + MAKE_paths + "\"";

  edbg << "Running make from `" << MAKE_location << "'" << flushl;
  edbg << "Full command line: " << MAKE_location << " " << make << flushl;

//  #if CURRENT_PLATFORM_ID == OS_MACOSX
  //int makeres = better_system("cd ","/MacOS/");
//  int makeres = better_system(MAKE_location,"MacOS");

  // Pick a file and flush it
  const char* redirfile = "redirfile.txt";
  fclose(fopen(redirfile,"wb"));

  // Redirect it
  ide_output_redirect_file(redirfile);
  int makeres = e_execs(MAKE_location,make,"&>",redirfile);

  // Stop redirecting GCC output
  ide_output_redirect_reset();

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

  #if OS_ANDROID
    "ENIGMAsystem/SHELL/Platforms/Android/EnigmaAndroidGame/libs/armeabi/libndkEnigmaGame.so";
  #endif

  FILE *gameModule;
  int resourceblock_start = 0;
  cout << "`" << extensions::targetOS.resfile << "` == '$exe': " << (extensions::targetOS.resfile == "$game"?"true":"FALSE") << endl;
  if (extensions::targetOS.resfile == "$exe")
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
    string resname = extensions::targetOS.resfile;
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

  res = module_write_sprites(es, gameModule);
  irrr();

  edbg << "Finalized sprites." << flushl;
  idpr("Adding Sounds",93);

  module_write_sounds(es,gameModule);

  module_write_backgrounds(es,gameModule);

  module_write_fonts(es,gameModule);

  module_write_paths(es,gameModule);

  // Tell where the resources start
  fwrite("\0\0\0\0res0",8,1,gameModule);
  fwrite(&resourceblock_start,4,1,gameModule);

  //Close the game module; we're done adding resources
  idpr("Closing game module and running if requested.",99);
  edbg << "Closing game module and running if requested." << flushl;
  fclose(gameModule);

  if (mode == emode_run or mode == emode_build)
  {
    string rprog = extensions::targetOS.runprog, rparam = extensions::targetOS.runparam;
    rprog = string_replace_all(rprog,"$game",gameFname);
    rparam = string_replace_all(rparam,"$game",gameFname);
    user << "Running \"" << rprog << "\" " << rparam << flushl;
    int gameres = e_execs(rprog, rparam);
    user << "Game returned " << gameres << "\n";
  }

  idpr("Done.", 100);
  return 0;
};
