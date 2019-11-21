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

#include "config.h"
#include "compiler/event_reader/event_parser.h"

extern int global_script_argument_count;

struct scope_ignore {
  map<string,int> ignore;
  bool is_with;
  
  scope_ignore(scope_ignore*x): is_with(x->is_with) {}
  scope_ignore(bool x): is_with(x) {}
  scope_ignore(int x): is_with(x) {}
};

#include "object_storage.h"
#include "collect_variables.h"
#include "languages/language_adapter.h"

void collect_variables(language_adapter *lang, string &code, string &synt, parsed_event* pev)
{
  int igpos = 0;
  darray<scope_ignore*> igstack;
  igstack[igpos] = new scope_ignore(0);
  
  cout << "\nCollecting some variables...\n";
  
  pt dec_start_pos = 0;
  
  int in_decl = 0, dec_out_of_scope = 0; //Not declaring, not declaring outside this scope via global or local
  string dec_prefixes, dec_type, dec_suffixes; //Unary referencers, type names and flags, and unary-postfix array subscripts
  int dec_initializing = false; //This tells us we've passed an = in our initialization
  pt dec_equals_at = 0;
  int bracklevel = 0; //This will help us make sure not to add unwanted unary symbols or miss variables/scripts.
  
  bool dec_name_givn = false; //Has an identifier been named in this declaration?
  string dec_name; //Identifier being declared
  
  int  with_after_parenths = 0;
  bool with_until_semi = false;

  bool grab_tline_index = false; //Are we currently trying to stockpile a list of known timeline indices?
  
  for (pt pos = 0; pos < code.length(); pos++)
  {
    //Stop grabbing the timeline index?
    if (grab_tline_index) {
      grab_tline_index = (synt[pos]=='n') || (synt[pos]=='=');
    }

    if (synt[pos] == '{') {
      bool isw = igstack[igpos]->is_with | with_until_semi; with_until_semi = 0;
      igstack[++igpos] = new scope_ignore(isw);
      continue;
    }
    if (synt[pos] == '}') {
      delete igstack[igpos--];
      continue;
    }
    
    if (synt[pos] == '(' and with_after_parenths) with_after_parenths++;
    if (synt[pos] == ')' and with_after_parenths and --with_after_parenths == 1)
      with_until_semi = with_after_parenths--;
    
    with_until_semi &= (synt[pos] != ';');
    
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
            cout << "Declared " << dec_type << " " << dec_prefixes << dec_name << dec_suffixes << " as " << (dec_out_of_scope-1 ? "global" : "local") << endl;
            if (dec_out_of_scope - 1) //to be placed at global scope
              pev->myObj->globals[dec_name] = dectrip(dec_type,dec_prefixes,dec_suffixes);
            else
              pev->myObj->locals[dec_name] = dectrip(dec_type,dec_prefixes,dec_suffixes);

            if (!dec_initializing) //If this statement does nothing other than declare, remove it
            {
              cout << "ERASE FROM CODE: " << code.substr(dec_start_pos,pos+1-dec_start_pos) << endl;
              code.erase(dec_start_pos,pos+1-dec_start_pos);
              synt.erase(dec_start_pos,pos+1-dec_start_pos);
              pos = dec_start_pos;
            }
            else
            {
              cout << "ERASE FROM CODE: " << code.substr(dec_start_pos,dec_equals_at-dec_start_pos) << endl;
              code.replace(dec_start_pos,dec_equals_at-dec_start_pos, dec_name);
              synt.replace(dec_start_pos,dec_equals_at-dec_start_pos, string(dec_name.length(),'n'));
              pos -= dec_equals_at - dec_start_pos - 1 - dec_name.length();
            }
            dec_start_pos = pos;
          }
          else //Add to this scope
          {
            igstack[igpos]->ignore[dec_name] = pos;
            pos++; //cout << "Added `" << dec_name << "' to ig\n";
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
          dec_equals_at = pos;
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
      //Bookmark this spot
      const int sp = pos;
      
      //Determine which meaning it is.
      pos += 5; //Skip "L-O-C-A-L" or "G-L-O-B-A"
      if (synt[pos] == 'L') pos++;
      if (synt[pos] != 't')
      {
        for (pt i = sp; i < pos; i++)
          synt[i] = 'n'; //convert to regular identifier; in this case marking a constant.
        continue;
      }
      //We're at either global declarator or local declarator: record which scope it is.
      dec_out_of_scope = 1 + (code[sp] == 'g'); //If the first character of this token is 'g', it's global. Otherwise we assume it's local.
      
      //Remove the keyword from the code
      code.erase(sp,pos-sp);
      synt.erase(sp,pos-sp);
      pos = sp;
      
      fflush(stdout);
      goto past_this_if;
    }
    if (synt[pos] == 't')
    {
      past_this_if:
      
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
      dec_start_pos = pos--;
      continue;
    }
    if (synt[pos] == 'n')
    {
      bool nts = !pos or synt[pos-1] != '.';
      
      const pt spos = pos;
      while (synt[++pos] == 'n');
      
      //Looking at a straight identifier. Make sure it actually needs declared.
      const string nname = code.substr(spos,pos-spos);
      
      if (!nts)
        { pos--; continue; }
      
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

        //If we're currently looking for a timeline variable, check if this is it.
        if (grab_tline_index) {
          cout << "  Potentially calls timeline `" << nname << "'\n";
          pev->myObj->tlines.insert(pair<string,int>(nname,1));
          grab_tline_index = false;
        }

        //Before ignoring globals/locals, check if we're setting the timeline index.
        if (nname == "timeline_index") {
          grab_tline_index = true;
        }
        
        //First, check shared locals to see if we already have one
        if (shared_object_locals.find(nname) != shared_object_locals.end()) {
          pev->myObj->globallocals[nname]++;
          if (with_until_semi or igstack[igpos]->is_with) {
            pos += 5;
            cout << "Add a self. before " << nname;
            code.insert(spos,"self.");
            synt.insert(spos,"nnnn.");
          }
          cout << "Ignoring `" << nname << "' because it's a shared local.\n"; continue;
        }
        
        //Second, check that it's not a global
        if (lang->global_exists(nname)) {
          cout << "Ignoring `" << nname << "' because it's a global.\n";
          continue;
        }
        
        //Next, make sure we're not specifically ignoring it
        map<string,int>::iterator ex;
        for (int i = igpos; i >= 0; i--)
          if ((ex = igstack[i]->ignore.find(nname)) != igstack[i]->ignore.end()) {
            cout << "Ignoring `" << nname << "' because it's on the ignore stack for level " << i << " since position " << ex->second << ".\n";
            goto continue_2;
          }
        
        int argnum, iscr;
        iscr = sscanf(nname.c_str(),"argument%d",&argnum);
        if (iscr == 1)
        { //  not in a script or are but have exceeded arg number
          if (global_script_argument_count < argnum + 1)
            global_script_argument_count = argnum + 1;
          continue;
        }
        
        //Last, make sure we're not in a with.
        if (with_until_semi or igstack[igpos]->is_with)
        {
          pos += 5;
          code.insert(spos,"self.");
          synt.insert(spos,"nnnn.");
        }
        
        //Of course, we also don't want to risk overwriting a typed version
        if (pev->myObj->locals.find(nname) != pev->myObj->locals.end()) {
          cout << "Ignoring `" << nname << "' because it's already a local.\n"; continue;
        }
        
        cout << "Adding `" << nname << "' because that's just what I do.\n";
        pev->myObj->locals[nname] = dectrip();
        continue_2: continue;
      }
      else //Since a syntax check already completed, we assume this is a valid function
      {
        bool contented = false;
        unsigned pars = 1, args = 0;
        
        for (pt i = pos+2; pars; i++) //Start after parenthesis at pos+1, loop until that parenthesis is closed
        {
          if (synt[i] == ',' and pars == 1) {
            args += contented;
            contented = false; continue;
          }
          if (synt[i] == ')') { //TODO: if (a,) is one arg according to ISO, move this before contented = true;
            pars--; continue;
          }
          contented = true;
          if (synt[i] == '(') {
            pars++; continue;
          }
        }
        args += contented; //Final arg for closing parentheses
        pair<parsed_object::funcit,bool> a = pev->myObj->funcs.insert(pair<string,int>(nname,args));
        if (!a.second and a.first->second < signed(args))
          a.first->second = args;
        cout << "  Calls script `" << nname << "'\n";
      }
    }
    else if (synt[pos] == 's')
    {
      const size_t sp = pos;
      while (synt[++pos] == 's');
      if (code.substr(sp,(pos--)-sp) == "with") {
        with_after_parenths = 1;
        continue;
      }
    }
  }
  
  //cout << "**Finished collections in " << (pev==NULL ? "some event for some unspecified object" : pev->myObj->name + ", event " + event_get_human_name(pev->mainId,pev->id))<< "\n";
  
  //Store these for later.
  pev->code = code;
  pev->synt = synt;
}
