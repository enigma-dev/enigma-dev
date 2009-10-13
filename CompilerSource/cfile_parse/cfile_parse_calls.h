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

inline bool is_letter(char x)
{
  return (x>='a' and x<='z') or (x>='A' and x<='Z') or (x=='_');
}

inline bool is_digit(char x)
{
  return x>='0' and x<='9';
}

inline bool is_letterd(char x)
{
  return  (x>='a' and x<='z') or (x>='A' and x<='Z') or (x=='_') or (x>='0' and x<='9');
}

inline bool is_useless(char x) 
{
  return x==' ' || x=='\r' || x=='\n' || x=='\t';
}

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
  or x=="const" 
  or x=="mutable" 
  or x=="volatile";
}



void regt(string x)
{
  externs* t = current_scope->members[x] = new externs;
  t->flags = EXTFLAG_TYPENAME;
  t->name = x;
}

void cparse_init()
{
  current_scope = &global_scope;
  
  regt("bool");
  regt("char");
  regt("int");
  regt("float");
  regt("double");
  
  regt("long");
  regt("short");
  regt("signed");
  regt("unsigned");
  regt("const");
  regt("static");
  regt("volatile");
  regt("register");
  regt("auto");
  
  #undef regt
}


int tpc;
varray<tpdata> tmplate_params;

int anoncount = 0;

bool ExtRegister(unsigned int last,string name,rf_stack refs,externs *type = NULL,varray<tpdata> &tparams = tmplate_params, int tpc = 0)
{
  unsigned int is_tdef = last & LN_TYPEDEF;
  last &= ~LN_TYPEDEF;
  
  if (name != "")
  {
    extiter it = current_scope->members.find(name);
    
    //cout << "  Receiving " << (refs.empty()?"empty":"unempty") << " reference stack\r\n";
    
    if (it != current_scope->members.end())
    {
      if (last != LN_NAMESPACE or !(it->second->flags & EXTFLAG_NAMESPACE))
      {
        cferr = "Redeclaration of `"+name+"' at this point";
        return 0;
      }
      ext_retriever_var = it->second;
      return 1;
    }
  }
  else
  {
    if (last == LN_STRUCT or last == LN_CLASS)
      name = "<anonymous"+tostring(anoncount++)+">";
    else
    {
      cferr = "Identifier required except in class definition";
      return 0;
    }
  }
  
  externs* e = new externs;
  current_scope->members[name] = e;
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
    e->flags = EXTFLAG_ENUM | EXTFLAG_TYPENAME;
  else if (last == LN_NAMESPACE)
    e->flags = EXTFLAG_NAMESPACE;
  
  if (is_tdef)
    e->flags |= EXTFLAG_TYPEDEF; //If this is a new type being typedef'd, it will later be undone
  
  if (tpc != 0)
  {
    e->flags |= EXTFLAG_TEMPLATE;
    for (int i=0; i<tpc; i++)
      e->tempargs[tparams[i].name] = tparams[i].def;
  }
  
  e->type = type;
  e->parent = current_scope;
  e->refstack = refs;
  
  return 1;
}
