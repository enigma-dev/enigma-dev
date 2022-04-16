/**
 * @file  definition_duplicate.cpp
 * @brief System source implementing duplicate and remap components of the
 *        definition storage class.
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
#include <General/utils.h>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::make_unique;

namespace jdi {

void definition_scope::copy(const definition_scope* from, remap_set &n) {
  for (defiter_c it = from->members.begin(); it != from->members.end(); it++)
  #ifdef DEBUG_MODE
  if (it->second == nullptr) {
    cerr << "COMPLETE FAILURE. `" << it->first << "' in `" << from->name << "' is nullptr!" << endl;
    abort();
  } else
  #endif
  if (!(it->second->flags & (DEF_CLASS | DEF_ENUM | DEF_UNION))) {
    inspair dest = members.insert(entry(it->first, nullptr));
    if (dest.second) {
      dest.first->second = it->second->duplicate(n);
    }
  }
  for (defiter_c it = from->c_structs.begin(); it != from->c_structs.end(); ++it) {
    inspair dest = c_structs.insert(entry(it->first, nullptr));
    if (dest.second) {
      dest.first->second = it->second->duplicate(n);
    }
  }
  dec_order = from->dec_order;
  for (orditer it = dec_order.begin(); it != dec_order.end(); ++it) {
    definition *def = (*it)->second.get();
    remap_iter rit = n.find(def);
    if (rit == n.end() && !(def->flags & DEF_HYPOTHETICAL))
      cerr << "FAILURE! Definition `" << def->name << "' in the order deque was not found in the remap set!" << endl;
    if (auto p = members.find(def->name); p != members.end()) {
      *it = p;
    } else if (auto ps = c_structs.find(def->name); ps != c_structs.end()) {
      *it = ps;
    } else {
      cerr << "OMG we don't actually own this definition." << endl;
    }
  }
  using_scopes = from->using_scopes;
  using_general = from->using_general;
}

//========================================================================================================
//======: Duplicators :===================================================================================
//========================================================================================================

template<class d> unique_ptr<d> dup(const unique_ptr<d> &x) {
  return x? x->duplicate() : nullptr;
}
template<class d> decltype(std::declval<d>().duplicate())
dup(const d *x) { return x? x->duplicate() : nullptr; }

unique_ptr<definition> definition::duplicate(remap_set &n) const {
  auto res = make_unique<definition>(name, parent, flags);
  n[this] = res.get();
  return res;
}

unique_ptr<definition> definition_class::duplicate(remap_set &n) const {
  auto res = make_unique<definition_class>(name, parent, flags);
  res->definition_scope::copy(this, n);
  res->ancestors = ancestors;
  n[this] = res.get();
  return res;
}

unique_ptr<definition> definition_enum::duplicate(remap_set &n) const {
  auto res = make_unique<definition_enum>(name, parent, flags);
  res->type = type;
  res->constants.reserve(constants.size());
  for (const auto &c : constants) {
    auto dd = c.def->duplicate(n);
    auto *dv = (definition_valued*) dd.get();
    decpair dp = res->declare(dv->name, std::move(dd));
    if (!dp.inserted) {
      cerr << "ERROR: enum member " << c.def->name
           << " in new definition is already mapped! This is a program error!"
           << endl;
    }
    res->constants.emplace_back(dv, dup(c.ast));
  }
  n[this] = res.get();
  return res;
}

unique_ptr<definition> definition_valued::duplicate(remap_set &n) const {
  auto res = make_unique<definition_valued>(name, parent, type, modifiers, flags, value_of);
  n[this] = res.get();
  return res;
}

unique_ptr<definition> definition_overload::duplicate(remap_set &n) const {
  ref_stack dup; dup.copy(referencers);
  auto res = make_unique<definition_overload>(name, parent, type, dup, modifiers, flags);
  n[this] = res.get();
  return res;
}

unique_ptr<definition> definition_function::duplicate(remap_set &n) const {
  auto res = make_unique<definition_function>(name, parent, flags);
  for (auto it = overloads.begin(); it != overloads.end(); ++it) {
    res->overloads.insert({
        it->first, cast_unique<definition_overload>(it->second->duplicate(n))});
  }
  n[this] = res.get();
  return res;
}

unique_ptr<definition> definition_scope::duplicate(remap_set &n) const {
  auto res = make_unique<definition_scope>(name, parent, flags);
  res->copy(this, n);
  n[this] = res.get();
  return res;
}

unique_ptr<definition> definition_template::duplicate(remap_set &n) const {
  auto res = make_unique<definition_template>(name, parent, flags);

  res->def = def->duplicate(n);
  res->specializations.reserve(specializations.size());
  res->params.reserve(params.size());

  for (const auto &param : params) {
    res->params.push_back(
        cast_unique<definition_tempparam>(param->duplicate(n)));
  }

  for (const auto &spec : specializations) {
    res->specializations.emplace_back(make_unique<specialization>(
        *spec,
        cast_unique<definition_template>(spec->spec_temp->duplicate(n))));
  }

  for (const auto &key_inst : instantiations) {
    instantiation *ninst = (res->instantiations[key_inst.first] =
        make_unique<instantiation>()).get();
    ninst->def = key_inst.second->def->duplicate(n);
    for (const auto &pdef : key_inst.second->parameter_defs) {
      ninst->parameter_defs.push_back(pdef->duplicate(n));
    }
  }

  n[this] = res.get();
  return res;
}

unique_ptr<definition> definition_tempparam::duplicate(remap_set &n) const {
  auto res = make_unique<definition_tempparam>(name, parent, flags);
  res->integer_type.copy(integer_type);
  res->default_assignment = dup(default_assignment);
  res->definition_class::copy(this, n);
  n[this] = res.get();
  return res;
}

unique_ptr<definition> definition_typed::duplicate(remap_set &n) const {
  unique_ptr<definition_typed> res =
      make_unique<definition_typed>(name, parent, type, modifiers, flags);
  res->referencers.copy(referencers);
  n[this] = res.get();
  return res;
}

unique_ptr<definition> definition_union::duplicate(remap_set &n) const {
  auto res = make_unique<definition_union>(name, parent, flags);
  res->definition_scope::copy(this, n);
  n[this] = res.get();
  return res;
}

unique_ptr<definition> definition_atomic::duplicate(remap_set &) const {
  // There can only be one... or whatever. Just don't add ourself to the map.
  cerr << "Who is calling duplicate on a primitive type?" << endl;
  return nullptr;
}

unique_ptr<definition> definition_hypothetical::duplicate(remap_set &n) const {
  auto res = make_unique<definition_hypothetical>(name, parent, flags,
                                                  def->duplicate());
  n[this] = res.get();
  return res;
}


//========================================================================================================
//======: Re-map Functions :==============================================================================
//========================================================================================================

#ifdef DEBUG_MODE
  #define DEBUG_ONLY(x) x
#else
  #define DEBUG_ONLY(x)
#endif

template<class dc> inline dc *filter(dc *x, const remap_set &remap) {
  remap_citer it = remap.find(x);
  return it == remap.end()? x : (dc*)it->second;
}

void definition::remap(remap_set &n, const ErrorContext &) {
  parent = filter(parent, n);
}

void definition_scope::remap(remap_set &n, const ErrorContext &errc) {
  definition::remap(n, errc);
  // cout << "Scope `" << name << "' has " << dec_order.size() << " ordered members" << endl;
  for (orditer it = dec_order.begin(); it != dec_order.end(); ++it) {
    definition *def = (*it)->second.get();
    remap_set::const_iterator ex = n.find(def);
    if (ex != n.end()) {
      errc.error() << "Request to change own member from " << def
                   << " to " << ex->second;
    } else {
      def->remap(n, errc);
    }
  }
  for (defiter it = members.begin(); it != members.end(); ++it) {
    remap_set::const_iterator ex = n.find(it->second.get());
    if (ex != n.end()) {
      errc.error() << "Request to change own member from " << it->second.get()
                   << " to " << ex->second;
    }
  }
  for (definition_scope *use : using_scopes) {
    remap_set::const_iterator ex = n.find(use);
    if (ex == n.end())
      use = (definition_scope*)ex->second;
  }
  for (auto &defpair : using_general) {
    if (remap_set::const_iterator ex = n.find(defpair.second); ex != n.end())
      defpair.second = ex->second;
  }
}

void definition_class::remap(remap_set &n, const ErrorContext &errc) {
  definition_scope::remap(n, errc);
  for (vector<ancestor>::iterator it = ancestors.begin(); it != ancestors.end(); ++it) {
    ancestor& an = *it;
    remap_set::const_iterator ex = n.find(an.def);
    if (ex != n.end()) {
      definition *ancc = ex->second;
      for (;;)
        if ((ancc->flags & (DEF_TYPED | DEF_TYPENAME)) == (DEF_TYPED | DEF_TYPENAME)) {
          full_type r = ancc; r.reduce();
          ancc = r.def;
        }
        else if (ancc->flags & DEF_DEPENDENT) {
          ex = n.find(ancc);
          if (ex != n.end())
            ancc = ex->second;
          else if ((ancc->remap(n, errc), ex = n.find(ancc)) != n.end())
            ancc = ex->second;
          else {
            errc.warning() << "Can't fully evaluate ancestor for class "
                           << PQuote(name) << ": no resolution for " << *ancc;
            break;
          }
        }
        else break;

      if (!ancc) {
        errc.error() << "ERROR! Replacing ancestor " << PQuote(an.def->name)
                     << " with bad typedef!";
      } else if (not(ancc->flags & DEF_CLASS)) {
        errc.error() << "ERROR! Replacing ancestor " << PQuote(an.def->name)
                     << " with non-class " << PQuote(ancc->name)
                     << " (" << flagnames(ancc->flags) << ")";
      } else {
        an.def = (definition_class*)ancc;
      }
    }
  }
  for (set<definition*>::iterator fiter = friends.begin(); fiter != friends.end(); ) {
    definition *rep = filter(*fiter, n);
    if (rep != *fiter) {
      friends.erase(fiter++);
      friends.insert(rep);
    }
    else ++fiter;
  }
}

void definition_enum::remap(remap_set& n, const ErrorContext &errc) {
  #ifdef DEBUG_MODE
  if (n.find(type) != n.end()) {
    errc.warning() << "Why are you replacing " << PQuote(type->name) << "?";
  }
  #endif
  type = filter(type, n);

  for (vector<const_pair>::iterator it = constants.begin(); it != constants.end(); ++it) {
    definition_valued *d = filter(it->def, n);
    if (it->def == d) {
      it->def->remap(n, errc);
      if (it->ast) { // FIXME: This doesn't keep counting after a successful remap!
        value cache = it->def->value_of;
        it->ast->remap(n, errc);
        it->def->value_of = it->ast->eval(errc);
        if (it->def->value_of.type == VT_DEPENDENT)
          errc.warning("Enumeration value still dependent after remap");
        else {
          cout << "Refactored " << it->ast->toString() << ": "
               << cache.toString() << " → " << it->def->value_of.toString()
               << endl;
          it->ast = nullptr;
        }
      }
    }
    else {
      delete it->def;
      it->def = d;
    }
  }
}

void definition_function::remap(remap_set& n, const ErrorContext &errc) {
  definition::remap(n, errc);
  for (overload_iter it = overloads.begin(); it != overloads.end(); ++it) {
    remap_citer rit = n.find(it->second.get());
    if (rit != n.end()) {
      // it->second = (definition_overload*)rit->second;
      cerr << "Attempt to replace owned overload with a different pointer!" << endl;
    } else {
      it->second->remap(n, errc);
    }
  }
}

void definition_overload::remap(remap_set& n, const ErrorContext &errc) {
  definition_typed::remap(n, errc);
  // TODO: remap_function_implementation();
}

void definition_template::remap(remap_set &n, const ErrorContext &errc) {
  for (auto it = params.begin(); it != params.end(); ++it) {
    if ((*it)->default_assignment)
      (*it)->default_assignment->remap(n, errc);
  }
  for (institer it = instantiations.begin(); it != instantiations.end(); ++it)
    it->second->def->remap(n, errc);
  for (speciter it = specializations.begin(); it != specializations.end(); ++it)
    (*it)->spec_temp->remap(n, errc);
  if (def)
    def->remap(n, errc);
}

void definition_tempparam::remap(remap_set&, const ErrorContext &errc) {
  // TODO: Implement
  errc.warning("Not implemented: tempparam::remap");
}

void definition_typed::remap(remap_set &n, const ErrorContext &errc) {
  remap_set::const_iterator ex = n.find(type);
  if (ex != n.end())
    type = ex->second;
  /*else {
    type->remap(n, errc);
    if ((ex = n.find(type)) != n.end())
      type = ex->second;
  }*/
  if (!(type->flags & DEF_DEPENDENT))
    flags &= ~DEF_DEPENDENT;
  definition::remap(n, errc);
}

