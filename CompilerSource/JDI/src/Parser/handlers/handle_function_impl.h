/**
 * @file  handle_function_impl.h
 * @brief Header to allow external APIs to parse C++ or assembly code for
 *        a function definition.
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

#ifndef _HANDLE_FUNCTION_IMPLEMENTATIONS__H
#define _HANDLE_FUNCTION_IMPLEMENTATIONS__H

#include <System/lex_cpp.h>

/**
  Function pointer to handle parsing function code content, either a C++ function
  body between matching braces, or an assembly block given by an __asm__ token.
  
  In the former case, this function will be invoked with token.type = TT_LEFTBRACE;
  in the latter case, the function will be invoked with token.type = TT_ASM.
  
  @param lex    The lexer to use to poll for further tokens.
  @param token  The initial token which invoked this function call.
  @param scope  The scope from which definitions can be read.
  @param herr   The error handler to which errors can be reported.
*/
extern void* (*handle_function_implementation)(jdi::lexer *lex, jdi::token_t &token, jdi::definition_scope *scope, jdi::ErrorHandler *herr);
/**
  Function pointer to handle parsing constructor initializer lists.
  This function will be invoked with token.type = TT_COLON.
  This function should finish with token.type = TT_LEFTBRACE.
  
  @param lex    The lexer to use to poll for further tokens.
  @param token  The initial token which invoked this function call.
  @param scope  The scope from which definitions can be read.
  @param herr   The error handler to which errors can be reported.
*/
extern void* (*handle_constructor_initializers)(jdi::lexer *lex, jdi::token_t &token, jdi::definition_scope *scope, jdi::ErrorHandler *herr);

/**
  Function pointer to handle freeing function code content as allocated by a corresponding
  call to handle_function_implementation. Invoked on destruct of the owning function definition.
  
  @param impl  The implementation data, as returned by handle_function_implementation.
*/
extern void (*delete_function_implementation)(void *impl);

/**
  Function pointer to handle freeing initializer list content as allocated by a corresponding
  call to handle_constructor_initializers. Invoked on destruct of the owning function definition.
  
  @param impl  The implementation data, as returned by handle_constructor_initializers.
*/
extern void (*delete_constructor_initializers)(void *impl);

#endif
