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
//...No, it's not really that simple.

#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
using namespace std;

struct scope_ignore {
  map<string,int> ignore;
};

#define and_safety
#include "object_storage.h"

#include "../externs/externs.h"

void collect_variables(string code, string synt, parsed_event* pev = NULL)
{
  int igpos = 0;
  darray<scope_ignore*> igstack;
  igstack[igpos] = new scope_ignore;
  
  cout << "\n\n\nCollecting variables for event " << pev->id << "..." << endl;
  cout << code << endl;
  cout << synt << endl;
  
  pt dec_start_pos = 0;
  
  int in_decl = 0, dec_out_of_scope = 0; //Not declaring, not declaring outside this scope via global or local
  string dec_prefixes, dec_type, dec_suffixes; //Unary referencers, type names and flags, and unary-postfix array subscripts
  int dec_initializing = false; //This tells us we've passed an = in our initialization
  int bracklevel = 0; //This will help us make sure not to add unwanted unary symbols or miss variables/scripts.
  
  bool dec_name_givn = false; //Has an identifier been named in this declaration?
  string dec_name; //Identifier being declared
  
  for (pt pos = 0; pos < code.length(); pos++)
  {
    if (synt[pos] == '{') {
      igstack[++igpos] = new scope_ignore;
      continue;
    }
    if (synt[pos] == '}') {
      delete igstack[igpos--];
      continue;
    }
    
    if (bracklevel == 0 and in_decl)
    {
      if (synt[pos] == ';' or synt[pos] == ',')
      {
        const bool was_a_semicolon = synt[pos] == ';';
        
        if (dec_name_givn)
        {
          if (dec_out_of_scope) //Designated for a different scope: global or local
          {
            //Declare this as a specific type
            if (dec_out_of_scope - 1) //to be placed at global scope
              pev->myObj->globals[dec_name] = dectrip(dec_type,dec_prefixes,dec_suffixes);
            else
              pev->myObj->locals[dec_name] = dectrip(dec_type,dec_prefixes,dec_suffixes);

            if (!dec_initializing) //If this statement does nothing other than declare, remove it
            {
              cout << "ERASE FROM CODE: " << code.substr(dec_start_pos,pos+1-dec_start_pos) << endl << endl << endl;
              code.erase(dec_start_pos,pos+1-dec_start_pos);
              synt.erase(dec_start_pos,pos+1-dec_start_pos);
              pos = dec_start_pos;
            }
            else pos++;
            dec_start_pos = pos;
          }
          else //Add to this scope
          {
            cout << "Add to ig" << endl;
            igstack[igpos]->ignore[dec_name] = pos;
            pos++;
            cout << "Added `" << dec_name << "' to ig" << endl;
          }
        }
        
        if (was_a_semicolon)
          dec_out_of_scope = in_decl = 0, dec_type = "";
        dec_prefixes = dec_suffixes = "";
        dec_initializing = false;
        dec_name_givn = false;
      }
      if (!dec_initializing)
      {
        if (synt[pos] == '*') {
          dec_prefixes += "*";
          continue;
        }
        if (synt[pos] == '&') {
          dec_prefixes += "&";
          continue;
        }
        if (synt[pos] == '=') {
          dec_initializing = true;
          continue;
        }
        if (synt[pos] == '(') {
          ((dec_name_givn)?dec_suffixes:dec_prefixes) += '(';
          continue;
        }
        if (synt[pos] == '(') {
          ((dec_name_givn)?dec_suffixes:dec_prefixes) += ')';
          continue;
        }
      }
    }
    if (synt[pos] == '[')
    {
      if (in_decl and !dec_initializing)
      {
        pt ep = pos + 1;
        for (int bc = 1; bc > 0; ep++)
          if (synt[ep] == '[') bc++;
          else if (synt[ep] == ']') bc--;
        dec_suffixes += code.substr(pos,ep - pos);
      }
      bracklevel++;
      continue;
    }
    if (synt[pos] == ']') {
      bracklevel--;
      continue;
    }
    
    if (synt[pos] == 'L') //Double meaning.
    {
      cout << "L"; fflush(stdout);
      
      //Bookmark this spot
      const int sp = pos;
      
      //Determine which meaning it is.
      pos += 5; //Skip "L-O-C-A-L" or "G-L-O-B-A"
      if (synt[pos] == 'L') pos++;
      if (synt[pos] != 't')
      {
        for (pt i = sp; i < pos; i++)
          synt[pos] = 'n'; //convert to regular identifier; in this case marking a constant.
        continue;
      }
      //We're at either global declarator or local declarator: record which scope it is.
      dec_out_of_scope = 1 + (code[sp] == 'g'); //If the first character of this token is 'g', it's global. Otherwise we assume it's local.
      
      //Remove the keyword from the code
      code.erase(sp,pos-sp);
      synt.erase(sp,pos-sp);
      pos = sp;
      
      cout << "\\"; fflush(stdout);
      goto past_this_if;
    }
    if (synt[pos] == 't')
    {
      past_this_if:
      cout << "t"; fflush(stdout);
      
      //Skip to the end of the typename, remember where we started
      const int tsp = pos;
      while (synt[++pos] == 't');
      
      //Copy the type
      dec_type = code.substr(tsp,pos-tsp);
      
      if (dec_out_of_scope)
      {
        //Remove the keyword from the code
        code.erase(tsp,pos-tsp);
        synt.erase(tsp,pos-tsp);
        pos = tsp;
      }
      
      // Indicate that we're in a declaration and should start
      // Ignoring shit rather than adding it to the local list
      in_decl = true;
      
      // Log this position
      dec_start_pos = pos; 
    }
    if (synt[pos] == 'n')
    {
      cout << "N"; fflush(stdout);
      const pt spos = pos;
      while (synt[++pos] == 'n');
      
      //Looking at a straight identifier. Make sure it actually needs declared.
      const string nname = code.substr(spos,pos-spos);
      
      //Decrement pos to avoid skipping a char on continue
      if (synt[pos--] != '(') // If it isn't a function (we assume it's nothing other than a function or varname)
      {
        if (in_decl and !dec_initializing)
        {
          if (!dec_name_givn) {
            dec_name_givn = true;
            dec_name = nname;
          } continue;
        }
        
        //First, check that it's not a global
        if (find_extname(nname,0xFFFFFFFF)) {
          cout << "Ignoring `" << nname << "' because it's a global.\n"; continue;
        }
        
        //Check shared locals to see if we already have one
        if (shared_object_locals.find(nname) != shared_object_locals.end()) {
          cout << "Ignoring `" << nname << "' because it's a local.\n"; continue;
        }
        
        //Now make sure we're not specifically ignoring it
        map<string,int>::iterator ex;
        for (int i = igpos; i >= 0; i--)
          if ((ex = igstack[i]->ignore.find(nname)) != igstack[i]->ignore.end()) {
            cout << "Ignoring `" << nname << "' because it's on the ignore stack for level " << i << " since position " << ex->second << ".\n"; goto continue_2;
          }
        
        cout << "Adding `" << nname << "' because that's just what I do.\n";
        pev->myObj->locals[nname] = dectrip();
        continue_2: continue;
      }
      else //Since a syntax check already completed, we assume this is a valid function
      {
        cout << "!"; fflush(stdout);
        cout << "\nLooking at " <<nname << "...\n";
        bool contented = false;
        unsigned pars = 1, args = 0;
        
        for (pt i = pos+2; pars; i++) //Start after parenthesis at pos+1, loop until that parenthesis is closed
        {
          if (synt[i] == ',' and pars == 1) {
            args += contented;
            contented = false; continue;
          }
          contented = true;
          if (synt[i] == '(') {
            pars++; continue;
          }
          if (synt[i] == ')') { //TODO: if (a,) is one arg according to ISO, move this before contented = true;
            pars--; continue;
          }
        }
        args += contented; //Final arg for closing parentheses
        pev->myObj->funcs[nname] = args;
        cout << "  Calls script `" << nname << "'\n";
        cout << "\\"; fflush(stdout);
      }
      cout << "\\"; fflush(stdout);
    }
  }

  cout << "*****************************FINISHED********************\n";
  
  cout << "\n\n\n\n";
  
  //Store these for later.
  pev->code = code;
  pev->synt = synt;
}