void definition_union::remap(remap_set &n, const ErrorContext &errc) {
  definition_scope::remap(n, errc);
}

void definition_atomic::remap(remap_set &n, const ErrorContext &errc) { definition::remap(n, errc); }

void definition_hypothetical::remap(remap_set &n, const ErrorContext &errc) {
  def->remap(n, errc);
  full_type ft = def->coerce(errc);

  if (ft.def && ft.def != arg_key::abstract) {
    #ifdef DEBUG_MODE
      if (ft.refs.size() || ft.flags)
        cerr <<
            "Coerced refstack somehow has additional info attached! Discarded!"
             << endl << endl << endl;
    #endif
    n[(definition*)this] = ft.def;
  }
}

void arg_key::remap(const remap_set &r, const ErrorContext &errc) {
  for (node* n = values; n != endv; ++n)
    if (n->type == AKT_FULLTYPE) {
      n->ft().def = filter(n->ft().def, r);
      n->ft().reduce();
    }
    else if (n->av().ast) {
      AST &a = *n->av().ast;
      a.remap(r, errc);
      value v = a.eval(errc);
      if (v.type != VT_DEPENDENT) {
        n->val() = std::move(v);
        n->av().ast = nullptr;
      }
      else {
        errc.error() << "No dice in unrolling template expression\n"
                     << "evaluated expression: " << a;
      }
    }
}


