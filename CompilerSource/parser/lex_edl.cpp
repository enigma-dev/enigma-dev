/**
 * @file lex_edl.cpp
 * @brief Source implementing the C++ \c lexer class extensions.
 * 
 * This file's function will be referenced, directly or otherwise, by every
 * other function in the parser. The efficiency of its implementation is of
 * crucial importance. If this file runs slow, so do the others.
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
**/

#include "lex_edl.h"
#include <General/debug_macros.h>
#include <General/parse_basics.h>
#include <General/debug_macros.h>
#include <Parser/parse_context.h>
#include <System/builtins.h>
#include <API/context.h>
#include <API/AST.h>

#include <API/compile_settings.h>

using namespace jdi;
using namespace jdip;

#define code data //I'm sorry, but I can't spend the whole file calling the file buffer "data."



/// Returns whether s1 begins with s2, followed by whitespace.
static inline bool strbw(const char* s1, const char (&s2)[3]) { return *s1 == *s2 and s1[1] == s2[1] and (!is_letterd(s1[2])); }
static inline bool strbw(const char* s1, const char (&s2)[4]) { return *s1 == *s2 and s1[1] == s2[1] and s1[2] == s2[2] and (!is_letterd(s1[3])); }
static inline bool strbw(const char* s1, const char (&s2)[5]) { return *s1 == *s2 and s1[1] == s2[1] and s1[2] == s2[2] and s1[3] == s2[3] and (!is_letterd(s1[4])); }
static inline bool strbw(const char* s1, const char (&s2)[6]) { return *s1 == *s2 and s1[1] == s2[1] and s1[2] == s2[2] and s1[3] == s2[3] and s1[4] == s2[4] and (!is_letterd(s1[5])); }
static inline bool strbw(const char* s1, const char (&s2)[7]) { return *s1 == *s2 and s1[1] == s2[1] and s1[2] == s2[2] and s1[3] == s2[3] and s1[4] == s2[4] and s1[5] == s2[5] and (!is_letterd(s1[6])); }
static inline bool strbw(const char* s1, const char (&s2)[11]){ return *s1 == *s2 and s1[1] == s2[1] and s1[2] == s2[2] and s1[3] == s2[3] and s1[4] == s2[4] and s1[5] == s2[5] and s1[6] == s2[6] and s1[7] == s2[7] and s1[8] == s2[8] and s1[9] == s2[9] and (!is_letterd(s1[10])); }
static inline bool strbw(char s) { return !is_letterd(s); }

void lexer_edl::skip_gml_string(error_handler *herr)
{
  register const char endc = code[pos];
  while (++pos < length and code[pos] != endc) {
    if (code[pos] == '\n' or (code[pos] == '\r' and code[pos + 1] != '\n'))
      ++line, lpos = pos;
  }
  if (code[pos] != endc)
    herr->error("Unterminated string literal", filename, line, pos-lpos);
}

static inline void skip_string(const char* code, size_t &pos, size_t length)
{
  register const char endc = code[pos];
  while (++pos < length and code[pos] != endc)
    if (code[pos] == '\\') if (code[pos++] == '\r' and code[pos] == '\n') ++pos;
}

/// Space-saving macro to skip comments and string literals.
#define skip_noncode(cond) {\
  if (code[pos] == '/') \
    if (code[pos+1] == '*') { skip_multiline_comment(); cond; } \
    else if (code[pos+1] == '/') { skip_comment(); cond; } else {} \
  else if (code[pos] == '"' or code[pos] == '\'') { skip_string(herr), ++pos; cond; } }

/// Optional AST rendering
#include <General/debug_macros.h>

