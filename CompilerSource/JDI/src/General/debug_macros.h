/**
 * @file  debug_macros.h
 * @brief A header declaring conditional macros for your parser debugging convenience.
 * 
 * This file declares macros for reporting loop iterations, stack traces, and
 * other information that would otherwise need to be acquired via a debugger.
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

/// Render an AST to an SVG, if debug AST rendering is enabled.
/// @param AST The jdi::AST to render.
/// @param cat The category under which the AST is rendered, such as "ArrayBounds".

#ifndef _DEBUG_MACROS__H
#define _DEBUG_MACROS__H

#define render_ast(AST, cat) //Do nothing

#ifdef DEBUG_MODE
  
  #ifndef DEBUG_OUTPUT_PATH
    #define DEBUG_OUTPUT_PATH "/home/josh/Desktop"
  #endif
  
  #include <assert.h>
  #define dbg_assert(x) assert(x)
  #define DBG_TERNARY(x,y) x
  
  #include <API/AST_forward.h>
  void render_ast_nd(jdi::AST& ast, std::string cat);

#else // NOT DEBUG_MODE

  #define dbg_assert(x)
  #define DBG_TERNARY(x,y) y
  #define render_ast_nd(x, y) void()

#endif // DEBUG_MODE ELSE
#endif // Guard

#ifndef _DEBUG_MACROS__H__RENDER_ASTS
  #if defined(RENDER_ASTS) && defined(_AST__H__DEBUG)
    #define _DEBUG_MACROS__H__RENDER_ASTS
    #include <string>
    #undef render_ast
    void render_ast(jdi::AST& ast, std::string cat);
  #endif
#endif

