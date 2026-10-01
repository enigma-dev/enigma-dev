/** Copyright (C) 2024 Fares Atef
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

#include <JDI/src/System/builtins.h>
#include "ast.h"
#include "precedence.h"

#include <iomanip>

using namespace enigma::parsing;

#define VISIT_AND_CHECK(node) \
  if (!Visit(node)) return false;

AST::CppPrettyPrinter::CppPrettyPrinter() {
  of = new std::ofstream();
  if (!of->is_open()) of->open("./CompilerSource/parsing/output.txt");
  print_type = false;
  is_script = false;
}

AST::CppPrettyPrinter::CppPrettyPrinter(const LanguageFrontend *lfe) : CppPrettyPrinter() {
  this->language_fe = lfe;
  print_type = false;
  is_script = false;
}

AST::CppPrettyPrinter::CppPrettyPrinter(std::ofstream &ofs, const LanguageFrontend *lfe, bool is_script)
    : of(&ofs), is_script(is_script), language_fe(lfe) {
  print_type = false;
}

void AST::CppPrettyPrinter::print(std::string code) { *of << code; }

bool AST::CppPrettyPrinter::GmlDialect() const {
  return language_fe && language_fe->compatibility_opts().use_gml_equals;
}

bool AST::CppPrettyPrinter::Gm8Compliance() const {
  return GmlDialect() && language_fe->compatibility_opts().compliance_mode <= 81;
}

// A value used as a truth value. GM8 counts a real as true when it is >= 0.5.
bool AST::CppPrettyPrinter::VisitCondition(PNode &condition) {
  if (!Gm8Compliance()) return Visit(condition);
  // Own parentheses: `if` prints a parenthetical condition flush after itself.
  print("(enigma::gml_truth(");
  if (!Visit(condition)) return false;
  print("))");
  return true;
}

static bool NeedsParens(const AST::BinaryExpression &parent, const AST::Node &operand, bool right);

// A variable read with no side effects: bound by value when operands are ordered.
static bool IsPlainRead(const AST::Node &node) {
  if (node.type == AST::NodeType::IDENTIFIER || node.type == AST::NodeType::SCOPE_ACCESS) return true;
  return node.type == AST::NodeType::BINARY_EXPRESSION &&
         static_cast<const AST::BinaryExpression &>(node).operation.type == TT_BEGINBRACKET &&
         !static_cast<const AST::BinaryExpression &>(node).evaluate_in_order;
}

// The variable an assignment target names: `a` for both `a` and `a[i]`.
static std::string_view AssignedName(const AST::Node &node) {
  const AST::Node *root = &node;
  if (root->type == AST::NodeType::BINARY_EXPRESSION &&
      static_cast<const AST::BinaryExpression &>(*root).operation.type == TT_BEGINBRACKET)
    root = static_cast<const AST::BinaryExpression &>(*root).left.get();
  if (root->type != AST::NodeType::IDENTIFIER) return {};
  return static_cast<const AST::IdentifierAccess &>(*root).name.content;
}

// Runtime hook for an assignment to a built-in array, or null.
const char *AST::CppPrettyPrinter::AssignHook(const AST::Node &target) const {
  // GM8 stores these view variables as integers: an assignment rounds.
  static const std::set<std::string, std::less<>> kIntViews = {
      "view_xview", "view_yview", "view_wview", "view_hview", "view_xport", "view_yport",
      "view_wport", "view_hport", "view_hborder", "view_vborder", "view_hspeed", "view_vspeed"};
  if (!GmlDialect()) return nullptr;
  const std::string_view name = AssignedName(target);
  if (name == "background_index") return "enigma::gml_background_index_assigned(";
  if (Gm8Compliance() && kIntViews.count(name)) return "enigma::gml_round_view(";
  return nullptr;
}

// a[i] / a[i, j] in GML: GM8 rounds indices; side-effecting indices bind left to right.
bool AST::CppPrettyPrinter::PrintGmlSubscript(AST::BinaryExpression &node) {
  std::vector<AST::PNode *> indices{&node.right};
  if (node.right->type == AST::NodeType::BINARY_EXPRESSION) {
    auto *comma = node.right->As<AST::BinaryExpression>();
    if (comma->operation.type == TT_COMMA) indices = {&comma->left, &comma->right};
  }
  const bool gm8 = Gm8Compliance();
  auto print_index = [&](AST::PNode &index) {
    if (gm8) print("enigma::gml_index(");
    if (!Visit(index)) return false;
    if (gm8) print(")");
    return true;
  };
  if (node.evaluate_in_order) {
    print("[&]() -> decltype(auto) { ");
    for (std::size_t i = 0; i < indices.size(); ++i) {
      print("auto enigma_index" + std::to_string(i) + " = ");
      if (!print_index(*indices[i])) return false;
      print("; ");
    }
    print("return ");
  }
  const bool paren = NeedsParens(node, *node.left, false);
  if (paren) print("(");
  VISIT_AND_CHECK(node.left);
  if (paren) print(")");
  const bool multi = indices.size() > 1;
  print(multi ? "(" : "[");
  for (std::size_t i = 0; i < indices.size(); ++i) {
    if (i) print(", ");
    if (node.evaluate_in_order) print("enigma_index" + std::to_string(i));
    else if (!print_index(*indices[i])) return false;
  }
  print(multi ? ")" : "]");
  if (node.evaluate_in_order) print("; }()");
  return true;
}

void AST::CppPrettyPrinter::PrintSemiColon(AST::PNode &node) {
  if (node->type != AST::NodeType::BLOCK && node->type != AST::NodeType::IF && node->type != AST::NodeType::FOR &&
      node->type != AST::NodeType::CASE && node->type != AST::NodeType::DEFAULT &&
      node->type != AST::NodeType::SWITCH && node->type != AST::NodeType::WHILE && node->type != AST::NodeType::DO &&
      node->type != AST::NodeType::WITH) {
    print("; ");
  }
}

std::string AST::CppPrettyPrinter::GetPrintedCode() {
  of->close();
  std::ifstream file("./CompilerSource/parsing/output.txt");
  std::string code = "";

  if (file.is_open()) {
    std::string line = "";
    while (getline(file, line)) {
      code += line;
    }
  }

  return code;
}

namespace {
// Collects every declared name in a code tree. The printer runs this before
// printing a script: declared names are C++ locals in the emitted function
// (GML `var` hoists and shadows instance variables), so the identifier
// lowering must print them bare.
struct DeclaredNameCollector : AST::Visitor {
  std::set<std::string, std::less<>> *out;
  explicit DeclaredNameCollector(std::set<std::string, std::less<>> *out): out(out) {}
  bool VisitDeclarationStatement(AST::DeclarationStatement &node) final {
    if (node.clause) {
      for (auto &d : node.clause->declarators)
        if (!d->name.content.empty()) out->insert(std::string(d->name.content));
    }
    node.RecursiveSubVisit(*this);
    return false;
  }
};
}  // namespace

void AST::CppPrettyPrinter::CollectDeclaredNames(AST::Node &root) {
  DeclaredNameCollector collector(&declared_names_);
  root.RecurusiveVisit(collector);
}

namespace {
bool IsHoistableVar(AST::DeclarationStatement &node) {
  if (node.storage_class != AST::DeclarationStatement::StorageClass::TEMPORARY) return false;
  if (!node.clause || !node.clause->specifiers) return false;
  jdi::definition *def = node.clause->specifiers->to_jdi_fulltype().def;
  if (!def || def->name != "var") return false;
  for (auto &d : node.clause->declarators) {
    if (d->name.content.empty()) return false;
    if (d->declarator_expr && d->declarator_expr->type != AST::NodeType::IDENTIFIER) return false;
    if (d->init && d->init->kind != AST::Initializer::Kind::ASSIGN) return false;
  }
  return true;
}

struct VarCollector : AST::Visitor {
  std::set<std::string, std::less<>> *out;
  explicit VarCollector(std::set<std::string, std::less<>> *out): out(out) {}
  bool VisitDeclarationStatement(AST::DeclarationStatement &node) final {
    if (IsHoistableVar(node))
      for (auto &d : node.clause->declarators) out->insert(std::string(d->name.content));
    return true;
  }
};
}  // namespace

void AST::CppPrettyPrinter::HoistVarDeclarations(AST::Node &root) {
  VarCollector collector(&hoisted_vars_);
  root.RecurusiveVisit(collector);
  if (hoisted_vars_.empty()) return;
  std::string decl = "    var";
  for (const std::string &name : hoisted_vars_) decl += (decl.size() > 7 ? ", " : " ") + name;
  print(decl + ";\n");
}

bool AST::CppPrettyPrinter::VisitIdentifierAccess(AST::IdentifierAccess &node) {
  if (print_type) print("auto ");
  std::string name = node.name.content;
  if (in_declarator_ || declared_names_.count(name)) {
    print(name);
    return true;
  }
  if (is_script && name != "self") {
    if (language_fe->is_shared_local(name)) {
      print("enigma::glaccess(int(self))->" + name);
    } else if (language_fe->global_exists(name)) {
      print(name);
    } else if (node.def) {
      print(name);
    } else if (name.substr(0, 8) == "argument") {
      print(name);
    } else {
      print("enigma::varaccess_" + name + "(int(self))");
    }
  } else {
    print(name);
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitScopeAccess(AST::ScopeAccess &node) {
  using AccessKind = AST::ScopeAccess::AccessKind;
  std::string name(node.name.content);

  // The annotator's classification wins. VARACCESS lowers chains naturally:
  // the lhs visit emits its own lowering inside the accessor's argument.
  switch (node.access_kind) {
    case AccessKind::MEMBER:
      if (node.lhs) VISIT_AND_CHECK(node.lhs);
      print("." + name);
      return true;
    case AccessKind::LOCAL:
      print(name);
      return true;
    case AccessKind::GLOBAL:
      print("enigma::varaccess_" + name + "(int(global))");
      return true;
    case AccessKind::VARACCESS:
      print("enigma::varaccess_" + name + "(");
      if (node.lhs) VISIT_AND_CHECK(node.lhs);
      print(")");
      return true;
    case AccessKind::SHARED:
      // Built-in tier locals: one generated accessor returns the tier
      // pointer; the member access is direct.
      print("enigma::glaccess(int(");
      if (node.lhs) VISIT_AND_CHECK(node.lhs);
      print("))->" + name);
      return true;
    case AccessKind::UNRESOLVED:
      break;
  }

  // Unannotated dot (unit harnesses, pre-annotation prints): the spelling
  // heuristics the annotator supersedes.
  if (node.op.type == TT_DOT) {
    if (node.lhs && node.lhs->type == AST::NodeType::IDENTIFIER) {
      std::string left(node.lhs->As<AST::IdentifierAccess>()->name.content);
      if (left == "local") {
        print(name);
        return true;
      }
      if (left == "global") {
        print("enigma::varaccess_" + name + "(int(global))");
        return true;
      }
      print("enigma::varaccess_" + name + "(" + left + ")");
      return true;
    }
    print("enigma::varaccess_" + name + "(");
    if (node.lhs) VISIT_AND_CHECK(node.lhs);
    print(")");
    return true;
  }

  if (node.lhs) VISIT_AND_CHECK(node.lhs);
  print("::");
  print(name);
  return true;
}

bool AST::CppPrettyPrinter::VisitTemplateId(AST::TemplateId &node) {
  VISIT_AND_CHECK(node.name);
  print("<");
  for (std::size_t i = 0; i < node.args.size(); i++) {
    if (i) print(", ");
    VISIT_AND_CHECK(node.args[i]);
  }
  print(">");
  return true;
}

bool AST::CppPrettyPrinter::VisitDecltype(AST::Decltype &node) {
  print("decltype(");
  VISIT_AND_CHECK(node.operand);
  print(")");
  return true;
}

bool AST::CppPrettyPrinter::VisitImplicitType(AST::ImplicitType &node) {
  // Implied `int` prints nothing: the user didn't write it, and C++ accepts
  // the bare sign/length run (`unsigned x`). The untyped fallback (var/variant)
  // MUST print -- `const x` is not valid C++ -- using the resolved definition's
  // name. An unresolved fallback (header-less harness) prints nothing.
  if (node.kind == AST::ImplicitType::Kind::UNTYPED && node.def) {
    print(node.def->name);
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitLiteral(AST::Literal &node) {
  std::string value = std::get<std::string>(node.value.value);
  if (node.value.type != TT_CHARLIT && node.value.type != TT_STRINGLIT) {
    // Prefixed-literal tokens hold bare digits; restore the C++ spelling.
    // Octal gets a plain leading zero: C++ has no 0o prefix.
    if (node.value.type == TT_HEXLITERAL) {
      print("0x");
    } else if (node.value.type == TT_BINLITERAL) {
      print("0b");
    } else if (node.value.type == TT_OCTLITERAL) {
      print("0");
    }
    print(value);
    return true;
  }
  enigma::parsing::TokenType type = node.value.type;
  if (type == TT_CHARLIT && value.size() > 1) {
    type = TT_STRINGLIT;
  }
  print(type == TT_CHARLIT ? "'" : "\"");
  std::string to_print;
  for (char c : value) {
    if (c == '\\') {
      to_print += "\\\\";
    } else if (c == '"' || c == '\'') {
      // The literal's own delimiter must be escaped or it truncates the
      // emitted string; escaping both quotes is valid C++ either way.
      to_print += '\\';
      to_print += c;
    } else if (c >= ' ' && c <= '~') {
      to_print += c;
    } else if (c == '\n') {
      to_print += "\\n";
    } else if (c == '\t') {
      to_print += "\\t";
    } else if (c == '\v') {
      to_print += "\\v";
    } else if (c == '\b') {
      to_print += "\\b";
    } else if (c == '\r') {
      to_print += "\\r";
    } else if (c == '\f') {
      to_print += "\\f";
    } else if (c == '\a') {
      to_print += "\\a";
    } else if (c == '\?') {
      to_print += "\\?";
    } else {
      // Bytes, not (possibly signed) chars: 0x80-0xFF must stay three octal
      // digits, and the fixed width keeps a following digit character from
      // extending the escape.
      std::ostringstream oss;
      oss << '\\' << std::setw(3) << std::setfill('0') << std::oct
          << static_cast<int>(static_cast<unsigned char>(c));
      to_print += oss.str();
    }
  }
  print(to_print);
  print(type == TT_CHARLIT ? "'" : "\"");

  return true;
}

bool AST::CppPrettyPrinter::VisitParenthetical(AST::Parenthetical &node) {
  print("(");
  if (node.expression) {
    VISIT_AND_CHECK(node.expression);
  }
  print(")");
  return true;
}

bool AST::CppPrettyPrinter::VisitUnaryPostfixExpression(AST::UnaryPostfixExpression &node) {
  VISIT_AND_CHECK(node.operand);
  print(node.operation.token);
  return true;
}

bool AST::CppPrettyPrinter::VisitUnaryPrefixExpression(AST::UnaryPrefixExpression &node) {
  if (node.operation.type == TT_TILDE && GmlDialect()) {
    print("enigma::gml_bitnot(");
    VISIT_AND_CHECK(node.operand);
    print(")");
    return true;
  }
  print(node.operation.type == TT_NOT ? "!" : node.operation.token);
  if (node.operation.type == TT_NOT || node.operation.type == TT_BANG) return VisitCondition(node.operand);
  VISIT_AND_CHECK(node.operand);
  return true;
}

bool AST::CppPrettyPrinter::VisitDeleteExpression(AST::DeleteExpression &node) {
  if (node.is_global) {
    print("::");
  }
  print("delete ");
  if (node.is_array) {
    print("[] ");
  }

  VISIT_AND_CHECK(node.expression);

  return true;
}

namespace {
// Counts an enclosing break/continue target while its body prints.
struct TargetScope {
  int &breaks, &continues;
  bool loop;
  TargetScope(int &b, int &c, bool loop) : breaks(b), continues(c), loop(loop) { ++breaks, continues += loop; }
  ~TargetScope() { --breaks, continues -= loop; }
};
}  // namespace

bool AST::CppPrettyPrinter::VisitBreakStatement(AST::BreakStatement &node) {
  if (!break_targets_ && GmlDialect()) {  // GM8: outside any loop, break exits
    print("return 0");
    return true;
  }
  print("break");
  if (node.count) {
    print(" ");
    VISIT_AND_CHECK(node.count);
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitContinueStatement(AST::ContinueStatement &node) {
  if (!continue_targets_ && GmlDialect()) {  // GM8: outside any loop, continue exits
    print("return 0");
    return true;
  }
  print("continue");
  if (node.count) {
    print(" ");
    VISIT_AND_CHECK(node.count);
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitWithStatement(AST::WithStatement &node) {
  TargetScope scope(break_targets_, continue_targets_, true);
  print("with");
  if (node.object->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }
  VISIT_AND_CHECK(node.object);
  if (node.object->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }

  // Bare names in the body belong to the iterated instance, so resolve them
  // through self as script code does, not as members of this object.
  const bool saved_is_script = is_script;
  if (language_fe) is_script = true;
  VISIT_AND_CHECK(node.body);
  is_script = saved_is_script;
  PrintSemiColon(node.body);

  return true;
}

// Whether C++ would regroup this operand when printed flat, as when the tree
// was parsed with GML precedence (kGmlBinaryPrec).
static bool NeedsParens(const AST::BinaryExpression &parent, const AST::Node &operand, bool right) {
  if (operand.type != AST::NodeType::BINARY_EXPRESSION) return false;
  const auto &child = static_cast<const AST::BinaryExpression &>(operand);
  // Value-position = prints flat on purpose (see lower_gml_equals below).
  if (parent.lower_gml_equals || child.lower_gml_equals) return false;
  if (parent.operation.type == TT_BEGINBRACKET) return false;
  auto p = Precedence::kBinaryPrec.find(parent.operation.type);
  auto c = Precedence::kBinaryPrec.find(child.operation.type);
  if (p == Precedence::kBinaryPrec.end() || c == Precedence::kBinaryPrec.end()) return false;
  if (c->second.precedence != p->second.precedence) return c->second.precedence > p->second.precedence;
  return right == (p->second.associativity == Associativity::LTR);
}

bool AST::CppPrettyPrinter::VisitBinaryExpression(AST::BinaryExpression &node) {
  if (node.operation.type == TT_BEGINBRACKET && !in_declarator_ &&
      (node.evaluate_in_order || Gm8Compliance())) {
    return PrintGmlSubscript(node);
  }
  const bool ordered = node.evaluate_in_order;
  const TokenType op_type = node.operation.type;
  const bool assigns = op_type == TT_ASSIGN || op_type == TT_ASSOP ||
                       (op_type == TT_EQUALS && !node.lower_gml_equals);
  if (const char *hook = assigns && !in_assign_hook_ ? AssignHook(*node.left) : nullptr) {
    in_assign_hook_ = true;
    print(hook);
    const bool ok = VisitBinaryExpression(node);
    print(")");
    in_assign_hook_ = false;
    return ok;
  }
  // A plain read is copied (a later operand may change it); an assignment
  // target stays a reference.
  const bool copy_lhs = ordered && !assigns && IsPlainRead(*node.left);
  if (ordered) {
    print(std::string("[&]() -> decltype(auto) { ") + (copy_lhs ? "auto" : "auto&&") + " enigma_lhs = ");
    VISIT_AND_CHECK(node.left);
    print("; return ");
  }
  auto is_string_literal = [](const PNode &n) {
    return n->type == AST::NodeType::LITERAL &&
           static_cast<const AST::Literal &>(*n).value.type == TT_STRINGLIT;
  };
  // "a" + "b" would add two C++ pointers, "B" < "a" compare them; make the left
  // one a string.
  static const std::set<TokenType> kStringPairOps = {
      TT_PLUS, TT_EQUALS, TT_EQUALTO, TT_NOTEQUAL, TT_LESS, TT_GREATER, TT_LESSEQUAL, TT_GREATEREQUAL};
  const bool string_pair = kStringPairOps.count(node.operation.type) &&
                           is_string_literal(node.left) && is_string_literal(node.right);
  // GM8: && || ^^ take their operands' truth (>= 0.5).
  const bool truth_operands = (node.operation.type == TT_AND || node.operation.type == TT_OR ||
                               node.operation.type == TT_XOR) && Gm8Compliance();
  auto print_operand = [&](PNode &operand, bool right) {
    if (string_pair && !right) {
      print("std::string{");
      if (!Visit(operand)) return false;
      print("}");
      return true;
    }
    if (ordered && !right) {
      print(copy_lhs ? "enigma_lhs" : "std::forward<decltype(enigma_lhs)>(enigma_lhs)");
      return true;
    }
    const bool paren = NeedsParens(node, *operand, right);
    if (paren) print("(");
    if (!Visit(operand)) return false;
    if (paren) print(")");
    return true;
  };
  auto visit_operand = [&](PNode &operand, bool right) {
    if (!truth_operands) return print_operand(operand, right);
    print("enigma::gml_truth(");
    if (!print_operand(operand, right)) return false;
    print(")");
    return true;
  };
  // GML bitwise operands are reals: a C++ `double & int` doesn't compile and an
  // int `1 << 40` overflows. The helpers convert to 64-bit integers (GM8: rounding).
  static const std::map<TokenType, std::string> kGmlBitwise = {
      {TT_AMPERSAND, "enigma::gml_bitand("}, {TT_PIPE, "enigma::gml_bitor("},
      {TT_CARET, "enigma::gml_bitxor("}, {TT_LSH, "enigma::gml_shl("}, {TT_RSH, "enigma::gml_shr("}};
  if (auto op = kGmlBitwise.find(node.operation.type); op != kGmlBitwise.end() && GmlDialect()) {
    print(op->second);
    if (!visit_operand(node.left, false)) return false;
    print(", ");
    VISIT_AND_CHECK(node.right);
    print(")");
    if (ordered) print("; }()");
    return true;
  }
  // GM8 computes both operands, left first, even when the left decides.
  if (truth_operands) {
    print("enigma::gml_both{");
    if (!visit_operand(node.left, false)) return false;
    print(", ");
    if (!visit_operand(node.right, true)) return false;
    print(node.operation.type == TT_AND ? "}.all()" : node.operation.type == TT_OR ? "}.any()" : "}.one()");
    if (ordered) print("; }()");
    return true;
  }
  if (node.operation.type == TT_XOR) {  // C++ has no logical xor
    print("(bool(");
    if (!visit_operand(node.left, false)) return false;
    print(") != bool(");
    VISIT_AND_CHECK(node.right);
    print("))");
    if (ordered) print("; }()");
    return true;
  }
  if (!visit_operand(node.left, false)) return false;

  std::string operation = node.operation.token;
  bool is_multi_dim = false;
  if (node.operation.type == TT_BEGINBRACKET) {
    if (node.right->type == AST::NodeType::BINARY_EXPRESSION) {
      auto bin = node.right->As<AST::BinaryExpression>();
      if (bin->operation.type == TT_COMMA) {
        is_multi_dim = true;
        operation = "(";
      }
    }
  }

  if (operation == ":=") operation = "=";
  // GML value-position =. The operands print flat (no tree parens), so the
  // == re-associates at C++ equality precedence -- which is GML's own
  // grouping for =, regardless of how the assignment-precedence parse
  // grouped the tree.
  if (node.lower_gml_equals) operation = "==";
  print(" " + operation + " ");

  // The annotator marks EDL divisions: / is real division regardless of
  // operand types (1/4 is 0.25; div is the integer kind), so coerce.
  // The rhs of / is never a lower-precedence subtree unparenthesized,
  // so the cast binds to the whole printed operand.
  if (node.lower_real_division && node.operation.type == TT_SLASH) {
    print("(double) ");
  }

  // A subscript's index is a value expression even inside a declarator
  // (`var x[count]`): only the spine declares.
  bool saved_decl = in_declarator_;
  if (node.operation.type == TT_BEGINBRACKET) in_declarator_ = false;
  if (!visit_operand(node.right, true)) return false;
  in_declarator_ = saved_decl;

  if (is_multi_dim) {
    print(")");
  } else if (node.operation.type == TT_BEGINBRACKET) {
    print("]");
  }
  if (ordered) print("; }()");
  return true;
}

bool AST::CppPrettyPrinter::VisitFunctionCallExpression(AST::FunctionCallExpression &node) {
  const bool ordered = node.evaluate_in_order;
  if (ordered) {
    print("[&]() -> decltype(auto) { ");
    for (std::size_t i = 0; i < node.arguments.size(); i++) {
      // A plain read is copied: a later argument may change the variable.
      print(std::string(IsPlainRead(*node.arguments[i]) ? "auto" : "auto&&") +
            " enigma_arg" + std::to_string(i) + " = ");
      VISIT_AND_CHECK(node.arguments[i]);
      print("; ");
    }
    print("return ");
  }
  // A called name is a function or script, never a variable to lower.
  // GML round() is GM8-aware (half to even); engine code keeps std::round.
  if (node.function->type == AST::NodeType::IDENTIFIER) {
    std::string name(node.function->As<AST::IdentifierAccess>()->name.content);
    print(name == "round" && GmlDialect() ? "enigma::gml_round" : name);
  }
  else
    VISIT_AND_CHECK(node.function);
  print("(");

  bool is_variadic = false;
  int variadic_index = 0;
  if (node.function->type == AST::NodeType::IDENTIFIER && language_fe) {
    auto fn = node.function->As<AST::IdentifierAccess>();
    jdi::definition *def = fn->def;
    if (def && language_fe->is_variadic_function(def)) {
      is_variadic = true;
      variadic_index = language_fe->function_variadic_after((jdi::definition_function *)def);
    }
  }

  bool varargs_opened = false;
  for (std::size_t i = 0; i < node.arguments.size(); i++) {
    if (is_variadic && i == std::size_t(variadic_index)) {
      print("(enigma::varargs(),");
      varargs_opened = true;
    }
    if (ordered) {
      const std::string arg = "enigma_arg" + std::to_string(i);
      print(IsPlainRead(*node.arguments[i]) ? arg : "std::forward<decltype(" + arg + ")>(" + arg + ")");
    } else {
      VISIT_AND_CHECK(node.arguments[i]);
    }
    if (i < node.arguments.size() - 1) {
      print(", ");
    }
  }

  // Close the varargs wrapper only if it opened: a variadic function called
  // with no variadic arguments never reaches the opening index.
  if (varargs_opened) print(")");

  print(")");
  if (ordered) print("; }()");
  return true;
}

bool AST::CppPrettyPrinter::VisitTernaryExpression(AST::TernaryExpression &node) {
  if (!VisitCondition(node.condition)) return false;
  print(" ? ");

  VISIT_AND_CHECK(node.true_expression);
  print(" : ");

  VISIT_AND_CHECK(node.false_expression);
  return true;
}

bool AST::CppPrettyPrinter::VisitLambdaExpression(AST::LambdaExpression &node) {
  print("[&]");

  if (node.parameters->type == AST::NodeType::IDENTIFIER) {
    print("(");
  }
  print_type = true;
  VISIT_AND_CHECK(node.parameters);
  print_type = false;
  if (node.parameters->type == AST::NodeType::IDENTIFIER) {
    print(")");
  }

  if (node.body->type != AST::NodeType::BLOCK) {
    print("{");
  }
  VISIT_AND_CHECK(node.body);
  PrintSemiColon(node.body);
  if (node.body->type != AST::NodeType::BLOCK) {
    print("}");
  }

  return true;
}

bool AST::CppPrettyPrinter::VisitReturnStatement(AST::ReturnStatement &node) {
  print("return ");
  if (node.expression) {
    VISIT_AND_CHECK(node.expression);
  }
  if (node.is_exit) {
    print("0");
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitSizeofExpression(AST::SizeofExpression &node) {
  // `sizeof expr` takes no parens; `sizeof(type)` and `sizeof...(pack)` require
  // them. A redundantly-parenthesised operand (`sizeof(x)`) round-trips via the
  // operand's own Parenthetical node, so EXPR never synthesises parens.
  print("sizeof");
  switch (node.kind) {
    case AST::SizeofExpression::Kind::EXPR:
      print(" ");
      if (node.argument) VISIT_AND_CHECK(node.argument);
      break;
    case AST::SizeofExpression::Kind::VARIADIC:
      print("...(");
      if (node.argument) VISIT_AND_CHECK(node.argument);
      print(")");
      break;
    case AST::SizeofExpression::Kind::TYPE:
      print("(");
      if (node.argument) VISIT_AND_CHECK(node.argument);
      print(")");
      break;
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitAlignofExpression(AST::AlignofExpression &node) {
  print("alignof(");
  if (node.type) {
    VISIT_AND_CHECK(node.type);
  }
  print(")");
  return true;
}

bool AST::CppPrettyPrinter::VisitDeclSpecList(AST::DeclSpecList &node) {
  // Source-order replay; consumers expect "unsigned long const" to print as it
  // was written (or as the parser normalized it).
  for (std::size_t i = 0; i < node.specs.size(); ++i) {
    if (i > 0) print(" ");
    print(std::string{node.specs[i].content});
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitTypeSpecifierSeq(AST::TypeSpecifierSeq &node) {
  // Prints the shared part of a type — decl-specs + base type name. Per-
  // declaration declarator chains (`*x`, `[10]`) are printed by their
  // owning Declaration via VisitFullType with print_type=false.
  if (node.declspecs && !node.declspecs->specs.empty()) {
    if (!node.declspecs->accept(*this)) return false;
    print(" ");
  }
  // The base type is the id-expression tree (type-name leaf, qualified-id,
  // template-id, ...); printing it preserves qualification/template-args the
  // bare definition name would drop. An inferred base type is an ImplicitType
  // leaf: implied `int` renders nothing (the user didn't write it, and C++
  // accepts the bare spec run), the untyped var/variant fallback renders its
  // definition's name.
  if (node.id_expression) {
    VISIT_AND_CHECK(node.id_expression);
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitDeclaratorClause(AST::DeclaratorClause &node) {
  // type-specifier-seq, then each declarator as an expression tree. An abstract
  // declarator's tree bottoms out in an empty-name leaf, which prints as
  // nothing, so a bare type-id (`int`) renders just its specifiers.
  if (node.specifiers && !node.specifiers->accept(*this)) return false;
  for (std::size_t i = 0; i < node.declarators.size(); ++i) {
    if (i > 0) print(",");
    auto &decl = *node.declarators[i];
    if (decl.declarator_expr) {
      print(" ");
      if (!decl.declarator_expr->accept(*this)) return false;
    }
    if (decl.init && !decl.init->accept(*this)) return false;
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitCastExpression(AST::CastExpression &node) {
  // Functional casts now live in Initializer (with a TypeSpecifierSeq target); not handled here.
  if (node.kind == AST::CastExpression::Kind::C_STYLE) {
    print("(");
    if (node.type) VISIT_AND_CHECK(node.type);
    print(")");
  } else {
    switch (node.kind) {
      case AST::CastExpression::Kind::STATIC:      print("static_cast<");      break;
      case AST::CastExpression::Kind::DYNAMIC:     print("dynamic_cast<");     break;
      case AST::CastExpression::Kind::CONST:       print("const_cast<");       break;
      case AST::CastExpression::Kind::REINTERPRET: print("reinterpret_cast<"); break;
      default: break;
    }
    if (node.type) VISIT_AND_CHECK(node.type);
    print(">(");
  }

  if (node.expr) {
    VISIT_AND_CHECK(node.expr);
  }

  if (node.kind != AST::CastExpression::Kind::C_STYLE) {
    print(")");
  }

  return true;
}

bool AST::CppPrettyPrinter::VisitArray(AST::Array &node) {
  print("[");
  for (std::size_t i = 0; i < node.elements.size(); ++i) {
    if (i > 0) print(", ");
    VISIT_AND_CHECK(node.elements[i]);
  }
  print("]");
  return true;
}

bool AST::CppPrettyPrinter::VisitInitializer(AST::Initializer &node) {
  if (node.kind == AST::Initializer::Kind::ASSIGN) {
    // target is the optional designator (`.name`); values[0] is the value.
    if (node.target) {
      print(".");
      VISIT_AND_CHECK(node.target);
    }
    print(" = ");
    if (!node.values.empty()) {
      VISIT_AND_CHECK(node.values[0]);
    }
    return true;
  }

  if (node.kind == AST::Initializer::Kind::EXPR) {
    if (!node.values.empty()) {
      VISIT_AND_CHECK(node.values[0]);
    }
    return true;
  }

  // BRACE or PAREN. A non-null target represents a functional-cast / temporary
  // construction: TypeSpecifierSeq( values... ) or TypeSpecifierSeq{ values... }.
  if (node.target) {
    VISIT_AND_CHECK(node.target);
  }
  print(node.kind == AST::Initializer::Kind::PAREN ? "(" : "{");
  for (std::size_t i = 0; i < node.values.size(); ++i) {
    if (i > 0) print(", ");
    VISIT_AND_CHECK(node.values[i]);
  }
  print(node.kind == AST::Initializer::Kind::PAREN ? ")" : "}");
  return true;
}

bool AST::CppPrettyPrinter::VisitNewExpression(AST::NewExpression &node) {
  if (node.is_global) {
    print("::");
  }

  print("new ");

  if (!node.placement_args.empty()) {
    print("(");
    for (size_t i = 0; i < node.placement_args.size(); ++i) {
      if (i > 0) print(", ");
      VISIT_AND_CHECK(node.placement_args[i]);
    }
    print(") ");
  }

  // Parenthesised type-id (`new (int*)`): VisitDeclaratorClause prints the
  // type-specifier-seq + the declarator as its expression tree (abstract leaf
  // prints as nothing, ptr-ops/array bounds as `*`/`[n]`).
  print("(");
  if (node.type) {
    // Identical to VISIT_AND_CHECK modulo PNode covariance issue: node.type is a
    // unique_ptr<DeclaratorClause>, which won't bind to Visit(PNode&).
    if (!node.type->accept(*this)) return false;
  }
  print(")");

  if (node.initializer) {
    if (!VisitInitializer(*node.initializer)) return false;
  }

  return true;
}

bool AST::CppPrettyPrinter::VisitDeclarationStatement(AST::DeclarationStatement &node) {
  if (!hoisted_vars_.empty() && IsHoistableVar(node)) {
    bool printed = false;
    for (auto &entry : node.clause->declarators) {
      if (!entry->init) continue;
      if (printed) print(", ");
      print(std::string(entry->name.content));
      if (!VisitInitializer(*entry->init)) return false;
      printed = true;
    }
    return true;
  }
  bool is_global = node.storage_class == DeclarationStatement::StorageClass::GLOBAL;
  bool is_local = node.storage_class == DeclarationStatement::StorageClass::LOCAL;
  if (is_global || is_local) {
    // GLOBAL/LOCAL print the access-shim assignment, not the declaration form
    // itself — so this fork doesn't go through VisitInitDeclarator.
    bool printed = false;
    for (auto &entry : node.clause->declarators) {
      if (entry->init) {
        if (printed) print(", ");
        std::string name(entry->name.content);
        if (is_global)
          print("enigma::varaccess_" + name + "(int(global))");
        else
          print(name);
        // The Initializer owns the separator (ASSIGN prints " = v").
        if (!VisitInitializer(*entry->init)) return false;
        printed = true;
      }
    }
    return true;
  }

  // The shared type (declspecs + base type name) prints once via VisitTypeSpecifierSeq.
  // Each init-declarator then contributes its own declarator chain + init via
  // VisitInitDeclarator.
  if (node.clause && node.clause->specifiers) {
    // Identical to VISIT_AND_CHECK modulo PNode covariance issue
    if (!node.clause->specifiers->accept(*this)) return false;
    print(" ");
  }
  for (std::size_t i = 0; i < node.clause->declarators.size(); i++) {
    if (!VisitInitDeclarator(*node.clause->declarators[i])) return false;
    if (i != node.clause->declarators.size() - 1) {
      print(", ");
    }
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitInitDeclarator(AST::InitDeclarator &node) {
  // Print the declarator from the AST-layer expression-tree. A null tree means
  // a name-only declarator (no pointer/array/function modifiers); fall back to
  // the declared name. Abstract declarators have neither and print nothing.
  if (node.declarator_expr) {
    // Identifiers in the declarator are declared names, not value reads; the
    // script lowering must not rewrite them.
    bool saved = in_declarator_;
    in_declarator_ = true;
    bool ok = node.declarator_expr->accept(*this);
    in_declarator_ = saved;
    if (!ok) return false;
  } else if (!node.name.content.empty()) {
    print(std::string(node.name.content));
  }
  if (node.init) {
    // The Initializer owns its full rendering, including any separator: ASSIGN
    // prints " = v", brace/paren print "{...}"/"(...)" with no '='. Copy-list-init
    // ("= {...}") round-tripping the literal '=' requires recording copy-vs-direct
    // on the node, which is tracked separately as init-form fidelity.
    if (!VisitInitializer(*node.init)) return false;
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitCode(AST::CodeBlock &node) {
  for (auto &stmt : node.statements) {
    print("    ");
    VISIT_AND_CHECK(stmt);
    PrintSemiColon(stmt);
    print("\n");
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitCodeBlock(AST::CodeBlock &node) {
  print("{\n");
  if (!VisitCode(node)) return false;
  print("}");
  return true;
}

bool AST::CppPrettyPrinter::VisitIfStatement(AST::IfStatement &node) {
  print("if");
  if (node.not_condition) print("(!");
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  if (!VisitCondition(node.condition)) return false;

  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  if (node.not_condition) print(")");

  print(" ");
  if (node.true_branch) {
    VISIT_AND_CHECK(node.true_branch);
    PrintSemiColon(node.true_branch);
  } else {
    print(";");
  }
  print(" ");

  if (node.false_branch) {
    print("else ");
    VISIT_AND_CHECK(node.false_branch);
    PrintSemiColon(node.false_branch);
    print(" ");
  }

  return true;
}

bool AST::CppPrettyPrinter::VisitForLoop(AST::ForLoop &node) {
  TargetScope scope(break_targets_, continue_targets_, true);
  print("for(");

  // Omitted clauses are null.
  if (node.assignment) VISIT_AND_CHECK(node.assignment);
  print("; ");

  if (node.condition && !VisitCondition(node.condition)) return false;
  print("; ");

  if (node.increment) VISIT_AND_CHECK(node.increment);
  print(") ");

  if (node.body) {
    VISIT_AND_CHECK(node.body);
    PrintSemiColon(node.body);
  } else {
    print(";");
  }
  print(" ");

  return true;
}

bool AST::CppPrettyPrinter::VisitCaseStatement(AST::CaseStatement &node) {
  print("case ");
  VISIT_AND_CHECK(node.value);

  print(": ");
  if (!VisitCodeBlock(*node.statements->As<AST::CodeBlock>())) return false;
  print(" ");

  return true;
}

bool AST::CppPrettyPrinter::VisitDefaultStatement(AST::DefaultStatement &node) {
  print("default: ");
  if (!VisitCodeBlock(*node.statements->As<AST::CodeBlock>())) return false;
  print(" ");
  return true;
}

bool AST::CppPrettyPrinter::VisitSwitchStatement(AST::SwitchStatement &node) {
  TargetScope scope(break_targets_, continue_targets_, false);
  if (node.lower_gml_switch) {
    // Pick the first matching case by ==, then switch on its index so
    // fallthrough and break keep their meaning.
    print("{ const variant enigma_switch_value = (");
    VISIT_AND_CHECK(node.expression);
    print("); switch (");
    // GM8 checks labels in order, default included: reaching default matches,
    // even when a later case would. Otherwise default is the no-match target.
    const bool ordered_default = Gm8Compliance();
    auto numbered = [&](const AST::PNode &stmt) {
      return stmt->type == AST::NodeType::CASE ||
             (ordered_default && stmt->type == AST::NodeType::DEFAULT);
    };
    int index = 0;
    bool reached_default = false;
    for (auto &stmt : node.body->statements) {
      if (!numbered(stmt)) continue;
      if (stmt->type == AST::NodeType::DEFAULT) {
        print(std::to_string(++index));
        reached_default = true;
        break;
      }
      print("enigma_switch_value == (");
      VISIT_AND_CHECK(stmt->As<AST::CaseStatement>()->value);
      print(") ? " + std::to_string(++index) + " : ");
    }
    if (!reached_default) print("0");
    print(") { ");
    index = 0;
    for (auto &stmt : node.body->statements) {
      if (numbered(stmt)) {
        print("case " + std::to_string(++index) + ": ");
        auto &body = stmt->type == AST::NodeType::CASE ? stmt->As<AST::CaseStatement>()->statements
                                                       : stmt->As<AST::DefaultStatement>()->statements;
        if (!VisitCodeBlock(*body)) return false;
        print(" ");
      } else {
        VISIT_AND_CHECK(stmt);
      }
    }
    print("} }");
    return true;
  }
  print("switch(int(");
  VISIT_AND_CHECK(node.expression);
  print(")) ");

  if (!VisitCodeBlock(*node.body->As<AST::CodeBlock>())) return false;
  print(" ");

  return true;
}

bool AST::CppPrettyPrinter::VisitWhileLoop(AST::WhileLoop &node) {
  TargetScope scope(break_targets_, continue_targets_, true);
  if (node.kind == AST::WhileLoop::Kind::REPEAT) {
    // Lower `repeat (N) body` to a counted while. The pair is braced so it
    // acts as ONE statement wherever the repeat sits (an unbraced loop or if
    // body would otherwise detach the while), and the block scopes the
    // counter; nesting levels get numbered counters so an inner repeat's
    // condition can still read outer locals without shadowing surprises.
    std::string counter = "strange_name";
    if (repeat_depth_ > 0) counter += std::to_string(repeat_depth_);
    ++repeat_depth_;
    // The count converts like an index (GM8 rounds it); a negative one runs 0 times.
    print("{ int " + counter + " = enigma::gml_index(");
    VISIT_AND_CHECK(node.condition);
    print("); while(" + counter + "-- > 0) ");
    VISIT_AND_CHECK(node.body);
    PrintSemiColon(node.body);
    print(" }");
    --repeat_depth_;
    return true;
  }

  print("while");
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  if (node.kind == AST::WhileLoop::Kind::UNTIL) {
    if (node.condition->type == AST::NodeType::PARENTHETICAL) {
      print("(!");
    } else {
      print("!(");
    }
  }

  if (!VisitCondition(node.condition)) return false;

  if (node.kind == AST::WhileLoop::Kind::UNTIL) {
    print(")");
  }
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }

  print(" ");
  VISIT_AND_CHECK(node.body);
  PrintSemiColon(node.body);

  return true;
}

bool AST::CppPrettyPrinter::VisitDoLoop(AST::DoLoop &node) {
  TargetScope scope(break_targets_, continue_targets_, true);
  print("do");

  if (node.body->type != AST::NodeType::BLOCK) {
    print("{");
  }

  VISIT_AND_CHECK(node.body);
  PrintSemiColon(node.body);

  if (node.body->type != AST::NodeType::BLOCK) {
    print("}");
  }

  print("while");
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  if (node.is_until) {
    if (node.condition->type == AST::NodeType::PARENTHETICAL) {
      print("(!");
    } else {
      print("!(");
    }
  }

  if (!VisitCondition(node.condition)) return false;

  if (node.is_until) {
    print(")");
  }
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  print(";");

  return true;
}
