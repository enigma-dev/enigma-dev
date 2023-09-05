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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "definition.h"
#include <iostream>
#include <cstdio>
#include <typeinfo>
#include <algorithm>
#include <System/builtins.h>
#include <Parser/handlers/handle_function_impl.h>
#include <API/compile_settings.h>
#include <Parser/context_parser.h>
#include <Parser/is_potential_constructor.h>
using namespace std;

namespace jdi {
  definition::definition(string n,definition* p,unsigned int f): flags(f), name(n), parent((definition_scope*)p) {}
  definition::definition(): flags(0), name(), parent(NULL) {}
  definition::~definition() {}
  
  ptrdiff_t definition::defcmp(definition *d1, definition *d2) {
    return d1 - d2;
  }
  
  definition_typed::definition_typed(string n, definition* p, definition* tp, unsigned int typeflags, int flgs): definition(n,p,flgs | DEF_TYPED), type(tp), referencers(), modifiers(typeflags) {
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
  definition_typed::~definition_typed() {}
  
  string definition::qualified_id() const {
    if (!parent) return name;
    return parent->qualified_id() + "::" + name;
  }
  
  // NOTICE:
  // This (flgs | DEF_FUNCTION) &~(DEF_PRIVATE | DEF_PROTECTED) stuff is to make sure that definition_function, which is basically a method group,
  // is not itself protected or private. Only specific overloads, which actually have a prototype, may be private or protected.
  // XXX: Should all flags be discarded in this manner?
  
  definition_function::definition_function(string n, definition* p, definition* tp, const ref_stack &rf, unsigned int typeflags, int flgs): 
    definition(n, p, (flgs | DEF_FUNCTION) &~(DEF_PRIVATE | DEF_PROTECTED)) {
    overload(tp, rf, typeflags, flgs, NULL, def_error_handler);
  }
  definition_function::definition_function(string n, definition* p, int flgs): 
    definition(n, p, (flgs | DEF_FUNCTION) &~(DEF_PRIVATE | DEF_PROTECTED)) {}
    
  definition_overload::definition_overload(string n, definition *p, definition* tp, const ref_stack &rf, unsigned int typeflags, int flgs): 
    definition_typed(n, p, tp, rf, typeflags, (flgs & ~(DEF_FUNCTION)) | DEF_OVERLOAD), implementation(NULL) {}
  
  definition_overload *definition_function::overload(definition *tp, const ref_stack &rf, unsigned int typeflags, unsigned int addflags, void *implementation, error_handler *herr) {
    arg_key key(rf);
    pair<overload_iter, bool> ins = overloads.insert(pair<arg_key,definition_overload*>(key, NULL));
    if (!ins.second) {
      if (implementation) {
        if (ins.first->second->implementation) {
          herr->error("Reimplementation of function; old implementation discarded");
          delete_function_implementation(ins.first->second->implementation);
          FATAL_RETURN(NULL);
          ins.first->second->implementation = implementation;
        }
        ins.first->second->implementation = implementation;
      }
    }
    else
      ins.first->second = new definition_overload(name, parent, tp, rf, typeflags, flags | addflags);
    
    return ins.first->second;
  }
  definition_overload *definition_function::overload(const full_type &ft, unsigned int addflags, error_handler *herr) {
    return overload(ft.def, ft.refs, ft.flags, addflags, NULL, herr);
  }
  
  void definition_function::overload(definition_template* ovrl, error_handler *) {
    // FIXME: This could push a forward declaration and an actual implementation
    template_overloads.push_back(ovrl);
  }
  
  definition_overload* definition_scope::overload_function(string fname, full_type &ft, unsigned inflags, const jdip::token_t& errtok, error_handler *herr) {
    definition_function *df;
    definition_overload *dov;
    decpair dp = declare(fname, NULL);
    if (dp.inserted)
      dp.def = df = new definition_function(fname, this, inflags);
    else {
      if (dp.def->flags & DEF_FUNCTION)
        df = (definition_function*)dp.def;
      else {
        errtok.report_error(herr, "Cannot redeclare `" + fname + "' as function in this scope");
        return NULL;
      }
    }
    
    dov = df->overload(ft, inflags, herr);
    return dov;
  }
  
  definition_function* definition_scope::overload_function(string fname, definition_template* dtemp, unsigned inflags, const jdip::token_t& errtok, error_handler *herr) {
    definition_function *df;
    decpair dp = declare(fname, NULL);
    if (dp.inserted)
      dp.def = df = new definition_function(fname, this, inflags);
    else {
      if (dp.def->flags & DEF_FUNCTION)
        df = (definition_function*)dp.def;
      else {
        errtok.report_error(herr, "Cannot redeclare `" + fname + "' as function in this scope");
        return NULL;
      }
    }
    
    df->overload(dtemp, herr);
    return df;
  }
  
  definition_function::~definition_function() {
    for (overload_iter it = overloads.begin(); it != overloads.end(); ++it)
      delete it->second;
    for (vector<definition_template*>::iterator it = template_overloads.begin(); it != template_overloads.end(); ++it)
      delete *it;
  }
  
  decpair definition_scope::declare_c_struct(string n, definition* def) {
    pair<defiter, bool> insp = c_structs.insert(pair<string,definition*>(n,def));
    dec_order.push_back(new dec_order_defiter(insp.first));
    return decpair(&insp.first->second, insp.second);
  }
  definition *definition_scope::look_up(string sname) {
    defiter it = members.find(sname);
    if (it != members.end())
      return it->second;
    //if (members.size() == 1)
    //  cout << sname << ": Not my member: " << members.begin()->first << " => " << members.begin()->second->toString() << endl;
    if ((it = using_general.find(sname)) != using_general.end())
      return it->second;
    /*if (members.size() == 1) {
      cout << "Not in my using: {" << endl;
      for (defiter u = using_general.begin(); u != using_general.end(); ++u)
        cout << u->second->toString() << endl;
      cout << "}" << endl;
    }*/
    definition *res;
    for (using_node* n = using_front; n; n = n->next)
      if ((res = n->use->find_local(sname)))
        return res;
    if (parent == NULL)
      return NULL;
    return parent->look_up(sname);
  }
  definition *definition_class::look_up(string sname) {
    defiter it = members.find(sname);
    if (it != members.end())
      return it->second;
    if ((it = using_general.find(sname)) != using_general.end())
      return it->second;
    definition *res;
    for (using_node* n = using_front; n; n = n->next)
      if ((res = n->use->find_local(sname)))
        return res;
    for (vector<ancestor>::iterator ait = ancestors.begin(); ait != ancestors.end(); ++ait)
      if ((res = ait->def->find_local(sname)))
        return res;
    if (parent == NULL)
      return NULL;
    return parent->look_up(sname);
  }
  definition *definition_scope::find_local(string sname) {
    defiter it = members.find(sname);
    if (it != members.end())
      return it->second;
    if ((it = using_general.find(sname)) != using_general.end())
      return it->second;
    bool a = false;
    if (a) {
      cout << name << "::using {" << endl;
      for (it = using_general.begin(); it != using_general.end(); ++it) cout << "  " << it->first << " => " << it->second->name << endl;
      cout << "}" << endl;
    }
    definition *res;
    for (using_node* n = using_front; n; n = n->next)
      if ((res = n->use->find_local(sname)))
        return res;
    return NULL;
  }
  definition *definition_class::find_local(string sname) {
    definition *res = definition_scope::find_local(sname);
    if (res) return res;
    for (vector<ancestor>::iterator ait = ancestors.begin(); ait != ancestors.end(); ++ait)
      if ((res = ait->def->find_local(sname)))
        return res;
    return NULL;
  }
  
  definition *definition_tempparam::look_up(string sname) {
    must_be_class = true;
    return definition_class::look_up(sname);
  }
  decpair definition_tempparam::declare(string sname, definition *def) {
    must_be_class = true;
    return definition_class::declare(sname, def);
  }
  definition *definition_scope::get_local(string n) {
    return find_local(n);
  }
  definition *definition_tempparam::get_local(string sname) {
    must_be_class = true;
    pair<defmap::iterator, bool> insp = members.insert(defmap::value_type(sname, NULL));
    if (insp.second) {
      insp.first->second = new definition_tempparam(sname, this);  
    }
    return insp.first->second;
  }
  definition *definition_class::get_local(string sname) {
    defiter it;
    definition *res = definition_scope::get_local(sname);
    if (!res && (sname == name or (instance_of && sname == instance_of->name)) and (it = members.find(constructor_name)) != members.end())
      res = it->second;
    return res;
  }
  definition *definition_hypothetical::get_local(string sname) {
    required_flags |= DEF_CLASS;
    return def? context_p->handle_hypothetical_access(this, sname) : NULL;
  }
  
  definition_scope::using_node *definition_scope::use_namespace(definition_scope *ns) {
    using_node *res;
    if (using_back)
      res = using_back = new using_node(ns, using_back);
    else
      res = using_front = using_back = new using_node(ns);
    return res;
  }
  void definition_scope::unuse_namespace(definition_scope::using_node *n) {
    if (using_back == n) using_back = n->prev;
    if (using_front == n) using_front = n->next;
    if (n->next) n->next->prev = n->prev;
    if (n->prev) n->prev->next = n->next;
    delete n;
  }
  
  void definition_scope::use_general(string n, definition *def) {
    using_general.insert(pair<string,definition*>(n,def));
  }
  
  definition_scope::definition_scope(): definition("",NULL,DEF_SCOPE), using_front(NULL), using_back(NULL) { }
  definition_scope::definition_scope(const definition_scope&): definition() {
    // TODO: Implement
  }
  definition_scope::definition_scope(string name_, definition *parent_, unsigned int flags_): definition(name_,parent_,flags_ | DEF_SCOPE), using_front(NULL), using_back(NULL) {}
  definition_scope::~definition_scope() {
    for (defiter it = members.begin(); it != members.end(); it++)
      if (!(it->second->flags & (DEF_ENUM | DEF_CLASS | DEF_UNION)))
      delete it->second;
    for (using_node *n = using_front; n; ) {
      using_node *dm = n; n = n->next;
      delete dm;
    }
    for (orditer it = dec_order.begin(); it != dec_order.end(); ++it)
      delete *it;
    members.clear();
    for (defiter it = c_structs.begin(); it != c_structs.end(); ++it)
      delete it->second;
  }
  definition_scope::using_node::using_node(definition_scope* scope): use(scope), next(NULL), prev(NULL) { }
  definition_scope::using_node::using_node(definition_scope* scope, using_node* nprev): use(scope), next(nprev->next), prev(nprev) { nprev->next = this; }
  
  definition_enum::~definition_enum() {
    for (vector<const_pair>::iterator it = constants.begin(); it != constants.end(); ++it)
      delete it->ast;
  }
  
  definition_class::ancestor::ancestor(unsigned protection_level, definition_class* inherit_from): protection(protection_level), def(inherit_from) {}
  definition_class::ancestor::ancestor() {}
  definition_class::definition_class(string classname, definition_scope* prnt, unsigned flgs): definition_scope(classname, prnt, flgs), instance_of(NULL) {}
  
  definition_union::definition_union(string classname, definition_scope* prnt, unsigned flgs): definition_scope(classname, prnt, flgs) {}
  
  //definition_valued::definition_valued(string vname, definition *parnt, definition* tp, unsigned int flgs, value &val): definition_typed(vname, parnt, tp, 0, flgs | DEF_VALUED), value_of(val) {}
  definition_valued::definition_valued(string vname, definition *parnt, definition* tp, unsigned tflgs, unsigned int flgs, const value &val): definition_typed(vname, parnt, tp, tflgs, flgs | DEF_VALUED), value_of(val) {}
  
  definition_enum::definition_enum(string classname, definition_scope* parnt, unsigned flgs): definition_class(classname, parnt, flgs | DEF_ENUM) {}
  
  definition_template::definition_template(string n, definition *p, unsigned f): definition_scope(n, p, f | DEF_TEMPLATE), def(NULL)  {}
  definition_template::~definition_template() {
    for (size_t i = 0; i < params.size(); ++i)
      delete params[i];
    for (speciter i = specializations.begin(); i != specializations.end(); ++i)
      delete *i;
    for (depiter i = dependents.begin(); i != dependents.end(); ++i)
      delete *i;
    for (institer i = instantiations.begin(); i != instantiations.end(); ++i)
      delete i->second;
    delete def;
  }
  
  definition* definition_template::dec_order_hypothetical::def() { return hyp; }
  
  definition_template::instantiation::~instantiation() {
    delete def;
    for (vector<definition*>::iterator it = parameter_defs.begin(); it != parameter_defs.end(); ++it)
      delete *it;
  }
  
  definition_template::specialization::specialization(const specialization &x): key(x.key, false), spec_temp(x.spec_temp) {}
  definition_template::specialization::~specialization() { delete spec_temp; }
  
  definition_tempparam::definition_tempparam(string p_name, definition_scope* p_parent, unsigned p_flags): definition_class(p_name, p_parent, p_flags | DEF_TEMPPARAM | DEF_DEPENDENT), default_assignment(NULL) {}
  definition_tempparam::definition_tempparam(string p_name, definition_scope* p_parent, AST* defval, unsigned p_flags): definition_class(p_name, p_parent, p_flags | DEF_TEMPPARAM | DEF_DEPENDENT), default_assignment(defval) {}
  definition_tempparam::~definition_tempparam() { delete default_assignment; }
  
  static int nest_count = 0;
  struct nest_ { nest_() { ++nest_count; } ~nest_() { --nest_count; } };
  definition* definition_template::instantiate(const arg_key& key, const error_context &errc) {
    if (nest_count >= 128) {
      cerr << "Maximum nested template depth of 128 (GCC default) exceeded. Bailing." << endl;
      return NULL;
    }
    nest_ this_nest;
    
    // TODO: Move this specialization search into the not-found if (ins.second) below, then add the specialization to the instantiation map.
    // TODO: Be careful not to double free those specializations. You may need to keep a separate map for regular instantiations to free.
    specialization *spec = find_specialization(key);
    if (spec) {
      arg_key speckey = spec->key.get_key(key);
      return spec->spec_temp->instantiate(speckey, errc);
    }
    
    pair<institer, bool> ins = instantiations.insert(pair<arg_key, instantiation*>(key, NULL));
    if (ins.second) {
      //cout << "Instantiating new " << name << "<" << key.toString() << "> (abstract: " << key.is_abstract() << ")" << endl;
      remap_set n;
      size_t ind = 0;
      definition *ntemp = def->duplicate(n);
      ntemp->name += "<" + key.toString() + ">";
      if (ntemp->flags & DEF_CLASS)
        ((definition_class*)ntemp)->instance_of = this;
      else
        cout << "Not a class lol" << endl;
      ins.first->second = new instantiation();
      ins.first->second->def = ntemp;
      for (piterator it = params.begin(); it != params.end(); ++it) {
        definition *ndef = key.new_definition(ind++, (*it)->name, this);
        ins.first->second->parameter_defs.push_back(ndef);
        n[*it] = ndef;
      }
      size_t keyc = key.size();
      if (keyc != params.size()) {
        errc.report_error("Attempt to instantiate template with an incorrect number of parameters; passed " + value(long(key.end() - key.begin())).toString() + ", required " + value(long(params.size())).toString());
        FATAL_RETURN(NULL);
      }
      
      ntemp->remap(n, errc);
    }
    
    definition *d = ins.first->second->def;
    return d;
  }
  
  definition_template::specialization *definition_template::find_specialization(const arg_key &key) const
  {
    specialization *spec = NULL;
    int merit = 0;
    
    for (speclist::const_iterator i = specializations.begin(); i != specializations.end(); ++i) {
      if (!(*i)->filter.matches(key))
        continue;
      int m = (*i)->key.merit(key);
      if (m > merit) {
        spec = *i;
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
      int matchc = *arg_inds[i]; // The number of arguments this is matching
      int o = arg_inds[i][1];
      for (size_t j = 1; j <= *arg_inds[i]; ++j)
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
  spec_key::spec_key(size_t big_count, size_t small_count): arg_inds(new unsigned*[small_count]), ind_count(small_count)  {
    ++big_count;
    for (size_t i = 0; i < small_count; ++i)
      *(arg_inds[i] = new unsigned[big_count]) = 0;
  }
  
  spec_key::spec_key(const spec_key &k, bool) {
    arg_inds = new unsigned*[k.ind_count];
    for (size_t i = 0; i < k.ind_count; ++i) {
      unsigned nc = *k.arg_inds[i];
      arg_inds[i] = new unsigned[nc + 1];
      for (size_t j = 0; j <= nc; ++j)
        arg_inds[i][j] = k.arg_inds[i][j]; 
    }
    max_param = k.max_param;
    ind_count = k.ind_count;
  }
  
  spec_key::~spec_key() {
    for (size_t i = 0; i < ind_count; ++i)
      delete[] arg_inds[i];
    delete[] arg_inds;
  }
  
  definition_atomic::definition_atomic(string n, definition* p, unsigned int f, size_t size): definition_scope(n,p,f), sz(size) {}
  
  definition_hypothetical::definition_hypothetical(string n, definition_scope *p, unsigned f, AST* d): definition_class(n,p,f | DEF_HYPOTHETICAL | DEF_DEPENDENT), def(d) {}
  definition_hypothetical::definition_hypothetical(string n, definition_scope *p, AST* d): definition_class(n, p, DEF_HYPOTHETICAL | DEF_DEPENDENT), def(d) {}
  definition_hypothetical::~definition_hypothetical() { delete def; }
  
  
  using_scope::using_scope(string n, definition_scope* u): definition_scope(n, u, DEF_NAMESPACE), using_me(u->use_namespace(this)) {}
  using_scope::~using_scope() { parent->unuse_namespace(using_me); }
  
  //========================================================================================================
  //======: Declare Functions :=============================================================================
  //========================================================================================================
  decpair::decpair(definition* *d, bool insd): def(d), inserted(insd) {}
  
  decpair definition_scope::declare(string n, definition* def) {
    inspair insp = members.insert(entry(n,def));
    if (!(def && (def->flags & (DEF_CLASS | DEF_ENUM | DEF_UNION))))
      dec_order.push_back(new dec_order_defiter(insp.first));
    return decpair(&insp.first->second, insp.second);
  }
  decpair definition_class::declare(string n, definition* def) {
    return definition_scope::declare(n, def);
  }
  
  //========================================================================================================
  //======: Sizeof functions :==============================================================================
  //========================================================================================================
  
  value definition::size_of(const error_context &errc) {
    errc.report_warning("Taking size of bare definition");
    return 0L;
  }

  value definition_class::size_of(const error_context &errc) {
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

  value definition_enum::size_of(const error_context &errc) {
    return type->size_of(errc);
  }

  value definition_function::size_of(const error_context &errc) {
    errc.report_error("Computing size of function");
    return 0L;
  }

  value definition_scope::size_of(const error_context &errc) {
    size_t sz = 0;
    for (defiter it = members.begin(); it != members.end(); ++it)
      if (not(it->second->flags & DEF_TYPENAME)) {
        value szadd = it->second->size_of(errc);
        if (szadd.type == VT_INTEGER)
          sz += (long)szadd;
      }
    return (long)sz;
  }

  value definition_template::size_of(const error_context &errc) {
    errc.report_error("Attempt to take size of template `" + name + "'");
    return 0L;
  }

  value definition_typed::size_of(const error_context &errc) {
    return type? type->size_of(errc) : value(0L);
  }

  value definition_union::size_of(const error_context &errc) {
    size_t sz = 0;
    for (defiter it = members.begin(); it != members.end(); ++it)
      if (not(it->second->flags & DEF_TYPENAME))
        sz = max((long)sz, (long)it->second->size_of(errc));
    return (long)sz;
  }
  
  value definition_atomic::size_of(const error_context &) {
    return (long)sz;
  }
  
  value definition_hypothetical::size_of(const error_context &) {
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
    for (vector<definition_template*>::const_iterator it = template_overloads.begin(); it != template_overloads.end(); ++it) {
      if (res.size()) res += "\n";
      res += (*it)->toString(levels, indent);
    }
    return res;
  }
  string definition_scope::toString(unsigned levels, unsigned indent) const {
    string inds(indent, ' '), res = inds;
    if (flags & DEF_NAMESPACE)
      res += name.empty()? "namespace " : "namespace " + name + " ";
    if (levels) {
      res += "{\n";
      for (defiter_c it = members.begin(); it != members.end(); ++it)
        res += it->second->toString(levels-1, indent+2) + "\n";
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
    for (pciterator it = params.begin(); it != params.end(); ++it) {
      definition_tempparam *d = *it;
      if (!first) res += ", ";
      if (d->flags & DEF_TYPENAME) {
        res += d->name.empty()? "typename" : "typename " + d->name;
        if (d->default_assignment)
          res += " = " + d->default_assignment->toString();
      }
      else {
        res += (d->integer_type.def? d->integer_type.toString() : "<ERROR>");
        if (d->flags & DEF_VALUED)
          res += " = " + ((definition_valued*)d)->value_of.toString();
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
    res += type? typeflags_string(type, modifiers) : "<NULL>";
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
        case DEF_TYPENAME:     flagnamemap[DEF_TYPENAME]     = "DEF_TYPENAME";
        case DEF_NAMESPACE:    flagnamemap[DEF_NAMESPACE]    = "DEF_NAMESPACE";
        case DEF_CLASS:        flagnamemap[DEF_CLASS]        = "DEF_CLASS";
        case DEF_ENUM:         flagnamemap[DEF_ENUM]         = "DEF_ENUM";
        case DEF_UNION:        flagnamemap[DEF_UNION]        = "DEF_UNION";
        case DEF_SCOPE:        flagnamemap[DEF_SCOPE]        = "DEF_SCOPE";
        case DEF_TYPED:        flagnamemap[DEF_TYPED]        = "DEF_TYPED";
        case DEF_FUNCTION:     flagnamemap[DEF_FUNCTION]     = "DEF_FUNCTION";
        case DEF_OVERLOAD:     flagnamemap[DEF_OVERLOAD]     = "DEF_OVERLOAD";
        case DEF_VALUED:       flagnamemap[DEF_VALUED]       = "DEF_VALUED";
        case DEF_EXTERN:       flagnamemap[DEF_EXTERN]       = "DEF_EXTERN";
        case DEF_TEMPLATE:     flagnamemap[DEF_TEMPLATE]     = "DEF_TEMPLATE";
        case DEF_TEMPPARAM:    flagnamemap[DEF_TEMPPARAM]    = "DEF_TEMPPARAM";
        case DEF_HYPOTHETICAL: flagnamemap[DEF_HYPOTHETICAL] = "DEF_HYPOTHETICAL";
        case DEF_DEPENDENT:    flagnamemap[DEF_DEPENDENT]    = "DEF_DEPENDENT";
        case DEF_PRIVATE:      flagnamemap[DEF_PRIVATE]      = "DEF_PRIVATE";
        case DEF_PROTECTED:    flagnamemap[DEF_PROTECTED]    = "DEF_PROTECTED";
        case DEF_INCOMPLETE:   flagnamemap[DEF_INCOMPLETE]   = "DEF_INCOMPLETE";
        case DEF_ATOMIC:       flagnamemap[DEF_ATOMIC]       = "DEF_ATOMIC";
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
}

#ifdef CUSTOM_MEMORY_MANAGEMENT
struct memblock {
  char data[sizeof(jdi::definition) << 7];
  char *atpt;
  memblock *next;
  static void *getnew(size_t sz);
  memblock(): atpt(data), next(NULL) {}
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