/**
  @section Implementation
  
  For the preprocessor's implementation, we switch gears a bit. The preprocessor needs
  to be the fastest aspect of the entire parser, as directives are everywhere. As such,
  it operates on a hacked-up switch statement using a whole heap of gotos, designed to
  support a specific set of macros. Inserting a new directive into this switch is still
  simple enough, but unlike other aspects of the parser, is not as trivial as a single
  function call. Don't gripe; originally I was going to use a perfect hash.
**/
void lexer_edl::handle_preprocessor(error_handler *herr)
{
  top:
  bool variadic = false; // Whether this function is variadic
  while (code[pos] == ' ' or code[pos] == '\t') ++pos;
  const size_t pspos = pos;
  switch (code[pos++])
  {
    case 'd':
      if (strbw(code+pos, "efine")) { pos += 5; goto case_define; }
      goto failout;
	  case 'e':
      if (code[pos] == 'n') { if (strbw(code+pos+1, "dif")) { pos += 4; goto case_endif; } goto failout; }
      if (code[pos] == 'l')
      { 
        if (code[++pos] == 's') { if (code[++pos] == 'e') { ++pos; goto case_else; } goto failout; }
        if (code[pos] == 'i' and code[++pos] == 'f')
        {
          if (strbw(code[++pos])) goto case_elif;
          if (code[pos] == 'd') { if (strbw(code+pos+1, "ef"))  { pos += 3; goto case_elifdef;  } goto failout; }
          if (code[pos] == 'n') { if (strbw(code+pos+1, "def")) { pos += 4; goto case_elifndef; } goto failout; }
        }
        goto failout;
      }
      if (strbw(code+pos, "rror")) { pos += 4; goto case_error; }
      goto failout;
	  case 'i':
      if (code[pos] == 'f')
      {
        if (strbw(code[++pos])) goto case_if;
        if (code[pos] == 'd') { if (strbw(code+pos+1, "ef"))  { pos += 3; goto case_ifdef;  } goto failout; }
        if (code[pos] == 'n') { if (strbw(code+pos+1, "def")) { pos += 4; goto case_ifndef; } goto failout; }
        goto failout;
      }
	  case 'u':
      if (strbw(code+pos, "ndef")) { pos += 4; goto case_undef; }
      goto failout;
    case 'w':
      if (strbw(code+pos, "arning")) { pos += 6; goto case_warning; }
      goto failout;
    default: goto failout;
  }
  
  for (;;)
  {
    break;
    case_define: {
      string argstrs = read_preprocessor_args(herr);
      const char* argstr = argstrs.c_str();
      if (!conditionals.empty() and !conditionals.top().is_true)
        break;
      size_t i = 0;
      while (is_useless(argstr[i])) ++i;
      if (!is_letter(argstr[i])) {
        herr->error("Expected macro definiendum at this point", filename, line, pos-lpos);
      }
      const size_t nsi = i;
      while (is_letterd(argstr[++i]));
      pair<macro_iter, bool> mins = macros.insert(pair<string,macro_type*>(argstrs.substr(nsi,i-nsi),NULL));
      
      if (argstr[i] == '(') {
        vector<string> paramlist;
        while (is_useless(argstr[++i]));
        if (argstr[i] != ')') for (;;) {
          if (!is_letter(argstr[i])) {
            if (argstr[i] == '.' and argstr[i+1] == '.' and argstr[i+2] == '.') {
              variadic = true, i += 3;
              while (is_useless(argstr[i])) ++i;
              if (argstr[i] != ')')
                herr->error("Expected end of parameters after variadic", filename, line, pos-lpos);
              break;
            }
            else {
              herr->error("Expected parameter name for macro declaration", filename, line, pos-lpos);
              break;
            }
          }
          const size_t si = i;
          while (is_letterd(argstr[++i]));
          paramlist.push_back(argstrs.substr(si, i-si));
          
          while (is_useless(argstr[i])) ++i;
          if (argstr[i] == ')') break;
          if (argstr[i] == ',') { while (is_useless(argstr[++i])); continue; }
          
          // Handle variadic macros (if we are at ...)
          if (argstr[i] == '.' and argstr[i+1] == '.' and argstr[i+2] == '.') {
            i += 2; while (is_useless(argstr[++i]));
            variadic = true;
            if (argstr[i] == ')') break;
            herr->error("Expected closing parenthesis at this point; further parameters not allowed following variadic", filename, line, pos-lpos);
          }
          else
            herr->error("Expected comma or closing parenthesis at this point", filename, line, pos-lpos);
        }
        
        if (!mins.second) { // If no insertion was made; ie, the macro existed already.
        //  if ((size_t)mins.first->second->argc != paramlist.size())
        //    herr->warning("Redeclaring macro function `" + mins.first->first + '\'', filename, line, pos-lpos);
          macro_type::free(mins.first->second);
          mins.first->second = NULL;
        }
        mins.first->second = new macro_function(mins.first->first,paramlist, argstrs.substr(++i), variadic, herr);
      }
      else
      {
        if (!mins.second) { // If no insertion was made; ie, the macro existed already.
        //  if (mins.first->second->argc != -1 or ((macro_scalar*)mins.first->second)->value != argstr.substr(i))
        //    herr->warning("Redeclaring macro `" + mins.first->first + '\'', filename, line, pos-lpos);
          macro_type::free(mins.first->second);
          mins.first->second = NULL;
        }
        while (is_useless(argstr[i])) ++i;
        mins.first->second = new macro_scalar(mins.first->first,argstrs.substr(i));
      }
    } break;
    case_error: {
        string emsg = read_preprocessor_args(herr);
        if (conditionals.empty() or conditionals.top().is_true)
          herr->error(token_basics("#error " + emsg,filename,line,pos-lpos));
      } break;
      break;
    case_elif:
        if (conditionals.empty())
          herr->error(token_basics("Unexpected #elif directive; no matching #if",filename,line,pos-lpos));
        else {
          if (conditionals.top().is_true)
            conditionals.top().is_true = conditionals.top().can_be_true = false;
          else {
            if (conditionals.top().can_be_true) {
              conditionals.pop();
              goto case_if;
            }
          }
        }
      break;
    case_elifdef:
        if (conditionals.empty())
          herr->error(token_basics("Unexpected #elifdef directive; no matching #if",filename,line,pos-lpos));
        else {
          if (conditionals.top().is_true)
            conditionals.top().is_true = conditionals.top().can_be_true = false;
          else {
            if (conditionals.top().can_be_true) {
              conditionals.pop();
              goto case_ifdef;
            }
          }
        }
      break;
    case_elifndef:
        if (conditionals.empty())
          herr->error(token_basics("Unexpected #elifndef directive; no matching #if",filename,line,pos-lpos));
        else {
          if (conditionals.top().is_true)
            conditionals.top().is_true = conditionals.top().can_be_true = false;
          else {
            if (conditionals.top().can_be_true) {
              conditionals.pop();
              goto case_ifndef;
            }
          }
        }
      break;
    case_else:
        if (conditionals.empty())
          herr->error(token_basics("Unexpected #else directive; no matching #if",filename,line,pos-lpos));
        else {
          if (conditionals.top().is_true)
            conditionals.top().is_true = conditionals.top().can_be_true = false;
          else
            conditionals.top().is_true = conditionals.top().can_be_true;
        }
      break;
    case_endif:
        if (conditionals.empty())
          return herr->error(token_basics("Unexpected #endif directive: no open conditionals.",filename,line,pos-lpos));
        conditionals.pop();
      break;
    case_if: 
        if (conditionals.empty() or conditionals.top().is_true) {
          mlex->update();
          
          AST a;
          if (a.parse_expression(mlex, herr) or !a.eval()) {
            token_t res;
            render_ast(a, "if_directives");
            conditionals.push(condition(0,1));
            break;
          }
          render_ast(a, "if_directives");
          conditionals.push(condition(1,0));
        }
        else
          conditionals.push(condition(0,0));
      break;
    case_ifdef: {
        while (is_useless(code[pos])) ++pos;
        if (!is_letter(code[pos])) {
          herr->error("Expected identifier to check against macros",filename,line,pos);
          break;
        }
        const size_t msp = pos;
        while (is_letterd(code[++pos]));
        string macro(code+msp, pos-msp);
        if (conditionals.empty() or conditionals.top().is_true) {
          if (macros.find(macro) == macros.end()) {
            token_t res;
            conditionals.push(condition(0,1));
            break;
          }
          conditionals.push(condition(1,0));
        }
        else
          conditionals.push(condition(0,0));
      } break;
    case_ifndef: {
        while (is_useless(code[pos])) ++pos;
        if (!is_letter(code[pos])) {
          herr->error("Expected identifier to check against macros",filename,line,pos);
          break;
        }
        const size_t msp = pos;
        while (is_letterd(code[++pos]));
        string macro(code+msp, pos-msp);
        if (conditionals.empty() or conditionals.top().is_true) {
          if (macros.find(macro) != macros.end()) {
            token_t res;
            conditionals.push(condition(0,1));
            break;
          }
          conditionals.push(condition(1,0));
        }
        else
          conditionals.push(condition(0,0));
      } break;
    case_undef:
        if (!conditionals.empty() and !conditionals.top().is_true)
          break;
        
        while (is_useless(code[pos])) ++pos;
        if (!is_letter(code[pos]))
          herr->error("Expected macro identifier at this point", filename, line, pos);
        else {
          const size_t nspos = pos;
          while (is_letterd(code[++pos]));
          macro_iter mdel = macros.find(string(code+nspos,pos-nspos));
          if (mdel != macros.end()) {
            macro_type::free(mdel->second);
            macros.erase(mdel);
          }
        }
      break;
    case_warning: {
        string wmsg = read_preprocessor_args(herr);
        if (conditionals.empty() or conditionals.top().is_true)
          herr->warning(token_basics("#warning " + wmsg,filename,line,pos-lpos));
      } break;
  }
  
  // skip shit here
  while (code[pos] != '}' or code[pos-1] != '}') {
    if (code[pos] == '\n') ++line;
    ++pos;
  }
  ++pos;
  
  if (conditionals.empty() or conditionals.top().is_true)
    return;
  
  // skip_to_macro:
  while (pos < length) {
    if (code[pos] == '{' and code[++pos] == '{')
    {
      ++pos;
      goto top;
    }
    if (code[pos++] == '\n')
      ++line;
  }
  
  herr->error("Expected closing preprocessors before end of code",filename,line,pos-lpos);
  return;
  
  failout:
    while (is_letterd(code[pos])) ++pos;
    string ppname(code + pspos, pos - pspos);
    herr->error(token_basics("Invalid preprocessor directive `" + ppname + "'",filename,line,pos-lpos));
    while (pos < length and (code[pos] != '}' or code[pos-1] != '}')) ++pos;
    ++pos;
}


