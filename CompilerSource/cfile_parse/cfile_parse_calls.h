/*********************************************************************************\
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
\*********************************************************************************/


//Some things that only cause clutter when left in the same file

#include "../general/parse_basics.h"

bool is_tflag(string x)
{
  return 
     x=="unsigned" 
  or x=="signed" 
  or x=="register" 
  or x=="extern" 
  or x=="short" 
  or x=="long" 
  or x=="static" 
  or x=="mutable" 
  or x=="volatile";
}



inline externs* regt(string x)
{
  externs* t = current_scope->members[x] = new externs;
  t->flags = EXTFLAG_TYPENAME;
  t->name = x;
  return t;
}
inline void regmacro(string m)
{
  macros[m];
}
inline void regmacro(string m,string val)
{
  macros[m] = val;
}
inline void regmacro(string m,string val,string arg1)
{
  macro_type *mac = &macros[m];
  *mac = val;
  mac->assign_func(m);
  mac->addarg(arg1);
}

externs *builtin_type__int, *builtin_type__void;
varray<string> include_directories;
unsigned int include_directory_count;

void cparse_init()
{
  current_scope = &global_scope;
  
  regt("bool");
  regt("char");
  builtin_type__int = regt("int");
  regt("float");
  regt("double");
  
  builtin_type__void = regt("void"); //this was only after careful consideration
  
  regt("long");
  regt("short");
  regt("signed");
  regt("unsigned");
  regt("const");
  regt("static");
  regt("volatile");
  regt("register");
  regt("auto");
  
  //lesser used types
  //regt("size_t"); //size_t doesn't need registered here as it is typdef'd in stdio somewhere.
  regt("wchar_t");
  
  //__builtin_ grabbage
  regt("__builtin_va_list");
  
  #undef regt
  
  //These are GCC things and must be hard coded in
  regmacro("__attribute__","","x"); //__attribute__(x) 
  regmacro("__typeof__","int","x"); //__attribute__(x) 
  regmacro("__extension__"); //__extension__
  regmacro("false","0"); //false
  regmacro("true","1"); //true
  
  #ifdef linux
  include_directories[0] = "/usr/include/";
  include_directories[1] = "/usr/include/c++/4.3/";
  include_directories[2] = "/usr/include/c++/4.3/i486-linux-gnu/";
  include_directories[3] = "/usr/lib/gcc/i486-linux-gnu/4.3.3/include/";
  include_directory_count = 4;
  #else
  include_directories[0] = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\include\\";
  include_directories[1] = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\lib\\gcc\\mingw32\\3.4.5\\install-tools\\include\\";
  include_directories[2] = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\include\\c++\\3.4.5\\";
  include_directories[3] = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\include\\c++\\3.4.5\\mingw32\\";
  include_directory_count = 4;
  #endif
  
}



