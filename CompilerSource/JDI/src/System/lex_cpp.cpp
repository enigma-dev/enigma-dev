/**
 * @file lex_cpp.cpp
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

#include "lex_cpp.h"
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

#define cfile data //I'm sorry, but I can't spend the whole function calling the file buffer "data."

/// Returns whether s1 begins with s2, followed by whitespace.
static inline bool strbw(const char* s1, const char (&s2)[3]) { return *s1 == *s2 and s1[1] == s2[1] and (!is_letterd(s1[2])); }
static inline bool strbw(const char* s1, const char (&s2)[4]) { return *s1 == *s2 and s1[1] == s2[1] and s1[2] == s2[2] and (!is_letterd(s1[3])); }
static inline bool strbw(const char* s1, const char (&s2)[5]) { return *s1 == *s2 and s1[1] == s2[1] and s1[2] == s2[2] and s1[3] == s2[3] and (!is_letterd(s1[4])); }
static inline bool strbw(const char* s1, const char (&s2)[6]) { return *s1 == *s2 and s1[1] == s2[1] and s1[2] == s2[2] and s1[3] == s2[3] and s1[4] == s2[4] and (!is_letterd(s1[5])); }
static inline bool strbw(const char* s1, const char (&s2)[7]) { return *s1 == *s2 and s1[1] == s2[1] and s1[2] == s2[2] and s1[3] == s2[3] and s1[4] == s2[4] and s1[5] == s2[5] and (!is_letterd(s1[6])); }
static inline bool strbw(const char* s1, const char (&s2)[11]){ return *s1 == *s2 and s1[1] == s2[1] and s1[2] == s2[2] and s1[3] == s2[3] and s1[4] == s2[4] and s1[5] == s2[5] and s1[6] == s2[6] and s1[7] == s2[7] and s1[8] == s2[8] and s1[9] == s2[9] and (!is_letterd(s1[10])); }
static inline bool strbw(char s) { return !is_letterd(s); }

void lexer_cpp::skip_comment()
{
  #if ALLOW_MULTILINE_COMMENTS
  while (++pos < length and cfile[pos] != '\n' and cfile[pos] != '\r') if (cfile[pos] == '\\') {
    if (cfile[++pos] == '\n') ++line, lpos = pos;
    else if (cfile[pos] == '\r') pos += cfile[pos+1] == '\n', ++line, lpos = pos;
  }
  #else
  while (++pos < length and cfile[pos] != '\n' and cfile[pos] != '\r');
  #endif
}

static inline void skip_comment(const char* cfile, size_t &pos, size_t length)
{
  #if ALLOW_MULTILINE_COMMENTS
  while (++pos < length and cfile[pos] != '\n' and cfile[pos] != '\r') if (cfile[pos] == '\\') ++pos;
  #else
  while (++pos < length and cfile[pos] != '\n' and cfile[pos] != '\r');
  #endif
}

inline void lexer_cpp::skip_multiline_comment()
{
  if (cfile[pos++] == '/')
    ++pos; // Skip one more char so we don't break on /*/
  do if (pos >= length) return;
    else if (cfile[pos] == '\n' or (cfile[pos] == '\r' and cfile[pos+1] != '\n')) ++line, lpos = pos;
    while (cfile[pos++] != '*' or cfile[pos] != '/');
  ++pos;
}

static inline void skip_multiline_comment(const char* cfile, size_t &pos, size_t length)
{
  if (cfile[pos++] == '/') ++pos; // Skip one more char so we don't break on /*/
  do if (pos >= length) return; while (cfile[pos++] != '*' or cfile[pos] != '/');
  ++pos;
}

void lexer_cpp::skip_string(error_handler *herr)
{
  register const char endc = cfile[pos];
  while (++pos < length and cfile[pos] != endc)
  {
    if (cfile[pos] == '\\') {
      if (cfile[++pos] == '\n') ++line, lpos = pos;
      else if (cfile[pos] == '\r') {
        if (cfile[++pos] != '\n') --pos;
        ++line, lpos = pos; 
      }
    }
    else if (cfile[pos] == '\n' or cfile[pos] == '\r') {
      herr->error("Unterminated string literal", filename, line, pos-lpos);
      break;
    }
  }
  if (cfile[pos] != endc)
    herr->error("Unterminated string literal", filename, line, pos-lpos);
}

static inline void skip_string(const char* cfile, size_t &pos, size_t length)
{
  register const char endc = cfile[pos];
  while (++pos < length and cfile[pos] != endc)
    if (cfile[pos] == '\\') if (cfile[pos++] == '\r' and cfile[pos] == '\n') ++pos;
}


void lexer_cpp::skip_whitespace()
{
  while (pos < length) {
    while (cfile[pos] == ' ' or cfile[pos] == '\t') if (++pos >= length) return ++line, lpos = pos, void();
    if (cfile[pos] == '\n' or (cfile[pos] == '\r' and cfile[pos+1] != '\n')) { ++line; lpos = pos++; continue; }
    if (cfile[pos] == '/') {
      if (cfile[++pos] == '/') { skip_comment(); continue; }
      if (cfile[pos] == '*') { skip_multiline_comment(); continue; }
      return;
    }
    return;
  }
}

