/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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

//Welcome to the ENIGMA EDL-to-C++ parser; just add semicolons.
//No, it's not really that simple.

/*/
  As a fun fact, this parser was the first one I ever wrote, and was originally
  coded in GML. When it was complete, it parsed itself to C++.
  Large parts of it have since undergone recode to account for new features and
  for reasons of efficiency as well as overall legibility. (The parser does not
  preserve whitespace or comments, for one thing, so that was all lost when the
  original parsed itself over.)

  It's also worth noting that this is my only parser which incorporates a lexer
  or conducts multiple passes. It uses what I call a syntax string, which some-
  one once told me was akin to a token stream. I don't really care; I made this
  up myself long ago without any help from external sources.

  At any rate, this means that this parser is really easy to follow as far as
  telling what's going on, as it can be divided into separate sections.
/*/

#include <map> //Log lookup
#include <string> //Ease of use
#include <iostream> //Print shit
#include <stdio.h> //stdout, fflush
using namespace std; //More ease
#include "../externs/externs.h" //To interface with externally defined types and functions
#include "parser_components.h" //duh

#include "../general/darray.h"
#include "../general/parse_basics.h"
#include "object_storage.h"

#include "collect_variables.h"


//This adds all keywords to a tree structure for quick lookup of their token.
void parser_init()
{
  //Single statements taking no parameters requiring semicolon/other dividing symbol before and then a semicolon directly after
  //Tokenized as 'b'
  edl_tokens["break"]     = 'b';
  edl_tokens["continue"]  = 'b';
  edl_tokens["default"]   = 'b';
  //Labels; anything followed by :, can be classified the same way: semicolon before, but colon after.
  edl_tokens["private"]   = 'b';
  edl_tokens["protected"] = 'b';
  edl_tokens["public"]    = 'b';

  /*These are type flags that don't really imply a type
  edl_tokens["signed"]   = 'd';
  edl_tokens["unsigned"] = 'd';
  edl_tokens["register"] = 'd';
  edl_tokens["static"]   = 'd';
  edl_tokens["const"]    = 'd';*/

  //Word ops: these aren't actually replaced, just tokenized.
  edl_tokens["and"] = '&';
  edl_tokens["or"]  = '|';
  edl_tokens["xor"] = '^';
  edl_tokens["not"] = '!';

  //These must be marked separately for easy retrieval
  //tokenized as 'C'
  edl_tokens["class"]     = 'C';
  edl_tokens["enum"]      = 'C';
  edl_tokens["namespace"] = 'C';
  edl_tokens["struct"]    = 'C';

  //These two are... Special.
  edl_tokens["do"]   = 'r';
  edl_tokens["else"] = 'e';

  //These must be passed a parameter in parentheses
  //Token is 's'
  edl_tokens["case"]   = 's';
  edl_tokens["catch"]  = 's';
  edl_tokens["if"]     = 's';
  edl_tokens["repeat"] = 's';
  edl_tokens["switch"] = 's';
  edl_tokens["until"]  = 's';
  edl_tokens["while"]  = 's';
  edl_tokens["with"]   = 's';
  //For gets an f
  edl_tokens["for"] = 'f';
  
  //Type flags
  //token is 't'
  edl_tokens["unsigned"] = 't';
  edl_tokens["long"]     = 't';
  edl_tokens["short"]    = 't';
  edl_tokens["signed"]   = 't';
  edl_tokens["register"] = 't';
  edl_tokens["unsigned"] = 't';
  edl_tokens["const"]    = 't';
  edl_tokens["volatile"] = 't';
  edl_tokens["static"]   = 't';
  
  //These are all templates, which can later be revised to 'd' when passed a parameter in <>
  //token is also 't'
  edl_tokens["const_cast"]       = 't';
  edl_tokens["dynamic_cast"]     = 't';
  edl_tokens["reinterpret_cast"] = 't';
  edl_tokens["static_cast"]      = 't';
  edl_tokens["template"]         = 'X'; //template is disabled

  //Local and Global are special.
  edl_tokens["local"] = 'L';  // Each of these has a dual meaning. They can either change the scope
  edl_tokens["global"] = 'L'; // of the declaration that follows, or just serve as a negative constant

  //These each require a parameter of some sort, so should not be given a semicolon right after
  //Token is 'p'
  edl_tokens["delete"] = 'p';
  edl_tokens["goto"]   = 'p';
  edl_tokens["return"] = 'p';
  edl_tokens["throw"]  = 'p';

  //Token is 'a'
  edl_tokens["new"] = 'a';

  //Classify this as O then replace O() with nnn and then O(.*)\( with nn...n
  edl_tokens["operator"] = 'o';
}



