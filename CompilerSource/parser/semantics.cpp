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

#include "semantics.h"

#include "object_storage.h"

namespace enigma::parsing {

bool SemanticAnnotator::VisitScopeAccess(AST::ScopeAccess &node) {
  if (node.op.type == TT_DOT) classify_access(node);
  return true;
}

bool SemanticAnnotator::VisitBinaryExpression(AST::BinaryExpression &node) {
  // EDL's / is real division regardless of operand types; div is the
  // integer kind. C++ would truncate 1/4 to 0, so mark for lowering.
  if (node.operation.type == TT_SLASH) node.lower_real_division = true;
  // GML's = compares except at statement position. The parser groups = at
  // assignment precedence; the printer emits operands flat, so the printed
  // == re-associates at C++ equality precedence, which is GML's grouping.
  if (gml_equals_ && node.operation.type == TT_EQUALS &&
      !statement_equals_.count(&node)) {
    node.lower_gml_equals = true;
  }
  // An assignment's right-hand side is statement-like in GML only for the
  // leftmost =; nested ones compare, which the set membership handles.
  return true;
}

// GML has no void: a call always has a value, and a function with nothing
// to return yields 0. Any void call used as a value (anywhere but statement
// position) gets wrapped `(call, 0)`; C++ would reject the use outright.
void SemanticAnnotator::check_void_value(AST::FunctionCallExpression &node) {
  if (statement_calls_.count(&node)) return;
  if (node.function->type != AST::NodeType::IDENTIFIER) return;
  auto *func = node.function->As<AST::IdentifierAccess>();
  if (!func->def || !frontend_->definition_returns_void(func->def)) return;
  node.lower_void_value = true;
  herr_->Warning(func->name)
      << "`" << func->name.content << "' returns no value; using 0";
}

void SemanticAnnotator::mark_statement(const AST::PNode &stmt) {
  if (!stmt) return;
  if (stmt->type == AST::NodeType::BINARY_EXPRESSION) {
    auto *bin = stmt->As<AST::BinaryExpression>();
    if (bin->operation.type == TT_EQUALS) statement_equals_.insert(bin);
  } else if (stmt->type == AST::NodeType::FUNCTION_CALL) {
    // A call at statement position discards its value; void is fine there.
    statement_calls_.insert(stmt.get());
  }
}

bool SemanticAnnotator::VisitCodeBlock(AST::CodeBlock &node) {
  for (auto &stmt : node.statements) mark_statement(stmt);
  return true;
}

bool SemanticAnnotator::VisitIfStatement(AST::IfStatement &node) {
  mark_statement(node.true_branch);
  mark_statement(node.false_branch);
  return true;
}

bool SemanticAnnotator::VisitForLoop(AST::ForLoop &node) {
  mark_statement(node.assignment);
  mark_statement(node.increment);
  mark_statement(node.body);
  return true;
}

bool SemanticAnnotator::VisitWhileLoop(AST::WhileLoop &node) {
  mark_statement(node.body);
  return true;
}

bool SemanticAnnotator::VisitDoLoop(AST::DoLoop &node) {
  mark_statement(node.body);
  return true;
}

bool SemanticAnnotator::VisitWithStatement(AST::WithStatement &node) {
  mark_statement(node.body);
  return true;
}

bool SemanticAnnotator::VisitFunctionCallExpression(AST::FunctionCallExpression &node) {
  validate_call(node);
  check_void_value(node);
  return true;
}

// Relocated from the parse-time SyntaxChecker: a callee's identity (and thus
// its arity) isn't knowable until dot accesses and instance lookups resolve,
// so argument checking belongs here. Only globally-resolved named callees
// are checkable today; member callees await the binder. Hard errors: the
// clang-populated bounds carry real parameters, defaults, and variadic
// marks.
void SemanticAnnotator::validate_call(AST::FunctionCallExpression &node) {
  if (node.function->type != AST::NodeType::IDENTIFIER) return;
  auto *func = node.function->As<AST::IdentifierAccess>();
  jdi::definition *def = func->def;
  if (!def) return;
  unsigned int min = 0;
  unsigned int max = 0;
  frontend_->definition_parameter_bounds(def, min, max);
  if (max == unsigned(-1)) return;
  if (node.arguments.size() < min) {
    herr_->Error(func->name)
        << "Too few arguments to `" << func->name.content << "': wanted at least "
        << min << ", got " << node.arguments.size();
  } else if (node.arguments.size() > max) {
    herr_->Error(func->name)
        << "Too many arguments to `" << func->name.content << "': wanted at most "
        << max << ", got " << node.arguments.size();
  }
}

bool SemanticAnnotator::VisitDeclarationStatement(AST::DeclarationStatement &node) {
  default_template_args(node);
  record_locals(node);
  return true;
}

// EDL: a bare template name used as a type means the template with every
// undefaulted argument filled by variant. An explicit argument list parses
// as a TemplateId, which this leaves alone.
void SemanticAnnotator::default_template_args(AST::DeclarationStatement &node) {
  if (!node.clause || !node.clause->specifiers) return;
  auto *spec = node.clause->specifiers.get();
  if (spec->id_expression &&
      spec->id_expression->type == AST::NodeType::TEMPLATE_ID) {
    return;
  }
  jdi::definition *def = spec->Definition();
  if (!def || !(def->flags & jdi::DEF_TEMPLATE)) return;
  spec->default_template_args = frontend_->template_undefaulted_params(def);
}

// Locals declared with a class type get plain member access; everything
// else keeps instance semantics. var and variant are classes to JDI but are
// EDL's instance-handle types, so they stay on the varaccess route.
void SemanticAnnotator::record_locals(AST::DeclarationStatement &node) {
  if (!node.clause || !node.clause->specifiers) return;
  jdi::definition *def = node.clause->specifiers->Definition();
  if (!def || !(def->flags & jdi::DEF_CLASS)) return;
  if (def->name == "var" || def->name == "variant") return;
  for (const auto &decl : node.clause->declarators) {
    if (!decl || decl->name.content.empty()) continue;
    // Only plain declarators: a pointer or array of the class type is not
    // directly member-accessible through the dot.
    if (!decl->declarator_expr ||
        decl->declarator_expr->type != AST::NodeType::IDENTIFIER) {
      continue;
    }
    struct_locals_[std::string(decl->name.content)] = def;
  }
}

// Dot accesses classify in three tiers: the global./local. keyword
// prefixes, then locals declared with a definition-page class type (plain
// member access), then everything else as an instance varaccess. A chained
// or computed lhs (a.b.c, f(x).y) is an instance handle under GML
// semantics: built-in locals on it go through the tier accessor, anything
// else through varaccess. Struct-typed chains are future binder work.
void SemanticAnnotator::classify_access(AST::ScopeAccess &node) {
  using AccessKind = AST::ScopeAccess::AccessKind;
  if (!node.lhs) return;  // `::name` global-scope id: not a dot access.
  bool shared = frontend_->is_shared_local(node.name.content);
  if (node.lhs->type != AST::NodeType::IDENTIFIER) {
    node.access_kind = shared ? AccessKind::SHARED : AccessKind::VARACCESS;
    return;
  }
  const std::string &left = node.lhs->As<AST::IdentifierAccess>()->name.content;
  if (left == "global") {
    node.access_kind = AccessKind::GLOBAL;
  } else if (left == "local") {
    node.access_kind = AccessKind::LOCAL;
  } else if (struct_locals_.count(left)) {
    node.access_kind = AccessKind::MEMBER;
  } else if (shared) {
    // Built-in locals (x, y, speed...) are inherited by every instance
    // through the object tiers; access goes through the generated tier
    // accessor, never a per-name varaccess.
    node.access_kind = AccessKind::SHARED;
  } else {
    node.access_kind = AccessKind::VARACCESS;
  }
}

}  // namespace enigma::parsing

