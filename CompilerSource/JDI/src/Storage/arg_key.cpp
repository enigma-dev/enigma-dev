#include <cstdio>
#include <iostream>

#include "arg_key.h"
#include "definition.h"
#include <System/builtins.h>

using namespace std;

namespace jdi {
  static definition n_abstract("<unspecified>", nullptr, 0);
  definition *arg_key::abstract = &n_abstract;

  unique_ptr<definition> arg_key::make_definition(size_t index, string name, definition_scope* parent) const {
    if (values[index].type == AKT_FULLTYPE) {
      return make_unique<definition_typed>(name, parent, values[index].ft().def,
                 values[index].ft().refs, values[index].ft().flags,
                 DEF_TYPED | DEF_TYPENAME);
    }
    if (values[index].type == AKT_VALUE) {
      value v = values[index].val();
      definition* type =
          v.type == VT_INTEGER? builtin_type__int    :
          v.type == VT_DOUBLE?  builtin_type__double :
          v.type == VT_STRING?  builtin_type__char   : builtin_type__void;
      return make_unique<definition_valued>(name, parent, type,
                                            0, DEF_VALUED, v);
    }
    return nullptr;
  }

  void arg_key::put_node(size_t argnum, const node &n) {
    if (n.type == AKT_FULLTYPE)
      put_type(argnum, n.ft());
    else
      put_value(argnum, n.val());
  }

  string arg_key::toString() const {
    string str;
    bool c = false;
    for (node* n = values; n != endv; ++n) {
      if (c) str += ", ";
      str += n->type == AKT_FULLTYPE? n->ft().toString() : n->type == AKT_VALUE? n->val().toString() : "<empty>";
      c = true;
    }
    return str;
  }

  bool arg_key::operator<(const arg_key& other) const {
    // cout << "Comparing (" << toString() << ") < (" << other.toString() << ")" << endl;
    {
      int l1 = other.endv - other.values, l2 = endv - values;
      if (l1 != l2)
        return l1 > l2;
    }
    for (arg_key::node *i = values, *j = other.values; j != other.endv; ++i, ++j) {
      if (i == endv) return true;
      if (i->type == AKT_VALUE) {
        if (j->type != AKT_VALUE) return false;
        if (i->val().type == VT_DEPENDENT || i->val().type == VT_DEPENDENT) continue;
        if (i->val() < j->val()) return true;
        if (j->val() < i->val()) return false;
      }
      else if (i->type == AKT_FULLTYPE) { // I is not a value; ie, it is a full_type
        if (j->type != AKT_FULLTYPE) return true;
        if (i->ft().def == abstract) {
          if ((j->ft().flags & i->ft().flags) != i->ft().flags || !j->ft().refs.ends_with(i->ft().refs))
            return false;
          continue;
        }
        if (j->ft().def == abstract) {
          if ((i->ft().flags & j->ft().flags) != j->ft().flags || !i->ft().refs.ends_with(j->ft().refs))
            return true;
          continue;
        }
        if (i->ft() < j->ft()) return true;
        if (j->ft() < i->ft()) return false;
      }
    } return false;
  }

  void arg_key::mirror_types(definition_template *temp) {
    for (size_t i = 0; i < temp->params.size(); ++i)
      if (temp->params[i]->flags & DEF_TYPENAME) {
        new(&values[i].data) full_type();
        values[i].type = AKT_FULLTYPE;
      }
      else {
        new(&values[i].data) aug_value();
        values[i].type = AKT_VALUE;
      }
  }

  void arg_key::put_final_type(size_t argnum, const full_type &type) { new (&values[argnum].data) full_type(); values[argnum].ft().copy(type); values[argnum].type = AKT_FULLTYPE; }
  void arg_key::swap_final_type(size_t argnum, full_type &type)      { new (&values[argnum].data) full_type(); values[argnum].ft().swap(type); values[argnum].type = AKT_FULLTYPE; }
  void arg_key::put_type(size_t argnum, const full_type &type) {
    if (type.def) {
      if (type.def->flags & DEF_DEPENDENT)
        return put_final_type(argnum, abstract);
      if (type.def->flags & DEF_TYPED and ((definition_typed*)type.def)->type) {
        // Copy the type we were given
        full_type ft; ft.refs.copy(type.refs);
        // Copy the referencers that our type has of its own; not ref_stack::referencers, but ref_stack::def->referencers.
        ft.refs.prepend(((definition_typed*)type.def)->referencers);
        // Tack on the modifiers
        ft.flags |= ((definition_typed*)type.def)->modifiers;
        // Change out the type
        ft.def = ((definition_typed*)type.def)->type;
        return swap_type(argnum, ft);
      }
    }
    return put_final_type(argnum, type);
  }
  void arg_key::swap_type(size_t argnum, full_type &type) {
    if (type.def and type.def->flags & DEF_TYPED and ((definition_typed*)type.def)->type) {
      // Copy the referencers that our type has of its own; not ref_stack::referencers, but ref_stack::def->referencers.
      type.refs.prepend(((definition_typed*)type.def)->referencers);
      // Tack on the modifiers
      type.flags |= ((definition_typed*)type.def)->modifiers;
      // Change out the type
      type.def = ((definition_typed*)type.def)->type;
      return swap_type(argnum, type);
    }
    return swap_final_type(argnum, type);
  }
  void arg_key::put_value(size_t argnum, const value &val) {
    new(&values[argnum].data) aug_value(val);
    values[argnum].type = AKT_VALUE;
  }

