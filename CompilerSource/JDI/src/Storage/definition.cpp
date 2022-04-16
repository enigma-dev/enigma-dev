/**
 * @file  definition.cpp
 * @brief System source implementing arbitrary components of the definition storage class.
 *
 * This file is likely used by absolutely everything in the parse system, as
 * it is the medium through which output definitions are created and manipulated.
 *
 * @section License
 *
 * Copyright (C) 2011-2014 Josh Ventura
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

#include "definition.h"
#include <iostream>
#include <cstdio>
#include <typeinfo>
#include <System/builtins.h>
#include <Parser/handlers/handle_function_impl.h>
#include <API/compile_settings.h>
#include <Parser/context_parser.h>
#include <Parser/is_potential_constructor.h>
using namespace std;

namespace jdi {

definition::definition(string n,definition* p,unsigned int f): flags(f), name(n), parent((definition_scope*)p) {}
definition::definition(): flags(0), name(), parent(nullptr) {}

ptrdiff_t definition::defcmp(definition *d1, definition *d2) {
  return d1 - d2;
}

definition_typed::definition_typed(string n, definition* p, definition* tp,
                                   unsigned int typeflags, int flgs):
    definition(n,p,flgs | DEF_TYPED), type(tp), referencers(), modifiers(typeflags) {
  if (type and type->flags & DEF_DEPENDENT) flags |= DEF_DEPENDENT;
}
definition_typed::definition_typed(string n, definition* p, definition* tp, ref_stack *rf, unsigned int typeflags, int flgs): definition(n,p,flgs), type(tp), referencers(*rf), modifiers(typeflags) {
  if (type and type->flags & DEF_DEPENDENT) flags |= DEF_DEPENDENT;
}
definition_typed::definition_typed(string n, definition* p, definition* tp, const ref_stack &rf, unsigned int typeflags, int flgs): definition(n,p,flgs), type(tp), referencers(rf), modifiers(typeflags) {
  if (type and type->flags & DEF_DEPENDENT) flags |= DEF_DEPENDENT;
}
definition_typed::definition_typed(string n, definition* p, const full_type &tp, int flgs): definition(n,p,flgs), type(tp.def), referencers(tp.refs), modifiers(tp.flags) {
  if (type and type->flags & DEF_DEPENDENT) flags |= DEF_DEPENDENT;
}

string definition::qualified_id() const {
  if (!parent) return name;
  return parent->qualified_id() + "::" + name;
}

// NOTICE:
// This (flgs | DEF_FUNCTION) &~(DEF_PRIVATE | DEF_PROTECTED) stuff is to make sure that definition_function, which is basically a method group,
// is not itself protected or private. Only specific overloads, which actually have a prototype, may be private or protected.
// XXX: Should all flags be discarded in this manner?

definition_function::definition_function(string n, definition* p, definition* tp, const ref_stack &rf, unsigned int typeflags, int flgs, SourceLocation sloc, ErrorHandler *herr):
  definition(n, p, (flgs | DEF_FUNCTION) &~(DEF_PRIVATE | DEF_PROTECTED)) {
  overload(tp, rf, typeflags, flgs, nullptr, herr->at(sloc));
}
definition_function::definition_function(string n, definition* p, int flgs):
  definition(n, p, (flgs | DEF_FUNCTION) &~(DEF_PRIVATE | DEF_PROTECTED)) {}

definition_overload::definition_overload(string n, definition *p, definition* tp, const ref_stack &rf, unsigned int typeflags, int flgs):
  definition_typed(n, p, tp, rf, typeflags, (flgs & ~(DEF_FUNCTION)) | DEF_OVERLOAD), implementation(nullptr) {}

definition_overload *definition_function::overload(
    definition *tp, const ref_stack &rf, unsigned int typeflags,
    unsigned int addflags, void *implementation, ErrorContext errc) {
  arg_key key(rf);
  pair<overload_iter, bool> ins = overloads.insert(pair<arg_key,definition_overload*>(key, nullptr));
  if (!ins.second) {
    if (implementation) {
      if (ins.first->second->implementation) {
        errc.error(
            "Reimplementation of function; old implementation discarded");
        delete_function_implementation(ins.first->second->implementation);
        FATAL_RETURN(nullptr);
        ins.first->second->implementation = implementation;
      }
      ins.first->second->implementation = implementation;
    }
  }
  else {
    ins.first->second = make_unique<definition_overload>(
                           name, parent, tp, rf, typeflags, flags | addflags);
  }
  return ins.first->second.get();
}

definition_overload *definition_function::overload(
    const full_type &ft, unsigned int addflags, ErrorContext errc) {
  return overload(ft.def, ft.refs, ft.flags, addflags, nullptr, errc);
}

void definition_function::overload(unique_ptr<definition_template> ovrl,
                                   ErrorContext) {
  // FIXME: This could push a forward declaration and an actual implementation.
  // Something needs to deduplicate them. Probably also check if this overload
  // conflicts with other overloads.
  template_overloads.push_back(std::move(ovrl));
}

definition_overload* definition_scope::overload_function(
      string fname, full_type &ft, unsigned inflags, ErrorContext errc) {
  definition_function *df;
  definition_overload *dov;
  decpair dp = declare(fname, nullptr);
  if (dp.inserted) {
    auto uf = make_unique<definition_function>(fname, this, inflags);
    df = uf.get();
    dp.def = std::move(uf);
  } else {
    if (dp.def->flags & DEF_FUNCTION) {
      df = (definition_function*) dp.def.get();
    } else {
      errc.error() << "Cannot redeclare " << PQuote(fname)
                   << " as function in this scope";
      return nullptr;
    }
  }

  dov = df->overload(ft, inflags, errc);
  return dov;
}

definition_function* definition_scope::overload_function(
    string fname, unique_ptr<definition_template> dtemp, unsigned inflags,
    ErrorContext errc) {
  definition_function *df;
  decpair dp = declare(fname, nullptr);
  if (dp.inserted) {
    auto uf = make_unique<definition_function>(fname, this, inflags);
    df = uf.get();
    dp.def = std::move(uf);
  } else {
    if (dp.def->flags & DEF_FUNCTION) {
      df = (definition_function*) dp.def.get();
    } else {
      errc.error() << "Cannot redeclare " << PQuote(fname)
                   << " as function in this scope";
      return nullptr;
    }
  }

  df->overload(std::move(dtemp), errc);
  return df;
}

decpair definition_scope::declare_c_struct(string n,
                                           unique_ptr<definition> def) {
  pair<defiter, bool> insp = c_structs.insert(std::make_pair(n, std::move(def)));
  dec_order.push_back(insp.first);
  return decpair(insp.first->second, insp.second);
}

definition *definition_scope::look_up(string sname) {
  if (defiter it = members.find(sname); it != members.end())
    return it->second.get();
  //if (members.size() == 1)
  //  cout << sname << ": Not my member: " << members.begin()->first << " => " << members.begin()->second->toString() << endl;
  if (auto it = using_general.find(sname); it != using_general.end())
    return it->second;
  // Lookup of class/struct names happens after using lookup but before checking
  // used scopes.
  if (auto it = c_structs.find(sname); it != c_structs.end())
    return it->second.get();
  /*if (members.size() == 1) {
    cout << "Not in my using: {" << endl;
    for (defiter u = using_general.begin(); u != using_general.end(); ++u)
      cout << u->second->toString() << endl;
    cout << "}" << endl;
  }*/
  definition *res;
  for (definition_scope *scope : using_scopes)
    if ((res = scope->find_local(sname)))
      return res;
  if (parent == nullptr)
    return nullptr;
  return parent->look_up(sname);
}
definition *definition_class::look_up(string sname) {
  if (defiter it = members.find(sname); it != members.end())
    return it->second.get();
  if (auto it = using_general.find(sname); it != using_general.end())
    return it->second;
  definition *res;
  for (definition_scope *scope : using_scopes)
    if ((res = scope->find_local(sname)))
      return res;
  for (vector<ancestor>::iterator ait = ancestors.begin(); ait != ancestors.end(); ++ait)
    if ((res = ait->def->find_local(sname)))
      return res;
  if (parent == nullptr)
    return nullptr;
  return parent->look_up(sname);
}
definition *definition_scope::find_local(string sname) {
  if (auto it = members.find(sname); it != members.end())
    return it->second.get();
  if (auto it = using_general.find(sname); it != using_general.end())
    return it->second;
  definition *res;
  for (definition_scope *scope : using_scopes)
    if ((res = scope->find_local(sname)))
      return res;
  return nullptr;
}
definition *definition_class::find_local(string sname) {
  definition *res = definition_scope::find_local(sname);
  if (res) return res;
  for (vector<ancestor>::iterator ait = ancestors.begin(); ait != ancestors.end(); ++ait)
    if ((res = ait->def->find_local(sname)))
      return res;
  return nullptr;
}

