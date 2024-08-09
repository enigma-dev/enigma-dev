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

  bool VisitCodeBlock(AST::CodeBlock &node) final {
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
    if (node.operation.type == enigma::parsing::TokenType::TT_DOT) {
      if (node.right->type == AST::NodeType::IDENTIFIER) {
        parsed_scope->dots[node.left->As<AST::IdentifierAccess>()->name.content] = 0;
      }
    } else if (node.operation.type == enigma::parsing::TokenType::TT_EQUALS) {
      if (node.left->type == AST::NodeType::IDENTIFIER) {
        std::string name = node.left->As<AST::IdentifierAccess>()->name.content;
        // if (std::find(defined.begin(), defined.end(), name) == defined.end()) parsed_scope->locals[name] = dectrip();
      }
    }
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitFunctionCall(AST::FunctionCallExpression &node) {
    if (node.function->type == AST::NodeType::IDENTIFIER) {
      parsed_scope->funcs[node.function->As<AST::IdentifierAccess>()->name.content] = node.arguments.size();
    }
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitWithStatement(AST::WithStatement &node) final {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitIfStatement(AST::IfStatement &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

 public:
  DeclGatheringVisitor(const LanguageFrontend *language_fe, ParsedScope *pscope, const NameSet &scripts)
      : lang(language_fe), parsed_scope(pscope), script_names(scripts) {}

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
                       const NameSet &script_names) {
  DeclGatheringVisitor visitor(lang, parsed_scope, script_names);
  ast->VisitNodes(visitor);
}
