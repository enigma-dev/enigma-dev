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

//Error codes
enum {
  E_ERROR_NO_ERROR_LOL,
  E_ERROR_PLUGIN_FUCKED_UP,
  E_ERROR_SYNTAX,
  E_ERROR_BUILD,
  E_ERROR_LOAD_LOCALS,
  E_ERROR_WUT
};

//Mode codes
enum cmodes {
  mode_run,
  mode_debug,
  mode_build,
  mode_compile
};

struct parsed_script
{
  parsed_object obj; //Script will pretend to be an object, having locals and globals inherited by all who call it.
  parsed_event pev; //Contains only one "event," though.
  parsed_script(): obj(), pev(&obj) {}; //automatically link our even to our object.
};
