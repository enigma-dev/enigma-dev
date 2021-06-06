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

#include <Storage/definition.h>

#include "general/parse_basics_old.h"
#include "lexer.h"
#include "settings.h"

#include <array>
#include <memory>
#include <initializer_list>

namespace enigma {
namespace parsing {
namespace {

class TokenTrie {
 public:
  std::pair<TokenType, size_t> Get(std::string_view code, size_t pos) {
    if (pos >= code.length() || !children_) return { token_, pos };
    auto child = Child(code[pos]).Get(code, pos + 1);
    if (child.first != TT_ERROR) return child;
    return { token_, pos };
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

  TokenTrie() = default;
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
  { "?",   TT_QMARK        },
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

static std::map<std::string, TokenType, std::less<>> keyword_lookup {
  { "break",    TT_TINYSTATEMENT  },
  { "case",     TT_S_CASE         },
  { "catch",    TT_S_CATCH        }, 
  { "continue", TT_TINYSTATEMENT  },
  { "default",  TT_S_DEFAULT      },
  { "do",       TT_S_DO           },
  { "else",     TT_S_ELSE         },
  { "exit",     TT_TINYSTATEMENT  },
  { "for",      TT_S_FOR          },
  { "if",       TT_S_IF           },
  { "return",   TT_SHORTSTATEMENT },
  { "repeat",   TT_S_REPEAT       },
  { "switch",   TT_S_SWITCH       },
  { "try",      TT_S_TRY          },
  { "until",    TT_S_UNTIL        },
  { "while",    TT_S_WHILE        },
  { "with",     TT_S_WITH         },

  { "and", TT_AND },
  { "div", TT_DIV },
  { "mod", TT_MOD },
  { "not", TT_NOT },
  { "or",  TT_OR  },
  { "xor", TT_XOR },
};

#define PURE_VIRTUAL(t, x) t x final { \
  std::cerr << "Call to pure virtual " #t " NullLanguage::" #x << std::endl; \
  abort(); \
}

static const MacroMap kNoMacros;

// Used for preprocessing parses. Not the prettiest solution, but works for now.
struct NullLanguageFrontend : LanguageFrontend {
  PURE_VIRTUAL(syntax_error*, definitionsModified(const char*, const char*));

  PURE_VIRTUAL(int,  load_shared_locals());
  PURE_VIRTUAL(void, load_extension_locals());

  PURE_VIRTUAL(void, definition_parameter_bounds(jdi::definition *, unsigned &, unsigned &) const);
  PURE_VIRTUAL(void, quickmember_script(jdi::definition_scope*, string));
  PURE_VIRTUAL(void, quickmember_integer(jdi::definition_scope*, string));

  bool global_exists(string) const final { return false; }
  const MacroMap &builtin_macros() const final { return kNoMacros; }
  jdi::definition* look_up(const string&) const final { return nullptr; }
  jdi::definition* find_typename(string) const final { return nullptr; }

  bool is_variadic_function(jdi::definition *) const final { return false; }
  int function_variadic_after(jdi::definition_function *) const final { return 0; }
  bool definition_is_function(jdi::definition *) const final { return false; }
  size_t definition_overload_count(jdi::definition *) const final { return 0; }

