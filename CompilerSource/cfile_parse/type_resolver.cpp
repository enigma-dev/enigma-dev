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
#include <stack>
#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;

#include "type_resolver.h"

#include "general/parse_basics.h"
#include "externs/externs.h"

#ifdef cpp
  #define and_not_accessor()
  #define default_var(x) return NULL
#else
  #define and_not_accessor() and !is_letter(exp[pos+1])
  #define get_var_fordef() externs *retrievable_var = (find_extname("var",EXTFLAG_TYPENAME), ext_retriever_var);
  #define default_var(x) x = retrievable_var
#endif

enum {
  ot_binary,
  ot_unary_pre,
  ot_unary_post
};

onode::onode(): op(), type(builtin_type__void), otype(0), prec(0), pad(0), deref(NULL) {};
onode::onode(externs* t): op(), type(t), otype(0), prec(0), pad(0), deref(t?t->refstack.last:NULL) {};
onode::onode(string oN,unsigned oT,short pr,externs *e): op(oN), type(e), otype(oT), prec(pr), pad(0), deref(e ? e->refstack.last : 0) {};

void onode::operator <= (const onode &t) { type = t.type; prec = t.prec; pad = t.pad; deref = t.deref; }

enum {
  of_binary = 1,
  of_unary_pre = 2,
  of_unary_post = 4,
  of_unary = of_unary_pre | of_unary_post
};

struct opinfo {
  unsigned short prec, flags;
  opinfo(): prec(0), flags(0) {}
  opinfo(unsigned short f,unsigned short p): prec(p), flags(f) {}
};

map<string,opinfo> ops;
typedef map<string,opinfo>::iterator opiter;

void exp_typeof_init()
{
  // This will cover most of the work, but operators ::, ->, and . need manual handling
  ops[")"] = ops["]"] = opinfo(of_unary_post, 0); // This is executed immediately
  ops["("] = ops["["] = opinfo(of_binary, 0);  // This is executed after everything following it is closed; nothing else is low enough to give way to it
  
  ops["++"] = ops["--"] = opinfo(of_unary, 20);
  ops["!"] = ops["~"] = ops["compl"] = ops["sizeof"] = ops["new"] = ops["delete"] = ops["cast"] = opinfo(of_unary_pre, 19);
  ops["*"] = opinfo(of_binary | of_unary_pre, 18);
  ops["/"] = ops["%"] = opinfo(of_binary, 18);
  ops["+"] = ops["-"] = opinfo(of_binary | of_unary_pre, 17);
  ops["<<"] = ops[">>"] = opinfo(of_binary, 16);
  ops["<"] = ops["<="] = ops[">"] = ops[">="] = opinfo(of_binary, 15);
  ops["=="] = ops["!="] = opinfo(of_binary, 14);
  ops["&"] = opinfo(of_binary | of_unary_pre, 13);
  ops["^"] = opinfo(of_binary, 12);
  ops["|"] = opinfo(of_binary, 11);
  ops["&&"] = opinfo(of_binary, 10);
  ops["^^"] = opinfo(of_binary, 9);
  ops["||"] = opinfo(of_binary, 8);
  ops["?"] = ops[":"] = opinfo(of_binary, 7);
  ops["="] = ops["+="] = ops["-="] = ops["*="] = ops["/="] = ops["%="] = ops["&="] = ops["^="] = ops["|="] = ops["<<="] = ops[">>="] = opinfo(of_binary, 6);
  ops["throw"] = opinfo(of_binary, 5);
  ops[","] = opinfo(of_binary, 4);
}

externs *type_lookup_op_result(externs* t1, string op, externs* t2 = NULL)
{
  externs *t = current_scope; current_scope = t1;
  const bool f = find_extname("operator" + op, 0xFFFFFFFF);
  current_scope = t;
  if (!f)
    return NULL;
  externs *rt = ext_retriever_var->type;
  if (rt->parent == t1) // t1's type is a member of t1: must be a template
    return ext_retriever_var->type;
  return ext_retriever_var->type;
}