string lexer_edl::read_preprocessor_args(error_handler *herr)
{
  size_t spos = pos; const size_t lenm1 = length - 1;
  while (++pos < lenm1 and (code[pos] != '}' or code[pos+1] != '}'));
  if (pos >= lenm1) herr->error("Unterminated preprocessor directive",filename,line,pos-lpos);
  return string(code, spos, pos-spos);
}

#include <cstdio>
token_t lexer_edl::get_token(error_handler *herr)
{
  #ifdef DEBUG_MODE
    static int number_of_times_GDB_has_dropped_its_ass = 0;
    ++number_of_times_GDB_has_dropped_its_ass;
  #endif
  
  for (;;) // Loop until we find something or hit world's end
  {
    if (pos >= length) goto POP_FILE;
    
    // Skip all whitespace
    while (is_useless(code[pos])) {
      if (code[pos] == '\n') ++line, lpos = pos;
      if (code[pos] == '\r') { ++line; lpos = pos; if (code[++pos] != '\n') --pos; }
      if (++pos >= length) goto POP_FILE;
    }
    
    //============================================================================================
    //====: Check for and handle comments. :======================================================
    //============================================================================================
    
    if (code[pos] == '/') {
      if (code[++pos] == '*') { skip_multiline_comment(); continue; }
      if (code[pos] == '/') { skip_comment(); continue; }
      if (code[pos] == '=') {
        ++pos;
        return  token_t(token_basics(TT_OPERATOR,filename,line,pos-lpos), code+pos-2, 2);
      }
      return token_t(token_basics(TT_OPERATOR,filename,line,pos-lpos), code+pos-1,1);
    }
    
    //============================================================================================
    //====: Not at a comment. See if we're at an identifier. :====================================
    //============================================================================================
    
    if (is_letter(code[pos])) // Check if we're at an identifier or keyword.
    {
      const size_t spos = pos; // Record where we are
      while (++pos < length and is_letterd(code[pos]));
      if (code[spos] == 'L' and pos - spos == 1 and code[pos] == '\'') {
        skip_string(herr);
        return token_t(token_basics(TT_CHARLITERAL,filename,line,spos-lpos), code + spos, ++pos-spos);
      }
      
      string fn(code + spos, code + pos); // We'll need a copy of this thing for lookup purposes
      
      macro_iter mi = macros.find(fn);
      if (mi != macros.end()) {
        domacro_mi:
        if (mi->second->argc < 0) {
          bool already_open = false; // Test if we're in this macro already
          quick::stack<openfile>::iterator it = files.begin();
          for (unsigned i = 0; i < open_macro_count; ++i)
            if (it->filename == fn) { already_open = true; break; }
            else --it;
          if (!already_open) {
            enter_macro((macro_scalar*)mi->second);
            continue;
          }
        }
        else {
          if (parse_macro_function((macro_function*)mi->second, herr))
            continue;
        }
      }
      
      keyword_map::iterator kwit = keywords.find(fn);
      if (kwit != keywords.end()) {
        if (kwit->second == TT_INVALID) {
          mi = kludge_map.find(fn);
          #ifdef DEBUG_MODE
          if (mi == kludge_map.end())
            cerr << "SYSTEM ERROR! KEYWORD `" << fn << "' IS DEFINED AS INVALID" << endl;
          #endif
          goto domacro_mi;
        }
        return token_t(token_basics(kwit->second,filename,line,spos-lpos));
      }
      
      tf_iter tfit = builtin_declarators.find(fn);
      if (tfit != builtin_declarators.end()) {
        if ((tfit->second->usage & UF_STANDALONE_FLAG) == UF_PRIMITIVE)
          return token_t(token_basics(TT_DECLARATOR,filename,line,spos-lpos), tfit->second->def);
        return token_t(token_basics(TT_DECFLAG,filename,line,spos-lpos), (definition*)tfit->second);
      }
      
      return token_t(token_basics(TT_IDENTIFIER,filename,line,spos-lpos), code + spos, fn.length());
    }
    
    //============================================================================================
    //====: Not at an identifier. Maybe at a number? :============================================
    //============================================================================================
    
    if (is_digit(code[pos])) {
      if (code[pos] == '0') { // Check if the number is hexadecimal or octal.
        if (code[++pos] == 'x') { // Check if the number is hexadecimal.
          // Yes, it is hexadecimal.
          const size_t sp = pos;
          while (++pos < length and is_hexdigit(code[pos]));
          while (pos < length and is_letter(code[pos])) pos++; // Include the flags, like ull
          return token_t(token_basics(TT_HEXLITERAL,filename,line,pos-lpos), code+sp, pos-sp);  
        }
        // Turns out, it's octal.
        const size_t sp = --pos;
        while (++pos < length and is_hexdigit(code[pos]));
        while (pos < length and is_letter(code[pos])) pos++; // Include the flags, like ull
        return token_t(token_basics(TT_OCTLITERAL,filename,line,pos-lpos), code+sp, pos-sp);
      }
      // Turns out, it's decimal.
      handle_decimal:
      const size_t sp = pos;
      while (++pos < length and is_digit(code[pos]));
      if (code[pos] == '.')
        while (++pos < length and is_digit(code[pos]));
      if (code[pos] == 'e' or code[pos] == 'E') { // Accept exponents
        if (code[++pos] == '-') ++pos;
        while (pos < length and is_digit(code[pos])) ++pos;
      }
      while (pos < length and is_letter(code[pos])) ++pos; // Include the flags, like ull
      return token_t(token_basics(TT_DECLITERAL,filename,line,pos-lpos), code+sp, pos-sp);
    }
    
    //============================================================================================
    //====: Not at a number. Find out where we are. :=============================================
    //============================================================================================
    
    const size_t spos = pos;
    switch (code[pos++])
    {
      case ';':
        return token_t(token_basics(TT_SEMICOLON,filename,line,spos-lpos));
      case ',':
        return token_t(token_basics(TT_COMMA,filename,line,spos-lpos));
      case '+': case '-':
        pos += code[pos] == code[spos] or code[pos] == '=' or (code[pos] == '>' and code[spos] == '-');
        pos += (code[pos-1] == '>' and code[pos] == '*');
        return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), code+spos, pos-spos);
      case '=': pos += code[pos] == code[spos];
        return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), code+spos, pos-spos);
      case '!': case '~':
        if (code[pos] == '=')
          return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), code+spos, ++pos-spos);
        return token_t(token_basics(TT_TILDE,filename,line,spos-lpos), code+spos, pos-spos);
      case '%': case '/':
        if (code[pos] == '=')
          return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), code+spos, ++pos-spos);
        return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), code+spos, pos-spos);
      case '>': case '<': case '*': case '^': case '&': case '|':
        pos += code[pos] == code[spos]; pos += code[pos] == '=';
        return token_t(token_basics((pos-spos==1?code[spos]=='<'?TT_LESSTHAN:TT_GREATERTHAN:TT_OPERATOR),filename,line,spos-lpos), code+spos, pos-spos);
      case ':':
        pos += code[pos] == code[spos];
        return token_t(token_basics(pos - spos == 1 ? TT_COLON : TT_SCOPE,filename,line,spos-lpos), code+spos, pos-spos);
      case '?':
        return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), code+spos, pos-spos);
      
      case '.':
          if (is_digit(code[pos]))
            goto handle_decimal;
          else if (code[pos] == '.') {
            if (code[++pos] == '.')
              return token_t(token_basics(TT_ELLIPSIS,filename,line,spos-lpos), code+spos, pos++ - spos);
            else
              pos -= 2;
          }
          pos += code[pos] == '*';
        return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), code+spos, pos-spos);
      
      case '(': return token_t(token_basics(TT_LEFTPARENTH,filename,line,spos-lpos));
      case '[': return token_t(token_basics(TT_LEFTBRACKET,filename,line,spos-lpos));
      case '{': if (code[pos] == '{') return (++pos, handle_preprocessor(herr), get_token(herr));
                return token_t(token_basics(TT_LEFTBRACE,  filename,line,spos-lpos));
      case '}': return token_t(token_basics(TT_RIGHTBRACE,  filename,line,spos-lpos));
      case ']': return token_t(token_basics(TT_RIGHTBRACKET,filename,line,spos-lpos));
      case ')': return token_t(token_basics(TT_RIGHTPARENTH,filename,line,spos-lpos));
      
      case '\\':
        if (code[pos] != '\n' and code[pos] != '\r')
          herr->error("Stray backslash", filename, line, pos-lpos);
        continue;
      
      case '"': {
        --pos; skip_string(herr);
        return token_t(token_basics(TT_STRINGLITERAL,filename,line,spos-lpos), code + spos, ++pos-spos);
      }
      
      case '\'': {
        --pos; skip_string(herr);
        return token_t(token_basics(TT_STRINGLITERAL,filename,line,spos-lpos), code + spos, ++pos-spos);
      }
      
      default: {
        char errbuf[320];
        sprintf(errbuf, "Unrecognized symbol (char)0x%02X '%c'", (int)code[spos], code[spos]);
        herr->error(errbuf);
        return token_t(token_basics(TT_INVALID,filename,line,pos-lpos++));
      }
    }
  }
  
  cerr << "UNREACHABLE BLOCK REACHED" << endl;
  return token_t(TT_INVALID,filename,line,pos-lpos++);
  
  POP_FILE: // This block was created instead of a helper function to piss Rusky off.
  if (pop_file())
    return token_t(token_basics(TT_ENDOFCODE,filename,line,pos-lpos));
  
  size_t sz = 0;
  for (quick::stack<openfile>::iterator it = files.begin(); it != files.end(); ++it) ++sz;
  return get_token(herr);
}

