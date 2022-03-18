#include "ast.h"
#include "parser/parser.h"  // print_to_file, parser_main
#include "parser/collect_variables.h"  // collect_variables

#include <string>
#include <memory>
#include <vector>

namespace {

using namespace enigma::parsing;

enum class Associativity {
  LTR, // Left-to-right; a + b + c + d   →   (((a + b) + c) + d)
  RTL, // Right-to-left; a = b = c = d   →   (a = (b = (c = d)))
};
struct OperatorPrecedence {
  int precedence;
  Associativity associativity;
};

struct Precedence {
  constexpr static int
      kScope = 1,         // Scope resolution; LTR-parsed.
      kUnaryPostfix = 2,  // Suffix/postfix increment and decrement; LTR-parsed.
      kTypeInit = kUnaryPostfix,      // Functional cast; LTR-parsed.
      kFuncCall = kUnaryPostfix,      // Function call; LTR-parsed.
      kSubscript = kUnaryPostfix,     // Subscript; LTR-parsed.
      kMemberAccess = kUnaryPostfix,  // Member access; LTR-parsed.
      kUnaryPrefix = 3,          // Prefix increment and decrement, unary plus and minus, logical and bitwise NOT, C-style cast, Dereference, Address-of; RTL-parsed.
      kSizeOf = kUnaryPrefix,    // sizeof; RTL-parsed.
      kAwait = kUnaryPrefix,     // await-expression; RTL-parsed.
      kNew = kUnaryPrefix,       // Dynamic memory allocation; RTL-parsed.
      kDelete = kUnaryPrefix,    // Dynamic memory deallocation; RTL-parsed.
      kMemberPointer = 4,        // Pointer-to-member (a.*b, a->*b); LTR-parsed.
      kMultiply = 5,             // Multiplication (a*b); LTR-parsed.
      kDivide = kMultiply,       // Division (a/b); LTR-parsed.
      kModulo = kMultiply,       // Modulus (a%b); LTR-parsed.
      kAdd = 6,                  // Addition (a+b); LTR-parsed.
      kSubtract = kAdd,          // Subtraction (a-b); LTR-parsed.
      kShift = 7,                // Bitwise left shift and right shift(a<<b, a>>b); LTR-parsed.
      kThreeWayComp = 8,         // Three-way comparison operator (a<=>b); LTR-parsed.
      kRelational = 9,           // For relational operators < and ≤ and > and ≥ respectively  (a<b, a<=b, a>b, a>=b); LTR-parsed.
      kEquality = 10,            // For equality operators = and ≠ alike (a==b, a!=b); LTR-parsed.
      kBitAnd = 11,              // Bitwise AND (a&b); LTR-parsed.
      kBitXOr = 12,              // Bitwise XOR (a^b); LTR-parsed.
      kBitOr = 13,               // Bitwise OR (a|b); LTR-parsed.
      kBoolAnd = 14,             // Logical AND (a&&b); LTR-parsed.
      kBoolOr = 15,              // Logical OR (a||b); LTR-parsed.
      kTernary = 16,             // Ternary conditional (a?b:c); RTL-parsed.
      kThrow = 16,               // throw operator; RTL-parsed.
      kYield = 16,               // yield-expression (co_yield); RTL-parsed.
      kAssign = 16,              // Direct assignment (provided by default for C++ classes) (a=b); RTL-parsed.
      kCompoundAssign = kAssign, // Compound assignment by sum and difference (a+=b, a-=b, a*=b, a/=b, a%=b, a<<=b, a>>=b, a&=b, a^=b, a|=b); RTL-parsed.
      kComma = 17,               // Comma (a,b); LTR-parsed.
      kAll = 100;                // Everything, including comma.