definition *definition_tempparam::look_up(string sname) {
  must_be_class = true;
  return definition_class::look_up(sname);
}
decpair definition_tempparam::declare(string sname,
                                      unique_ptr<definition> def) {
  must_be_class = true;
  return definition_class::declare(sname, std::move(def));
}
definition *definition_scope::get_local(string n) {
  return find_local(n);
}
definition *definition_tempparam::get_local(string sname) {
  must_be_class = true;
  pair<defmap::iterator, bool> insp = members.insert(defmap::value_type(sname, nullptr));
  if (insp.second) {
    insp.first->second = std::make_unique<definition_tempparam>(sname, this);
  }
  return insp.first->second.get();
}
definition *definition_class::get_local(string sname) {
  definition *res = definition_scope::get_local(sname);
  if (!res && (sname == name || (instance_of && sname == instance_of->name)))
    if (defiter it = members.find(constructor_name); it != members.end())
      res = it->second.get();
  return res;
}
definition *definition_hypothetical::get_local(string sname) {
  required_flags |= DEF_CLASS;
  auto errc = default_error_handler->at({
     __FILE__ " @ definition_hypothetical::get_local", __LINE__ + 1, 0});
  return def ? context_parser::handle_hypothetical_access(this, sname, errc)
             : nullptr;
}

