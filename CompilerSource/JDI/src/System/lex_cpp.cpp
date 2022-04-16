/**
 * @file lex_cpp.cpp
 * @brief Source implementing the C++ \c lexer class extensions.
 *
 * This file's function will be referenced, directly or otherwise, by every
 * other function in the parser. The efficiency of its implementation is of
 * crucial importance. If this file runs slow, so do the others.
 *
 * @section License
 *
 * Copyright (C) 2011-2014 Josh Ventura
 * This file is part of JustDefineIt.
 *
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 *
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "lex_cpp.h"

#include <API/AST.h>
#include <API/compile_settings.h>
#include <API/context.h>
#include <General/debug_macros.h>
#include <General/parse_basics.h>
#include <General/debug_macros.h>
#include <Parser/context_parser.h>
#include <System/builtins.h>

#include <cassert>
#include <cstring>
#include <csignal>
#include <filesystem>
#include <unordered_map>

#ifdef DEBUG_MODE
/// This function will be passed signals and will respond to them appropriately.
static void donothing(int) {}
#endif

#define E_MATCHED_IF "matching #if already has an #else"

// Optional AST rendering
#include <General/debug_macros.h>

using namespace jdi;
using namespace std;


//==============================================================================
//====: Helpers. Helpers everywhere. :==========================================
//==============================================================================
//                                                                            ==
//  These are from older JDI. The preprocessor still performs some redundant  ==
//  lexing, like skipping comments between preprocessor names and meaningful  ==
//  tokens. It reuses these helpers. So for now, just keep them here.         ==
//                                                                            ==
//==============================================================================

/// Returns true if the given character is not a word character (/\w/).
static inline bool strbw(char s) { return !is_letterd(s); }

static inline bool is_numeric(string_view sv) {
  for (char c : sv) if (c < '0' || c > '9') return false;
  return sv.size();
}

/// Skips to the next newline, terminating at the newline character.
static inline void skip_comment(llreader &cfile) {
  if (ALLOW_MULTILINE_COMMENTS) {
    while (cfile.advance() && cfile.at() != '\n' && cfile.at() != '\r')
      if (cfile.at() == '\\')
        if (cfile.next() == '\n' || cfile.at() == '\r')
          cfile.take_newline();
  } else {
    while (cfile.advance() && cfile.at() != '\n' and cfile.at() != '\r');
  }
}

/// Skips a multiline (/**/) comment. Invoke when at() == '*'.
static inline void skip_multiline_comment(llreader &cfile) {
  cfile.advance(); // Skip another char so we don't break on /*/
  for (;;) {
    if (cfile.eof()) {
      return;
    } else if (cfile.at() == '\n' || cfile.at() == '\r') {
      cfile.take_newline();
    } else if (cfile.take('*')) {
      if (cfile.take('/')) return;
    } else {
      cfile.advance();
    }
  }
}

// Skips an integer-suffix (u, ul, ull, l, lu, ll, llu)
static inline void skip_integer_suffix(llreader &cfile) {
  if (cfile.eof()) return;
  if (cfile.at() == 'u' || cfile.at() == 'U') {
    if (!cfile.advance()) return;
    if (cfile.at() == 'l' || cfile.at() == 'L') {
      const char l = cfile.at();
      if (cfile.next() == l) cfile.advance();
    }
    return;
  }
  // Per ISO, LLu is fine, llU is fine, but Llu and lul are not.
  const char l = cfile.at();
  if (l != 'l' && l != 'L') return;
  if (!cfile.advance()) return;
  if (cfile.at() == l)
    if (!cfile.advance()) return;
  if (cfile.at() == 'u' || cfile.at() == 'U') cfile.advance();
  return;
}

struct StringPrefixFlags {
  bool valid, raw;
  enum : short {
    UNSPECIFIED = 0,
    CHAR8  = 1,  // UTF-8 char.
    CHAR16 = 2,
    WCHAR  = 3,  // Varies whimsically between 2 and 4.
    CHAR32 = 4,
  } width;
};

static inline StringPrefixFlags parse_string_prefix(string_view pre) {
  StringPrefixFlags res;
  // Valid prefixes: R, u8, u8R, u, uR, U, UR, L, or LR.
  size_t i = 0;
  res.raw = false;
  res.valid = false;
  res.width = StringPrefixFlags::UNSPECIFIED;
  // The u, u8, U, and L flags are mutually-exclusive.
  if (pre[i] == 'u') {
    if (++i < pre.length() && pre[i] == '8') {
      ++i;
      res.width = StringPrefixFlags::CHAR8;
    } else {
      res.width = StringPrefixFlags::CHAR16;
    }
  } else if (pre[i] == 'U') {
    res.width = StringPrefixFlags::CHAR32;
    ++i;
  } else if (pre[i] == 'L') {
    res.width = StringPrefixFlags::WCHAR;
    ++i;
  }
  // R may follow any of the above, or stand alone.
  if (i < pre.length() && pre[i] == 'R') {
    res.raw = true;
    ++i;
  }
  res.valid = i == pre.length();
  return res;
}

void lexer::enter_macro(const token_t &otk, const macro_type &macro) {
  if (macro.optimized_value.empty()) return;
  push_buffer({macro.name, otk, &macro.optimized_value});
}

/// Invoked while the reader is *PAST* the opening quote. Terminates with the
/// reader at the closing quote.
static inline bool skip_string(llreader &cfile, char qc, ErrorHandler *herr) {
  while (cfile.next() != EOF && cfile.at() != qc) {
    if (cfile.at() == '\\') {
      if (cfile.next() == EOF) {
        herr->error(cfile, "You can't escape the file ending, jackwagon.");
        return false;
      } else if (cfile.take_newline()) {
        continue;
      }
    } else if (cfile.at() == '\n' or cfile.at() == '\r') {
      herr->error(cfile, "Unterminated string literal");
      return false;
    }
  }
  if (cfile.eof() or cfile.at() != qc) {
    herr->error(cfile, "Unterminated string literal");
    return false;
  }
  cfile.advance();
  return true;
}

/// Invoked while the reader is at the opening quote. Terminates with the reader
/// at the closing quote.
static inline bool skip_rstring(llreader &cfile, ErrorHandler *herr) {
  // Read delimeter
  bool warned = false;
  const size_t spos = cfile.tell();
  while (cfile.next() != EOF && cfile.at() != '(') {
    if (cfile.at() == '\\' || cfile.at() == ' ') {
      if (!warned) {
        herr->warning(cfile, "ISO C++ forbids backslash, space, and quote "
                             "characters in raw-literal delimiters.");
        warned = true;
      }
    }
  }
  const string delim = ")"s + string{cfile.slice(spos + 1)};
  while (cfile.smart_advance()) {
    if (cfile.at() != '"') continue;
    string_view d = cfile.slice(cfile.tell() - delim.length(), cfile.tell());
    if (d == delim) return true;
  }
  return false;
}



/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
████▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀████████████████████████████████████████████████
████ Basic Lexer Implementation ████████████████████████████████████████████████
████▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄████████████████████████████████████████████████
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */

