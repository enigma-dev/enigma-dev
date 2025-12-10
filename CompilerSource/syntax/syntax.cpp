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
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
/*
#include "settings.h"
#include "general/parse_basics_old.h"
#include "general/macro_integration.h"
#include "parser/object_storage.h"

#include "parsing/tokens.h"

#include "../OS_Switchboard.h"
#ifdef WRITE_UNIMPLEMENTED_TXT
extern std::map <string, char> unimplemented_function_list;
#endif

#include <API/context.h>
#include <System/macros.h>
#include <System/lex_cpp.h>
#include <Storage/definition.h>
#include "languages/language_adapter.h"

using namespace std;

extern string tostring(int);

namespace {
  std::set<std::string> blacklist;
  using namespace enigma::parsing;
}

namespace syncheck
{
  #include "syntaxtools.h"

  map<string,int> scripts;
  void addscr(string name) {
    scripts[name]++;
  }

  #define superPos (mymacroind ? mymacrostack[0].pos : pos)
  #define ptrace() for (unsigned i = 0; i < lex.size(); i++) cout << (string)lex[i] << "\t\t" << endl
  #define lexlast (lex.size()-1)
  std::unique_ptr<AST> syntaxcheck(string raw_code) {
    auto result = std::make_unique<AST>(std::move(raw_code));

    //Build our blacklist.
    if (blacklist.empty()) {
      std::stringstream keyword;
      for (std::string::const_iterator it=setting::keyword_blacklist.begin(); it!=setting::keyword_blacklist.end(); it++) {
        char c = *it;
        if (c==',') {
          if (!keyword.str().empty()) {
            blacklist.insert(keyword.str());
            keyword.str("");
          }
        } else {
          keyword << c;
        }
      }
      if (!keyword.str().empty()) {
        blacklist.insert(keyword.str());
      }
    }

    pt pos = 0;
    vector<open_parenth_info> open_parenths; // Any open brace, bracket, or parenthesis

    // First, collapse everything into a massive lex vector.
    Lexer lex(code);

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
            #ifndef WRITE_UNIMPLEMENTED_TXT
            syerr = "Unknown function or script `" + lex[i].content + "'";
            if (lex[lex[i+1].match+1].type == TT_DECIMAL)
              syerr += ": use semicolon to separate object ID and variable name.";
            return lex[i].pos;
            #else
             unimplemented_function_list[lex[i].content] = 'U';
            #endif
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
            unsigned minarg, maxarg; current_language->definition_parameter_bounds(lex[i].ext, minarg, maxarg);
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

            #ifndef WRITE_UNIMPLEMENTED_TXT
            if (maxarg != (unsigned) -1) {
              if (exceeded_at)
                return (syerr = "Too many arguments to function `" + lex[i].content + "': provided " + tostring(params) + ", allowed " + tostring(maxarg) + ".", lex[exceeded_at].pos);
              if (params > maxarg)
                return (syerr = "Too many arguments to function `" + lex[i].content + "': provided " + tostring(params) + ", allowed " + tostring(maxarg) + ".", lex[lm].pos);
            }
            if (params < minarg)
              return (syerr = "Too few arguments to function `" + lex[i].content + "': provided " + tostring(params) + ", required " + tostring(minarg) + ".", lex[lm].pos);

            #else
                 if (!lex[i].ext->refstack.is_varargs() && (exceeded_at || params > maxarg))
                          unimplemented_function_list[lex[i].content] = 'M'; //M for too many arguments
                 if (params < minarg)
                          unimplemented_function_list[lex[i].content] = 'F'; //F for too few arguments
            #endif
          }
          break;
        default: ;
      }
    }

    newcode = code;
    return -1;
  }
}
*/
