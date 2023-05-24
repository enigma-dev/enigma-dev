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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef JDI_GENERAL_DEBUG_MACROS_h
#define JDI_GENERAL_DEBUG_MACROS_h

/// Render an AST to an SVG, if debug AST rendering is enabled.
/// @param AST The jdi::AST to render.
/// @param cat The category under which the AST is rendered, such as "ArrayBounds".

#define render_ast(AST, cat) // Do nothing

#ifdef DEBUG_MODE

#ifndef DEBUG_OUTPUT_PATH
#define DEBUG_OUTPUT_PATH "/home/josh/Desktop"
#endif

#include <assert.h>
#define dbg_assert(x) assert(x)
#define DBG_TERNARY(x,y) x

#include <API/AST_forward.h>
void render_ast_nd(jdi::AST& ast, std::string cat);

namespace jdi_debug {

class RecursionHelper {
  size_t &count;
 public:
  RecursionHelper(size_t &static_variable, size_t maximum_recursions):
      count(static_variable) {
    if (++count > maximum_recursions) asm("int3");
  }
  ~RecursionHelper() { --count; }
};

}

#define SUPERMACROCAT(x, y) x ## y
#define MACROCAT(x, y) SUPERMACROCAT(x, y)
#define SET_MAXIMUM_RECURSIONS(k)                                              \
  constexpr size_t MACROCAT(max_recursions_, __LINE__) = (k);                  \
  static size_t MACROCAT(current_recursions_, __LINE__) = 0;                   \
  jdi_debug::RecursionHelper MACROCAT(recursion_helper_, __LINE__)(            \
      MACROCAT(current_recursions_, __LINE__),                                 \
      MACROCAT(max_recursions_, __LINE__))

#else  // NOT DEBUG_MODE

#define dbg_assert(x)
#define DBG_TERNARY(x,y) y
#define render_ast_nd(x, y) void()
#define SET_MAXIMUM_RECURSIONS(k)

#endif  // DEBUG_MODE ELSE

#if defined(RENDER_ASTS) && defined(JDI_API_AST_h_debug)
  #include <string>
  #undef render_ast
  void render_ast(jdi::AST& ast, std::string cat);
#endif

#endif  // Guard
