/**
 * @file jdi_utility.h
 * @brief File declaring utility functions to pluck extra information from
 *        JDI constructs.
 * 
 * This file was written as a helper set in plugging JDI into the old parser.
 * 
 * @section License
 * Copyright (C) 2011-2012 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _JDI_UTILITY__H
#define _JDI_UTILITY__H

#include <string>
#include <Storage/definition.h>
#include <API/error_reporting.h>
using std::string;


struct compile_error_handler: jdi::error_handler {
  static inline void basic(const char* level, string msg, string file, int l, int p);
  void error  (string err,  string file, int l, int p);
  void warning(string warn, string file, int l, int p);
};

int referencers_varargs_at(jdi::ref_stack &refs);
static inline bool referencers_varargs(jdi::ref_stack &refs) {
  return referencers_varargs_at(refs) != -1; // I'm afraid to put ~referencers_varargs because (1) the optimizer should do that and (2) I have no idea what C++ guarantees
}
/// Read parameter bounds from the current definition into args min and max. For variadics, max = unsigned(-1).
void definition_parameter_bounds(jdi::definition *d, unsigned &min, unsigned &max);
/// Create a standard variable member in the given scope.
void quickmember_variable(jdi::definition_scope* scope, jdi::definition* type, string name);
/// Create a script with the given name (and an assumed 16 parameters, all defaulted) to the given scope.
void quickmember_script(jdi::context *ctex, jdi::definition_scope* scope, string name);

#endif