token_t jdi::read_token(llreader &cfile, ErrorHandler *herr) {
  #ifdef DEBUG_MODE
    static int number_of_times_GDB_has_dropped_its_ass = 0;
    ++number_of_times_GDB_has_dropped_its_ass;
  #endif

  if (cfile.pos < cfile.length) { // Sanity check the stupid reader.
    if (cfile.pos < cfile.validated_pos) {
      herr->error(cfile, "Someone rewound the file.");
      cfile.validated_lnum = cfile.validated_lpos = cfile.validated_pos = 0;
    }
    for (; cfile.validated_pos < cfile.pos; ++cfile.validated_pos) {
      if (cfile[cfile.validated_pos] == '\n' ||
              (cfile[cfile.validated_pos] == '\r' &&
               cfile.at(cfile.validated_pos + 1) != '\n')) {
        ++cfile.validated_lnum;
        cfile.validated_lpos = cfile.validated_pos + 1;
      }
    }
    if (cfile.lnum != cfile.validated_lnum ||
        cfile.lpos != cfile.validated_lpos) {
      herr->error(
          cfile, "At line " + to_string(cfile.validated_lnum) + ", position " +
                  to_string(cfile.pos - cfile.validated_lpos) + ", the reader "
                  "believes it is at line " + to_string(cfile.lnum) +
                  ", position " + to_string(cfile.pos - cfile.lpos) + "...");
      cfile.lnum = cfile.validated_lnum;
      cfile.lpos = cfile.validated_lpos;
    }
  }

  // Dear C++ committee: do you know what would be exponentially more awesome
  // than this line? Just declaring a normal fucking function, please and thanks
  auto mktok = [&cfile](TOKEN_TYPE tp, size_t pos, int length) -> token_t {
    return token_t(tp, cfile.name, cfile.lnum, pos - cfile.lpos,
                   cfile + pos, length);
  };

  for (;;) {  // Loop until we find something or hit world's end
    if (cfile.eof()) return mktok(TT_ENDOFCODE, cfile.tell(), 0);

    // Skip all whitespace
    if (is_useless(cfile.at())) {
      size_t spos = cfile.tell();
      do {
        if (cfile.at() == '\n' || cfile.at() == '\r') {
          cfile.take_newline();
          return mktok(TTM_NEWLINE, spos, cfile.tell() - spos);
        }
        if (!cfile.advance()) return mktok(TT_ENDOFCODE, cfile.tell(), 0);
      } while (is_useless(cfile.at()));
      return mktok(TTM_WHITESPACE, spos, cfile.tell() - spos);
    }

    //==========================================================================
    //====: Check for and handle comments. :====================================
    //==========================================================================

    const size_t spos = cfile.tell();
    switch (cfile.getc()) {

    case '/': {
      if (cfile.at() == '*') {
        skip_multiline_comment(cfile);
        return mktok(TTM_WHITESPACE, spos, cfile.tell() - spos);
      }
      if (cfile.at() == '/') {
        skip_comment(cfile);
        return mktok(TTM_WHITESPACE, spos, cfile.tell() - spos);
      }
      if (cfile.take('=')) {
        return mktok(TT_DIVIDE_ASSIGN, spos, 2);
      }
      return mktok(TT_SLASH, spos, 1);
    }

    default:
    //==========================================================================
    //====: Not at a comment. See if we're at an identifier. :==================
    //==========================================================================

    if (is_letter(cfile[spos])) {
      while (!cfile.eof() && is_letterd(cfile.at())) cfile.advance();
      if (cfile.tell() - spos <= 2 &&
          (cfile.at() == '\'' || cfile.at() == '"')) {
        auto prefix = parse_string_prefix(cfile.slice(spos));
        if (prefix.valid) {
          if (prefix.raw) {
            if (skip_rstring(cfile, herr)) cfile.advance();
            return mktok(TT_STRINGLITERAL, spos, cfile.tell() - spos);
          }
          skip_string(cfile, cfile.at(), herr);
          return mktok(TT_CHARLITERAL, spos, cfile.tell() - spos);
        }
      }
      return mktok(TT_IDENTIFIER, spos, cfile.tell() - spos);
    }

    goto unknown;

    //==========================================================================
    //====: Not at an identifier. Maybe at a number? :==========================
    //==========================================================================

    case '0': { // Check if the number is hexadecimal, binary, or octal.
      // TODO: Handle apostrophes.
      // Check if the number is hexadecimal.
      if (cfile.at() == 'x' || cfile.at() == 'X') {
        // Here, it is hexadecimal.
        while (cfile.advance() && is_hexdigit(cfile.at()));
        skip_integer_suffix(cfile);
        return mktok(TT_HEXLITERAL, spos, cfile.tell() - spos);
      }
      // Check if the number is Binary.
      if (cfile.at() == 'b' || cfile.at() == 'B') {
        // In this case, it's binary
        while (cfile.advance() && is_hexdigit(cfile.at()));
        skip_integer_suffix(cfile);
        return mktok(TT_BINLITERAL, spos, cfile.tell() - spos);
      }
      // Turns out, it's octal.
      if (cfile.eof() || !is_octdigit(cfile.at())) {
        // Literal 0. According to ISO, this is octal, because a decimal literal
        // does not start with zero, while octal literals begin with 0 and
        // consist of octal digits.
        return mktok(TT_OCTLITERAL, spos, 1);
      }
      while (cfile.advance() && is_octdigit(cfile.at()));
      skip_integer_suffix(cfile);
      return mktok(TT_OCTLITERAL, spos, cfile.tell() - spos);
    }

    case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8':
    case '9': {
      // Turns out, it's decimal.
      handle_decimal:
      while (!cfile.eof() and is_digit(cfile.at())) cfile.advance();
      if (cfile.at() == '.')
        while (cfile.advance() and is_digit(cfile.at()));
      if (cfile.at() == 'e' or cfile.at() == 'E') { // Accept exponents
        if (cfile.next() == '-') cfile.advance();
        if (cfile.eof()) {
          herr->error(cfile, "Numeric literal truncated and end of file.");
        }
        else while (is_digit(cfile.at()) && cfile.advance());
      }
      skip_integer_suffix(cfile);
      return mktok(TT_DECLITERAL, spos, cfile.tell()  -spos);
    }

    //==========================================================================
    //====: Not at a number. Find out where we are. :===========================
    //==========================================================================

      case ';':
        return mktok(TT_SEMICOLON, spos, 1);
      case ',':
        return mktok(TT_COMMA, spos, 1);
      case '+':
        if (cfile.at() == '+') {
          cfile.advance();
          return mktok(TT_INCREMENT, spos, 2);
        }
        if (cfile.at() == '=') {
          cfile.advance();
          return mktok(TT_ADD_ASSIGN, spos, 2);
        }
        return mktok(TT_PLUS, spos, 1);
      case '-':
        if (cfile.at() == '-') {
          cfile.advance();
          return mktok(TT_DECREMENT, spos, 2);
        }
        if (cfile.at() == '=') {
          cfile.advance();
          return mktok(TT_SUBTRACT_ASSIGN, spos, 2);
        }
        if (cfile.at() == '>') {
          cfile.advance();
          return mktok(TT_ARROW, spos, 2);
        }
        return mktok(TT_MINUS, spos, 1);
      case '=':
        if (cfile.at() == cfile[spos]) {
          cfile.advance();
          return mktok(TT_EQUAL_TO, spos, 2);
        }
        return mktok(TT_EQUAL, spos, 1);
      case '&':
        if (cfile.at() == '&') {
          cfile.advance();
          return mktok(TT_AMPERSANDS, spos, 2);
        }
        if (cfile.at() == '=') {
          cfile.advance();
          return mktok(TT_AND_ASSIGN, spos, 2);
        }
        return mktok(TT_AMPERSAND, spos, 1);
      case '|':
        if (cfile.at() == '|') {
          cfile.advance();
          return mktok(TT_PIPES, spos, 2);
        }
        if (cfile.at() == '=') {
          cfile.advance();
          return mktok(TT_OR_ASSIGN, spos, 2);
        }
        return mktok(TT_PIPE, spos, 1);
      case '~':
        if (cfile.at() == '=') {
          cfile.advance();
          return mktok(TT_NEGATE_ASSIGN, spos, 2);
        }
        return mktok(TT_TILDE, spos, 1);
      case '!':
        if (cfile.at() == '=') {
          cfile.advance();
          return mktok(TT_NOT_EQUAL_TO, spos, 2);
        }
        return mktok(TT_NOT, spos, 1);
      case '%':
        if (cfile.at() == '=') {
          cfile.advance();
          return mktok(TT_MODULO_ASSIGN, spos, 2);
        }
        return mktok(TT_MODULO, spos, 1);
      case '*':
        if (cfile.at() == '=') {
          cfile.advance();
          return mktok(TT_MULTIPLY_ASSIGN, spos, 2);
        }
        return mktok(TT_STAR, spos, 1);
      case '^':
        if (cfile.at() == '=') {
          cfile.advance();
          return mktok(TT_XOR_ASSIGN, spos, 2);
        }
        return mktok(TT_NOT, spos, 1);
      case '>':
        if (cfile.at() == '>') {
          cfile.advance();
          if (cfile.at() == '=') {
            cfile.advance();
            return mktok(TT_RSHIFT_ASSIGN, spos, 3);
          }
          return mktok(TT_RSHIFT, spos, 2);
        }
        if (cfile.at() == '=') {
          cfile.advance();
          return mktok(TT_GREATER_EQUAL, spos, 2);
        }
        return mktok(TT_GREATERTHAN, spos, 1);
      case '<':
        if (cfile.at() == '<') {
          cfile.advance();
          if (cfile.at() == '=') {
            cfile.advance();
            return mktok(TT_LSHIFT_ASSIGN, spos, 3);
          }
          return mktok(TT_LSHIFT, spos, 2);
        }
        if (cfile.at() == '=') {
          cfile.advance();
          return mktok(TT_LESS_EQUAL, spos, 2);
        }
        return mktok(TT_LESSTHAN, spos, 1);
      case ':':
        if (cfile.at() == cfile[spos]) {
          cfile.advance();
          return mktok(TT_SCOPE, spos, 2);
        }
        return mktok(TT_COLON, spos, 1);
      case '?':
        return mktok(TT_QUESTIONMARK, spos, 1);

      case '.':
          if (is_digit(cfile.at()))
            goto handle_decimal;
          else if (cfile.at() == '.') {
            if (cfile.peek_next() == '.') {
              cfile.skip(2);
              return mktok(TT_ELLIPSIS, spos, 3);
            }
          }
          if (cfile.at() == '*') {
            cfile.advance();
            return mktok(TT_DOT_STAR, spos, 1);
          }
        return mktok(TT_DOT, spos, 1);

      case '(': return mktok(TT_LEFTPARENTH,  spos, 1);
      case '[': return mktok(TT_LEFTBRACKET,  spos, 1);
      case '{': return mktok(TT_LEFTBRACE,    spos, 1);
      case '}': return mktok(TT_RIGHTBRACE,   spos, 1);
      case ']': return mktok(TT_RIGHTBRACKET, spos, 1);
      case ')': return mktok(TT_RIGHTPARENTH, spos, 1);

      case '#':
        if (cfile.at() == '#') {
          cfile.advance();
          return mktok(TTM_CONCAT, spos, 2);
        }
        return mktok(TTM_TOSTRING, spos, 1);

      case '\\':
        // ISO Translation phase 2
        if (cfile.at() != '\n' || cfile.at() != '\r')
          cfile.take_newline();
        continue;

      case '"': {
        if (!cfile.take('"')) skip_string(cfile, '"', herr);
        return mktok(TT_STRINGLITERAL, spos, cfile.tell() - spos);
      }

      case '\'': {
        if (cfile.at() == '\'') {
          herr->error(cfile, "Zero-length character literal");
        } else {
          skip_string(cfile, '\'', herr);
        }
        return mktok(TT_CHARLITERAL, spos, cfile.tell() - spos);
      }

      unknown: {
        char errbuf[320];
        sprintf(errbuf, "Unrecognized symbol (char)0x%02X '%c'",
                (int)cfile[spos], cfile[spos]);
        herr->error(cfile, errbuf);
        return mktok(TT_INVALID, spos, 1);
      }
    }
  }

  herr->error(cfile, "UNREACHABLE BLOCK REACHED");
  return mktok(TT_INVALID, cfile.tell(), 0);
}