  ~NullLanguageFrontend() {}
} const kNullLanguage;

}  // namespace

ParseContext::ParseContext(nullptr_t): ParseContext(&kNullLanguage) {}

bool Lexer::MacroRecurses(std::string_view name) const {
  for (const auto &macro : open_macros) if (macro.name == name) return true;
  return false;
}

void Lexer::PopMacro() {
  open_macros.pop_back();
}
void Lexer::PushMacro(std::string_view name, TokenVector value) {
  open_macros.emplace_back(name, std::move(value));
}

TokenVector Lexer::PreprocessBuffer(const TokenVector &tokens) {
  TokenVector res;
  Lexer lex(tokens, context, herr);
  for (Token t; (t = lex.ReadToken()).type != TT_ENDOFCODE; res.push_back(t));
  return res;
}

size_t Lexer::ComputeLineNumber(size_t lpos) {
  if (lpos < line_last_evaluated_at_) {
    std::cerr << "INTERNAL ERROR: Lexer asked for a new line position from earlier in the buffer. "
                 "At " << line_last_evaluated_at_ << ", requested " << lpos
                 << std::endl;
    line_last_evaluated_at_ = 0;
    line_number = 1;
  }
  while (line_last_evaluated_at_ < lpos) {
    if (code[line_last_evaluated_at_] == '\n') {
      ++line_number;
      last_line_position = line_last_evaluated_at_++;
      continue;
    }
    if (code[line_last_evaluated_at_] == '\r') {
      ++line_number;
      last_line_position = line_last_evaluated_at_++;
      if (code[line_last_evaluated_at_] == '\n')
        last_line_position = line_last_evaluated_at_++;
      continue;
    }
    // Not at a newline char.
    ++line_last_evaluated_at_;
  }
  return line_number;
}

CodeSnippet Lexer::Mark(size_t pos, size_t length) {
  ComputeLineNumber(pos);
  return CodeSnippet{std::string_view{code}.substr(pos, length),
                     line_number, pos - last_line_position};
}

Token Lexer::ReadRawToken() {
  if (pos >= code.length()) {
    // We need custom logic for this because string_view::substr checks bounds
    // even for zero-width views.
    ComputeLineNumber(pos);
    return Token(TT_ENDOFCODE, CodeSnippet{
                     std::string_view{code.data() + pos, 0},
                     line_number, pos - last_line_position});
  }

  if (isspace(code[pos])) {
    while (++pos < code.length() && isspace(code[pos]));
    return ReadRawToken();
  }

  const size_t spos = pos;
  switch (code[pos++]) {
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
    case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
    case 'v': case 'w': case 'x': case 'y': case 'z':
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
    case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
    case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
    case 'V': case 'W': case 'X': case 'Y': case 'Z':
    case '_': {
      while (is_letterd(code[++pos]));
      const auto name = std::string_view{code}.substr(spos, pos - spos);
      return Token(TT_VARNAME, Mark(spos, name.length()));
    }

    case '$': {
      if (!is_nybble(code[pos])) {
        herr->Error(Mark(spos, 1)) << "Unclosed double quote at this point";
        return ReadRawToken();
      }
      while (!is_nybble(code[++pos]));
      return Token(TT_HEXLITERAL, Mark(pos, pos - spos));
    }

    case '"': {
      if (setting::use_cpp_escapes) {
        while (code[++pos]!='"') {
          if (pos >= code.length()) {
            herr->Error(Mark(spos, 1)) << "Unclosed double quote at this point";
            return Token(TT_STRING, Mark(spos, pos - spos));
          }
          if (code[pos] == '\\')
            pos++;
        }
      } else {
        while (code[++pos]!='"');
      }
      return Token(TT_STRING, Mark(spos, pos - spos));
    }

    case '\'': {
      if (setting::use_cpp_escapes) {
        while (code[++pos] != '\'') {
          if (pos >= code.length()) {
            herr->Error(Mark(spos, 1)) << "Unclosed quote at this point";
            return Token(TT_STRING, Mark(spos, pos - spos));
          }
          if (code[pos] == '\\')
            pos++;
        }
      } else {
        while (code[++pos]!='\'');
      }
      return Token(TT_STRING, Mark(spos, ++pos - spos));
    }

    case '0': {
      if (setting::use_cpp_literals) {
        if (code[pos] == 'x') {
          while (is_nybble(code[++pos]));
          return Token(TT_HEXLITERAL, Mark(spos, pos - spos));
        }
        if (code[pos] == 'b') {
          while (is_bit(code[++pos]));
          return Token(TT_BINLITERAL, Mark(spos, pos - spos));
        }
        if (code[pos] == 'o') {
          while (is_octal(code[++pos]));
          return Token(TT_OCTLITERAL, Mark(spos, pos - spos));
        }
      }
      [[fallthrough]]; case '1': case '2': case '3': case '4': case '5':
                       case '6': case '7': case '8': case '9':
      while (is_digit(code[++pos]));
      return Token(TT_DECLITERAL, Mark(spos, pos - spos));
    }

    case '/': {
      if (code[pos] == '/') { // Two-slash comments
        while (pos < code.length() and code[pos] != '\n' and code[pos] != '\r')
          ++pos;
        ++pos;  // Optimization: skip the whitespace
        return ReadRawToken();
      }
      if (code[pos] == '*') {
        ++pos; // GM /**/ Comments
        while (pos < code.length() and (code[pos] != '/' or code[pos-1] != '*'))
          ++pos;
        ++pos;  // Important: skip the trailing slash
        return ReadRawToken();
      }
      break;
    }
  }

  if (auto tnode = token_lookup.Get(code, spos); tnode.first != TT_ERROR) {
    pos = tnode.second;
    return Token(tnode.first, Mark(spos, pos - spos));
  }

  herr->Error(Mark(spos, 1)) << "Unexpected symbol '" << code[spos] << "'";
  return ReadRawToken();
}

bool Lexer::HandleMacro(std::string_view name) {
  auto itm = context->macro_map.find(name);
  if (itm == context->macro_map.end() || MacroRecurses(name))
    return false;

  const Macro &macro = itm->second;
  if (macro.parameters) {
    const size_t mpos = pos;
    Token t = ReadRawToken();
    if (t.type == TT_BEGINPARENTH) {
      ErrorContext errc = herr->At(t);
      int paren = 1;
      std::vector<TokenVector> args(1);

      do {
        t = ReadRawToken();
        if (t.type == TT_BEGINPARENTH) ++paren;
        else if (t.type == TT_ENDPARENTH) --paren;
        if (t.type == TT_COMMA && paren == 1) {
          args.emplace_back();
        } else {
          args.back().push_back(t);
        }
      } while (paren && t.type != TT_ENDOFCODE);

      if (paren == 0) {
        std::vector<TokenVector> args_evald(args.size());
        for (size_t i = 0; i < args.size(); ++i)
          args_evald[i] = PreprocessBuffer(args[i]);
        PushMacro(macro.name,
                  macro.SubstituteAndUnroll(args, args_evald, errc));
      } else {
        errc.Error() << "No closing parenthesis for arguments to `"
                     << name << '`';
        pos = mpos;
      }
    }
  } else {
    PushMacro(macro.name, macro.value);
  }
  return true;
}

Token &Lexer::TranslateNameToken(Token &token) {
  std::string_view name = token.content;

  // TODO(new parser): C++ keyword conflict handling deleted from here

  if (auto kw = keyword_lookup.find(name); kw != keyword_lookup.end()) {
    token.type = kw->second;
    return token;
  }

  if (context->shared_locals.find(name) == context->shared_locals.end()) {
    // TODO: REMOVEME
    std::string slow_ass_conversion{name};
    jdi::definition *d = context->language_fe->look_up(slow_ass_conversion);
    if (d) {
      token.ext = d;
      if (d->flags & jdi::DEF_TYPENAME) {
        token.type = TT_TYPE_NAME;
        return token;
      }
      if (d->flags & jdi::DEF_NAMESPACE) {
        token.type = TT_NAMESPACE;
        return token;
      }
      if (context->language_fe->definition_is_function(d)) {
        token.type = TT_FUNCTION;
        return token;
      }
      // Global variables
      token.type = TT_VARNAME;
      return token;
    }
  }
  return token;
}

Token Lexer::ReadToken() {
  if (!open_macros.empty()) {
    auto &macro = open_macros.back();
    if (macro.index >= macro.tokens.size()) {
      PopMacro();
      return ReadToken();
    }
    return macro.tokens[macro.index++];
  }
  Token res = ReadRawToken();
  if (res.type == TT_VARNAME) {
    if (HandleMacro(res.content)) return ReadToken();
    return TranslateNameToken(res);
  }
  return res;
}

Lexer::Lexer(TokenVector tokens, const ParseContext *ctex, ErrorHandler *herr_):
    context(ctex), herr(herr_) {
  PushMacro("", std::move(tokens));
}

}  // namespace parsing
}  // namespace enigma
