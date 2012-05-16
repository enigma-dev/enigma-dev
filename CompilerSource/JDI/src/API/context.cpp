/**
 * @file  context.cpp
 * @brief Source implementing methods for creating contexts of parsed code.
 * 
 * In general, the implementation is unremarkable. See the header documentation
 * for details on behavior and usage.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <string>
#include <fstream>
#include <cstring>
using namespace std;

#include "context.h"
#include <System/macros.h>
#include <System/builtins.h>

using namespace jdi;
using namespace jdip;

void context::read_macros(const char* filename)
{
  ifstream in(filename);
  if (!in.is_open())
    return;
  // TODO: IMPLEMENT
  in.close();
}
void context::add_macro(string definiendum, string definiens) {
  macros[definiendum] = (macro_type*)new macro_scalar(definiendum, definiens);
}
void context::add_macro_func(string definiendum, string definiens) {
  macros[definiendum] = (macro_type*)new macro_function(definiens);
}
void context::add_macro_func(string definiendum, string p1, string definiens, bool variadic)
{
  vector<string> arglist;
  arglist.push_back(p1);
  macros[definiendum] = (macro_type*)new macro_function(definiendum, arglist, definiens, variadic);
}
void context::add_macro_func(string definiendum, string p1, string p2, string definiens, bool variadic)
{
  vector<string> arglist;
  arglist.push_back(p1);
  arglist.push_back(p2);
  macros[definiendum] = (macro_type*)new macro_function(definiendum, arglist, definiens, variadic);
}
void context::add_macro_func(string definiendum, string p1, string p2, string p3, string definiens, bool variadic)
{
  vector<string> arglist;
  arglist.push_back(p1);
  arglist.push_back(p2);
  arglist.push_back(p3);
  macros[definiendum] = (macro_type*)new macro_function(definiendum, arglist, definiens, variadic);
}

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

void context::read_search_directories(const char* filename) {
  read_search_directories_gnu(filename,NULL,NULL);
}
void context::read_search_directories_gnu(const char* filename, const char* begin_line, const char* end_line)
{
  ifstream in(filename);
  if (!in.is_open()) return;
  
  const size_t ln = strlen(filename) + MAX_PATH;
  const size_t bll = begin_line ? strlen(begin_line) : size_t(-1), ell = end_line ? strlen(end_line) : size_t(-1);
  char *sdir = new char[ln];
  
  while (!in.eof())
  {
    *sdir = 0;
    in.getline(sdir,ln);
    const size_t sl = strlen(sdir);
    if (sl < bll)
      continue;
    if (strncmp(sdir + sl - bll, begin_line, bll) == 0)
      break;
  }
  while (!in.eof())
  {
    *sdir = 0;
    in.getline(sdir,ln);
    string dir = sdir;
    if (dir.length() >= ell)
      if (strncmp(sdir, end_line, ell) == 0)
        break;
    add_search_directory(dir);
  }
  
  delete[] sdir;
  in.close();
}
void context::add_search_directory(string dir)
{
  search_directories.push_back(dir);
}

void context::reset()
{
  
}
void context::reset_all()
{
  
}
void context::copy(const context &ct)
{
  ct.global->copy(global);
  for (macro_iter_c mi = ct.macros.begin(); mi != ct.macros.end(); ++mi){
    pair<macro_iter,bool> dest = macros.insert(pair<string,macro_type*>(mi->first,NULL));
    if (dest.second) {
      dest.first->second = mi->second;
      ++mi->second->refc;
    }
  }
}

string context::get_last_error() {
  return error;
}

void context::load_standard_builtins()
{
  // Nothing to load
}
void context::load_gnu_builtins()
{
  
}

#include <iostream>
#include <General/parse_basics.h>

void context::output_types(ostream &out) {
  out << "Unimplemented";
}
static inline void print_macro(macro_iter it, ostream &out = cout)
{
  if (it->second->argc >= 0)
  {
    macro_function *mf = (macro_function*)it->second;
    out << "#define " << it->first << "(";
    for (size_t i = 0; i < mf->args.size(); i++)
      out << mf->args[i] << (i+1 < mf->args.size() ? ", " : ((size_t)it->second->argc > mf->args.size()? "...": ""));
    out << ") \\" << endl;
    for (size_t i = 0; i < mf->value.size(); i++)
      out << "  " << mf->value[i].toString(mf) << (i+1 < mf->value.size()? "\\" : "") << endl;
  }
  else {
    out << "#define " << it->first << endl << "  " << ((macro_scalar*)it->second)->value << endl;
  }
}
void context::output_macro(string macroname, ostream &out)
{
  macro_iter it = macros.find(macroname);
  if (it == macros.end()) out << "Macro `" << macroname << "' has not been defined." << endl;
  else print_macro(it, out);
}
void context::output_macros(ostream &out) 
{
  for (macro_iter it = macros.begin(); it != macros.end(); it++)
    print_macro(it, out);
}

static string fulltype_string(full_type& ft);

static string typeflags_string(definition *type, unsigned flags) {
  string res;
  for (int i = 1; i <= 0x10000; i <<= 1)
    if (flags & i) {
      jdip::tf_flag_map::iterator tfi = builtin_decls_byflag.find(i);
      if (tfi == builtin_decls_byflag.end()) res += "<ERROR:NOSUCHFLAG:" + toString(i) + "> ";
      else res += tfi->second->name + " ";
    }
  if (type)
    res += type->name;
  else res += "<NULL>";
  return res;
}

static string type_referencers_string_prefix(ref_stack& rf) {
  string res;
  ref_stack::iterator it = rf.begin();
  while (it)
  {
    while (it and (it->type == ref_stack::RT_ARRAYBOUND || it->type == ref_stack::RT_FUNCTION)) ++it;
    while (it and (it->type == ref_stack::RT_POINTERTO || it->type == ref_stack::RT_REFERENCE))
      res = (it->type == ref_stack::RT_POINTERTO? '*' : '&') + res, ++it;
    if (it) res = '(' + res;
  }
  return res;
}

static string arraybound_string(size_t b) {
  if (b == ref_stack::node_array::nbound)
    return "[]";
  char buf[32]; sprintf(buf,"[%lu]",(long unsigned)b);
  return buf;
}

static string type_referencers_string_postfix(ref_stack& rf) {
  string res;
  ref_stack::iterator it = rf.begin();
  while (it)
  {
    while (it and (it->type == ref_stack::RT_ARRAYBOUND || it->type == ref_stack::RT_FUNCTION)) {
      if (it->type == ref_stack::RT_ARRAYBOUND) res += arraybound_string(it->arraysize());
      else {
        res += '(';
        ref_stack::node_func* nf = (ref_stack::node_func*)*it;
        for (size_t i = 0; i < nf->params.size(); i++) {
          res += fulltype_string(nf->params[i]);
          if (i + 1 < nf->params.size()) res += ", ";
        }
        res += ')';
      }
      ++it;
    }
    while (it and (it->type == ref_stack::RT_POINTERTO || it->type == ref_stack::RT_REFERENCE)) ++it;
    if (it) res += ')';
  }
  return res;
}

static string fulltype_string(full_type& ft) {
  string res = typeflags_string(ft.def, ft.flags);
  string app = type_referencers_string_prefix(ft.refs) + ft.refs.name + type_referencers_string_postfix(ft.refs);
  if (app.length()) res += " " + app;
  return res;
}

static void utility_printtype(definition_typed* t, ostream &out) {
  out << typeflags_string(t->type, t->modifiers) << " ";
  out << type_referencers_string_prefix(t->referencers);
  out << t->name;
  out << type_referencers_string_postfix(t->referencers);
}

static void utility_printrc(definition_scope* scope, ostream &out, string indent) {
  for (map<string,definition*>::iterator it = scope->members.begin(); it != scope->members.end(); it++)
  {
    out << indent;
    if (it->second->flags & DEF_TYPED)
    {
      if (it->second->flags & DEF_TYPENAME)
        out << "typedef ";
      utility_printtype((definition_typed*)it->second, out);
      out << ";" << endl;
    }
    else if (it->second->flags & DEF_NAMESPACE)
    {
      out << indent << "namespace " << it->second->name << " {" << endl;
      utility_printrc((definition_scope*)it->second, out, indent + "  ");
      out << indent << "}" << endl;
    }
    else if (it->second->flags & DEF_CLASS)
    {
      out << "class " << it->second->name;
      definition_class *dc = (definition_class*)it->second;
      if (dc->ancestors.size()) {
        out << ": ";
        for (size_t i = 0; i < dc->ancestors.size(); i++)
          out << (dc->ancestors[i].protection == DEF_PRIVATE? "private "
                 :dc->ancestors[i].protection == DEF_PROTECTED? "protected "
                 :"public ") << dc->ancestors[i].def->name
              << (i+1 < dc->ancestors.size() ? ", " : "");
      }
      out << " {" << endl;
      utility_printrc((definition_scope*)it->second, out, indent + "  ");
      out << indent << "}" << endl;
    }
  }
}
void context::output_definitions(ostream &out) {
  utility_printrc(global, out, "");
}

context::context(): parse_open(false), lex(NULL), herr(def_error_handler), global(new definition_scope()) {
  copy(builtin);
}

const macro_map& context::get_macros() { return macros; }

context::context(int): parse_open(false), lex(NULL), herr(def_error_handler), global(new definition_scope()) { }

size_t context::search_dir_count() { return search_directories.size(); }
string context::search_dir(size_t index) { return search_directories[index]; }

void context::dump_macros() {
  // Clean up macros
  for (macro_iter it = macros.begin(); it != macros.end(); it++)
    macro_type::free(it->second);
}

context::~context() {
  delete global;
  delete lex;
  dump_macros();
}