/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
████▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀████████████████████████████████████████
████ Preprocessing Lexer Implementation ████████████████████████████████████████
████▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄████████████████████████████████████████
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */

bool lexer::inside_macro(string_view name) const {
  for (const auto &buf : open_buffers)
    if (buf.macro_info && buf.macro_info->name == name)
      return true;
  return false;
}

bool lexer::parse_macro_function(const token_t &otk, const macro_type &mf) {
  if (inside_macro(mf.name))
    return false;

  /* Find a parenthesis or exit. */
  token_t maybe_paren = read_raw(); {
    token_vector maybe_rewind;
    while (maybe_paren.preprocesses_away()) {
      maybe_rewind.push_back(maybe_paren);
      maybe_paren = read_raw();
    }
    if (maybe_paren.type != TT_LEFTPARENTH) {
      maybe_rewind.push_back(maybe_paren);
      push_buffer(std::move(maybe_rewind));
      return false;
    }
  }

  vector<token_vector> args;
  args.reserve(mf.params.size());
  if (!mf.params.empty()) args.emplace_back();

  // Read the parameters into our argument vector
  int too_many_args = 0;
  for (int nestcnt = 1;;) {
    token_t tok = read_raw();
    if (tok.type == TT_ENDOFCODE) {
      herr->error(maybe_paren, "Unterminated parameters to macro function");
      return false;
    }
    if (tok.type == TT_LEFTPARENTH) ++nestcnt;
    if (tok.type == TT_RIGHTPARENTH) {
      if (!--nestcnt) break;
    }
    if (args.empty()) args.emplace_back();
    if (tok.type == TT_COMMA && nestcnt == 1) {
      if (args.size() < mf.params.size()) {
        args.emplace_back();
        continue;
      } else if (!mf.is_variadic) {
        ++too_many_args;
      }
    }
    // XXX: It would be nice to offer a flag to preserve comments in this case.
    // Removing these tokens here and below is necessary unless we handle this
    // trimming in TOSTRING during macro expansion.
    if (!args.back().empty() || !tok.preprocesses_away())
      args.back().push_back(tok);
  }

  // Strip trailing whitespace in arguments.
  for (token_vector &arg : args) {
    while (!arg.empty() && arg.back().preprocesses_away()) arg.pop_back();
  }

  // Briefly. What we've done above is read the raw tokens for each argument.
  // These will be substituted for parameters that are arguments to `##` or `#`.
  // But parameters that appear raw in the replacement list are substituted with
  // a fully-evaluated version, "as if they formed the rest of the preprocessing
  // file with no other preprocessing tokens being available." (cpp.subst.1.2)

  if (too_many_args) {
    herr->error(cfile, "Too many arguments to macro function `%s`; "
                       "expected %s but got %s",
                mf.name, mf.params.size(), mf.params.size() + too_many_args);
  }

  vector<token_vector> evald;
  evald.reserve(args.size());
  for (const token_vector &arg : args) {
    push_frozen_buffer(&arg);
    token_vector &v = evald.emplace_back();
    for (token_t t = preprocess_and_read_token(); t.type != TT_ENDOFCODE;
                 t = preprocess_and_read_token()) {
      v.push_back(t);
    }
    pop_frozen_buffer();
  }

  token_vector tokens = mf.substitute_and_unroll(args, evald, herr->at(otk));
  push_buffer({mf.name, otk, std::move(tokens)});
  return true;
}