lexer_edl::lexer_edl(llreader &input, macro_map &pmacros, const char *fname): lexer_cpp(input, pmacros, fname)
{
  pair<map<string, TOKEN_TYPE>::iterator, bool> ins = keywords.insert(pair<string, TOKEN_TYPE>("with", TT_WITH));
  if (ins.second) {
    /* *****************************
    ** Add EDL Keywords not in C++
    ** ****************************/
    keywords["if"]       = TT_IF;
    keywords["else"]     = TT_ELSE;
    keywords["repeat"]   = TT_REPEAT;
    keywords["do"]       = TT_DO;
    keywords["while"]    = TT_WHILE;
    keywords["until"]    = TT_UNTIL;
    keywords["for"]      = TT_FOR;
    keywords["switch"]   = TT_SWITCH;
    keywords["case"]     = TT_CASE;
    keywords["default"]  = TT_DEFAULT;
    keywords["break"]    = TT_BREAK;
    keywords["continue"] = TT_CONTINUE;
    keywords["return"]   = TT_RETURN;
    keywords["try"]      = TT_TRY;
    keywords["catch"]    = TT_CATCH;
    
    /* *******************************
    ** Remove C++ keywords not in EDL
    ** ******************************/
    // Note: This list is incomplete.
    keywords.erase("extern");
    keywords.erase("asm");
    keywords.erase("operator");
    keywords.erase("typename");
  }
}
lexer_edl::~lexer_edl() {
  
}
#undef code

