/** Copyright (C) 2020 Josh Ventura
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
**/

#include "ast.h"
#include "parser.h"
#include "parser/collect_variables.h"  // collect_variables
#include "macro_helper.h"

#include <string>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace enigma::parsing {

bool AST::empty() const { return !root_; }

// Utility routine: Apply this AST to a specified instance.
void AST::ApplyTo(int instance_id) {
  apply_to_ = instance_id;
}

void AST::WriteCppToStream(std::ofstream &of, int base_indent, bool is_script) const {
  CppPrettyPrinter visitor(of, lexer->GetContext().language_fe, is_script);
  if (root_) visitor.CollectDeclaredNames(*root_);
  if (root_ && lexer->GetContext().compatibility_opts.use_gml_equals)
    visitor.HoistVarDeclarations(*root_);
  if (apply_to_) {
    of << std::string(base_indent, ' ') << "with (" << *apply_to_ << ") {\n";
  }

  if (root_) root_->accept(visitor);
  
  if (apply_to_) {
    of << std::string(base_indent, ' ') << "}\n";
  }
}

AST AST::Parse(std::string_view code, const ParseContext* ctex) {
  AST res(code, ctex);
  res.root_ = enigma::parsing::Parse(res.lexer.get(), &res.herr);
  return res;
}

void AST::ExtractDeclarations(ParsedScope *destination_scope, CompileState *cs) {
  std::cout << "collecting variables..." << std::flush;
  const ParseContext &ctex = lexer->GetContext();
  collect_variables(ctex.language_fe, this, destination_scope, ctex.script_names, cs);
  std::cout << " done." << std::endl;
}

template<typename... SubNodes>
void AST::Node::RV(AST::Visitor &visitor, const SubNodes &...nodes) {
  (RVF(visitor, nodes), ...);
}

void AST::SyntaxError::RecursiveSubVisit(AST::Visitor &) {
  // Leaf — no children.
}
void AST::CodeBlock::RecursiveSubVisit(AST::Visitor &visitor) {
  RV(visitor, statements);
}
void AST::BinaryExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, left, right);
}
void AST::FunctionCallExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, function, arguments);
}
void AST::UnaryPrefixExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, operand);
}
bool AST::UnaryPrefixExpression::CanBeTypeSpecifier() const {
  // C++ pointer/reference declarator operators: *, &, &&.
  return operation.type == TT_STAR
      || operation.type == TT_AMPERSAND
      || operation.type == TT_AND;
}
void AST::UnaryPostfixExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, operand);
}
void AST::TernaryExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, condition, true_expression, false_expression);
}
void AST::TypeSpecifierSeq::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, id_expression, declspecs);
}
jdi::definition *AST::TypeSpecifierSeq::Definition() const {
  return id_expression ? id_expression->Definition() : nullptr;
}

jdi::full_type AST::TypeSpecifierSeq::to_jdi_fulltype() {
  // Spec-base only: { def, no refs, flags }. The declarator ref_stack is built
  // separately from the AST declarator-expression-tree by the owning
  // DeclaratorClause (see DeclaratorClause::to_jdi_fulltype). Returned by value
  // because downstream callers `swap_in` the result into JDI structures.
  //
  // TODO(jdi2): `flags` is std::size_t, but jdi::full_type's flag-taking ctor
  // takes `int` (even though its own `flags` field is unsigned long). The cast
  // is intentionally omitted so the narrowing shows up as a build warning rather
  // than being silenced; reconcile these flag widths when integrating the
  // libclang-based JDI replacement (jdi2).
  return {Definition(), flags};
}

// Combine the shared spec-seq with the i-th declarator's expression-tree into
// the JDI-bridge full_type. The complete-full_type constructor; the recursive
// ref_stack piece-builder it wraps is walk_declarator_expr.
jdi::full_type AST::DeclaratorClause::to_jdi_fulltype(std::size_t i) {
  jdi::full_type base = specifiers ? specifiers->to_jdi_fulltype() : jdi::full_type{};
  jdi::ref_stack rt;
  if (i < declarators.size())
    walk_declarator_expr(declarators[i]->declarator_expr.get(), rt);
  // TODO(jdi2): same flags-width narrowing as TypeSpecifierSeq::to_jdi_fulltype
  // (here jdi::full_type's own `flags` is unsigned long, the ctor takes int). Cast
  // omitted so it warns; reconcile during the jdi2 integration.
  return jdi::full_type{base.def, rt, base.flags};
}