string lexer::read_preprocessor_args() {
  for (;;) {
    while (cfile.at() == ' ' or cfile.at() == '\t') {
      if (!cfile.advance()) return "";
    }
    if (cfile.at() == '/') {
      if (cfile.next() == '/') { skip_comment(cfile); return ""; }
      if (cfile.at() == '*') { skip_multiline_comment(cfile); continue; }
      break;
    }
    if (cfile.at_newline()) return "";
    if (cfile.at() == '\\') {
      if (!cfile.advance()) return "";
      cfile.take_newline();
    }
    break;
  }
  string res;
  res.reserve(256);
  size_t spos = cfile.pos;
  while (!cfile.eof() && cfile.at() != '\n' && cfile.at() != '\r') {
    if (cfile.at() == '/') {
      if (cfile.next() == '/') {
        res += cfile.slice(spos, cfile.pos - 1);
        skip_comment(cfile);
        return res;
      }
      if (cfile.at() == '*') {
        res += cfile.slice(spos, cfile.pos - 1);
        res += " ";
        skip_multiline_comment(cfile);
        spos = cfile.pos;
        continue;
      }
    }
    if (cfile.at() == '\'' || cfile.at() == '"') {
      skip_string(cfile, cfile.at(), herr);
    } else if (cfile.at() == '\\') {
      if (!cfile.advance()) break;
      cfile.take_newline();
    }
    else cfile.advance();
  }
  res += cfile.slice(spos);
  {
    size_t trim = res.length() - 1;
    if (is_useless(res[trim])) {
      while (is_useless(res[--trim]));
      res.erase(++trim);
    }
  }
  return res;
}

vector<string> debug_list;
class DebugSeer {
 public:
  DebugSeer(const token_vector &toks) {
    string enqueue;
    for (const token_t &tok : toks) enqueue += tok.to_string() + " ";
    debug_list.push_back(enqueue);
  }
  DebugSeer(string_view str) {
    debug_list.emplace_back(str);
  }
  ~DebugSeer() {
    debug_list.pop_back();
  }
};

/// Helper for performing conditional item removal/replacement in a vector.
/// Allows iteration of items in a vector and removal of items, in one pass.
/// Most of this class' utility is its destructor.
template<class T> struct VectorCompact {
  std::vector<T> &vec;
  size_t left = 0, right = 0;

  operator bool() const { return right < vec.size(); }

  void next() {
    if (left < right) vec[left] = vec[right];
    ++left, ++right;
  }
  bool has_next(size_t n = 1) const { return right < vec.size() - n; }
  T &peek_next(size_t n = 1) { return vec[right + n]; }

  void drop() { ++right; }
  void replace(const T &with) { vec[right] = with; }
  void replace(T &&with) { vec[right] = std::move(with); }
  T &at() { return vec[right]; }

  VectorCompact(std::vector<T> &v): vec(v) {}
  ~VectorCompact() {
    if (left < right) while (right < vec.size()) next();
    vec.resize(left);
  }
};

// Performs a search for an include file, returning the first
// successfully-opened file.
static llreader try_find_and_open(const llreader &cfile, const Context *ctx,
    string_view fnfind, bool check_local, bool find_next) {
  llreader incfile;
  filesystem::path incfn, fdir;
  filesystem::path cur_path = filesystem::path(cfile.name).parent_path();
  if (check_local)
    incfile.open(incfn = cur_path / fnfind);
  for (size_t i = 0; !incfile.is_open() && i < ctx->search_dir_count(); ++i) {
    if (!find_next) {
      incfile.open(incfn = (fdir = ctx->search_dir(i)) / fnfind);
    } else {
      // Linear search for this directory in include_next.
      find_next = cur_path != ctx->search_dir(i);
    }
  }
  return incfile;
}


/* ************************************************************************** *\
████▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀██████████████████████████████████████████████████
████ Main preprocessor unit.  ██████████████████████████████████████████████████
████▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄██████████████████████████████████████████████████
\* ************************************************************************** */

enum class PreprocessorDirective {
  UNKNOWN, DEFINE, ENDIF, ELSE, ELIF, ELIFDEF, ELIFNDEF, ERROR, IF, IFDEF,
  IFNDEF, IMPORT, INCLUDE, INCLUDE_NEXT, LINE, PRAGMA, UNDEF, USING, WARNING
};

const std::unordered_map<std::string_view, PreprocessorDirective>
kPreprocessorDirectives {
  {"define", PreprocessorDirective::DEFINE},
  {"endif", PreprocessorDirective::ENDIF},
  {"else", PreprocessorDirective::ELSE},
  {"elif", PreprocessorDirective::ELIF},
  {"elifdef", PreprocessorDirective::ELIFDEF},
  {"elifndef", PreprocessorDirective::ELIFNDEF},
  {"error", PreprocessorDirective::ERROR},
  {"if", PreprocessorDirective::IF},
  {"ifdef", PreprocessorDirective::IFDEF},
  {"ifndef", PreprocessorDirective::IFNDEF},
  {"import", PreprocessorDirective::IMPORT},
  {"include", PreprocessorDirective::INCLUDE},
  {"include_next", PreprocessorDirective::INCLUDE_NEXT},
  {"line", PreprocessorDirective::LINE},
  {"pragma", PreprocessorDirective::PRAGMA},
  {"undef", PreprocessorDirective::UNDEF},
  {"using", PreprocessorDirective::USING},
  {"warning", PreprocessorDirective::WARNING},
};