//========================================================================================================
//======: AST Node Duplicate Functions :==================================================================
//========================================================================================================

unique_ptr<AST> AST::duplicate() const {
  return make_unique<AST>(root->duplicate());
}

unique_ptr<AST_Node> AST_Node::duplicate() const {
  return make_unique<AST_Node>(content, type);
}
unique_ptr<AST_Node> AST_Node_Scope::duplicate() const {
  return make_unique<AST_Node_Scope>(dup(left), dup(right), content);
}
unique_ptr<AST_Node> AST_Node_Unary::duplicate() const {
  return make_unique<AST_Node_Unary>(dup(operand), content, prefix,
                                     AST_Node::type);
}
unique_ptr<AST_Node> AST_Node_sizeof::duplicate() const {
  return make_unique<AST_Node_sizeof>(dup(operand), negate);
}
unique_ptr<AST_Node> AST_Node_Definition::duplicate() const {
  return make_unique<AST_Node_Definition>(def, content);
}
unique_ptr<AST_Node> AST_Node_Type::duplicate() const {
  full_type dt(dec_type);
  return make_unique<AST_Node_Type>(dt);
}
unique_ptr<AST_Node> AST_Node_Cast::duplicate() const {
  return make_unique<AST_Node_Cast>(dup(operand), cast_type);
}
unique_ptr<AST_Node> AST_Node_Binary::duplicate() const {
  return make_unique<AST_Node_Binary>(dup(left), dup(right), content);
}
unique_ptr<AST_Node> AST_Node_Ternary::duplicate() const {
  return make_unique<AST_Node_Ternary>(dup(exp), dup(left), dup(right),
                                       content);
}
unique_ptr<AST_Node> AST_Node_new::duplicate() const {
  return make_unique<AST_Node_new>(alloc_type, dup(position), dup(bound));
}
unique_ptr<AST_Node> AST_Node_delete::duplicate() const {
  return make_unique<AST_Node_delete>(dup(operand), array);
}
unique_ptr<AST_Node> AST_Node_Subscript::duplicate() const {
  return make_unique<AST_Node_Subscript>(dup(left), dup(index));
}

