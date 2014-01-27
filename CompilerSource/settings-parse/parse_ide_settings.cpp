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
#include "workdir.h"

#include "workdir.h"
#include "settings.h"
#include "OS_Switchboard.h" //Tell us where the hell we are
#include <cstdlib>
#include <string.h>

using std::string;

string myReplace(string str, const string& oldStr, const string& newStr)
{
  std::string nstr = str;
  size_t pos = 0;
  while((pos = nstr.find(oldStr, pos)) != std::string::npos)
  {
     nstr.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
  return nstr;
}

string escapeEnv(string str, string env) {
	char* val = getenv(env.c_str());
	if (val != NULL)
		return myReplace(str, "%" + env + "%", val);
	return str;
}

string escapeEnv(string str) {
	string escaped = escapeEnv(str, "PROGRAMDATA");
	escaped = escapeEnv(escaped, "ALLUSERSPROFILE");
	escaped = escapeEnv(escaped, "HOME");
	return escaped;
}

#include "parse_ide_settings.h"
#include "workdir.h"
// Only include the headers for mkdir() if we are not on Windows; on Windows we use CreateDirectory() from windows.h
#if CURRENT_PLATFORM_ID != OS_WINDOWS
#include <sys/stat.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

string workdir = "";

void clear_ide_editables();
static inline vector<string> explode(string n) {
  vector<string> ret;
  size_t pos = 0, epos;
  while (is_useless(n[pos])) pos++;
  for (epos = n.find(','); epos != string::npos; epos = n.find(',',pos)) {
    ret.push_back(n.substr(pos,epos-pos));
    pos = epos; while (is_useless(n[++pos]));
  }
  if (n.length() > pos)
    ret.push_back(n.substr(pos));
  return ret;
}

inline string tolower(string x) {
  for (size_t i = 0; i < x.length(); i++)
    if (x[i] >= 'A' and x[i] <= 'Z') x[i] -= 'A' - 'a';
  return x;
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
  setting::make_directory = settree.get("make-directory").toString();

#if CURRENT_PLATFORM_ID == OS_WINDOWS
	workdir = myReplace(escapeEnv(setting::make_directory), "\\","/");
#else
	workdir = escapeEnv(setting::make_directory);
#endif

#if CURRENT_PLATFORM_ID == OS_WINDOWS
  CreateDirectory((workdir).c_str(), NULL);
  if (!CreateDirectory((workdir +"Preprocessor_Environment_Editable").c_str(), NULL)) {
	DWORD error = GetLastError();
	switch (error) {
		case ERROR_ALREADY_EXISTS: 
			std::cout << "WARNING! Failed to create build directory, directory already exists: \"" << workdir << "\"" << endl;
			break;
		case ERROR_PATH_NOT_FOUND:
			std::cout << "ERROR! Failed to create build directory, path not found: \"" << workdir << "\"" << endl;
			break;
		default:
			std::cout << "Created build directory: \"" << workdir << "\"" << endl;
			break;
	}
  }
#else
  mkdir((workdir).c_str(),0755);
  if (mkdir((workdir +"Preprocessor_Environment_Editable").c_str(),0755) == -1)
  {
	  std::cout << "Failed to create build directory at " << workdir << endl;
  } else {
	  std::cout << "Created build directory: \"" << workdir << "\"" << endl;
  }
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

