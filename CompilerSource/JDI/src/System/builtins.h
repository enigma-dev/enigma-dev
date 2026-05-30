/**
 * @file  builtins.h
 * @brief A header declaring a centralized context representing built-in objects.
 * 
 * This file is used for configuring the parse environment ahead of time. Contents
 * of the builtin context are inherited by user-created contexts, and the contents
 * of \c builtin_flags determines accepted modifiers when parsing code in general,
 * regardless of the enclosing context.
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


#ifndef _JDI_BUILTINS__H
#define _JDI_BUILTINS__H

namespace jdip {
  class typeflag;
}

#include <API/context.h>

namespace jdi {
  /**
    A context containing built-in macros and compiler parameters.
    This context contains all types, macros, and search directories that are
    considered to be built in to the compiler. Here, you will find types such
    as int and double, macros such as __WIN32__ or __GNUG__, as well as search
    directories such as /usr/include.
  **/
  extern jdi::context *builtin;
  
  /**
    Read a list of declarators from a file, toggling on their usage with the given flags.
    @param filename      The name of the file from which to read the declarators.
    @param usage_flags   The usage flag to be enabled for the read declarators.
  **/
  void read_declarators(const char* filename, USAGE_FLAG usage_flags);
  /// Structure for getting info about newly-added declarator
  struct add_decl_info {
    definition* def; ///< The typeflag struct, as stored in the token.
    unsigned long flag; ///< The typeflag struct, as stored in the token.
    jdip::typeflag *tf_struct; ///< The typeflag struct, as stored in the token.
    add_decl_info(definition *d, unsigned long f, jdip::typeflag *tf);
  };
  /**
    Add a single declarator, toggling on its usage for the given flag.
    @param type_name     The name of the declarator, as it appears in the code.
    @param usage_flags   The usage flag to be enabled for the given declarator.
    @param prim_name     If this declarator can be used as a type, this is the name of that type if it is different from type_name.
    @return  Returns the definition of the primitive object by that type.
  **/
  add_decl_info add_declarator(string type_name, USAGE_FLAG usage_flags, size_t sz = 0, string prim_name = "");
  /// Add declarator flags used in the default GNU implementation.
  void add_gnu_declarators();
  /// Free memory for all declarators. This should be called only at program end.
  void cleanup_declarators();
  
  
  //==== Builtin Types ========================================
  //===========================================================
  
  extern unsigned long builtin_flag__volatile; ///< Builtin volatile flag
  extern unsigned long builtin_flag__static;   ///< Builtin static flag
  extern unsigned long builtin_flag__const;    ///< Builtin const flag
  extern unsigned long builtin_flag__mutable;  ///< Builtin mutable flag
  extern unsigned long builtin_flag__register; ///< Builtin register flag
  extern unsigned long builtin_flag__inline;   ///< Builtin inline flag
  extern unsigned long builtin_flag__Complex;  ///< Builtin complex flag
  
  extern unsigned long builtin_flag__unsigned; ///< Builtin unsigned flag
  extern unsigned long builtin_flag__signed;  ///< Builtin signed flag
  extern unsigned long builtin_flag__short;  ///< Builtin short flag
  extern unsigned long builtin_flag__long;  ///< Builtin long flag
  
  extern unsigned long builtin_flag__restrict;  ///< Builtin __restrict / __restrict__ flag
  
  extern definition *builtin_type__unsigned; ///< Builtin unsigned flag
  extern definition *builtin_type__signed;  ///< Builtin signed flag
  extern definition *builtin_type__short;  ///< Builtin short flag
  extern definition *builtin_type__long;  ///< Builtin long flag
  
  extern definition *builtin_type__void;   ///< Builtin void type
  extern definition *builtin_type__bool;   ///< Builtin char type
  extern definition *builtin_type__char;   ///< Builtin char type
  extern definition *builtin_type__int;    ///< Builtin int type
  extern definition *builtin_type__float;  ///< Builtin float type
  extern definition *builtin_type__double; ///< Builtin double type
  
  extern definition *builtin_type__wchar_t; ///< Builtin wchar_t type
  extern definition *builtin_type__va_list; ///< Builtin va_list type
  
  extern unsigned long builtin_flag__virtual; ///< Builtin virtual flag
  extern unsigned long builtin_flag__explicit; ///< Builtin explicit flag
  
  string typeflags_string(definition *type, unsigned flags);
}

namespace jdip {
  using namespace jdi;
  /// Class for storing information about a 'typeflag,' declarators such as 'int', 'short', and 'const' which refer to or modify primitives.
  class typeflag {
    friend add_decl_info jdi::add_declarator(string, USAGE_FLAG, size_t, string);
    friend void jdi::cleanup_declarators();
    
    typeflag(); ///< Construct a new type flag.
    ~typeflag();
    typeflag(string, USAGE_FLAG);
    
    public:
    /** The name of this flag, as represented in code.
        This is unambiguous, but may be redundant. For instance, 
        short and __short may both map to this type. **/
    string name;
    /** Tells us whether this is a flag or a primitive, or both.
        This is neither redundant nor ambiguous; short, __short,
        and __short__ will all have the same flag bit index. **/
    USAGE_FLAG usage;
    
    unsigned int flagbit; ///< If this is a flag, this is its bitmask index.
    definition *def; ///< If this is a primitive, this is the definition affiliated with it.
  };
  
  extern typeflag* builtin_typeflag__throw; ///< Builtin __throw typeflag, as stored in the token.
  
  typedef map<string,typeflag*> tf_map; ///< A map of declarators by name.
  typedef map<unsigned int,typeflag*> tf_flag_map; ///< A map of declarator flags by flag bit, as a power of two.
  typedef map<string,definition*> prim_map; ///< A map of definitions by name.
  typedef tf_map::iterator tf_iter; ///< An iterator type for \c tf_map, eg, \c builtin_declarators.
  typedef prim_map::iterator prim_iter; ///< An iterator type for \c prim_map, eg, \c builtin_primitives.
  /**
    A map of flags and primitives to their usage and absolute type.
    This map *only* contains flags. Types can be aliased through typedef,
    and as such must be represented in a /c jdi::context.
    @see jdi::USAGE_FLAG
    @see jdi::builtin
  **/
  extern tf_map builtin_declarators; ///< A map of all builtin declarators, such as const, unsigned, long, and int, by name.
  extern prim_map builtin_primitives; ///< A map of all builtin primitives, such as int and double, by name. By technicality, this includes long.
  extern tf_flag_map builtin_decls_byflag; ///< A map of all flags by their bit mask, in 1 << (0 to 31).
}

#endif
