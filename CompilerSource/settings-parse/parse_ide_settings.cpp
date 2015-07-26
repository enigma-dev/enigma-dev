/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
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
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

using namespace std;

#include "filesystem/file_find.h"
#include "general/parse_basics_old.h"
#include "OS_Switchboard.h"
#include "parser/object_storage.h"
#include "crawler.h"
#include "eyaml.h"

#include "gcc_interface/gcc_backend.h"
#include "parse_ide_settings.h"
#include "compiler/compile_common.h"
#include "makedir.h"

string makedir = "";

string fc(const char* fn);
static void clear_ide_editables()
{
  ofstream wto;
  string f2comp = fc((makedir + "API_Switchboard.h").c_str());
  string f2write = license;
    string inc = "/include.h\"\n";
    f2write += "#include \"Platforms/" + (extensions::targetAPI.windowSys)            + "/include.h\"\n"
               "#include \"Graphics_Systems/" + (extensions::targetAPI.graphicsSys)   + "/include.h\"\n"
               "#include \"Audio_Systems/" + (extensions::targetAPI.audioSys)         + "/include.h\"\n"
               "#include \"Collision_Systems/" + (extensions::targetAPI.collisionSys) + "/include.h\"\n"
               "#include \"Networking_Systems/" + (extensions::targetAPI.networkSys) + "/include.h\"\n"
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
    wto.open((makedir +"API_Switchboard.h").c_str(),ios_base::out);
      wto << f2write << endl;
    wto.close();
  }

  wto.open((makedir +"Preprocessor_Environment_Editable/LIBINCLUDE.h").c_str());
    wto << license;
    wto << "/*************************************************************\nOptionally included libraries\n****************************/\n";
    wto << "#define STRINGLIB 1\n#define COLORSLIB 1\n#define STDRAWLIB 1\n#define PRIMTVLIB 1\n#define WINDOWLIB 1\n"
           "#define STDDRWLIB 1\n#define GMSURFACE 0\n#define BLENDMODE 1\n";
    wto << "/***************\nEnd optional libs\n ***************/\n";
  wto.close();

  wto.open((makedir +"Preprocessor_Environment_Editable/GAME_SETTINGS.h").c_str(),ios_base::out);
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

//#include "backend/ideprint.h"
//#include "backend/JavaCallbacks.h"

#define user cout << "\n\n\n\n\n"
#define flushl "\n\n\n\n\n"

extern const char* establish_bearings(const char *compiler);

void parse_ide_settings(const char* eyaml)
{
  ey_data settree = parse_eyaml_str(eyaml);
  eyit it;
  
  // Read settings info
  setting::use_cpp_strings   = settree.get("inherit-strings-from").toInt();
  setting::use_cpp_escapes   = settree.get("inherit-escapes-from").toInt();
  setting::use_incrementals  = settree.get("inherit-increment-from").toInt();
  setting::use_gml_equals    = !settree.get("inherit-equivalence-from").toInt();
  setting::literal_autocast  = settree.get("treat-literals-as").toInt();
  setting::inherit_objects   = settree.get("inherit-objects").toBool();
  setting::automatic_semicolons   = settree.get("automatic-semicolons").toBool();
  setting::compliance_mode = settree.get("compliance-mode").toInt()==1 ? setting::COMPL_GM5 : setting::COMPL_STANDARD;
  setting::keyword_blacklist = settree.get("keyword-blacklist").toString();

  // Use a platform-specific make directory.
  std::string make_directory = "./ENIGMA/";
#if CURRENT_PLATFORM_ID == OS_WINDOWS
  make_directory = "%PROGRAMDATA%/ENIGMA/";
#elif CURRENT_PLATFORM_ID == OS_LINUX
  make_directory = "%HOME%/.enigma/";
#elif CURRENT_PLATFORM_ID == OS_MACOSX
  make_directory = "./ENIGMA/";
#endif

  //Now actually set it, taking backslashes into account.
#if CURRENT_PLATFORM_ID == OS_WINDOWS
	setMakeDirectory(myReplace(escapeEnv(make_directory), "\\","/"));
#else
	setMakeDirectory(escapeEnv(make_directory));
#endif

  //ide_dia_open();

  #define ey_cp(v,x,y) \
  it = settree.find("target-" #x); \
  if (it == settree.end()) { \
     user << "ERROR! IDE has not specified a target " #x " " #y "!" << flushl; \
     extensions::targetAPI.v ## Sys = "None"; \
  } else  extensions::targetAPI.v ## Sys = eyscalar(it);

  // Get target's windowing api
  ey_cp(window,    windowing,api)
  // Get requested graphics system
  ey_cp(graphics,  graphics,system)
  // Get requested audio system
  ey_cp(audio,     audio,system)
  // Get requested collision system
  ey_cp(collision, collision,system)
  // Get requested widget system
  ey_cp(widget,    widget,system)
  // Get requested networking system
  ey_cp(network,   networking,system)

  ifstream ifs; string eyname;
  ifs.open((eyname = "ENIGMAsystem/SHELL/Platforms/" + extensions::targetAPI.windowSys + "/Info/About.ey").c_str());
  if (ifs.is_open())
  {
    ey_data l = parse_eyaml(ifs, eyname.c_str());
    it = l.find("links");
    if (it != l.end())
      extensions::targetAPI.windowLinks = eyscalar(it);
    ifs.close();
  }
  string platn = tolower(extensions::targetAPI.windowSys);

  #define eygl(fn,v) {};
  /*{\
    ifs.open((eyname = "ENIGMAsystem/SHELL/" #fn "/" + extensions::targetAPI.v ## Sys + "/Config/" + platn + ".ey").c_str()); \
    if (ifs.is_open()) \
    { \
      ey_data l = parse_eyaml(ifs, eyname.c_str()); \
      user << "Opened " << eyname << flushl; \
      if ((it = l.find("links")) != l.end()) \
        extensions::targetAPI.v ## Links = eyscalar(it); \
      else \
        user << "Links not named in " << eyname << flushl; \
      ifs.close(); \
    } \
    else user << "Could not open " << eyname << ".\n"; \
  }*/

  eygl(Graphics_Systems, graphics);
  eygl(Widget_Systems, widget);
  eygl(Audio_Systems, audio);
  eygl(Networking_Systems, network);

  string cinffile = settree.get("target-compiler");
  cinffile = "Compilers/" CURRENT_PLATFORM_NAME "/" + cinffile + ".ey";

  const char *a = establish_bearings(cinffile.c_str());
  if (a) cout << "Parse fail: " << a << endl;

  // Read info about the compiler
  ifstream cinfstream(cinffile.c_str());
  ey_data cinfo = parse_eyaml(cinfstream,cinffile);
  extensions::targetOS.resfile   = cinfo.get("resources");
  extensions::targetOS.buildext  = cinfo.get("build-extension");
  extensions::targetOS.buildname = cinfo.get("run-output");
  extensions::targetOS.runprog   = cinfo.get("run-program");
  extensions::targetOS.runparam  = cinfo.get("run-params");
  extensions::targetOS.identifier = cinfo.get("target-platform");

  cout << "Setting up IDE editables... " << endl;
  requested_extensions.clear();
  requested_extensions = explode((string)settree.get("extensions"));
  extensions::parse_extensions(requested_extensions);
  clear_ide_editables();
}

