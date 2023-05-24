#include <Parser/context_parser.h>

using std::make_unique;

bool jdi::context_parser::read_attribute_clause(
    token_t &token, definition_scope *scope) {
  token = lex->get_token_in_scope(scope);
  if (token.type != TT_LEFTPARENTH) {
    token.report_errorf(herr, "Expected (()) after __attribute__");
    return false;
  }
  int parenths = 1;
  while (parenths > 0 && token.type != TT_ENDOFCODE) {
    token = lex->get_token_in_scope(scope);
    if (token.type == TT_LEFTPARENTH) ++parenths;
    else if (token.type == TT_RIGHTPARENTH) --parenths;
  }
  if (parenths) {
    token.report_errorf(herr, "Unclosed parentheses after __attribute__");
    return false;
  }
  // TODO: Attributes like __noreturn__, __always_inline__, etc,
  // are useful to report on functions and such. They don't have
  // a place in the current JDI type schema.
  token = lex->get_token_in_scope(scope);
  return true;
}