int anoncount = 0;
bool ExtRegister(unsigned int last,unsigned phase,string name,rf_stack refs,externs *type = NULL,varray<tpdata> &tparams = tmplate_params, int tpc = -1,long long last_value = 0)
{
  unsigned int is_tdef = last & LN_TYPEDEF;
  last &= ~LN_TYPEDEF;
  
  externs *scope_to_use = immediate_scope ? immediate_scope : current_scope;
  if (name != "")
  {
    extiter it = scope_to_use->members.find(name);
    
    //cout << "  Receiving " << (refs.empty()?"empty":"unempty") << " reference stack\r\n";
    
    if (it != scope_to_use->members.end())
    {
      if (last == LN_NAMESPACE and (it->second->flags & EXTFLAG_NAMESPACE)) {
        ext_retriever_var = it->second;
        return 1;
      }
      else
      {
        if (it->second->is_function() and refs.is_function())
        {
          it->second->parameter_unify(refs);
          ext_retriever_var = it->second;
          return 1;
        }
        else
        {
          if (it->second->parent == current_scope) {
            cferr = "Redeclaration of `"+name+"' at this point";
            return 0;
          }
          if (!(it->second->parent->flags & EXTFLAG_TEMPLATE)) {
            cferr = "Cannot declare that here.";
            return 0;
          }
          ext_retriever_var = it->second;
          return 1;
        }
      }
    }
  }
  else
  {
    if (last != LN_NAMESPACE)
      name = "<anonymous"+tostring(anoncount++)+">";
    else
    {
      cferr = "Identifier required for namespace definition";
      return 0;
    }
  }
  
  externs* e = new externs;
  scope_to_use->members[name] = e;
  ext_retriever_var = e;
  e->name = name;
  
  e->flags = 0;
  if (last == LN_CLASS)
    e->flags = EXTFLAG_CLASS | EXTFLAG_TYPENAME;
  else if (last == LN_STRUCT)
    e->flags = EXTFLAG_STRUCT | EXTFLAG_TYPENAME;
  else if (last == LN_UNION) // Same as struct, for our purposes. 
    e->flags = EXTFLAG_STRUCT | EXTFLAG_TYPENAME; //sizeof() can't be used in Macros, you bunch of arses.
  else if (last == LN_ENUM)
    e->flags = (phase == EN_IDENTIFIER or phase == EN_NOTHING) ? EXTFLAG_ENUM | EXTFLAG_TYPENAME : EXTFLAG_VALUED;
  else if (last == LN_NAMESPACE)
    e->flags = EXTFLAG_NAMESPACE;
  
  if (is_tdef)
    e->flags |= EXTFLAG_PENDING_TYPEDEF; //If this is a new type being typedef'd, it will later be undone
  
  if (tpc > 0)
  {
    e->flags |= EXTFLAG_TEMPLATE;
    for (int i=0; i<tpc; i++)
    {
      if (!tparams[i].standalone)
        tparams[i].def->parent = e;
      else
      {
        if (tparams[i].def == NULL) {
          cferr = "Parse error: Template parameter is marked as definite but defined as NULL";
          return 0;
        }
      }
      e->tempargs[e->tempargs.size] = tparams[i].def;
    }
  }
  
  e->type = type;
  e->parent = current_scope;
  e->value_of = last_value;
  e->refstack = refs;
  
  immediate_scope = NULL;
  return 1;
}

#include "expression_evaluator.h"

map<string,bool> constant_types; //Load this with types like "int" and flags like "unsigned"
struct init_const_types { init_const_types() { //These are acceptable for const-expressions
  constant_types["bool"] = 1; 
  constant_types["char"] = 1; 
  constant_types["short"] = 1; 
  constant_types["long"] = 1; 
  constant_types["int"] = 1; 
  constant_types["unsigned"] = 1; 
  constant_types["signed"] = 1; 
  constant_types["const"] = 1; 
  constant_types["static"] = 1; 
  } } init_const_types_now;

string temp_parse_seg(string seg, externs* type_default, externs **kt = NULL)
{
  //cout << seg << endl;
  if (type_default->flags & EXTFLAG_TYPEDEF)
  {
    string p = "";
    externs* is = immediate_scope;
    immediate_scope = NULL;
    
    unsigned lvl = 0;
    for (unsigned i = 0; i<seg.length(); i++)
    {
      if (is_letter(seg[i]) and lvl == 0)
      {
        const unsigned is = i;
        while (is_letterd(seg[++i]));
        string tn = seg.substr(is,i-is);
        
        if (!find_extname(tn,EXTFLAG_TYPENAME | EXTFLAG_NAMESPACE)) {
          cferr = "`"+tn+"' cannot be used in a constant expression <" + type_default->name + ":" + tostring(type_default->flags) + "> [" + seg + "] in ";
          return "";
        }
        
        //cout << "find " << tn << ": ";
        if (ext_retriever_var->flags & EXTFLAG_NAMESPACE) {
          immediate_scope = ext_retriever_var;
          //cout << "move into\n";
        }
        else {
          immediate_scope = NULL;
          //cout << "use\n";
        }
        
        if (kt)
          *kt = ext_retriever_var;
        p += "_" + tn;
      }
      else if (seg[i] == '<')
        lvl++;
      else if (seg[i] == '>')
        lvl--;
    }
    immediate_scope = is;
    
    if (p == "")
      cferr = "No valid type named in template parameters <" + type_default->name + ":" + tostring(type_default->flags) + "> [" + seg + "] in ";
    return p;
  }
  else if (type_default->flags & EXTFLAG_TYPENAME)
  {
    if (constant_types.find(type_default->type->name) == constant_types.end()) {
      cferr = "Type `"+type_default->type->name+"' cannot be used in a constant expression... [" + seg + "] in ";
      return "";
    }
    
    value a = evaluate_expression(seg);
    if (rerrpos != -1) {
      cferr = "Error in template expression, position " + tostring(rerrpos) + ": " + rerr + " [" + seg + "] in ";
      return "";
    }
    
    if (kt)
    {
      externs *a = *kt = new externs;
      a->name = type_default->name;
      a->flags = type_default->flags | EXTFLAG_DEFAULTED | EXTFLAG_VALUED;
      a->parent = current_scope;
      a->value_of = (long long) a;
      a->type = NULL;
    }
    return "_" + tostring((UTYPE_INT)a);
  }
  cferr = "Macro parameter set up wrong. This error shouldn't occur...  [" + seg + "] in ";
  return "";
}