namespace {

// Annotates one AST; returns 1 if the pass left errors on it. Parse-phase
// errors abort the compile before this pass runs, so any error present
// afterward is semantic. `where` names the code for the diagnostic.
int annotate(ParsedCode &code, const std::string &where) {
  enigma::parsing::SemanticAnnotator annotator(
      &code.ast.herr, code.ast.lexer->GetContext().language_fe,
      code.ast.lexer->GetContext().compatibility_opts.use_gml_equals);
  code.ast.VisitNodes(annotator);
  if (!code.ast.HasError()) return 0;
  std::cerr << "Semantic error in " << where << ":\n"
            << code.ast.ErrorString() << std::endl;
  return 1;
}

}  // namespace

int annotate_semantics(CompileState &state) {
  int errors = 0;
  for (parsed_object *obj : state.parsed_objects)
    for (ParsedEvent &event : obj->all_events)
      errors += annotate(event, "object `" + obj->name + "'");
  for (ParsedScript *script : state.parsed_scripts) {
    errors += annotate(script->code, "script `" + script->name + "'");
    if (script->global_code)
      errors += annotate(*script->global_code, "script `" + script->name + "'");
  }
  for (ParsedScript *tline : state.parsed_tlines) {
    errors += annotate(tline->code, "timeline `" + tline->name + "'");
    if (tline->global_code)
      errors += annotate(*tline->global_code, "timeline `" + tline->name + "'");
  }
  for (parsed_room *room : state.parsed_rooms) {
    if (room->creation_code)
      errors += annotate(*room->creation_code, "room `" + room->name + "' creation code");
    for (auto &[id, icc] : room->instance_create_codes)
      if (icc.code)
        errors += annotate(*icc.code, "instance creation code in room `" + room->name + "'");
    for (auto &[id, icc] : room->instance_precreate_codes)
      if (icc.code)
        errors += annotate(*icc.code, "instance precreation code in room `" + room->name + "'");
  }
  return errors;
}
