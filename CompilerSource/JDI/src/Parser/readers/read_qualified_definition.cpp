#include <Parser/bodies.h>
#include <API/compile_settings.h>

using namespace jdip;
definition* jdip::read_qualified_definition(lexer *lex, definition_scope* scope, token_t &token, context_parser *cp, error_handler *herr)
{
  definition *res = NULL;
  if (token.type == TT_SCOPE) {
    token = lex->get_token_in_scope(scope);
    res = token.def = cp->get_global();
  }
  for (;;) {
    if (token.def->flags & DEF_TEMPLATE)
    {
      res = token.def;
      definition_template* dt = (definition_template*)res;
      if (dt->def->flags & DEF_CLASS)
      {
        token = lex->get_token_in_scope(scope, herr);
        if (token.type == TT_LESSTHAN)
        {
          arg_key k(dt->params.size());
          if (read_template_parameters(k, dt, lex, token, scope, cp, herr))
            return FATAL_TERNARY(NULL,res);
          res = dt->instantiate(k);
          token = lex->get_token_in_scope(scope,herr);
        }
      }
      else {
        token.report_error(herr, "Template `" + token.def->name + "' cannot be used as a type");
        //cerr << token.def->toString();
        return FATAL_TERNARY(NULL,res);
      }
    }
    else if (token.def->flags & DEF_SCOPE)
    {
      definition_scope* as = (definition_scope*)token.def;
      token = lex->get_token_in_scope(scope, herr);
      if (token.type != TT_SCOPE)
        break;
      token = lex->get_token_in_scope(as, herr);
      if (token.type != TT_DEFINITION and token.type != TT_DECLARATOR) {
        if (token.type == TT_OPERATORKW) {
          res = token.def = as->look_up(read_operatorkw_name(lex, token, scope, herr));
          if (!token.def)
            return FATAL_TERNARY(NULL,res);
          if (token.type != TT_SCOPE)
            break;
          continue;
        }
        else {
          token.report_errorf(herr, "Expected type or qualified-id before %s");
          return FATAL_TERNARY(NULL,res);
        }
      }
      res = token.def;
      continue;
    }
    else {
      token = lex->get_token_in_scope(scope, herr);
      break;
    }
    
    if (token.type == TT_SCOPE) {
      #ifdef DEBUG_MODE
        if (!res) { token.report_error(herr, "Accessing NULL scope..."); return NULL; }
        if (!(res->flags & DEF_SCOPE)) { token.report_error(herr, "Accessing non-scope object " + res->name + "..."); return NULL; }
      #endif
      token = lex->get_token_in_scope((definition_scope*)res, herr);
      if (token.type != TT_DEFINITION and token.type != TT_DECLARATOR) {
        token.report_errorf(herr, "Expected qualified-id following `::' before %s");
        return NULL;
      }
      else
        res = token.def;
    }
    else break;
  }
  return res;
}