inline onode type_op_resolve(const onode &t1, const onode &t2, bool &isol) // Evaluates `t1 t2.op t2` for type
{
  string op = t2.op;
  if (t2.prec) // If the operator associated with t1 has any precedence
  {
    if (t2.otype == ot_binary) // Binary operator
    {
      if (t1.pad or t1.deref)
        if (t2.pad or t2.deref)
          return onode(t1);
      externs *n = type_lookup_op_result(t1.type,op,t2.type);
      return n ? onode(n) : onode(t1);
    }
    else if (t2.otype != ot_unary_pre) {
      puts("Shouldn't be sent here");
      return onode();
    }
    else // Unary prefix operator
    {
      if (op == "*")
      {
        if (t2.pad) {
          onode res = t2;
          return (res.pad--, res);
        }
        if (t2.deref) {
          onode res = t2;
          return (res.deref = res.deref->prev, res);
        }
        return onode(type_lookup_op_result(t2.type,"*",builtin_type__void));
      }
      else if (op == "&") {
        onode res = t2;
        return (res.pad++, res);
      }
    }
  }
  else // This is an opening parenthesis or bracket
  {
    //This could either be dereferencing a function/array, or it could just be a regular parenthetical expression
    if (!t1.type) // It's free-standing, so it had better be a parenthesis
    {
      if (op == "(")
        return onode(t2);
      return onode();
    }
    else //We have something[somethingelse] or something(somethingelse)
    {
      if (op == "(") {
        onode res = t1;
        if (res.deref && res.deref->ref.symbol == '(')
          return type_lookup_op_result(t1.type,"()",t2.type);
        if (res.deref)
          res.deref = res.deref->prev;
        return res;
      }
      else if (op == "[") {
        onode res = t1;
        if (res.pad)
          return (res.pad--, res);
        if (res.deref)
          return (res.deref = res.deref->prev, res);
        externs *rt = type_lookup_op_result(t1.type,"[]",t2.type);
        return rt? rt: t1;
      }
    }
  }
  return onode();
}


onode type_op_resolve_upost(const onode &t, string op) // Evaluates `t1 op`, where op is unary postfix
{
  return onode(t);
}

void onode_from_dectrip(onode& x, const dectrip& y)
{
  string tts = y.type;
  
  if (tts == "")
    tts = "var";
  else
  {
    int tbc = 0; // Triangle bracket count
    pt pos = y.type.length(); // Iteration position, as always
    while (pos > 0 and (tbc or !is_letterd(y.type[pos]))) // Skip to the last type name on the list, which will be before any <>s
    {
      if (tts[pos] == '>') tbc++;
      else if (tts[pos] == '<') tbc--;
      pos--;
    }
    
    const pt epos = pos;
    while (pos > 0 and is_letterd(y.type[--pos]));
    if (!is_letter(y.type[pos])) pos++;
    tts = tts.substr(pos,epos-pos+1);
  }
  
  if (!find_extname(tts, EXTFLAG_TYPENAME))
    return (printf("Error: type not a type, derp\n"), void(0));
  
  x.type = ext_retriever_var;
  x.pad = y.prefix.length();
}

