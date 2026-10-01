/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
**  Copyright (C) 2024 Fares Atef                                               **
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

//Welcome to the ENIGMA EDL-to-C++ parser; just add semicolons.
//...No, it's not really that simple.

#include "event_reader/event_parser.h"

#include <JDI/src/System/builtins.h>
#include <cstdio>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_set>

using namespace std;

extern int global_script_argument_count;

struct scope_ignore {
  map<string, int> ignore;
  bool is_with;

  scope_ignore(scope_ignore *x) : is_with(x->is_with) {}
  scope_ignore(bool x) : is_with(x) {}
  scope_ignore(int x) : is_with(x) {}
};

#include "../parsing/tokens.h"
#include "collect_variables.h"
#include "languages/language_adapter.h"
#include "object_storage.h"

using enigma::parsing::AST;

std::string GetFullType(const jdi::full_type &ft, std::string_view declared_name) {
  std::string type;
  std::vector<std::size_t> flags_values = {
      jdi::builtin_flag__const->value,    jdi::builtin_flag__static->value,       jdi::builtin_flag__volatile->value,
      jdi::builtin_flag__mutable->value,  jdi::builtin_flag__register->value,     jdi::builtin_flag__inline->value,
      jdi::builtin_flag__Complex->value,  jdi::builtin_flag__unsigned->value,     jdi::builtin_flag__signed->value,
      jdi::builtin_flag__short->value,    jdi::builtin_flag__long->value,         jdi::builtin_flag__long_long->value,
      jdi::builtin_flag__restrict->value, jdi::builtin_typeflag__override->value, jdi::builtin_typeflag__final->value};

  std::vector<std::size_t> flags_masks = {
      jdi::builtin_flag__const->mask,    jdi::builtin_flag__static->mask,       jdi::builtin_flag__volatile->mask,
      jdi::builtin_flag__mutable->mask,  jdi::builtin_flag__register->mask,     jdi::builtin_flag__inline->mask,
      jdi::builtin_flag__Complex->mask,  jdi::builtin_flag__unsigned->mask,     jdi::builtin_flag__signed->mask,
      jdi::builtin_flag__short->mask,    jdi::builtin_flag__long->mask,         jdi::builtin_flag__long_long->mask,
      jdi::builtin_flag__restrict->mask, jdi::builtin_typeflag__override->mask, jdi::builtin_typeflag__final->mask};

  std::vector<std::string> flags_names = {"const",  "static",    "volatile", "mutable",  "register",
                                          "inline", "complex",   "unsigned", "signed",   "short",
                                          "long",   "long long", "restrict", "override", "final"};

  for (std::size_t i = 0; i < flags_values.size(); i++) {
    if ((ft.flags & flags_masks[i]) == flags_values[i]) {
      // ft.def may be null: an unresolved type defers to the semantic phase.
      if (flags_names[i] != "signed" || (ft.def && ft.def->name == "char")) {
        type += flags_names[i] + " ";
      }
    }
  }

  // type += ft.def->name + " ";

  std::string name(declared_name);
  if (name != "" && ft.refs.size() == 0) {
    type += name + " ";
  }

  const jdi::ref_stack &stack = ft.refs;
  auto first = stack.begin();

  std::string ref;
  bool flag = false;
  bool print_name = true;

  for (auto it = first; it != stack.end(); it++) {
    if (it->type == jdi::ref_stack::RT_POINTERTO) {
      flag = true;
      ref = '*' + ref;
    } else if (it->type == jdi::ref_stack::RT_REFERENCE) {
      flag = true;
      ref = '&' + ref;
    } else {
      if (it->type == jdi::ref_stack::RT_ARRAYBOUND) {
        if (flag) {
          ref = '(' + ref + ')';
        }

        std::size_t arr_size = it->arraysize();
        if (arr_size != 0) {
          ref += '[' + std::to_string(arr_size) + ']';
        } else {
          ref += "[]";
        }
      }
      // TODO: RT_MEMBER_POINTER
      flag = false;
    }

    if (print_name) {
      if (name != "") {
        if (it->type == jdi::ref_stack::RT_ARRAYBOUND) {
          ref = name + ref;
        } else {
          ref += name;
        }
      }
      print_name = false;
    }
  }

  type += ref;
  return type;
}