unique_ptr<AST_Node> AST_Node_Parameters::duplicate() const {
  auto res = make_unique<AST_Node_Parameters>();
  res->func = dup(func);
  res->params.reserve(params.size());
  for (auto p = params.begin(); p != params.end(); ++p)
    res->params.push_back(dup(*p));
  return res;
}
unique_ptr<AST_Node> AST_Node_TempInst::duplicate() const {
  auto res = make_unique<AST_Node_TempInst>(temp->duplicate(), content);
  res->params.reserve(params.size());
  for (auto p = params.begin(); p != params.end(); ++p)
    res->params.push_back(dup(*p));
  return res;
}
unique_ptr<AST_Node> AST_Node_TempKeyInst::duplicate() const {
  return make_unique<AST_Node_TempKeyInst>(temp, key);
}
unique_ptr<AST_Node> AST_Node_Array::duplicate() const {
  auto res = make_unique<AST_Node_Array>();
  res->elements.reserve(elements.size());
  for (auto e = elements.begin(); e != elements.end(); ++e)
    res->elements.push_back(dup(*e));
  return res;
}



//========================================================================================================
//======: AST Node Re-map Functions :=====================================================================
//========================================================================================================

template<class c> void nremap(const unique_ptr<c> &x, const remap_set &n,
                              ErrorContext errc) {
  if (x)
    x->remap(n, errc);
  else
    errc.error() << "Why is this null?";
}

