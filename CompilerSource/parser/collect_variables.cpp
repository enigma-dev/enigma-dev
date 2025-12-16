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

#include <cstdio>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_set>

#include "../parsing/tokens.h"
#include "../parsing/declarator.h"
#include "../parsing/full_type.h"
#include "collect_variables.h"
#include "languages/language_adapter.h"
#include "object_storage.h"

using namespace std;
using namespace enigma::parsing;
using enigma::parsing::AST;

extern int global_script_argument_count;

struct scope_ignore {
  map<string, int> ignore;
  bool is_with;

  scope_ignore(scope_ignore *x) : is_with(x->is_with) {}
  scope_ignore(bool x) : is_with(x) {}
  scope_ignore(int x) : is_with(x) {}
};

// Helper function to convert Declarator to string representation  
static std::string declarator_to_string(const FullType& ft) {
  const Declarator& decl = ft.decl;
  std::string result;
  std::string name = std::string(decl.name.content);
  bool need_parens = false;
  
  // Track if we've printed the name yet
  bool name_printed = false;
  
  // Process components in reverse order for correct C++ declarator syntax
  // We need to handle: base_type *name[10] -> name[10]*
  // So we process arrays/functions first, then pointers/references
  
  // Separate components into pre-name (arrays, functions) and post-name (pointers, references)
  std::vector<const DeclaratorNode*> post_name_ops;
  std::vector<const DeclaratorNode*> pre_name_ops;
  
  for (const auto& component : decl.components) {
    if (component.kind == DeclaratorNode::Kind::ARRAY_BOUND ||
        component.kind == DeclaratorNode::Kind::FUNCTION) {
      pre_name_ops.push_back(&component);
    } else {
      post_name_ops.push_back(&component);
    }
  }
  
  // Build string: post_name_ops (pointers/references) + name + pre_name_ops (arrays/functions)
  for (const auto* comp : post_name_ops) {
    switch (comp->kind) {
      case DeclaratorNode::Kind::POINTER_TO:
        result += '*';
        need_parens = true;
        break;
      case DeclaratorNode::Kind::REFERENCE:
        result += '&';
        need_parens = true;
        break;
      case DeclaratorNode::Kind::RVAL_REFERENCE:
        result += "&&";
        need_parens = true;
        break;
      case DeclaratorNode::Kind::MEMBER_POINTER:
        // Member pointer - would need class name
        result += "::*";
        need_parens = true;
        break;
      default:
        break;
    }
  }
  
  // Add name with parentheses if needed
  if (need_parens && (pre_name_ops.size() > 0 || name_printed)) {
    result = "(" + result + name;
    name_printed = true;
  } else if (!name_printed && !name.empty()) {
    result += name;
    name_printed = true;
  }
  
  // Add pre-name operations (arrays, functions)
  for (const auto* comp : pre_name_ops) {
    if (comp->kind == DeclaratorNode::Kind::ARRAY_BOUND) {
      const auto& arr = std::get<ArrayBoundNode>(const_cast<DeclaratorNode*>(comp)->value);
      if (arr.size == 0 || arr.size == static_cast<size_t>(-1)) {
        result += "[]";
      } else {
        result += "[" + std::to_string(arr.size) + "]";
      }
    } else if (comp->kind == DeclaratorNode::Kind::FUNCTION) {
      result += "()";  // Simplified - full parameter list would be complex
    }
  }
  
  // Close parentheses if we opened them
  if (need_parens && pre_name_ops.size() > 0) {
    result += ")";
  }
  
  return result;
}