/**
  AST Node Visitor that mines a piece of code for variables declared,
  functions called, and variables dot-accessed or used in with() blocks.
  The legacy implementation also grabbed timeline indices used through
  direct timeline_index assignment, though I'm baffled as to why.
**/
class DeclGatheringVisitor : public AST::Visitor {
  const LanguageFrontend *const lang;
  ParsedScope *const parsed_scope;
  const NameSet &script_names;
  CompileState *cs;

  std::string CheckIfIdentifier(AST::PNode &node) {
    if (node->type == AST::NodeType::IDENTIFIER) {
      std::string name = node->As<AST::IdentifierAccess>()->name.content;
      if (auto d = declared_.find(name); d != declared_.end()) {
        node->As<AST::IdentifierAccess>()->def = d->second;
        return "";
      } else if (script_names.find(name) != script_names.end()) {
        if (node->type == AST::NodeType::FUNCTION_CALL) {
          parsed_scope->funcs[name] = node->As<AST::FunctionCallExpression>()->arguments.size();
        }
        return "";
      } else
        return name;
    }
    return "";
  }

  void AddLocal(AST::PNode &node) {
    if (!node) return;
    AddLocalName(CheckIfIdentifier(node));
  }

  void AddLocalName(const std::string &name) {
    if (name == "") return;
    if (lang->is_shared_local(name)) {
      parsed_scope->globallocals[name] = 0;
      return;
    }
    if (cs->timeline_lookup.find(name) != cs->timeline_lookup.end()) {
      parsed_scope->tlines[name] = 0;
      return;
    }
    if (!lang->global_exists(name)) {
      parsed_scope->locals[name] = dectrip("var");
      cs->add_dot_accessed_local(name);
    }
  }


  void AddFunction(AST::FunctionCallExpression &node) {
    std::string name = CheckIfIdentifier(node.function);
    if (name != "") parsed_scope->funcs[name] = node.arguments.size();
  }

