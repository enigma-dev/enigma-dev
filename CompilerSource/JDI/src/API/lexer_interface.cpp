/// @file  lexer_interface.cpp
/// @brief Microfile to implement the virtual destructor for \c lexer.
#include "lexer_interface.h"
jdi::lexer::~lexer() {}

using namespace jdip;

token_t lexer::get_token_in_scope(jdi::definition_scope *scope, error_handler *herr) {
  token_t res = get_token(herr);
  
  if (res.type == TT_IDENTIFIER) {
    const string name((const char*)res.extra.content.str, res.extra.content.len);
    definition *def = scope->look_up(name);
    if (!def) return res;
    
    if (def->flags & DEF_TYPENAME) {
      res.extra.def = def;
      res.type = TT_DECLARATOR;
      return res;
    }
  }
  
  return res;
}
