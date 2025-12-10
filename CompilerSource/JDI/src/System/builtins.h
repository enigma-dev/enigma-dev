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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/


#ifndef _JDI_BUILTINS__H
#define _JDI_BUILTINS__H

namespace jdi {
  class typeflag;
}

#include <API/context.h>

namespace jdi {

/**
  Retrieves the builtin context, which contains built-in macros and compiler
  parameters.

  This context contains all types, macros, and search directories that are
  considered to be built into the compiler. Here, you will find types such
  as int and double, macros such as __WIN32__ or __GNUG__, as well as search
  directories such as /usr/include.
**/
Context &builtin_context();

/**
  Read a list of declarators from a file, toggling on their usage with the given flags.
  @param filename      The name of the file from which to read the declarators.
  @param usage_flags   The usage flag to be enabled for the read declarators.
**/
void read_declarators(const char* filename);

/// Add a built-in flag. Used to add flags like volatile, const, unsigned, long.
/// If the flag was already defined, it is returned as-is, with no modification.
typeflag *add_decflag(string name, USAGE_FLAG usage = UF_FLAG, int bitsize = 1);
/// Add a built-in flag with the same bits as another flag. The specified value
/// will be ANDed with the mask before assignment.
typeflag *add_decflag(string name, typeflag *base, int value);
/// Add a built-in primitive. Used to add types like `int`, `char`, `double`,
/// and `__va_list`. If the primitive was already defined, it is returned as-is.
definition *add_primitive(string name, size_t sz);
/// Create a new alias for a given flag.
void alias_decflag(string name, typeflag *flag);
/// Create a new alias for a given primitive.
void alias_primitive(string name, definition *def);

/// Add declarator flags used in the default GNU implementation.
void add_gnu_declarators();
/// Free memory for all declarators. This should be called only at program end.
void cleanup_declarators();


//==== Builtin Types ========================================
//===========================================================

string typeflags_string(definition *type, unsigned long flags);

/// Class for storing information about a "typeflag," a term coined here to
/// refer to anything in the declaration-seq that is not the
/// defining-type-specifier. This includes most decl-specifiers, both
/// cv-qualifications, both function-specifiers, all storage-class-specifiers,
/// and many type-specifiers. Thinking about these as fifty independent things
/// overcomplicates the parsing logic.
class typeflag {
  friend typeflag *add_decflag(string, USAGE_FLAG, int);
  friend typeflag *add_decflag(string, typeflag*,  int);
  friend definition *add_primitive(string, size_t sz);
  friend void alias_primitive(string, definition*);
  friend void jdi::cleanup_declarators();
  friend std::unique_ptr<typeflag>::deleter_type;

  typeflag(string name_, USAGE_FLAG usage_, int mask_, int value_):
      name(name_), usage(usage_), mask(mask_), value(value_) {}
  typeflag(string name_, definition *def_):
      name(name_), usage(UF_PRIMITIVE), def(def_) {}
  ~typeflag() = default;

 public:
  /** The name of this flag, as represented in code.
      This is unambiguous, but may be redundant. For instance,
      short and __short may both map to this type. **/
  const string name;
  /// Tells us whether this is a flag, a primitive, or both. Note that
  /// cv-qualifiers, function-specifiers, and storage-class-specifiers are
  /// flags. Type specifiers are not all included here, but those that are
  /// will be primitives and may also be flags. For examples, see USAGE_FLAG.
  /// @see USAGE_FLAG
  const USAGE_FLAG usage;

  /// Returns whether this flag can be applied. This is true if the flag or a
  /// conflicting flag has not already been applied. Some flags can be applied
  /// multiple times (namely, long).
  bool CanApply(unsigned long mask, unsigned long value) const;
  /// Apply this flag to a definition's mask and value.
  void Apply(unsigned long *mask, unsigned long *value,
             const ErrorContext &errc) const;
  bool Matches(unsigned long flags) const {
    return (flags & mask) == value;
  }

  /// If this is a flag, this is its bitmask. Most flags have a single bit,
  /// but the signed/unsigned specifiers share a bit, and the long and short
  /// specifiers share three bits.
  const unsigned long mask = 0;

  /// The value (bits within the mask) of this particular flag. Storing these
  /// separately allows different flags to share a particular bit, and the
  /// mask bits can be used to avoid flag conflicts.
  const unsigned long value = 0;

  /// For primitives, this is the corresponding definition.
  definition *const def = nullptr;
};

extern typeflag *builtin_flag__volatile; ///< Builtin volatile flag
extern typeflag *builtin_flag__static;   ///< Builtin static flag
extern typeflag *builtin_flag__const;    ///< Builtin const flag
extern typeflag *builtin_flag__mutable;  ///< Builtin mutable flag
extern typeflag *builtin_flag__register; ///< Builtin register flag
extern typeflag *builtin_flag__inline;   ///< Builtin inline flag
extern typeflag *builtin_flag__Complex;  ///< Builtin complex flag

extern typeflag *builtin_flag__unsigned; ///< Builtin `unsigned` flag
extern typeflag *builtin_flag__signed;   ///< Builtin `signed` flag
extern typeflag *builtin_flag__short;    ///< Builtin `short` flag
extern typeflag *builtin_flag__long;     ///< Builtin `long` flag
extern typeflag *builtin_flag__long_long;  ///< Matches two `long` flags.

/// Builtin `__restrict` flag (and alternative spellings).
extern typeflag *builtin_flag__restrict;

extern typeflag *builtin_flag__virtual; ///< Builtin `virtual` flag
extern typeflag *builtin_flag__explicit; ///< Builtin `explicit` flag

extern definition *builtin_type__void;   ///< Builtin void type
extern definition *builtin_type__bool;   ///< Builtin char type
extern definition *builtin_type__char;   ///< Builtin char type
extern definition *builtin_type__int;    ///< Builtin int type
extern definition *builtin_type__float;  ///< Builtin float type
extern definition *builtin_type__double; ///< Builtin double type

extern definition *builtin_type__wchar_t; ///< Builtin `wchar_t` type
extern definition *builtin_type__va_list; ///< Builtin `va_list` type

extern typeflag* builtin_typeflag__override; ///< Builtin `override` flag.
extern typeflag* builtin_typeflag__final;    ///< Builtin `final` flag.

typedef map<string,typeflag*> tf_map; ///< A map of declarators by name.
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
extern prim_map builtin_primitives; ///< A map of all builtin primitives, such as int and double, by name.

void clean_up();

}  // namespace jdi

#endif
