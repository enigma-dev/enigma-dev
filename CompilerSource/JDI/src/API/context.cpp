/**
 * @file  context.cpp
 * @brief Source implementing methods for creating contexts of parsed code.
 * 
 * In general, the implementation is unremarkable. See the header documentation
 * for details on behavior and usage.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2012 Josh Ventura
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

static definition* find_mirror(definition *x, definition_scope* root) {
  if (x) {
    definition_scope *n = (definition_scope*)find_mirror(x->parent, root);
    return n? n->look_up(x->name) : n;
  }
  return root;
}

void context::reset()
{
  
}
void context::reset_all()
{
  
}
void context::copy(const context &ct)
{
  remap_set n;
  ct.global->copy(global, n);
  ct.global->remap(n, error_context(def_error_handler, "Internal Copy Operation", 0, 0));
  
  for (macro_iter_c mi = ct.macros.begin(); mi != ct.macros.end(); ++mi){
    pair<macro_iter,bool> dest = macros.insert(pair<string,macro_type*>(mi->first,NULL));
    if (dest.second) {
      dest.first->second = mi->second;
      ++mi->second->refc;
    }
  }
  for (set<definition*>::iterator it = ct.variadics.begin(); it != ct.variadics.end(); ++it) {
    if ((*it)->parent)
      variadics.insert(find_mirror(*it, global));
    else
      variadics.insert(*it);
  }
}
void context::swap(context &ct) {
  if (!parse_open and !ct.parse_open) {
    { definition_scope* gs = ct.global;
      ct.global = global; global = gs; }
    macros.swap(ct.macros);
    variadics.swap(ct.variadics);
  }
  else cerr << "ERROR! Cannot swap context while parse is active" << endl;
}

macro_map &context::global_macros() {
  return builtin->macros;
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
static inline void print_macro(macro_iter it, ostream &out)
{
  out << it->second->toString();
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

void context::output_definitions(ostream &out) {
  out << global->toString();
}

context::context(): parse_open(false), global(new definition_scope()) {
  copy(*builtin);
}
context::context(int): parse_open(false), global(new definition_scope()) { }

size_t context::search_dir_count() { return search_directories.size(); }
string context::search_dir(size_t index) { return search_directories[index]; }

void context::dump_macros() {
  // Clean up macros
  for (macro_iter it = macros.begin(); it != macros.end(); it++)
    macro_type::free(it->second);
}

context::~context() {
  delete global;
  dump_macros();
}
