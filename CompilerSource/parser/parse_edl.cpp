/**
 * @file parse_edl.cpp
 * @brief Source implementing the C++ \c lexer class extensions.
 * 
 * This file's function will be referenced, directly or otherwise, by every
 * other function in the parser. The efficiency of its implementation is of
 * crucial importance. If this file runs slow, so do the others.
 * 
 * @section License
 * 
 * Copyright (C) 2012-2013 Josh Ventura
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

#include "parse_edl.h"

namespace settings {
  bool pedantic_edl = true;
  bool pedantic_errors = false;
}

bool pedantic_warn(token_t &token, error_handler *herr, string w) {
  settings::pedantic_errors? token.report_error(herr, w) : token.report_warning(herr, w);
  return settings::pedantic_errors;
}

map<string, declaration> global_grabbag;

/// Definition representing an ENIGMA object; 
struct definition_object: definition_scope {
  map<egm_event, EDL_AST::AST_Node_Block> events; ///< The code for all events in this object
  vector<definition_object*> parents; ///< List of parents of this object
};

// ---------------------------------------------------------------------
// Constructor and destructor implementations --------------------------
// ---------------------------------------------------------------------

EDL_AST::AST_Node_Statement::AST_Node_Statement(): AST_Node() {}
EDL_AST::AST_Node_Statement::~AST_Node_Statement() {}

EDL_AST::AST_Node_Block::AST_Node_Block() {}
EDL_AST::AST_Node_Block::~AST_Node_Block() {}

EDL_AST::AST_Node_Declaration::AST_Node_Declaration(const full_type &ft): base_type(ft) {}

EDL_AST::AST_Node_Statement_case::AST_Node_Statement_case(AST_Node_Statement_switch *sw, AST_Node *cv): AST_Node_Statement_default(sw), value(cv) {}
EDL_AST::AST_Node_Statement_default::AST_Node_Statement_default(AST_Node_Statement_switch *sw): st_switch(sw) {}
EDL_AST::AST_Node_Statement_case::~AST_Node_Statement_case() { delete value; }

EDL_AST::AST_Node_Statement_if::AST_Node_Statement_if(AST_Node *cd, AST_Node_Statement *doif, AST_Node_Statement *doelse): condition(cd), do_if(doif), do_else(doelse) {}
EDL_AST::AST_Node_Statement_if::~AST_Node_Statement_if() { delete condition; delete do_if; delete do_else; }

EDL_AST::AST_Node_Statement_repeat::AST_Node_Statement_repeat(AST_Node *cond, AST_Node_Statement *c): condition(cond), code(c) {}
EDL_AST::AST_Node_Statement_repeat::~AST_Node_Statement_repeat() { delete condition; delete code; }

EDL_AST::AST_Node_Statement_do::AST_Node_Statement_do(AST_Node_Statement *loop, AST_Node *cond, bool n): code(loop), condition(cond), negate(n) {}
EDL_AST::AST_Node_Statement_do::~AST_Node_Statement_do() { delete code; delete condition; }

EDL_AST::AST_Node_Statement_for::AST_Node_Statement_for(AST_Node_Statement* stpre, AST_Node* c, AST_Node_Statement *stpost, AST_Node_Statement *cd): operand_pre(stpre), condition(c), operand_post(stpost), code(cd) {}
EDL_AST::AST_Node_Statement_for::~AST_Node_Statement_for() { delete operand_pre; delete condition; delete operand_post; delete code; }

EDL_AST::AST_Node_Statement_while::AST_Node_Statement_while(AST_Node *cond, AST_Node_Statement *wcode, bool neg): condition(cond), code(wcode), negate(neg) {}
EDL_AST::AST_Node_Statement_while::~AST_Node_Statement_while() { delete condition; delete code; }

EDL_AST::AST_Node_Statement_with::AST_Node_Statement_with(AST_Node *whom, AST_Node_Statement *wcode): instances(whom), code(wcode) {}
EDL_AST::AST_Node_Statement_with::~AST_Node_Statement_with() { delete instances; delete code; }

EDL_AST::AST_Node_Statement_trycatch::AST_Node_Statement_trycatch(AST_Node_Statement *c): code_try(c) {}
EDL_AST::AST_Node_Statement_trycatch::~AST_Node_Statement_trycatch() { delete code_try; }

EDL_AST::AST_Node_Statement_switch::AST_Node_Statement_switch(AST_Node *swv, AST_Node_Statement *swb): expression(swv), code(swb) {}
EDL_AST::AST_Node_Statement_switch::~AST_Node_Statement_switch() {}

// ---------------------------------------------------------------------
// ToString implementations --------------------------------------------
// ---------------------------------------------------------------------

// Default implementation; queries toString() with indent = 0
string EDL_AST::AST_Node_Statement::toString() const { return toString(0); }

string EDL_AST::AST_Node_Statement_Standard::toString(int indent) const {
  return string(indent, ' ') + statement->toString() + ";";
}

string EDL_AST::AST_Node_Block::toString(int indent) const {
  if (statements.size() > 1) {
    string res(indent, ' ');
    res += "{\n";
    for (size_t i = 0; i < statements.size(); ++i)
      res += statements[i]->toString(indent + 2) + "\n";
    return res + string(indent, ' ') + "}";
  }
  else {
    if (statements.empty())
      return "{}";
    return string(indent, ' ') + statements[0]->toString(indent + 2);
  }
}

string EDL_AST::AST_Node_Structdef::toString(int indent) const {
  return def->toString(-1, indent);
}
string EDL_AST::AST_Node_Enumdef::toString(int indent) const {
  return def->toString(-1, indent);
}

string EDL_AST::AST_Node_Declaration::toString(int indent) const {
  string res(indent, ' ');
  res += base_type.toString();
  const char *sep = " ";
  for (vector<declaration>::const_iterator it = declarations.begin(); it != declarations.end(); ++it) {
    res += sep + it->def->name;
    if (it->initialization) res += " = (", res += it->initialization->toString(), res += ")";
    sep = ", ";
  }
  return res + ";";
}

string EDL_AST::AST_Node_Statement_repeat::toString(int indent) const {
  return string(indent, ' ') + "repeat (" + condition->toString() + ") {\n" + code->toString(indent + 2) + "\n" + string(indent, ' ') + "}";
}
string EDL_AST::AST_Node_Statement_while::toString(int indent) const {
  return string(indent, ' ') + (negate? "until (" : "while (") + condition->toString() + ") {\n" + code->toString(indent + 2) + "\n" + string(indent, ' ') + "}";
}
string EDL_AST::AST_Node_Statement_case::toString(int indent) const {
  return string(indent, ' ') + "case " + value->toString() + ":";
}
string EDL_AST::AST_Node_Statement_default::toString(int indent) const {
  return string(indent, ' ') + "default:";
}
string EDL_AST::AST_Node_Statement_if::toString(int indent) const {
  string res(indent, ' '); res += "if (" + condition->toString() + ")\n";
  res += do_if? do_if->toString(indent + 2) : ";";
  if (do_else)
    res += "\n" + string(indent, ' ') + "else\n" + do_else->toString(indent + 2);
  return res;
}
string EDL_AST::AST_Node_Statement_do::toString(int indent) const {
  string res(indent, ' '); res += "do\n";
  res += code->toString(indent + 2);
  res += "\n" + string(indent, ' ') + (negate? "until (" : "while (") + condition->toString() + ");";
  return res;
}
string EDL_AST::AST_Node_Statement_for::toString(int indent) const {
  return "for (" + (operand_pre? operand_pre->toString() : " ") + "; " + (condition? condition->toString() : " ")
    + (operand_post? operand_post->toString() : " ")  + ")" + (code? "\n"+code->toString(indent + 2) : ";");
}
string EDL_AST::AST_Node_Statement_with::toString(int indent) const {
  return string(indent, ' ') + "with (" + (instances? instances->toString() : "noone") + ")\n" + code->toString(indent + 2);
}
string EDL_AST::AST_Node_Statement_trycatch::toString(int indent) const {
  string res = string(indent, ' ') + "try" + (code_try? "\n" + code_try->toString(indent + 2) : ";") + "\n";
  for (vector<catch_clause>::const_iterator it = catches.begin(); it != catches.end(); ++it)
    res += string(indent, ' ') + "catch (" + it->type_catch.toString() + ")" + (it->code_catch? "\n" + it->code_catch->toString() : ";");
  return res;
}
string EDL_AST::AST_Node_Statement_switch::toString(int indent) const {
  return string(indent, ' ') + "switch (" + (expression? expression->toString() : "") + ")" + (code? "\n" + code->toString(indent + 2) : ";");
}
string EDL_AST::AST_Node_Statement_return::toString(int indent) const {
  return string(indent, ' ') + "return" + (value? " " + value->toString() + ";" : ";");
}


// ---------------------------------------------------------------------
// SVG Rendering implementations --------------------------------------------
// ---------------------------------------------------------------------

void EDL_AST::AST_Node_Block::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Block::width()  {return 0;}
int EDL_AST::AST_Node_Block::height() {return 0;}

void EDL_AST::AST_Node_Statement_repeat::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Statement_repeat::width()  { return 0; }
int EDL_AST::AST_Node_Statement_repeat::height() { return 0; }

void EDL_AST::AST_Node_Statement_trycatch::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Statement_trycatch::width()  { return 0; }
int EDL_AST::AST_Node_Statement_trycatch::height() { return 0; }

void EDL_AST::AST_Node_Statement_with::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Statement_with::width()  { return 0; }
int EDL_AST::AST_Node_Statement_with::height() { return 0; }

void EDL_AST::AST_Node_Statement_switch::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Statement_switch::width()  { return 0; }
int EDL_AST::AST_Node_Statement_switch::height() { return 0; }

void EDL_AST::AST_Node_Statement_while::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Statement_while::width()  { return 0; }
int EDL_AST::AST_Node_Statement_while::height() { return 0; }

void EDL_AST::AST_Node_Statement_case::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Statement_case::width()  { return 0; }
int EDL_AST::AST_Node_Statement_case::height() { return 0; }

void EDL_AST::AST_Node_Statement_default::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Statement_default::width()  { return 0; }
int EDL_AST::AST_Node_Statement_default::height() { return 0; }

void EDL_AST::AST_Node_Statement_for::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Statement_for::width()  { return 0; }
int EDL_AST::AST_Node_Statement_for::height() { return 0; }

void EDL_AST::AST_Node_Statement_do::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Statement_do::width()  { return 0; }
int EDL_AST::AST_Node_Statement_do::height() { return 0; }

void EDL_AST::AST_Node_Statement_if::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Statement_if::width()  { return 0; }
int EDL_AST::AST_Node_Statement_if::height() { return 0; }

void EDL_AST::AST_Node_Statement_return::toSVG(int, int, SVGrenderInfo*) {}
int EDL_AST::AST_Node_Statement_return::width()  { return 0; }
int EDL_AST::AST_Node_Statement_return::height() { return 0; }

/* ****************************** ************************************************************** *\
** Actual parser code starts here ************************************************************** **
\* ****************************** ************************************************************** */