static inline void skip_whitespace(const char* cfile, size_t &pos, size_t length) {
  while (pos < length) {
    while (cfile[pos] == ' ' or cfile[pos] == '\t') if (++pos >= length) return;
    if (cfile[pos] == '\n' or (cfile[pos] == '\r' and cfile[pos+1] != '\n')) continue;
    if (cfile[pos] == '/') {
      if (cfile[++pos] == '/') { skip_comment(cfile, pos, length); continue; }
      if (cfile[pos] == '*') { skip_multiline_comment(cfile, pos, length); continue; }
      return;
    }
    return;
  }
}

/// Space-saving macro to skip comments and string literals.
#define skip_noncode(cond) {\
  if (cfile[pos] == '/') \
    if (cfile[pos+1] == '*') { skip_multiline_comment(); cond; } \
    else if (cfile[pos+1] == '/') { skip_comment(); cond; } else {} \
  else if (cfile[pos] == '"' or cfile[pos] == '\'') { skip_string(herr), ++pos; cond; } }

/// Space-saving static macro to skip comments and string literals.
#define _skip_noncode(cond) {\
  if (cfile[pos] == '/') \
    if (cfile[pos+1] == '*') { ::skip_multiline_comment(cfile, pos, length); cond; } \
    else if (cfile[pos+1] == '/') { ::skip_comment(cfile, pos, length); cond; } else {} \
  else if (cfile[pos] == '"' or cfile[pos] == '\'') { ::skip_string(cfile, pos, length), ++pos; cond; } }


void lexer_cpp::enter_macro(macro_scalar* ms)
{
  if (ms->value.empty()) return;
  openfile of(filename, line, lpos, *this);
  files.enswap(of);
  filename = ms->name.c_str();
  this->encapsulate(ms->value);
  line = lpos = pos = 0;
  ++open_macro_count;
}

string lexer_cpp::_flatten(const string param, const macro_map& macros, const token_t &errep, error_handler *herr)
{
  const char* s;
  string result = param;
  const char* begin = param.c_str();
  int resOffset = 0;
  for (const char* i = begin; *i; ) {
    bool id = is_letterd(*i);
    s = i; while (is_letterd(*i)) ++i;
    if (id)
    {
      if (is_letter(*s)) {
        while (is_useless(*i)) ++i;
        if (*i == '(') {
          string name(s,i);
          macro_iter_c mac = macros.find(name);
          if (mac != macros.end() and mac->second->argc != -1) {
            vector<string> arguments;
            size_t p = i - begin;
            const macro_function* mf = (macro_function*)mac->second;
            parse_macro_params(mf, macros, begin, p, param.length(), arguments, errep, herr);
            char *buf=0, *bufe=0;
            mf->parse(arguments, buf, bufe, errep, herr);
            i = begin + p;
            result.replace(s-begin+resOffset, i-s, buf, bufe-buf);
            resOffset += (bufe-buf) - (i-s);
            delete[] buf;
          }
        }
      }
    }
    else ++i;
  }
  return result;
}

bool lexer_cpp::parse_macro_params(const macro_function* mf, const macro_map &macros, const char* cfile, size_t &pos, size_t length, vector<string>& dest, const token_t &errep, error_handler *herr)
{
  ::skip_whitespace(cfile, pos, length);
  
  size_t pspos = ++pos;
  dest.reserve(mf->argc);
  
  // Read the parameters into our argument vector
  int nestcnt = 1;
  while (pos < length and nestcnt > 0) {
    if (cfile[pos] == ',' and nestcnt == 1) {
      dest.push_back(_flatten(string(cfile+pspos, pos-pspos), macros, errep, herr));
      pspos = ++pos;
      _skip_noncode(continue);
      continue;
    } else if (cfile[pos] == ')' || cfile[pos] == ']') { if (--nestcnt) ++pos; continue; }
      else if (cfile[pos] == '(' || cfile[pos] == '[') ++nestcnt;
      else if (cfile[pos] == '"' or cfile[pos] == '\'') 
        ::skip_string(cfile, pos, length);
    ++pos; _skip_noncode(continue);
  }
  if (pos >= length or cfile[pos] != ')') {
    errep.report_error(herr, "Unterminated parameters to macro function");
    return false;
  }
  
  if (pos > pspos) // If there was a parameter
    dest.push_back(_flatten(string(cfile+pspos, pos-pspos), macros, errep, herr)); // Nab the final parameter
  ++pos; // Don't get bitten in the ass by the closing parenthesis after the file pop.
  return true;
}

bool lexer_cpp::parse_macro_params(const macro_function* mf, vector<string>& dest, error_handler *herr)
{
  skip_whitespace();
  
  size_t pspos = ++pos;
  dest.reserve(mf->argc);
  
  // Read the parameters into our argument vector
  int nestcnt = 1;
  while (pos < length and nestcnt > 0) {
    if (cfile[pos] == ',' and nestcnt == 1) {
      dest.push_back(_flatten(string(cfile+pspos, pos-pspos), macros, token_t(token_basics(TT_INVALID, filename, line, pos)), herr));
      pspos = ++pos;
      skip_noncode(continue);
      continue;
    } else if (cfile[pos] == ')' || cfile[pos] == ']') { if (--nestcnt) ++pos; continue; }
      else if (cfile[pos] == '(' || cfile[pos] == '[') ++nestcnt;
      else if (cfile[pos] == '"' or cfile[pos] == '\'') 
        skip_string(herr);
    ++pos; skip_noncode(continue);
  }
  if (pos >= length or cfile[pos] != ')') {
    herr->error("Unterminated parameters to macro function", filename, line, pos - lpos);
    return false;
  }
  
  if (pos > pspos) // If there was a parameter
    dest.push_back(_flatten(string(cfile+pspos, pos-pspos), macros, token_t(token_basics(TT_INVALID, filename, line, pos)), herr)); // Nab the final parameter
  ++pos; // Don't get bitten in the ass by the closing parenthesis after the file pop.
  return true;
}