// A named/simple/abstract param (`int x`, `int *p`, `int`) is a
// DeclarationStatement: split the base spec-seq from the declarator tree via the
// clause and swap the recomposed full_type into the parameter slot. We fill only
// the full_type portion (default-arg/variadic left empty -- parity with legacy
// to_jdi_refstack).
bool AST::DeclarationStatement::to_jdi_refstack_parameter(jdi::ref_stack::parameter &out) {
  if (!clause || !clause->specifiers || clause->declarators.empty()) return false;
  jdi::full_type ft = clause->to_jdi_fulltype(0);
  out.swap_in(ft);
  return true;
}

// Descend a (possibly nested) declarator-expression to the leaf TypeSpecifierSeq
// that carries its base type. Used for the abstract/nested param shape that has
// no DeclarationStatement wrapper.
static AST::TypeSpecifierSeq *find_leaf_type_spec(AST::Node *expr) {
  for (;;) {
    if (!expr) return nullptr;
    switch (expr->type) {
      case AST::NodeType::TYPE_SPECIFIER_SEQ: return expr->As<AST::TypeSpecifierSeq>();
      case AST::NodeType::UNARY_PREFIX_EXPRESSION:
        expr = expr->As<AST::UnaryPrefixExpression>()->operand.get();
        break;
      case AST::NodeType::BINARY_EXPRESSION:
        expr = expr->As<AST::BinaryExpression>()->left.get();
        break;
      case AST::NodeType::FUNCTION_CALL:
        expr = expr->As<AST::FunctionCallExpression>()->function.get();
        break;
      case AST::NodeType::PARENTHETICAL:
        expr = expr->As<AST::Parenthetical>()->expression.get();
        break;
      default:
        return nullptr;
    }
  }
}

// Bridge one function-declarator parameter (an entry in the call-shaped
// `arguments`) into a ref_stack::parameter. Params arrive heterogeneously:
//   * a named/simple/abstract param is a DeclarationStatement -- handled
//     polymorphically by to_jdi_refstack_parameter;
//   * an abstract/nested param (`int (*)[10]`) is left as the call-shaped
//     expression itself, embedding its base type as a leaf TypeSpecifierSeq -- no
//     clause node to home the virtual on, so it falls through to find_leaf_type_spec.
// Returns false for any other arg shape (a bare value expression isn't a
// parameter-declaration), aborting the surrounding walk: not a function declarator.
static bool arg_to_parameter(AST::Node *arg, jdi::ref_stack::parameter &out) {
  if (!arg) return false;
  if (arg->to_jdi_refstack_parameter(out)) return true;
  if (AST::TypeSpecifierSeq *spec = find_leaf_type_spec(arg)) {
    jdi::ref_stack rt;
    if (!walk_declarator_expr(arg, rt)) return false;
    jdi::full_type base = spec->to_jdi_fulltype();
    // TODO(jdi2): flags-width narrowing (unsigned long -> int), as above. Cast omitted to warn.
    jdi::full_type ft{base.def, rt, base.flags};
    out.swap_in(ft);
    return true;
  }
  return false;
}

