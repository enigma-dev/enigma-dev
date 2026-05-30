/**
 * @file token.h
 * @brief Header defining token types.
 * 
 * These are system constants; use only if you have read the \c jdip disclaimer.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _TOKEN__H
#define _TOKEN__H

#include <string>

namespace jdip {
  enum TOKEN_TYPE {
    TT_DECLARATOR, ///< Phrases like int, struct {}, std::map; anything that can be used as a type. 
    TT_DECFLAG,    ///< Phrases which can be used multiple times in a declaration as flags, like const, unsigned, long. 
    TT_CLASS,      ///< The `class' keyword.
    TT_STRUCT,     ///< The `struct' keyword.
    TT_ENUM,       ///< The `enum' keyword.
    TT_UNION,      ///< The `union' keyword.
    TT_NAMESPACE,  ///< The `namespace' keyword.
    TT_EXTERN,     ///< The `extern' keyword.
    
    TT_ASM,        ///< The `asm' keyword.
    TT_OPERATORKW, ///< The `operator' keyword; TT_OPERATOR is a generic operator.
    TT_SIZEOF,     ///< The `sizeof' keyword.
    TT_ISEMPTY,    ///< The `is_empty' keyword.
    TT_DECLTYPE,   ///< The `decltype' keyword.
    TT_TYPEID,     ///< The `typeid' keyword.
    
    TT_ALIGNAS,       ///< The `alignas' specifier.      (C++11)
    TT_ALIGNOF,       ///< The `alignof' operator.       (C++11)
    TT_AUTO,          ///< The `auto' type resolver.     (C++11)
    TT_CONSTEXPR,     ///< The `constexpr' specifier.    (C++11)
    TT_NOEXCEPT,      ///< The `noexcept' specifier.     (C++11)
    TT_STATIC_ASSERT, ///< The `static_assert' operator. (C++11)
    
    TT_IDENTIFIER, ///< A standard identifier.
    TT_DEFINITION, ///< Something previously declared that is not immediately useful, like a field or function.
    
    TT_TEMPLATE,   ///< The `template' keyword, which should be followed by <...>
    TT_TYPENAME,   ///< The `typename' keyword.
    
    TT_TYPEDEF,    ///< The `typedef' keyword.
    TT_USING,      ///< The `using' keyword.
    
    TT_PUBLIC,     ///< The `public' keyword.
    TT_PRIVATE,    ///< The `private' keyword.
    TT_PROTECTED,  ///< The `protected' keyword.
    TT_FRIEND,     ///< The `friend' keyword.
    
    TT_COLON,      ///< A simple colon, which should always mark a label.
    TT_SCOPE,      ///< The scope accessor `::' symbol.
    TT_MEMBEROF,   ///< A pseudo-token representing ::*. Will be returned instead of * when reading passes the ::.
    
    TT_LEFTPARENTH,   ///< A left parenthesis, `('.
    TT_RIGHTPARENTH,  ///< A right parenthesis, `)'.
    TT_LEFTBRACKET,   ///< A left bracket, `['.
    TT_RIGHTBRACKET,  ///< A right bracket, `]'.
    TT_LEFTBRACE,     ///< A left brace, `{'.
    TT_RIGHTBRACE,    ///< A right brace, `}'.
    TT_LESSTHAN,      ///< A less-than symbol, or left triangle bracket, `<'.
    TT_GREATERTHAN,   ///< A greater-than symbol, or right triangle bracket, `>'.
    
    TT_TILDE,         ///< The tilde `~' symbol.
    TT_ELLIPSIS,      ///< An ellipsis: Three dots. (...)
    TT_OPERATOR,      ///< Any sort of operator. This token is used only when the token has no other purpose but as an operator.
    
    TT_COMMA,         ///< A comma, `,'. Separates items in lists.
    TT_SEMICOLON,     ///< A semicolon, `;'. Separates statements and declarations.
    
    TT_STRINGLITERAL, ///< A string literal, such as "hello, world!"
    TT_CHARLITERAL,   ///< A character literal, such as 'h'
    TT_DECLITERAL,    ///< A decimal literal, such as 12345
    TT_HEXLITERAL,    ///< A hexadecimal literal, such as 0xDEC0DED
    TT_OCTLITERAL,    ///< An octal literal, such as 07654321.
    
    TTM_CONCAT,       ///< A macro-only token meaning the concatenation of two tokens to form a new token, '##'.
    TTM_TOSTRING,     ///< A macro-only token meaning the value of a parameter, treated as a string literal, '#'.
    
    TT_NEW,           ///< The `new' keyword.
    TT_DELETE,        ///< The `delete' keyword.
    
    TT_CONST_CAST,       ///< The `const_cast' operator
    TT_STATIC_CAST,      ///< The `static_cast' operator
    TT_DYNAMIC_CAST,     ///< The `dynamic_cast' operator
    TT_REINTERPRET_CAST, ///< The `reinterpret_cast' operator
    
    TT_ENDOFCODE,     ///< This token signifies that the code has reached its end.
    #include <User/token_types.h>
    TT_INVALID        ///< Invalid token; read failed.
  };
  
  #ifdef DEBUG_MODE
    /// A debug listing of token descriptions by value ID
    extern const char* TOKEN_TYPE_NAME[];
  #endif
  
  struct token_t;
}



//=========================================================================================================
//===: Implementation carries extended dependencies:=======================================================
//=========================================================================================================

#include <API/error_reporting.h>
#include <Storage/definition_forward.h>

namespace jdip {
  using std::string;
  using namespace jdi; // If you are in the private namespace, you probably intend access to the regular namespace as well.
  
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
    
    /// Construct a new, invalid token.
    token_t();
    #ifndef NO_ERROR_REPORTING
      /// Log the name of the file from which this token was read.
      volatile const char* file;
      /// Log the line on which this token was named in the file.
      int linenum;
      #ifndef NO_ERROR_POSITION
        /// We are logging positions for precise error reporting.
        int pos;
        
        /// Construct a new token with the file, line number, and position in which it was declared.
        #define full_token_constructor_parameters int, const char*, int, int
        /// Wrapper to the token_t constructor which can ignore removed attributes.
        #define token_basics(type,filename,line,pos) type,filename,line,pos
      #else
        /// Wrapper to the token_t constructor which can ignore removed attributes.
        #define token_basics(type,filename,line,pos) type,filename,line
        /// Construct a new token with the file and line number in which it was declared.
        #define full_token_constructor_parameters int, const char*, int
      #endif
      /// Construct a new token without origin information.
      token_t(TOKEN_TYPE t, const char* fn, int l);
    #else
      /// Wrapper to the token_t constructor which can ignore removed attributes.
      #define token_basics(type,filename,line,pos) type
      #define full_token_constructor_parameters TOKEN_TYPE, const char*, int
    #endif
    
    /// Structure containing a pointer inside a string, and a length, representing a substring.
    struct content {
      /// A pointer to a substring of a larger buffer of code. NEITHER is null-terminated!
      /// This pointer is to be considered volatile as the buffer belongs to the system and
      /// can be modified or freed as soon as the file is closed. As such, any use of it must
      /// be made before the file is closed.
      volatile const char* str;
      
      /// The length of the string pointed to by \c content.
      int len;
      
      /// Get the string contents of this token: This operation is somewhat costly.
      inline string toString() { return string((const char*)str,len); }
      
      inline content() {}
      inline content(const char* s, int l): str(s), len(l) {}
    } content;
    
    /// For types, namespace-names, etc., the definition by which the type of this token was determined.
    definition* def;
    
    /// Construct a token with the requested amount of information.
    token_t(token_basics(TOKEN_TYPE t, const char* fn, int l, int p));
    /// Construct a token with extra information regarding its content.
    token_t(token_basics(TOKEN_TYPE t, const char* fn, int l, int p), const char*, int);
    /// Construct a token with extra information regarding its definition.
    token_t(token_basics(TOKEN_TYPE t, const char* fn, int l, int p), definition*);
    
    /**
      Pass error information to an error handler.
      If no information is available, then zeros are copied in its place.
      @param herr  The error_handler which will receive this notification.
      @param error The text of the error.
    **/
    void report_error(error_handler *herr, std::string error) const;
    /**
      Pass error information to an error handler, inserting token name.
      If no information is available, then zeros are copied in its place.
      The token is inserted in place of any %s in the error string.
      @param herr  The error_handler which will receive this notification.
      @param error The text of the error; use %s for token name.
    **/
    void report_errorf(error_handler *herr, std::string error) const;
    /**
      Pass error information to an error handler.
      If no information is available, then zeros are copied in its place.
      @param herr  The error_handler which will receive this notification.
      @param error The text of the error.
    **/
    void report_warning(error_handler *herr, std::string error) const;
  };
}

#endif
