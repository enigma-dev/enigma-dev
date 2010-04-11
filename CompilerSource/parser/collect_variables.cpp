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

#include <map>
#include <string>
#include <iostream>
using namespace std;

struct scope_ignore {
  map<string,int> i;
};

#define and_safety
#include "object_storage.h"

#include "../externs/externs.h"

void collect_variables(string code, string synt, parsed_event* pev = NULL)
{
  int igpos = 0;
  darray<scope_ignore*> igstack;
  igstack[igpos] = new scope_ignore;
  
  externs* pscope = NULL;
  current_scope = &global_scope;
  extiter ns_enigma = current_scope->members.find("enigma");
  
  // Find the parent object
  if (ns_enigma != current_scope->members.end()) {
    extiter parent = ns_enigma->second->members.find("object_collisions");
    if (parent != ns_enigma->second->members.end())
      pscope = parent->second;
  }
  
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
          //cout << "\n" << code.substr(0,spos) << "<<" << code.substr(spos,pos-spos) << ">>" << code.substr(pos) << endl << endl;// << synt.substr(0,pos) << "<<>>" << code.substr(pos) << endl << endl;
          
          if (out_of_scope) //Designated for a different scope: global or local
          {
            //Declare this as a specific type
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
            cout << "Added `" << lid << "' to ig" << endl;
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
    if (synt[pos] == 'n')
    {
      const unsigned spos = pos;
      while (synt[++pos] == 'n');
      if (synt[pos] != '(') // If it isn't a function (we assume it's nothing else)
      {
        //Looking at a straight identifier. Make sure it actually needs declared.
        const string nname = code.substr(spos,pos-spos);
        
        //First, check that it's not a global
        if (find_extname(nname,0xFFFFFFFF))
          continue;
        
        //Iterate the tiers of the parent object
        for (externs *cs = pscope; cs; cs = (cs->ancestors.size ? cs->ancestors[0] : NULL) )
        {
          cout << " >> Checking ancestor " << cs->name << endl;
          if (cs->members.find(nname) != cs->members.end())
            goto continue_2;
        }
        
        //Now make sure we're not specifically ignoring it
        for (int i = igpos; i >= 0; i--)
          if (igstack[i]->i.find(nname) != igstack[igpos]->i.end())
            goto continue_2;
        
        pev->myObj->locals[nname] = dectrip();
        continue_2: continue;
      }
    }
  }
  
  cout << "*****************************FINISHED********************\n";
  
  //Store these for later.
  pev->code = code;
  pev->synt = synt;
}
