#include "tokens.h"
#include "lexer.h"

#include <string>
#include <memory>
#include <vector>

namespace enigma {
namespace parsing {

struct AST {
  // A lexed (tokenized) view of the code.
  const std::unique_ptr<Lexer> lexer;
  // The raw input code, owned by the lexer.
  const std::string &code;

  struct Error {
    size_t line, pos;
    std::string message;

    std::string ToString();
  };
  std::vector<Error> syntax_errors;

  bool HasError() { return !syntax_errors.empty(); }
  std::string ErrorString() {
    if (syntax_errors.empty()) return "No error";
    return syntax_errors.front().ToString();
  }

  // Utility routine: Apply this AST to a specified instance.
  void ApplyTo(int instance_id);

  // Parses the given code, returning an AST*. The resulting AST* is never null.
  // If syntax errors were encountered, they are stored within the AST.
  static AST Parse(std::string code);

  // Disallow copy. Our tokens point into our code.
  AST(const AST &) = delete;
  AST(AST &&) = default;

 private:
  // Constructs an AST from the code it will parse. Does not initiate parse.
  AST(std::string &&code_):
      lexer(std::make_unique<Lexer>(std::move(code_))),
      code(lexer->GetCode()) {}
};

}  // namespace parsing
}  // namespace enigma