  bool VisitCodeBlock(AST::CodeBlock &node) {
    for (auto &stmt : node.statements) AddLocal(stmt);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitDeclarationStatement(AST::DeclarationStatement &node) {
    bool is_global = node.storage_class == AST::DeclarationStatement::StorageClass::GLOBAL;
    bool is_local = node.storage_class == AST::DeclarationStatement::StorageClass::LOCAL;
    auto *type_id = node.clause ? node.clause->specifiers.get() : nullptr;
    jdi::definition *spec_def = type_id ? type_id->to_jdi_fulltype().def : nullptr;
    for (const auto &entry : node.clause->declarators) {
      std::string name(entry->name.content);
      jdi::full_type jft;
      if (type_id) jft = type_id->to_jdi_fulltype();
      enigma::parsing::walk_declarator_expr(entry->declarator_expr.get(), jft.refs);
      std::string ftype = GetFullType(jft, entry->name.content);
      std::string type = spec_def ? spec_def->name : "";
      size_t pos = ftype.find(name);
      std::string prefix;
      std::string suffix;
      if (pos != std::string::npos) {
        prefix = ftype.substr(0, pos);
        suffix = ftype.substr(pos + name.length());
      }
      prefix = std::regex_replace(prefix, std::regex("^ +| +$|( ) +"), "$1");
      suffix = std::regex_replace(suffix, std::regex("^ +| +$|( ) +"), "$1");
      dectrip dtrip(type, prefix, suffix);
      if (is_global) parsed_scope->globals[name] = dtrip, cs->globalvar_names.insert(name);
      if (is_local) parsed_scope->locals[name] = dtrip;
      cs->add_dot_accessed_local(name);
      parsed_scope->declarations[name] = spec_def;
      declared_[name] = spec_def;
    }

    for (const auto &entry : node.clause->declarators) {
      if (entry->init) {
        VisitInitializer(*entry->init);
      }
    }
    return false;
  }

  bool VisitBinaryExpression(AST::BinaryExpression &node) {
    AddLocal(node.left);
    AddLocal(node.right);
    node.RecursiveSubVisit(*this);
    return false;
  }

  // EDL dot accesses arrive as the uniform access node. global.<name>
  // declares a global; any other dot marks <name> as a dot-accessed local
  // so the accessor generator can reach it on every object.
  bool VisitScopeAccess(AST::ScopeAccess &node) {
    if (node.op.type == enigma::parsing::TokenType::TT_DOT) {
      std::string name(node.name.content);
      if (node.lhs && node.lhs->type == AST::NodeType::IDENTIFIER &&
          node.lhs->As<AST::IdentifierAccess>()->name.content == "global") {
        parsed_scope->globals[name] = dectrip("var");
        parsed_scope->locals[name] = dectrip("var");
        cs->add_dot_accessed_local(name);
      } else {
        AddLocal(node.lhs);
        // Built-in tier locals route through the generated tier accessor,
        // not a per-name varaccess; keep them out of the dot map.
        if (!lang->is_shared_local(name)) {
          parsed_scope->dots[name] = 0;
          cs->add_dot_accessed_local(name);
        }
      }
    }
    node.RecursiveSubVisit(*this);
    return false;
  }

  // A bare name in any other position (if/while conditions, unary operands,
  // subscripts, ...) is also a variable use.
  bool VisitIdentifierAccess(AST::IdentifierAccess &node) {
    std::string name(node.name.content);
    if (auto d = declared_.find(name); d != declared_.end())
      node.def = d->second;
    else if (!script_names.count(name))
      AddLocalName(name);
    return true;
  }

  bool VisitFunctionCallExpression(AST::FunctionCallExpression &node) {
    if (auto call = node.VariableNameCall(); call && !lang->is_shared_local(call->second)) {
      if (call->first == "variable_local_exists") parsed_scope->locals[call->second] = dectrip("var");
      cs->add_dot_accessed_local(call->second);
    }
    AddFunction(node);
    if (node.function->type != AST::NodeType::IDENTIFIER) node.function->RecurusiveVisit(*this);
    for (auto &arg : node.arguments) arg->RecurusiveVisit(*this);
    return false;
  }

  bool VisitWithStatement(AST::WithStatement &node) {
    AddLocal(node.object);

    vector<std::string> prev_locals;
    for (auto it = parsed_scope->locals.begin(); it != parsed_scope->locals.end(); ++it) {
      prev_locals.push_back(it->first);
    }

    AddLocal(node.body);
    node.RecursiveSubVisit(*this);

    for (auto it = parsed_scope->locals.begin(); it != parsed_scope->locals.end();) {
      if (it->first.substr(0, 8) == "argument") {
        it = parsed_scope->locals.erase(it);
      } else {
        if (std::find(prev_locals.begin(), prev_locals.end(), it->first) == prev_locals.end()) {
          parsed_scope->ambiguous[it->first] = dectrip();
          it = parsed_scope->locals.erase(it);
        } else {
          it++;
        }
      }
    }

    return false;
  }

  bool VisitInitializer(AST::Initializer &node) {
    if (node.target) {
      // Target is usually a type or identifier, not a variable usage in the expression sense
      node.target->accept(*this);
    }
    for (auto &val : node.values) {
      val->RecurusiveVisit(*this);
    }
    return false;
  }

 public:
  DeclGatheringVisitor(const LanguageFrontend *language_fe, ParsedScope *pscope, const NameSet &scripts,
                       CompileState *cs)
      : lang(language_fe), parsed_scope(pscope), script_names(scripts), cs(cs) {}

 private:
  DeclGatheringVisitor *parent_ = nullptr;
  /// Names declared in the code being visited (one event or script); a var in
  /// one event doesn't make the name local in the object's other events.
  std::unordered_map<std::string, jdi::definition*> declared_;
  /// Collection of names declared in this scope. Used to suppress
  /// emitting local variable usage information for temporaries.
  std::unordered_set<std::string> decls_;

  bool Declared(const std::string &id) const {
    if (decls_.find(id) != decls_.end()) return true;
    if (parent_) return parent_->Declared(id);
    return false;
  }
  std::unordered_set<std::string> &RootDecls() {
    if (parent_) return parent_->RootDecls();
    return decls_;
  }

  DeclGatheringVisitor(DeclGatheringVisitor *parent)
      : lang(parent_->lang),
        parsed_scope(parent_->parsed_scope),
        script_names(parent_->script_names),
        parent_(parent) {}
};

void collect_variables(const LanguageFrontend *lang, enigma::parsing::AST *ast, ParsedScope *parsed_scope,
                       const NameSet &script_names, CompileState *cs) {
  DeclGatheringVisitor visitor(lang, parsed_scope, script_names, cs);
  ast->VisitNodes(visitor);
}