void definition_scope::use_namespace(definition_scope *ns) {
  using_scopes.push_back(ns);
}
void definition_scope::unuse_namespace(definition_scope *ns) {
  for (size_t i = using_scopes.size(); i > 0; )
    if (using_scopes[--i] == ns) {
      using_scopes.erase(using_scopes.begin() + i, using_scopes.begin() + i);
      return;
    }
}

void definition_scope::use_general(string n, definition *def) {
  using_general.insert(pair<string, definition*>(n, def));
}

definition_scope::definition_scope(): definition("", nullptr, DEF_SCOPE) {}
definition_scope::definition_scope(const definition_scope&): definition() {
  // TODO: Implement
}

definition_scope::definition_scope(string name_, definition *parent_,
                                   unsigned int flags_):
    definition(name_, parent_, flags_ | DEF_SCOPE) {}

definition_class::ancestor::ancestor(unsigned protection_level,
                                     definition_class* inherit_from):
    protection(protection_level), def(inherit_from) {}
definition_class::ancestor::ancestor() {}
definition_class::definition_class(string classname, definition_scope* prnt,
                                   unsigned flgs):
    definition_scope(classname, prnt, flgs), instance_of(nullptr) {}

definition_union::definition_union(string classname, definition_scope* prnt,
                                   unsigned flgs):
    definition_scope(classname, prnt, flgs) {}

definition_valued::definition_valued(string vname, definition *parnt,
                                     definition* tp, unsigned tflgs,
                                     unsigned int flgs, const value &val):
    definition_typed(vname, parnt, tp, tflgs, flgs | DEF_VALUED), value_of(val) {}

definition_enum::definition_enum(string classname, definition_scope* parnt,
                                 unsigned flgs):
    definition_class(classname, parnt, flgs | DEF_ENUM) {}

definition_template::definition_template(string n, definition *p, unsigned f):
    definition_scope(n, p, f | DEF_TEMPLATE), def(nullptr)  {}

definition_tempparam::definition_tempparam(
      string p_name, definition_scope* p_parent, unsigned p_flags):
    definition_class(p_name, p_parent, p_flags | DEF_TEMPPARAM | DEF_DEPENDENT),
    default_assignment(nullptr) {}
definition_tempparam::definition_tempparam(
      string p_name, definition_scope* p_parent, unique_ptr<AST> defval,
      unsigned p_flags):
    definition_class(p_name, p_parent, p_flags | DEF_TEMPPARAM | DEF_DEPENDENT),
    default_assignment(std::move(defval)) {}

