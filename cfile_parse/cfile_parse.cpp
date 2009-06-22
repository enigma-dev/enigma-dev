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

#include <map>
#include <string>
#include <iostream>
#include "darray.h"

using namespace std;

#include "externs.h"
#include "expev_macros.h"
#include "value.h"

value evaluate_expression(string exp);
extern string rerr; extern int rerrpos;

inline bool is_tflag(string x)
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
  or x=="volatile";
}

bool find_extname(string name,unsigned int flags)
{
  externs* inscope=current_scope;
  extiter it = inscope->members.find(name);
  while (it==inscope->members.end())
  {
    if (inscope==&global_scope)
      return 0;
    inscope=inscope->parent;
    it = inscope->members.find(name);
  } 
  return ((it->second->flags & flags) != 0);
}





#define is_letter(x)  ((x>='a' && x<='z') || (x>='A' && x<='Z') || (x=='_'))
  #define is_digit(x)   (x>='0' && x<='9')
  #define is_letterd(x) (is_letter(x) || is_digit(x))
  #define is_unary(x)   (x=='!' || x=='~' || x=='+' || x=='-' || x=='&' || x=='*')
  #define is_linker(x)  (x=='+' || x=='-' || x=='*' || x=='/' || x=='=' || x=='!' || x=='~' || x=='&' || x=='|' || x=='^' || x=='.')
#define is_useless(x) (x==' ' || x=='\r' || x=='\n' || x=='\t')

inline void move_newline_a(string &cfile,unsigned int &pos,const unsigned int len)
{
  while (cfile[pos]!='\r' and cfile[pos]!='\n' and pos<len)
  { 
    pos++; 
    if (cfile[pos]=='\\' and pos+1<len)
    {
      pos++;
      if (cfile[pos]=='\\') 
      pos++; 
      else 
      {
        if (cfile[pos]=='\r') { pos++; if (cfile[pos]=='\n') pos++; }
        else if (cfile[pos]=='\n') pos++; 
      }
    }
  }
}

#define move_newline() move_newline_a(cfile,pos,len)

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

#define encountered pos++;
#define quote while (cfile[pos]!='"') { pos++; if (cfile[pos]=='\\' and (cfile[pos+1]=='\\'||cfile[pos]=='"')) pos+=2; }
#define squote while (cfile[pos]!='\'') { pos++; if (cfile[pos]=='\\' and (cfile[pos+1]=='\\'||cfile[pos]=='\'')) pos+=2; }

