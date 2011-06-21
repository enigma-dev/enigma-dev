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

#include "settings.h"

namespace extensions
{
  sdk_descriptor targetSDK;
  api_descriptor targetAPI;
  compiler_descriptor targetOS;
}

namespace setting
{
  //Compatibility / Progess options
  bool use_cpp_strings = 0;  // Defines what language strings are inherited from.    0 = GML,               1 = C++
  bool use_cpp_escapes = 0;  // Defines what language strings are inherited from.    0 = GML,               1 = C++
  bool use_gml_equals = 0;   // Defines what language operator= is inherited from.   0 = C++,               1 = GML
  bool use_incrementals = 0; // Defines how operators ++ and -- are treated.         0 = GML,               1 = C++
  bool literal_autocast = 0; // Determines how literals are treated.                 0 = enigma::variant,   1 = C++ scalars
};