  bool arg_key::is_abstract() const {
    for (const node* n = values; n != endv; ++n)
      if (n->is_abstract()) return true;
    return false;
  }
  bool arg_key::is_dependent() const {
    for (const node* n = values; n != endv; ++n)
      if (n->is_abstract()) return true;
    return false;
  }
  int arg_key::conflicts_with(const arg_key &k) const {
    int confs = 0;
    for (const node *n1 = values, *n2 = k.values; n1 != endv and n2 != k.endv; ++n1, ++n2) {
      if (n1->is_abstract() || n2->is_abstract()) continue;
      if (n1->type != n2->type) { ++confs; continue; }
      if (n1->type == AKT_FULLTYPE && n1->ft().def != n2->ft().def) ++confs;
    }
    return confs;
  }
  bool arg_key::matches(const arg_key &k) const {
    for (const node *n1 = values, *n2 = k.values; n1 != endv and n2 != k.endv; ++n1, ++n2) {
      if (n1->is_abstract()) continue;
      if (n1->type != n2->type) return false;
      if (n1->type == AKT_FULLTYPE && n1->ft().def != n2->ft().def) return false;
    }
    return true;
  }

  /// Default constructor; mark values nullptr.
  arg_key::arg_key(): values(nullptr), endv(nullptr) {}
  /// Construct with a size, reserving sufficient memory.
  arg_key::arg_key(size_t n): values(new node[n]), endv(values+n) {} // Word to the wise: Do not switch the order of this initialization.
  /// Construct from a ref_stack.
  arg_key::arg_key(const ref_stack& rf) {
    #ifdef DEBUG_MODE
      if (rf.empty()) {
        cerr << "Critical error." << endl;
        asm("int3"); return;
      }
    #endif
    const ref_stack::node &n = rf.top();
    #ifdef DEBUG_MODE
      if (n.type != ref_stack::RT_FUNCTION) {
        cerr << "Critical error." << endl;
        asm("int3"); return;
      }
    #endif

    const ref_stack::parameter_ct &p = ((ref_stack::node_func*)&n)->params;
    values = new node[p.size()]; endv = values + p.size();
    for (size_t i = 0; i < p.size(); ++i)
      this->put_type(i, p[i]);
  }
  /// Construct a copy.
  arg_key::arg_key(const arg_key& other): values(new node[other.endv-other.values]) {
    node *i = values;
    for (node *j = other.values; j != other.endv; ++i, ++j)
      *i = *j;
    endv = i;
  }
  /// Just copy.
  arg_key &arg_key::operator=(const arg_key& other) {
    delete[] values;
    values = new node[other.endv-other.values];
    node *i = values;
    for (node *j = other.values; j != other.endv; ++i, ++j)
      *i = *j;
    endv = i;
    return *this;
  }
  /// Destruct, freeing items.
  arg_key::~arg_key() { delete[] values; }

  arg_key::node &arg_key::node::operator=(const node& other) {
    type = other.type;
    if (type == AKT_FULLTYPE)
      new(&data) full_type(other.ft());
    else
      new(&data) aug_value(other.av());
    return *this;
  }

  bool arg_key::node::is_abstract() const {
    return type == AKT_FULLTYPE?
      ft().def == abstract || !ft().def || (ft().def->flags & DEF_DEPENDENT)
    : val().type == VT_DEPENDENT;
  }

  arg_key::node::~node() {
    if (type == AKT_FULLTYPE)
      ((full_type*)&data)->~full_type();
    else if (type == AKT_VALUE)
      ((aug_value*)&data)->~aug_value();
  }

  bool arg_key::node::operator!=(const node &n) const {
    if (type != n.type) return true;
    if (type == AKT_FULLTYPE) return ft().def != abstract and n.ft().def != abstract and ft() != n.ft();
    return val().type != VT_DEPENDENT && n.val().type != VT_DEPENDENT && val() != n.val();
  }

  arg_key::aug_value::aug_value(): value(), ast(nullptr) {}
  arg_key::aug_value::aug_value(const value &v, AST *n): value(v), ast(n) {}
  arg_key::aug_value::aug_value(const aug_value &o): value(o), ast(o.ast? o.ast->duplicate() : nullptr) {}
}
