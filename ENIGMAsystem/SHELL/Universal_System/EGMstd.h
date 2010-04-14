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

/**Bare minumum functions******************************************************\

void dispstr(char* str, double value)
int show_error(double errortext, double fatal)

\******************************************************************************/

//const double pi = M_PI;

namespace enigma{
	std::string evname(int num);
	int getID();
}

int show_error(std::string errortext, int fatal);
std::string string(double val);
std::string string(var& val);
std::string string(enigma::variant& val);
std::string string(std::string val);
std::string string(char* val);
std::string string_hex(double val);
std::string string_hex(var& val);
std::string string_hex(enigma::variant& val);
std::string string_hex(std::string val);
std::string string_hex(char* val);
std::string string_pointer(void* val);