/// Fetch a loop from our stack which can be break'd.
EDL_AST::statement_ref *EDL_AST::loops_get_breakable(statement_kind mask) {
  for (loopstack::reverse_iterator it = loops.rbegin(); it != loops.rend(); ++it)
    if (it->kind & mask) return &*it;
  return NULL;
}
/// Fetch a loop from our stack of a certain type (must match @p kind exactly)
EDL_AST::statement_ref *EDL_AST::loops_get_kind(statement_kind kind) {
  for (loopstack::reverse_iterator it = loops.rbegin(); it != loops.rend(); ++it)
    if (it->kind == kind) return &*it;
  return NULL;
}

EDL_AST::AST_Node_Structdef *EDL_AST::handle_struct(token_t &) {
  AST_Node_Structdef* res = new AST_Node_Structdef();
  return res;
}

EDL_AST::AST_Node_Enumdef *EDL_AST::handle_enum(token_t &) {
  AST_Node_Enumdef* res = new AST_Node_Enumdef();
  return res;
}

EDL_AST::AST_Node_Declaration *EDL_AST::handle_declaration(token_t &token)
{
  bool isglobal = false;
  definition_scope *dscope = search_scope;
  if (token.type == TT_LOCAL)
    dscope = object_scope, token = get_next_token();
  else if (token.type == TT_GLOBAL)
    dscope = global_scope, token = get_next_token(), isglobal = true;
  full_type base_type = read_type(lex, token, search_scope, (context_parser*)main_context, herr);
  if (base_type.def == NULL)
    return NULL;
  AST_Node_Declaration *res = new AST_Node_Declaration(base_type);
  for (;;)
  {
    full_type nt(base_type);
    read_referencers(nt.refs, nt, lex, token, search_scope, (context_parser*)main_context, herr);
    if (nt.refs.name.empty()) {
      token.report_errorf(herr, "Variable name to declare expected before %s");
      delete res;
      return NULL;
    }
    definition_typed *decdef = new definition_typed(nt.refs.name, dscope, nt.def, nt.flags);
    dscope->declare(decdef->name, decdef);
    res->declarations.push_back(decdef);
    
    // Read initialization
    if (token.type == TT_OPERATOR and token.content.len == 1 and *token.content.str == '=') {
      AST *ast = new AST();
      token = get_next_token();
      ast->parse_expression(token, lex, precedence::comma + 1, herr);
      res->declarations.rbegin()->initialization = ast;
      if (isglobal) {
        declaration &d = global_grabbag[decdef->name];
        if (d.def != decdef) { cerr << "LOGIC HAS FAILED US" << endl; abort(); }
        if (decdef->flags & builtin_flag__const) {
          if (d.initialization/* and (*d.initialization != *ast)*/) { // TODO: AST <=> AST comparison operators
            token.report_error(herr, "Redefinition of global constant `" + decdef->name + "' in this code; TODO: Give previous definition");
          }
        }
      }
    }
    
    if (token.type == TT_COMMA)
      token = get_next_token();
    else break;
  }
  if (token.type == TT_SEMICOLON)
    token = get_next_token();
  else if (settings::pedantic_edl)
    pedantic_warn(token, herr, "Semicolon expected here following declaration");
  return res;
}