string cferr;
int parse_cfile(string cfile)
{
  cferr="No error";
  const unsigned int len=cfile.length();
  char type_named=0,identifier_named=0,fargs_named=0,targs_named=0;
  unsigned int fargs_count=0,targs_count=0;
  darray<char> fargs_array;
  string last_identifier,last_typename;
  bool skiptocomma=0,preprocallowed=1;
  int lvl=0;
  
  #define LN_NOTHING 0
  #define LN_TEMPLATE 1
  #define LN_CLASS 2
  #define LN_STRUCT 3
  #define LN_NAMESPACE 4
  #define LN_ENUM 5
  #define LN_USING 6
//  #define LN_TEMPLATE 5
  int last_named=LN_NOTHING;
  int last_named_phase=0;
  
  unsigned int pos=0;
  while (pos<len)
  {
    while (is_useless(cfile[pos])) 
    { 
      if (cfile[pos]=='\r' or cfile[pos]=='\n') 
        preprocallowed=1; 
      pos++; 
    }
    
    if (pos>210)
    {
      int a;
      a++;
    }
    
    //cout << cfile.substr(pos,7) << endl;
    
    if (!(pos<len))
    break;

    if (cfile[pos]=='#')
    {
      pos++;
      while(is_useless(cfile[pos])) pos++;
      
      unsigned int poss=pos;
      if (!is_letter(cfile[pos])) { cferr="Preprocessor directive expected"; return pos; }
      while (is_letterd(cfile[pos])) pos++;
      string next=cfile.substr(poss,pos-poss);
      
      if (next=="error")
      {
        move_newline();
        
        //Execute the following if and when ENIGMA supports #if
        /*int poss=pos;
        move_newline();
        cferr=cfile.substr(poss,pos-poss+1);
        return pos;*/
      }
      {
        if (next=="define")
        {
          while (is_useless(cfile[pos])) pos++;
          
          unsigned poss=pos;
          if (!is_letter(cfile[pos])) { cferr="Identifier expected for #define"; return pos; }
          while (is_letterd(cfile[pos])) pos++;
          
          unsigned int flags=0;
          
          string defiendum=cfile.substr(poss,pos-poss);
          if (cfile[pos]=='(') //macro function
          {
            flags |= EXTFLAG_FUNCTION;
            int lvl=1, an=0,ac=0;
            pos++;
            while (lvl>0 and pos<len)
            {
              if (cfile[pos]=='(') lvl++;
              else if (cfile[pos]==')') lvl--;
              else if (cfile[pos]==',') { if (an==0) { cferr="Identifier expected before ',' symbol"; return pos; } ac++; an=0; }
              else if (is_letter(cfile[pos])) 
              {
                if (an) { cferr="Symbol ',' expected"; return pos; } 
                while (is_letterd(cfile[pos])) pos++; pos--; 
                an=1; if (ac==0) ac=1;
              }
              else if (cfile[pos]!=' ') { cferr="Unexpected symbol in macro parameters"; return pos; }
              pos++;
            }
          }
          
          while (is_useless(cfile[pos])) pos++;
          
          poss=pos;
          move_newline();
          string defiens=cfile.substr(poss,pos-poss);
          
          //cout << "Define \"" << defiendum << "\" as \"" << defiens << "\"\r\n";
          if (current_scope->members.find(defiendum) != current_scope->members.end())
            current_scope->members.erase(defiendum);
          macros[defiendum]=defiens;
        }
        if (next=="undef")
        {
          
        }
      }
      {
        if (next=="include")
        {
          
        }
        if (next=="import")
        {
          move_newline();
        }
        if (next=="using")
        {
          move_newline();
        }
      }
      {
        if (next=="if" or next=="ifdef" or next=="ifndef" or next=="else" or next=="elif" or next=="endif")
        {
          move_newline();
        }
      }
      if (next=="line")
      {
        cferr="Fuck you.";
        return pos;
      }
      if (next=="pragma")
      {
        move_newline();
      }
    }
    else //it's not a macro
    {
      preprocallowed=0;
      if (skiptocomma)
      { 
        while (!is_useless(cfile[pos]) and cfile[pos]!=';' and cfile[pos]!=',')
        {
          if (cfile[pos]=='"') { encountered quote }
          if (cfile[pos]=='\'') { encountered squote }
          if (cfile[pos]=='(') lvl++;
          if (cfile[pos]==')') lvl--;
          if (cfile[pos]=='[') lvl++;
          if (cfile[pos]==']') lvl--;
          if (cfile[pos]=='{') lvl++;
          if (cfile[pos]=='}') lvl--;
          if (cfile[pos]=='<') lvl++;
          if (cfile[pos]=='>') lvl--;
          pos++;
        }
        if (cfile[pos]!=';' and cfile[pos]!=',')
        continue; else { skiptocomma=0; }
      }
      if (last_named==LN_NAMESPACE)
      {
        if (last_named_phase==0) //expect namespace identifier
        {
          if (!is_letter(cfile[pos]))
          { cferr="Expected identifier at this point"; return pos; }
          int poss=pos; while (is_letterd(cfile[pos])) pos++;
          string n=cfile.substr(poss,pos-poss);
          
          extiter it=current_scope->members.find(n);
          if (it != current_scope->members.end())
          {
            if (it->second->flags & EXTFLAG_NAMESPACE)
              current_scope =  it->second;
            else
            { cferr="Redeclaration of `"+n+"'"; return pos; }
          }
          else
          {
            externs* nns = it->second->members[n]=new externs;
            nns->flags = EXTFLAG_NAMESPACE;
            nns->parent = current_scope;
            current_scope=nns;
            nns->name = n;
          }
          last_named_phase++;
        }
        else if (last_named_phase==1)
        {
          if (cfile[pos]!='{') 
          { cferr="Expected '{' at this point"; return pos; }
          last_named_phase=0; last_named=LN_NOTHING;
          pos++;
        }
        continue;
      }
      if (last_named==LN_CLASS | last_named==LN_STRUCT)
      {
        if (last_named_phase==0) //expect namespace identifier
        {
          if (!is_letter(cfile[pos]))
          { cferr="Expected identifier at this point"; return pos; }
          int poss=pos; while (is_letterd(cfile[pos])) pos++;
          string n=cfile.substr(poss,pos-poss);
          
          extiter it=current_scope->members.find(n);
          if (it != current_scope->members.end())
          {
            cferr="Redeclaration of `"+n+"'"; 
            return pos;
          }
                    
          unsigned int extf;
          if (last_named==LN_STRUCT) extf = EXTFLAG_STRUCT;
          if (last_named==LN_CLASS) extf = EXTFLAG_CLASS;
          extf |= EXTFLAG_TYPENAME;
          
          externs* nns = it->second->members[n]=new externs;
          nns->parent = current_scope;
          nns->flags = extf;
          current_scope=nns;
          nns->name = n;
          
          last_named_phase++;
        }
        else if (last_named_phase==1)
        {
          if (cfile[pos]!='{') 
          { cferr="Expected '{' at this point"; return pos; }
          last_named_phase=0; last_named=LN_NOTHING;
          pos++;
        }
        continue;
      }
      
      if (!is_letter(cfile[pos])) //NOT a letter
      {
        if (cfile[pos]=='=')
        {
          skiptocomma=1;
        }
        else if (cfile[pos]=='(')
        {
          int lvl=1; pos++;
          if (fargs_named) { cferr="Multiple sets of function parameters named"; return pos; }
          bool eq=0,nl=0;
          while (pos<len and lvl>0)
          {
            if (cfile[pos]=='"') { encountered quote }
            else if (cfile[pos]=='\'') { encountered squote }
            else if (cfile[pos]=='(') lvl++;
            else if (cfile[pos]==')') lvl--;
            else if (cfile[pos]=='[') lvl++;
            else if (cfile[pos]==']') lvl--;
            else if (cfile[pos]=='{') lvl++;
            else if (cfile[pos]=='}') lvl--;
            else if (cfile[pos]=='<') lvl++;
            else if (cfile[pos]=='>') lvl--;
            else if (cfile[pos]==',') { fargs_array[fargs_count++]=eq; eq=0; nl=0; }
            else nl=1; 
            pos++;
          }
          if (nl) { fargs_array[fargs_count++]=eq; eq=0; nl=0; }
          fargs_named=1;
        }
        else if (cfile[pos]=='{' or cfile[pos]=='[')
        {
          int lvl=1; pos++;
          while (pos<len and lvl>0)
          {
            if (cfile[pos]=='"') { encountered quote }
            if (cfile[pos]=='\'') { encountered squote }
            if (cfile[pos]=='(') lvl++;
            if (cfile[pos]==')') lvl--;
            if (cfile[pos]=='[') lvl++;
            if (cfile[pos]==']') lvl--;
            if (cfile[pos]=='{') lvl++;
            if (cfile[pos]=='}') lvl--;
            if (cfile[pos]=='<') lvl++;
            if (cfile[pos]=='>') lvl--;
            pos++;
          }
        }
        else if (cfile[pos]=='}')
        {
          if (current_scope == &global_scope)
          { cferr="Unexpected closing brace at this point"; return pos; }
          if (current_scope->flags & (EXTFLAG_CLASS | EXTFLAG_STRUCT))
            last_typename = current_scope->name;
          current_scope = current_scope->parent; pos++;
        }
        else if ((cfile[pos]==';') or (cfile[pos]==','))
        {
          //check name for conflict, register it, wipe name and function args, wipe type name and template args if this is ';'
          
          //As a preliminary, make sure they actually said something -.-
          if (!identifier_named)
          {
            if (type_named) { cferr="Declaration doesn't declare anything."; return pos; }
            pos++; continue;
          }
          
          //Moving on. First, check for conflict
          extiter it;
          it = current_scope->members.find(last_identifier);
          if (it != current_scope->members.end()) //There's already something by this name in the current scope, and this is a ;
          {
            if ((it->second->flags & EXTFLAG_FUNCTION)==0)
            {
              cferr="Redeclaration of `"+last_identifier+"'";
              return pos;
            }
            else if (!fargs_named)
            {
              cferr="Function `"+last_identifier+"' can only be overlaoded as function";
              return pos;
            }
          }
          
          //next, register it
          externs* x = current_scope->members[last_identifier] = new externs;
          
          x->name = last_identifier;
          x->flags=((fargs_named!=0)*EXTFLAG_FUNCTION) | ((targs_named!=0)*EXTFLAG_TEMPLATE);
          if (last_typename[last_typename.length()-1]==' ') last_typename.erase(last_typename.length()-1,1);
          /*
          string tscope="";
          for (externs* i=current_scope; i != &global_scope; i=i->parent) tscope+=i->name + "::";*/
          
          if (fargs_named)
          {
            for (unsigned int i=0;i<fargs_count;i++)
            x->fargs[i]=fargs_array[i];
          }
          
          //cout<<"Added `"<<last_identifier<<"'.\r\n";
          
          
          //wipe name and function args
          fargs_named=0; fargs_count=0;
          identifier_named=0; last_identifier="";
          
          //wipe type name and template args if this is ;
          if (cfile[pos]==';')
          {
            targs_named=0; targs_count=0;
            type_named=0; last_typename="";
          }
          
          pos++;
        }
        else if (cfile[pos]=='/')
        {
          pos++;
          if (cfile[pos]=='/')
          {
            while (cfile[pos]!='\r' and cfile[pos]!='\n') pos++;
          }
          else if (cfile[pos]=='*')
          {
            pos++;
            while (cfile[pos]!='*' and cfile[pos+1]!='/') pos++;
            pos+=2;
          }
          else { cferr="Unexpected symbol reached"; return pos; }
        }
        else { cferr="Unexpected symbol at this point"; return pos; }
      }
      else //it's an identifier or keyword
      {
        int poss=pos;
        while is_letterd(cfile[pos]) pos++;
        string lword=cfile.substr(poss,pos-poss);
        
        if (type_named!=1) //Expect typename or keyword
        {
          if (!is_tflag(lword)) //It's not unsigned, or the like
          {
            if (!find_extname(lword,EXTFLAG_TYPENAME)) //Didn't find it on the list of typenames
            {
              if (type_named!=2) //We haven't named LONG or anything
              { 
              //This means it'd best be a keyword.
                if (last_named != LN_NOTHING)
                { cferr="Unexpected keyword in expression"; return pos; }
                if (lword=="template")
                {
                  last_named=LN_TEMPLATE;
                }
                else if (lword=="struct")
                {
                  last_named=LN_STRUCT;
                }
                else if (lword=="class")
                {
                  last_named=LN_CLASS;
                }
                else if (lword=="namespace")
                {
                  last_named=LN_NAMESPACE;
                }
                else if (lword=="using")
                {
                  last_named=LN_USING;
                }
                else if (lword=="typedef")
                {
                  last_named=LN_TEMPLATE;
                }
                else //alas, it is nothing we can deal with
                { cferr="Expected type name or keyword; unknown type `"+lword+"'"; return pos; }
                continue; //Don't count this as a typename or flag
              }
              else //It's an identifier of type "long" or "short"
              {
                identifier_named=1;
                last_identifier=lword;
              }
            }
            type_named=1; last_typename+=lword+" "; continue;
          }
          
          last_typename+=lword+" ";
          if (lword=="short" or lword=="long") type_named=2;
          continue;
        }
        else if (!identifier_named)
        {
          identifier_named=1;
          last_identifier=lword;
        }
        else { cferr="Symbol ',' or ';' expected."; return pos-lword.length(); }
      }
      
      //iterator_for_namemap f=namemap.find(THE FOLLOWING WORD)
      
      //if (f==namemap.end())
      //{ cferr="Expected declarator before undefined identifier"; return pos; }
      
      
    }
  }
  return -1;
}

