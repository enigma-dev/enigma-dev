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
using namespace std; //More ease
#include "parser_components.h" //duh
#include "../externs/externs.h" //To interface with externally defined types and functions

#include "../general/darray.h"
#include "../general/parse_basics.h"
#include "object_storage.h"




void parser_init()
{
  //Single statements taking no parameters requiring semicolon/other dividing symbol before and then a semicolon directly after
  //Tokenized as 'b'
  edl_tokens["break"] = 'b';
  edl_tokens["continue"] = 'b';
  edl_tokens["default"] = 'b';
  //Labels; anything followed by :, can be classified the same way: semicolon before, but colon after.
  edl_tokens["private"] = 'b';
  edl_tokens["protected"] = 'b';
  edl_tokens["public"] = 'b';
  
  /*These are type flags that don't really imply a type
  edl_tokens["signed"] = 'd';
  edl_tokens["unsigned"] = 'd';
  edl_tokens["register"] = 'd';
  edl_tokens["static"] = 'd';
  edl_tokens["const"] = 'd';*/
  
  //Word ops: these aren't actually replaced, just tokenized.
  edl_tokens["and"] = '&';
  edl_tokens["or"]  = '|';
  edl_tokens["xor"] = '^';
  edl_tokens["not"] = '!';
  
  //These must be marked separately for easy retrieval
  //tokenized as 'C'
  edl_tokens["class"] = 'C';
  edl_tokens["enum"] = 'C';
  edl_tokens["namespace"] = 'C';
  edl_tokens["struct"] = 'C';
  
  //These two are... Special.
  edl_tokens["do"] = 'r';
  edl_tokens["else"] = 'e';
  
  //These must be passed a parameter in parentheses
  //Token is 's'
  edl_tokens["case"] = 's';
  edl_tokens["catch"] = 's';
  edl_tokens["if"] = 's';
  edl_tokens["repeat"] = 's';
  edl_tokens["switch"] = 's';
  edl_tokens["until"] = 's';
  edl_tokens["while"] = 's';
  edl_tokens["with"] = 's';
  //For gets an f
  edl_tokens["for"] = 'f';
  
  //These are all templates, which can later be revised to 'd' when passed a parameter in <>
  //token is 't'
  edl_tokens["const_cast"] = 't';
  edl_tokens["dynamic_cast"] = 't';
  edl_tokens["reinterpret_cast"] = 't';
  edl_tokens["static_cast"] = 't';
  edl_tokens["template"] = 't';
  
  //Local and Global are special.
  edl_tokens["local"] = 'L';  // Each of these has a dual meaning. They can either change the scope
  edl_tokens["global"] = 'L'; // of the declaration that follows, or just serve as a negative constant
  
  //These each require a parameter of some sort, so should not be given a semicolon right after
  //Token is 'p'
  edl_tokens["delete"] = 'p';
  edl_tokens["goto"] = 'p';
  edl_tokens["return"] = 'p';
  edl_tokens["throw"] = 'p';
  
  //Token is 'a'
  edl_tokens["new"] = 'a';

  //Classify this as O then replace O() with nnn and then O(.*)\( with nn...n
  edl_tokens["operator"] = 'o';
}


struct scope_ignore {
  map<string,int> i;
};

#define and_safety

