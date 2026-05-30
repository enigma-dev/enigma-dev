/**
 * @file  error_reporting.h
 * @brief Header declaring a base class for error and warning reporting.
 *
 * Also defines a default error handling class, which shall write all warnings
 * and errors to stderr.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef ERROR_REPORTING__H
#define ERROR_REPORTING__H

#include <string>

namespace jdi {
  /// Abstract class for error handling and warning reporting.
  /// Implement this class yourself, or use \c default_error_handler.
  struct error_handler {
    /** Method invoked when an error occurs. As much detail about the location of the error
        as possible is sent to the method. If information is unavailable, due to build flags
        of the system or to circumstances by which the error arose, the default values in the
        prototype are passed.
    **/
    virtual void error(std::string err, std::string filename = "", int line = -1, int pos = -1) = 0;
    /** Method invoked when an error occurs. As much detail about the location of the warning
        as possible is sent to the method. If information is unavailable, due to build flags
        of the system or to circumstances by which the error arose, the default values in the
        prototype are passed.
    **/
    virtual void warning(std::string err, std::string filename = "", int line = -1, int pos = -1) = 0;
    /// Virtual destructor in case children have additional data types to free.
    virtual ~error_handler();
  };

  /// Default class for error handling and warning reporting.
  /// Prints all errors and warnings to stderr alike. No other action is taken.
  struct default_error_handler: error_handler {
    unsigned error_count, warning_count;
    /// Prints the error to stderr, in the format "ERROR[(<file>[:<line>[:<pos>]])]: <error string>"
    void error(std::string err, std::string filename = "", int line = -1, int pos = -1);
    /// Prints the error to stderr, in the format "Warning[(<file>[:<line>[:<pos>]])]: <warning string>"
    void warning(std::string err, std::string filename = "", int line = -1, int pos = -1);
    default_error_handler();
  };

  /// A pointer to an instance of \c default_error_handler, for use wherever.
  extern default_error_handler *def_error_handler;
}
#endif
