/**
 * @file token.h
 * @brief Header defining token types.
 *
 * These are system constants; use only if you have read the \c jdi disclaimer.
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

#ifndef _TOKEN__H
#define _TOKEN__H

#include <string>
#include <vector>

namespace jdi {
  constexpr int kGlossBits = 5;

  enum GLOSS_TOKEN_TYPE {
    GTT_DECLARATOR,     ///< Type names and modifiers.
    GTT_CONSTRUCT,      ///< Tokens like class, struct, enum, union, namespace.
    GTT_TYPEOP,         ///< Operations like sizeof, alignof, is_empty, decltype...
    GTT_VISIBILITYSPEC, ///< Visibility specifier, like public or private.
    GTT_IDENTIFIER,     ///< An identifier, with or without an attached definition.
    GTT_USING,          ///< The `using` keyword.
    GTT_TEMPLATE,       ///< The `template` keyword.
    GTT_ASM,            ///< The `using` keyword.
    GTT_OPERATORKW,     ///< The `operator` keyword.
    GTT_SEMICOLON,      ///< Semicolon.
    GTT_ARITHMETIC,     ///< Arithmetic and comparison operators, such as `+` or `==`.
    GTT_EQUAL,          ///< Assignment operators, such as `=`, `+=`, and `<<=`.
    GTT_RELATIVE_ASSIGN,///< Assignment operators, such as `=`, `+=`, and `<<=`.
    GTT_ANGLE,          ///< Angle brackets or comparison operators, like `<` and `>>`.
    GTT_OPERATORMISC,   ///< Miscellaneous operators that appear outside of expressions, like `:` or `~`.
    GTT_LITERAL,        ///< String or numeric literals.
    GTT_BRACKET,        ///< Brackets, including braces and parentheses, but not angle brackets (GTT_ANGLE).
    GTT_MEMORYOP,       ///< Memory and cast operators, eg, `new`, `delete`, `static_cast`.
    GTT_CONTROL,        ///< Control statements, such as `return`, `break`, `continue`, `throw`.
    GTT_PREPROCESSOR,   ///< Preprocessor-only tokens.
    GTT_ENDOFCODE,      ///< Signals the end of input.
    GTT_INVALID         ///< Invalid; read failed.
  };

  /// Used when defining `TOKEN_TYPE`s to choose a specific token value based on a gloss token value.
  constexpr int GLOSS(int gloss_token) { return gloss_token << kGlossBits; }

  enum TOKEN_TYPE {
    TT_DECLARATOR = GLOSS(GTT_DECLARATOR), ///< Primitive type catch-it-all.
    TT_DECFLAG,    ///< Modifiers and storage specifiers, like const, unsigned, long.
    TT_TYPENAME,   ///< The `typename` keyword.
    TT_INLINE,     ///< The `inline` keyword.
    TT_EXTERN,     ///< The `extern` keyword.
    TT_TYPEDEF,    ///< The `typedef` keyword.
    TT_AUTO,       ///< The `auto` type resolver.        (C++11)
    TT_ALIGNAS,    ///< The `alignas` specifier.         (C++11)
    TT_ATTRIBUTE,  ///< The GNU `__attribute__` specifier.
    TT_CONSTEXPR,  ///< The `constexpr` specifier.       (C++11)
    TT_NOEXCEPT,   ///< The `noexcept` specifier.        (C++11)

    TT_CLASS      = GLOSS(GTT_CONSTRUCT), ///< The `class` keyword.
    TT_STRUCT,     ///< The `struct` keyword.
    TT_ENUM,       ///< The `enum` keyword.
    TT_UNION,      ///< The `union` keyword.
    TT_NAMESPACE,  ///< The `namespace` keyword.

    TT_USING      = GLOSS(GTT_USING),      ///< The `using` keyword.
    TT_TEMPLATE   = GLOSS(GTT_TEMPLATE),   ///< The `template` keyword.
    TT_ASM        = GLOSS(GTT_ASM),        ///< The `asm` keyword.
    TT_OPERATORKW = GLOSS(GTT_OPERATORKW), ///< The `operator` keyword.

    TT_SIZEOF    = GLOSS(GTT_TYPEOP), ///< The `sizeof` keyword.
    TT_ALIGNOF,    ///< The `alignof` operator.          (C++11)
    TT_DECLTYPE,   ///< The `decltype` keyword.          (C++11)
    TT_TYPEOF,     ///< The GNU `typeof` keyword. Like decltype, but accepts bare types and does not include references.
    TT_ISEMPTY,    ///< The `is_empty` keyword.
    TT_TYPEID,     ///< The `typeid` keyword.
    TT_STATIC_ASSERT, ///< The `static_assert` operator. (C++11)

    TT_IDENTIFIER = GLOSS(GTT_IDENTIFIER), ///< A standard identifier.
    TT_DEFINITION, ///< Something previously declared that is not immediately useful, like a field or function.

    TT_PUBLIC = GLOSS(GTT_VISIBILITYSPEC), ///< The `public` keyword.
    TT_PRIVATE,    ///< The `private` keyword.
    TT_PROTECTED,  ///< The `protected` keyword.
    TT_FRIEND,     ///< The `friend` keyword.

    TT_PLUS = GLOSS(GTT_ARITHMETIC), ///< The `+` operator.
    TT_MINUS,         ///< The `-` operator.
    TT_STAR,          ///< The `*` operator.
    TT_SLASH,         ///< The `/` operator.
    TT_MODULO,        ///< The `%` operator.
    TT_NOT_EQUAL_TO,  ///< The `!=` operator.
    TT_EQUAL_TO,      ///< The `==` operator.
    TT_LESS_EQUAL,    ///< The `<=` operator.
    TT_GREATER_EQUAL, ///< The `>=` operator.
    TT_AMPERSAND,     ///< The `&` operator.
    TT_AMPERSANDS,    ///< The `&&` operator.
    TT_PIPE,          ///< The `|` operator.
    TT_PIPES,         ///< The `||` operator.
    TT_CARET,         ///< The `^` operator.
    TT_INCREMENT,     ///< The `++` operator.
    TT_DECREMENT,     ///< The `--` operator.
    TT_ARROW,         ///< The `->` operator.
    TT_DOT,           ///< The `.` operator.
    TT_ARROW_STAR,    ///< The `->*` operator.
    TT_DOT_STAR,      ///< The `.*` operator.
    TT_QUESTIONMARK,  ///< The ternary `?` operator.

    TT_EQUAL = GLOSS(GTT_EQUAL), ///< The `=` operator.

    TT_ADD_ASSIGN = GLOSS(GTT_RELATIVE_ASSIGN), ///< The `+=` operator.
    TT_SUBTRACT_ASSIGN, ///< The `-=` operator.
    TT_MULTIPLY_ASSIGN, ///< The `*=` operator.
    TT_DIVIDE_ASSIGN,   ///< The `/=` operator.
    TT_MODULO_ASSIGN,   ///< The `%=` operator.
    TT_LSHIFT_ASSIGN,   ///< The `<<=` operator.
    TT_RSHIFT_ASSIGN,   ///< The `>>=` operator.
    TT_AND_ASSIGN,      ///< The `&=` operator.
    TT_OR_ASSIGN,       ///< The `|=` operator.
    TT_XOR_ASSIGN,      ///< The `^=` operator.
    TT_NEGATE_ASSIGN,   ///< The `~=` operator.

    TT_LESSTHAN = GLOSS(GTT_ANGLE), ///< A less-than symbol, or left angle bracket, `<`.
    TT_GREATERTHAN, ///< A greater-than symbol, or right angle bracket, `>`.
    TT_LSHIFT,     ///< The `<<` operator.
    TT_RSHIFT,     ///< The `>>` operator.

    TT_COLON = GLOSS(GTT_OPERATORMISC),  ///< A simple colon, which should always mark a label.
    TT_SCOPE,        ///< The scope accessor `::` symbol.
    TT_MEMBER,       ///< The member pointer access operator, `::*`.
    TT_TILDE,        ///< The tilde `~` symbol.
    TT_NOT,          ///< The `!` symbol or `not` keyword.
    TT_ELLIPSIS,     ///< An ellipsis: Three dots. (...)
    TT_COMMA,         ///< A comma, `,`. Separates items in lists.

    TT_SEMICOLON = GLOSS(GTT_SEMICOLON),  ///< A semicolon, `;`. Separates different statements and declarations.

    TT_LEFTPARENTH = GLOSS(GTT_BRACKET), ///< A left parenthesis, `(`.
    TT_RIGHTPARENTH,  ///< A right parenthesis, `)`.
    TT_LEFTBRACKET,   ///< A left bracket, `[`.
    TT_RIGHTBRACKET,  ///< A right bracket, `]`.
    TT_LEFTBRACE,     ///< A left brace, `{`.
    TT_RIGHTBRACE,    ///< A right brace, `}`.

    TT_STRINGLITERAL = GLOSS(GTT_LITERAL), ///< A string literal, such as "hello, world!".
    TT_CHARLITERAL,   ///< A character literal, such as 'h'.
    TT_DECLITERAL,    ///< A decimal literal, such as 12345.
    TT_HEXLITERAL,    ///< A hexadecimal literal, such as 0xDEC0DED.
    TT_OCTLITERAL,    ///< An octal literal, such as 07654321.
    TT_BINLITERAL,    ///< A binary literal, such as 0b1001010.
    TT_TRUE,          ///< The true keyword. One of two boolean literals.
    TT_FALSE,         ///< The false keyword. One of two boolean literals.

    TT_NEW = GLOSS(GTT_MEMORYOP), ///< The `new` keyword.
    TT_DELETE,           ///< The `delete` keyword.
    TT_CONST_CAST,       ///< The `const_cast` operator
    TT_STATIC_CAST,      ///< The `static_cast` operator
    TT_DYNAMIC_CAST,     ///< The `dynamic_cast` operator
    TT_REINTERPRET_CAST, ///< The `reinterpret_cast` operator
    TT_EXTENSION,        ///< The GNU `__extension__` indicator.

    TT_THROW = GLOSS(GTT_CONTROL),  ///< The `throw` keyword.

    TTM_CONCAT = GLOSS(GTT_PREPROCESSOR), ///< A macro-only token meaning the concatenation of two tokens to form a new token, `##`.
    TTM_TOSTRING, ///< A macro-only token meaning the value of a parameter, treated as a string literal, `#`.
    TTM_WHITESPACE, ///< A comment, including the symbols that delimit the comment (excluding any newline).
    TTM_COMMENT, ///< A comment, including the symbols that delimit the comment (excluding any newline).
    TTM_NEWLINE, ///< A newline, which has semantic meaning in preprocessor directives.

    TT_ENDOFCODE = GLOSS(GTT_ENDOFCODE), ///< This token signifies that the code has reached its end.
    #include <User/token_types.h>
    TT_INVALID = GLOSS(GTT_INVALID) ///< Invalid token; read failed.
  };

  struct token_t;
}

namespace std {
  string to_string(jdi::TOKEN_TYPE type);
  string to_string(const jdi::token_t &token);
}

namespace jdi {
  static inline
  std::ostream &operator<<(std::ostream &stream, jdi::TOKEN_TYPE type) {
    return stream << std::to_string(type);
  }

  static inline
  std::ostream &operator<<(std::ostream &stream, const jdi::token_t &token) {
    return stream << std::to_string(token);
  }
}

//=========================================================================================================
//===: Implementation carries extended dependencies:=======================================================
//=========================================================================================================

#include <API/error_reporting.h>
#include <Storage/definition_forward.h>

namespace jdi {
  using std::string;
  using std::string_view;

  /**
    A structure for representing complete info about a token.

    First and foremost, the structure denotes the type of the token. If that
    type is insufficient to discern the text of the token, the text is included
    as well in the \c token_t::extra member. Otherwise, there is little point
    to maintaining a pointer to that information in memory.

    If the type of the token is a declarator or other sort of object backed by
    a \c definition, then \c token_t::extra member will contain a \c definition*.

    For reasons of error reporting, the structure also contains information on
    the origin of the token: the filename, the line number, and the position.
  **/
  struct token_t {
    TOKEN_TYPE type; ///< The type of this token
    inline GLOSS_TOKEN_TYPE gloss_type() const {
      return (GLOSS_TOKEN_TYPE) (type >> kGlossBits);
    }

    /// Log the name of the file from which this token was read.
    std::string file;
    /// Log the line on which this token was named in the file.
    size_t linenum;
    /// We are logging positions for precise error reporting.
    size_t pos;

    std::string_view get_filename() const { return file; }
    size_t get_line_number()        const { return linenum; }
    size_t get_line_position()      const { return pos; }

    union {
      /// For types, namespace-names, etc., the definition by which the type
      /// of this token was determined.
      definition *def;
      /// For TT_DECFLAG, the typeflag lookup for this token.
      typeflag *tflag;
    };

    /// Structure containing a pointer inside a string, and a length, representing a substring.
    struct content {
      /// A pointer to a substring of a larger buffer of code. NEITHER is null-terminated!
      /// This pointer is to be considered volatile as the buffer belongs to the system and
      /// can be modified or freed as soon as the file is closed. As such, any use of it must
      /// be made before the file is closed.
      volatile const char* str;

      /// The length of the string pointed to by \c content.
      size_t len;

      /// Lazy string value initialization via caching. Also allows external ownership.
      mutable const std::string *cached_str = nullptr;
      /// Allows building, then caching.
      mutable std::string owned_str;

      /// Get the string contents of this token: This operation is somewhat costly.
      inline const string &toString() const {
        if (cached_str) return *cached_str;
        owned_str = string((const char*) str, len);
        cached_str = &owned_str;
        return *cached_str;
      }

      /// Return this content as a string_view.
      string_view view() const { return {(const char*) str, len}; }

      /// Copy another token's content, handling ownership.
      void copy(const content&);
      /// Copy another token's content, handling ownership.
      void consume(content&&);

      inline content() {}
      inline content(const char *s, int l): str(s), len(l) {}
      inline content(std::string_view sv): str(sv.data()), len(sv.length()) {}
      inline content(const string *s):
          str(s->data()), len(s->length()), cached_str(s) {}
      inline content(string &&s): owned_str(std::move(s)) {
        str = owned_str.data();
        len = owned_str.length();
        cached_str = &owned_str;
      }
      template<size_t n>
      inline content(const char (&sv)[n]): str(sv), len(n) {}

      content(const content &other) { copy(other); }
      content(content &&other) { consume(std::move(other)); }
      content &operator=(const content &other) {
        copy(other);
        return *this;
      }
      content &operator=(content &&other) {
        consume(std::move(other));
        return *this;
      }
    } content;

    void validate() const;

    /// Returns whether this token preprocesses to nothing.
    /// This includes whitespace and comments.
    bool preprocesses_away() const {
      return gloss_type() == GTT_PREPROCESSOR && (
          type == TTM_NEWLINE    ||
          type == TTM_WHITESPACE ||
          type == TTM_COMMENT);
    }

    /// Construct a new, invalid token.
    token_t():
        type(TT_INVALID), file("<no file>"), linenum(0), pos(-1), def(nullptr),
        content("default token") { validate(); }
    /// Construct a token with extra information regarding its content.
    token_t(TOKEN_TYPE t, string_view fn, int l, int p,
            const char *content_data, size_t content_length):
        type(t), file(fn), linenum(l), pos(p), def(nullptr),
        content(content_data, content_length) { validate(); }
    /// Construct a token with extra information regarding its content.
    token_t(TOKEN_TYPE t, string_view fn, int l, int p, string_view cntnt):
        type(t), file(fn), linenum(l), pos(p), def(nullptr), content(cntnt) {
             validate(); }
    /// Construct a token with extra information regarding its definition.
    token_t(TOKEN_TYPE t, string_view fn, int l, int p,
            definition *d, string_view name):
        type(t), file(fn), linenum(l), pos(p), def(d), content(name) {
            validate();
    }
    /// Construct a DECFLAG token with extra information about its meaning.
    token_t(TOKEN_TYPE t, string_view fn, int l, int p,
            typeflag *tf, string_view name):
        type(t), file(fn), linenum(l), pos(p), tflag(tf), content(name) {
            validate();
    }

    bool operator==(const token_t &tok) const {
      return type == tok.type && file == tok.file
            && linenum == tok.linenum && pos == tok.pos
            && content.toString() == tok.content.toString();
    }

    /**
      Pass error information to an error handler.
      If no information is available, then zeros are copied in its place.
      @param herr  The ErrorHandler which will receive this notification.
      @param error The text of the error.
    **/
    void report_error(ErrorHandler *herr, std::string error) const;
    /**
      Pass error information to an error handler, inserting token name.
      If no information is available, then zeros are copied in its place.
      The token is inserted in place of any %s in the error string.
      @param herr  The ErrorHandler which will receive this notification.
      @param error The text of the error; use %s for token name.
    **/
    void report_errorf(ErrorHandler *herr, std::string error) const;
    /**
      Pass error information to an error handler.
      If no information is available, then zeros are copied in its place.
      @param herr  The ErrorHandler which will receive this notification.
      @param error The text of the error.
    **/
    void report_warning(ErrorHandler *herr, std::string error) const;

    /// Comparatively-slow method to represent this token as a human-readable string.
    std::string to_string() const;

    static std::string get_name(TOKEN_TYPE type);
  };

  typedef std::vector<token_t> token_vector;
}

#endif
