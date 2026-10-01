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
#include "parsing/precedence.h"

namespace enigma::parsing {

namespace {

// Finds anything in an expression that can change state another operand
// reads: calls (scripts touch instance and global variables), assignments,
// increments, new/delete. In GML an = inside an operand compares.
struct SideEffectFinder : AST::Visitor {
  bool found = false;
  bool Mark(bool side_effect) {
    found = found || side_effect;
    return !found;
  }
  bool VisitFunctionCallExpression(AST::FunctionCallExpression &) final { return Mark(true); }
  bool VisitNewExpression(AST::NewExpression &) final { return Mark(true); }
  bool VisitDeleteExpression(AST::DeleteExpression &) final { return Mark(true); }
  bool VisitUnaryPrefixExpression(AST::UnaryPrefixExpression &node) final {
    return Mark(node.operation.type == TT_INCREMENT || node.operation.type == TT_DECREMENT);
  }
  bool VisitUnaryPostfixExpression(AST::UnaryPostfixExpression &node) final {
    return Mark(node.operation.type == TT_INCREMENT || node.operation.type == TT_DECREMENT);
  }
  bool VisitBinaryExpression(AST::BinaryExpression &node) final {
    const TokenType op = node.operation.type;
    return Mark(op == TT_ASSIGN || op == TT_ASSOP);
  }
};

bool HasSideEffects(AST::Node &node) {
  SideEffectFinder finder;
  node.RecurusiveVisit(finder);
  return finder.found;
}

// C++ leaves the order of call arguments and most operands unspecified (GCC
// goes right to left); GML evaluates left to right. Order matters once one
// operand has side effects and another isn't a literal.
bool OrderMatters(const std::vector<AST::Node *> &operands) {
  for (std::size_t i = 0; i < operands.size(); ++i) {
    if (!HasSideEffects(*operands[i])) continue;
    for (std::size_t j = 0; j < operands.size(); ++j)
      if (j != i && operands[j]->type != AST::NodeType::LITERAL) return true;
  }
  return false;
}

bool IsOrderedOperator(TokenType op) {
  switch (op) {
    case TT_PLUS: case TT_MINUS: case TT_STAR: case TT_SLASH: case TT_DIV: case TT_MOD:
    case TT_PERCENT: case TT_LSH: case TT_RSH: case TT_AMPERSAND: case TT_CARET: case TT_PIPE:
    case TT_XOR: case TT_LESS: case TT_LESSEQUAL: case TT_GREATER: case TT_GREATEREQUAL:
    case TT_EQUALTO: case TT_NOTEQUAL:
      return true;
    default:
      return false;
  }
}

}  // namespace


bool SemanticAnnotator::VisitScopeAccess(AST::ScopeAccess &node) {
  if (node.op.type == TT_DOT) classify_access(node);
  return true;
}

// Whether `op` binds looser than == (&& || ^^, and C++'s & | ^), so a
// value-position = parsed above it must move below it.
bool SemanticAnnotator::looser_than_equality(TokenType op) const {
  if (op == TT_EQUALS || op == TT_ASSOP || op == TT_COMMA) return false;
  const bool gml = frontend_ && frontend_->compatibility_opts().use_gml_precedence;
  auto find = [&](TokenType t) -> const OperatorPrecedence * {
    if (gml) if (auto it = Precedence::kGmlBinaryPrec.find(t); it != Precedence::kGmlBinaryPrec.end())
      return &it->second;
    auto it = Precedence::kBinaryPrec.find(t);
    return it == Precedence::kBinaryPrec.end() ? nullptr : &it->second;
  };
  const OperatorPrecedence *p = find(op);  // a[i] and other non-binary operators: tight
  return p && p->precedence > find(TT_EQUALTO)->precedence;
}

bool SemanticAnnotator::VisitBinaryExpression(AST::BinaryExpression &node) {
  // GML's = compares except at statement position, at equality precedence;
  // the parser grouped it at assignment precedence, so `a and b = c` came out
  // as (a and b) = c. Push the = below a looser operand: (L = (c op d)) becomes
  // ((L = c) op d), and ((a op b) = R) becomes (a op (b = R)). The moved =
  // is visited, and fixed further, as a child. The parser already turns an if
  // condition's root = into == (TT_EQUALTO) with the same grouping; a written
  // == never has an unparenthesized looser operand, so fix those too.
  if ((gml_equals_ && node.operation.type == TT_EQUALS && !statement_equals_.count(&node)) ||
      node.operation.type == TT_EQUALTO) {
    auto loose = [&](const AST::PNode &n) {
      return n->type == AST::NodeType::BINARY_EXPRESSION &&
             looser_than_equality(n->As<AST::BinaryExpression>()->operation.type);
    };
    const bool right = loose(node.right);
    if (right || loose(node.left)) {
      AST::PNode moved = std::move(right ? node.right : node.left);
      auto *op = moved->As<AST::BinaryExpression>();
      if (right) {  // node = (L = (c op d)): moved becomes (L = c), node becomes (moved op d)
        AST::PNode d = std::move(op->right);
        op->right = std::move(op->left);
        op->left = std::move(node.left);
        node.left = std::move(moved);
        node.right = std::move(d);
      } else {  // node = ((a op b) = R): moved becomes (b = R), node becomes (a op moved)
        AST::PNode a = std::move(op->left);
        op->left = std::move(op->right);
        op->right = std::move(node.right);
        node.right = std::move(moved);
        node.left = std::move(a);
      }
      std::swap(node.operation, op->operation);
    }
  }
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
  if (gml_equals_ && IsOrderedOperator(node.operation.type) &&
      OrderMatters({node.left.get(), node.right.get()})) {
    node.evaluate_in_order = true;
  }
  // An assignment's right-hand side is statement-like in GML only for the
  // leftmost =; nested ones compare, which the set membership handles.
  return true;
}

void SemanticAnnotator::mark_statement(const AST::PNode &stmt) {
  if (!stmt) return;
  if (stmt->type != AST::NodeType::BINARY_EXPRESSION) return;
  auto *bin = stmt->As<AST::BinaryExpression>();
  if (bin->operation.type == TT_EQUALS) statement_equals_.insert(bin);
}

bool SemanticAnnotator::VisitCodeBlock(AST::CodeBlock &node) {
  for (auto &stmt : node.statements) mark_statement(stmt);
  return true;
}

bool SemanticAnnotator::VisitSwitchStatement(AST::SwitchStatement &node) {
  node.lower_gml_switch = gml_equals_;
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
  if (gml_equals_ && node.arguments.size() > 1) {
    std::vector<AST::Node *> arguments;
    for (auto &arg : node.arguments) arguments.push_back(arg.get());
    node.evaluate_in_order = OrderMatters(arguments);
  }
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
  record_locals(node);
  return true;
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
// semantics, so it lowers through varaccess too; struct-typed chains are
// future binder work.
void SemanticAnnotator::classify_access(AST::ScopeAccess &node) {
  using AccessKind = AST::ScopeAccess::AccessKind;
  if (!node.lhs) return;  // `::name` global-scope id: not a dot access.
  const std::string left = node.lhs->type == AST::NodeType::IDENTIFIER
      ? node.lhs->As<AST::IdentifierAccess>()->name.content : "";
  if (left == "global") {
    node.access_kind = AccessKind::GLOBAL;
  } else if (left == "local") {
    node.access_kind = AccessKind::LOCAL;
  } else if (struct_locals_.count(left)) {
    node.access_kind = AccessKind::MEMBER;
  } else if (frontend_->is_shared_local(node.name.content)) {
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
