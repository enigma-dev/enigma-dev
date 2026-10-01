/* Copyright (C) 2026 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
*/

#ifndef ENIGMA_SEMANTICS_H
#define ENIGMA_SEMANTICS_H

#include "parsing/ast.h"

#include <set>
#include <string>
#include <unordered_map>

struct CompileState;

namespace enigma::parsing {

/// Post-link semantic annotation. Walks each parsed AST once, after the
/// linking phase has harvested locals across all objects, and writes
/// semantic facts onto the nodes for the printers (and future checks) to
/// read. Each concern is a named helper dispatched from the node visits;
/// canonicalization lives here, never in the parser.
class SemanticAnnotator : public AST::Visitor {
 public:
  SemanticAnnotator(ErrorHandler *herr, const LanguageFrontend *frontend,
                    bool gml_equals = true,
                    const std::set<std::string> *globalvars = nullptr)
      : herr_(herr), frontend_(frontend), gml_equals_(gml_equals), globalvars_(globalvars) {}

  bool VisitScopeAccess(AST::ScopeAccess &node) final;
  bool VisitIdentifierAccess(AST::IdentifierAccess &node) final;
  bool VisitBinaryExpression(AST::BinaryExpression &node) final;
  bool VisitFunctionCallExpression(AST::FunctionCallExpression &node) final;
  bool VisitDeclarationStatement(AST::DeclarationStatement &node) final;
  // Statement-position holders: their expression children are the only
  // places GML's = assigns (see mark_statement).
  bool VisitCodeBlock(AST::CodeBlock &node) final;
  bool VisitIfStatement(AST::IfStatement &node) final;
  bool VisitSwitchStatement(AST::SwitchStatement &node) final;
  bool VisitForLoop(AST::ForLoop &node) final;
  bool VisitWhileLoop(AST::WhileLoop &node) final;
  bool VisitDoLoop(AST::DoLoop &node) final;
  bool VisitWithStatement(AST::WithStatement &node) final;

  /// Register a name as a class-typed local ahead of the walk. The walk
  /// itself records declarations it encounters; this is the seam for scopes
  /// harvested elsewhere (and for tests).
  void DeclareLocal(std::string_view name, jdi::definition *def) {
    struct_locals_[std::string(name)] = def;
  }

 private:
  void classify_access(AST::ScopeAccess &node);
  void validate_call(AST::FunctionCallExpression &node);
  void record_locals(AST::DeclarationStatement &node);
  // Register a statement-position expression: if it is an = at its root,
  // that = assigns. Parents visit before children, so the registration is
  // in place before VisitBinaryExpression reaches the node.
  void mark_statement(const AST::PNode &stmt);
  bool looser_than_equality(TokenType op) const;

  ErrorHandler *herr_;
  const LanguageFrontend *frontend_;
  // GML dialect: = compares in value position. Off under C++ inheritance
  // (CompatibilityOptions::use_gml_equals).
  bool gml_equals_;
  const std::set<std::string> *globalvars_;
  // Statement-position = nodes; every other = is a comparison.
  std::set<const AST::Node*> statement_equals_;
  // Locals declared with a class type (definition pages) in this AST, in
  // walk order. Flat per-event scoping for now; block scopes can refine it.
  std::unordered_map<std::string, jdi::definition*> struct_locals_;
};

}  // namespace enigma::parsing

/// Annotate every parsed AST in the game: object events, scripts,
/// timelines, and room creation code. Returns the number of ASTs that
/// gained semantic errors; the compile sequence treats nonzero as fatal
/// (parse-phase errors abort earlier, so any error present afterward is
/// this pass's).
int annotate_semantics(CompileState &state);

#endif
