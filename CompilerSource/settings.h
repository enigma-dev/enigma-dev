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
#ifndef _SETTINGS_H
#define _SETTINGS_H
#include <string>
using std::string;

namespace extensions
{
  struct sdk_descriptor {
    string name, identifier, represents, description, author, build_platforms;
  };
  struct api_descriptor
  {
    string
      windowSys,   graphicsSys,   audioSys,   collisionSys,   widgetSys,   networkSys;
    string
      windowLinks, graphicsLinks, audioLinks, collisionLinks, widgetLinks, networkLinks;
  };
  struct compiler_descriptor {
    string identifier, resfile, buildext, buildname, runprog, runparam, builddir;
  };

  extern sdk_descriptor targetSDK;
  extern api_descriptor targetAPI;
  extern compiler_descriptor targetOS;
}

namespace setting
{
  //Compliance levels. These enforce various settings specific to certain versions of GM.
  //Prefer generalized solutions; use these for direct incompatibilities. (Add other GM versions as needed).
  enum COMPLIANCE_LVL {
    COMPL_GM567 = 5,       //GM5,6,7 compliance. timeline_running will default to "true". exit will abort single code actions. Sprite and background alphas default to 1
    COMPL_GM8 = 8,         //GM8 compliance. exit will abort single code actions. Sprite and background alphas default to 1.
    COMPL_STANDARD = 255,    //Standard (enigma) compliance. Default and recommended.
  };

  //Compatibility / Progess options
  extern bool use_cpp_strings;  // Defines what language strings are inherited from.    0 = GML,               1 = C++
  extern bool use_cpp_escapes;  // Defines what language strings are inherited from.    0 = GML,               1 = C++
  extern bool use_gml_equals;   // Defines what language operator= is inherited from.   0 = GML,               1 = C++
  extern bool use_incrementals; // Defines how operators ++ and -- are treated.         0 = GML,               1 = C++
  extern bool literal_autocast; // Determines how literals are treated.                 0 = enigma::variant,   1 = C++ scalars
  extern bool inherit_objects;  // Determines whether objects should automatically inherit locals and events from their parents
  extern bool automatic_semicolons; // Determines whether semicolons should automatically be added or if the user wants strict syntax
  extern COMPLIANCE_LVL compliance_mode; // How to resolve differences between GM versions.
  extern string keyword_blacklist; //Words to blacklist from user scripts, separated by commas.
}

#endif


