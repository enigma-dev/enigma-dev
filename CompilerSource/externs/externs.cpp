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

#include <map>
#include <string>
#include <iostream>
#include "../general/darray.h"
using namespace std;
#include "externs.h"

#define NAME__USING_SCOPE "<using>"

bool externs::is_function()
{
  return refstack.is_function();
}

//This thing was really designed with 32bit architecture in mind
int externs::parameter_count_min()
{
  return refstack.parameter_count_min();
}
int externs::parameter_count_max()
{
  return refstack.parameter_count_max();
}
void externs::parameter_unify(rf_stack& x)
{
  const unsigned short n1 = parameter_count_min(), n2 = x.parameter_count_min();
  const unsigned short x1 = parameter_count_max(), x2 = x.parameter_count_max();
  const int nn = n1<n2?n1:n2, nx = x1>x2?x1:x2;
  refstack.parameter_count_set(nn,nx);
}

#ifdef ENIGMA_PARSERS_DEBUG
  int ext_count;
  map<externs*, int> bigmap;
  int tp_just_instd = 0;
  int total_alloc_count[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int tpcsval;
  #define tp_just_instd__TRUE tp_just_instd = tpcsval;
  #define ecpp ext_count++; if (!(ext_count % 100)) cout << "Number of externs: " << ext_count << endl; bigmap[this] = tpcsval; tpcsval=0;
  #define ecmm ext_count--; if (!(ext_count % 100)) cout << "Number of externs: " << ext_count << endl; bigmap.erase(this);
#else
  #define ecpp
  #define ecmm
  #define tp_just_instd__TRUE
#endif

#include "../cfile_parse/cparse_shared.h"
#define other //if (n == "x" and p == &global_scope)
#define other1(s) //cout << "\n\n\n\n\n\n\n\n\n\n\n" << cfile.substr(pos-10,12) << "\n\n\n\n\n\n\n\n" << s;

externs::externs(): flags(0), name(), type(NULL), parent(NULL), value_of(0)
{
  ecpp;
}
externs::externs(string n,externs* p,unsigned int f): flags(f), name(n), type(NULL), parent(p), value_of(0)
{
  ecpp; other
    other1(1)
}
externs::externs(string n,externs* t,externs* p,unsigned int f): flags(f), name(n), type(t), parent(p), value_of(0)
{
  ecpp; other
    other1(2)
}
externs::externs(string n,externs* t,externs* p,unsigned int f,long long v): flags(f), name(n), type(t), parent(p), value_of(v)
{
  ecpp; other
    other1(3)
}
externs::externs(string n,externs* t,externs* p,unsigned int f,long long v,rf_stack rfs): flags(f), name(n), type(t), parent(p), value_of(v), refstack(rfs)
{
  ecpp; other
    other1(4)
}

bool externs::is_using_scope()
{
  return name == NAME__USING_SCOPE;
}
/*
externs *tpdata::def
{
  return def;
}
externs *tpdata::def
{
  externs* r = def;
  def = NULL;
  return r;
}*/

void externs::clear_all()
{
  if (!is_using_scope())
    for (extiter i = members.begin(); i != members.end(); i++)
    {
      //cout << "Delete " << i->second->name << "\n";
      if (i->second->name == "x")
        cout << "poof.\n";
      delete i->second;
    }
  for (unsigned i = 0; i < tempargs.size; i++)
    delete tempargs[i];
  tempargs.size = 0;
  members.clear();
}

externs::~externs()
{
  clear_all();
  //cout << name << ": I'm being deleted!\n";
  ecmm;
}

macro_type::operator string() { return val; }
macro_type::macro_type(): argc(-1), args_uat(-1), recurse_danger(0) {}
macro_type::macro_type(string x): argc(-1), val(x), args_uat(-1), recurse_danger(0) {}

macro_type &macro_type::operator= (string x) { val = x; return *this; }
void macro_type::clear_func()          { argc = -1; }
void macro_type::assign_func(string n) { if (argc==-1) argc=0; recurse_danger = check_recurse_danger(n); }
void macro_type::set_unltd_args(int x) { args_uat = x; }
void macro_type::addarg(string x) { args[argc++] = x; }

#include "../general/parse_basics.h"
bool macro_type::check_recurse_danger(string n)
{
  register bool dgr = 0;
  for (unsigned i = 0; i < val.length(); i++)
  {
    if (is_letter(val[i]))
    {
      const unsigned si = i;
      while (is_letterd(val.c_str()[i])) i++;
      if (val.substr(si,i-si) == n)
      { dgr = 1; break; }
    }
  }
  return dgr;
}



//Map to sort, darray for polymorphic things
map<string, varray<externs> > extarray;
externs global_scope,*current_scope = &global_scope,*immediate_scope=NULL;
map<string,macro_type> macros;



extiter scope_find_member(string name)
{
  for (externs *s = current_scope; s != NULL; s = s->parent)
  {
    extiter it = s->members.find(name);
    if (it != s->members.end() or s == &global_scope)
      return it;
  }
  return global_scope.members.end();
}

externs* scope_get_using(externs* scope)
{
  extiter u = scope->members.find(NAME__USING_SCOPE);
  if (u != scope->members.end())
    return u->second;

  externs* rv = new externs(NAME__USING_SCOPE,NULL,scope,EXTFLAG_NAMESPACE);
  scope->members[NAME__USING_SCOPE] = rv;
  return rv;
}

externs* temp_get_specializations(externs* scope)
{
  extiter u = scope->members.find("<specializations>");
  if (u != scope->members.end())
    return u->second;

  externs* rv = new externs("<specializations>",scope,NULL,EXTFLAG_NAMESPACE);
  scope->members["<specializations>"] = rv;
  return rv;
}
externs* temp_get_specializations_ie(externs* scope)
{
  extiter u = scope->members.find("<specializations>");
  if (u != scope->members.end())
    return u->second;
  return NULL;
}

externs* temp_get_instantiations(externs* scope)
{
  extiter u = scope->members.find("<instantiations>");
  if (u != scope->members.end())
    return u->second;

  externs* rv = new externs("<instantiations>",scope,NULL,EXTFLAG_NAMESPACE);
  //scope->members["<instantiations>"] = rv;
  return rv;
}

externs* scope_get_using_ie(externs* scope)
{
  extiter u = scope->members.find(NAME__USING_SCOPE);
  if (u != scope->members.end())
    return u->second;
  return NULL;
}

string strace(externs *f);
externs* ext_retriever_var = NULL;
void print_scope_members(externs*, int);


#define GARBAGE_SCOPE NULL

//Implement TpData's constructors
extern string tostring(int);
tpdata::tpdata(): name(""), valdefd(0)
{
  tp_just_instd__TRUE;
  static int howmany = 0;
  def = new externs("<nameless"+tostring(howmany++)+">",NULL,GARBAGE_SCOPE,EXTFLAG_TEMPPARAM | EXTFLAG_TYPEDEF);
}
tpdata::tpdata(string n,externs* d): name(n), val(0), standalone(false), valdefd(0)
{
  tp_just_instd__TRUE;
  def = new externs(n,d,GARBAGE_SCOPE,EXTFLAG_TYPEDEF | EXTFLAG_TEMPPARAM | EXTFLAG_TYPENAME | (d?EXTFLAG_DEFAULTED:0));
}
tpdata::tpdata(string n,externs* d, bool sa): name(n), val(0), standalone(sa), valdefd(0)
{
  tp_just_instd__TRUE;
  def = new externs(n,d,GARBAGE_SCOPE,(sa?0:EXTFLAG_TYPEDEF) | EXTFLAG_TEMPPARAM | EXTFLAG_TYPENAME | ((d and !sa)?EXTFLAG_DEFAULTED:0));
}
tpdata::tpdata(string n,externs* d, long long v, bool sa, bool vd): name(n), standalone(sa), valdefd(vd)
{
  tp_just_instd__TRUE;
  def = new externs(name,d,GARBAGE_SCOPE,(sa?0:EXTFLAG_TYPEDEF) | EXTFLAG_TEMPPARAM | EXTFLAG_TYPENAME | ((d and !sa)?EXTFLAG_DEFAULTED:0) | (vd?EXTFLAG_VALUED:0),v);
}

/*tpdata::~tpdata()
{
  delete def;
}*/

int tpc = -1;
varray<tpdata> tmplate_params;
void tmplate_params_clear_used(varray<tpdata> &vatp, int vs)
{
  for (int i=0; i<vs; i++)
  {
    vatp[i].def = NULL;
  }
}
void tmplate_params_clear(varray<tpdata> &vatp, int vs)
{
  for (int i=0; i<vs; i++)
  {
    delete vatp[i].def;
    vatp[i].def = NULL;
  }
}


ihdata::ihdata()
{
  parent = NULL;
  scopet = s_public;
}
ihdata::ihdata(externs* p,heredtypes t)
{
  parent = p;
  scopet = t;
}
int ihc;
varray<ihdata> inheritance_types;

bool find_in_parents(externs* whose, string name,unsigned int flags);
bool find_in_using(externs* inscope,string name, unsigned flags);
bool find_in_specializations(externs* inscope,string name, unsigned flags);

bool find_in_all_ancestors_generic(externs* whose, string name)
{
  extiter it = whose->members.find(name);
  if (it != whose->members.end())
  {
    ext_retriever_var = it->second;
    return 1;
  }

  if (find_in_specializations(whose,name,0xFFFFFFFF))
    return true;

  for (unsigned i=0; i<whose->ancestors.size; i++)
  {
    if (find_in_all_ancestors_generic(whose->ancestors[i],name))
      return 1;
  }
  return 0;
}


bool find_in_parents(externs* whose, string name,unsigned int flags)
{
  if (whose->name == name and whose->flags & flags) {
    ext_retriever_var = whose;
    return 1;
  }

  extiter it = whose->members.find(name);
  if (it != whose->members.end())
  {
    if ((it->second->flags & flags) or flags == 0xFFFFFFFF) {
      ext_retriever_var = it->second;
      return 1;
    } //else cout << it->second->flags << " : " << flags << " of " << strace(it->second) << endl << endl;
  }

  if (find_in_specializations(whose,name,flags))
    return true;

  for (unsigned i=0; i<whose->ancestors.size; i++)
  {
    externs* a = whose->ancestors[i];
    for (unsigned ii = 0; ii < a->ancestors.size; ii++) //FIXME: Should this function really be skipping whose->ancestors?
      if (find_in_parents(a->ancestors[ii],name,flags))
        return 1;
  }
  return 0;
}

bool find_in_using(externs* inscope,string name, unsigned flags)
{
  externs* using_scope = scope_get_using_ie(inscope);
  if (using_scope != NULL)
  {
    extiter it = using_scope->members.find(name);
    if (it != using_scope->members.end())
      if (((it->second->flags & flags) or flags == 0xFFFFFFFF)) {
        ext_retriever_var = it->second;
        return 1;
    }
    for (it = using_scope->members.begin(); it != using_scope->members.end(); it++)
    {
      if (it->second->flags & EXTFLAG_NAMESPACE)
      {
        extiter sit = it->second->members.find(name);
        if (sit != it->second->members.end()
        and  (((sit->second->flags & flags) != 0) or (flags == 0xFFFFFFFF)))
        {
          ext_retriever_var = sit->second;
          return 1;
        }
      }
    }
  }
  return 0;
}

bool find_in_specializations(externs* inscope,string name, unsigned flags)
{
  externs* spec_scope = temp_get_specializations(inscope);
  if (spec_scope != NULL)
  {
    for (extiter it = spec_scope->members.begin(); it != spec_scope->members.end(); it++)
    {
      extiter sit = it->second->members.find(name);
      if (sit != it->second->members.end()
      and (((it->second->flags & flags) != 0) or (flags == 0xFFFFFFFF)))
      {
        ext_retriever_var = sit->second;
        return 1;
      }
    }
  }
  return 0;
}

//This function is used a lot; it traverses scopes, checks
//template parameters and scope names, and filters by type.

extern string cferr;
  #include "../cfile_parse/cfile_parse_constants.h"
void print_err_line_at(pt a = pos);
bool find_extname(string name,unsigned int flags,bool expect_find)
{
  //If we've been given a qualified id, check in the path or give up
  if (immediate_scope != NULL)
  {
    externs* iscope = immediate_scope;
    while ((iscope->flags & EXTFLAG_TYPEDEF) and !(iscope->flags & EXTFLAG_HYPOTHETICAL) and iscope->type)
      iscope = iscope->type;

    extiter f = iscope->members.find(name);

    if (f == iscope->members.end())
    {
      if (iscope->flags & EXTFLAG_HYPOTHETICAL)
      {
        //We know it doesn't exist to our immediate findings; create it.
        ext_retriever_var = new externs(name,builtin_type__int,iscope,
          EXTFLAG_TYPENAME
          | EXTFLAG_TYPEDEF         // Not a base type, but typedef to NULL.
          | EXTFLAG_HYPOTHETICAL);  // HYPOTHETICAL is viral.

        if ((last_named & ~LN_TYPEDEF) == LN_TYPENAME and last_named_phase == TN_TEMPLATE) {
          ext_retriever_var->flags |= EXTFLAG_TEMPLATE;
          ext_retriever_var->tempargs[0] = tpdata().def;
        }

        iscope->members[name] = ext_retriever_var;
        immediate_scope = NULL;
        return true;
      }

      //Check ancestors
      for (unsigned i = 0; i < iscope->ancestors.size; i++)
        if (find_in_parents(iscope->ancestors[i],name,flags))
          return immediate_scope = NULL, true;

      if (find_in_using(iscope,name,flags))
        return immediate_scope = NULL, true;

      if (find_in_specializations(iscope,name,flags))
        return immediate_scope = NULL, true;

      if (expect_find) {
        cout << "Unable to locate member `" + name + "' in scope `" + strace(iscope) + "' (" + strace(immediate_scope) + ")" << endl;
        print_err_line_at(); immediate_scope = NULL;
        return find_extname(name,flags,expect_find);
      }
      return false;
    }
    ext_retriever_var = f->second;

    const bool ret = ((f->second->flags & flags) != 0) or (flags == 0xFFFFFFFF);
    if (ret) immediate_scope = NULL;
    return ret;
  } // immediate_scope != NULL

  //Start looking in this scope
  externs* inscope = current_scope;

  //If we're looking for a type name, try the template args
  if (flags & (EXTFLAG_TYPENAME | EXTFLAG_TEMPPARAM))
  {
    //The actual scope we're in should get search precedence, otherwise constructors will flop
    if (inscope->flags & EXTFLAG_TYPENAME)
    {
      if (inscope->name == name) {
        ext_retriever_var = inscope;
        return true;
      }
      if (inscope->ancestors.size and inscope->ancestors[0]->name == name) //Templates...
      {
        externs* a = temp_get_specializations_ie(inscope->ancestors[0]);
        //If we're in "a__int" (inscope), stored in parent is '__int', and name (our search) is 'a'
        if (a and name.length() < inscope->name.length()) //TODO: find this comment's twin and replace both with a CONSTRUCTOR extflag.
        {
          //Find templated constructor by this name in the parent class
          extiter it = a->members.find(inscope->name.substr(name.length()));
          if (it != a->members.end() and it->second == inscope) {
            ext_retriever_var = inscope;
            return true;
          }
        }
      }
    }

    //Check new templates
    for (int ti=0; ti<tpc; ti++)
    {
      if (tmplate_params[ti].name == name)
      {
        ext_retriever_var = tmplate_params[ti].def;
        if (ext_retriever_var == NULL)
          return 0;
        return 1;
      }
    }

    //Check scope's own templates
    for (unsigned ti=0; ti<current_scope->tempargs.size; ti++)
    {
      //cout << "if ("<<current_scope->tempargs[ti]->name<<" == "<<name<<") {\n";
      if (current_scope->tempargs[ti]->name == name) {
        ext_retriever_var = current_scope->tempargs[ti];
        return 1;
      }
    }
  }

  //Check all scopes here and above.
  extiter it = inscope->members.find(name);
  while (it == inscope->members.end()) //Until we find it
  {
    if (find_in_using(inscope,name,flags))
      return true;

    //Check ancestors
    for (unsigned i = 0; i < inscope->ancestors.size; i++)
      if (find_in_parents(inscope->ancestors[i],name,flags))
        return 1;
    
    //Check all typedefs
    externs *tdscope = inscope;
    while (tdscope->flags & EXTFLAG_TYPEDEF and (tdscope = tdscope->type))
    {
      it = inscope->members.find(name);
      if (it != inscope->members.end() and it->second->flags & flags)
        break;
    }
    
    if (inscope != &global_scope) //If we're not at global scope, move up
      inscope = inscope->parent; //This must ALWAYS be nonzero when != global_scope
    else return 0;

    //Check template params of new scope first
    if (flags & EXTFLAG_TYPENAME)
    {
      for (unsigned ti=0; ti<inscope->tempargs.size; ti++)
      {
        if (inscope->tempargs[ti]->name == name) {
          ext_retriever_var = inscope->tempargs[ti];
          return 1;
        }
      }

      //The actual scope we're in should then get search precedence, otherwise constructors will flop
      if (inscope->flags & EXTFLAG_TYPENAME and inscope->name == name) {
        ext_retriever_var = inscope;
        return true;
      }
    }


    //Try to find it as a member of this scope
    it = inscope->members.find(name);
    if (cfile_debug)
    {
      cout << "Found '" << name << "' in " << strace(inscope) << ": " << (it != inscope->members.end()) << endl;
    //tree_scope_members(inscope,1);
    //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl << endl << endl << endl << endl;
    }
  }
  ext_retriever_var = it->second;
  return ((it->second->flags & flags) != 0 or flags == 0xFFFFFFFF);

  return 0;
}


bool find_extname_global(string name,unsigned int flags)
{
  extiter it = global_scope.members.find(name);
  if (it != global_scope.members.end()) {
    ext_retriever_var = it->second;
    return (it->second->flags & flags) or (flags == 0xFFFFFFFF);
  }

  externs *using_scope = scope_get_using_ie(&global_scope);
  if (using_scope)
  for (it = using_scope->members.begin(); it != using_scope->members.end(); it++)
    if (it->second->name == name){
      ext_retriever_var = it->second;
      return (it->second->flags & flags) or (flags == 0xFFFFFFFF);
    }
    else if (it->second->flags & EXTFLAG_NAMESPACE)
    {
      extiter sit = it->second->members.find(name);
      if (sit != it->second->members.end()
      and  (((it->second->flags & flags) != 0) or (flags == 0xFFFFFFFF))) {
        ext_retriever_var = sit->second;
        return 1;
      }
    }

  return 0;
}
