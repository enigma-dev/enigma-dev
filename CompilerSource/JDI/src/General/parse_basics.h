/**
 * @file parse_basics.h
 * @brief Header defining basic methods required during a parse.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <string>
#include <cstdlib>
typedef size_t pt;
/** Namespace defining basic utility functions.
    Its public members are used for you from the
    enclosing header. **/
namespace parse_bacics {
  /** Small class meant to compensate for
      a lack of static code blocks in C++. **/
  extern struct is_
  {
    int whatever[256]; ///< The array of character type flags by character ordinal.
    enum {
      LETTER   = 1,  ///< Flag set if the character is a valid C++ identifier.
      DIGIT    = 2,  ///< Flag set if the character is a digit from 0-9.
      LETTERD  = 3,  ///< The bitwise OR of the LETTER and DIGIT flags.
      HEXDIGIT = 4,  ///< Flag set if the character is a hexadecimal digit (0-9, A-F, a-f).
      OCTDIGIT = 8,  ///< Flag set if the character is an octal digit (0-7).
      SPACER   = 16, ///< Flag set if the character is a non-newline whitespace character.
      NEWLINE  = 32, ///< Flag set if the character is a newline whitespace character.
      USELESS  = 48  ///< The bitwise OR of the two newline flags.
    };
    /// A default constructor in lieu of static {}.
    is_();
  } is; ///< A global instance of \c is_ so we can have our static code run.
  
  /// The public namespace of non-conflicting function names.
  namespace visible {
    /// Returns whether the given character is a valid C++ identifier.
    inline bool is_letter(char x) {
      return is.whatever[(unsigned char) x] & is.LETTER;
    }
    
    /// Returns whether the given character is a digit from 0 to 9.
    inline bool is_digit(char x) {
      return is.whatever[(unsigned char) x] & is.DIGIT;
    }
    
    /// Returns whether the given character is an octal digit from 0 to 7.
    inline bool is_octdigit(char x) {
      return is.whatever[(unsigned char) x] & is.OCTDIGIT;
    }
    
    /// Returns whether the given character is a hexadecimal digit.
    /// These can be from 0 to 9, or from A-F or a-f.
    inline bool is_hexdigit(char x) {
      return is.whatever[(unsigned char) x] & is.HEXDIGIT;
    }
    
    /// Returns whether the given character is either a letter or a digit.
    /// In other words, returns \c is_letter(x) || \c is_digit(x).
    inline bool is_letterd(char x) {
      return is.whatever[(unsigned char) x] & is.LETTERD;
    }
    
    /// Returns whether the given character is white.
    /// This includes spaces, tabs, and newline characters.
    inline bool is_useless(char x) {
      return is.whatever[(unsigned char) x] & is.USELESS;
    }
    
    /// Returns whether the given character is white, but not a newline.
    /// In other words, returns if the character is a space or tab.
    inline bool is_spacer(char x) {
      return is.whatever[(unsigned char) x] & is.SPACER;
    }
    
    using std::string;
    string toString(int);
    string toString(long);
    string toString(short);
    string toString(unsigned);
    string toString(unsigned long);
    string toString(unsigned short);
    string toString(char);
    string toString(char*);
    string toString(float);
    string toString(double);
  }
}
using namespace parse_bacics::visible;