string parser_main(string code, parsed_event* pev = NULL)
{
  //Converting EDL to C++ is still relatively simple.
  //It can be done, for the most part, using only find and replace.
  
  //For the sake of efficiency, however, we will reduce the number of passes by replacing multiple things at once.
  
  string synt;
  
  //Reset things
    //Nothing to reset :trollface:
  
  //Initialize us a spot in the global scope
  initscope("script0");
  
  if (pev) {
    pev->strc = 0; //Number of strings in this code
    parser_ready_input(code,synt,pev->strc,pev->strs);
  }
  else
  {
    varray<string> strst;
    unsigned int strct = 0;
    parser_ready_input(code,synt,strct,strst);
  }
  
  parser_reinterpret(code,synt);
  
  parser_add_semicolons(code,synt);
  
  //cout << synt << endl;
  //cout << code << endl;
  
  if (pev) { cout << "<collecting variables..."; fflush(stdout);
    collect_variables(code,synt,pev); cout << " done>"; fflush(stdout);
  }
  
  return code;
}

#include "../cfile_parse/type_resolver.h"

typedef map<string,dectrip> localscope;

int parser_secondary(string& code, string& synt)
{
  // We'll have to again keep track of temporaries
  // Fortunately, this time, there are no context-dependent tokens to resolve
  
  int slev = 0;
  darray<localscope*> sstack;
  sstack[slev] = new localscope();
  
  for (pt pos = 0; pos < synt.length(); pos++)
  {
    if (synt[pos] == 't')
    {
      pt spos = pos;
      while (synt[++pos] == 't');
      pos += parser_fix_templates(code,pos,spos,&synt);
      continue;
    }
    else if (synt[pos] == '.' and synt[pos+1] == 'n')
    {
      const pt epos = pos--; backloop:
      while (synt[pos] == ']' or synt[pos] == ')' or synt[pos] == 'n' or synt[pos] == 't')
      {
        if (synt[pos] == ']' or synt[pos] == ')')
        {
          for (int cnt = (pos--, 1); cnt; pos--)
            if (synt[pos] == '[' or synt[pos] == '(') cnt--;
            else if (synt[pos] == ')' or synt[pos] == ']') cnt++;
          pos++;
        }
        else
          for (char c = synt[pos--]; pos and synt[pos] == c; pos--);
      }
      if (pos) {
        if (synt[pos-1] == '.')
          { pos--; goto backloop; }
        if (synt[pos-1] == '>' and synt[pos-2] == '-')
          { pos -= 2; goto backloop; }
        if (synt[pos-1] == ':' and synt[pos-2] == ':')
          { pos -= 2; goto backloop; }
      }
      
      string exp = code.substr(pos,epos-pos), // Copy the expression being operated upon by .
         expsynt = synt.substr(pos,epos-pos); // Copy the lex of it, too
      const pt ebp = pos; // Expression's beginning position
      pt ep = pos = epos; // Restore our original ending position.
      
      // Determine the member being accessed
      while (synt[++ep] == 'n');
      string member = code.substr(epos,ep-epos);
      
      // Determine the type of the left-hand expression
      onode n = exp_typeof(exp);
      
      if (n.type->members.find(member) == n.type->members.end()) // No member by this name can be accessed
      {
        string repstr = "enigma::varaccess_";
        string repsyn = "nnnnnnnnnnnnnnnnnn";
        
        repstr += member;
        repsyn += string('n',member.length());
        
        repstr += "(int(";
        repsyn += "(ttt(";
        
        repstr += exp;
        repsyn += expsynt;
        
        repstr += "))->";
        repsyn += "))->";
        
        code.replace(ebp,exp.length() + 1 + member.length(),repstr);
        synt.replace(ebp,exp.length() + 1 + member.length(),repsyn);
      }
      else // There is a member by this name in the type of that expression
      {
        if (n.deref) { // The type is a pointer. This dot should be a ->
          code.replace(epos,1,"->");
          synt.replace(epos,1,"->");
        }
      }
    }
    else {
      while (synt[pos] == synt[pos+1]) pos++;
    }
  }
  return -1;
}