string temp_parse_list(externs* last,string specs,varray<externs*> *va = NULL)
{
  string ns;
  unsigned is = 0, ti = 0, cnt = 0, i;
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
    if (last->tempargs[ti]->flags & EXTFLAG_DEFAULTED)
    {
      if (va)
      {
        externs *nta = (*va)[ti] = new externs;
        
        nta->name = last->tempargs[ti]->name;
        nta->type = last->tempargs[ti]->type;
        nta->parent = last->tempargs[ti]->parent;
        nta->flags = last->tempargs[ti]->flags;
        nta->value_of = last->tempargs[ti]->value_of;
      }
      ti++;
    }
    else {
      cferr = "Too few parameters to template";
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
      @b->tempargs is known simply as @last->tempargs, and represents the original list.
      @template <typename a> has not yet been dumped into anything, and is accessed with  @tmplate_params.
      @implementations is a new array we allocate, and contains data from parsed implementation string @specs.
  */
  
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
  
  externs* ret = new externs;
  ret->name = last->name + ns;
  ret->flags = last->flags;
  ret->type = last->type;
  ret->parent = last->parent;
  
  //cout << "for (unsigned i = 0; i < " << implementations.size << " and i < " << last->tempargs.size << "; i++)" << endl;
  for (unsigned i = 0; i < last->tempargs.size; i++) 
  {
    externs* n = ret->tempargs[i] = new externs;
    n->flags = last->tempargs[i]->flags;
    n->name = last->tempargs[i]->name;
    n->type = implementations[i];
    n->flags = (last->tempargs[i]->flags & ~EXTFLAG_DEFAULTED) | (n->type?EXTFLAG_DEFAULTED:0);
    n->parent = ret;
    
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
  
  temp_get_specializations(last)->members[ns] = ret;
  return ret;
}

bool access_specialization(externs* &whom, string specs)
{
  if (not(whom->flags & EXTFLAG_TEMPLATE)) {
    cferr = "Attempting to access specialization of non-template type `" + whom->name + "'";
    return NULL;
  } //cout << "Accessing specialization <" << specs << "> from " << whom->name << endl ;
  
  string ns = temp_parse_list(whom,specs);
  if (ns == "")
    return 0;
  
  externs* s = temp_get_specializations(whom);
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
  return 1;
}



unsigned int handle_comments()
{
  if (cfile[pos]=='/')
  {
    pos++;
    if (cfile[pos]=='/')
    {
      while (cfile[pos] != '\r' and cfile[pos] != '\n' and (pos++)<len);
      return unsigned(-2);
    }
    if (cfile[pos]=='*')
    {
      int spos=pos;
      pos+=2;
      
      while ((cfile[pos] != '/' or cfile[pos-1] != '*') and (pos++)<len);
      if (pos>=len)
      {
        cferr="Unterminating comment";
        return spos;
      }
      
      pos++;
      return unsigned(-2);
    }
    pos--;
  }
  return unsigned(-1);
}
