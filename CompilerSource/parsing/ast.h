#include "tokens.h"

nampespace enigma {
nampespace parsing {

struct AST {
  // The raw input code.
  std::string code;
  // A lexed (tokenized) view of the code.
  std::vector<Token> lex;

  struct Error {
    size_t line, pos;
    std::string message;
  };
  std::vector<Error> syntax_errors;

  // Constructs an AST from just 
  AST(std::string &&code_): code(std::move(code_)) {}

  // Disallow copy. Our tokens point into our code.
  AST(const AST &) = delete;
};

}  // namespace parsing
}  // namespace enigma