  constexpr static int kMin = 0;
};

class AstBuilder {

Lexer *lexer;
ErrorHandler *herr;
Token token;

std::unique_ptr<AST::Node> TryParseExpression(int precedence) {
  token = lexer->ReadToken();
  switch (token.type) {
    case TT_SEMICOLON:
      herr->ReportWarning(token, "Statement doesn't do anything (consider using `{}` instead of `;`)");
      return std::make_unique<AST::CodeBlock>();
    case TT_COLON:
      herr->ReportError(token, "Expected label or ternary expression before colon");
      return nullptr;
    case TT_COMMA:
      herr->ReportError(token, "Expected expression before comma");
      return nullptr;
    case TT_ASSIGN:
    case TT_ASSOP:
      herr->ReportError(token, "Expected assignable expression before assignment operator");
      return nullptr;
    case TT_DOT:
      herr->ReportError(token, "Expected expression before dot");
      return nullptr;
    case TT_PERCENT:
    case TT_PIPE:
    case TT_CARET:
    case TT_AND:
    case TT_OR:
    case TT_XOR:
    case TT_DIV:
    case TT_MOD:
    case TT_EQUALS:
    case TT_SLASH:
    case TT_EQUALTO:
    case TT_NOTEQUAL:
    case TT_LESS:
    case TT_GREATER:
    case TT_LESSEQUAL:
    case TT_GREATEREQUAL:
    case TT_LSH:
    case TT_RSH:
      herr->Error(token) << "Expected expression before binary operator `" << token.content << '`';
      return nullptr;

    case TT_QMARK:
      herr->Error(token) << "Expected expression before unary operator ?";
      return nullptr;

    case TT_NOT:
    case TT_BANG:
    case TT_PLUS:
    case TT_MINUS:
    case TT_STAR:
    case TT_AMPERSAND:
    case TT_TILDE:
    case TT_INCREMENT:
    case TT_DECREMENT: {
      Token unary_op = token;
      token = lexer->ReadToken();
      if (auto exp = TryParseExpression(Precedence::kUnaryPrefix))
        return std::make_unique<AST::UnaryExpression>(std::move(exp), unary_op.type);
      herr->Error(unary_op) << "Expected expression following unary operator";
    }

    case TT_BEGINPARENTH: {
      token = lexer->ReadToken();
      auto exp = TryParseExpression(Precedence::kMin);
      if (token.type != TT_ENDPARENTH) {
        herr->Error(token) << "Expected closing parenthesis before " << token;
      }
      return std::make_unique<AST::Parenthetical>(std::move(exp));
    }
    case TT_BEGINBRACKET: {
      token = lexer->ReadToken();
      std::vector<std::unique_ptr<AST::Node>> elements;
      while (std::unique_ptr<AST::Node> element = TryParseExpression(Precedence::kComma)) {
        elements.push_back(std::move(element));
        if (token.type != TT_COMMA) break;
        token = lexer->ReadToken();
      }
      if (token.type != TT_ENDBRACKET) {
        herr->Error(token) << "Expected closing `]` for array";
      }
      return std::make_unique<AST::Array>(std::move(elements));
    }
    case TT_BEGINBRACE:
    case TT_BEGINTRIANGLE:

    case TT_IDENTIFIER:
    case TT_DECLITERAL: case TT_BINLITERAL: case TT_OCTLITERAL:
    case TT_HEXLITERAL: case TT_STRINGLIT: case TT_CHARLIT:
    
    case TT_SCOPEACCESS:

    case TT_TYPE_NAME:

    case TT_LOCAL:
    case TT_GLOBAL:
    case TT_RETURN:
    case TT_EXIT:
    case TT_BREAK:
    case TT_CONTINUE:
    case TT_S_SWITCH:
    case TT_S_REPEAT:
    case TT_S_CASE:
    case TT_S_DEFAULT:
    case TT_S_FOR:
    case TT_S_IF:
    case TT_S_THEN:
    case TT_S_ELSE:
    case TT_S_DO:
    case TT_S_WHILE:
    case TT_S_UNTIL:
    case TT_S_WITH:
    case TT_S_TRY:
    case TT_S_CATCH:
    case TT_S_NEW:
    case TT_S_DELETE:
    case TT_CLASS:
    case TT_STRUCT:
    case TTM_WHITESPACE:
    case TTM_CONCAT:
    case TTM_STRINGIFY:

    case TT_ERROR:
    case TT_ENDOFCODE:
    case TT_ENDPARENTH:
    case TT_ENDBRACKET:
    case TT_ENDBRACE:
    case TT_ENDTRIANGLE:
      return nullptr;
  }
}

/// Reads if()/for()/while()/with() statements.
template<typename ExpNode>
std::unique_ptr<ExpNode> ReadConditionalStatement() {
  
}

std::unique_ptr<AST::Node> TryReadStatement() {
  token = lexer->ReadToken();
  switch (token.type) {
    case TT_SEMICOLON:
      herr->ReportWarning(token, "Statement doesn't do anything (consider using `{}` instead of `;`)");
      return std::make_unique<AST::CodeBlock>();
    case TT_COLON:
      herr->ReportError(token, "Expected label or ternary expression before colon");
      return nullptr;
    case TT_COMMA:
      herr->ReportError(token, "Expected expression before comma");
      return nullptr;
    case TT_ASSIGN:
    case TT_ASSOP:
      herr->ReportError(token, "Expected assignable expression before assignment operator");
      return nullptr;
    case TT_DOT:
    case TT_PLUS:
    case TT_MINUS:
    case TT_STAR:
    case TT_PERCENT:
    case TT_AMPERSAND:
    case TT_PIPE:
    case TT_CARET:
    case TT_AND:
    case TT_OR:
    case TT_XOR:
    case TT_DIV:
    case TT_MOD:
    case TT_NOT:
    case TT_SLASH:
    case TT_EQUALS:
    case TT_EQUALTO:
    case TT_NOTEQUAL:
    case TT_BANG:
    case TT_TILDE:
    case TT_INCREMENT:
    case TT_DECREMENT:
    case TT_LESS:
    case TT_GREATER:
    case TT_LESSEQUAL:
    case TT_GREATEREQUAL:
    case TT_LSH:
    case TT_RSH:
    case TT_QMARK:
    case TT_BEGINPARENTH:
    case TT_ENDPARENTH:
    case TT_BEGINBRACKET:
    case TT_ENDBRACKET:
    case TT_BEGINBRACE:
    case TT_ENDBRACE:
    case TT_BEGINTRIANGLE:
    case TT_ENDTRIANGLE:

    case TT_DECLITERAL: case TT_BINLITERAL: case TT_OCTLITERAL:
    case TT_HEXLITERAL: case TT_STRINGLIT: case TT_CHARLIT:
    case TT_SCOPEACCESS:
    case TT_TYPE_NAME:
    case TT_LOCAL:
    case TT_GLOBAL:
    case TT_RETURN:
    case TT_EXIT:
    case TT_BREAK:
    case TT_CONTINUE:
    case TT_S_SWITCH:
    case TT_S_REPEAT:
    case TT_S_CASE:
    case TT_S_DEFAULT:
    case TT_S_FOR:
    case TT_S_IF:
    case TT_S_THEN:
    case TT_S_ELSE:
    case TT_S_DO:
    case TT_S_WHILE:
    case TT_S_UNTIL:
    case TT_S_WITH:
    case TT_S_TRY:
    case TT_S_CATCH:
    case TT_S_NEW:
    case TT_S_DELETE:
    case TT_CLASS:
    case TT_STRUCT:
    case TTM_WHITESPACE:
    case TTM_CONCAT:
    case TTM_STRINGIFY:
    case TT_ERROR:
    case TT_ENDOFCODE:
    ;
  }
}
std::unique_ptr<AST::CodeBlock> ParseCodeBlock() {
}
std::unique_ptr<AST::BinaryExpression> ParseBinaryExpression() {
  
}
std::unique_ptr<AST::UnaryExpression> ParseUnaryExpression() {
  
}
std::unique_ptr<AST::TernaryExpression> ParseTernaryExpression() {
  
}
std::unique_ptr<AST::IfStatement> ParseIfStatement() {
  
}
std::unique_ptr<AST::ForLoop> ParseForLoop() {
  
}
std::unique_ptr<AST::WhileLoop> ParseWhileLoop() {
  
}
std::unique_ptr<AST::DoLoop> ParseDoLoop() {
  
}
std::unique_ptr<AST::ReturnStatement> ParseReturnStatement() {
  
}
std::unique_ptr<AST::BreakStatement> ParseBreakStatement() {
  
}
std::unique_ptr<AST::BreakStatement> ParseContinueStatement() {
  
}

};  // class AstBuilder

}

