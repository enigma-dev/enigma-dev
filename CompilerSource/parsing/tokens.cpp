#include "tokens.h"

#include <string>
#include <sstream>
#include <vector>

namespace enigma {
namespace parsing {
namespace {

using std::string;

static const std::vector<std::string> kTokenNames = [](){
  std::vector<std::string> res;
  res.resize(TT_ERROR + 1);
  #define REGISTER(name) [[fallthrough]]; case name: res[name] = #name
  switch (TT_ENDOFCODE) {
    default:
    REGISTER(TT_ENDOFCODE);
    REGISTER(TT_ERROR);
    REGISTER(TT_VARNAME);
    REGISTER(TT_SEMICOLON);
    REGISTER(TT_COLON);
    REGISTER(TT_COMMA);
    REGISTER(TT_ASSOP);
    REGISTER(TT_DECIMAL);
    REGISTER(TT_PLUS);
    REGISTER(TT_MINUS);
    REGISTER(TT_INCREMENT);
    REGISTER(TT_DECREMENT);
    REGISTER(TT_STAR);
    REGISTER(TT_SLASH);
    REGISTER(TT_LESS);
    REGISTER(TT_GREATER);
    REGISTER(TT_LESSEQUAL);
    REGISTER(TT_GREATEREQUAL);
    REGISTER(TT_NOTEQUAL);
    REGISTER(TT_LSH);
    REGISTER(TT_RSH);
    REGISTER(TT_QMARK);
    REGISTER(TT_BEGINPARENTH);
    REGISTER(TT_ENDPARENTH);
    REGISTER(TT_BEGINBRACKET);
    REGISTER(TT_ENDBRACKET);
    REGISTER(TT_BEGINBRACE);
    REGISTER(TT_ENDBRACE);
    REGISTER(TT_BEGINTRIANGLE);
    REGISTER(TT_ENDTRIANGLE);
    REGISTER(TT_DIGIT);
    REGISTER(TT_STRING);
    REGISTER(TT_SCOPEACCESS);
    REGISTER(TT_FUNCTION);
    REGISTER(TT_TYPE_NAME);
    REGISTER(TT_NAMESPACE);
    REGISTER(TT_LOCGLOBAL);
    REGISTER(TT_SHORTSTATEMENT);
    REGISTER(TT_TINYSTATEMENT);
    REGISTER(TT_S_SWITCH);
    REGISTER(TT_S_CASE);
    REGISTER(TT_S_DEFAULT);
    REGISTER(TT_S_FOR);
    REGISTER(TT_S_IF);
    REGISTER(TT_S_ELSE);
    REGISTER(TT_S_DO);
    REGISTER(TT_S_WHILE);
    REGISTER(TT_S_UNTIL);
    REGISTER(TT_S_WITH);
    REGISTER(TT_S_TRY);
    REGISTER(TT_S_CATCH);
    REGISTER(TT_S_NEW);
  }
  return res;
}();

}  // namespace

string Token::ToString() const {
  std::stringstream str;
  str << kTokenNames[type] << "{\"" << content << "\", ";
  str << (antiquated.separator ? "separated, " : "unseparated, ");
  str << (antiquated.breakandfollow ? "breakandfollow, " : "nobreak, ");
  str << (antiquated.operatorlike ? "operatorlike, " : "notoperator, ");
  str << antiquated.macrolevel;
  str << "}";
  return str.str();
}

}  // namespace parsing
}  // namespace enigma
