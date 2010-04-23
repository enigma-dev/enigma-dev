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

#include <stack>
#include <string>
#include <iostream>
using namespace std;
#include "../general/darray.h"
#include "../general/implicit_stack.h"
#include "../externs/externs.h"
#include "cfile_parse_constants.h"

#include "cfile_pushing.h"
#include "macro_functions.h"
#include "../general/parse_basics.h"

#include "expression_evaluator.h"

//This is a cheap hack to execute things at startup without main()
map<string,bool> constant_types; //Load this with types like "int" and flags like "unsigned"
struct init_const_types { init_const_types() { //These are acceptable for const-expressions
  constant_types["bool"] = 1; 
  constant_types["char"] = 1; 
  constant_types["short"] = 1; 
  constant_types["long"] = 1; 
  constant_types["int"] = 1; 
  constant_types["signed"] = 1; 
  constant_types["unsigned"] = 1; 
  constant_types["const"] = 1; 
  constant_types["static"] = 1; 
  } } init_const_types_now;

extern externs* builtin_type__int;
string temp_parse_seg(string seg, externs* tparam_ext, externs **kt = NULL)
{
  //cout << seg << endl;
  if (tparam_ext->flags & EXTFLAG_TYPEDEF)
  {
    string p = "";
    externs* is = immediate_scope;
    immediate_scope = NULL;
    
    unsigned lvl = 0;
    bool hypothesize = false;
    externs *move_into = &global_scope;
    for (pt i = 0; i<seg.length(); i++)
    {
      if (!lvl and is_letter(seg[i]))
      {
        const pt is = i;
        while (is_letterd(seg[++i]));
        string tn = seg.substr(is,i-- - is);
        
        if (tn == "typename") {
          hypothesize = true;
          continue; //Extraneous, useless.
        }
        
        if (!find_extname(tn,EXTFLAG_TYPENAME | EXTFLAG_NAMESPACE))
        {
          if (hypothesize and move_into) //This is a new typename! Yay standard! Looks like this:  template<...> ... typename a::doesnotexist b;
          {
            externs *a = move_into->members[tn] = new externs("<new template param alias>",builtin_type__int,move_into,EXTFLAG_TYPEDEF | EXTFLAG_TYPENAME);
            move_into = a;
          } //TODO: add a "templatize" to go with "hypothesize" 
          else if (constant_types.find(tn) != constant_types.end())
            ext_retriever_var = builtin_type__int;
          else
          {
            cferr = "`"+tn+"' cannot be used in a constant expression: not found in `" + 
                    (immediate_scope?strace(immediate_scope):strace(current_scope)) + 
                    "' <" + tparam_ext->name + ":" + tostring(tparam_ext->flags) + "> [" + seg + "] in ";
            return "";
          }
        }
        hypothesize = false;
        
        move_into = ext_retriever_var;
        
        if (kt)
          *kt = ext_retriever_var;
        p += "_" + tn;
      }
      else if (seg[i] == '<')
        lvl++;
      else if (seg[i] == '>')
        lvl--;
      else if (!lvl and seg[i] == ':' and seg[i+1] == ':')
        immediate_scope = move_into;
    }
    immediate_scope = is;
    
    if (p == "")
      cferr = "No valid type named in template parameters <" + tparam_ext->name + ":" + tostring(tparam_ext->flags) + "> [" + seg + "] in ";
    return p;
  }
  else if (tparam_ext->flags & EXTFLAG_TYPENAME)
  {
    if (constant_types.find(tparam_ext->type->name) == constant_types.end()) {
      cferr = "Type `"+tparam_ext->type->name+"' cannot be used in a constant expression... [" + seg + "] in ";
      return "";
    }
    
    value a = evaluate_expression(seg);
    if (rerrpos != -1) {
      cferr = "Error in template expression, position " + tostring(rerrpos) + ": " + rerr + " [" + seg + "] in ";
      return "";
    }
    
    if (kt)
      externs *a = *kt = new externs(tparam_ext->name,NULL,current_scope,tparam_ext->flags | EXTFLAG_DEFAULTED | EXTFLAG_VALUED,(long long)(long)a);
    return "_" + tostring((UTYPE_INT)a);
  }
  cferr = "Macro parameter set up wrong. This error shouldn't occur...  [" + seg + "] in ";
  return "";
}