static int nest_count = 0;
struct nest_ { nest_() { ++nest_count; } ~nest_() { --nest_count; } };
definition* definition_template::instantiate(const arg_key& key, const ErrorContext &errc) {
  if (nest_count >= 128) {
    cerr << "Maximum nested template depth of 128 (GCC default) exceeded. Bailing." << endl;
    return nullptr;
  }
  nest_ this_nest;

  // TODO: Move this specialization search into the not-found if (ins.second) below, then add the specialization to the instantiation map.
  // TODO: Be careful not to double free those specializations. You may need to keep a separate map for regular instantiations to free.
  specialization *spec = find_specialization(key);
  if (spec) {
    arg_key speckey = spec->key.get_key(key);
    return spec->spec_temp->instantiate(speckey, errc);
  }

  pair<institer, bool> ins = instantiations.insert(pair<arg_key, instantiation*>(key, nullptr));
  if (ins.second) {
    //cout << "Instantiating new " << name << "<" << key.toString() << "> (abstract: " << key.is_abstract() << ")" << endl;
    remap_set n;
    size_t ind = 0;
    unique_ptr<definition> ntemp = def->duplicate(n);
    ntemp->name += "<" + key.toString() + ">";
    if (ntemp->flags & DEF_CLASS)
      ((definition_class*) ntemp.get())->instance_of = this;
    else
      cerr << "Not a class lol" << endl;
    definition *remap_me = ntemp.get();
    ins.first->second = make_unique<instantiation>();
    ins.first->second->def = std::move(ntemp);
    for (piterator it = params.begin(); it != params.end(); ++it) {
      unique_ptr<definition> ndef = key.make_definition(ind++, (*it)->name, this);
      n[it->get()] = ndef.get();
      ins.first->second->parameter_defs.push_back(std::move(ndef));
    }
    size_t keyc = key.size();
    if (keyc != params.size()) {
      errc.error() << "Attempt to instantiate template with an incorrect number of parameters; "
                   << "passed " << (key.end() - key.begin())
                   <<  ", required " << params.size();
      FATAL_RETURN(nullptr);
    }

    remap_me->remap(n, errc);
  }

  return ins.first->second->def.get();
}

definition_template::specialization *definition_template::find_specialization(const arg_key &key) const
{
  specialization *spec = nullptr;
  int merit = 0;

  for (speclist::const_iterator i = specializations.begin(); i != specializations.end(); ++i) {
    if (!(*i)->filter.matches(key))
      continue;
    int m = (*i)->key.merit(key);
    if (m > merit) {
      spec = i->get();
      merit = m;
    }
  }

  return spec;
}

arg_key spec_key::get_key(const arg_key &src_key)
{
  arg_key res(ind_count);
  for (size_t i = 0; i < ind_count; ++i)
    res.put_node(i, src_key.begin()[arg_inds[i][1]]); // Copy the node from the first use
  return res;
}

int spec_key::merit(const arg_key &k)
{
  size_t tmerit = 1;
  for (size_t i = 0; i < ind_count; ++i)
  {
    int matchc = arg_inds[i][0]; // The number of arguments this is matching
    int o = arg_inds[i][1];
    for (size_t j = 1; j <= arg_inds[i][0]; ++j)
      if (k[arg_inds[i][j]] != k[o])
        return 0;
    if (matchc > 1)
      tmerit += matchc;
  }
  return tmerit;
}

bool spec_key::same_as(const spec_key &) {
  // TODO: Implement
  return false;
}

/*spec_key::spec_key(const definition_template *small_kt, const arg_key &big_key) {
  ind_count = small_kt->params.size();
  arg_inds = new unsigned*[ind_count];
  for (size_t i = 0; i < ind_count; ++i)
    arg_inds[i] = 0;
  unsigned big_count = big_key.end() - big_key.begin();
  for (size_t mini = 0; mini < ind_count; ++mini) {
    for (size_t maxi = 0; maxi < big_count; ++maxi) {
      if (big_key[maxi] == small_kt->params[mini])
        ++arg_inds[mini];
    }
  }
}*/
spec_key::spec_key(size_t big_count, size_t small_count):
    arg_inds(small_count), ind_count(small_count)  {
  ++big_count;
  for (size_t i = 0; i < small_count; ++i) {
    arg_inds[i].resize(big_count);
    arg_inds[i][0] = 0;
  }
}

spec_key::spec_key(const spec_key &k, bool) {
  arg_inds.resize(k.ind_count);
  for (size_t i = 0; i < k.ind_count; ++i) {
    unsigned nc = k.arg_inds[i][0];
    arg_inds[i].resize(nc + 1);
    for (size_t j = 0; j <= nc; ++j)
      arg_inds[i][j] = k.arg_inds[i][j];
  }
  max_param = k.max_param;
  ind_count = k.ind_count;
}

