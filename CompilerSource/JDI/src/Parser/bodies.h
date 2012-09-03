/**
 * @file  bodies.h
 * @brief A header declaring all token handler functions for use by the parser.
 * 
 * The class named in this file is meant to extend \c context, giving it a full
 * set of parser functions.
 * 
 * @section License
 * 
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
 *
 * @section Details
 * 
 * This file declares two types of functions, hereafter referred to as "handlers"
 * and "readers." Handlers are big-picture functions meant to do any work or 
 * additional delegation required to accomplish a task, while "readers" are meant
 * to work with simple token patterns. For instance, handle_declarators() would
 * take care of parsing lines such as int a = 10, and might delegate to functions
 * such as read_fulltype(), which would read type info from a string of tokens. While
 * Readers still do a fair amount of delegation, they are responsible for more of
 * the grunt work involved in parsing a file.
 * 
 * @section Handlers
 * 
 * As a general trend for handler functions, all parameters are in in-out mode.
 * The input \p cfile will have its position changed as more tokens are read in,
 * and may hop files in the process. In that case, \p pcm the parse context, will
 * be modified to reflect the new 'open files' stack. If the function takes a \p
 * token parameter, the token may or may not be modified to reflect the first
 * unhandled token read. The \p scope parameter is a pointer and is not modified
 * directly; instead, the contents pointed at are modified as definitions are
 * added or removed.
 * 
 * @section Readers
 * 
 * Readers will also tend to have in-out mode parameters, but are more likely
 * to modify the input \p token to return the next unhandled token. In general,
 * readers aren't interested in the scope being parsed into, or about any state
 * information. Their purpose is simply to interpret the input file.
 * 
**/

#ifndef _PARSE_BODIES__H
#define _PARSE_BODIES__H

#include <API/context.h>
#include <System/token.h>
#include <API/lexer_interface.h>
#include <Storage/definition.h>
#include <Storage/value.h>
#include "parse_context.h"

