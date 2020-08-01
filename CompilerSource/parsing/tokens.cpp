#include "tokens.h"

namespace enigma {
namespace parsing {
namespace {

static const std::string kTokenNames[] = {
  "TT_VARNAME",
  "TT_SEMICOLON",
  "TT_COLON",
  "TT_COMMA",
  "TT_ASSOP",
  "TT_DECIMAL",
  "TT_OPERATOR",
  "TT_UNARYPRE",
  "TT_UNARYPOST",
  "TT_TERNARY",
  "TT_BEGINPARENTH",
  "TT_ENDPARENTH",
  "TT_BEGINBRACKET",
  "TT_ENDBRACKET",
  "TT_BEGINBRACE",
  "TT_ENDBRACE",
  "TT_BEGINTRIANGLE",
  "TT_ENDTRIANGLE",
  "TT_DIGIT",
  "TT_STRING",
  "TT_SCOPEACCESS",
  "TT_FUNCTION",
  "TT_TYPE_NAME",
  "TT_LOCGLOBAL",
  "TT_GEN_STATEMENT",
  "TT_SHORTSTATEMENT",
  "TT_TINYSTATEMENT",
  "TT_S_SWITCH",
  "TT_S_CASE",
  "TT_S_DEFAULT",
  "TT_S_FOR",
  "TT_S_IF",
  "TT_S_ELSE",
  "TT_S_TRY",
  "TT_S_CATCH",
  "TT_S_DO",
  "TT_S_NEW",
  "<implicit semicolon>"
};

}  // namespace

std::string Token::ToString() const {
  string str = string(TTN[type]) + "{" + buf + ", ";
  sprintf(buf,"%lu",(long unsigned)length); str += buf; str += ", ";
  str += separator ? "separated, " : "unseparated, ";
  str += breakandfollow ? "breakandfollow, " : "nobreak, ";
  str += operatorlike ? "operatorlike, " : "notoperator, ";
  sprintf(buf,"%d",macrolevel); str += buf;
  return str + "}";
}

}  // namespace parsing
}  // namespace enigma