void lexer::handle_preprocessor() {
  top:
  token_t tk = read_token(cfile, herr);
  while (tk.preprocesses_away()) tk = read_token(cfile, herr);
  if (tk.type != TT_IDENTIFIER &&
      tk.type != TT_DECLITERAL && tk.type != TT_OCTLITERAL) {
    herr->error(tk, "Expected preprocessor directive; found %s",
                tk.to_string());
    return;
  }
  auto directive_str = tk.content.view();
  auto directive = kPreprocessorDirectives.find(directive_str);
  switch (directive == kPreprocessorDirectives.end()
              ? PreprocessorDirective::UNKNOWN : directive->second) {
    case PreprocessorDirective::DEFINE: {
      bool variadic = false; // Whether this function is variadic
      string argstrs = read_preprocessor_args();
      const char* argstr = argstrs.c_str();
      if (!conditionals.empty() and !conditionals.back().is_true)
        break;
      size_t i = 0;
      while (is_useless(argstr[i])) ++i;
      if (!is_letter(argstr[i])) {
        herr->error(cfile, "Expected macro definiendum at this point");
      }
      const size_t nsi = i;
      while (is_letterd(argstr[++i]));
      auto mins = macros.insert({argstrs.substr(nsi, i - nsi), nullptr});

      if (argstr[i] == '(') {
        vector<string> paramlist;
        while (is_useless(argstr[++i]));
        if (argstr[i] != ')') for (;;) {
          if (!is_letter(argstr[i])) {
            // If we're at TT_ELLIPSIS.
            if (argstr[i] == '.' && argstr[i+1] == '.' && argstr[i+2] == '.') {
              variadic = true, i += 3;
              // Push an empty (anonymous) variadic parameter.
              // This looks like a hack, but makes a lot of sense under the GNU
              // interpretation of variadic macro functions, and it simplifies
              // the code later on--we have a convenient extra parameter to bind
              // all additional arguments to.
              paramlist.push_back("");
              while (is_useless(argstr[i])) ++i;
              if (argstr[i] != ')')
                herr->error(cfile, "Expected end of parameters after variadic");
              break;
            }
            else {
              herr->error(cfile, "Expected parameter name for macro declaration");
              break;
            }
          }
          const size_t si = i;
          while (is_letterd(argstr[++i]));
          paramlist.push_back(argstrs.substr(si, i-si));

          while (is_useless(argstr[i])) ++i;
          if (argstr[i] == ')') break;
          if (argstr[i] == ',') { while (is_useless(argstr[++i])); continue; }

          // Handle variadic macros (if we are at ...)
          if (argstr[i] == '.' and argstr[i+1] == '.' and argstr[i+2] == '.') {
            i += 2; while (is_useless(argstr[++i]));
            variadic = true;
            if (argstr[i] == ')') break;
            herr->error(cfile, "Expected closing parenthesis at this point; "
                        "further parameters not allowed following variadic");
          }
          else {
            herr->error(cfile,
                        "Expected comma or closing parenthesis at this point");
          }
        }

        if (!mins.second) { // If no insertion was made; ie, the macro existed already.
        //  if ((size_t)mins.first->second->argc != paramlist.size())
        //    herr->warning("Redeclaring macro function `" + mins.first->first + '\'', filename, line, pos-lpos);
          mins.first->second = nullptr;
        }
        mins.first->second = std::make_shared<const macro_type>(
            mins.first->first, std::move(paramlist), variadic,
            std::move(tokenize(cfile.name, argstrs.substr(++i), herr)),
            herr);
      } else {
        while (is_useless(argstr[i])) ++i;
        mins.first->second = std::make_shared<const macro_type>(
            mins.first->first, tokenize(cfile.name, argstrs.substr(i), herr),
            herr);
      }
    } break;
    case PreprocessorDirective::ERROR: {
        string emsg = read_preprocessor_args();
        if (conditionals.empty() or conditionals.back().is_true)
          herr->error(cfile, "#error " + emsg);
      } break;
      break;
    case PreprocessorDirective::ELIF:
        if (conditionals.empty())
          herr->error(cfile, "Unexpected #elif directive; no matching #if");
        else {
          if (conditionals.back().seen_else) {
            herr->error(cfile, "Unexpected #elif directive: " E_MATCHED_IF);
          }
          if (conditionals.back().is_true) {
            conditionals.back().is_true = false;
            conditionals.back().parents_true = false;
          } else {
            if (conditionals.back().parents_true) {
              conditionals.pop_back();
              goto case_if;
            }
          }
        }
      break;
    case PreprocessorDirective::ELIFDEF:
        if (conditionals.empty())
          herr->error(cfile, "Unexpected #elifdef directive; no matching #if");
        else {
          if (conditionals.back().seen_else) {
            herr->error(cfile, "Unexpected #elifdef directive: " E_MATCHED_IF);
          }
          if (conditionals.back().is_true) {
            conditionals.back().is_true = false;
            conditionals.back().parents_true = false;
          } else {
            if (conditionals.back().parents_true) {
              conditionals.pop_back();
              goto case_ifdef;
            }
          }
        }
      break;
    case PreprocessorDirective::ELIFNDEF:
        if (conditionals.empty())
          herr->error(cfile, "Unexpected #elifndef directive; no matching #if");
        else {
          if (conditionals.back().seen_else) {
            herr->error(cfile, "Unexpected #elifdef directive: " E_MATCHED_IF);
          }
          if (conditionals.back().is_true) {
            conditionals.back().is_true = false;
            conditionals.back().parents_true = false;
          } else {
            if (conditionals.back().parents_true) {
              conditionals.pop_back();
              goto case_ifndef;
            }
          }
        }
      break;
    case PreprocessorDirective::ELSE:
        if (conditionals.empty())
          herr->error(cfile, "Unexpected #else directive; no matching #if");
        else {
          conditionals.back().is_true =
              conditionals.back().parents_true && !conditionals.back().is_true;
          conditionals.back().seen_else = true;
        }
      break;
    case PreprocessorDirective::ENDIF:
        if (conditionals.empty())
          return
           herr->error(cfile, "Unexpected #endif directive: no open conditionals.");
        conditionals.pop_back();
      break;
    case PreprocessorDirective::IF: case_if:
        if (conditionals.empty() or conditionals.back().is_true) {
          token_vector toks;
          for (token_t tok; tok = read_token(cfile, herr),
              tok.type != TT_ENDOFCODE && tok.type != TTM_NEWLINE; ) {
            toks.push_back(tok);
          }

          // Since both the current user of this lexer *and* the AST builder
          // are likely to be using rewind buffers, just make a new lexer.
          //
          // Using a second lexer here makes it so we don't have to fuck around
          // with our rewind buffers to stop them from recording macros. Above,
          // we don't have that problem because the macro system never calls
          // directly into the lexer. Here, we just need a device that feeds
          // a sequence of tokens into things. It's called a lexer. Since
          // facilities used in the AST builder require unbounded lookahead,
          // it's easiest just to use the system we have.
          lexer l(&toks, *this);
          l.evaluate_cpp_cond_ = true;

          token_t endofcode;
          AST a = parse_expression(&l, herr, &endofcode);
          render_ast(a, "if_directives");

          if (endofcode.type != TT_ENDOFCODE) {
            herr->error(cfile)
                << "Extra tokens at end of conditional: "
                << endofcode << " not handled.";
            herr->error(endofcode) << "Extraneous token read from here.";
          }

          if (!a.eval({herr, toks[0]}))
            conditionals.push_back(condition(0,1));
          else
            conditionals.push_back(condition(1,0));
        }
        else
          conditionals.push_back(condition(0,0));
      break;
    case PreprocessorDirective::IFDEF: case_ifdef: {
        cfile.skip_whitespace();
        if (!is_letter(cfile.at())) {
          herr->error(cfile, "Expected identifier to check against macros");
          break;
        }
        const size_t msp = cfile.tell();
        while (is_letterd(cfile.next()));
        string macro(cfile + msp, cfile.tell() - msp);
        if (conditionals.empty() or conditionals.back().is_true) {
          if (macros.find(macro) == macros.end()) {
            token_t res;
            conditionals.push_back(condition(0,1));
            break;
          }
          conditionals.push_back(condition(1,0));
        }
        else
          conditionals.push_back(condition(0,0));
      } break;
    case PreprocessorDirective::IFNDEF: case_ifndef: {
        cfile.skip_whitespace();
        if (!is_letter(cfile.at())) {
          herr->error(cfile, "Expected identifier to check against macros");
          break;
        }
        const size_t msp = cfile.tell();
        while (is_letterd(cfile.next()));
        string macro(cfile+msp, cfile.tell()-msp);
        if (conditionals.empty() or conditionals.back().is_true) {
          if (macros.find(macro) != macros.end()) {
            token_t res;
            conditionals.push_back(condition(0,1));
            break;
          }
          conditionals.push_back(condition(1,0));
        }
        else
          conditionals.push_back(condition(0,0));
      } break;
    case PreprocessorDirective::IMPORT:
      break;
    case PreprocessorDirective::INCLUDE: {
        bool incnext;
        if (true) incnext = false;
        else { case PreprocessorDirective::INCLUDE_NEXT: incnext = true; }

        string include_args = read_preprocessor_args();
        string_view fnfind = include_args;
        if (!conditionals.empty() && !conditionals.back().is_true) break;

        bool chklocal = false;
        char match = '>';
        if (!incnext and fnfind[0] == '"')
          chklocal = true, match = '"';
        else if (fnfind[0] != '<') {
          herr->error(cfile, "Expected filename inside <> or \"\" delimiters");
          break;
        }
        for (size_t i = 1; i < fnfind.length(); ++i) if (fnfind[i] == match) {
          fnfind = fnfind.substr(1, i - 1);
          break;
        }

        if (files.size() > 9000) {
          herr->error(cfile, "Nested include count is OVER NINE THOUSAAAAAAND. "
                             "Not including another.");
          break;
        }

        llreader incfile =
            try_find_and_open(cfile, builtin, fnfind, chklocal, incnext);
        if (!incfile.is_open()) {
          herr->error(cfile, incnext ? "Could not find next %s"
                                     : "Could not find %s", fnfind);
        }

        files.emplace_back(std::move(cfile));
        visited_files.insert(incfile.name).first;
        cfile.consume(incfile);
      } break;
    case PreprocessorDirective::LINE:
      // TODO: Handle line directives.
      break;
    case PreprocessorDirective::PRAGMA:
        #ifdef DEBUG_MODE
        {
          string n = read_preprocessor_args();
          if (n == "DEBUG_ENTRY_POINT" &&
              (conditionals.empty() || conditionals.back().is_true)) {
            signal(3, donothing); // Catch what we're about to raise in case there's no debugger
            asm("INT3;"); // Raise hell in the interrupt handler; the debugger will grab us from here
            cout << "* Debug entry point" << endl;
          }
        }
        #else
          read_preprocessor_args();
        #endif
      break;
    case PreprocessorDirective::UNDEF:
        if (!conditionals.empty() and !conditionals.back().is_true)
          break;

        cfile.skip_whitespace();
        if (!is_letter(cfile.at()))
          herr->error(cfile, "Expected macro identifier at this point");
        else {
          const size_t nspos = cfile.tell();
          while (is_letterd(cfile.next()));
          macros.erase((string) cfile.slice(nspos));  // TODO(C++20): remove cast
        }
      break;
    case PreprocessorDirective::USING:
      break;
    case PreprocessorDirective::WARNING: {
        string wmsg = read_preprocessor_args();
        if (conditionals.empty() or conditionals.back().is_true)
          herr->warning(cfile, "#warning " + wmsg);
      } break;
    case PreprocessorDirective::UNKNOWN:
    default:
      while (is_letterd(cfile.at()) && cfile.advance());
      if (is_numeric(directive_str)) {
        // TODO: Handle line directives.
      } else {
        herr->error(cfile, "Invalid preprocessor directive `%s`",
                    directive_str);
      }
      if (!cfile.eof())
        while (cfile.at() != '\n' && cfile.at() != '\r' && cfile.advance());
  }
  if (conditionals.empty() or conditionals.back().is_true)
    return;

  // skip_to_macro:
  do {
    tk = read_token(cfile, herr);
    if (tk.type == TTM_TOSTRING) goto top;
  } while (tk.type != TT_ENDOFCODE);
  herr->error(cfile, "Expected closing preprocessors before end of code");
  return;
}

