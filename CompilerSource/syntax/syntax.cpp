/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <map>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

#include "settings.h"
#include "externs/externs.h"
#include "general/parse_basics.h"
#include "general/macro_integration.h"
#include "parser/object_storage.h"

using namespace std;

extern string tostring(int);

namespace syncheck
{
  enum TT {
    TT_VARNAME,         // somevar
    TT_SEMICOLON,       // A semicolon.
    TT_COLON,           // A colon.
    TT_COMMA,           // A comma.
    TT_ASSOP,           // = += -= *= /= ...
    TT_DECIMAL,         // A dot. '.'. Whether for number or for access is determined in the second pass.
    TT_OPERATOR,        // Basic operators. = + - *...
    TT_UNARYPRE,        // Unary prefix operators. + - ! ++(int) --(int)
    TT_UNARYPOST,       // Unary postfix operators. ++ --
    TT_TERNARY,         // ?
    TT_BEGINPARENTH,    // (
    TT_ENDPARENTH,      // )
    TT_BEGINBRACKET,    // [
    TT_ENDBRACKET,      // ]
    TT_BEGINBRACE,      // {
    TT_ENDBRACE,        // }
    TT_BEGINTRIANGLE,   // <
    TT_ENDTRIANGLE,     // >
    TT_DIGIT,           // 0 1 2... (...)
    TT_STRING,          // "", ''
    TT_SCOPEACCESS,     // ::
    TT_FUNCTION,   // game_end
    TT_TYPE_NAME,       // int, double, whatever
    TT_NAMESPACE,       // std, enigma, sf
    TT_LOCGLOBAL,       // global/local
    TT_GEN_STATEMENT,   // Generic statements; if while switch
    TT_SHORTSTATEMENT,  // Short statements; return, mostly
    TT_TINYSTATEMENT,   // break, continue, exit...
    TT_S_SWITCH,        // switch
    TT_S_CASE,          // case
    TT_S_DEFAULT,       // default
    TT_S_FOR,           // for
    TT_S_IF,            // if
    TT_S_ELSE,          // else
    TT_S_TRY,           // try
    TT_S_CATCH,         // catch
    TT_S_DO,            // do
    TT_S_NEW,           // new
    
    TT_IMPLICIT_SEMICOLON,
    TT_ERROR = -1
  };
  
  const char* TTN[] = {
    "TT_VARNAME",
    "TT_SEMICOLON",
    "TT_COLON",
    "TT_COMMA",
    "TT_ASSOP",
    "TT_DECIMAL",
    "TT_OPERATOR",
    "TT_UNARYPRE",
    "TT_UNARYPOST",
    "TT_TERNARY",
    "TT_BEGINPARENTH",
    "TT_ENDPARENTH",
    "TT_BEGINBRACKET",
    "TT_ENDBRACKET",
    "TT_BEGINBRACE",
    "TT_ENDBRACE",
    "TT_BEGINTRIANGLE",
    "TT_ENDTRIANGLE",
    "TT_DIGIT",
    "TT_STRING",
    "TT_SCOPEACCESS",
    "TT_FUNCTION",
    "TT_TYPE_NAME",
    "TT_LOCGLOBAL",
    "TT_GEN_STATEMENT",
    "TT_SHORTSTATEMENT",
    "TT_TINYSTATEMENT",
    "TT_S_SWITCH",
    "TT_S_CASE",
    "TT_S_DEFAULT",
    "TT_S_FOR",
    "TT_S_IF",
    "TT_S_ELSE",
    "TT_S_TRY",
    "TT_S_CATCH",
    "TT_S_DO",
    "TT_S_NEW",
    "<implicit semicolon>"
  };
  struct token {
    TT type;
    string content;
    pt pos, length;
    bool separator,      // Separator is true for, ie, any of "; , { ( [  }". 
         breakandfollow, // Break and follow is true for "] )" and strings/varnames/digits.
         operatorlike;   // Operator like is true for OPERATOR, ASSOP, (, [, and statements expecting an expression.
    int macrolevel;
    unsigned match; // The index of the matching parenthesis/bracket/brace in the lex
    externs* ext;
    