bool lexer_cpp::parse_macro_function(const macro_function* mf, error_handler *herr)
{
  const size_t spos = pos, slpos = lpos, sline = line;
  skip_whitespace(); // Move to the next "token"
  if (pos >= length or cfile[pos] != '(') { pos = spos, lpos = slpos, line = sline; return false; }
  
  vector<string> params;
  parse_macro_params(mf, params, herr);
  
  // Enter the macro
  openfile of(filename, line, lpos, *this);
  files.enswap(of);
  alias(files.top().file);
  char *buf=0, *bufe=0;
  if (!mf->parse(params, buf, bufe, token_t(token_basics(TT_INVALID,filename,line,lpos-pos)), herr)) {
    this->consume(files.top().file);
    files.pop();
    return true;
  }
  this->consume(buf, bufe-buf);
  filename = mf->name.c_str();
  lpos = line = 0;
  return true;  
}


string lexer_cpp::read_preprocessor_args(error_handler *herr)
{
  for (;;) {
    while (cfile[pos] == ' ' or cfile[pos] == '\t') if (++pos >= length) return "";
    if (cfile[pos] == '/') {
      if (cfile[++pos] == '/') { skip_comment(); return ""; }
      if (cfile[pos] == '*') { skip_multiline_comment(); continue; }
      break;
    }
    if (cfile[pos] == '\n' or cfile[pos] == '\r') return "";
    if (cfile[pos] == '\\' and (cfile[++pos] == '\n' or (cfile[pos] == '\r' and (cfile[++pos] == '\n' or --pos)))) { lpos = pos++; ++line; }
    break;
  }
  string res;
  res.reserve(256);
  size_t spos = pos;
  while (pos < length and cfile[pos] != '\n' and cfile[pos] != '\r') {
    if (cfile[pos] == '/') {
      if (cfile[++pos] == '/') { res += string(cfile+spos,pos-spos-1); skip_comment(); return res; }
      if (cfile[pos] == '*') {
        res.reserve(res.length()+pos-spos);
        res += string(cfile+spos,pos-spos-1), res += " ";
        skip_multiline_comment(); spos = pos; continue; }
    }
    if (cfile[pos] == '\'' or cfile[pos] == '"') skip_string(herr), ++pos;
    else if (cfile[pos] == '\\' and (cfile[++pos] == '\n' or (cfile[pos] == '\r' and (cfile[++pos] == '\n' or --pos)))) { lpos = pos++; ++line; }
    else ++pos;
  }
  res += string(cfile+spos,pos-spos);
  {
    register size_t trim = res.length() - 1;
    if (is_useless(res[trim])) {
      while (is_useless(res[--trim]));
      res.erase(++trim);
    }
  }
  return res;
}

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
void lexer_cpp::handle_preprocessor(error_handler *herr)
{
  top:
  bool variadic = false; // Whether this function is variadic
  while (cfile[pos] == ' ' or cfile[pos] == '\t') ++pos;
  const size_t pspos = pos;
  switch (cfile[pos++])
  {
    case 'd':
      if (strbw(cfile+pos, "efine")) { pos += 5; goto case_define; }
      goto failout;
	  case 'e':
      if (cfile[pos] == 'n') { if (strbw(cfile+pos+1, "dif")) { pos += 4; goto case_endif; } goto failout; }
      if (cfile[pos] == 'l')
      { 
        if (cfile[++pos] == 's') { if (cfile[++pos] == 'e') { ++pos; goto case_else; } goto failout; }
        if (cfile[pos] == 'i' and cfile[++pos] == 'f')
        {
          if (strbw(cfile[++pos])) goto case_elif;
          if (cfile[pos] == 'd') { if (strbw(cfile+pos+1, "ef"))  { pos += 3; goto case_elifdef;  } goto failout; }
          if (cfile[pos] == 'n') { if (strbw(cfile+pos+1, "def")) { pos += 4; goto case_elifndef; } goto failout; }
        }
        goto failout;
      }
      if (strbw(cfile+pos, "rror")) { pos += 4; goto case_error; }
      goto failout;
	  case 'i':
      if (cfile[pos] == 'f')
      {
        if (strbw(cfile[++pos])) goto case_if;
        if (cfile[pos] == 'd') { if (strbw(cfile+pos+1, "ef"))  { pos += 3; goto case_ifdef;  } goto failout; }
        if (cfile[pos] == 'n') { if (strbw(cfile+pos+1, "def")) { pos += 4; goto case_ifndef; } goto failout; }
        goto failout;
      }
      if (cfile[pos] == 'n') {
        if (strbw(cfile+pos+1, "clude")) { pos += 6; goto case_include; }
        if (strbw(cfile+pos+1, "clude_next")) { pos += 11; goto case_include_next; }
        goto failout;
      }
      if (cfile[pos] == 'm') { if (strbw(cfile+pos+1, "port"))  { pos += 5; goto case_import;  } goto failout; }
      goto failout;
	  case 'l':
      if (strbw(cfile+pos, "ine")) { pos += 3; goto case_line; }
      goto failout;
	  case 'p':
      if (strbw(cfile+pos, "ragma")) { pos += 5; goto case_pragma; }
      goto failout;
	  case 'u':
      if (strbw(cfile+pos, "ndef")) { pos += 4; goto case_undef; }
      if (strbw(cfile+pos, "sing")) { pos += 4; goto case_using; }
      goto failout;
    case 'w':
      if (strbw(cfile+pos, "arning")) { pos += 6; goto case_warning; }
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
        while (is_useless(cfile[pos])) ++pos;
        if (!is_letter(cfile[pos])) {
          herr->error("Expected identifier to check against macros",filename,line,pos);
          break;
        }
        const size_t msp = pos;
        while (is_letterd(cfile[++pos]));
        string macro(cfile+msp, pos-msp);
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
        while (is_useless(cfile[pos])) ++pos;
        if (!is_letter(cfile[pos])) {
          herr->error("Expected identifier to check against macros",filename,line,pos);
          break;
        }
        const size_t msp = pos;
        while (is_letterd(cfile[++pos]));
        string macro(cfile+msp, pos-msp);
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
    case_import:
      break;
    case_include: {
        bool incnext;
        if (true) incnext = false;
        else { case_include_next: incnext = true; }
        
        string fnfind = read_preprocessor_args(herr);
        if (!conditionals.empty() and !conditionals.top().is_true)
          break;
        
        bool chklocal = false;
        char match = '>';
        if (!incnext and fnfind[0] == '"')
          chklocal = true, match = '"';
        else if (fnfind[0] != '<') {
          herr->error("Expected filename inside <> or \"\" delimiters", filename, line, pos - lpos);
          break;
        }
        fnfind[0] = '/';
        for (size_t i = 0; i < fnfind.length(); ++i)
          if (fnfind[i] == match) { fnfind.erase(i); break; }
        
        string incfn;
        llreader incfile;
        if (chklocal)
          incfile.open((incfn = path + fnfind).c_str());
        for (size_t i = 0; i < builtin->search_dir_count(); ++i) {
          if (incfile.is_open()) {
            if (incnext) {
              incnext = incfn != filename;
              incfile.close();
            } else break;
          }
          incfile.open((incfn = builtin->search_dir(i) + fnfind).c_str());
        }
        if (!incfile.is_open()) {
          herr->error("Could not find " + fnfind.substr(1), filename, line, pos-lpos);
          if (chklocal) cerr << "  Checked " << path << endl;
          for (size_t i = 0; !incfile.is_open() and i < builtin->search_dir_count(); ++i)
            cerr << "  Checked " << builtin->search_dir(i) << endl;
          break;
        }
        
        openfile of(filename, line, lpos, *this);
        files.enswap(of);
        pair<set<string>::iterator, bool> fi = visited_files.insert(incfn);
        filename = fi.first->c_str();
        this->consume(incfile);
        line = 1;
      } break;
    case_line:
      break;
    case_pragma:
        #ifdef DEBUG_MODE
        {
          string n = read_preprocessor_args(herr);
          if (n == "DEBUG_ENTRY_POINT" and (conditionals.empty() or conditionals.top().is_true))
            cout << "* Debug entry point" << endl;
        }
        #else
          read_preprocessor_args(herr);
        #endif
      break;
    case_undef:
        if (!conditionals.empty() and !conditionals.top().is_true)
          break;
        
        while (is_useless(cfile[pos])) ++pos;
        if (!is_letter(cfile[pos]))
          herr->error("Expected macro identifier at this point", filename, line, pos);
        else {
          const size_t nspos = pos;
          while (is_letterd(cfile[++pos]));
          macro_iter mdel = macros.find(string(cfile+nspos,pos-nspos));
          if (mdel != macros.end()) {
            macro_type::free(mdel->second);
            macros.erase(mdel);
          }
        }
      break;
    case_using:
      break;
    case_warning: {
        string wmsg = read_preprocessor_args(herr);
        if (conditionals.empty() or conditionals.top().is_true)
          herr->warning(token_basics("#warning " + wmsg,filename,line,pos-lpos));
      } break;
  }
  if (conditionals.empty() or conditionals.top().is_true)
    return;
  
  // skip_to_macro:
  while (pos < length) {
    while (pos < length and cfile[pos] != '\n' and cfile[pos] != '\r') {
      skip_noncode(continue);
      ++pos;
    }
    if (pos >= length)
      break;
    while (is_useless(cfile[pos])) {
      if (cfile[pos] == '\n') lpos = pos++, ++line;
      else if (cfile[pos] == '\r' and (cfile[++pos] == '\n' or --pos)) lpos = pos++, ++line;
      else ++pos;
    }
    if (cfile[pos] == '#')
      { ++pos; goto top; }
  }
  herr->error("Expected closing preprocessors before end of code",filename,line,pos-lpos);
  return;
  
  failout:
    while (is_letterd(cfile[pos])) ++pos;
    string ppname(cfile + pspos, pos - pspos);
    herr->error(token_basics("Invalid preprocessor directive `" + ppname + "'",filename,line,pos-lpos));
    while (pos < length and cfile[pos] != '\n' and cfile[pos] != '\r') ++pos;
}