token_vector jdi::tokenize(string fname, string_view str, ErrorHandler *herr) {
  token_vector res;
  llreader read(fname, str, false);
  for (token_t tk = read_token(read, herr); tk.type != TT_ENDOFCODE;
               tk = read_token(read, herr))
    res.push_back(tk);
  return res;
}

enum class LexerKeyword {
  FILENAME,
  LINE,
  FUNC,
  FUNC_PRETTY,

  // Do not add cpp.cond keywords above this line, nor non-cpp.cond keywords
  // below it. Used to quickly check if we're evaluating these.
  CPP_COND_BEGIN,
  DEFINED,
  HAS_INCLUDE,
  HAS_INCLUDE_NEXT,
  HAS_CPP_ATTRIBUTES
};
static const unordered_map<string, LexerKeyword> kLexerKeywords {
  { "__FILE__", LexerKeyword::FILENAME },
  { "__LINE__", LexerKeyword::LINE },
  { "__FUNCTION__", LexerKeyword::FUNC },
  { "__func__", LexerKeyword::FUNC },
  { "__PRETTY_FUNCTION__", LexerKeyword::FUNC_PRETTY },
  { "defined", LexerKeyword::DEFINED },
  { "__has_include", LexerKeyword::HAS_INCLUDE },
  { "__has_include__", LexerKeyword::HAS_INCLUDE },  // GNU spelling
  {"__has_include_next", LexerKeyword::HAS_INCLUDE_NEXT},
  {"__has_include_next__", LexerKeyword::HAS_INCLUDE_NEXT},  // GNU spelling
  {"__has_cpp_attribute", LexerKeyword::HAS_CPP_ATTRIBUTES},
};