definition_atomic::definition_atomic(string n, definition* p, unsigned int f,
                                     size_t size):
    definition_scope(n,p,f), sz(size) {}

definition_hypothetical::definition_hypothetical(string n, definition_scope *p,
                                                 unsigned f, unique_ptr<AST> d):
    definition_class(n, p, f | DEF_HYPOTHETICAL | DEF_DEPENDENT),
    def(std::move(d)) {}
definition_hypothetical::definition_hypothetical(string n, definition_scope *p,
                                                 unique_ptr<AST> d):
    definition_class(n, p, DEF_HYPOTHETICAL | DEF_DEPENDENT),
    def(std::move(d)) {}

using_scope::using_scope(string n, definition_scope* u): definition_scope(n, u, DEF_NAMESPACE) {}
using_scope::~using_scope() { parent->unuse_namespace(this); }

//========================================================================================================
//======: Declare Functions :=============================================================================
//========================================================================================================

decpair definition_scope::declare(string n, unique_ptr<definition> def) {
  inspair insp = members.insert(entry(n, std::move(def)));
  dec_order.push_back(insp.first);
  return decpair(insp.first->second, insp.second);
}
decpair definition_class::declare(string n, unique_ptr<definition> def) {
  return definition_scope::declare(n, std::move(def));
}

//========================================================================================================
//======: Sizeof functions :==============================================================================
//========================================================================================================

value definition::size_of(const ErrorContext &errc) {
  errc.warning("Taking size of bare definition");
  return 0L;
}

value definition_class::size_of(const ErrorContext &errc) {
  value sz = 0L;
  for (defiter it = members.begin(); it != members.end(); ++it)
    if (not(it->second->flags & DEF_TYPENAME))
    {
      value as = it->second->size_of(errc);
      if (as.type == VT_INTEGER && as) // TODO: FIXME: This entire size thing is beans; an alignof() is needed.
      {
        size_t addsize, padsize;
        addsize = (long)as;
        padsize = (long)sz;
        padsize += addsize - 1;
        padsize /= addsize; padsize *= addsize;
        padsize += addsize;
        sz = (long)padsize;
      }
      else
        return VT_DEPENDENT;
    }
  if (!sz)
    sz = 1L;
  return sz;
}

value definition_enum::size_of(const ErrorContext &errc) {
  return type->size_of(errc);
}

value definition_function::size_of(const ErrorContext &errc) {
  errc.error("Computing size of function");
  return 0L;
}

value definition_scope::size_of(const ErrorContext &errc) {
  size_t sz = 0;
  for (defiter it = members.begin(); it != members.end(); ++it)
    if (not(it->second->flags & DEF_TYPENAME)) {
      value szadd = it->second->size_of(errc);
      if (szadd.type == VT_INTEGER)
        sz += (long)szadd;
    }
  return (long)sz;
}

value definition_template::size_of(const ErrorContext &errc) {
  errc.error() << "Attempt to take size of template " << PQuote(name);
  return 0L;
}

value definition_typed::size_of(const ErrorContext &errc) {
  return type? type->size_of(errc) : value(0L);
}

value definition_union::size_of(const ErrorContext &errc) {
  size_t sz = 0;
  for (defiter it = members.begin(); it != members.end(); ++it)
    if (not(it->second->flags & DEF_TYPENAME))
      sz = max((long)sz, (long)it->second->size_of(errc));
  return (long)sz;
}

value definition_atomic::size_of(const ErrorContext &) {
  return (long)sz;
}

value definition_hypothetical::size_of(const ErrorContext &) {
  return VT_DEPENDENT;
}

//========================================================================================================
//======: String printers :===============================================================================
//========================================================================================================

inline string tostr(int x) { char buf[16]; sprintf(buf, "%d", x); return buf; }
inline unsigned dl(unsigned l) { return l == unsigned(-1)? l:l-1; }