    token(): type(TT_ERROR) {}
    token(TT t,              string ct, pt p,pt l,bool s,bool bnf,bool ol,int ml): type(t), content(ct), pos(p), length(l), separator(s), breakandfollow(bnf), operatorlike(ol), macrolevel(ml), match(0), ext(NULL) {}
    token(TT t, unsigned m,  string ct, pt p,pt l,bool s,bool bnf,bool ol,int ml): type(t), content(ct), pos(p), length(l), separator(s), breakandfollow(bnf), operatorlike(ol), macrolevel(ml), match(m), ext(NULL) {}
    token(TT t, externs *ex, string ct, pt p,pt l,bool s,bool bnf,bool ol,int ml): type(t), content(ct), pos(p), length(l), separator(s), breakandfollow(bnf), operatorlike(ol), macrolevel(ml), match(0), ext(ex) {}
    operator string() { 
      char buf[12]; sprintf(buf,"%lu",(long unsigned)pos);
      string str = string(TTN[type]) + "{" + buf + ", ";
      sprintf(buf,"%lu",(long unsigned)length); str += buf; str += ", ";
      str += separator ? "separated, " : "unseparated, ";
      str += breakandfollow ? "breakandfollow, " : "nobreak, ";
      str += operatorlike ? "operatorlike, " : "notoperator, ";
      sprintf(buf,"%d",macrolevel); str += buf;
      return str + "}";
    }
  };
  
  string syerr;
  vector<token> lex;
  
  struct open_parenth_info {
    unsigned ind;
    int macrolevel;
    char type;
    open_parenth_info() {}
    open_parenth_info(unsigned i, int ml, char type): ind(i), macrolevel(ml), type(type) {}
  };
  pt pop_open_parenthesis(vector<open_parenth_info> &ops, vector<token>& lex, pt pos, int ind, char opener, string whathavewe) {
    if (ops.empty()) {
      syerr = "Unexpected " + whathavewe + ": None open."; return pos;
    }
    const char p = ops[ops.size()-1].type;
    if (p != opener) {
      syerr = "Expected closing " + string(p == '(' ? "parenthesis" : p == '[' ? "bracket" : p == '{' ? "brace" : "triangle bracket") + " before " + whathavewe;
      return pos;
    }
    const int oi = ops[ops.size()-1].ind;
    lex[oi].match = ind;
    lex[ind].match = oi;
    ops.pop_back();
    return pt(-1);
  }
  
  #include "syntaxtools.h"
  
  map<string,int> scripts;
  void addscr(string name) {
    scripts[name]++;
  }
  
