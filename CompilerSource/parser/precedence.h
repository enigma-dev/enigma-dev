#include "../parsing/tokens.h"

#include <unordered_map>
#include <unordered_set>

namespace enigma::parsing {

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
        kMemberAccess = kUnaryPostfix,  // Member access (a.b, a->b); LTR-parsed.
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
        kBoolXOr = 15,              // Logical OR (a||b); LTR-parsed.
        kBoolOr = 16,              // Logical OR (a||b); LTR-parsed.
        kTernary = 17,             // Ternary conditional (a?b:c); RTL-parsed.
        kThrow = 17,               // throw operator; RTL-parsed.
        kYield = 17,               // yield-expression (co_yield); RTL-parsed.
        kAssign = 17,              // Direct assignment (provided by default for C++ classes) (a=b); RTL-parsed.
        kCompoundAssign = kAssign, // Compound assignment (a+=b, a-=b, a*=b, a/=b, a%=b, a<<=b, a>>=b, a&=b, a^=b, a|=b); RTL-parsed.
        kComma = 18,               // Comma (a,b); LTR-parsed.
        kAll = 100,                // Everything, including comma.
        kNone = 999;               // Unknown operator

    constexpr static int kMin = 0;

    static std::unordered_map<TokenType, OperatorPrecedence> kBinaryPrec;
    static std::unordered_map<TokenType, OperatorPrecedence> kUnaryPostfixPrec;
    static std::unordered_map<TokenType, OperatorPrecedence> kTernaryPrec;
    static std::unordered_map<TokenType, OperatorPrecedence> kSpecialPrec;
    static std::unordered_set<TokenType> kUnaryPrefixOps;
  };
  std::unordered_map<TokenType, OperatorPrecedence> Precedence::kBinaryPrec{
      {TT_SCOPEACCESS,  {Precedence::kScope,          Associativity::LTR}},
      {TT_DOT,          {Precedence::kMemberAccess,   Associativity::LTR}},
      {TT_ARROW,        {Precedence::kMemberAccess,   Associativity::LTR}},
      {TT_DOT_STAR,     {Precedence::kMemberPointer,  Associativity::LTR}},
      {TT_ARROW_STAR,   {Precedence::kMemberPointer,  Associativity::LTR}},
      {TT_STAR,         {Precedence::kMultiply,       Associativity::LTR}},
      {TT_SLASH,        {Precedence::kDivide,         Associativity::LTR}},
      {TT_DIV,          {Precedence::kDivide,         Associativity::LTR}},
      {TT_MOD,          {Precedence::kModulo,         Associativity::LTR}},
      {TT_PERCENT,      {Precedence::kModulo,         Associativity::LTR}},
      {TT_PLUS,         {Precedence::kAdd,            Associativity::LTR}},
      {TT_MINUS,        {Precedence::kSubtract,       Associativity::LTR}},
      {TT_LSH,          {Precedence::kShift,          Associativity::LTR}},
      {TT_RSH,          {Precedence::kShift,          Associativity::LTR}},
      {TT_THREEWAY,     {Precedence::kThreeWayComp,   Associativity::LTR}},
      {TT_LESS,         {Precedence::kRelational,     Associativity::LTR}},
      {TT_LESSEQUAL,    {Precedence::kRelational,     Associativity::LTR}},
      {TT_GREATER,      {Precedence::kRelational,     Associativity::LTR}},
      {TT_GREATEREQUAL, {Precedence::kRelational,     Associativity::LTR}},
      {TT_EQUALTO,      {Precedence::kEquality,       Associativity::LTR}},
      {TT_NOTEQUAL,     {Precedence::kEquality,       Associativity::LTR}},
      {TT_AMPERSAND,    {Precedence::kBitAnd,         Associativity::LTR}},
      {TT_CARET,        {Precedence::kBitXOr,         Associativity::LTR}},
      {TT_PIPE,         {Precedence::kBitOr,          Associativity::LTR}},
      {TT_AND,          {Precedence::kBoolAnd,        Associativity::LTR}},
      {TT_XOR,          {Precedence::kBoolXOr,        Associativity::LTR}},
      {TT_OR,           {Precedence::kBoolOr,         Associativity::LTR}},
      {TT_ASSIGN,       {Precedence::kAssign,         Associativity::RTL}},
      {TT_EQUALS,       {Precedence::kAssign,         Associativity::RTL}},
      {TT_ASSOP,        {Precedence::kCompoundAssign, Associativity::RTL}},
      {TT_COMMA,        {Precedence::kComma,          Associativity::LTR}},

      // TODO: TT_THROW as kThrow RTL
      // TODO: TT_ kYield RTL
  };

  std::unordered_map<TokenType, OperatorPrecedence> Precedence::kUnaryPostfixPrec {
      {TT_INCREMENT,    {Precedence::kUnaryPostfix, Associativity::LTR}},
      {TT_DECREMENT,    {Precedence::kUnaryPostfix, Associativity::LTR}},
  };

  std::unordered_map<TokenType, OperatorPrecedence> Precedence::kTernaryPrec {
      {TT_QMARK, {Precedence::kTernary, Associativity::RTL}},
  };

  std::unordered_map<TokenType, OperatorPrecedence> Precedence::kSpecialPrec {
      {TT_BEGINBRACKET, {Precedence::kSubscript, Associativity::LTR}},
      {TT_BEGINPARENTH, {Precedence::kFuncCall,  Associativity::LTR}},
  };

  std::unordered_set<TokenType> Precedence::kUnaryPrefixOps{
      TT_PLUS, TT_MINUS, TT_STAR, TT_AMPERSAND, TT_TILDE, TT_NOT, TT_BANG,
      TT_INCREMENT, TT_DECREMENT
  };
}