// AST→JDI bridge: walk a declarator-expression-tree into a jdi::ref_stack.
// Inverse of Declarator::to_expression. Replaces Declarator::to_jdi_refstack
// for the AST-side path; called by callers that have a TypeSpecifierSeq + InitDeclarator
// and need the combined jdi::full_type for downstream JDI bridging.
bool walk_declarator_expr(AST::Node *expr, jdi::ref_stack &result) {
  if (!expr) return true;
  switch (expr->type) {
    case AST::NodeType::IDENTIFIER:
    case AST::NodeType::LITERAL:
    // A TypeSpecifierSeq is the leaf of an abstract/nested function-param tree
    // (`int (*)[10]`): the base type sits here, the declarator modifiers wrap
    // it. Terminates the walk the same as a name leaf -- the spec contributes
    // no ref_stack node, only the base type (read separately via find_leaf_type_spec).
    case AST::NodeType::TYPE_SPECIFIER_SEQ:
      return true;
    case AST::NodeType::UNARY_PREFIX_EXPRESSION: {
      auto &u = *expr->As<AST::UnaryPrefixExpression>();
      if (!walk_declarator_expr(u.operand.get(), result)) return false;
      // The walk is post-order (leaf->root), i.e. name-outward, so every
      // modifier must attach at the *bottom* of the stack to land in canonical
      // order (see ref_stack's documented example). push_array/push_func already
      // append at the bottom; jdi::ref_stack::push (pointer/reference) attaches
      // at the *top* instead, which would cluster all pointers ahead of all
      // arrays and collapse `int (*p)[N]` into `int *p[N]`. Route pointers
      // through prepend_c so they bottom-attach like the postfix modifiers.
      jdi::ref_stack one;
      if (u.operation.type == TT_STAR) {
        one.push(jdi::ref_stack::RT_POINTERTO);
      } else if (u.operation.type == TT_AMPERSAND) {
        one.push(jdi::ref_stack::RT_REFERENCE);
      } else {
        return false;
      }
      result.prepend_c(one);
      return true;
    }
    case AST::NodeType::BINARY_EXPRESSION: {
      auto &b = *expr->As<AST::BinaryExpression>();
      if (b.operation.type != TT_BEGINBRACKET) return false;
      if (!walk_declarator_expr(b.left.get(), result)) return false;
      std::size_t size = 0;
      if (b.right && b.right->type == AST::NodeType::LITERAL) {
        auto &lit = *b.right->As<AST::Literal>();
        if (auto *s = std::get_if<std::string>(&lit.value.value)) {
          try { size = std::stoul(*s); } catch (...) {}
        }
      }
      result.push_array(size);
      return true;
    }
    case AST::NodeType::FUNCTION_CALL: {
      auto &c = *expr->As<AST::FunctionCallExpression>();
      if (!walk_declarator_expr(c.function.get(), result)) return false;
      // Function-declarator parameters live in `arguments`, heterogeneously (see
      // arg_to_parameter): bridge each into a ref_stack::parameter and push the
      // list, mirroring Declarator::to_jdi_refstack's per-parameter build.
      jdi::ref_stack::parameter_ct params;
      for (auto &arg : c.arguments) {
        jdi::ref_stack::parameter p;
        if (!arg_to_parameter(arg.get(), p)) return false;
        params.throw_on(p);
      }
      result.push_func(params);
      return true;
    }
    case AST::NodeType::PARENTHETICAL: {
      // Grouping parens carry no ref_stack node of their own; the tree already
      // encodes the grouping. Walk through transparently into the same stack so
      // the name-outward bottom-attach ordering is preserved (a separate nested
      // stack + append_c would re-cluster, reintroducing the ordering bug).
      auto &p = *expr->As<AST::Parenthetical>();
      return walk_declarator_expr(p.expression.get(), result);
    }
    default:
      return false;
  }
}

bool AST::DebugPrinter::emit(AST::Node &node, const std::string &detail) {
  out << std::string(depth * 2, ' ') << AST::NodeToString(node.type);
  if (!detail.empty()) out << ' ' << detail;
  out << '\n';
  depth++;
  node.RecursiveSubVisit(*this);
  depth--;
  return false;  // children already walked; suppress caller's auto-recursion
}

std::string AST::DebugPrinter::Dump(AST::Node &node) {
  std::ostringstream oss;
  DebugPrinter printer(oss);
  node.accept(printer);
  return oss.str();
}

bool AST::DebugPrinter::DefaultVisit(AST::Node &node) { return emit(node, ""); }

bool AST::DebugPrinter::VisitUnaryPrefixExpression(AST::UnaryPrefixExpression &node) {
  return emit(node, "'" + node.operation.token + "'");
}

bool AST::DebugPrinter::VisitUnaryPostfixExpression(AST::UnaryPostfixExpression &node) {
  return emit(node, "'" + node.operation.token + "'");
}

bool AST::DebugPrinter::VisitBinaryExpression(AST::BinaryExpression &node) {
  return emit(node, "'" + node.operation.token + "'");
}

bool AST::DebugPrinter::VisitFunctionCallExpression(AST::FunctionCallExpression &node) {
  return emit(node, "args=" + std::to_string(node.arguments.size()));
}

bool AST::DebugPrinter::VisitLiteral(AST::Literal &node) {
  std::string detail;
  if (auto *s = std::get_if<std::string>(&node.value.value)) detail = "'" + *s + "'";
  else if (auto *i = std::get_if<long long>(&node.value.value)) detail = std::to_string(*i);
  else if (auto *d = std::get_if<long double>(&node.value.value)) detail = std::to_string((double)*d);
  return emit(node, detail);
}

bool AST::DebugPrinter::VisitIdentifierAccess(AST::IdentifierAccess &node) {
  return emit(node, "'" + std::string(node.name.content) + "'");
}

bool AST::DebugPrinter::VisitScopeAccess(AST::ScopeAccess &node) {
  return emit(node, "'" + std::string(node.name.content) + "'");
}

bool AST::DebugPrinter::VisitTemplateId(AST::TemplateId &node) {
  return emit(node, "args=" + std::to_string(node.args.size()));
}

bool AST::DebugPrinter::VisitDecltype(AST::Decltype &node) {
  return emit(node, "");
}

