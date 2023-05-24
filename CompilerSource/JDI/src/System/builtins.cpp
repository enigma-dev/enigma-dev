/**
 * @file  builtins.cpp
 * @brief A small source implementing the \c jdi::builtin context and functions
 *        for manipulating other builtins.
 *
 * This file is used for configuring the parse environment ahead of time.
 * Contents of the builtin context are inherited by user-created contexts, and
 * the contents of \c builtin_flags determines accepted modifiers when parsing
 * code in general, regardless of the enclosing context.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <map>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

#include "builtins.h"

namespace jdi {
  tf_map builtin_declarators;
  prim_map builtin_primitives;
  typeflag* builtin_typeflag__throw;
}

using namespace jdi;
namespace jdi {

typeflag *builtin_flag__volatile;
typeflag *builtin_flag__static;
typeflag *builtin_flag__const;
typeflag *builtin_flag__mutable;
typeflag *builtin_flag__register;
typeflag *builtin_flag__inline;
typeflag *builtin_flag__Complex;

typeflag *builtin_flag__unsigned;
typeflag *builtin_flag__signed;
typeflag *builtin_flag__short;
typeflag *builtin_flag__long;
typeflag *builtin_flag__long_long;

typeflag *builtin_flag__throw;
typeflag *builtin_flag__restrict;
typeflag *builtin_typeflag__override;
typeflag *builtin_typeflag__final;

definition *builtin_type__void;
definition *builtin_type__bool;
definition *builtin_type__char;
definition *builtin_type__int;
definition *builtin_type__float;
definition *builtin_type__double;

definition *builtin_type__wchar_t;
definition *builtin_type__va_list;

typeflag *builtin_flag__virtual;
typeflag *builtin_flag__explicit;

static std::vector<std::unique_ptr<typeflag>> builtin_typeflags;

// Make entities cache this or not use it.
static jdi::Context *builtin = nullptr;

bool typeflag::CanApply(unsigned long mask_, unsigned long value_) const {
  if (!(mask_ & mask_)) return true;
  // This is a hack. Better to store an upgrade map. But this is more efficient,
  // and long is the only type this applies to.
  if (this == builtin_flag__long) return (value_ & mask) == value;
  return false;
}

void typeflag::Apply(unsigned long *mask_, unsigned long *value_,
                     const ErrorContext &errc) const {
  if (!(*mask_ & mask)) {
    *mask_ |= mask;
    *value_ |= value;
    return;
  }
  // Sorta perpetuating more hacks, here, but whatever.
  if (this == builtin_flag__long) {
    if ((*value_ & mask) == value) {
      *value_ = (*value_ & ~mask) | builtin_flag__long_long->value;
    } else {
      if (builtin_flag__short->Matches(*value_)) {
        errc.error() << "Conflicting use of short and long specifiers.";
      } else {
        errc.error() << "Resulting type is too long.";
      }
    }
    return;
  }
  if (!Matches(*value_)) {
    if (this == builtin_flag__unsigned || this == builtin_flag__signed) {
      errc.error() << "Conflicting use of signed and unsigned specifiers.";
    } else {
      errc.error() << name << " specifier conflicts with a previous specifier.";
    }
  }
  errc.warning() << "Redundant " << name << " specifier in type.";
}

definition *add_primitive(string name, size_t sz) {
  auto ntit = builtin_primitives.insert({name, nullptr});
  auto defit = builtin_declarators.insert({name, nullptr});
  if (ntit.second && defit.second) {
    ntit.first->second = new definition_atomic(name, nullptr, DEF_TYPENAME, sz);
    defit.first->second = new typeflag(name, ntit.first->second);
  } else {
    std::cerr << "Internal error: Redefinition of builtin type `"
              << name << "`.\n";
    abort();
  }
  return ntit.first->second;
}

// Masks always group bits, so just shift left one and AND by the negation to
// remove any extra bits from the last mask.
static unsigned long nextbit(unsigned long mask) {
  return (mask << 1) & ~mask;
}

typeflag *add_decflag(string name, USAGE_FLAG usage, int bitsize) {
  auto insit = builtin_declarators.insert({name, nullptr});
  if (insit.second) {
    unsigned long mask = 0;
    const unsigned long firstbit =
        builtin_typeflags.empty() ? 1 : nextbit(builtin_typeflags.back()->mask);
    for (int i = 0; i < bitsize; ++i) mask |= firstbit << i;
    builtin_typeflags.emplace_back(
        new typeflag(name, usage, mask, firstbit));
    insit.first->second = builtin_typeflags.back().get();
  } else {
    std::cerr << "Internal error: Redefinition of builtin flag `"
              << name << "`.\n";
    abort();
  }
  return insit.first->second;
}

typeflag *add_decflag(string name, typeflag *base, int value) {
  auto insit = builtin_declarators.insert({name, nullptr});
  if (insit.second) {
    const auto mask = base->mask;
    builtin_typeflags.emplace_back(
        new typeflag(name, base->usage, mask, value & mask));
    insit.first->second = builtin_typeflags.back().get();
  } else {
    std::cerr << "Internal error: Redefinition of builtin flag `"
              << name << "`.\n";
    abort();
  }
  return insit.first->second;
}

void alias_primitive(string name, definition *def)  {
  auto ntit = builtin_primitives.insert({name, nullptr});
  auto defit = builtin_declarators.insert({name, nullptr});
  if (ntit.second && defit.second) {
    ntit.first->second = def;
    defit.first->second = new typeflag(name, def);
  } else {
    std::cerr << "Internal error: Redefinition of builtin type `"
              << name << "`.\n";
    abort();
  }
}

void alias_decflag(string name, typeflag *flag) {
  auto insit = builtin_declarators.insert({name, nullptr});
  if (insit.second) {
    insit.first->second = flag;
  } else {
    std::cerr << "Internal error: Redefinition of built-in flag `"
              << name << "`.\n";
    abort();
  }
}

void add_gnu_declarators() {
  builtin_flag__volatile = add_decflag("volatile");
  builtin_flag__static   = add_decflag("static");
  builtin_flag__const    = add_decflag("const");
  builtin_flag__mutable  = add_decflag("mutable");
  builtin_flag__register = add_decflag("register");
  builtin_flag__inline   = add_decflag("inline");
  builtin_flag__Complex  = add_decflag("_Complex");

  builtin_typeflag__throw    = add_decflag("throw");
  builtin_flag__restrict     = add_decflag("restrict");
  builtin_typeflag__override = add_decflag("override");
  builtin_typeflag__final    = add_decflag("final");

  alias_decflag("__restrict", builtin_flag__restrict);

  builtin_type__void   = add_primitive("void",  -1);
  builtin_type__bool   = add_primitive("bool",   1);
  builtin_type__char   = add_primitive("char",   1);
  builtin_type__int    = add_primitive("int",    4);
  builtin_type__float  = add_primitive("float",  4);
  builtin_type__double = add_primitive("double", 8);

  builtin_flag__unsigned = add_decflag("unsigned", UF_PRIMITIVE_FLAG);
  builtin_flag__long     = add_decflag("long",     UF_PRIMITIVE_FLAG, 3);

  // Opposite day!
  builtin_flag__signed = add_decflag("signed", builtin_flag__unsigned, 0);
  builtin_flag__short  = add_decflag("short",  builtin_flag__long,    -1);

  builtin_flag__long_long = add_decflag("long long", builtin_flag__long,
                                        builtin_flag__long->value << 1);

  // TODO: wchar size needs to be configurable.
  builtin_type__wchar_t = add_primitive("wchar_t",           2);
  builtin_type__va_list = add_primitive("__builtin_va_list", 8);
  builtin_type__va_list = add_primitive("__int128",          16);
  builtin_type__va_list = add_primitive("__float128",        16);

  builtin_flag__virtual  = add_decflag("virtual");
  builtin_flag__explicit = add_decflag("explicit");

  builtin->variadics.insert(builtin_type__va_list);
  // builtin->add_macro("JUST_DEFINE_IT_RUN", {});
}

void cleanup_declarators() {
  builtin_declarators.clear();
  builtin_typeflags.clear();
  {
    std::set<definition*> uniques;
    for (const auto &prim : builtin_primitives) uniques.insert(prim.second);
    for (definition *def : uniques) delete def;
  }
}

string typeflags_string(definition *type, unsigned long flags) {
  string res;
  unsigned long covered = 0;
  for (const auto &flag : builtin_typeflags) {
    if (flag->Matches(flags) && flag->value) {
      res += flag->name + " ";
      covered |= flag->mask;
    }
  }
  if (type)
    res += type->name;
  else res += "<null>";
  return res;
}

static void initialize() {
  builtin = new Context(0);
  add_gnu_declarators();
  builtin->load_standard_builtins();
}

void clean_up() {
  cleanup_declarators();
  delete builtin;
  builtin = nullptr;
}

Context &builtin_context() {
  if (!builtin) initialize();
  return *builtin;
}

}  // namespace jdi