namespace jdip {
  /**
    Read a complete type from the given input stream.
    
    This function is a reader. Many inputs are liable to be modified in some form or another.
    See \section Readers for details.
    
    When the read_fulltype function is invoked, the passed token must be a declarator of some sort.
    It is up to the calling method to ensure this.
    
    When the read_fulltype function terminates, the passed token will have been set to the first
    unhandled token, meaning it will NOT have the types \c TT_DECLARATOR, \c TT_DECFLAG, \c
    TT_LEFTBRACKET, \c TT_LEFTPARENTH, or \c TT_IDENTIFIER. Each of those is handled before
    the termination of this function, either in the function itself or in a call to
    \c read_referencers. If a name is specified along with the type, it will be copied into
    the `referencers` member of the resulting \c full_type.
    
    @param  lex    The lexer to be polled for tokens. [in-out]
    @param  token  The token for which this function was invoked.
                   The type of this token must be either \c TT_DECLARATOR or \c TT_DECFLAG. [in-out]
    @param  scope  The scope used to resolve identifiers. [in]
    @param  herr   The error handler which will be used to report errors. [in]
    
    @return Returns the \c full_type read from the stream. Leaves \p token indicating the
            first unhandled token.
  **/
  full_type read_fulltype(lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr = def_error_handler);
  /**
    Read a complete type from the given input stream.
    
    This function is a reader. Many inputs are liable to be modified in some form or another.
    See \section Readers for details.
    
    When the read_fulltype function is invoked, the passed token must be a declarator of some sort.
    It is up to the calling method to ensure this.
    
    When the read_fulltype function terminates, the passed token will have been set to the first
    unhandled token, meaning it will NOT have the types \c TT_DECLARATOR or \c TT_DECFLAG. 
    Each of those is handled before the termination of this function. No referencers are read,
    no name is read. Only flags and type specifiers.
    
    @param  lex    The lexer to be polled for tokens. [in-out]
    @param  token  The token for which this function was invoked.
                   The type of this token must be either \c TT_DECLARATOR or \c TT_DECFLAG. [in-out]
    @param  scope  The scope used to resolve identifiers. [in]
    @param  herr   The error handler which will be used to report errors. [in]
    
    @return Returns the \c full_type read from the stream. Leaves \p token indicating the
            first unhandled token.
  **/
  full_type read_type(lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr = def_error_handler);
  /**
    Read a series of referencers from the given input stream.
    
    This function is a reader. Many inputs are liable to be modified in some form or another.
    See \section Readers for details.
    
    This method will read a complete rf_stack from the given input stream. This includes pointer-to asterisks (*),
    reference ampersands (&), array bounds in square brackets [], and function parameters in parentheses (). Parentheses
    used for the purpose of putting precedence on a given referencer will be handled, but will not be literally denoted
    in the returned stack.
    
    @param  refs   The ref_stack onto which referencers should be pushed. [out]
    @param  ft     The type which was read before asking for a reference stack. Used to
                   distinguish constructors from ugly definitions. [in]
    @param  lex    The lexer to be polled for tokens. [in-out]
    @param  token  The token for which this function was invoked. If the given token is a
                   type, it will be part of the return \c full_type, otherwise it will
                   just be overwritten. [in-out]
    @param  scope  The scope used to resolve identifiers. [in]
    @param  cp     The context parser to be polled for any additional information. [in]
    @param  herr   The error handler which will be used to report errors. [in]
    @return  Returns 0 on success, or a non-zero error state otherwise. You do not need to act on this
             error state, as the error will have already been reported to the given error handler.
  **/
  int read_referencers(ref_stack& refs, const full_type &ft, lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr = def_error_handler);
  /**
    Read function parameters into a \c ref_stack from an input stream. This function should be invoked with the first
    token following the opening parentheses, and will terminate with the first token after the closing parentheses.
     
    This function is a reader. Many inputs are liable to be modified in some form or another.
    See \section Readers for details.
    
    This method will read a complete rf_stack from the given input stream. This includes pointer-to asterisks (*),
    reference ampersands (&), array bounds in square brackets [], and function parameters in parentheses (). Parentheses
    used for the purpose of putting precedence on a given referencer will be handled, but will not be literally denoted
    in the returned stack.
    
    @param  refs   The ref_stack onto which referencers should be pushed. [out]
    @param  lex    The lexer to be polled for tokens. [in-out]
    @param  token  The token for which this function was invoked. If the given token is a
                   type, it will be part of the return \c full_type, otherwise it will
                   just be overwritten. [in-out]
    @param  scope  The scope used to resolve identifiers. [in]
    @param  cp     The context parser to be polled for any additional information. [in]
    @param  herr   The error handler which will be used to report errors. [in]
    @return  Returns 0 on success, or a non-zero error state otherwise. You do not need to act on this
             error state, as the error will have already been reported to the given error handler.
  **/
  int read_function_params(ref_stack& refs, lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr = def_error_handler);
  /**
    Read the latter half of referencers, as handled in read_referencers.
    
    @param  refs   The ref_stack onto which referencers should be pushed. [out]
    @param  lex    The lexer to be polled for tokens. [in-out]
    @param  token  The token for which this function was invoked. If the given token is a
                   type, it will be part of the return \c full_type, otherwise it will
                   just be overwritten. [in-out]
    @param  scope  The scope used to resolve identifiers. [in]
    @param  cp     The context parser to be polled for any additional information. [in]
    @param  herr   The error handler which will be used to report errors. [in]
    @return  Returns 0 on success, or a non-zero error state otherwise. You do not need to act on this
             error state, as the error will have already been reported to the given error handler.
  **/
  int read_referencers_post(ref_stack& refs, lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr = def_error_handler);
  /**
    Read a list of template parameters from the given input stream.
    
    This function is a reader. Many inputs are liable to be modified in some form or another.
    See \section Readers for details.
    
    This method will read from the opening '<' token (which must be the active token passed)
    to the corresponding '>' token, populating the given arg_key structure.
    
    @param  argk   The arg_key into which the parameters will be copied. The
                   key must be initialized with the parameter count of the given template. [in-out]
    @param  temp   The template definition for which argument data will be read.
    @param  lex    The lexer to be polled for tokens. [in-out]
    @param  token  The token for which this function was invoked. If the given token is a
                   type, it will be part of the return \c full_type, otherwise it will
                   just be overwritten. [in-out]
    @param  scope  The scope used to resolve identifiers. [in]
    @param  cp     The context parser to be polled for any additional information. [in]
    @param  herr   The error handler which will be used to report errors. [in]
    @return  Returns 0 on success, or a non-zero error state otherwise. You do not need to act on this
             error state, as the error will have already been reported to the given error handler.
  **/
  int read_template_parameters(arg_key &argk, definition_template *temp, lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr = def_error_handler);
  
  /**
    Handle accessing dependent types and members. Shoves the definitions into the
    template for fix later.
    
    This function is a complete handler. All inputs are liable to be modified.
    See \section Handlers for details.
    
    @param  lex    The lexer to poll for tokens. [in-out]
    @param  scope  The scope from which the member is being accessed. [in-out]
    @param  token  The first token to handle, and the token structure into which the next unhandled token will be placed. [in-out]
    @param  flags  Flags known about this hypothetical type. [in]
    @param  herr   Error handler to which errors will be reported. [in-out]
    
    @return A representation of the dependent member, or NULL if an error occurred.
  **/
  definition_hypothetical* handle_hypothetical(lexer *lex, definition_scope *scope, token_t& token, unsigned flags, error_handler *herr);
  