EDL_AST::AST_Node_Statement* EDL_AST::handle_statement(token_t &token) {
  for (;;) {
    switch (token.type) {
      case TT_UNION:
      case TT_STRUCT:
        return handle_struct(token);
      
      case TT_ENUM:
        return handle_enum(token);
      
      case TT_GLOBAL:
      case TT_LOCAL:
      case TT_TYPEDEF:
      case TT_DECFLAG:
      case TT_DECLTYPE:
      case TT_DECLARATOR:
        return handle_declaration(token);
      
      case TT_NAMESPACE:
        token.report_error(herr, "Keyword `namespace' out of nowhere; did you mean `using namespace'? (Namespace declarations not valid in a function)");
        return NULL;
      case TT_USING:
        token.report_error(herr, "FIXME: Unimplemented! This feature is coming soon.");
        return NULL;
        break;
      
      // Assigning an identifier or calling a function
      case TT_IDENTIFIER:
      case TT_DEFINITION:
      // Numbers can be used to access variables out of scope
      case TT_DECLITERAL:
      case TT_HEXLITERAL:
      case TT_OCTLITERAL:
      case TT_STRINGLITERAL:
      case TT_CHARLITERAL:
      // Some operators can appear at the beginning of an expression, such as unary *; just hand them all off
      case TT_OPERATOR:
      case TT_TILDE:
      case TT_SCOPE:
      case TT_NEW:
      case TT_DELETE:
      // Parentheses and brackets can be used to set off any of the above, braces can be used to group multiple expressions
      case TT_LEFTPARENTH:
      case TT_LEFTBRACKET:
      // Object size resolution operators
      case TT_SIZEOF:
      case TT_ISEMPTY: {
          AST_Node_Statement_Standard *res = new AST_Node_Statement_Standard();
          if (not(res->statement = parse_expression(token, precedence::all)))
            { delete res; return NULL; }
          if (token.type == TT_SEMICOLON) token = get_next_token();
          else if (settings::pedantic_edl) pedantic_warn(token, herr, "Semicolon should follow statement at this point");
        return res;
      };
      
      case TT_LEFTBRACE: {
        token = get_next_token();
        AST_Node_Block *res = handle_block(token);
        if (!res) return NULL;
        if (token.type != TT_RIGHTBRACE) { 
          token.report_errorf(herr, "Expected closing brace before %s");
          delete res; return NULL;
        }
        token = get_next_token();
        return res;
      }
      
      case TT_ELLIPSIS:
        token.report_error(herr, "Elipsis not valid here; did you forget to fill something in?");
        return NULL;
      
      case TT_THEN: token.report_errorf(herr, "Unexpected `then' keyword here: `then' should only follow `if'");
        return NULL;
      case TT_ELSE: token.report_errorf(herr, "Unexpected `else' clause here: no corresponding `if' block");
        return NULL;
      case TT_CATCH: token.report_errorf(herr, "Unexpected `catch' clause here: no corresponding `try' block");
        return NULL;
      
      case TT_RIGHTPARENTH: token.report_errorf(herr, "Unexpected closing parenthesis here: none open"); return NULL;
      case TT_RIGHTBRACKET: token.report_errorf(herr, "Unexpected closing bracket here: none open"); return NULL;
      case TT_COLON: token.report_error(herr, "Unexpected colon bracket here: No label given"); return NULL;
      case TT_LESSTHAN: case TT_GREATERTHAN: case TT_COMMA: token.report_errorf(herr, "Unexpected %s here: No label given"); return NULL;
      
      case TT_SEMICOLON:
        token = get_next_token();
        return new AST_Node_Block();
      
      case TT_CASE: {
        bool c_or_d;
        c_or_d = false;
        if (false) {
      case TT_DEFAULT:
          c_or_d = true;
        }
        
        statement_ref *const sref = loops_get_kind(SK_SWITCH);
        if (!sref) {
          token.report_errorf(herr, "%s can only appear in `switch' statements");
          return NULL;
        }
        AST_Node_Statement_switch *const sw = (AST_Node_Statement_switch*)sref->statement;
        
        AST_Node_Statement *res;
        token = get_next_token();
        if (c_or_d)
          res = new AST_Node_Statement_default(sw);
        else {
          AST_Node *cv = parse_expression(token, precedence::all);
          if (!cv) return NULL;
          res = new AST_Node_Statement_case(sw, cv);
        }
        if (token.type != TT_COLON and settings::pedantic_edl)
          if (pedantic_warn(token, herr, token.type == TT_SEMICOLON?
            "A colon should follow case labels rather than a semicolon" : "A colon should follow `case' and `default' labels"))
            { delete res; return NULL; }
        return res;
      }
      
      case TT_BREAK:    // Overflow
      case TT_CONTINUE: // Overflow
      case TT_RETURN:   return handle_return(token);
      
      case TT_IF:     return handle_if(token);
      case TT_REPEAT: return handle_repeat(token);
      case TT_DO:     return handle_do(token);
      case TT_WHILE:  // Overflow:
      case TT_UNTIL:  return handle_while(token);
      case TT_FOR:    return handle_for(token);
      case TT_SWITCH: return handle_switch(token);
      case TT_WITH:   return handle_with(token);
      case TT_TRY:    return handle_trycatch(token);
        break;
      
      case TT_TEMPLATE: // Scrapped since ENIGMA's dynamic enough as it is
      case TT_CLASS: // Scrapped from the spec for simplicity
      case TT_PUBLIC: case TT_PRIVATE: case TT_PROTECTED: // Scrapped along with classes
      case TT_EXTERN: case TT_ASM: case TT_OPERATORKW: case TT_TYPENAME: case TTM_CONCAT: case TTM_TOSTRING:
        token.report_errorf(herr, "Lexer has honored C++ %s: this should not happen.");
        return NULL;
      
      case TT_INVALID: default:
        token.report_error(herr, "Invalid token read at this point; lex failed");
      
      case TT_ENDOFCODE: case TT_RIGHTBRACE:
        return NULL;
     }
  }
}

