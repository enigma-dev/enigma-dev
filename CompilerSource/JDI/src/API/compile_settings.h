/**
 * @file  compile_settings.h
 * @brief Header containing basic settings for the parse sequence.
 *
 * @section License
 * 
 * Copyright (C) 2012 Josh Ventura
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
 
/// True if comments ending in backslashes should continue onto the next line.
#define ALLOW_MULTILINE_COMMENTS 1

/// True if errors are to be treated as fatal (end the parse)
#define FATAL_ERRORS 0
/// Optimized verion of (FATAL_ERRORS? x : y)
#define FATAL_ERRORS_T(x,y) (x)
/// Conditional return: Return if ( FATAL_ERRORS ).
#define FATAL_RETURN(ret_value) { return ret_value; }
/// Conditional return: Return if ( FATAL_ERRORS and ( expression ) ).
#define FATAL_RETURN_IF(expression,ret_value) if (expression) return ret_value; else void()
/// Conditional: first param #if FATAL_ERRORS, second otherwise.
#define  FATAL_TERNARY(fatal,nonfatal) fatal
/// Conditional: Make gone if !FATAL_ERRORS.
#define  IF_FATAL(fatal) fatal

#if !FATAL_ERRORS
  #undef  FATAL_ERRORS_T
  #undef  FATAL_RETURN
  #undef  FATAL_RETURN_IF
  #undef  FATAL_TERNARY
  #undef  IF_FATAL
  #define FATAL_ERRORS_T(x,y) (y)
  #define FATAL_RETURN(ret_value) void()
  #define FATAL_RETURN_IF(expression,ret_value) void(expression)
  #define FATAL_TERNARY(fatal,nonfatal) nonfatal
  #define IF_FATAL(fatal) fatal
#endif
