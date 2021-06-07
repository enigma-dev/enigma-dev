#include "ast.h"
#include "parser/parser.h"  // print_to_file

#include <string>
#include <memory>
#include <vector>

namespace enigma::parsing {

bool AST::empty() const { return junkshit.code.empty(); }  // HACK

// Utility routine: Apply this AST to a specified instance.
void AST::ApplyTo(int instance_id) {
  std::cerr << "TODO: implement me: AST::ApplyTo" << std::endl;
}

void AST::PrettyPrint(std::ofstream &of, int base_indent) const {
  print_to_file(lexer->GetContext(), junkshit.code, junkshit.synt,
                junkshit.strc, junkshit.strs, base_indent, of);
}

AST AST::Parse(std::string code, const ParseContext *ctex) {
  AST res(std::move(code), ctex);
  std::cerr << "TODO: implement me: AST::Parse" << std::endl;
  return res;
}

}  // namespace enigma::parsing