string temp_parse_list(externs* last,string specs,varray<externs*> *va = NULL)
{
  string ns;
  pt is = 0, ti = 0, cnt = 0, i;
  for (i = 0; i<specs.length(); i++)
  {
    if (specs[i] == ',' and cnt == 0)
    {
      if (ti >= last->tempargs.size) 
      {
        cferr = "Too many parameters to template`"+last->name+"': Passed at least " + tostring(ti+1) 
              + ", requested " + tostring(last->tempargs.size);
        return "";
      }
      
      externs* pd = last->tempargs[ti];
      const string exps = temp_parse_seg(specs.substr(is,i-is),pd, va? &((*va)[ti]) : NULL);
      if (exps == "") {
        cferr += "{" + specs + "} at " + tostring(i);
        return "";
      }
      ns += "_" + exps;
      is = i+1;
      ti++;
    }
    else if (specs[i] == '<') cnt++;
    else if (specs[i] == '>') cnt--;
  }
  if (i - is)
  {
    if (ti >= last->tempargs.size) 
    {
      cferr = "Too many parameters to template`"+last->name+"': Passed at least " + tostring(ti)
            + ", requested " + tostring(last->tempargs.size);
      return "";
    }
    
    externs* pd = last->tempargs[ti];
    const string exps = temp_parse_seg(specs.substr(is,i-is),pd, va? &((*va)[ti]) : NULL);
    if (exps == "") {
      cferr += "{" + specs + "} at " + tostring(i);
      return "";
    }
    ns += "_" + exps;
    ti++;
  }
  
  while (ti < last->tempargs.size)
  {
    if ((last->tempargs[ti]->flags & EXTFLAG_DEFAULTED) or (last->tempargs[ti]->flags & EXTFLAG_VALUED))
    {
      if (va)
        (*va)[ti] = new externs(last->tempargs[ti]->name,last->tempargs[ti]->type,last->tempargs[ti]->parent,last->tempargs[ti]->flags,last->tempargs[ti]->value_of);
      ti++;
    }
    else {
      cferr = "Too few parameters to template `" + last->name + "': Passed " + tostring(ti) + ", expected " + tostring(last->tempargs.size);
      return "";
    }
  }
  
  return ns;
}

externs* TemplateSpecialize(externs* last, string specs) //Last is the type we're specializing, specs is the the string content of the <> in the code.
{
  /**
    There are three template lists to consider in this function, which applies to this code:
    template<typename a> b<c,a>
    Since str_b is an already-fully-defined template we are specializing, we will use its list as a reference to check our work.
      @c b->tempargs is known simply as @c last->tempargs, and represents the original list.
      @c template <typename a> has not yet been dumped into anything, and is accessed with @c tmplate_params.
      @c implementations is a new array we allocate, and contains data from parsed implementation string @c specs.
  **/
  
  if (not(last->flags & EXTFLAG_TEMPLATE)) {
    cferr = "Attempting to specialize non-template type `" + last->name + "'";
    return NULL;
  }
  
  varray<externs*> implementations; //Set of new values to template. These will be either solid types, or a new typedef.
  string ns = temp_parse_list(last,specs,&implementations); //Fetch a string and varray containing (possibly partial) specialization data
  if (ns == "")
    return NULL;
  
  if (implementations.size != last->tempargs.size) {
    cferr = "Wrong number of parameters to template specialization";
    return NULL;
  }
  
  externs* specm = temp_get_specializations(last);
  extiter exiterexists = specm->members.find(ns);
  if (exiterexists != specm->members.end())
    return exiterexists->second;
  
  externs* ret = new externs(last->name + ns,last->type,last->parent,last->flags);
  ret->ancestors[0] = last;
  
  //cout << "for (unsigned i = 0; i < " << implementations.size << " and i < " << last->tempargs.size << "; i++)" << endl;
  for (unsigned i = 0; i < last->tempargs.size; i++) 
  {
    externs* n = ret->tempargs[i] = new externs(last->tempargs[i]->name,implementations[i],ret,(last->tempargs[i]->flags & ~EXTFLAG_DEFAULTED) | (n->type?EXTFLAG_DEFAULTED:0));
    
    //cout << "Copied " << n->name << " to new instantiation";
    //Iterate through the new template parameters. If this is one of them, inherit its name.
    
    if (last->tempargs[i]->flags & EXTFLAG_TYPEDEF)
    {
      for (unsigned ii = 0; ii < tmplate_params.size; ii++)
      {
        if (implementations[i] == tmplate_params[ii].def) { //If the current implementation is on the list of new typenames
          n->name = tmplate_params[ii].name;
          break;
        }
      }
    }
  }
  
  specm->members[ns] = ret;
  return ret;
}

bool access_specialization(externs* &whom, string specs)
{
  if (not(whom->flags & EXTFLAG_TEMPLATE)) {
    cferr = "Attempting to access specialization of non-template type `" + whom->name + "'";
    return 0;
  } //cout << "Accessing specialization <" << specs << "> from " << whom->name << endl ;
  
  string ns = temp_parse_list(whom,specs);
  if (ns == "")
    return 0;
  
  externs* s = temp_get_specializations_ie(whom);
  if (s)
  {
    extiter it = s->members.find(ns);
    /*if (it == s->members.end())
    {
      cout << endl << endl << endl << endl << endl << "New section: "<< endl ;
      for (extiter it = s->members.begin(); it != s->members.end(); it++)
        cout << "*  " << it->second->name << endl;
      cferr = "Failed to access specialization <" + ns + "> in `" + whom->name + "': Probably does not exist";
      return 0;
    }
    */
    if (it != s->members.end())
      whom = it->second;
  }
  return 1;
}