  #define superPos (mymacroind ? mymacrostack[0].pos : pos)
  #define ptrace() for (unsigned i = 0; i < lex.size(); i++) cout << (string)lex[i] << "\t\t" << endl
  #define lexlast (lex.size()-1)
  int syntacheck(string code)
  {
    pt pos = 0;
    unsigned mymacroind = 0;
    macro_stack_t mymacrostack;
    lex.clear(); 
    lex.push_back(token(TT_IMPLICIT_SEMICOLON, ";", superPos, 0, true, false, false, mymacroind));
          
    
    syerr = "No error";
    vector<open_parenth_info> open_parenths; // Any open brace, bracket, or parenthesis
    
    // First, collapse everything into a massive lex vector, 
    // Doing minor syntax checking along the way
    for (;;)
    {
      // Handle end of code
      if (pos >= code.length()) {
        if (mymacroind)
          mymacrostack[--mymacroind].release(code,pos);
        else break; continue;
      }
      
      if (is_useless(code[pos])) {
        while (is_useless(code[++pos]));
        continue;
      }
      
      if (is_letter(code[pos]))
      {
        const pt spos = pos;
        while (is_letterd(code[++pos]));
        const string name = code.substr(spos,pos-spos);
        
        // First, check if it's a macro.
        maciter mi = macros.find(name);
        if (mi != macros.end())
        {
          if (!macro_recurses(name,mymacrostack,mymacroind))
          {
            string macrostr = mi->second;
            if (mi->second.argc != -1) //Expect ()
            {
              pt cwp = skip_comments(code,pos);
              if (code[cwp] != '(')
                goto not_a_macro;
              if (!macro_function_parse(code.c_str(),code.length(),name,pos,macrostr,mi->second.args,mi->second.argc,mi->second.args_uat,false,true)) {
                syerr = macrostr;
                return superPos;
              }
            }
            mymacrostack[mymacroind++].grab(name,code,pos);
            code = macrostr; pos = 0;
          }
          continue;
        }
        
        not_a_macro:
        if (is_wordop(name)) { //this is actually a word-based operator, such as `and', `or', and `not' 
          bool unary = name[0] == 'n'; //not is the only unary word operator.
          if (unary and (!lex[lexlast].separator and !lex[lexlast].operatorlike)) {
            syerr = "Unexpected unary keyword `not'";
            return pos;
          }
          if (!unary and (!lex.size() or lex[lexlast].separator or lex[lexlast].operatorlike)) {
            syerr = "Expected primary expression before `" + name + "' operator";
            return pos;
          }
          lex.push_back(token(unary ? TT_UNARYPRE : TT_OPERATOR, name, superPos, name.length(), false, false, true, mymacroind));
          continue;
        }
        
        if (shared_object_locals.find(name) == shared_object_locals.end())
        {
          if (find_extname(name, 0xFFFFFFFF))
          {
            // Handle typenames
            if (ext_retriever_var->flags & EXTFLAG_TYPENAME) {
              if (lex[lexlast].type == TT_TYPE_NAME)
                lex[lexlast].length = superPos - lex[lexlast].pos + name.length();
              else {
                if (lex[lexlast].breakandfollow)
                  lex.push_back(token(TT_IMPLICIT_SEMICOLON, ";", superPos, 0, true, false, false, mymacroind));
                lex.push_back(token(TT_TYPE_NAME, ext_retriever_var, name, superPos, name.length(), false, false, false, mymacroind));
              }
              continue;
            }
            
            if (lex[lexlast].breakandfollow)
              lex.push_back(token(TT_IMPLICIT_SEMICOLON, ";", superPos, 0, true, false, false, mymacroind));
            
            if (ext_retriever_var->flags & EXTFLAG_NAMESPACE) {
              lex.push_back(token(TT_NAMESPACE, ext_retriever_var, name, superPos, name.length(), false, false, false, mymacroind));
              continue;
            }
            
            if (ext_retriever_var->is_function()) {
              lex.push_back(token(TT_FUNCTION, ext_retriever_var, name, superPos, name.length(), false, true, false, mymacroind));
              continue;
            }
            // Global variables
            lex.push_back(token(TT_VARNAME, ext_retriever_var, name, superPos, name.length(), false, true, false, mymacroind));
            continue;
          }
          
          if (is_statement(name)) // Our control statements
          {
            if (!lex[lexlast].separator and !lex[lexlast].operatorlike)
            {
              if (lex[lexlast].breakandfollow)
                lex.push_back(token(TT_IMPLICIT_SEMICOLON, ";", superPos, 0, true, false, false, mymacroind));
              else if (lex[lexlast].type != TT_S_ELSE and lex[lexlast].type != TT_S_TRY)
              {
                ptrace();
                syerr = "Unexpected `" + name + "' statement at this point";
                return superPos;
              }
            }
            TT mt = statement_type(name);
            lex.push_back(token(mt, name, superPos, name.length(), false, mt == TT_TINYSTATEMENT, mt != TT_TINYSTATEMENT, mymacroind));
            continue;
          }
        }
        if (lex[lexlast].breakandfollow)
          lex.push_back(token(TT_IMPLICIT_SEMICOLON, ";", superPos, 0, true, false, false, mymacroind));
        
        lex.push_back(token(TT_VARNAME, name, superPos, name.length(), false, true, false, mymacroind));
        continue;
      }
      
      if (is_digit(code[pos]))
      {
        if (!lex[lexlast].separator and !lex[lexlast].operatorlike) {
          lex.push_back(token(TT_IMPLICIT_SEMICOLON, ";", superPos, 0, true, false, false, mymacroind));
        }
        const pt spos = pos;
        while (is_digit(code[++pos]));
        lex.push_back(token(TT_DIGIT, code.substr(spos,pos-spos), superPos, pos-spos, false, true, false, mymacroind));
        continue;
      }
      
      switch (code[pos]) {
        case ';':
            lex.push_back(token(TT_SEMICOLON, ";", superPos, 1, true, false, false, mymacroind));
          pos++; continue;
        case ':':
            if (code[pos+1] == '=')
              lex.push_back(token(TT_ASSOP, ":", superPos, 2, true, false, false, mymacroind)), pos += 2;
            else if (code[pos+1] == ':') {
              if (lex[lexlast].type != TT_NAMESPACE)
              {
                if (!lex[lexlast].separator)
                  return (syerr = lex[lexlast].type == TT_VARNAME?"Unexpected namespace accessor: `"+lex[lexlast].content+"' is not a namespace":"Unexpected namespace accessor at this point", superPos);
                immediate_scope = &global_scope;
              } else
                immediate_scope = lex[lexlast].ext;
              lex.push_back(token(TT_SCOPEACCESS, "::", superPos, 2, false, false, true, mymacroind)), pos += 2;
            }
            else
              lex.push_back(token(TT_COLON, ":", superPos, 1, true, false, false, mymacroind)), ++pos;
          continue;
        case ',':
            lex.push_back(token(TT_COMMA, ",", superPos, 1, true, false, true, mymacroind));
          pos++; continue;
        case '$': {
            const pt spos = pos;
            if (!is_hexdigit(code[++pos])) {
              syerr = "Hexadecimal literal expected after '$' symbol";
              return superPos;
            }
            while (is_hexdigit(code[++pos]));
            lex.push_back(token(TT_DIGIT, code.substr(spos,pos-spos+1), superPos, pos-spos, false, true, false, mymacroind));
          } continue;
          
        pt open_error;
        case '{':
            if (lex[lexlast].type == TT_OPERATOR)
              return (syerr = "Expected secondary expression before brace", superPos);
            lex.push_back(token(TT_BEGINBRACE, "{", superPos, 1, true, false, false, mymacroind));
            open_parenths.push_back(open_parenth_info(lexlast, mymacroind, '{'));
          pos++; continue;
        case '}':
            if (lex[lexlast].operatorlike)
              return (syerr = "Expected identifier before closing brace", superPos);
            if (open_parenths.size())
              lex.push_back(token(TT_ENDBRACE, open_parenths[open_parenths.size()-1].ind, "}", superPos, 1, true, false, false, mymacroind));
            open_error = pop_open_parenthesis(open_parenths, lex, superPos, lexlast, '{', "closing brace");
            if (open_error != pt(-1)) return open_error;
          pos++; continue;
        case '[':
            if (lex[lexlast].operatorlike)
              return (syerr = "Expected identifier before bracket; ENIGMA arrays not yet implemented", superPos);
            lex.push_back(token(TT_BEGINBRACKET, "[", superPos, 1, true, false, true, mymacroind));
            open_parenths.push_back(open_parenth_info(lexlast, mymacroind, '['));
          pos++; continue;
        case ']':
            if (lex[lexlast].operatorlike and lex[lexlast].type != TT_BEGINBRACKET)
              return (syerr = "Expected secondary expression before closing bracket", superPos);
            if (open_parenths.size())
              lex.push_back(token(TT_ENDBRACKET, open_parenths[open_parenths.size()-1].ind, "]",superPos, 1, false, true, false, mymacroind));
            open_error = pop_open_parenthesis(open_parenths, lex, superPos, lexlast, '[', "closing bracket");
            if (open_error != pt(-1)) return open_error;
          pos++; continue;
        case '(':
            lex.push_back(token(TT_BEGINPARENTH, "(", superPos, 1, true, false, true, mymacroind));
            open_parenths.push_back(open_parenth_info(lexlast, mymacroind, '('));
          pos++; continue;
        case ')':
            if (lex[lexlast].operatorlike and lex[lexlast].type != TT_BEGINPARENTH)
              return (syerr = "Expected secondary expression before closing parenthesis", superPos);
            if (open_parenths.size())
              lex.push_back(token(TT_ENDPARENTH, open_parenths[open_parenths.size()-1].ind, ")", superPos, 1, false, lex[lexlast].type != TT_TYPE_NAME, false, mymacroind));
            open_error = pop_open_parenthesis(open_parenths, lex, superPos, lexlast, '(', "closing parenthesis");
            if (open_error != pt(-1)) return open_error;
          pos++; continue;
        
        case '.': // We can't really do checking on this yet. It's one of the reasons we have two passes.
            lex.push_back(token(TT_DECIMAL, ".", superPos, 1, false, true, false, mymacroind)); ++pos;
          break;
        
        pt spos;
        case '"':
            spos = pos;
            if (setting::use_cpp_escapes)
              while (code[++pos]!='"') {
                if (pos >= code.length()) {
                  syerr = "Unclosed double quote at this point";
                  return superPos;
                }
                if (code[pos] == '\\')
                  pos++;
              }
            else
              while (code[++pos]!='"');
            lex.push_back(token(TT_STRING, code.substr(spos,pos-spos+1), superPos, pos-spos, false, true, false, mymacroind));
          pos++; break;
        case '\'':
            spos = pos;
            if (setting::use_cpp_escapes)
              while (code[++pos] != '\'') {
                if (pos >= code.length()) {
                  syerr = "Unclosed quote at this point";
                  return superPos;
                }
                if (code[pos] == '\\')
                  pos++;
              }
            else
              while (code[++pos]!='\'');
            lex.push_back(token(TT_STRING, code.substr(spos,pos-spos+1), superPos, pos-spos, false, true, false, mymacroind));
          pos++; break;
        
        case '?':
            if (!lex.size() or lex[lexlast].separator or lex[lexlast].operatorlike) {
              syerr = "Primary expression expected before ternary operator";
              return superPos;
            }
            lex.push_back(token(TT_TERNARY, "?", superPos, 1, false, true, true, mymacroind));
            pos++;
          break;
        
        int sz;
        case '+': case '-':
            sz = 1 + (code[pos+1] == code[pos] and setting::use_incrementals);
            if (!lex.size() or lex[lexlast].separator or lex[lexlast].operatorlike)
              lex.push_back(token(TT_UNARYPRE, code.substr(pos,sz), superPos, sz, false, false, true, mymacroind));
            else if (sz == 2)
              lex.push_back(token(TT_UNARYPOST, code.substr(pos,sz), superPos, sz, false, true, false, mymacroind));
            else if (code[pos+1] == '=')
              lex.push_back(token(TT_ASSOP, code.substr(pos,2), superPos, sz = 2, false, false, true, mymacroind));
            else
              lex.push_back(token(TT_OPERATOR, code.substr(pos,sz), superPos, sz, false, false, true, mymacroind));
            pos += sz;
          break;
        case '*':
            if (!lex.size() or lex[lexlast].separator or lex[lexlast].operatorlike)
              lex.push_back(token(TT_UNARYPRE, "*", superPos, 1, false, false, true, mymacroind)), ++pos;
            else if (code[pos+1] == '=')
              lex.push_back(token(TT_ASSOP, "*=", superPos, 2, false, false, true, mymacroind)), pos += 2;
            else
              lex.push_back(token(TT_OPERATOR, "*", superPos, 1, false, false, true, mymacroind)), ++pos;
          break;
        case '>': case '<':
            if (code[pos] == '<')
              if (lex[lexlast].type == TT_TYPE_NAME) {
                lex.push_back(token(TT_BEGINTRIANGLE, "<", superPos, 1, false, false, false, mymacroind)), pos++;
                open_parenths.push_back(open_parenth_info(lexlast, mymacroind, '<'));
                continue;
              } else;
            else if (open_parenths.size() && open_parenths[open_parenths.size()-1].type == '<') {
              lex.push_back(token(TT_ENDTRIANGLE, ">", superPos, 1, false, false, false, mymacroind)), pos++;
              open_error = pop_open_parenthesis(open_parenths, lex, superPos, lexlast, '<', "closing triangle bracket");
              if (open_error != pt(-1)) return open_error;
              pos++; continue;
            }
        case '&': case '|': case '^':
            if (!lex.size() or lex[lexlast].separator or lex[lexlast].operatorlike) {
              if (code[pos] != '&') {
                syerr = "Expected primary expression before operator";
                return superPos;
              }
              lex.push_back(token(TT_UNARYPRE, code.substr(pos,1), superPos, 1, false, false, true, mymacroind)), ++pos;
            }
            else if (code[pos+1] == code[pos] or (code[pos+1] == '>' and code[pos] == '<')) {
              if ((code[pos] == '<' or code[pos] == '>') and code[pos] == code[pos+1] and code[pos+2] == '=')
                lex.push_back(token(code[pos] != code[pos+1] ? TT_OPERATOR : TT_ASSOP, code.substr(pos,3), superPos, 3, false, false, true, mymacroind)), pos += 3; // <<= >>=
              else
                lex.push_back(token(TT_OPERATOR, code.substr(pos,2), superPos, 2, false, false, true, mymacroind)), pos += 2; // << >> && || ^^ <>
            }
            else if (code[pos+1] == '=')
              lex.push_back(token((code[pos] == '<' or code[pos] == '>')?TT_OPERATOR:TT_ASSOP, code.substr(pos,2), superPos, 2, false, false, true, mymacroind)), pos += 2; // >= <= &= |= ^=
            else
              lex.push_back(token(TT_OPERATOR, code.substr(pos,1), superPos, 1, false, false, true, mymacroind)), ++pos; // > < & | ^
          break;
        case '/':
            if (code[pos+1] == '/') { // Two-slash comments
              while (++pos < code.length() and code[pos] != '\n' and code[pos] != '\r');
              continue;
            }
            if (code[pos+1] == '*') { ++pos; // GM /**/ Comments
              while (++pos < code.length() and (code[pos] != '/' or code[pos-1] != '*'));
              pos++; continue;
            }
        case '%': 
            if (!lex.size() or lex[lexlast].separator or lex[lexlast].operatorlike) {
              syerr = "Primary expression expected before operator";
              return superPos;
            }
            if (code[pos+1] == '=')
              lex.push_back(token(TT_ASSOP, "%=", superPos, 2, false, false, true, mymacroind)), pos += 2; // %= /=
            else
              lex.push_back(token(TT_OPERATOR, "%=", superPos, 1, false, false, true, mymacroind)), ++pos;
          break;
        
        case '!':
            if (code[pos+1] == '=') {
              lex.push_back(token(TT_OPERATOR, "!=", superPos, 2, false, false, true, mymacroind)), pos += 2;
              continue;
            }
        case '~':
            if (!lex.size() or lex[lexlast].separator or lex[lexlast].operatorlike)
              lex.push_back(token(TT_UNARYPRE, code.substr(pos,1), superPos, 1, false, false, true, mymacroind)), ++pos; // ~ !
            else {
              ptrace();
              syerr = "Unexpected unary operator at this point";
              return superPos;
            }
          break;
        
        case '=':
            if (!lex.size() or lex[lexlast].separator or lex[lexlast].operatorlike) {
              syerr = "Primary expression expected before operator";
              return superPos;
            }
            if (lex[lexlast].type == TT_OPERATOR) {
              syerr = "Unexpected = at this point.";
              return superPos;
            }
            sz = (code[pos+1] == '=') + 1;
            lex.push_back(token(sz==2 ? TT_OPERATOR : TT_ASSOP, string(sz,'='), superPos, sz, false, false, true, mymacroind)), pos += sz; 
          break;
        default:
            syerr = "Unexpected symbol `" + code.substr(pos,1) + "': unknown to compiler";
          return superPos;
      }
    }
    
    if (open_parenths.size()) {
      const char p = open_parenths.rbegin()->type;
      syerr = "Unterminated " + string (p == '(' ? "parenthesis" : p == '[' ? "bracket" : p == '{' ? "brace" : "triangle bracket")
              + " at this point";
      return lex[open_parenths.rbegin()->ind].pos;
    }
    
    for (size_t i = 1; i < lex.size()-1; i++) {
      cout << lex[i].content << " ";
      switch (lex[i].type) 
      {
        case TT_VARNAME:
          if (lex[i+1].type == TT_BEGINPARENTH)
          {
            syerr = "Unknown function or script `" + lex[i].content + "'";
            if (lex[lex[i+1].match+1].type == TT_DECIMAL)
              syerr += ": use semicolon to separate object ID and variable name.";
            return lex[i].pos;
          }
          break;
        case TT_FUNCTION:
          if (lex[i+1].type != TT_BEGINPARENTH)
          {
            if (lex[i+1].type == TT_ASSOP)
              return (syerr = "Invalid assignment to function `" + lex[i].content + "'", lex[i+1].pos);
            if (lex[i+1].type == TT_ASSOP)
              return (syerr = "Invalid operation on function `" + lex[i].content + "'", lex[i+1].pos);
            continue;
          }
          else
          {
            bool contented = false;
            unsigned params = 0, exceeded_at = 0;
            const unsigned minarg = lex[i].ext->parameter_count_min();
            const unsigned maxarg = lex[i].ext->parameter_count_max();
            const unsigned lm = lex[i+1].match;
            for (unsigned ii = i+2; ii < lm; ii++)
            {
              if (lex[ii].type == TT_COMMA) {
                contented = false;
                if (params++ == maxarg)
                  exceeded_at = ii;
                continue;
              }
              contented = true;
              if (lex[ii].match)
                ii = lex[ii].match;
            }
            params += contented;
            if (!lex[i].ext->refstack.is_varargs()) {
              if (exceeded_at)
                return (syerr = "Too many arguments to function `" + lex[i].content + "': provided " + tostring(params) + ", allowed " + tostring(maxarg) + ".", lex[exceeded_at].pos);
              if (params > maxarg)
                return (syerr = "Too many arguments to function `" + lex[i].content + "': provided " + tostring(params) + ", allowed " + tostring(maxarg) + ".", lex[lm].pos);
            }
            if (params < minarg)
              return (syerr = "Too few arguments to function `" + lex[i].content + "': provided " + tostring(params) + ", required " + tostring(minarg) + ".", lex[lm].pos);
          }
          break;
        default: ;
      }
    }
    
    return -1;
  }
}

