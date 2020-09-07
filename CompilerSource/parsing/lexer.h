#ifndef ENIGMA_COMPILER_PARSING_LEXER_h
#define ENIGMA_COMPILER_PARSING_LEXER_h

#include "tokens.h"
#include "macros.h"

#include <string>
#include <deque>

namespace enigma {
namespace parsing {

class Lexer {
 public:
  Token ReadToken();
  Lexer(std::string &&code_): code(code_) {}

  const std::string &GetCode();

 private:
  struct OpenMacro {
    TokenVector tokens;
    size_t index = 0;
  };

  void PopMacro();

  std::string code;
  size_t pos = 0;
  std::deque<OpenMacro> open_macros;
};

}  // namespace parsing
}  // namespace enigma

#endif
