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
#include <System/lex_cpp.h>

using namespace jdi;

#define new_macro std::make_shared<const macro_type>

void Context::read_macros(const char* filename)
{
  ifstream in(filename);
  if (!in.is_open())
    return;
  // TODO: IMPLEMENT
  in.close();
}
inline vector<token_t> parse_macro(const string &definiendum, const string &definiens, ErrorHandler *herr) {
  llreader str_reader(definiendum, definiens, false);
  vector<token_t> tokens;
  for (token_t t;
      (t = read_token(str_reader, herr)).type != TT_ENDOFCODE; )
    tokens.push_back(t);
  return tokens;
}
void Context::add_macro(string definiendum, string definiens) {
  macros[definiendum] = new_macro(definiendum,
                                  parse_macro(definiendum, definiens, herr),
                                  herr);
}
void Context::add_macro_func(string definiendum, string definiens) {
  macros[definiendum] = new_macro(definiendum,
                                  parse_macro(definiendum, definiens, herr),
                                  herr);
}
void Context::add_macro_func(string definiendum, string p1, string definiens, bool variadic) {
  vector<string> arglist;
  arglist.push_back(p1);
  macros[definiendum] = new_macro(definiendum, std::move(arglist), variadic,
                                  parse_macro(definiendum, definiens, herr),
                                  herr);
}
void Context::add_macro_func(string definiendum, string p1, string p2, string definiens, bool variadic) {
  vector<string> arglist;
  arglist.push_back(p1);
  arglist.push_back(p2);
  macros[definiendum] = new_macro(definiendum,  std::move(arglist), variadic,
                                  parse_macro(definiendum, definiens, herr),
                                  herr);
}
void Context::add_macro_func(string definiendum, string p1, string p2, string p3, string definiens, bool variadic)
{
  vector<string> arglist;
  arglist.push_back(p1);
  arglist.push_back(p2);
  arglist.push_back(p3);
  macros[definiendum] = new_macro(definiendum,  std::move(arglist), variadic,
                                  parse_macro(definiendum, definiens, herr),
                                  herr);
}

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

void Context::read_search_directories(const char* filename) {
  read_search_directories_gnu(filename,nullptr,nullptr);
}
void Context::read_search_directories_gnu(const char* filename, const char* begin_line, const char* end_line)
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
void Context::add_search_directory(string dir)
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

void Context::reset()
{

}
void Context::reset_all()
{

}
void Context::copy(const Context &ct)
{
  remap_set n;
  ct.global->copy(global.get(), n);
  ct.global->remap(n, ErrorContext(herr, {"Internal Copy Operation", 0, 0}));

  for (macro_iter_c mi = ct.macros.begin(); mi != ct.macros.end(); ++mi){
    pair<macro_iter,bool> dest = macros.insert(pair<string,macro_type*>(mi->first,nullptr));
    if (dest.second) {
      dest.first->second = new_macro(*mi->second);
    }
  }
  for (definition* var : ct.variadics) {
    if (var->parent)
      variadics.insert(find_mirror(var, global.get()));
    else
      variadics.insert(var);
  }
}
void Context::swap(Context &ct) {
  if (!parse_open and !ct.parse_open) {
    ct.global.swap(global);
    macros.swap(ct.macros);
    variadics.swap(ct.variadics);
  } else {
    herr->error({"Internal Swap Operation", 0, 0})
        << "ERROR! Cannot swap context while parse is active!";
  }
}

macro_map &Context::global_macros() {
  return builtin_context().macros;
}

void Context::load_standard_builtins()
{
  keywords["asm"] = TT_ASM;
  keywords["__asm"] = TT_ASM;
  keywords["__asm__"] = TT_ASM;
  keywords["class"] = TT_CLASS;
  keywords["decltype"] = TT_DECLTYPE;
  keywords["typeid"] = TT_TYPEID;
  keywords["enum"] = TT_ENUM;
  keywords["extern"] = TT_EXTERN;
  keywords["namespace"] = TT_NAMESPACE;
  keywords["operator"] = TT_OPERATORKW;
  keywords["private"] = TT_PRIVATE;
  keywords["protected"] = TT_PROTECTED;
  keywords["public"] = TT_PUBLIC;
  keywords["friend"] = TT_FRIEND;
  keywords["sizeof"] = TT_SIZEOF;
  keywords["__is_empty"] = TT_ISEMPTY;
  // keywords["__is_pod"] = TT_ISEMPTY;
  keywords["struct"] = TT_STRUCT;
  keywords["template"] = TT_TEMPLATE;
  keywords["typedef"] = TT_TYPEDEF;
  keywords["typename"] = TT_TYPENAME;
  keywords["union"] = TT_UNION;
  keywords["using"] = TT_USING;
  keywords["new"] = TT_NEW;
  keywords["delete"] = TT_DELETE;

  keywords["const_cast"] = TT_CONST_CAST;
  keywords["static_cast"] = TT_STATIC_CAST;
  keywords["dynamic_cast"] = TT_DYNAMIC_CAST;
  keywords["reinterpret_cast"] = TT_REINTERPRET_CAST;

  keywords["auto"] = TT_AUTO;
  keywords["alignas"] = TT_ALIGNAS;
  keywords["alignof"] = TT_ALIGNOF;
  keywords["constexpr"] = TT_CONSTEXPR;
  keywords["noexcept"] = TT_NOEXCEPT;
  keywords["static_assert"] = TT_STATIC_ASSERT;

  // GNU Extensions - These are all rolled into the standard in some form.
  keywords["__attribute__"] = TT_ATTRIBUTE;
  keywords["__extension__"] = TT_EXTENSION;
  keywords["__typeof__"] = TT_TYPEOF;
  keywords["__typeof"] = TT_TYPEOF;

  // MinGW Fuckery
  keywords["__MINGW_IMPORT"] = TT_INVALID;

  // C++ Extensions
  keywords["false"] = TT_FALSE;
  keywords["true"] = TT_TRUE;
}
void Context::load_gnu_builtins()
{

}

#include <iostream>
#include <General/parse_basics.h>

void Context::output_types(ostream &out) {
  out << "Unimplemented";
}
static inline void print_macro(macro_iter it, ostream &out)
{
  out << it->second->toString();
}
void Context::output_macro(string macroname, ostream &out)
{
  macro_iter it = macros.find(macroname);
  if (it == macros.end()) out << "Macro `" << macroname << "' has not been defined." << endl;
  else print_macro(it, out);
}
void Context::output_macros(ostream &out)
{
  for (macro_iter it = macros.begin(); it != macros.end(); it++)
    print_macro(it, out);
}

void Context::output_definitions(ostream &out) {
  out << global->toString();
}

Context::Context(ErrorHandler *herr_):
    parse_open(false), global(new definition_scope()), herr(herr_) {
  copy(builtin_context());
}
Context::Context(int): parse_open(false), global(new definition_scope()) { }

size_t Context::search_dir_count() const { return search_directories.size(); }
string Context::search_dir(size_t index) const { return search_directories[index]; }