void print_scope_members(externs* gscope, int indent)
{
  for (extiter i=gscope->members.begin();i!=gscope->members.end();i++)
  {
    bool comma=0;
    string indstr(indent,' ');
    
    cout << indstr << i->second->name << ":  ";
    
    if (i->second->flags&EXTFLAG_FUNCTION)
    {
      cout << " function (" << i->second->fargs.size << " parameters)"/* returning "<<(i->second)[0].macrotext*/;
      comma=1;
    } /*else cout << " " << (i->second)[0].macrotext;*/
    
    if (i->second->flags&EXTFLAG_TEMPLATE)
    {
      cout << " template with " << i->second->targs.size << " parameters";
      comma=1;
    }
    
    if (i->second->flags & EXTFLAG_TYPENAME)
    {
      if (comma) 
      cout << ", serves as typename";
      else
      cout << " Serves as typename";
    }
    
    if (i->second->flags & EXTFLAG_NAMESPACE) cout << " namespace";
    if (i->second->flags & EXTFLAG_STRUCT) cout << " : struct";
    if (i->second->flags & EXTFLAG_CLASS) cout << " : class";
    
    if (!i->second->members.empty())
    {
      cout << "\r\n" << indstr << "{\r\n";
      print_scope_members(i->second,indent+2);
      cout << indstr << "}\r\n";
    }
    
    cout << "\r\n";
  }
}