bool AST::DebugPrinter::VisitImplicitType(AST::ImplicitType &node) {
  return emit(node, node.kind == AST::ImplicitType::Kind::INT ? "int"
              : node.def ? node.def->name : "untyped");
}

void AST::DeclSpecList::RecursiveSubVisit(Visitor &visitor) {
  // `specs` are Tokens, not nodes; any surplus base-type trees are visitable
  // (diagnostics), though never printed.
  RV(visitor, extra_base_types);
}
void AST::LambdaExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, parameters, body);
}
void AST::SizeofExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, argument);
}
void AST::AlignofExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, type);
}
void AST::CastExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, type, expr);
}
void AST::Parenthetical::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, expression);
}
void AST::Array::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, elements);
}
void AST::IdentifierAccess::RecursiveSubVisit(Visitor &visitor) {
  (void) visitor;  // Varname token that appeared in code; it's a leaf.
}
void AST::ScopeAccess::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, lhs);  // `name` is a Token, not a node.
}
void AST::TemplateId::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, name, args);
}
void AST::Decltype::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, operand);
}
void AST::ImplicitType::RecursiveSubVisit(Visitor &visitor) {
  (void) visitor;  // Token-free builtin-type leaf; nothing to recurse into.
}
void AST::Literal::RecursiveSubVisit(Visitor &visitor) {
  (void) visitor;  // Literal in the middle of code; also a leaf.
}
void AST::IfStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, condition, true_branch, false_branch);
}
void AST::ForLoop::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, assignment, condition, increment, body);
}
void AST::WhileLoop::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, condition, body);
}
void AST::DoLoop::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, body, condition);
}
void AST::CaseStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, value, statements);
}
void AST::DefaultStatement::RecursiveSubVisit(Visitor &visitor) {
  // The parser nests each label's statements inside the node (see
  // ParseCaseOrDefaultStatement), so they are this node's to visit.
  RV(visitor, statements);
}
void AST::SwitchStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, expression, body);
}
void AST::ReturnStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, expression);
}
void AST::BreakStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, count);
}
void AST::ContinueStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, count);
}
void AST::WithStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, object, body);
}
void AST::Initializer::RecursiveSubVisit(Visitor &visitor) {
  if (target) RV(visitor, target);
  RV(visitor, values);
}
void AST::NewExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, placement_args, type, initializer);
}
void AST::DeleteExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, expression);
}
void AST::DeclarationStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, clause);
}
void AST::InitDeclarator::RecursiveSubVisit(Visitor &visitor) {
  // `declarator` is the FullType (JDI-bridge) form, not a Node;
  // `declarator_expr` is the AST-layer expression-tree (nullable until the
  // declarator parser emits it).
  RV(visitor, declarator_expr, init);
}
void AST::DeclaratorClause::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, specifiers, declarators);
}

const std::vector<std::string> AST::NodesNames = ENUM_NAME_VECTOR(NodeType,
    ERROR,
    BLOCK,
    BINARY_EXPRESSION,
    UNARY_PREFIX_EXPRESSION,
    UNARY_POSTFIX_EXPRESSION,
    TERNARY_EXPRESSION,
    LAMBDA_EXPRESSION,
    TYPE_SPECIFIER_SEQ,
    DECL_SPEC_LIST,
    SIZEOF,
    ALIGNOF,
    CAST,
    NEW,
    DELETE,
    PARENTHETICAL,
    ARRAY,
    IDENTIFIER,
    SCOPE_ACCESS,
    LITERAL,
    FUNCTION_CALL,
    IF,
    FOR,
    WHILE,
    DO,
    WITH,
    REPEAT,
    SWITCH,
    CASE,
    DEFAULT,
    BREAK,
    CONTINUE,
    RETURN,
    DECLARATION,
    INIT_DECLARATOR,
    INITIALIZER,
    DECLARATOR_CLAUSE,
    TEMPLATE_ID,
    DECLTYPE,
    IMPLICIT_TYPE);

const std::vector<std::string> AST::DeclarationStatement::StorageNames =
    ENUM_NAME_VECTOR(StorageClass, TEMPORARY, LOCAL, GLOBAL);

const std::vector<std::string> AST::CastExpression::KindNames = ENUM_NAME_VECTOR(Kind,
    C_STYLE,
    STATIC,
    DYNAMIC,
    REINTERPRET,
    CONST);

std::string AST::NodeToString(NodeType nt){
  return NodesNames[(int)nt];
}

std::string AST::DeclarationStatement::StorageToString(StorageClass st){
  return StorageNames[(int)st];
}

std::string AST::CastExpression::KindToString(Kind k){
  return KindNames[(int)k];
}

}  // namespace enigma::parsing