const map<string, long> kAttributeSupportDate {
  { "assert", 201806L },
  { "carries_dependency", 200809L },
  { "deprecated", 201309L },
  { "ensures", 201806L },
  { "expects", 201806L },
  { "fallthrough", 201603L },
  { "likely", 201803L },
  { "maybe_unused", 201603L },
  { "no_unique_address", 201803L },
  { "nodiscard", 201603L },
  { "noreturn", 200809L },
  { "unlikely", 201803L },
};

bool lexer::handle_macro(token_t &identifier) {
  if (identifier.type != TT_IDENTIFIER) {
    herr->error(identifier, "Internal error: Not an identifier: %s",
                identifier.to_string());
    return false;
  }
  string fn = identifier.content.toString();
  macro_iter mi;

  mi = macros.find(fn);
  if (mi != macros.end()) {
    if (mi->second->is_function) {
      if (parse_macro_function(identifier, *mi->second)) {
        // Upon success, restart routine. On failure, treat as identifier.
        return true;
      }
    } else {
      if (!inside_macro(fn)) {
        enter_macro(identifier, *mi->second);
        return true;
      }
    }
  }

  auto lexit = kLexerKeywords.find(fn);
  if (lexit != kLexerKeywords.end() &&
      (evaluate_cpp_cond_ || lexit->second < LexerKeyword::CPP_COND_BEGIN))
      switch (lexit->second) {
    case LexerKeyword::FILENAME: {
      // The current token was probably defined in a macro.
      // Prefer the name of the currently-open file.
      identifier.content = quote(cfile.is_open() ? cfile.name : identifier.file);
      identifier.type = TT_STRINGLITERAL;
      return false;
    }
    case LexerKeyword::LINE: {
      // The current token was probably defined in a macro.
      // Prefer the name of the currently-open file.
      identifier.content =
          std::to_string(cfile.is_open() ? cfile.lnum : identifier.linenum);
      identifier.type = TT_DECLITERAL;
      return false;
    }
    case LexerKeyword::DEFINED: {
      token_t tok = read_raw_non_empty();
      bool need_paren = false;
      if (tok.type == TT_LEFTPARENTH) {
        tok = read_raw_non_empty();
        need_paren = true;
      }
      if (tok.type != TT_IDENTIFIER) {
        herr->error(tok, "Expected identifier for `defined` expression; got %s",
                    tok.to_string());
        identifier.type = TT_DECLITERAL;
        identifier.content = "0";
        return false;
      }

      auto m = macros.find(tok.content.toString());
      identifier.type = TT_DECLITERAL;
      identifier.content = m != macros.end() ? "1" : "0";

      if (need_paren && (tok = read_raw_non_empty()).type != TT_RIGHTPARENTH) {
        herr->error(tok, "Expected closing parenthesis to `defined` expression "
                         "here before %s", tok.to_string());
      }
      return false;
    }
    case LexerKeyword::HAS_INCLUDE: {
      bool incnext;
      if ((incnext = false)) {  // Do not "fix": Handle both expressions.
        case LexerKeyword::HAS_INCLUDE_NEXT:
        incnext = true;
      }

      string fnfind;
      bool chklocal = false;
      token_t tok = read_raw_non_empty();
      if (tok.type == TT_LEFTPARENTH) {
        tok = read_raw_non_empty();
        if (tok.type == TT_LESSTHAN) {
          while ((tok = read_raw_non_empty()).type != TT_GREATERTHAN) {
            if (tok.type == TT_ENDOFCODE) break;
            fnfind += tok.content.view();
          }
        } else if(tok.type == TT_STRINGLITERAL) {
          string_view v = tok.content.view();
          // Bit of a hack: remove quotes.
          // TODO: Replace with token.evaluate_string_literal(), or something.
          fnfind = v.substr(1, v.length() - 2);
          chklocal = true;
        } else {
          herr->error(tok, "Expected < or string literal in `__has_include()` "
                           "expression before %s", tok.to_string());
        }
      } else {
        herr->error(tok, "Expected `(` after `__has_include` before %s",
                    tok.to_string());
      }

      if ((tok = read_raw_non_empty()).type != TT_RIGHTPARENTH) {
        herr->error(tok, "Expected closing parenthesis after include path in "
                         "`__has_include()` expression");
      }
      identifier.content =
          try_find_and_open(cfile, builtin, fnfind, chklocal, incnext).is_open()
              ? "1" : "0";
      identifier.type = TT_DECLITERAL;
      return false;
    }
    case LexerKeyword::FUNC:
    case LexerKeyword::FUNC_PRETTY:
    case LexerKeyword::HAS_CPP_ATTRIBUTES:
    default: case LexerKeyword::CPP_COND_BEGIN:
      herr->error(identifier, "Internal error: Keyword %s not handled",
                  identifier.content.view());
  }

  keyword_map::iterator kwit = builtin->keywords.find(fn);
  if (kwit != builtin->keywords.end()) {
    if (kwit->second == TT_INVALID) {
      herr->error(identifier) << "Internal error: keyword " << PQuote(fn)
                              << " was defined as an invalid token";
      return false;
    }
    identifier.type = kwit->second;
    return false;
  }

  translate_identifier(identifier);
  return false;
}

bool lexer::translate_identifier(token_t &identifier) {
  if (identifier.type != TT_IDENTIFIER) return false;
  string fn = identifier.content.toString();
  keyword_map::iterator kwit = builtin->keywords.find(fn);
  if (kwit != builtin->keywords.end()) {
    if (kwit->second == TT_INVALID) {
      herr->error(identifier) << "Internal error: keyword " << PQuote(fn)
                              << " was defined as an invalid token";
      return false;
    }
    identifier.type = kwit->second;
    return false;
  }

  tf_iter tfit = builtin_declarators.find(fn);
  if (tfit != builtin_declarators.end()) {
    if ((tfit->second->usage & UF_PRIMITIVE_FLAG) == UF_PRIMITIVE) {
      identifier.type = TT_DECLARATOR;
      identifier.def = tfit->second->def;
    } else {
      identifier.type = TT_DECFLAG;
      identifier.tflag = tfit->second;
    }
    return false;
  }

  // TODO: This *feels* like a miserable hack, but it's this or make the AST
  // evaluator handle it, which could mask real problems. I suspect this'll move
  // around over time.
  if (evaluate_cpp_cond_) {
    identifier.type = TT_DECLITERAL;
    identifier.content = "0";
  }
  return false;
}