string definition::toString(unsigned, unsigned indent) const {
  return string(indent, ' ') + "void " + name + ";";
}
string definition_atomic::toString(unsigned, unsigned indent) const {
  return string(indent, ' ') + "typedef __atom__[" + tostr(sz) + "] " + name + ";";
}
string definition_class::toString(unsigned levels, unsigned indent) const {
  const string inds(indent, ' ');
  string res = inds + "class " + name;
  if (!ancestors.empty()) {
    res += ": ";
    for (vector<ancestor>::const_iterator it = ancestors.begin(); it != ancestors.end(); ++it) {
      res += ((it->protection == DEF_PRIVATE)? "private " : (it->protection == DEF_PROTECTED)? "protected " : "public ");
      res += it->def->name + " ";
    }
  }
  if (levels and not(flags & DEF_INCOMPLETE))
    res += "\n", res += definition_scope::toString(dl(levels), indent);
  return res;
}
string definition_enum::toString(unsigned levels, unsigned indent) const {
  const string inds(indent, ' ');
  string res = inds + "enum " + name + ": " + (type? type->name + " " : "");
  if (levels) {
    res += "{\n";
    string sinds(indent+2, ' ');
    bool first = true;
    for (vector<const_pair>::const_iterator it = constants.begin(); it != constants.end(); ++it) {
      if (!first) res += ",\n";
      res += sinds + it->def->name + " = " + it->def->value_of.toString();
      first = false;
    }
    res += "\n" + inds + "}";
  }
  else res += "{ ... }";
  return res;
}
string definition_overload::toString(unsigned levels, unsigned indent) const {
  return definition_typed::toString(levels, indent);
}
string definition_function::toString(unsigned levels, unsigned indent) const {
  string res;
  for (overload_citer it = overloads.begin(); it != overloads.end(); ++it) {
    if (res.size()) res += "\n";
    res += it->second->toString(levels, indent);
  }
  for (const auto &overload : template_overloads) {
    if (res.size()) res += "\n";
    res += overload->toString(levels, indent);
  }
  return res;
}
string definition_scope::toString(unsigned levels, unsigned indent) const {
  string inds(indent, ' '), res = inds;
  if (flags & DEF_NAMESPACE)
    res += name.empty()? "namespace " : "namespace " + name + " ";
  if (levels) {
    res += "{\n";
    for (auto it : dec_order) {
      if (it->second) res += it->second->toString(levels-1, indent+2) + "\n";
    }
    res += inds + "}";
  }
  else res += "{ ... }";
  return res;
}
static inline string trimhead(string x) {
  size_t r = x.find_first_not_of(" \t");
  if (!r) return x;
  if (r == string::npos) return "";
  return x.substr(r);
}
string definition_template::toString(unsigned levels, unsigned indent) const {
  string res(indent, ' ');
  res += "template<";
  bool first = true;
  for (const unique_ptr<definition_tempparam> &d : params) {
    if (!first) res += ", ";
    if (d->flags & DEF_TYPENAME) {
      res += d->name.empty()? "typename" : "typename " + d->name;
      if (d->default_assignment)
        res += " = " + d->default_assignment->toString();
    }
    else {
      res += (d->integer_type.def? d->integer_type.toString() : "<ERROR>");
      if (d->flags & DEF_VALUED)
        res += " = " + ((definition_valued*) d.get())->value_of.toString();
    }
    first = false;
  }
  res += "> ";
  res += def? trimhead(def->toString(levels, indent)): "<null>";
  return res;
}
string definition_typed::toString(unsigned levels, unsigned indent) const {
  string res(indent, ' ');
  if (flags & DEF_TYPENAME) res += "typedef ";
  res += type? typeflags_string(type, modifiers) : "<null>";
  res += " ";
  res += referencers.toStringLHS() + name + referencers.toStringRHS();
  if (levels && (flags & DEF_TYPENAME) && type && (type->flags & DEF_TYPED))
    res += " (" + type->toString(levels - 1, 0) + ")";
  else res += ";";
  return res;
}
string definition_union::toString(unsigned levels, unsigned indent) const {
  string res = "union " + name + definition_scope::toString(levels,indent);
  return res;
}
string definition_valued::toString(unsigned, unsigned indent) const {
  return string(indent, ' ') + referencers.toStringLHS() + name + referencers.toStringRHS() + " = " + value_of.toString();
}
string definition_hypothetical::toString(unsigned, unsigned indent) const {
  return string(indent, ' ') + "template<...> " + parent->toString(0, 0) + "::" + name;
}

