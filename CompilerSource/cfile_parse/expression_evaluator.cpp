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
#include "../general/implicit_stack.h"

using namespace std;

#include "../externs/externs.h"
#include "expev_macros.h"
#include "value.h"

#include "macro_functions.h"

string tostring(int val);

int precedence[] = PRECEDENCE;
bool nz(value v)
{
  #if USETYPE_INT
    if (v.type==RTYPE_INT) return v.real.i!=0;
    #endif
  #if USETYPE_DOUBLE
    if (v.type==RTYPE_DOUBLE) return v.real.d!=0;
    #endif
  #if USETYPE_UINT
    if (v.type==RTYPE_UINT) return v.real.u!=0;
  #endif
  return 0;
}

bool is_opkeyword(string x)
{
  return x=="mod" or
  #if USE_INTIGERDIV
    x=="div" or
  #endif
    x=="and" or
    x=="or" or
    x=="xor";
}

#define pos position()
#define len explength()
#define exp expression()
string rerr=""; int rerrpos=-1;
value evaluate_expression(string expr)
{
  implicit_stack<string> expression;
  implicit_stack<pt> position;
  exp=expr;
  pos=0;
  
  implicit_stack<pt> explength;
  len=exp.length();
  
  unsigned int macrod=0;
  varray<string> inmacros; //To detect recursion
  varray<varray<value> > regval; //stack of stack is necessary for precedence
  varray<darray<int> > op; //Operator at this level, stack of stack for precedence
  varray<darray<int> > unary; //Unary, stack of stack for multiple: !! is a good example
  darray<int> uc,opc; //How many (unary, regular) operators there are on this level
  int level=0; //Parenthesis level
  
  VZERO(regval[0][0].); //zero the value
  regval[0][0].type = RTYPE_NONE;
  op[0][0] = OP_NONE; //no operator
  unary[0][0] = 0; //For posterity
  opc[0] = 0; //no normal operators
  uc[0] = 0; //no unary operators
  
  #if CASTS_ALLOWED
   darray<int> is_cast; //Tells if this level casts and how many flags there are
   is_cast[0] = 0;
  #endif
  
  //Note that the number of values should be the same as number of operators + 1.
  
  rerr="No error";
  rerrpos=-1;
  
  for (;;) //since goto is out of the question
  {
    if (!(pos<len))
    {
      if (expression.ind>0)
      {
        position.pop();
        explength.pop();
        expression.pop();
        macrod--;
        continue;
      }
      else break;
    }
    
    if (is_useless(exp[pos])) { pos++; continue; }
    
    #if CASTS_ALLOWED
      if (is_cast[level] and exp[pos]!=')' and !is_letter(exp[pos]))
      {
        cout << "'" << exp[pos] << "' = bad\r\n";
        rerr="Unexpected symbol in cast";
        rerrpos=pos;
        return 0;
      }
    #endif
    
    if (is_letterd(exp[pos]) or exp[pos]=='.' or exp[pos]==')' or exp[pos]=='"' or exp[pos]=='\'')
    {
      //cout << "Entering at " << pos << " -> " << regval[level][opc[level]].type << endl;
      long long val = 0;
      long double dval = 0;
      value setval;
      if (is_letter(exp[pos]))
      {
        if (exp[pos] == 'L' and (exp[pos+1] == '"' or exp[pos+1] == '\''))
        { pos++; continue; }
        
        int sp = pos;
        while (is_letterd(exp[pos])) pos++;
        string n = exp.substr(sp,pos-sp);
        
        #if CASTS_ALLOWED
          if (level > 0 and opc[level] == 0) //This is a parenthetical cast, and doesn't count as a level. Apply to the one below.
          {
            //FIXME: This should probably be eliminated in favor of the code below, but result in the unary being transfered to the level below at closing parenth
            if (n=="bool")     { unary[level - 1][uc[level - 1]++]=UNARY_BOOL;     is_cast[level]++; continue; }
            if (n=="char")     { unary[level - 1][uc[level - 1]++]=UNARY_CHAR;     is_cast[level]++; continue; }
            if (n=="int")      { unary[level - 1][uc[level - 1]++]=UNARY_LONG;     is_cast[level]++; continue; }
            if (n=="float")    { unary[level - 1][uc[level - 1]++]=UNARY_FLOAT;    is_cast[level]++; continue; }
            if (n=="double")   { unary[level - 1][uc[level - 1]++]=UNARY_DOUBLE;   is_cast[level]++; continue; }
            if (n=="short")    { unary[level - 1][uc[level - 1]++]=UNARY_SHORT;    is_cast[level]++; continue; }
            if (n=="long")     { unary[level - 1][uc[level - 1]++]=UNARY_LONG;     is_cast[level]++; continue; }
            if (n=="signed")   { unary[level - 1][uc[level - 1]++]=UNARY_SIGNED;   is_cast[level]++; continue; }
            if (n=="unsigned") { unary[level - 1][uc[level - 1]++]=UNARY_UNSIGNED; is_cast[level]++; continue; }
            if (n=="const")    { is_cast[level]=1; continue; }
            
            if (is_cast[level])
            {
              rerr="Unexpected symbol in cast";
              rerrpos=pos;
              return 0;
            }
          }
          else //Append unary in this level.
          {
            if (n=="bool")     { unary[level][uc[level]++]=UNARY_BOOL;     continue; }
            if (n=="char")     { unary[level][uc[level]++]=UNARY_CHAR;     continue; }
            if (n=="int")      { unary[level][uc[level]++]=UNARY_LONG;     continue; }
            if (n=="float")    { unary[level][uc[level]++]=UNARY_FLOAT;    continue; }
            if (n=="double")   { unary[level][uc[level]++]=UNARY_DOUBLE;   continue; }
            if (n=="short")    { unary[level][uc[level]++]=UNARY_SHORT;    continue; }
            if (n=="long")     { unary[level][uc[level]++]=UNARY_LONG;     continue; }
            if (n=="signed")   { unary[level][uc[level]++]=UNARY_SIGNED;   continue; }
            if (n=="unsigned") { unary[level][uc[level]++]=UNARY_UNSIGNED; continue; }
            if (n=="const")    { is_cast[level] = 1; continue; }
          }
          
        #endif
        
        #if USE_DEFINED_KEYWORD == 1
          if (n == "defined")
          {
            bool inps = 0;
            while (is_useless(exp[pos])) pos++;
            if (exp[pos] == '(')
            {
              pos++; while (is_useless(exp[pos])) pos++;
              inps = 1;
            }
            if (!is_letter(exp[pos])) 
            {
              rerr = "Expected identifier following `defined' token";
              rerrpos = pos;
              return 0;
            }
            const pt spos = pos;
            while (is_letterd(exp[pos])) pos++;
            
            bool addthis = (macros.find(exp.substr(spos,pos-spos)) != macros.end());
            setval = (value)addthis;
            
            if (inps)
            {
              while (is_useless(exp[pos])) pos++;
              if (exp[pos] != ')')
              { rerr = "Expected closing parenthesis after `defined' expression"; rerrpos = pos; return 0; }
              pos++;
            }
            
            goto electron_transport_chain;
          }
        #endif
        
        if (is_opkeyword(n))
        {
          if (regval[level][opc[level]].type==RTYPE_NONE)
          {
            rerr="Expected primary expression before `"+n+"' keyword";
            rerrpos=pos; return 0;
          }
          #if USE_INTIGERDIV
            else if (n=="div")
            {
              op[level][opc[level]++]=OP_IDIV;
              regval[level][opc[level]].type=RTYPE_NONE;
            }
          #endif
          else if (n=="mod")
          {
            op[level][opc[level]++]=OP_MOD;
          }
          else if (n=="and")
          {
            #if LOGIC_BOOLEAN
            op[level][opc[level]++]=OP_BOOLAND;
            #else
            op[level][opc[level]++]=OP_AND;
            #endif
          }
          else if (n=="or")
          {
            #if LOGIC_BOOLEAN
            op[level][opc[level]++]=OP_BOOLOR;
            #else
            op[level][opc[level]++]=OP_OR;
            #endif
          }
          else if (n=="xor")
          {
            #if LOGIC_BOOLEAN and BOOLXOR_ALLOWED
              op[level][opc[level]++]=OP_BOOLXOR;
            #else
              op[level][opc[level]++]=OP_XOR;
            #endif
          }
          
          regval[level][opc[level]].type=RTYPE_NONE;
          continue;
        }
        
        
        //Look up the value. In this case, this is a macro, and so we can not treat it as a number and must continue.
        //This part must be manually edited for compatibility with other languages or purposes.
        maciter i=macros.find(n);
        if (i != macros.end())
        {
          bool recurs=0;
          for (unsigned int iii=0;iii<macrod;iii++)
          if (inmacros[iii]==n) { recurs=1; break; }
          if (!recurs)
          {
            string macrostr = i->second;
            if (i->second.argc != -1) //Expect ()
            {
              pt pst = pos;
              if (!macro_function_parse(exp,n,pst,macrostr,i->second.args,i->second.argc,i->second.args_uat))
              {
                rerr = macrostr;
                rerrpos = pos;
                return 0;
              }
              pos = pst;
            }
            
            position.push();
            explength.push();
            expression.push();
            exp = macrostr;
            len = exp.length();
            pos = 0; //printf("var %s[%d] = %s\r\n",n.c_str(),ind,exp.c_str());
            inmacros[macrod++]=n;
            continue;
          }
        } //else printf("Unknown var %s\r\n",n.c_str());
        
        //okay, if it didn't actually exist, I lied. We're going to pretend it was just zero, and not continue;.
        setval=0;
        
        //However...
        #if !defined USE_CPP_ID_TREE || USE_CPP_ID_TREE
          externs *tis = immediate_scope;
          for (;;)
          {
            if (find_extname(n,EXTFLAG_STRUCT | EXTFLAG_CLASS | EXTFLAG_NAMESPACE | EXTFLAG_ENUM | EXTFLAG_VALUED))
            {
              if (ext_retriever_var->flags & EXTFLAG_VALUED) {
                setval = ext_retriever_var->value_of;
                break;
              }
              else
              {
                //Skip whitespace and comments
                while (is_useless(exp[pos]) or exp[pos] == '/')
                {
                  if (exp[pos++] == '/') {
                    if (exp[pos] == '/') while (exp[pos] != '\r' and exp[pos] != '\n') pos++;
                    else if (exp[pos] == '*') while (exp[++pos] != '*' and exp[pos+1] != '/');
                  }
                }
                if (exp[pos] == ':' and exp[pos+1] == ':')
                {
                  immediate_scope = ext_retriever_var;
                  pos += 2;
                  while (is_useless(exp[pos]) or exp[pos] == '/')
                  {
                    if (exp[pos++] == '/') {
                      if (exp[pos] == '/') while (exp[pos] != '\r' and exp[pos] != '\n') pos++;
                      else if (exp[pos] == '*') while (exp[++pos] != '*' and exp[pos+1] != '/');
                    }
                  }
                  if (!is_letter(exp[pos]))
                    break;
                  const pt sp = pos;
                  while (is_letterd(exp[++pos]));
                  n = exp.substr(sp, pos-sp);
                }
                else break;
              }
            }
            else break;
          }
          immediate_scope = tis;
        #endif
      }
      else if (exp[pos]=='0')
      {
        //We may have some fucked up number, or just a zero
        pos++;
        if (exp[pos]=='x') //Hexadecimal constant
        {
          pos++;
          int sp=pos;
          while (is_letterd(exp[pos])) pos++;
          string num=exp.substr(sp,pos-sp);
          const int vl=num.length();
          
          for (int i=0;i<vl;i++)
          {
            if (is_digit(num[i]))
            {
              val<<=4;
              val+=num[i]-'0';
            }
            else if (num[i]>='A' and num[i]<='F')
            {
              val<<=4;
              val+=num[i]-'A'+10;
            }
            else if (num[i]>='a' and num[i]<='f')
            {
              val<<=4;
              val+=num[i]-'a'+10;
            }
          }
          setval=val;
        }
        else if (exp[pos]=='b') //Binary constant
        {
          pos++;
          int sp=pos;
          while (is_letterd(exp[pos])) pos++;
          string num=exp.substr(sp,pos-sp);
          const int vl=num.length();
          
          for (int i=0;i<vl;i++)
          {
            if (num[i]=='1') { val<<=1; val++; }
            else if (num[i]=='0') val<<=1;
          }
          setval=val;
        }
        else if (exp[pos]=='.') //Decimal constant
        {
          #if USETYPE_DOUBLE
              pos++;
              int sp=pos;
              while (is_letterd(exp[pos])) pos++;
              string num=exp.substr(sp,pos-sp);
              const int vl=num.length();
              
              double div=1;
              for (int i=0;i<vl;i++)
              {
                if (isdigit(num[i]))
                { dval*=10; dval+=num[i]-'0'; div*=10; }
                if (i>30) break; //past the point of being riddiculous
              }
              dval/=div;
              setval=(UTYPE_DOUBLE)dval;
          #else
            rerr="Float literals not allowed here.";
            rerrpos=pos;
            return 0;
          #endif
        }
        else //Octal constant -- Pretty much the most useless thing ever
        {
          int sp=pos;
          while (is_letterd(exp[pos])) pos++;
          string num=exp.substr(sp,pos-sp);
          const int vl=num.length();
          
          for (int i=0;i<vl;i++)
          {
            if (num[i]>='0' and num[i]<'8')
            { val<<=3; val+=num[i]-'0'; }
          }
          setval=val;
        }
      } //end exp[pos]=='0'
      else if (is_digit(exp[pos]) or exp[pos]=='.') //decimal constant
      {
        int sp=pos;
        while (is_letterd(exp[pos]) || exp[pos]=='.') pos++;
        string num=exp.substr(sp,pos-sp);
        const int vl=num.length();
        
        double div=0;
        int isunsigned=0,istoobig=0;
        int trigger_f=0,trigger_u=0;
        for (int i=0;i<vl;i++)
        {
          if (isdigit(num[i]))
          {
            val*=10;
            val+=num[i]-'0';
            dval*=10;
            dval+=num[i]-'0';
            div*=10;
            if (dval-val > 1000000)
            {
              if (dval-(unsigned long long)val > 1000000)
              istoobig=1;
              else
              isunsigned=1;
            }
          }
          else if (num[i]=='.') div=1;
          else if (num[i]=='f') trigger_f=1;
          else if (num[i]=='F') trigger_f=1;
          else if (num[i]=='u') trigger_u=1;
          else if (num[i]=='U') trigger_u=1;
          //if (i>50) break; //past the point of being riddiculous
        }
        
        if (div>0)
        {
          #if USETYPE_DOUBLE
            setval=(double)val/div;
          #else
            rerr="Float literals not allowed here.";
            rerrpos=pos;
            return 0;
          #endif
        }
        else
        {
          if (trigger_f)
          {
            #if USETYPE_DOUBLE
              setval=(UTYPE_DOUBLE)dval;
            #else
              rerr="Float literals not allowed here";
              rerrpos=pos; return 0;
            #endif
          }
          else if (trigger_u)
          {
            #if USETYPE_UINT
              setval=(UTYPE_UINT)val;
            #else
              rerr="No unsigned types for literal";
              rerrpos=pos; return 0;
            #endif
          }
          else
          {
            if (istoobig)
            {
              rerr="Integer constant too big for unsigned long long";
              rerrpos=pos;
              #if USETYPE_UINT
                if (val<0)
                  setval=(UTYPE_UINT)val;
                else
              #endif
              setval=val;
            }
            else if (isunsigned)
            {
              #if USETYPE_UINT
                setval=(UTYPE_UINT)val;
              #endif
              rerr="Integer constant too big for signed long long";
              rerrpos=pos;
            }
            else setval=val;
          }
        }
      }
      else if (exp[pos]=='"')
      {
        string str;
        #if USETYPE_STRING
          int sp=pos;
          #if STRINGS_IGNORE_BACKSLASH
            pos++; while (pos<len and exp[pos]!='"') pos++;
            str = exp.substr(sp+1,pos-sp-1);
          #else
            pos++; while (pos<len and exp[pos]!='"')
            { if (exp[pos]=='\\' and (exp[pos+1]=='\\' or exp[pos+1]=='"')) pos++; pos++; }
            str = exp.substr(sp+1,pos-sp-1);
          #endif
          setval=str;
        #else
          rerr="String constants not allowed here";
          rerrpos=pos;
          return 0;
        #endif
        pos++;
      }
      else if (exp[pos]=='\'')
      {
        int sp=pos;
        string str;
       #if STRINGS_IGNORE_BACKSLASH
          pos++; while (pos<len and exp[pos]!='\'') pos++;
          str = exp.substr(sp+1,pos-sp-1);
       #else
          pos++; while (pos<len and exp[pos]!='\'')
          { if (exp[pos]=='\\' and (exp[pos+1]=='\\' or exp[pos+1]=='"')) pos++; pos++; }
          str = exp.substr(sp+1,pos-sp-1);
       #endif
       #if SQUOTE_IS_STRING
       
       #else
          val=0;
          size_t vl=str.length();
          for (size_t i=0;i<vl;i++) { val<<=8; val+=str[i]; }
       #endif
        setval=val;
        pos++;
      }
      else if (exp[pos]==')')
      {
        if (!(level>0))
        {
          rerr="Unexpected closing parenthesis";
          rerrpos=pos;
          return 0;
        }
        #if CASTS_ALLOWED
          if (is_cast[level])
          {
            level--; pos++;
            int ccount[UNARY_COUNT];
            for (int i=0;i<UNARY_COUNT;i++) ccount[i]=0;
            for (int i=uc[level]-is_cast[level];i<uc[level];i++)
              ccount[unary[level][i]]++;
            for (int i=0;i<UNARY_COUNT;i++)
            {
              if (ccount[i]>1)
              {
                if (unary[level][i]==UNARY_LONG)
                {
                  for (int ii=uc[level]-is_cast[level];ii<uc[level];ii++)
                  {
                    if (unary[level][ii] == UNARY_LONG)
                      unary[level][ii] = UNARY_LONGLONG;
                  }
                }
                else
                {
                  //cout << "Cast " << unary[level][i] << " appears " << ccount[i] << " times\r\n";
                  rerr="Duplicate cast type";
                  rerrpos=pos;
                  return 0;
                }
              }
            }
            continue;
          }
        #endif
        
        flush_opstack();
        setval=regval[level][0];
        level--; pos++;
      }
      
      electron_transport_chain:
      
      //Apply any immediate unary
      double nvd=0;
      long long nv=0;
      
      #if USETYPE_INT
        #define setitype(x) setval.type=RTYPE_INT; setval.real.i=x;
      #elif USETYPE_UINT
        #define setitype(x) setval.type=RTYPE_UINT; setval.real.u=x;
      #elif USETYPE_DOUBLE
        #define setitype(x) setval.type=RTYPE_DOUBLE; setval.real.d=x;
      #else
        #define setitype(x)
      #endif
      
      #if USETYPE_DOUBLE
        #define setdtype(x) setval.type=RTYPE_DOUBLE; setval.real.d=x;
      #elif USETYPE_INT
        #define setdtype(x) setval.type=RTYPE_INT; setval.real.i=(UTYPE_INT)x;
      #elif USETYPE_UINT
        #define setdtype(x) setval.type=RTYPE_UINT; setval.real.u=(UTYPE_UINT)x;
      #else
        #define setdtype(x)
      #endif
      
      while ((uc[level]--)>0)
      switch (unary[level][uc[level]])
      {
        case UNARY_NONE: break;
        case UNARY_BOOL:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) nv=(setval.real.i != 0);
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) nv=(setval.real.d != 0);
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) nv=(setval.real.u != 0);
            #endif
            setitype(nv);
          break;
        case UNARY_CHAR:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) nv=(char)setval.real.i;
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) nv=(char)setval.real.d;
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) nv=(char)setval.real.u;
            #endif
            setitype(nv);
          break;
        case UNARY_SHORT:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) nv=(short)setval.real.i;
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) nv=(short)setval.real.d;
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) nv=(short)setval.real.u;
            #endif
            setitype(nv);
          break;
        case UNARY_LONG:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) nv=(int)setval.real.i;
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) nv=(int)setval.real.d;
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) nv=(int)setval.real.u;
            #endif
            setitype(nv);
          break;
        case UNARY_LONGLONG:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) nv=(long long)setval.real.i;
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) nv=(long long)setval.real.d;
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) nv=(long long)setval.real.u;
            #endif
            setitype(nv);
          break;
        case UNARY_FLOAT:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) nvd=(float)setval.real.i;
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) nvd=(float)setval.real.d;
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) nvd=(float)setval.real.u;
            #endif
            setdtype(nvd);
          break;
        case UNARY_DOUBLE:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) nvd=(double)setval.real.i;
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) nvd=(double)setval.real.d;
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) nvd=(double)setval.real.u;
            #endif
            setdtype(nvd);
          break;
        case UNARY_SIGNED:
            #if USETYPE_UINT && (USETYPE_INT || USETYPE_DOUBLE)
            if (setval.type==RTYPE_UINT)
            {
              #if USETYPE_INT
                setval.real.i=(UTYPE_INT)setval.real.u;
                setval.type=RTYPE_INT;
              #elif USETYPE_DOUBLE
                setval.real.i=(UTYPE_INT)setval.real.u;
                setval.type=RTYPE_INT;
              #endif
            }
            #endif
          break;
        case UNARY_UNSIGNED:
            #if USETYPE_INT && (USETYPE_INT || USETYPE_DOUBLE)
              if (setval.type==RTYPE_INT)
              {
                #if USETYPE_INT
                  setval.real.u=(UTYPE_UINT)setval.real.u;
                  setval.type=RTYPE_UINT;
                #elif USETYPE_DOUBLE
                  setval.real.U=(UTYPE_UINT)setval.real.u;
                  setval.type=RTYPE_UINT;
                #endif
              }
            #endif
          break;
        case UNARY_MINUS:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) setval.real.i= -setval.real.i;
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) setval.real.d= -setval.real.d;
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) setval.real.u= -setval.real.u;
            #endif
          break;
        case UNARY_NOT:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) setval.real.i= (setval.real.i == 0);
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) setval.real.d= (setval.real.d == 0);
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) setval.real.u= (setval.real.u == 0);
            #endif
          break;
        case UNARY_NEGATE:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) setval.real.i= ~setval.real.i;
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) setval.real.d= ~(int)(setval.real.d);
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) setval.real.u= ~(int)(setval.real.u);
            #endif
          break;
        case UNARY_INCNOW:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) setval.real.i++;
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) setval.real.d++;
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) setval.real.u++;
            #endif
          break;
        case UNARY_DECNOW:
            #if USETYPE_INT
            if (setval.type==RTYPE_INT) setval.real.i--;
            #endif
            #if USETYPE_DOUBLE
            if (setval.type==RTYPE_DOUBLE) setval.real.d--;
            #endif
            #if USETYPE_UINT
            if (setval.type==RTYPE_UINT) setval.real.u--;
            #endif
          break;
      }
      uc[level]=0;
      
      //Now we actually register it
      //cout << "while ("<<opc[level]<<">1)\r\n{ main="<<regval[0][0].real.i<<"\r\n";
      while (opc[level] > 1)
      {/*
        cout << "if (precedence[op[level][opc[level]-1]]>precedence[op[level][opc[level]]])\r\n";
        cout << "if (precedence[op["<<level<<"][opc["<<level<<"]-1]]>precedence[op["<<level<<"][opc["<<level<<"]]])\r\n";
        cout << "if (precedence[op["<<level<<"]["<<opc[level]<<"-1]]>precedence[op["<<level<<"]["<<opc[level]<<"]])\r\n";
        cout << "if (precedence["<<op[level][opc[level]-1]<<"]>precedence["<<op[level][opc[level]]<<"])\r\n";*/
        if (precedence[op[level][opc[level]-2]]<=precedence[op[level][opc[level]-1]])
        {
          //cout << "switch (op["<<level<<"]["<<opc[level]-1<<"])\r\n";
          //cout << "switch ("<<op[level][opc[level]-1]<<")\r\n";
          opc[level]--;
          opstack_pae(); //Pop and Evaluate
          opc[level]++;
          //Shove this operator behind us, then set current to -1
          op[level][opc[level]-1]=op[level][opc[level]];
          op[level][opc[level]]=OP_NONE;
          regval[level][opc[level]].type=RTYPE_NONE;
        } else break;
      }
      //cout << "} main="<<regval[0][0].real.i<<"\r\n";
      //cout << regval[level][opc[level]].type << endl;
      if (regval[level][opc[level]].type == -1)
      {
         regval[level][opc[level]]=setval;
      }
      else
      {
        //cout << "Debug info: " << regval[level][opc[level]].type << " : " << regval[level][opc[level]].real.d << "," << regval[level][opc[level]].str << "\r\n";
        rerr="Expected operator at this point";
        rerrpos=pos;
        return 0;
      }
      continue;
    } //end is_digit(exp[pos])
    
    if (exp[pos]=='(')
    {
      level++;
      VZERO(regval[level][0].); //zero the value
      regval[level][0].type=RTYPE_NONE; //nothing yet
      op[level][0]=OP_NONE; //no operator
      unary[level][0] = 0; //For posterity
      opc[level]=0; //no normal operators
      uc[level]=0; //no unary operators
      
      #if CASTS_ALLOWED
      is_cast[level]=0; //not a cast (or we can't tell yet)
      #endif
      
      pos++;
      continue;
    }
    
    if (exp[pos]=='-')
    {
      if (exp[pos+1]=='-')
      {
        if (regval[level][opc[level]].type==RTYPE_NONE)
          unary[level][uc[level]++]=UNARY_DECNOW;
        //else do nothing; it's too late. Almost as if they planned it.
      }
      else
      {
        if (regval[level][opc[level]].type==RTYPE_NONE)
          unary[level][uc[level]++]=UNARY_MINUS;
        else
        {
          op[level][opc[level]++]=OP_SUB;
          regval[level][opc[level]].type=RTYPE_NONE;
        }
      }
      pos++; continue;
    }
    if (exp[pos]=='+')
    {
      if (exp[pos+1]=='+')
      {
        if (regval[level][opc[level]].type==RTYPE_NONE)
          unary[level][uc[level]++]=UNARY_INCNOW;
        //else do nothing, it's too late. Almost as if they planned it.
      }
      else
      {
        if (regval[level][opc[level]].type==RTYPE_NONE )
          unary[level][uc[level]++]=UNARY_PLUS;
        else
        {
          op[level][opc[level]++]=OP_ADD;
          regval[level][opc[level]].type=RTYPE_NONE;
        }
      }
      pos++; continue;
    }
    if (exp[pos]=='*')
    {
      if (regval[level][opc[level]].type==RTYPE_NONE )
      {
        rerr="Dereferencing something outside program execution...?";
        rerrpos=pos; return 0;
        //unary[level][uc[level]++]=UNARY_DEREF;
      }
      else
      {
        op[level][opc[level]++]=OP_MUL;
        regval[level][opc[level]].type=RTYPE_NONE;
      }
      pos++; continue;
    }
    if (exp[pos]=='/')
    {
      if (exp[pos+1]=='/')
      {
        while (exp[pos]!='\r' and exp[pos]!='\n' and pos<len) pos++;
        continue;
      }
      if (exp[pos+1]=='*')
      {
        pos+=3;
        while (pos<len and exp[pos]!='/' and exp[pos-1]!='*') pos++;
        pos++; continue;
      }
      if (regval[level][opc[level]].type==RTYPE_NONE )
      {
        rerr="Expected primary expression before '/' symbol";
        rerrpos=pos; return 0;
      }
      else
      {
        op[level][opc[level]++]=OP_DIV;
        regval[level][opc[level]].type=RTYPE_NONE;
      }
      pos++; continue;
    }
    if (exp[pos]=='%')
    {
      if (regval[level][opc[level]].type==RTYPE_NONE )
      {
        rerr="Expected primary expression before '%' symbol";
        rerrpos=pos; return 0;
      }
      else
      {
        op[level][opc[level]++]=OP_MOD;
        regval[level][opc[level]].type=RTYPE_NONE;
      }
      pos++; continue;
    }
    if (exp[pos]=='&')
    {
      if (regval[level][opc[level]].type==RTYPE_NONE )
      {
        rerr="Expected primary expression before '&' symbol";
        rerrpos=pos; return 0;
      }
      else
      {
        if (exp[pos+1]=='&')
        {
          op[level][opc[level]++]=OP_BOOLAND;
          pos++;
        }
        else
          op[level][opc[level]++]=OP_AND;
        regval[level][opc[level]].type=RTYPE_NONE;
      }
      pos++; continue;
    }
    if (exp[pos]=='|')
    {
      if (regval[level][opc[level]].type==RTYPE_NONE )
      {
        rerr="Expected primary expression before '|' symbol";
        rerrpos=pos; return 0;
      }
      else
      {
        if (exp[pos+1]=='|')
        {
          op[level][opc[level]++]=OP_BOOLOR;
          pos++;
        }
        else
          op[level][opc[level]++]=OP_OR;
        regval[level][opc[level]].type=RTYPE_NONE;
      }
      pos++; continue;
    }
    if (exp[pos]=='^')
    {
      if (regval[level][opc[level]].type==RTYPE_NONE )
      {
        rerr="Expected primary expression before '^' symbol";
        rerrpos=pos; return 0;
      }
      else
      {
        #if BOOLXOR_ALLOWED
        if (exp[pos+1]=='^')
        {
          op[level][opc[level]++]=OP_BOOLXOR;
          pos++;
        }
        else
        #endif
          op[level][opc[level]++]=OP_XOR;
        regval[level][opc[level]].type=RTYPE_NONE;
      }
      pos++; continue;
    }
    if (exp[pos]=='=')
    {
      pos++;
      if (exp[pos]!='=')
      {
        rerr="Can't assign to that";
        rerrpos=pos; return 0;
      }
      if (regval[level][opc[level]].type==RTYPE_NONE )
      {
        rerr="Expected primary expression before '=' symbol";
        rerrpos=pos-1; return 0;
      }
      else
      {
        op[level][opc[level]++]=OP_EQUAL;
        regval[level][opc[level]].type=RTYPE_NONE;
        pos++;
      }continue;
    }
    if (exp[pos]=='<')
    {
      pos++;
      if (exp[pos]=='<')
      {
        if (regval[level][opc[level]].type==RTYPE_NONE )
        {
          rerr="Expected primary expression before '<<' token";
          rerrpos=pos; return 0;
        }
        else
        {
          op[level][opc[level]++]=OP_LSH;
          regval[level][opc[level]].type=RTYPE_NONE;
        }
        pos++; continue;
      }
      if (exp[pos]=='=')
      {
        if (regval[level][opc[level]].type==RTYPE_NONE )
        {
          rerr="Expected primary expression before '<=' token";
          rerrpos=pos; return 0;
        }
        else
        {
          op[level][opc[level]++]=OP_LESSOREQ;
          regval[level][opc[level]].type=RTYPE_NONE;
        }
        pos++; continue;
      }
      if (regval[level][opc[level]].type==RTYPE_NONE )
      {
        rerr="Expected primary expression before '<' token";
        rerrpos=pos; return 0;
      }
      else
      {
        op[level][opc[level]++]=OP_LESS;
        regval[level][opc[level]].type=RTYPE_NONE;
      }
      continue;
    }
    if (exp[pos]=='>')
    {
      pos++;
      if (exp[pos]=='>')
      {
        if (regval[level][opc[level]].type==RTYPE_NONE )
        {
          rerr="Expected primary expression before '>>' token";
          rerrpos=pos; return 0;
        }
        else
        {
          op[level][opc[level]++]=OP_RSH;
          regval[level][opc[level]].type=RTYPE_NONE;
        }
        pos++; continue;
      }
      if (exp[pos]=='=')
      {
        if (regval[level][opc[level]].type==RTYPE_NONE )
        {
          rerr="Expected primary expression before '>=' token";
          rerrpos=pos; return 0;
        }
        else
        {
          op[level][opc[level]++]=OP_MOREOREQ;
          regval[level][opc[level]].type=RTYPE_NONE;
        }
        pos++; continue;
      }
      if (regval[level][opc[level]].type==RTYPE_NONE )
      {
        rerr="Expected primary expression before '>' token";
        rerrpos=pos; return 0;
      }
      else
      {
        op[level][opc[level]++]=OP_MORE;
        regval[level][opc[level]].type=RTYPE_NONE;
      }
      continue;
    }

    if (exp[pos]=='!')
    {
      if (exp[pos+1]=='=')
      {
        if (regval[level][opc[level]].type==RTYPE_NONE)
        {
          rerr="Expected primary expression before '!=' token";
          rerrpos=pos; return 0;
        }
        else
        {
          op[level][opc[level]++]=OP_NEQUAL;
          regval[level][opc[level]].type=RTYPE_NONE;
          pos++;
        }
      }
      else
      {
        if (regval[level][opc[level]].type!=RTYPE_NONE)
        {
          rerr="Operator expected before unary '!'";
          rerrpos=pos; return 0;
        }
        unary[level][uc[level]++]=UNARY_NOT;
      }
      pos++; continue;
    }
    if (exp[pos]=='~')
    {
      if (regval[level][opc[level]].type!=RTYPE_NONE)
      {
        rerr="Operator expected before unary '~'";
        rerrpos=pos; return 0;
      }
      unary[level][uc[level]++]=UNARY_NEGATE;
      pos++; continue;
    }
    
    if (exp[pos]=='?')
    {
      int a=1,b=0;
      //cout << regval[level][0].real.d;
      flush_opstack();
      //cout << regval[level][0].real.d;
      if (
        #if USETYPE_INT
          (regval[level][0].type==RTYPE_INT    and regval[level][0].real.i == 0) or
        #endif
        #if USETYPE_DOUBLE
          (regval[level][0].type==RTYPE_DOUBLE and regval[level][0].real.d == 0) or
        #endif
        #if USETYPE_UINT
          (regval[level][0].type==RTYPE_UINT   and regval[level][0].real.u == 0) or
        #endif
          0)
      while (pos<len)
      {
        pos++;
        if (b==0)
        {
          if (exp[pos]==':') a--;
          else if (exp[pos]=='?') a++;
          if (a==0) break;
        }
        if (exp[pos]=='(') b++;
        else if (exp[pos]==')') b--;
        else if (exp[pos]=='/')
        {
          if (exp[pos+1]=='/')
          while (exp[pos]!='\r' and exp[pos]!='\r' and pos<len) pos++;
          else if (exp[pos+1]=='*')
          { pos+=3; while (pos<len and exp[pos]!='/' and exp[pos-1]!='*') pos++; }
        }
        else if (exp[pos+1]=='"')
        {
         #if USETYPE_STRING
           #if STRINGS_IGNORE_BACKSLASH
              pos++; while (pos<len and exp[pos]!='"') pos++;
           #else
              pos++; while (pos<len and exp[pos]!='"')
              { if (exp[pos]=='\\' and (exp[pos+1]=='\\' or exp[pos+1]=='"')) pos++; pos++; }
           #endif
         #else
            rerr="String constants not allowed here";
            rerrpos=pos;
            return 0;
         #endif
        }
        else if (exp[pos+1]=='\'')
        {
         #if STRINTS_IGNORE_BACKSLASH
            pos++; while (pos<len and exp[pos]!='\'') pos++;
         #else
            pos++; while (pos<len and exp[pos]!='\'')
            { if (exp[pos]=='\\' and (exp[pos+1]=='\\' or exp[pos+1]=='"')) pos++; pos++; }
         #endif
        }
      }
      regval[level][0].type=RTYPE_NONE;
      pos++; continue;
    }
    
    if (exp[pos]==':')
    {
      int a=0;
      while (a>=0 and pos<len)
      {
        pos++;
        if (exp[pos]=='(') a++;
        else if (exp[pos]==')') a--;
        else if (exp[pos]=='/')
        {
          if (exp[pos+1]=='/')
          while (exp[pos]!='\r' and exp[pos]!='\r' and pos<len) pos++;
          else if (exp[pos+1]=='*')
          { pos+=3; while (pos<len and exp[pos]!='/' and exp[pos-1]!='*') pos++; }
        }
        else if (exp[pos+1]=='"')
        {
         #if USETYPE_STRING
           #if STRINGS_IGNORE_BACKSLASH
              pos++; while (pos<len and exp[pos]!='"') pos++;
           #else
              pos++; while (pos<len and exp[pos]!='"')
              { if (exp[pos]=='\\' and (exp[pos+1]=='\\' or exp[pos+1]=='"')) pos++; pos++; }
           #endif
         #else
            rerr="String constants not allowed here";
            rerrpos=pos;
            return 0;
         #endif
        }
        else if (exp[pos+1]=='\'')
        {
         #if STRINGS_IGNORE_BACKSLASH
            pos++; while (pos<len and exp[pos]!='\'') pos++;
         #else
            pos++; while (pos<len and exp[pos]!='\'')
            { if (exp[pos]=='\\' and (exp[pos+1]=='\\' or exp[pos+1]=='"')) pos++; pos++; }
         #endif
        }
      }
      continue;
    }
    
    if (exp[pos] == '\\')
    {
      if (exp[pos+1] == '\r' or exp[pos+1]=='\n')
        { pos++; continue; }
    }
    
    rerr = string("Unrecognized symbol '") + exp[pos] + "' in [ " + exp + " ] at " + tostring(pos);
    rerrpos = pos; //cout << rerr;
    return 0;
  } //end of expression
  
  if (level>0)
  {
    rerr="Unterminated paretheses before end of expression";
    rerrpos=pos;
  }
  
  flush_opstack();
  
  return regval[0][0];
}
