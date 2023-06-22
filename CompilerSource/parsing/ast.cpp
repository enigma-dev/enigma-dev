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
#include "parser/parser.h"  // print_to_file, parser_main
#include "parser/collect_variables.h"  // collect_variables

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

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