  /**
    Read a string from code containing the name of an operator function, eg, `operator*', `operator[]', `operator.', `operator new[]'.
    @param  lex   The lexer to poll for tokens.
    @param token  The token which sparked this function call. Should be TT_OPERATORKW.
    @param scope  The scope from which class names will be read for class casts.
    @param herr   An error handler to receive any error reports.
    @return The string containing the legible name of the operator function.
  */
  string read_operatorkw_name(lexer* lex, token_t &token, definition_scope *scope, error_handler *herr);
  
  /**
    Read a definition from a string of scope qualifiers.
    @param lex    The lexer to poll for tokens.
    @param scope  The scope from which class names will be read for class casts.
    @param token  The token which sparked this function call. Should be TT_OPERATORKW.
    @param cp     The context parser to be polled for any additional information.
    @param herr   An error handler to receive any error reports.
    @return The string containing the legible name of the operator function.
  */
  definition* read_qualified_definition(lexer *lex, definition_scope* scope, token_t &token, context_parser *cp, error_handler *herr);
  
  extern definition* dangling_pointer;
  /**
    @class context_parser
    @brief A field-free utility class extending \c context, implementing the
           recursive-descent functions needed by the parser.
    
    This is some heavy shit.
  **/
  struct context_parser: jdi::context
  {
    /**
      Read in the next token, handling any preprocessing.
      
      This function wraps to two functions which together do a huge amount of work for
      you; in this one call to retrieve the next token, the lexer may skip hundreds of
      lines of code, enter a new file, or perform a number of other operations. The
      second call will then check the token against the given scope to find definitions.
      What is returned is ready to be used as-is, except if followed by the scope op.
      
      @param  scope  The scope from which identifiers will be looked up. [in]
      @return The next token in the stream.
    **/
    token_t read_next_token(definition_scope *scope);
    
    /**
      Parse a list of declarations, copying them into the given scope.
      
      This function is a complete handler. All inputs are liable to be modified.
      See \section Handlers for details.
      
      @param  scope  The scope in which declarations will be stored. [in-out]
      @param  token  The token that was read before this function was invoked.
                     This will be updated to represent the next non-type token
                     in the stream. [in-out]
      @param inherited_flags Any flags which should be given to each declared definition.
      @param res     A pointer to receive the last definition declared. [out]
      
      @return Zero if no error occurred, a non-zero exit status otherwise.
    **/
    int handle_declarators(definition_scope *scope, token_t& token, unsigned inherited_flags, definition* &res = dangling_pointer);
    /**
      Parse a list of declarations assuming the given type, copying them into the given scope.
      
      This function is a complete handler. All inputs are liable to be modified.
      See \section Handlers for details.
      
      @param  scope  The scope in which declarations will be stored. [in-out]
      @param  token  The token that was read before this function was invoked.
                     This will be updated to represent the next non-type token
                     in the stream. [in-out]
      @param type    The type we assume was already read. Referencers will be toasted if a comma is encountered. [in-out]
      @param inherited_flags Any flags which should be given to each declared definition.
      @param res     A pointer to receive the last definition declared. [out]
      
      @return Zero if no error occurred, a non-zero exit status otherwise.
    **/
    int handle_declarators(definition_scope *scope, token_t& token, full_type& type, unsigned inherited_flags, definition* &res = dangling_pointer);
    
    /**
      Parse a namespace definition.
      
      This function is a complete handler. All inputs are liable to be modified.
      See \section Handlers for details.
      
      @param  scope  The scope in which declarations will be stored. [in-out]
      @param  token  The token that was read before this function was invoked.
                     At the start of this call, the type of this token must be
                     TT_NAMESPACE. Upon termination, the type of this token will
                     be TT_RIGHTBRACE unless an error occurs. [in-out]
      
      @return Zero if no error occurred, a non-zero exit status otherwise.
    **/
    int handle_namespace(definition_scope *scope, token_t& token);
    
    /**
      Parse a class or struct definition.
      
      This function is a complete handler. All inputs are liable to be modified.
      See \section Handlers for details.
      
      @param  scope  The scope in which declarations will be stored. [in-out]
      @param  token  The token that was read before this function was invoked.
                     At the start of this call, the type of this token must be
                     either TT_CLASS or TT_STRUCT. Upon termination, the type
                     of this token will be TT_DECLARATOR with extra info set to
                     the new definition unless an error occurs. [in-out]
      @param inherited_flags  The flags that will be given to the class, if a new
                              definition is instantiated for it. These flags are
                              NOT given to memebers of the class.
      
      @return The enum created/referenced, or NULL if some unrecoverable error occurred.
    **/
    definition_class* handle_class(definition_scope *scope, token_t& token, int inherited_flags);
    
