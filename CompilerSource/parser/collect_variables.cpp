/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
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

  bool CheckIfReserved(const std::string &name) {
    bool res = lang->is_shared_local(name);
    res |= lang->global_exists(name);
    res |= script_names.find(name) != script_names.end();
    res |= parsed_scope->declarations.find(name) != parsed_scope->declarations.end();
    return res;
  }

  std::string CheckIfIdentifier(AST::PNode &node) {
    if (node->type == AST::NodeType::IDENTIFIER) {
      std::string name = node->As<AST::IdentifierAccess>()->name.content;
      if (!CheckIfReserved(name)) return name;
    }
    return "";
  }

  void AddLocal(AST::PNode &node) {
    if (!node) return;
    std::string name = CheckIfIdentifier(node);
    if (cs->timeline_lookup.find(name) != cs->timeline_lookup.end()) {
      parsed_scope->tlines[name] = 0;
      return;
    }
    if (name != "") parsed_scope->locals[name] = dectrip();
  }

  void AddDot(AST::PNode &node) {
    if (!node) return;
    std::string name = CheckIfIdentifier(node);
    if (name != "") parsed_scope->dots[name] = 0;
  }

  void AddFunction(AST::FunctionCallExpression &node) {
    std::string name = CheckIfIdentifier(node.function);
    if (name != "") parsed_scope->funcs[name] = node.arguments.size();
  }

  bool VisitCodeBlock(AST::CodeBlock &node) final {
    for (auto &stmt : node.statements) AddLocal(stmt);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitDeclarationStatement(AST::DeclarationStatement &node) {
    for (const auto &decl : node.declarations) {
      dectrip dtrip("var");  // FIXME
      parsed_scope->declarations[decl.declarator->decl.name.content] = dtrip;
    }
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitBinaryExpression(AST::BinaryExpression &node) final {
    AddLocal(node.left);
    if (node.operation.type == enigma::parsing::TokenType::TT_DOT) {
      AddDot(node.right);  // what if it is reserved?
    } else {
      AddLocal(node.right);
    }
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitFunctionCall(AST::FunctionCallExpression &node) {
    AddFunction(node);
    for (auto &arg : node.arguments) AddLocal(arg);
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitWithStatement(AST::WithStatement &node) final {
    AddLocal(node.object);
    AddLocal(node.body);
    node.RecursiveSubVisit(*this);
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

  // virtual bool VisitBraceOrParenInitializer(BraceOrParenInitializer &node);
  // virtual bool VisitAssignmentInitializer(AssignmentInitializer &node);
  // virtual bool VisitInitializer(Initializer &node);
  // virtual bool VisitNewExpression(NewExpression &node);
  // virtual bool VisitDeleteExpression(DeleteExpression &node)

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
