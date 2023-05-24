/**
 * @file  error_reporting.cpp
 * @brief Source implementing the default error handling class.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <iostream>
#include <API/error_reporting.h>

namespace jdi {

std::string SourceLocation::to_string() const {
  std::string res;
  if (!filename.empty()) {
    res += filename;
    if (line != npos) {
      res += ":" + std::to_string(line);
      if (pos != npos) {
        res += ":" + std::to_string(pos);
      }
    }
  }
  return res;
}

void DefaultErrorHandler::error(std::string_view err,
                              SourceLocation code_point) {
const std::string cps = code_point.to_string();
if (cps.empty()) {
  std::cerr << "ERROR: " << err << std::endl;
} else {
  std::cerr << "ERROR(" << cps << "): " << err << std::endl;
}
++error_count;
}
void DefaultErrorHandler::warning(std::string_view warn,
                                SourceLocation code_point) {
const std::string cps = code_point.to_string();
if (cps.empty()) {
  std::cerr << "Warning: " << warn << std::endl;
} else {
  std::cerr << "Warning(" << cps << "): " << warn << std::endl;
}
++warning_count;
}
void DefaultErrorHandler::info(std::string_view msg, int,
                             SourceLocation code_point) {
const std::string cps = code_point.to_string();
if (cps.empty()) {
  std::cerr << "Info: " << msg << std::endl;
} else {
  std::cerr << "Info(" << cps << "): " << msg << std::endl;
}
}

DefaultErrorHandler::DefaultErrorHandler():
  error_count(0), warning_count(0) {}

/// The instance of `DefaultErrorHandler` to which `default_error_handler`
/// will point.
static DefaultErrorHandler deh_instance;
DefaultErrorHandler *default_error_handler = &deh_instance;

}  // namespace jdi