string definition::kind() const           { return "definition"; }
string definition_atomic::kind() const    { return "primitive"; }
string definition_class::kind() const     { return "class"; }
string definition_enum::kind() const      { return "enum"; }
string definition_function::kind() const  { return "function"; }
string definition_hypothetical::kind() const { return "dependent object"; }
string definition_overload::kind() const  { return "function"; }
string definition_scope::kind() const     { return "namespace"; }
string definition_template::kind() const  { return "template"; }
string definition_tempparam::kind() const { return "template parameter"; }
string definition_typed::kind() const     { return "object"; }
string definition_union::kind() const     { return "union"; }
string definition_valued::kind() const    { return "constant"; }

static map<int, string> flagnamemap;
string flagnames(unsigned flags) {
  if (flagnamemap.empty()) {
    unsigned d = DEF_TYPENAME;
    switch (d) {
      case DEF_TYPENAME:     flagnamemap[DEF_TYPENAME]     = "DEF_TYPENAME";     // Fallthrough
      case DEF_NAMESPACE:    flagnamemap[DEF_NAMESPACE]    = "DEF_NAMESPACE";    // Fallthrough
      case DEF_CLASS:        flagnamemap[DEF_CLASS]        = "DEF_CLASS";        // Fallthrough
      case DEF_ENUM:         flagnamemap[DEF_ENUM]         = "DEF_ENUM";         // Fallthrough
      case DEF_UNION:        flagnamemap[DEF_UNION]        = "DEF_UNION";        // Fallthrough
      case DEF_SCOPE:        flagnamemap[DEF_SCOPE]        = "DEF_SCOPE";        // Fallthrough
      case DEF_TYPED:        flagnamemap[DEF_TYPED]        = "DEF_TYPED";        // Fallthrough
      case DEF_FUNCTION:     flagnamemap[DEF_FUNCTION]     = "DEF_FUNCTION";     // Fallthrough
      case DEF_OVERLOAD:     flagnamemap[DEF_OVERLOAD]     = "DEF_OVERLOAD";     // Fallthrough
      case DEF_VALUED:       flagnamemap[DEF_VALUED]       = "DEF_VALUED";       // Fallthrough
      case DEF_EXTERN:       flagnamemap[DEF_EXTERN]       = "DEF_EXTERN";       // Fallthrough
      case DEF_TEMPLATE:     flagnamemap[DEF_TEMPLATE]     = "DEF_TEMPLATE";     // Fallthrough
      case DEF_TEMPPARAM:    flagnamemap[DEF_TEMPPARAM]    = "DEF_TEMPPARAM";    // Fallthrough
      case DEF_HYPOTHETICAL: flagnamemap[DEF_HYPOTHETICAL] = "DEF_HYPOTHETICAL"; // Fallthrough
      case DEF_DEPENDENT:    flagnamemap[DEF_DEPENDENT]    = "DEF_DEPENDENT";    // Fallthrough
      case DEF_PRIVATE:      flagnamemap[DEF_PRIVATE]      = "DEF_PRIVATE";      // Fallthrough
      case DEF_PROTECTED:    flagnamemap[DEF_PROTECTED]    = "DEF_PROTECTED";    // Fallthrough
      case DEF_INCOMPLETE:   flagnamemap[DEF_INCOMPLETE]   = "DEF_INCOMPLETE";   // Fallthrough
      case DEF_ATOMIC:       flagnamemap[DEF_ATOMIC]       = "DEF_ATOMIC";       // Fallthrough
      default: ;
    }
  }
  string res;
  bool hadone = false;
  for (int i = 1; i < (1 << 30); i <<= 1)
    if (flags & i)
      res += (hadone? " | " : "") + flagnamemap[i], hadone = true;
  return res;
}

}  // namespace jdi

#ifdef CUSTOM_MEMORY_MANAGEMENT
struct memblock {
  char data[sizeof(jdi::definition) << 7];
  char *atpt;
  memblock *next;
  static void *getnew(size_t sz);
  memblock(): atpt(data), next(nullptr) {}
  ~memblock() { delete next; }
} first, *atmem = &first;

void *memblock::getnew(size_t sz) {
  char *np = atmem->atpt + sz;
  if (np < (char*)&atmem->atpt) {
    char *res = atmem->atpt;
    atmem->atpt = np;
    return res;
  }
  atmem = atmem->next = new memblock();
  return getnew(sz);
}

void *jdi::definition::operator new(size_t sz) {
  return atmem->getnew(sz);
}
void jdi::definition::operator delete(void*) {}
#endif