// XXX: This can probably be used as the basis of preprocess_and_read_token
token_t lexer::read_raw() {
  while (buffered_tokens) {
    if (buffer_pos >= buffered_tokens->size()) {
      if (!pop_buffer()) {
        return token_t(TT_ENDOFCODE, cfile.name.c_str(), cfile.lnum,
                       cfile.tell() - cfile.lpos, "", 0);
      }
      continue;
    }
    return (*buffered_tokens)[buffer_pos++];
  }
  return read_token(cfile, herr);
}

token_t lexer::read_raw_non_empty() {
  token_t res = read_raw();
  while (res.preprocesses_away()) res = read_raw();
  return res;
}

token_t lexer::preprocess_and_read_token() {
  token_t res;
  for (;;) {
    if (buffered_tokens) {
      if (buffer_pos >= buffered_tokens->size()) {
        if (!pop_buffer()) {
          return token_t(TT_ENDOFCODE, cfile.name.c_str(), cfile.lnum,
                         cfile.tell() - cfile.lpos, "", 0);
        }
        continue;
      }
      if (open_buffers.back().is_rewind) {
        return (*buffered_tokens)[buffer_pos++];
      }
      res = (*buffered_tokens)[buffer_pos++];
      if (res.type == TT_IDENTIFIER && handle_macro(res)) continue;
      return res;
    }
    do res = read_token(cfile, herr); while (res.preprocesses_away());
    if (res.type == TT_IDENTIFIER) {
      if (handle_macro(res)) continue;
    } else if (res.type == TTM_CONCAT) {
      res.report_error(herr, "Extraneous # ignored");
      handle_preprocessor();
      continue;
    } else if (res.type == TTM_TOSTRING) {
      handle_preprocessor();
      continue;
    } else if (res.type == TT_ENDOFCODE) {
      if (pop_file()) {
        return token_t(TT_ENDOFCODE, cfile.name.c_str(), cfile.lnum,
                       cfile.tell() - cfile.lpos, "", 0);
      }
      continue;
    }

    return res;
  }
}

token_t lexer::get_token() {
  token_t token = preprocess_and_read_token();
  while (token.preprocesses_away()) token = preprocess_and_read_token();
  if (lookahead_buffer) lookahead_buffer->push_back(token);
  return token;
}

token_t lexer::get_token_in_scope(jdi::definition_scope *scope) {
  token_t res = get_token();

  if (res.type == TT_IDENTIFIER) {
    definition *def = res.def = scope->look_up(res.content.toString());
    if (def) {
      res.type = (def->flags & DEF_TYPENAME) ? TT_DECLARATOR : TT_DEFINITION;
    }
  }

  return res;
}

void lexer::push_buffer(OpenBuffer &&buf) {
  assert(open_buffers.empty() == !buffered_tokens);
  if (buffered_tokens) {
    assert(open_buffers.empty() == !buffered_tokens);
    open_buffers.back().buf_pos = buffer_pos;
  }
  open_buffers.emplace_back(std::move(buf));
  buffered_tokens = &open_buffers.back().tokens;
  buffer_pos = 0;
}

void lexer::push_rewind_buffer(OpenBuffer &&buf) {
  push_buffer(std::move(buf));
  open_buffers.back().is_rewind = true;
}

void lexer::push_frozen_buffer(OpenBuffer &&buf) {
  push_buffer(std::move(buf));
  open_buffers.back().is_frozen = true;
}

bool lexer::pop_buffer() {
  assert(open_buffers.empty() == !buffered_tokens);
  assert(buffered_tokens);
  if (open_buffers.back().is_frozen) return false;
  open_buffers.pop_back();
  if (open_buffers.empty()) {
    buffered_tokens = nullptr;
  } else {
    buffered_tokens = &open_buffers.back().tokens;
    buffer_pos = open_buffers.back().buf_pos;
  }
  return true;
}

void lexer::pop_frozen_buffer() {
  if (open_buffers.empty()) {
    herr->error(cfile, "Internal error: A frozen buffer was somehow popped.");
    return;
  }
  if (!open_buffers.back().is_frozen) {
    herr->error(cfile) <<
        "Internal error: the current buffer is not frozen. "
        "Some tokens were not consumed.";
    for (auto it = open_buffers.rbegin(); it != open_buffers.rend(); ++it) {
      if (it->is_frozen) {
        while (pop_buffer());
        break;
      }
    }
  }
  open_buffers.back().is_frozen = false;
  pop_buffer();
}

bool lexer::pop_file() {
  /*
   * This lexer implementation has four layers of token source data:
   * 1. The open file stack. Files or string buffers (managed by an llreader)
   *    are lexed for raw tokens.
   * 2. Macros used within a file are expanded into tokens, and these buffers of
   *    tokens are stacked. Per ISO, a macro may not appear twice in this stack.
   * 3. Rewind operations produce queues of tokens. Each queue is stacked.
   * 4. During normal lexing operations, minor lookahead may be required.
   *    Tokens read during lookahead are stacked.
   */
  if (buffered_tokens) {
    if (buffer_pos < buffered_tokens->size()) {
      herr->error((*buffered_tokens)[buffer_pos],
                  "Internal error: Attempting to pop a file while there are "
                  "lexed tokens remaining to be returned.");
    } else {
      herr->error(cfile) << "Internal error: "
          "Attempting to pop a file without first popping open buffers.";
    }
  }

  if (files.empty())
    return true;

  // Close whatever file we have open now
  cfile.close();

  // Fetch data from top item and pop stack
  cfile.consume(files.back());
  files.pop_back();

  return false;
}

std::vector<SourceLocation> lexer::detailed_position() const {
  std::vector<SourceLocation> res;
  for (const auto &of : files) {
    res.emplace_back("File " + of.name, of.lnum, of.pos - of.lpos);
  }
  if (cfile.is_open())
    res.emplace_back("File " + cfile.name, cfile.lnum, cfile.pos - cfile.lpos);
  for (const auto &ob : open_buffers) if (ob.macro_info) {
    res.emplace_back("Usage of macro `" + ob.macro_info->name + "` in "
                                        + ob.macro_info->origin.file,
                     ob.macro_info->origin.linenum, ob.macro_info->origin.pos);
  }
  return res;
}

lexer::lexer(macro_map &pmacros, ErrorHandler *err):
    herr(err), macros(pmacros), builtin(&builtin_context()) {}

lexer::lexer(llreader &input, macro_map &pmacros, ErrorHandler *err):
    lexer(pmacros, err) {
  cfile.consume(input);
}

static macro_map no_macros;
lexer::lexer(token_vector &&tokens, const lexer &other):
    lexer(other.macros, other.herr) {
  push_buffer(std::move(tokens));
}
lexer::lexer(const token_vector *tokens, const lexer &other):
    lexer(other.macros, other.herr) {
  push_buffer(tokens);
}
lexer::lexer(const token_vector *tokens, ErrorHandler *err):
    lexer(no_macros, err) {
  push_rewind_buffer(tokens);
}

lexer::~lexer() {}

#undef cfile

lexer::condition::condition(bool t, bool pst):
    is_true(t), seen_else(false), parents_true(pst) {}