    /**
      Parse a union definition.
      
      This function is a complete handler. All inputs are liable to be modified.
      See \section Handlers for details.
      
      @param  scope  The scope in which declarations will be stored. [in-out]
      @param  token  The token that was read before this function was invoked.
                     At the start of this call, the type of this token must be
                     either TT_CLASS or TT_STRUCT. Upon termination, the type
                     of this token will be TT_DECLARATOR with extra info set to
                     the new definition unless an error occurs. [in-out]
      @param inherited_flags  The flags that will be given to the class, if a new
                              definition is instantiated for it. These flags are
                              NOT given to memebers of the class.
      
      @return The union created/referenced, or NULL if some unrecoverable error occurred.
    **/
    jdi::definition_union* handle_union(definition_scope *scope, token_t& token, int inherited_flags);
    
    /**
      Parse an enumeration. Reads the enumeration and its constants into the given scope.
      
      This function is a complete handler. All inputs are liable to be modified.
      See \section Handlers for details.
      
      @param  scope  The scope in which declarations will be stored. [in-out]
      @param  token  The token that was read before this function was invoked.
                     At the start of this call, the type of this token must be
                     either TT_CLASS or TT_STRUCT. Upon termination, the type
                     of this token will be TT_DECLARATOR with extra info set to
                     the new definition unless an error occurs. [in-out]
      @param inherited_flags  The flags that will be given to the class, if a new
                              definition is instantiated for it. These flags are
                              NOT given to memebers of the class.
      
      @return The enum created/referenced, or NULL if some unrecoverable error occurred.
    **/
    definition_enum* handle_enum(definition_scope *scope, token_t& token, int inherited_flags);
    
    /**
      Handle parsing an entire scope.
      
      This function is a complete handler. All inputs are liable to be modified.
      See \section Handlers for details.
      
      @param  scope  The scope into which declarations will be stored. [in-out]
      @param  token  The token structure into which the next unhandled token will be placed. [out]
      @param  inherited_flags  Any flags which must be given to all members of this scope. [in]
      
      @return Zero if no error occurred, a non-zero exit status otherwise.
    **/
    int handle_scope(definition_scope *scope, token_t& token, unsigned inherited_flags = 0);
    
    /**
      Handle parsing a template declaration.
      
      This function is a complete handler. All inputs are liable to be modified.
      See \section Handlers for details.
      
      @param  scope  The scope into which declarations will be stored. [in-out]
      @param  token  The token structure into which the next unhandled token will be placed. [out]
      @param  inherited_flags  Any flags which must be given to all members of this scope. [in]
      
      @return Zero if no error occurred, a non-zero exit status otherwise.
    **/
    int handle_template(definition_scope *scope, token_t& token, unsigned inherited_flags = 0);
    
    /**
      Read an expression from the given input stream, evaluating it for a value.
      
      This function is a reader. Many inputs are liable to be modified in some form or another.
      See \section Readers for details.
      
      The read_expression function will retrieve tokens from \p cfile until the stream ends,
      a comma is reached, or a token denoted by closing_token is reached. If a semicolon is
      encountered and \p closing_token is not \c TT_SEMICOLON, the function will return error.
      
      When the read_expression function finishes, \p token will be set to the first unhandled
      token in the stream. If the expression cannot be evaluated, the type of the returned \c
      value will be set to \c VT_NOTHING.
      
      @param  token          The \c token structure which will represent the first non-evaluated token. [out]
      @param  closing_token  The \c TOKEN_TYPE of an additional token which will close this expression. [in]
      @param  scope          The scope which may be passed to \c read_token. [in]
      
      @return Returns the \c full_type read from the stream.
    **/
    value read_expression(token_t &token, TOKEN_TYPE closing_token, definition_scope *scope);
    
    /**
      Retrieve the type of a token from a given string in a given scope.
      Traverses scopes backward, searching for the given string in the nearest scope.
      
      @param  scope  The scope in which to begin searching.
      @param  name   The name of the definition for which to search.
      @param  def    The default token to be returned if no other definition is found.
      
      @return  A token representing the found result. For instance, given name = "int", a
               token type \c TT_DECLARATOR would be returned.
    **/
    static token_t look_up_token(definition_scope* scope, string name, token_t def);
  };
}

#endif
