/**
 * @file handle_declarators.cpp
 * @brief Source implementing the parser function to handle standard declarations.
 * 
 * This file's function will be referenced by every other function in the
 * parser. The efficiency of its implementation is of crucial importance.
 * If this file runs slow, so do the others in the parser.
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

#include <Parser/bodies.h>
#include <API/context.h>
#include <General/parse_basics.h>
#include <General/debug_macros.h>
#include <Parser/parse_context.h>
#include <cstdio>
using namespace jdip;
using namespace jdi;

int jdip::context_parser::handle_declarators(definition_scope *scope, token_t& token, unsigned inherited_flags)
{
  // Outsource to read_type, which will take care of the hard work for us.
  // When this function finishes, per its specification, our token will be set to the next relevant, non-referencer symbol.
  // This means an identifier if the syntax is correct.
  full_type tp = read_type(lex, token, scope, this, herr);
  
  // Make sure we actually read a valid type.
  if (!tp.def) {
    token.report_error(herr, "Declaration does not give a valid type");
    return 1;
  }
  
  after_comma:
  
  // Make sure we do indeed find ourselves at an identifier to declare.
  if (tp.refs.name.empty())
    return 0;
  
  // Add it to our definitions map, without overwriting the existing member.
  definition_scope::inspair ins = ((definition_scope*)scope)->members.insert(definition_scope::entry(tp.refs.name,NULL));
  if (ins.second) { // If we successfully inserted,
    ins.first->second = new definition_typed(tp.refs.name,scope,tp.def,tp.refs,tp.flags,DEF_TYPED | inherited_flags);
  }
  #ifndef NO_ERROR_REPORTING
  else // Well, uh-oh. We didn't insert anything. This is non-fatal, and will not leak, so no harm done.
  {
    if (not(ins.first->second->flags & DEF_TYPED)) {
      token.report_error(herr, "Redeclaration of `" + tp.refs.name + "' as a different kind of symbol");
      return 3;
    }
    if (not(ins.first->second->flags & DEF_TYPED) & DEF_EXTERN) { //TODO: Implement
      token.report_error(herr, "Redeclaration of non-extern `" + tp.refs.name + "' as non-extern");
      return 4;
    }
  }
  #endif
  
  for (;;)
  {
    switch (token.type) {
      case TT_OPERATOR:
          if (*token.extra.content.str != '=' or token.extra.content.len != 1) { // If this operator isn't =, this is a fatal error. No idea where we are.
            case TT_GREATERTHAN: case TT_LESSTHAN:
            token.report_error(herr, "Unexpected operator " + string((const char*)token.extra.content.str,token.extra.content.len) + " at this point");
            return 5;
          }
          else {
            // If this thing's const, we need to make note of the value.
            value a = read_expression(token, TT_SEMICOLON, scope);
            if (a.type != VT_NONE) {
              // TODO: Store value
            }
          }
        break;
      case TT_COMMA:
          // Move past this comma
          token = read_next_token(scope);
          
          // Read a new type
          read_referencers(tp.refs, lex, token, scope, this, herr);
          
          // Just hop into the error checking above and pass through the definition addition again.
        goto after_comma;
      
      case TT_STRINGLITERAL: case TT_DECLITERAL: case TT_HEXLITERAL: case TT_OCTLITERAL:
          token.report_error(herr, "Expected initializer `=' here before literal.");
        return 5;
      
      case TT_SEMICOLON:
      
      case TT_DECLARATOR: case TT_DECFLAG: case TT_CLASS: case TT_STRUCT: case TT_ENUM: case TT_UNION: case TT_NAMESPACE: case TT_EXTERN: case TT_IDENTIFIER:
      case TT_TEMPLATE: case TT_TYPENAME: case TT_TYPEDEF: case TT_USING: case TT_PUBLIC: case TT_PRIVATE: case TT_PROTECTED: case TT_COLON:
      case TT_SCOPE: case TT_LEFTPARENTH: case TT_RIGHTPARENTH: case TT_LEFTBRACKET: case TT_RIGHTBRACKET: case TT_LEFTBRACE: case TT_RIGHTBRACE:
      case TT_ASM: case TT_TILDE: case TTM_CONCAT: case TTM_TOSTRING: case TT_ENDOFCODE: case TT_INVALID: default:
        return 0;
    }
  }
  
  return 0;
}