void AST_Node::remap(const remap_set&, ErrorContext) {}
void AST_Node_Scope::remap(const remap_set& n, ErrorContext errc) {
  AST_Node_Binary::remap(n, errc);
}
void AST_Node_Unary::remap(const remap_set& n, ErrorContext errc) {
  nremap(operand, n, errc);
}
void AST_Node_sizeof::remap(const remap_set& n, ErrorContext errc) {
  nremap(operand, n, errc);
}
void AST_Node_Definition::remap(const remap_set& n, ErrorContext) {
  def = filter(def, n);
}
void AST_Node_Type::remap(const remap_set& n, ErrorContext) {
  dec_type.def  = filter(dec_type.def,  n);
}
void AST_Node_Cast::remap(const remap_set& n, ErrorContext) {
  cast_type.def = filter(cast_type.def, n);
}
void AST_Node_Binary::remap(const remap_set& n, ErrorContext errc) {
  nremap(left, n, errc);
  nremap(right, n, errc);
}
void AST_Node_Ternary::remap(const remap_set& n, ErrorContext errc) {
  nremap(left, n, errc);
  nremap(right, n, errc);
  nremap(exp, n, errc);
}
void AST_Node_new::remap(const remap_set& n, ErrorContext errc) {
  alloc_type.def = filter(alloc_type.def, n);
  nremap(position, n, errc);
  nremap(bound, n, errc);
}
void AST_Node_delete::remap(const remap_set& n, ErrorContext errc) {
  AST_Node_Unary::remap(n, errc);
}
void AST_Node_Subscript::remap(const remap_set& n, ErrorContext errc) {
  nremap(left, n, errc);
  nremap(index, n, errc);
}

void AST_Node_Parameters::remap(const remap_set& n, ErrorContext errc) {
  nremap(func, n, errc);
  for (auto p = params.begin(); p != params.end(); ++p)
    (*p)->remap(n, errc);
}
void AST_Node_TempInst::remap(const remap_set& n, ErrorContext errc) {
  temp->remap(n, errc);
  for (auto p = params.begin(); p != params.end(); ++p)
    (*p)->remap(n, errc);
}
void AST_Node_TempKeyInst::remap(const remap_set& n, ErrorContext errc) {
  temp = filter(temp, n);
  key.remap(n, errc);
}
void AST_Node_Array::remap(const remap_set& n, ErrorContext errc) {
  for (auto e = elements.begin(); e != elements.end(); ++e)
    (*e)->remap(n, errc);
}

}