EDL_AST::AST_Node_Block* EDL_AST::handle_block(token_t &token) {
  AST_Node_Statement *addto;
  AST_Node_Block *res = new AST_Node_Block();
  for (;;) {
    addto = handle_statement(token);
    if (addto) res->statements.push_back(addto);
    else break;
  }
  return res;
}

EDL_AST::AST_Node_Statement_repeat* EDL_AST::handle_repeat(token_t &token) {
  AST_Node_Statement_repeat* res = new AST_Node_Statement_repeat();
  token = get_next_token();
  if (not(res->condition = parse_expression(token, precedence::all)))
    { delete res; return NULL; }
  res->code = handle_statement(token);
  if (!res->code) { delete res; return NULL; }
  return res;
}

EDL_AST::AST_Node_Statement_return* EDL_AST::handle_return(token_t &token) {
  AST_Node_Statement_return *res = new AST_Node_Statement_return();
  if (not(res->value = parse_expression(token, precedence::all)))
    { delete res; return NULL; }
  if (token.type == TT_SEMICOLON)
    token = get_next_token();
  else if (settings::pedantic_edl)
    pedantic_warn(token, herr, "Semicolon expected following return statement");
  return res;
}
EDL_AST::AST_Node_Statement_if*       EDL_AST::handle_if      (token_t &token) {
  token = get_next_token();
  AST_Node *cd = parse_expression(token, precedence::all);
  if (!cd) return NULL;
  // Read the if block
  AST_Node_Statement *dif = handle_statement(token);
  if (!dif) { delete cd; return NULL; }
  // Check if there's an else block
  if (token.type != TT_ELSE)
    return new AST_Node_Statement_if(cd, dif);
  // Read the else block
  token = get_next_token();
  AST_Node_Statement *delse = handle_statement(token);
  if (!delse) { delete cd; delete dif; return NULL; }
  // Full statement
  return new AST_Node_Statement_if(cd, dif, delse);
}
EDL_AST::AST_Node_Statement_for*      EDL_AST::handle_for     (token_t &token) {
  token = get_next_token();
  bool expect_rightp;
  if (token.type == TT_LEFTPARENTH)
    token = get_next_token(),
    expect_rightp = true;
  else {
    if (settings::pedantic_edl)
      if (pedantic_warn(token, herr, "Expected () around parameters to `for' loop"))
        return NULL;
    expect_rightp = false;
  }
  
  // Handle the A in for(A; B; C) { D }
  if (token.type == TT_LEFTBRACE and settings::pedantic_edl)
    if (pedantic_warn(token, herr, "Blocks not actually allowed in `for' parameters"))
      return NULL;
  AST_Node_Statement *spre = handle_statement(token);
  if (!spre) return NULL;
  
  // Handle the B in for(A; B; C) { D }
  AST_Node *cond = parse_expression(token, precedence::all);
  if (!cond) { delete spre; return NULL; }
  if (token.type == TT_SEMICOLON) token = get_next_token();
  else if (settings::pedantic_edl)
    if (pedantic_warn(token, herr, "Semicolon expected after `for' condition"))
      { delete spre; delete cond; return NULL; }
  
  // Handle the C in for(A; B; C) { D }
  AST_Node_Statement *spost = handle_statement(token);
  if (!spost) { delete spre; delete cond; return NULL; }
  
  if (expect_rightp) {
    if (token.type != TT_RIGHTPARENTH) {
      token.report_errorf(herr, "Expected closing parenthesis to `for' parameters before %s");
      delete spre; delete cond; delete spost; return NULL;
    }
    token = get_next_token();
  }
  
  AST_Node_Statement *loop = handle_statement(token);
  if (!loop) { delete spre; delete cond; delete spost; return NULL; }
  
  return new AST_Node_Statement_for(spre, cond, spost, loop);
}
EDL_AST::AST_Node_Statement_do*       EDL_AST::handle_do      (token_t &token) {
  token = get_next_token();
  AST_Node_Statement *code = handle_statement(token);
  if (!code) return NULL;
  
  bool negate;
  if (token.type == TT_UNTIL) negate = true;
  else if (token.type == TT_WHILE) negate = false;
  else {
    token.report_error(herr, "Expected `until' or `while' clause to complete `do' statement");
    delete code; return NULL;
  }
  
  token = get_next_token();
  AST_Node* cond = parse_expression(token, precedence::all);
  if (!cond) { delete code; return NULL; }
  
  if (token.type == TT_SEMICOLON) token = get_next_token();
  else if (settings::pedantic_edl) if (pedantic_warn(token, herr, "ISO C++ requires a semicolon after a do-while statement; EDL follows"))
    { delete code; delete cond; return NULL; }
  
  return new AST_Node_Statement_do(code, cond, negate);
}
EDL_AST::AST_Node_Statement_while*    EDL_AST::handle_while   (token_t &token) {
  bool negate;
  if (token.type == TT_WHILE) negate = false;
  else if (token.type == TT_UNTIL) negate = true;
  else token.report_error(herr, "Parse error: `until' or `while' clause expected here");
  
  token = get_next_token();
  AST_Node* cond = parse_expression(token, precedence::all);
  if (!cond) { return NULL; }
  
  AST_Node_Statement *code = handle_statement(token);
  if (!code) { delete cond; return NULL; }
  
  return new AST_Node_Statement_while(cond, code, negate);
}
EDL_AST::AST_Node_Statement_with*     EDL_AST::handle_with    (token_t &token) {
  token = get_next_token();
  AST_Node* whom = parse_expression(token, precedence::all);
  if (!whom) { return NULL; }
  
  AST_Node_Statement *code = handle_statement(token);
  if (!code) { delete whom; return NULL; }
  
  return new AST_Node_Statement_with(whom, code);
} 
EDL_AST::AST_Node_Statement_trycatch* EDL_AST::handle_trycatch(token_t &token) {
  token = get_next_token();
  AST_Node_Statement *tcode = handle_statement(token);
  if (!tcode) return NULL;
  
  if (token.type != TT_CATCH) {
    token.report_errorf(herr, "Expected `catch' clause to complete `try' statement here before %s");
    delete tcode; return NULL;
  }
  
  AST_Node_Statement_trycatch* res = new AST_Node_Statement_trycatch(tcode);
  
  do {
    token = get_next_token();
    full_type ctype = read_fulltype(lex, token, search_scope, (context_parser*)main_context, herr);
    if (!ctype.def) { delete res; return NULL; }
    
    AST_Node_Statement *ccode = handle_statement(token);
    if (!ccode) { delete res; return NULL; }
    
    res->catches.push_back(AST_Node_Statement_trycatch::catch_clause(ctype, ccode));
  }
  while (token.type == TT_CATCH);
  return res;
}
EDL_AST::AST_Node_Statement_switch*   EDL_AST::handle_switch  (token_t &token) {
  token = get_next_token();
  AST_Node *swval = parse_expression(token, precedence::all);
  if (!swval) return NULL;
  
  if (token.type != TT_LEFTBRACE) {
    token.report_error(herr, "Expected block for `switch' statement");
    return NULL;
  }
  
  AST_Node_Statement *swblock = handle_statement(token);
  if (!swblock) { delete swval; return NULL; }
  return new AST_Node_Statement_switch(swval, swblock);
}

bool EDL_AST::parse_edl(string code) {
  llreader codereader;
  codereader.encapsulate(code);
  lex = new lexer_edl(codereader, (macro_map&)main_context->get_macros(), "Code");
  herr = def_error_handler;
  token_t token = lex->get_token(herr);
  /*while (token.type != TT_ENDOFCODE) {
    token.report_errorf(herr, "read %s");
    token = lex.get_token(herr);
  }*/
  search_scope = new definition_scope("[code]", main_context->get_global(), DEF_FUNCTION);
  AST_Node_Block* wheee = handle_block(token);
  if (token.type == TT_ENDOFCODE) {
    if (token.type == TT_RIGHTBRACE)
      token.report_error(herr, "Unexpected closing brace at this point: none open");
    cout << "Parse finished without error" << endl;
  }
  else { cout << "One or more errors occurred during parse" << endl; }
  cout << (wheee? wheee->toString(0) : "ERROR! SHIT BE NULL") << endl << endl;
  return true;
}