std::string GetFullType(enigma::parsing::FullType &ft) {
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
      if (flags_names[i] != "signed" || (flags_names[i] == "signed" && ft.def->name == "char")) {
        type += flags_names[i] + " ";
      }
    }
  }

  // type += ft.def->name + " ";

  std::string decl_name = std::string(ft.decl.name.content);
  if (decl_name != "" && !ft.decl.components.size()) {
    type += decl_name + " ";
  }

  // Convert Declarator directly to string representation (replaces ref_stack conversion)
  std::string ref = declarator_to_string(ft);
  
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
      if (parsed_scope->declarations.find(name) != parsed_scope->declarations.end()) {
        node->As<AST::IdentifierAccess>()->type = parsed_scope->declarations[name];
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
    std::string name = CheckIfIdentifier(node);
    
    // If CheckIfIdentifier returned empty, it might be because the variable is in declarations
    // (declared in a parent object or globally). But if it's being used here and not in locals,
    // we should still add it to locals so it's available in this object.
    if (name == "" && node->type == AST::NodeType::IDENTIFIER) {
      std::string node_content = node->As<AST::IdentifierAccess>()->name.content;
      // Check if it's in declarations but not in locals - if so, add it to locals
      if (parsed_scope->declarations.find(node_content) != parsed_scope->declarations.end() &&
          parsed_scope->locals.find(node_content) == parsed_scope->locals.end()) {
        name = node_content;
      } else {
        return;  // It's a script name or something else we can't handle
      }
    }
    
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

  bool AddGlobal(AST::BinaryExpression &node) {
    if (node.left->type == AST::NodeType::IDENTIFIER) {
      auto left = node.left->As<AST::IdentifierAccess>();
      if (left->name.content == "global" && node.operation.type == enigma::parsing::TokenType::TT_DOT) {
        auto right = node.right->As<AST::IdentifierAccess>();
        parsed_scope->globals[right->name.content] = dectrip("var");
        parsed_scope->locals[right->name.content] = dectrip("var");
        cs->add_dot_accessed_local(right->As<AST::IdentifierAccess>()->name.content);
        return true;
      }
    }
    return false;
  }

  void AddDot(AST::PNode &node) {
    if (!node) return;
    std::string name = CheckIfIdentifier(node);
    if (name == "") return;
    parsed_scope->dots[name] = 0;
    cs->add_dot_accessed_local(name);
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
    for (const auto &decl : node.declarations) {
      std::string name = decl.declarator->decl.name.content;
      std::string ftype = GetFullType(*decl.declarator);
      std::string type = decl.declarator->def->name;
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
      if (is_global) parsed_scope->globals[name] = dtrip;
      if (is_local) parsed_scope->locals[name] = dtrip;
      cs->add_dot_accessed_local(name);
      parsed_scope->declarations[name] = node.def;
    }

    for (const auto &decl : node.declarations) {
      if (decl.init) {
        VisitInitializer(*decl.init);
      }
    }
    return false;
  }

  bool VisitBinaryExpression(AST::BinaryExpression &node) {
    bool added = AddGlobal(node);
    if (!added) {
      AddLocal(node.left);
      if (node.operation.type == enigma::parsing::TokenType::TT_DOT) {
        AddDot(node.right);
      } else {
        AddLocal(node.right);
      }
    }
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitFunctionCallExpression(AST::FunctionCallExpression &node) {
    AddFunction(node);
    for (auto &arg : node.arguments) AddLocal(arg);
    node.RecursiveSubVisit(*this);
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

  bool VisitIfStatement(AST::IfStatement &node) {
    AddLocal(node.condition);
    AddLocal(node.true_branch);
    AddLocal(node.false_branch);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitUnaryPrefixExpression(AST::UnaryPrefixExpression &node) {
    AddLocal(node.operand);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitUnaryPostfixExpression(AST::UnaryPostfixExpression &node) {
    AddLocal(node.operand);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitTernaryExpression(AST::TernaryExpression &node) {
    AddLocal(node.condition);
    AddLocal(node.true_expression);
    AddLocal(node.false_expression);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitLambdaExpression(AST::LambdaExpression &node) {
    // I think no need to add locals for the arguments, because we give them `auto` in the pretty printer
    AddLocal(node.body);
    node.RecursiveSubVisit(*this);
    return false;
  }

  virtual bool VisitSizeofExpression(AST::SizeofExpression &node) {
    if (node.kind == AST::SizeofExpression::Kind::EXPR) {
      AddLocal(std::get<AST::PNode>(node.argument));
    }
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitCastExpression(AST::CastExpression &node) {
    AddLocal(node.expr);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitParenthetical(AST::Parenthetical &node) {
    AddLocal(node.expression);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitArray(AST::Array &node) {
    for (auto &elem : node.elements) AddLocal(elem);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitForLoop(AST::ForLoop &node) {
    AddLocal(node.assignment);
    AddLocal(node.condition);
    AddLocal(node.increment);
    AddLocal(node.body);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitWhileLoop(AST::WhileLoop &node) {
    AddLocal(node.condition);
    AddLocal(node.body);
    node.RecursiveSubVisit(*this);
    return false;
  }

  virtual bool VisitDoLoop(AST::DoLoop &node) {
    AddLocal(node.body);
    AddLocal(node.condition);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitCaseStatement(AST::CaseStatement &node) {
    AddLocal(node.value);
    // CaseStatement::statements is code block, no need to add locals here
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitDefaultStatement(AST::DefaultStatement &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitSwitchStatement(AST::SwitchStatement &node) {
    AddLocal(node.expression);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitReturnStatement(AST::ReturnStatement &node) {
    AddLocal(node.expression);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitBreakStatement(AST::BreakStatement &node) {
    AddLocal(node.count);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitContinueStatement(AST::ContinueStatement &node) {
    AddLocal(node.count);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitBraceOrParenInitializer(AST::BraceOrParenInitializer &node) {
    for (auto &val : node.values) VisitInitializer(*val.second);
    return false;
  }

  bool VisitAssignmentInitializer(AST::AssignmentInitializer &node) {
    if (node.kind == AST::AssignmentInitializer::Kind::BRACE_INIT) {
      VisitBraceOrParenInitializer(*std::get<AST::BraceOrParenInitNode>(node.initializer));
    } else {
      auto &expr = std::get<AST::PNode>(node.initializer);
      AddLocal(expr);
      expr->accept(*this);
    }
    return false;
  }

  bool VisitInitializer(AST::Initializer &node) {
    if (node.kind == AST::Initializer::Kind::ASSIGN_EXPR) {
      auto &init = std::get<AST::AssignmentInitNode>(node.initializer);
      VisitAssignmentInitializer(*init);
    } else if (node.kind == AST::Initializer::Kind::BRACE_INIT) {
      auto &init = std::get<AST::BraceOrParenInitNode>(node.initializer);
      VisitBraceOrParenInitializer(*init);
    }
    return false;
  }

 public:
  DeclGatheringVisitor(const LanguageFrontend *language_fe, ParsedScope *pscope, const NameSet &scripts,
                       CompileState *cs)
      : lang(language_fe), parsed_scope(pscope), script_names(scripts), cs(cs) {}

 private:
  DeclGatheringVisitor *parent_ = nullptr;
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