extern externs *enigma_type__var;
struct bullshit{bullshit(){cout<<"enter"<<endl;}~bullshit(){cout<<"LEAVE"<<endl;}};
onode exp_typeof(string exp, map<string,dectrip>** lvars, int lvarc, parsed_object* globj, parsed_object* pobj)
{
  bullshit IcallIt;
  pt pos = 0;
  const pt len = exp.length();
  current_scope = &global_scope;
  
  get_var_fordef();
  stack<onode> perf; perf.push(onode());
  
  bool isol = true;
  
  while (pos < len)
  {
    if (is_useless(exp[pos])) {
      while (is_useless(exp[++pos]));
      continue;
    }
    if (is_digit(exp[pos]))
    {
      externs *rt = builtin_type__int;
      while (is_digit(exp[++pos]) or (exp[pos] == '.' and_not_accessor() and (rt = builtin_type__float)));
      perf.top().type = rt;
      isol = false;
      continue;
    }
    if (is_letter(exp[pos]))
    {
      const pt spos = pos;
      while (is_letterd(exp[++pos]));
      string tn = exp.substr(spos,pos-spos);
      
      if (tn.length() > 10 and tn.substr(0,10) == "varaccess_" // If we're looking at ::enigma::varaccess_*
      and immediate_scope and immediate_scope->name == "enigma" and immediate_scope->parent == &global_scope)
        cout << "***   ***  RESEARCH FOR `" << (tn = tn.substr(10)) << "`" << endl, // This gets a special case. One of the very few modifications can break if pissed with from the game.
        immediate_scope = NULL; // We found what we were looking for; reset the immediate scope.
      
      bool nt = false;
      map<string,dectrip>::iterator it;
      for (int i = 0; i < lvarc; i++)
        if ((it = lvars[i]->find(tn)) != lvars[i]->end()) {
          onode_from_dectrip(perf.top(), it->second);
          isol = false, nt = true; break;
        }
      
      if (!nt and globj)
      {
        cout << ">> Searching for `" << tn << "` in globj->locals (" << globj->locals.size() << " entries)" << endl; cout << "{ ";
        for (it = globj->locals.begin(); it != globj->locals.end(); it++) cout << it->first << ", "; cout << "end }" << endl;
        it = globj->locals.find(tn);
        if (it != globj->locals.end()) {
          onode_from_dectrip(perf.top(), it->second);
          isol = false, nt = true;
          cout << "Found it! Added " << perf.top().type->name << endl;
        }
        
        it = globj->globals.find(tn);
        if (it != globj->globals.end()) {
          onode_from_dectrip(perf.top(), it->second);
          isol = false, nt = true;
        }
        cout << ">>>" << (nt?"Found it" : "No dice") << endl;
      }
      
      if (!nt and pobj)
      {
        cout << ">> Searching for `" << tn << "` in pobj->locals (" << pobj->locals.size() << " entries)" << endl;
        it = pobj->locals.find(tn);
        if (it != pobj->locals.end()) {
          onode_from_dectrip(perf.top(), it->second);
          isol = false, nt = true;
        }
        cout << ">>>" << (nt?"Found it" : "No dice") << endl;
      }
      
      
      if (nt)
        continue;
      
      cout << ">> Searching for `" << tn << "` everywhere" << endl;
      
      nt = find_extname(tn,0xFFFFFFFF);
      externs* t = ext_retriever_var;
      
      string oname = ""; // We'll push an operator-less type to the stack
      
      if (!nt) default_var(t); // Not a valid type; default to var if legal, or return error otherwise
      else if (t->flags & EXTFLAG_TYPENAME) { // Turns out, this is not a global, but a typename.
        if (!isol) { cout<<"NULL"<<endl; return NULL; } // Must be a prefix;
        oname = "cast";        //  This is a cast.
      }
      else if (!t->type) // It's a global. Does it have an valid type?
      {
        if (!(t->flags & EXTFLAG_NAMESPACE)) // Heh, guess not. Bet it's a namespace.
          { cout<<"NULL2"<<endl; return NULL; } // If not, return NULL.
        immediate_scope = t;
        cout << "FOUND AND INSCOPED" << endl;
        continue;
      }
      else
        t = t->type,
        perf.top().pad = 0,
        perf.top().deref = refstack.last;
      
      perf.top().type = t;
      
      cout << "ACCEPTED" << endl;
      isol = false;
      continue;
    }
    
    string findme;
    if (exp[pos] == exp[pos+1])
      if (exp[pos+2] == '=')
        findme = exp.substr(pos,3);
      else findme = exp.substr(pos,2);
    else if (exp[pos+1] == '=')
      findme = exp.substr(pos,2);
    else findme = exp.substr(pos,1);
    
    if (findme == "::") {
      pos += 2; continue;
    }
    
    if (findme == "." or (findme == "-" and exp[pos+1] == '>'))
    {
      cout << "DOT LOL" << endl;
      pos += 1 + (exp[pos+1]=='>');
      const pt spos = pos;
      if (!is_letter(exp[pos]))
        return NULL;
      while (is_letterd(exp[++pos]));
      string member = exp.substr(spos,pos-spos);
      
      externs *ct = perf.top().type, *nt = NULL; extiter qit;
      if (!ct) { //-> without a type before it
        perf.top().type = enigma_type__var;
        continue;
      }
      
      bool tf = ((qit = ct->members.find(member)) != ct->members.end());
      if (tf) nt = qit->second;
        
      while (!tf and ct->flags & EXTFLAG_TYPEDEF and ct->type) {
        ct = ct->type, tf = ((qit = ct->members.find(member)) != ct->members.end());
        if (tf) nt = qit->second;
      }
      if (!tf and find_in_all_ancestors_generic(ct,member))
        ct = ext_retriever_var->parent, nt = ext_retriever_var, tf = true;
      
      if (!tf)
        return NULL;
      
      perf.top().type = nt->type;
      perf.top().pad  = 0; // FIXME: FIX ME
      
      continue;
    }
    
    opiter it;
    it = ops.find(findme);
    if (findme.length() > 1 and it == ops.end())
      it = ops.find(findme.erase(1));
    
    if (it != ops.end())
    {
      pos += it->first.length();
      
      // Resolve the kind of this operator: unary or binary? prefix or postfix?
      unsigned pflags = ot_unary_pre;
      if (!isol)
      {
        if (it->second.flags & of_binary)
          pflags = ot_binary;
        else if (it->second.flags & of_unary_post)
          pflags = ot_unary_post;
        else return NULL;
      }
      else if (!it->second.flags & of_unary_pre)
        return NULL;
      isol = true;
      
      // Prefix unary is granted a super-high precedence
      const unsigned pprec = pflags == ot_binary ? it->second.prec : 100;
      
      // But if it's a postfix unary, it automatically gets immediate precedence
      if (pflags == ot_unary_post)
      {
         if (it->first != "]" and it->first != ")") // Typical unary postfix
           perf.top() = type_op_resolve_upost(perf.top(),it->first);
         else // Closing parenthesis of some sort
         {
          while (perf.top().prec) // Pop these until we find matching parenthesis
          {
            onode opand1 = perf.top(); perf.pop();
            onode ee = type_op_resolve(opand1, perf.top(), isol);
            if (!ee.type) { 
              puts("SingleResultNull"); return NULL;
            }
          }
          
          // Evaluate our array subscript/function/whatever.
          if (perf.size() > 1) {
            onode opand2 = perf.top(); perf.pop();
            onode ee = type_op_resolve(perf.top(), opand2, isol);
            if (!ee.type) { puts("SingleResultNull"); return NULL; }
            perf.top() <= ee;
          }
          else printf("ERROR! Stack not large enough to accomodate matching parenthesis. Should be more than one item on stack.\nExpression: %s", exp.c_str());
         }
      }
      else // Not a postfix unary: either binary or prefix unary
      if (perf.top().type) // Well, there needs to be something at the top ready to be operated on.
      {
        // Traverse the stack for anything of higher precedence to take care of first.
        if (pprec) // But not for parentheses
          while (perf.top().prec > pprec)
          {
            onode opand2 = perf.top(); perf.pop();
            onode ee = type_op_resolve(perf.top(), opand2, isol);
            if (!ee.type) { puts("SingleResultNull"); return NULL; }
            perf.top() <= ee;
          }
        
        // Push an operator with the current string and classification with the given precedence
        perf.push(onode(it->first,pflags,pprec,NULL)); // But leave the type section blank.
      }
      else
        cout << "Typeless" << endl;
      continue;
    }
    pos++;
  }
  while (perf.size() > 1)
  {
    onode opand2 = perf.top(); perf.pop();
    onode ee = type_op_resolve(perf.top(), opand2, isol);
    if (!ee.type) { puts("SingleResultNull"); return NULL; }
    perf.top() <= ee;
  }
  return perf.top();
}

externs* ext_simplify(externs* e)
{
  externs* res = e;
  while (res->flags & EXTFLAG_TYPEDEF and res->type)
    res = res->type;
  return res;
}

string externs_name(onode e)
{
  if (!e.type)
    return "NULL";
  string res = e.type->name;
  if (e.type->tempargs.size)
  {
    res += "<";
    for (unsigned i = 0; i < e.type->tempargs.size; i++)
      res += e.type->tempargs[i]->type == NULL ? "NULL," : e.type->tempargs[i]->type->name + ",";
    res += ">";
  }
  return res;
}
