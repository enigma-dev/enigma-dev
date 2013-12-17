/**
* @file syntax_error.h
* @brief Header file of the syntax_error struct used in the functions of the plugin to the compiler.
*
* @section License
*
* Copyright (C) 2013 Robert B. Colton
* This file is part of ENIGMA.
*
* ENIGMA is free software: you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation, version 3 of the License, or (at your option) any later version.
*
* ENIGMA is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
* PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* ENIGMA. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef SYNTAX_ERROR_H_INCLUDED
#define SYNTAX_ERROR_H_INCLUDED

#include <string>
using namespace std;

/// Structure for reporting syntax issues to the IDE.
struct syntax_error {
  const char *err_str; ///< The text associated with the error.
  int line; ///< The line number on which the error occurred.
  int position; ///< The column number in the line at which the error occurred.
  int absolute_index; ///< The zero-based position in the entire code at which the error occurred.

  /// Set the error content in one fell swoop.
  void set(int x, int y,int a, string s);
};

/// Static instance so we don't end up with
extern syntax_error ide_passback_error;

/// An std::string to handle allocation and free for the error string passed to the IDE.
extern string error_sstring;

#endif // SYNTAX_ERROR_H_INCLUDED