string parser_main(string code, parsed_event* pev = NULL)
{
  //Converting EDL to C++ is still relatively simple.
  //It can be done, for the most part, using only find and replace.
  
  //To preserve efficiency, however, we will reduce the number of passes by replacing multiple things at once.
  
  /*/First off, remove comments, strings, and whitespace.
  parser_remove_whitespace(code,0);
  
  //Next, we generate a syntax map
  parser_buffer_syntax_map(code,synt,0);*/
  string synt;
  
  //Initialize us a spot in the global scope
  initscope("script0");
  
  parser_ready_input(code,synt);
  
  parser_reinterpret(code,synt);
  
  parser_add_semicolons(code,synt);
  
  //cout << synt << endl;
  //cout << code << endl;
  
  if (pev)
  {
    int igpos = 0;
    darray<scope_ignore*> igstack;
    igstack[igpos] = new scope_ignore;
    
    for (size_t pos = 0; pos < code.length(); pos++)
    {
      if (synt[pos] == '{') {
        igstack[++igpos] = new scope_ignore;
        continue;
      }
      if (synt[pos] == '}') {
        delete igstack[igpos--];
        continue;
      }
      
      int out_of_scope = 0; //Not declaring outside this scope
      
      if (synt[pos] == 'L') //Double meaning.
      {
        //Determine which meaning it is.
        const int sp = pos;
        pos += 5; //Skip "L-O-C-A-L" or "G-L-O-B-A"
        if (synt[pos] == 'L') pos++;
        if (synt[pos] != 't')
        {
          for (unsigned i = sp; i < pos; i++)
            synt[pos] = 'n'; //convert to regular identifier
          continue;
        }
        //We're at either global declarator or local declarator: record which scope it is.
        out_of_scope = 1 + (code[sp] == 'g'); //If the first character of this token is 'g', it's global. Otherwise we assume it's local.
        
        //Remove the keyword from the code
        code.erase(sp,pos-sp);
        synt.erase(sp,pos-sp);
        pos = sp;
        
        goto past_this_if;
      }
      if (synt[pos] == 't')
      {
        past_this_if:
        
        //Skip to the end of the typename, remember where we started
        const int tsp = pos;
        while (synt[++pos] == 't');
        
        //Copy the type
        string type_name = code.substr(tsp,pos-tsp);
        
        if (out_of_scope)
        {
          //Remove the keyword from the code
          code.erase(tsp,pos-tsp);
          synt.erase(tsp,pos-tsp);
          pos = tsp;
        }
        
        string lid;
        unsigned spos = pos;
        bool has_init = false;
        
        string prefixes, suffixes;
        
        //Begin iterating the declared variables
        while (pos < code.length())
        {
          if (synt[pos] == ',' or synt[pos] == ';')
          {
            const bool donehere = (synt[pos] == ';');
            
            cout << "*************" << pos << "; " << spos << "*************" << endl;
            //cout << "\n" << code.substr(0,spos) << "<<" << code.substr(spos,pos-spos) << ">>" << code.substr(pos) << endl << endl;// << synt.substr(0,pos) << "<<>>" << code.substr(pos) << endl << endl;
            
            if (out_of_scope) //Designated for a different scope: global or local
            {
              if (out_of_scope - 1) //to be placed at global scope
                pev->myObj->globals[lid] = dectrip(type_name,prefixes,suffixes);
              else
                pev->myObj->locals[lid] = dectrip(type_name,prefixes,suffixes);
              if (!has_init) //If this statement does nothing other than declare, remove it
              {
                code.erase(spos,pos+1-spos);
                synt.erase(spos,pos+1-spos);
                pos = spos;
              }
              else pos++;
            }
            else //Add to this scope
            {
              cout << "Add to ig" << endl;
              igstack[igpos]->i[lid] = 1;
              pos++;
              cout << "Added to ig" << endl;
            }
            cout << "endif ';'" << endl;
            
            prefixes = suffixes = "";
            
            spos = pos;
            has_init = false;
            if (donehere) {
              pos--; break;
            }
            continue;
          }
          if (synt[pos] == 'n')
          {
            const size_t spos = pos;
            while (synt[++pos] == 'n');
            lid = code.substr(spos,pos-spos);
            continue;
          }
          if (synt[pos] == '*') {
            prefixes += "*";
            pos++; continue;
          }
          if (synt[pos] == '&') {
            prefixes += "&";
            pos++; continue;
          }
          if (synt[pos] == '[')
          {
            const unsigned ssp = pos++;
            for (int cnt = 1; cnt > 0 and_safety; pos++)
              if (synt[pos] == '[') cnt++;
              else if (synt[pos] == ']') cnt--;
            suffixes += code.substr(ssp,pos-ssp);
            continue;
          }
          if (synt[pos++] == '=')
          {
            while (synt[pos] != ',' and synt[pos] != ';' and synt[pos]) pos++;
            has_init = true;
            continue;
          }
          cout << "~" << code[pos-1];
        }
      }
    }
    
    cout << "*****************************FINISHED********************\n";
    
    //Store these for later.
    pev->code = code;
    pev->synt = synt;
  }
  
  return code;
}

