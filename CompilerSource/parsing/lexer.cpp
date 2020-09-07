#include "lexer.h"

#include <array>
#include <memory>
#include <initializer_list>

namespace enigma {
namespace parsing {
namespace {

class TokenTrie {
 public:
  TokenType Get(std::string_view chars) {
    if (chars.empty() || !children_) return token_;
    TokenType child = Child(chars[0]).Get(chars.substr(1));
    return child != TT_ERROR ? child : token_;
  }
  void Put(std::string_view key, TokenType token) {
    if (key.empty()) {
      token_ = token;
      return;
    }
    if (!children_) children_ = std::make_unique<std::array<TokenTrie, 256>>();
    Child(key[0]).Put(key.substr(1), token);
  }
  TokenTrie &Child(char ch) { return (*children_)[uint8_t(ch)]; }
  const TokenTrie &Child(char ch) const { return (*children_)[uint8_t(ch)]; }
  struct TokenPair { std::string_view key; TokenType value; };
  TokenTrie(std::initializer_list<TokenPair> pairs) {
    for (const TokenPair &pair : pairs) Put(pair.key, pair.value);
  }

 private:
  TokenType token_ = TT_ERROR;
  std::unique_ptr<std::array<TokenTrie, 256>> children_;
};

static TokenTrie token_lookup {
  { "!",   TT_BANG         },
  { "!=",  TT_NOTEQUAL     },
  { "%",   TT_PERCENT      },
  { "%=",  TT_ASSOP        },
  { "&",   TT_AMPERSAND    },
  { "&&",  TT_AND          },
  { "(",   TT_BEGINPARENTH },
  { ")",   TT_ENDPARENTH   },
  { "*",   TT_STAR         },
  { "*=",  TT_ASSOP        },
  { "+",   TT_PLUS         },
  { "++",  TT_INCREMENT    },
  { "+=",  TT_ASSOP        },
  { ",",   TT_COMMA        },
  { "-",   TT_MINUS        },
  { "--",  TT_DECREMENT    },
  { "-=",  TT_ASSOP        },
  { ".",   TT_DECIMAL      },
  { ":",   TT_COLON,       },
  { "::",  TT_SCOPEACCESS  },
  { ":=",  TT_ASSIGN,      },
  { ";",   TT_SEMICOLON    },
  { "<",   TT_LESS         },
  { "<<",  TT_LSH          },
  { "<<=", TT_ASSOP        },
  { "<=",  TT_LESSEQUAL    },
  { "<>",  TT_NOTEQUAL     },
  { "=",   TT_EQUALS,      },
  { "==",  TT_EQUALTO,     },
  { ">",   TT_GREATER      },
  { ">=",  TT_GREATEREQUAL },
  { ">>",  TT_RSH          },
  { ">>=", TT_ASSOP        },
  { "?",   TT_TERNARY      },
  { "[",   TT_BEGINBRACKET },
  { "]",   TT_ENDBRACKET   },
  { "^",   TT_CARET        },
  { "^^",  TT_XOR          },
  { "{",   TT_BEGINBRACE   },
  { "|",   TT_PIPE         },
  { "||",  TT_OR           },
  { "}",   TT_ENDBRACE     },
  { "~",   TT_TILDE        },
};

}  // namespace

Token Lexer::ReadToken() {
  if (!open_macros.empty()) {
    auto &macro = open_macros.back();
    if (macro.index >= macro.tokens.size()) {
      PopMacro();
      return ReadToken();
    }
    return macro.tokens[macro.index++];
  }

  if (pos >= code.length()) return TT_ENDOFCODE;

  if (isspace(code[pos])) {
    while (++pos < code.length() && isspace(code[pos]));
    return ReadToken();
  }

  if (isletter(code[pos])) {
    const pt spos = pos;
    while (is_letterd(code[++pos]));
    string name = code.substr(spos,pos-spos);

    //Is this in our blacklist?
    if (blacklist.find(name) != blacklist.end()) {
      const string newname = name + "__________";  // There's better ways of doing this.
      code.replace(spos, pos-spos, newname);
      pos += newname.size() - name.size();
      name = newname;
    }

    // First, check if it's a macro.
    jdi::macro_iter_c itm = main_context->get_macros().find(name);
    if (itm != main_context->get_macros().end()) {
      if (!macro_recurses(name,mymacrostack,mymacroind)) {
        string macrostr;
        if (itm->second->argc != -1) {
          vector<string> mpvec;
          jdip::macro_function* mf = (jdip::macro_function*)itm->second;
          jdip::lexer_cpp::parse_macro_params(mf, main_context->get_macros(), code.c_str(), pos, code.length(), mpvec, jdip::token_t(), jdi::def_error_handler);
          char *mss, *mse; mf->parse(mpvec, mss, mse, jdip::token_t());
          macrostr = string (mss, mse);
        }
        else
          macrostr = ((jdip::macro_scalar*)itm->second)->value;
        mymacrostack[mymacroind++].grab(name,code,pos);
        code = macrostr; pos = 0;
        return ReadToken();
      }
    }

    if (is_wordop(name)) { //this is actually a word-based operator, such as `and', `or', and `not'
      bool unary = name[0] == 'n'; //not is the only unary word operator.
      return Token(unary ? TT_UNARYPRE : TT_OPERATOR, name, superPos, name.length(), false, false, true, mymacroind);
    }

    if (shared_object_locals.find(name) == shared_object_locals.end())
    {
      jdi::definition *d = current_language->look_up(name);
      if (d)
      {
        // Handle typenames
        if (d->flags & jdi::DEF_TYPENAME) {
          return Token(TT_TYPE_NAME, d, name, superPos, name.length(), false, false, false, mymacroind);
        }

        if (d->flags & jdi::DEF_NAMESPACE) {
          return Token(TT_NAMESPACE, d, name, superPos, name.length(), false, false, false, mymacroind);
        }

        if (current_language->definition_is_function(d)) {
          return Token(TT_FUNCTION, d, name, superPos, name.length(), false, true, false, mymacroind);
        }
        // Global variables
        return Token(TT_VARNAME, d, name, superPos, name.length(), false, true, false, mymacroind);
      }

      if (is_statement(name)) // Our control statements
      {
        if (!lex[lexlast].separator and !lex[lexlast].operatorlike)
        {
          if (lex[lexlast].breakandfollow)
            lex.push_back(Token(TT_IMPLICIT_SEMICOLON, ";", superPos, 0, true, false, false, mymacroind));
          else if (lex[lexlast].type != TT_S_ELSE and lex[lexlast].type != TT_S_TRY)
          {
            ptrace();
            syerr = "Unexpected `" + name + "' statement at this point";
            return superPos;
          }
        }
        TT mt = statement_type(name);
        return Token(mt, name, superPos, name.length(), false, mt == TT_TINYSTATEMENT, mt != TT_TINYSTATEMENT, mymacroind);
      }
    }
    return Token(TT_VARNAME, name, superPos, name.length(), false, true, false, mymacroind);
  }

  if (is_digit(code[pos])) {
    const pt spos = pos;
    while (is_digit(code[++pos]));
    return Token(TT_DIGIT, code.substr(spos,pos-spos), superPos, pos-spos, false, true, false, mymacroind);
  }

  const pt spos = pos;
  switch (code[pos++]) {
    case '$':
      if (!is_nybble(code[pos])) {
        syerr = "Unclosed double quote at this point";
      }
    case '"': {
      spos = pos;
      if (setting::use_cpp_escapes)
        while (code[++pos]!='"') {
          if (pos >= code.length()) {
            syerr = "Unclosed double quote at this point";
            return superPos;
          }
          if (code[pos] == '\\')
            pos++;
        }
      else
        while (code[++pos]!='"');
      return Token(TT_STRING, code, pos, pos - spos);
    }
    '\'':
      spos = pos;
      if (setting::use_cpp_escapes)
        while (code[++pos] != '\'') {
          if (pos >= code.length()) {
            syerr = "Unclosed quote at this point";
            return superPos;
          }
          if (code[pos] == '\\')
            pos++;
        }
      else
        while (code[++pos]!='\'');
      return Token(TT_STRING, code, spos, ++pos - spos);

    case '0':
      if (setting::use_cpp_literals) {
        if (code[pos] == 'x') {
          while (is_nybble(code[++pos]));
          return Token(TT_HEXLITERAL, code, spos, pos - spos);
        }
        if (code[pos] == 'b') {
          while (is_bit(code[++pos]));
          return Token(TT_BINLITERAL, code, spos, pos - spos);
        }
        if (code[pos] == 'o') {
          while (is_octal(code[++pos]));
          return Token(TT_OCTLITERAL, code, spos, pos - spos);
        }
      }
      while (is_digit(code[++pos]));
      return Token(TT_HEXLITERAL, code, spos, pos - spos);

    case '/':
      if (code[pos+1] == '/') { // Two-slash comments
        while (++pos < code.length() and code[pos] != '\n' and code[pos] != '\r');
        continue;
      }
      if (code[pos+1] == '*') { ++pos; // GM /**/ Comments
        while (++pos < code.length() and (code[pos] != '/' or code[pos-1] != '*'));
        pos++; continue;
      }
      break;
  }
}

}  // namespace parsing
}  // namespace enigma
