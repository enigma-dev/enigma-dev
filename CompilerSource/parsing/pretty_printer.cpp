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
  root.accept(collector);
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
  print(node.operation.type == TT_NOT ? "!" : node.operation.token);
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

bool AST::CppPrettyPrinter::VisitBreakStatement(AST::BreakStatement &node) {
  print("break");
  if (node.count) {
    print(" ");
    VISIT_AND_CHECK(node.count);
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitContinueStatement(AST::ContinueStatement &node) {
  print("continue");
  if (node.count) {
    print(" ");
    VISIT_AND_CHECK(node.count);
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitWithStatement(AST::WithStatement &node) {
  print("with");
  if (node.object->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }
  VISIT_AND_CHECK(node.object);
  if (node.object->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }

  VISIT_AND_CHECK(node.body);
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
  auto visit_operand = [&](PNode &operand, bool right) {
    const bool paren = NeedsParens(node, *operand, right);
    if (paren) print("(");
    if (!Visit(operand)) return false;
    if (paren) print(")");
    return true;
  };
  if (node.operation.type == TT_XOR) {  // C++ has no logical xor
    print("(bool(");
    VISIT_AND_CHECK(node.left);
    print(") != bool(");
    VISIT_AND_CHECK(node.right);
    print("))");
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
  return true;
}

bool AST::CppPrettyPrinter::VisitFunctionCallExpression(AST::FunctionCallExpression &node) {
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
    VISIT_AND_CHECK(node.arguments[i]);
    if (i < node.arguments.size() - 1) {
      print(", ");
    }
  }

  // Close the varargs wrapper only if it opened: a variadic function called
  // with no variadic arguments never reaches the opening index.
  if (varargs_opened) print(")");

  print(")");
  return true;
}

bool AST::CppPrettyPrinter::VisitTernaryExpression(AST::TernaryExpression &node) {
  VISIT_AND_CHECK(node.condition);
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

  VISIT_AND_CHECK(node.condition);

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
  print("for(");

  // Omitted clauses are null.
  if (node.assignment) VISIT_AND_CHECK(node.assignment);
  print("; ");

  if (node.condition) VISIT_AND_CHECK(node.condition);
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
  print("switch(int(");
  VISIT_AND_CHECK(node.expression);
  print(")) ");

  if (!VisitCodeBlock(*node.body->As<AST::CodeBlock>())) return false;
  print(" ");

  return true;
}

bool AST::CppPrettyPrinter::VisitWhileLoop(AST::WhileLoop &node) {
  if (node.kind == AST::WhileLoop::Kind::REPEAT) {
    // Lower `repeat (N) body` to a counted while. The pair is braced so it
    // acts as ONE statement wherever the repeat sits (an unbraced loop or if
    // body would otherwise detach the while), and the block scopes the
    // counter; nesting levels get numbered counters so an inner repeat's
    // condition can still read outer locals without shadowing surprises.
    std::string counter = "strange_name";
    if (repeat_depth_ > 0) counter += std::to_string(repeat_depth_);
    ++repeat_depth_;
    print("{ int " + counter + " = ");
    VISIT_AND_CHECK(node.condition);
    print("; while(" + counter + "--) ");
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

  VISIT_AND_CHECK(node.condition);

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

  VISIT_AND_CHECK(node.condition);

  if (node.is_until) {
    print(")");
  }
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  print(";");

  return true;
}