string file_contents(string file)
{
  FILE *f = fopen(file.c_str(),"rb");
  if (f==NULL) return "";
  
  fseek(f,0,SEEK_END);
  size_t sz = ftell(f);
  char a[sz];
  fseek(f,0,SEEK_SET);
  fread(a,sz,1,f);
  fclose(f);
  return a;
}

int main()
{
  cparse_init();
  
  string fc=file_contents("C:\\cfile.h");
  int a=parse_cfile(fc);
  
  if (a != -1)
  {
    int line=0,pos=0;
    for (int i=0; i<a; i++)
    {
      if (fc[i]=='\r')
        i++;
      if (fc[i]=='\n')
        line++, pos++;
    }
    printf("Line %d, position %d: %s\r\n",line,pos,cferr.c_str());
  }
  
  cout << "Macros:\r\n";
  for (maciter i=macros.begin(); i!=macros.end();i++)
    cout<<"  "<<i->second<<"\r\n";
  
  print_scope_members(&global_scope, 0);
  
  system("pause");
  
  #if 0
  while (1)
  {
    string p;
    char ghere[2048];
    value val;
    cin.getline(ghere,2048);
    p=ghere;
    if (p=="x") return 0;
    if (p=="c" or p=="cls") system("cls");
    else
    {
      val=evaluate_expression(p);
      #if USETYPE_INT
        if (val.type==RTYPE_INT)
          cout<<val.real.i;
      #endif
      #if USETYPE_DOUBLE
        if (val.type==RTYPE_DOUBLE)
          cout<<val.real.d;
      #endif
      #if USETYPE_UINT
        if (val.type==RTYPE_UINT)
          cout<<val.real.u;
      #endif
      #if USETYPE_STRING
        if (val.type==RTYPE_STRING)
          cout << '"' << val.str << '"';
      #endif
      cout << endl;
      if (rerr != "") cout <<"error at "<<rerrpos<<": "<<rerr << "\r\n\r\n";
    }
  }
  #endif
  
  return 0;
}