#include <cstdio>
token_t lexer_cpp::get_token(error_handler *herr)
{
  get_token_top:
  
  #ifdef DEBUG_MODE
    static int number_of_times_GDB_has_dropped_its_ass = 0;
    ++number_of_times_GDB_has_dropped_its_ass;
  #endif
  
  for (;;) // Loop until we find something or hit world's end
  {
    if (pos >= length) goto POP_FILE;
    
    // Skip all whitespace
    while (is_useless(cfile[pos])) {
      if (cfile[pos] == '\n') ++line, lpos = pos;
      if (cfile[pos] == '\r') { ++line; lpos = pos; if (cfile[++pos] != '\n') --pos; }
      if (++pos >= length) goto POP_FILE;
    }
    
    //============================================================================================
    //====: Check for and handle comments. :======================================================
    //============================================================================================
    
    if (cfile[pos] == '/') {
      if (cfile[++pos] == '*') { skip_multiline_comment(); continue; }
      if (cfile[pos] == '/') { skip_comment(); continue; }
      if (cfile[pos] == '=') {
        ++pos;
        return  token_t(token_basics(TT_OPERATOR,filename,line,pos-lpos), cfile+pos-2, 2);
      }
      return token_t(token_basics(TT_OPERATOR,filename,line,pos-lpos), cfile+pos-1,1);
    }
    
    //============================================================================================
    //====: Not at a comment. See if we're at an identifier. :====================================
    //============================================================================================
    
    if (is_letter(cfile[pos])) // Check if we're at an identifier or keyword.
    {
      const size_t spos = pos; // Record where we are
      while (++pos < length and is_letterd(cfile[pos]));
      if (cfile[spos] == 'L' and pos - spos == 1 and cfile[pos] == '\'') {
        skip_string(herr);
        return token_t(token_basics(TT_CHARLITERAL,filename,line,spos-lpos), cfile + spos, ++pos-spos);
      }
      
      string fn(cfile + spos, cfile + pos); // We'll need a copy of this thing for lookup purposes
      
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
      
      return token_t(token_basics(TT_IDENTIFIER,filename,line,spos-lpos), cfile + spos, fn.length());
    }
    
    //============================================================================================
    //====: Not at an identifier. Maybe at a number? :============================================
    //============================================================================================
    
    if (is_digit(cfile[pos])) {
      if (cfile[pos] == '0') { // Check if the number is hexadecimal or octal.
        if (cfile[++pos] == 'x') { // Check if the number is hexadecimal.
          // Yes, it is hexadecimal.
          const size_t sp = pos;
          while (++pos < length and is_hexdigit(cfile[pos]));
          while (pos < length and is_letter(cfile[pos])) pos++; // Include the flags, like ull
          return token_t(token_basics(TT_HEXLITERAL,filename,line,pos-lpos), cfile+sp, pos-sp);  
        }
        // Turns out, it's octal.
        const size_t sp = --pos;
        while (++pos < length and is_hexdigit(cfile[pos]));
        while (pos < length and is_letter(cfile[pos])) pos++; // Include the flags, like ull
        return token_t(token_basics(TT_OCTLITERAL,filename,line,pos-lpos), cfile+sp, pos-sp);
      }
      // Turns out, it's decimal.
      handle_decimal:
      const size_t sp = pos;
      while (++pos < length and is_digit(cfile[pos]));
      if (cfile[pos] == '.')
        while (++pos < length and is_digit(cfile[pos]));
      if (cfile[pos] == 'e' or cfile[pos] == 'E') { // Accept exponents
        if (cfile[++pos] == '-') ++pos;
        while (pos < length and is_digit(cfile[pos])) ++pos;
      }
      while (pos < length and is_letter(cfile[pos])) ++pos; // Include the flags, like ull
      return token_t(token_basics(TT_DECLITERAL,filename,line,pos-lpos), cfile+sp, pos-sp);
    }
    
    //============================================================================================
    //====: Not at a number. Find out where we are. :=============================================
    //============================================================================================
    
    const size_t spos = pos;
    switch (cfile[pos++])
    {
      case ';':
        return token_t(token_basics(TT_SEMICOLON,filename,line,spos-lpos));
      case ',':
        return token_t(token_basics(TT_COMMA,filename,line,spos-lpos));
      case '+': case '-':
        pos += cfile[pos] == cfile[spos] or cfile[pos] == '=' or (cfile[pos] == '>' and cfile[spos] == '-');
        pos += (cfile[pos-1] == '>' and cfile[pos] == '*');
        return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), cfile+spos, pos-spos);
      case '=': pos += cfile[pos] == cfile[spos];
        return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), cfile+spos, pos-spos);
      case '&': case '|':  case '!':
        pos += cfile[pos] == cfile[spos] || cfile[pos] == '=';
        return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), cfile+spos, pos-spos);
      case '~':
        if (cfile[pos] == '=')
          return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), cfile+spos, ++pos-spos);
        return token_t(token_basics(TT_TILDE,filename,line,spos-lpos), cfile+spos, pos-spos);
      case '%': case '*': case '/': case '^':
        if (cfile[pos] == '=')
          return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), cfile+spos, ++pos-spos);
        return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), cfile+spos, pos-spos);
      case '>': case '<':
        pos += cfile[pos] == cfile[spos]; pos += cfile[pos] == '=';
        return token_t(token_basics((pos-spos==1?cfile[spos]=='<'?TT_LESSTHAN:TT_GREATERTHAN:TT_OPERATOR),filename,line,spos-lpos), cfile+spos, pos-spos);
      case ':':
        pos += cfile[pos] == cfile[spos];
        return token_t(token_basics(pos - spos == 1 ? TT_COLON : TT_SCOPE,filename,line,spos-lpos), cfile+spos, pos-spos);
      case '?':
        return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), cfile+spos, pos-spos);
      
      case '.':
          if (is_digit(cfile[pos]))
            goto handle_decimal;
          else if (cfile[pos] == '.') {
            if (cfile[++pos] == '.')
              return token_t(token_basics(TT_ELLIPSIS,filename,line,spos-lpos), cfile+spos, pos++ - spos);
            else
              pos -= 2;
          }
          pos += cfile[pos] == '*';
        return token_t(token_basics(TT_OPERATOR,filename,line,spos-lpos), cfile+spos, pos-spos);
      
      case '(': return token_t(token_basics(TT_LEFTPARENTH,filename,line,spos-lpos));
      case '[': return token_t(token_basics(TT_LEFTBRACKET,filename,line,spos-lpos));
      case '{': return token_t(token_basics(TT_LEFTBRACE,  filename,line,spos-lpos));
      case '}': return token_t(token_basics(TT_RIGHTBRACE,  filename,line,spos-lpos));
      case ']': return token_t(token_basics(TT_RIGHTBRACKET,filename,line,spos-lpos));
      case ')': return token_t(token_basics(TT_RIGHTPARENTH,filename,line,spos-lpos));
      
      case '#':
        handle_preprocessor(herr);
        goto get_token_top; // Unoptimized tail call leads to death and destruction
      
      case '\\':
        if (cfile[pos] != '\n' and cfile[pos] != '\r')
          herr->error("Stray backslash", filename, line, pos-lpos);
        continue;
      
      case '"': {
        --pos; skip_string(herr);
        return token_t(token_basics(TT_STRINGLITERAL,filename,line,spos-lpos), cfile + spos, ++pos-spos);
      }
      
      case '\'': {
        --pos; skip_string(herr);
        return token_t(token_basics(TT_STRINGLITERAL,filename,line,spos-lpos), cfile + spos, ++pos-spos);
      }
      
      default: {
        char errbuf[320];
        sprintf(errbuf, "Unrecognized symbol (char)0x%02X '%c'", (int)cfile[spos], cfile[spos]);
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

bool lexer_cpp::pop_file() {
  if (files.empty())
    return true;
  
  // Close whatever file we have open now
  close();
  
  // Fetch data from top item
  openfile& of = files.top();
  line = of.line, lpos = of.lpos;
  filename = of.filename;
  consume(of.file);
  
  // Pop file stack and return next token in the containing file.
  files.pop();
  
  // Remove any open macro
  if (open_macro_count)
      --open_macro_count;
  
  return false;
}

macro_map lexer_cpp::kludge_map;
lexer_cpp::keyword_map lexer_cpp::keywords;
lexer_cpp::lexer_cpp(llreader &input, macro_map &pmacros, const char *fname): macros(pmacros), filename(fname), line(1), lpos(0), open_macro_count(0), mlex(new lexer_macro(this))
{
  consume(input); // We are also an llreader. Consume the given one using the inherited method.
  if (keywords.empty()) {
    keywords["asm"] = TT_ASM;
    keywords["__asm"] = TT_ASM;
    keywords["__asm__"] = TT_ASM;
    keywords["class"] = TT_CLASS;
    keywords["decltype"] = TT_DECLTYPE;
    keywords["enum"] = TT_ENUM;
    keywords["extern"] = TT_EXTERN;
    keywords["namespace"] = TT_NAMESPACE;
    keywords["operator"] = TT_OPERATORKW;
    keywords["private"] = TT_PRIVATE;
    keywords["protected"] = TT_PROTECTED;
    keywords["public"] = TT_PUBLIC;
    keywords["sizeof"] = TT_SIZEOF;
    keywords["__is_empty"] = TT_ISEMPTY;
    keywords["struct"] = TT_STRUCT;
    keywords["template"] = TT_TEMPLATE;
    keywords["typedef"] = TT_TYPEDEF;
    keywords["typename"] = TT_TYPENAME;
    keywords["union"] = TT_UNION;
    keywords["using"] = TT_USING;
    keywords["new"] = TT_NEW;
    keywords["delete"] = TT_DELETE;
    
    // GNU Extensions
    keywords["__attribute__"] = TT_INVALID;
    keywords["__extension__"] = TT_INVALID;
    keywords["__typeof__"] = TT_INVALID;
    keywords["__typeof"] = TT_INVALID;
   // keywords["__restrict"] = TT_INVALID;
    
    // MinGW Fuckery
    keywords["__MINGW_IMPORT"] = TT_INVALID;
    
    // C++ Extensions
    keywords["false"] = TT_INVALID;
    keywords["true"] = TT_INVALID;
    
    string x(1,'x');
    kludge_map.clear();
    context::global_macros().swap(kludge_map);
    builtin->add_macro_func("__attribute__", x, string(), false);
    builtin->add_macro_func("__typeof__", x, string("int"), false);
    builtin->add_macro_func("__typeof", x, string("int"), false);
    builtin->add_macro("__extension__", string());
    builtin->add_macro("__MINGW_IMPORT", string());
    builtin->add_macro("false", string(1,'0'));
    builtin->add_macro("true", string(1,'1'));
    context::global_macros().swap(kludge_map);
  }
}
lexer_cpp::~lexer_cpp() {
  delete mlex;
}

void lexer_cpp::cleanup() {
  keywords.clear();
  for (macro_iter it = kludge_map.begin(); it != kludge_map.end(); ++it)
    macro_type::free(it->second);
  kludge_map.clear();
}

openfile::openfile() {}
openfile::openfile(const char* fname): filename(fname), line(0), lpos(0) {}
openfile::openfile(const char* fname, size_t line_num, size_t line_pos, llreader &consume): filename(fname), line(line_num), lpos(line_pos) { file.consume(consume); }
void openfile::swap(openfile &f) {
  { register const char* tmpl = filename;
  filename = f.filename, f.filename = tmpl; }
  register size_t tmpl = line;
  line = f.line, f.line = tmpl;
  tmpl = lpos, lpos = f.lpos, f.lpos = tmpl;
  llreader tmpr;
  tmpr.consume(file);
  file.consume(f.file);
  f.file.consume(tmpr);
}

#undef cfile
lexer_macro::lexer_macro(lexer_cpp *enc): pos(enc->pos), lcpp(enc) { }
void lexer_macro::update() { cfile = lcpp->data; length = lcpp->length; }
token_t lexer_macro::get_token(error_handler *herr)
{
  #ifdef DEBUG_MODE
    static int number_of_times_GDB_has_dropped_its_ass_in_this_function = 0;
    ++number_of_times_GDB_has_dropped_its_ass_in_this_function;
  #endif
  
  for (;;) // Loop until we find something or hit world's end
  {
    if (pos >= length) {
      if (lcpp->pop_file())
        return token_t(token_basics(TT_ENDOFCODE,lcpp->filename,lcpp->line,pos-lcpp->lpos));
      update(); continue;
    }
    // Skip all whitespace
    if (cfile[pos] == ' ' or cfile[pos] == '\t') { ++pos; continue; }
    if (cfile[pos] == '\n' or cfile[pos] == '\r') return token_t(token_basics(TT_ENDOFCODE,lcpp->filename,lcpp->line,pos-lcpp->lpos));
    
    //============================================================================================
    //====: Check for and handle comments. :======================================================
    //============================================================================================
    
    if (cfile[pos] == '/') {
      if (++pos < length) {
        if (cfile[pos] == '/') { lcpp->skip_comment(); continue; }
        if (cfile[pos] == '*') { lcpp->skip_multiline_comment(); continue; }
        if (cfile[pos] == '=')
          return token_t(token_basics(TT_OPERATOR,lcpp->filename,lcpp->line,pos-lcpp->lpos), cfile+pos-1, 2);
      }
      return token_t(token_basics(TT_OPERATOR,lcpp->filename,lcpp->line,pos-lcpp->lpos), cfile+pos-1,1);
    }
    
    //============================================================================================
    //====: Not at a comment. See if we're at an identifier. :====================================
    //============================================================================================
    
    if (is_letter(cfile[pos])) // Check if we're at an identifier or keyword.
    {
      const size_t sspos = pos;
      const char* sp = cfile + pos; // Record where we are
      while (++pos < length and is_letterd(cfile[pos]));
      
      if (*sp == 'L' and cfile+pos - sp == 1 and cfile[pos] == '\'') {
        lcpp->skip_string(herr);
        return token_t(token_basics(TT_CHARLITERAL,lcpp->filename,lcpp->line,sspos-lcpp->lpos), sp, ++pos - sspos);
      }
      
      string fn(sp, cfile + pos); // We'll need a copy of this thing for lookup purposes
      
      static const char zero[] = "0", one[] = "1";
      
      if (fn == "defined") {
        while (is_useless_macros(cfile[pos])) ++pos;
        bool endpar = cfile[pos] == '(';
        if (endpar) while (is_useless_macros(cfile[++pos]));
        
        if (!is_letter(cfile[pos])) {
          herr->error("Expected identifier to look up as macro", lcpp->filename,lcpp->line,pos-lcpp->lpos);
          continue;
        }
        
        const size_t spos = pos;
        while (is_letterd(cfile[++pos]));
        string macro(cfile + spos, pos-spos);
        
        if (endpar) {
          while (is_useless_macros(cfile[pos])) ++pos;
          if (cfile[pos] != ')') herr->error("Expected ending parenthesis for defined()", lcpp->filename,lcpp->line,pos-lcpp->lpos);
          pos++;
        }
        
        return token_t(token_basics(TT_DECLITERAL,lcpp->filename,lcpp->line,pos-lcpp->lpos), lcpp->macros.find(macro)==lcpp->macros.end()? zero : one, 1);
      }
      
      macro_iter mi = lcpp->macros.find(fn);
      if (mi != lcpp->macros.end()) {
        if (mi->second->argc < 0) {
          lcpp->enter_macro((macro_scalar*)mi->second);
          update();
          continue;
        }
        else {
          if (lcpp->parse_macro_function((macro_function*)mi->second, herr)) {
            update();
            continue;
          }
        }
      }
      
      return token_t(token_basics(TT_DECLITERAL,lcpp->filename,lcpp->line,pos-lcpp->lpos), zero, 1);
    }
    
    //============================================================================================
    //====: Not at an identifier. Maybe at a number? :============================================
    //============================================================================================
    
    if (is_digit(cfile[pos])) {
      if (cfile[pos] == '0') { // Check if the number is hexadecimal or octal.
        if (cfile[++pos] == 'x') { // Check if the number is hexadecimal.
          // Yes, it is hexadecimal.
          const size_t sp = pos;
          while (++pos < length and is_hexdigit(cfile[pos]));
          while (pos < length and is_letter(cfile[pos])) pos++; // Include the flags, like ull
          return token_t(token_basics(TT_HEXLITERAL,lcpp->filename,lcpp->line,pos-lcpp->lpos), cfile+sp, pos-sp);  
        }
        // Turns out, it's octal.
        const size_t sp = --pos;
        while (++pos < length and is_hexdigit(cfile[pos]));
        while (pos < length and is_letter(cfile[pos])) pos++; // Include the flags, like ull
        return token_t(token_basics(TT_OCTLITERAL,lcpp->filename,lcpp->line,pos-lcpp->lpos), cfile+sp, pos-sp);
      }
      // Turns out, it's decimal.
      const size_t sp = pos;
      while (++pos < length and is_digit(cfile[pos]));
      if (cfile[pos-1] == 'e' or cfile[pos-1] == 'E') { // Accept exponents
        if (cfile[pos] == '-') ++pos;
        while (pos < length and is_digit(cfile[pos])) ++pos;
      }
      while (pos < length and is_letter(cfile[pos])) ++pos; // Include the flags, like ull
      return token_t(token_basics(TT_DECLITERAL,lcpp->filename,lcpp->line,pos-lcpp->lpos), cfile+sp, pos-sp);
    }
    
    
    //============================================================================================
    //====: Not at a number. Find out where we are. :=============================================
    //============================================================================================
    
    const size_t spos = pos;
    switch (cfile[pos++])
    {
      case ';':
        return token_t(token_basics(TT_SEMICOLON,lcpp->filename,lcpp->line,spos-lcpp->lpos));
      case ',':
        return token_t(token_basics(TT_COMMA,lcpp->filename,lcpp->line,spos-lcpp->lpos));
      case '+': case '-':
        pos += cfile[pos] == cfile[spos] or cfile[pos] == '=';
        return token_t(token_basics(TT_OPERATOR,lcpp->filename,lcpp->line,spos-lcpp->lpos), cfile+spos, pos-spos);
      case '=': pos += cfile[pos] == cfile[spos]; case '*': case '/': case '^':
        return token_t(token_basics(TT_OPERATOR,lcpp->filename,lcpp->line,spos-lcpp->lpos), cfile+spos, pos-spos);
      case '&': case '|':  case '!':
        pos += cfile[pos] == cfile[spos] || cfile[pos] == '=';
        return token_t(token_basics(TT_OPERATOR,lcpp->filename,lcpp->line,spos-lcpp->lpos), cfile+spos, pos-spos);
      case '~': case '%':
        if (cfile[pos] == '=')
          return token_t(token_basics(TT_OPERATOR,lcpp->filename,lcpp->line,spos-lcpp->lpos), cfile+spos, ++pos-spos);
        return token_t(token_basics(TT_TILDE,lcpp->filename,lcpp->line,spos-lcpp->lpos), cfile+spos, pos-spos);
      case '>': case '<':
        pos += cfile[pos] == cfile[spos]; pos += cfile[pos] == '=';
        return token_t(token_basics(TT_OPERATOR,lcpp->filename,lcpp->line,spos-lcpp->lpos), cfile+spos, pos-spos);
      case ':':
        pos += cfile[pos] == cfile[spos];
        return token_t(token_basics(pos - spos == 1 ? TT_COLON : TT_SCOPE,lcpp->filename,lcpp->line,spos-lcpp->lpos), cfile+spos, pos-spos);
      case '?':
        return token_t(token_basics(TT_OPERATOR,lcpp->filename,lcpp->line,spos-lcpp->lpos), cfile+spos, pos-spos);
        
      case '(': return token_t(token_basics(TT_LEFTPARENTH,lcpp->filename,lcpp->line,spos-lcpp->lpos));
      case '[': return token_t(token_basics(TT_LEFTBRACKET,lcpp->filename,lcpp->line,spos-lcpp->lpos));
      case '{': return token_t(token_basics(TT_LEFTBRACE,  lcpp->filename,lcpp->line,spos-lcpp->lpos));
      case '}': return token_t(token_basics(TT_RIGHTBRACE,  lcpp->filename,lcpp->line,spos-lcpp->lpos));
      case ']': return token_t(token_basics(TT_RIGHTBRACKET,lcpp->filename,lcpp->line,spos-lcpp->lpos));
      case ')': return token_t(token_basics(TT_RIGHTPARENTH,lcpp->filename,lcpp->line,spos-lcpp->lpos));
      
      case '#':
          if (cfile[pos] == '#')
            return token_t(token_basics(TTM_CONCAT,lcpp->filename,lcpp->line,spos-lcpp->lpos));
        return token_t(token_basics(TTM_TOSTRING,lcpp->filename,lcpp->line,spos-lcpp->lpos));
      
      case '\\':
          if (cfile[pos] == '\n' or (cfile[pos] == '\r' and (cfile[++pos] == '\n' or pos--)))
            lcpp->lpos = pos++, ++lcpp->line;
        return get_token(herr);
        
      case '\"': {
        --pos; lcpp->skip_string(herr);
        return token_t(token_basics(TT_STRINGLITERAL,lcpp->filename,lcpp->line,spos-lcpp->lpos), cfile + spos, ++pos-spos);
      }
      case '\'': {
        --pos; lcpp->skip_string(herr);
        return token_t(token_basics(TT_STRINGLITERAL,lcpp->filename,lcpp->line,spos-lcpp->lpos), cfile + spos, ++pos-spos);
      }
      
      default:
        return token_t(token_basics(TT_INVALID,lcpp->filename,lcpp->line,pos-lcpp->lpos++));
    }
  }
}

lexer_cpp::condition::condition() {}
lexer_cpp::condition::condition(bool t, bool cbt): is_true(t), can_be_true(cbt) {}
