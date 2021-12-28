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


#include "general/parse_basics_old.h"
#include "OS_Switchboard.h"
#include "parser/object_storage.h"
#include "crawler.h"

#include "gcc_interface/gcc_backend.h"
#include "parse_ide_settings.h"
#include "compiler/compile_common.h"
#include "settings.h"
#include "strings_util.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

using namespace std;

static void reset_ide_editables()
{
  string f2comp;
  std::filesystem::path API_Switchboard = codegen_directory/"API_Switchboard.h";
  if (std::filesystem::exists(API_Switchboard))
    f2comp = FileToString(API_Switchboard);

  ofstream wto;

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
      YAML::Node about = YAML::LoadFile(enigma_root/"ENIGMAsystem/SHELL"/(ext.path + ext.name)/"About.ey");
      f2write += incg + ext.pathname + inc;
      if (ext.implements != "")
        f2write += incg + ext.pathname + impl;
    }

  if (f2comp != f2write + "\n")
  {
    wto.open(API_Switchboard.u8string().c_str(),ios_base::out);
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

bool codegen_only = false;
std::filesystem::path enigma_root;
std::filesystem::path eobjs_directory;
std::filesystem::path codegen_directory;

void ey_cp(const YAML::Node& node, const std::string& key, std::string& sys) {
  if (!node[key].IsDefined()) {
    user << "ERROR! IDE has not specified a target " << key << "!" << flushl; \
    sys = "None";
  } else sys = node[key].as<std::string>();
}

void parse_ide_settings(const YAML::Node& yaml)
{
  // Read settings info
  setting::use_cpp_strings   = yaml["inherit-strings-from"].as<int>();
  setting::use_cpp_escapes   = yaml["inherit-escapes-from"].as<int>();
  setting::use_incrementals  = yaml["inherit-increment-from"].as<int>();
  setting::use_gml_equals    = !yaml["inherit-equivalence-from"].as<int>(0); //FIXME: LGM aint passing this.
  setting::literal_autocast  = yaml["treat-literals-as"].as<int>();
  setting::inherit_objects   = yaml["inherit-objects"].as<bool>();

  switch (yaml["compliance-mode"].as<int>()) {
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
  setting::automatic_semicolons   = yaml["automatic-semicolons"].as<bool>();
  setting::keyword_blacklist = yaml["keyword-blacklist"].as<std::string>();

  // Path to enigma sources
  enigma_root = yaml["enigma-root"].as<std::string>(".");

  // Use a platform-specific make directory.
  eobjs_directory = yaml["eobjs-directory"].as<std::string>();

  if (eobjs_directory.empty())
  {
  #if CURRENT_PLATFORM_ID == OS_WINDOWS
    eobjs_directory = "%LOCALAPPDATA%/ENIGMA/";
  #else
    eobjs_directory = "%HOME%/.enigma/";
  #endif
  }

  codegen_directory = yaml["codegen-directory"].as<std::string>();

  if (codegen_directory.empty())
    codegen_directory = eobjs_directory;

  eobjs_directory = escapeEnv(eobjs_directory.u8string());
  codegen_directory = escapeEnv(codegen_directory.u8string());
  enigma_root = escapeEnv(enigma_root.u8string());

  if (yaml["codegen-only"])
    codegen_only = yaml["codegen-only"].as<bool>();

  // Get target's windowing api
  ey_cp(yaml, "target-windowing", extensions::targetAPI.windowSys);
  // Get requested graphics system
  ey_cp(yaml, "target-graphics", extensions::targetAPI.graphicsSys);
  // Get requested audio system
  ey_cp(yaml, "target-audio", extensions::targetAPI.audioSys);
  // Get requested collision system
  ey_cp(yaml, "target-collision", extensions::targetAPI.collisionSys);
  // Get requested widget system
  ey_cp(yaml, "target-widget", extensions::targetAPI.widgetSys);
  // Get requested networking system
  ey_cp(yaml, "target-networking", extensions::targetAPI.networkSys);

  string target_compiler = yaml["target-compiler"].as<std::string>();
  std::filesystem::path cinffile = enigma_root/"Compilers"/CURRENT_PLATFORM_NAME/(target_compiler + ".ey");

  const char *a = establish_bearings(cinffile.u8string().c_str());
  if (a) cout << "Parse fail: " << a << endl;

  cout << "Setting up IDE editables... " << endl;
  requested_extensions_last_parse.clear();
  requested_extensions_last_parse = explode(yaml["extensions"].as<std::string>());

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
