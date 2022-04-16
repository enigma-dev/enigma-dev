/**
 * @file  handle_function_impl.cpp
 * @brief Source implementing a function that skips function bodies in code,
 *        returning nullptr.
 * 
 * JustDefineIt implements a function that skips function bodies in code, returning
 * nullptr. Other applications can implement their own handler, replacing the existing
 * one and returning a pointer to the parsed function data which will be stored in
 * the \c definition_function for later use.
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

#include "handle_function_impl.h"
using namespace jdi;

static void* code_ignorer(lexer *lex, token_t &token, definition_scope *, ErrorHandler *herr) {
  if (token.type == TT_LEFTBRACE) {
    for (size_t bc = 0;;) {
      if (token.type == TT_LEFTBRACE) ++bc;
      else if (token.type == TT_RIGHTBRACE and !--bc) return nullptr;
      token = lex->get_token();
      if (token.type == TT_ENDOFCODE) {
        token.report_errorf(herr, "Expected closing brace to code before %s");
        return nullptr;
      }
    }
  }
  else if (token.type == TT_ASM) {
    do token = lex->get_token();
    while (token.type != TT_RIGHTPARENTH);
    token = lex->get_token();
  }
  return nullptr;
}
static void *initializer_ignorer(lexer *lex, token_t &token, definition_scope *scope, ErrorHandler *herr) {
  do {
    token = lex->get_token_in_scope(scope);
    if (token.type == TT_SEMICOLON || token.type == TT_ENDOFCODE) {
      token.report_error(herr, "Expected constructor body here after initializers.");
      return nullptr;
    }
  } while (token.type != TT_LEFTBRACE);
  return nullptr;
}
static void do_nothing(void*) {}

void* (*handle_function_implementation)(lexer *lex, token_t &token, definition_scope *scope, ErrorHandler *herr) = code_ignorer;
void* (*handle_constructor_initializers)(lexer *lex, token_t &token, definition_scope *scope, ErrorHandler *herr) = initializer_ignorer;
void  (*delete_function_implementation)(void* impl) = do_nothing;
void  (*delete_constructor_initializers)(void* impl) = do_nothing;
