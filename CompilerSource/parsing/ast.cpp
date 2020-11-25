#include "ast.h"

#include <string>
#include <memory>
#include <vector>

namespace enigma::parsing {

// Utility routine: Apply this AST to a specified instance.
void AST::ApplyTo(int instance_id) {
  std::cerr << "TODO: implement me: AST::ApplyTo" << std::endl;
}

AST AST::Parse(std::string code, const ParseContext *ctex) {
  AST res(std::move(code), ctex);
  std::cerr << "TODO: implement me: AST::Parse" << std::endl;
  return res;
}

}  // namespace enigma::parsing
