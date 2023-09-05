/**
 * @file lex_cpp.h
 * @brief Header extending the \c lexer base class for parsing C++.
 * 
 * This file defines two subclasses of \c jdi::lexer. The first is meant to lex
 * C++ definitions, and so returns a wide range of token types. It will also do
 * any needed preprocessing, handing the second lexer to \c jdi::AST to handle
 * `#if` expression evaluation. The second lexer is much simpler, and treats all
 * identifiers the same.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2012 Josh Ventura
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

#ifndef _LEX_CPP__H
#define _LEX_CPP__H

namespace jdip {
  struct lexer_cpp;
  struct lexer_macro;
}

#include <set>
#include <string>
#include <API/lexer_interface.h>
#include <General/quickstack.h>
#include <General/llreader.h>
#include <API/context.h>

namespace jdip {
  using namespace jdi;
  /**
    @brief An extension of \c llreader which also stores information about the
           current line number and the position of the last line break.
  **/
  struct openfile {
    const char* filename; ///< The name of the open file.
    string searchdir; ///< The search directory from which this file was included, or the empty string.
    size_t line; ///< The index of the current line.
    size_t lpos; ///< The position of the most recent line break.
    llreader file; ///< The llreader of this file.
    openfile(); ///< Default constructor.
    openfile(const char* fname); ///< Construct a new openfile at position 0 with the given filename.
    /// Construct a new openfile with the works.
    /// @param fname     The name of the file in use
    /// @param sdir      The search directory from which this file was included, or the empty string
    /// @param line_num  The number of the line, to store
    /// @param line_pos  The position of the last newline, to store
    /// @param consume   The llreader to consume for storage
    openfile(const char* fname, string sdir, size_t line_num, size_t line_pos, llreader &consume);
    void swap(openfile&); ///< Swap with another openfile.
  };
  
  /**
    @brief An implementation of \c jdi::lexer for lexing C++. Handles preprocessing
           seamlessly, returning only relevant tokens.
  **/
  struct lexer_cpp: lexer, llreader {
    virtual token_t get_token(error_handler *herr = def_error_handler);
    quick::stack<openfile> files; ///< The files we have open, in the order we included them.
    macro_map &macros; ///< Reference to the \c jdi::macro_map which will be used to store and retrieve macros.
    
    const char* filename; ///< The name of the open file.
    string sdir; ///< The last loaded search directory.
    size_t line; ///< The current line number in the file
    size_t lpos; ///< The index in the file of the most recent line break.
    
    unsigned open_macro_count;
    
    /// Map of string to token type; a map-of-keywords type.
    typedef map<string,TOKEN_TYPE> keyword_map;
    /// List of C++ keywords, mapped to the type of their token.
    /// This list is assumed to contain tokens whose contents are unambiguous; one string maps to one token, and vice-versa.
    static keyword_map keywords;
    /// This is a map of macros to add bare-minimal support for a number of compiler-specific builtins.
    static macro_map kludge_map;
    
    /// Static cleanup function; safe to call without a matching init.
    static void cleanup();
    
    /** Sole constructor; consumes an llreader and attaches a new \c lex_macro.
        @param input    The file from which to read definitions. This file will be manipulated by the system.
        @param pmacros  A \c jdi::macro_map which will receive and be probed for macros.
        @param fname    The name of the file that was first opened.
    **/
    lexer_cpp(llreader& input, macro_map &pmacros, const char *fname = "stdcall/file.cpp");
    /** Destructor; free the attached macro lexer. **/
    ~lexer_cpp();
    
    /**
      Utility function designed to handle the preprocessor directive
      pointed to by \c pos upon invoking the function. Note that it should
      be the character directly after the pound pointed to upon invoking
      the function, not the pound itself.
      @param herr  The error handler to use if the preprocessor doesn't
                   exist or is malformed.
    **/
    void handle_preprocessor(error_handler *herr);
    
    /// Utility function to skip a single-line comment; invoke with pos indicating one of the slashes.
    void skip_comment();
    /// Utility function to skip a multi-line comment; invoke with pos indicating the starting slash.
    void skip_multiline_comment();
    /// Utility function to skip a string; invoke with pos indicating the quotation mark. Terminates indicating match.
    void skip_string(error_handler *herr);
    /// Skip anything that cannot be interpreted as code in any way.
    inline void skip_whitespace();
    /// Function used by the preprocessor to read in macro parameters in compliance with ISO.
    string read_preprocessor_args(error_handler *herr);
    /** Second-order utility function to skip lines until a preprocessor
        directive is encountered, then invoke the handler on the directive it found. **/
    void skip_to_macro(error_handler *herr);
    
    /// Enter a scalar macro, if it has any content.
    /// @param ms   The macro scalar to enter.
    void enter_macro(macro_scalar *ms);
    /// Parse for parameters to a given macro function, if there are any, then evaluate
    /// the macro function and set the open file to reflect the change.
    /// This call should be made while the position is just after the macro name.
    /// @param mf   The macro function to parse
    /// @param herr An error handler in case of parameter mismatch or non-terminated literals
    /// @return Returns whether parameters were encountered and parsed.
    bool parse_macro_function(const macro_function* mf, error_handler *herr);
    /// Parse for parameters to a given macro function, if there are any.
    /// This call should be made while the position is just after the macro name.
    /// @param mf    The macro function to parse.
    /// @param dest  The vector to receive the individual parameters [out].
    /// @param herr  An error handler in case of parameter mismatch or non-terminated literals.
    /// @return Returns whether parameters were encountered and parsed.
    bool parse_macro_params(const macro_function* mf, vector<string>& dest, error_handler *herr);
    /// Parse for parameters to a given macro function, if there are any.
    /// This call should be made while the position is just after the macro name.
    /// @param mf    The macro function to parse.
    /// @param cfile The buffer to read from.
    /// @param pos   The position in the buffer to read; modified as used [in-out].
    /// @param len   The length of the given buffer.
    /// @param dest  The vector to receive the individual parameters [out].
    /// @param errep A token to use to report errors [in].
    /// @param herr  An error handler in case of parameter mismatch or non-terminated literals.
    /// @return Returns whether parameters were encountered and parsed.
    static bool parse_macro_params(const macro_function* mf, const macro_map &macros, const char* cfile, size_t &pos, size_t length, vector<string>& dest, const token_t &errep, error_handler *herr);
    
    /// Pop the currently open file or active macro.
    /// @return Returns whether the end of all input has been reached.
    bool pop_file();
    
    set<string> visited_files; ///< For record and reporting purposes only.
  protected:
    /// Storage mechanism for conditionals, such as <code>\#if</code>, <code>\#ifdef</code>, and <code>\#ifndef</code>.
    struct condition {
      bool is_true; ///< True if code in this layer is to be parsed; ie, the condition given is true.
      bool can_be_true; ///< True if an `else` statement or the like can set is_true to true.
      condition(bool,bool); ///< Convenience constructor.
      condition(); ///< Default constructor.
    };
    /// FLatten a macro parameter, evaluating nested macro functions.
    static string _flatten(string param, const macro_map& macros, const token_t &errep, error_handler *herr);
    quick::stack<condition> conditionals; ///< Our conditional levels (one for each nested `\#if*`)
    lexer_macro *mlex; ///< The macro lexer that will be passed to the AST builder for #if directives.
    context_parser *mctex; ///< A context used for constructing ASTs from preprocessor expressions.
  };
  
  /**
    An implementation of \c jdi::lexer for handling macro expressions.
    Unrolls macros automatically. Treats non-macro identifiers as zero.
    Replaces `defined x` with 0 or 1, depending on whether x is defined.
  **/
  struct lexer_macro: lexer {
    const char* cfile;
    size_t &pos, length;
    lexer_cpp *lcpp;
    token_t get_token(error_handler *herr = def_error_handler);
    lexer_macro(lexer_cpp*);
    void update();
  };
}

#endif
