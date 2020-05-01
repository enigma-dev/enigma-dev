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

#include "eyaml/eyaml.h"
#include "general/parse_basics_old.h"
#include "OS_Switchboard.h"
#include "parser/object_storage.h"
#include "crawler.h"

#include "gcc_interface/gcc_backend.h"
#include "parse_ide_settings.h"
#include "compiler/compile_common.h"
#include "settings.h"

inline string fc(const char* fn);
static void reset_ide_editables()
{
  ofstream wto;
  string f2comp = fc((codegen_directory/"API_Switchboard.h").u8string().c_str());
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
    for (const auto &ext : parsed_extensions) {
      ifstream ifabout((ext.pathname + "/About.ey").c_str());
      ey_data about = parse_eyaml(ifabout, ext.path + ext.name + "/About.ey");
      f2write += incg + ext.pathname + inc;
      if (ext.implements != "")
        f2write += incg + ext.pathname + impl;
    }

  if (f2comp != f2write + "\n")
  {
    wto.open((codegen_directory/"API_Switchboard.h").u8string().c_str(),ios_base::out);
      wto << f2write << endl;
    wto.close();
  }

  wto.open((codegen_directory/"Preprocessor_Environment_Editable/LIBINCLUDE.h").c_str());
    wto << license;
    wto << "/*************************************************************\nOptionally included libraries\n****************************/\n";
    wto << "#define STRINGLIB 1\n#define COLORSLIB 1\n#define STDRAWLIB 1\n#define PRIMTVLIB 1\n#define WINDOWLIB 1\n"
           "#define STDDRWLIB 1\n#define GMSURFACE 0\n#define BLENDMODE 1\n";
    wto << "/***************\nEnd optional libs\n ***************/\n";
  wto.close();

  wto.open((codegen_directory/"Preprocessor_Environment_Editable/GAME_SETTINGS.h").c_str(),ios_base::out);
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

std::filesystem::path enigma_root;
std::filesystem::path eobjs_directory;
std::filesystem::path codegen_directory;

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
  switch (settree.get("compliance-mode").toInt()) {
    case 4:
      setting::compliance_mode = setting::COMPL_GM8;
      break;
    case 3:
      setting::compliance_mode = setting::COMPL_GM7;
      break;
    case 2:
      setting::compliance_mode = setting::COMPL_GM6;
      break;
    case 1:
      setting::compliance_mode = setting::COMPL_GM5;
      break;
    default:
      setting::compliance_mode = setting::COMPL_STANDARD;
  }
  setting::automatic_semicolons   = settree.get("automatic-semicolons").toBool();
  setting::keyword_blacklist = settree.get("keyword-blacklist").toString();

  // Use a platform-specific make directory.
  eobjs_directory = settree.get("eobjs-directory").toString();
  
  if (eobjs_directory.empty())
  {
  #if CURRENT_PLATFORM_ID == OS_WINDOWS
    eobjs_directory = "%LOCALAPPDATA%/ENIGMA/";
  #else
    eobjs_directory = "%HOME%/.enigma/";
  #endif
  }
  
  codegen_directory = settree.get("codegen-directory").toString();
  
  if (codegen_directory.empty())
    codegen_directory = eobjs_directory;
    
  eobjs_directory = escapeEnv(eobjs_directory.u8string());
  codegen_directory = escapeEnv(codegen_directory.u8string());
  enigma_root = escapeEnv(enigma_root.u8string());

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

  ifstream ifs; std::filesystem::path eyname;
  ifs.open((eyname = enigma_root/"ENIGMAsystem/SHELL/Platforms"/extensions::targetAPI.windowSys/"Info/About.ey").u8string().c_str());
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

  string target_compiler = settree.get("target-compiler");
  std::filesystem::path cinffile = enigma_root/"Compilers"/CURRENT_PLATFORM_NAME/(target_compiler + ".ey");

  const char *a = establish_bearings(cinffile.u8string().c_str());
  if (a) cout << "Parse fail: " << a << endl;

  cout << "Setting up IDE editables... " << endl;
  requested_extensions_last_parse.clear();
  requested_extensions_last_parse = explode((string)settree.get("extensions"));
  
  string sep = ": ";
  cout << "Loading extensions" << flush;
  for (const string &e : requested_extensions_last_parse) {
    cout << sep << e << flush;
    sep = ", ";
  }
  cout << endl;
  
  extensions::parse_extensions(requested_extensions_last_parse);
  reset_ide_editables();
}