namespace enigma::parsing {

bool AST::empty() const { return junkshit.code.empty(); }  // HACK

// Utility routine: Apply this AST to a specified instance.
void AST::ApplyTo(int instance_id) {
  apply_to_ = instance_id;
}

void AST::PrettyPrint(std::ofstream &of, int base_indent) const {
  if (apply_to_) {
    of << std::string(base_indent, ' ') << "with (" << *apply_to_ << ") {\n";
    print_to_file(lexer->GetContext(), junkshit.code, junkshit.synt,
                  junkshit.strc, junkshit.strs, base_indent, of);
    of << std::string(base_indent, ' ') << "}\n";
  } else {
    print_to_file(lexer->GetContext(), junkshit.code, junkshit.synt,
                  junkshit.strc, junkshit.strs, base_indent, of);
  }
}

AST AST::Parse(std::string code, const ParseContext *ctex) {
  AST res(std::move(code), ctex);
  // For now, we're not building the actual AST.
  // We should probably do some syntax checking, though...
  parser_main(&res, *ctex);
  return res;
}

void AST::ExtractDeclarations(ParsedScope *destination_scope) {
  std::cout << "collecting variables..." << std::flush;
  const ParseContext &ctex = lexer->GetContext();
  collect_variables(ctex.language_fe, this, destination_scope, ctex.script_names);
  std::cout << " done." << std::endl;
}

}  // namespace enigma::parsing
