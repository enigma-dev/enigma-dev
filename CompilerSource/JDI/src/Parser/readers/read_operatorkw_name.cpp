#include <Parser/bodies.h>
#include <API/compile_settings.h>

using namespace jdi;

string jdip::read_operatorkw_name(lexer* lex, token_t &token, definition_scope *scope, error_handler *herr)
{
  string res;
  token = lex->get_token_in_scope(scope, herr);
  if (token.type == TT_OPERATOR or token.type == TT_LESSTHAN or token.type == TT_GREATERTHAN or token.type == TT_TILDE) {
    res = "operator" + token.content.toString();
    token = lex->get_token_in_scope(scope, herr);
  }
  else if (token.type == TT_LEFTBRACKET) {
    res = "operator[]";
    token = lex->get_token_in_scope(scope, herr);
    if (token.type != TT_RIGHTBRACKET) {
      token.report_error(herr, "Expected closing bracket for `operator[]' definition");
      FATAL_RETURN("");
    }
    else
      token = lex->get_token_in_scope(scope, herr);
  }
  else if (token.type == TT_LEFTPARENTH) {
    res = "operator()";
    token = lex->get_token_in_scope(scope, herr);
    if (token.type != TT_RIGHTPARENTH) {
      token.report_error(herr, "Expected closing parenthesis for `operator()' definition");
      FATAL_RETURN("");
    }
    else
      token = lex->get_token_in_scope(scope, herr);
  }
  else if (token.type == TT_NEW) {
    token = lex->get_token_in_scope(scope, herr);
    if (token.type == TT_LEFTBRACKET) {
      token = lex->get_token_in_scope(scope, herr);
      if (token.type != TT_RIGHTBRACKET) {
        token.report_error(herr, "Expected closing bracket for `operator new[]' definition");
        FATAL_RETURN("");
      }
      else
        token = lex->get_token_in_scope(scope, herr);
      res = "operator new[]";
    }
    else
      res = "operator new";
  }
  else if (token.type == TT_DELETE) {
    token = lex->get_token_in_scope(scope, herr);
    if (token.type == TT_LEFTBRACKET) {
      token = lex->get_token_in_scope(scope, herr);
      if (token.type != TT_RIGHTBRACKET) {
        token.report_error(herr, "Expected closing bracket for `operator new[]' definition");
        FATAL_RETURN("");
      }
      else
        token = lex->get_token_in_scope(scope, herr);
      res = "operator delete[]";
    }
    else
      res = "operator delete";
  }
  else {
    token.report_errorf(herr, "Unexpected %s following `operator' keyword; does not form a valid operator");
    FATAL_RETURN("");
  }
  return res;
}
