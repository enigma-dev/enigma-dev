/**
 * @file  AST.h
 * @brief Header declaring a class that handles expression evaluation and type
 *        coercion via an AST.
 *
 * This file fuels "private" parser methods, but is designed to be a general-purpose
 * utility which is safe for use by typical JDI users as well. Simply pass it tokens
 * you create with the methods in \file user_tokens.h.
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
 *
**/

#ifndef JDI_API_AST_h
#define JDI_API_AST_h
#define JDI_API_AST_h_debug // Used in debug_macros.h. Do not rename on a whim.

#include "AST_forward.h"

#include <Storage/arg_key.h>
#include <System/token.h>
#include <System/lex_cpp.h>
#include <Storage/value.h>
#include <API/error_reporting.h>
#include <Storage/definition.h>

#include <string>
#include <filesystem>

namespace jdi {

/// Structure containing info for use when rendering SVGs.
/// This class can export SVG files. Some info needs tossed around to do so.
struct SVGrenderInfo;

/** Enum declaring basic node types for this AST. These include the three types of
    operators and the four types of data.
**/
enum AST_TYPE {
  AT_UNARY_PREFIX, ///< This node is some kind of unary prefix operator, such as *, &, ~, or -.
  AT_UNARY_POSTFIX, ///< This node is some kind of unary postfix operator, such as ++, [], or ().
  AT_BINARYOP,    ///< This node is a binary operator, like /, *, -, or +.
  AT_TERNARYOP,   ///< This node is a ternary operator. Note that ?: is the only one currently supported.
  AT_DECLITERAL,  ///< This node is a decimal literal, such as 1337.
  AT_HEXLITERAL,  ///< This node is a hexadecimal literal, such as 0x539
  AT_OCTLITERAL,  ///< This node is an octal literal, such as 2471.
  AT_BINLITERAL,  ///< This node is an binary literal, such as 0b1001.
  AT_BOOLLITERAL, ///< This node is an boolean literal, true or false.
  AT_CHRLITERAL,  ///< This node is a character literal, such as 'a'.
  AT_STRLITERAL,  ///< This node is a string literal, such as "abc".
  AT_IDENTIFIER,  ///< This node is an identifier that could not be looked up.
  AT_TEMPID,      ///< This node is an identifier that is specified to be a template.
  AT_DEFINITION,  ///< This node is a definition; an identifier that has been looked up.
  AT_TYPE,        ///< This node is a full type.
  AT_ARRAY,       ///< This node is an array of nodes.
  AT_SUBSCRIPT,   ///< This node is an array subscript, [expression].
  AT_SCOPE,       ///< This node is a scope access, such as ::
  AT_SIZEOF,      ///< This node is a sizeof() or empty() expression.
  AT_CAST,        ///< This node is a typecast expression.
  AT_PARAMLIST,   ///< This node is a list of parameters.
  AT_NEW,         ///< This node is a new, new[], new(), or new()[] operator.
  AT_DELETE,      ///< This node is a delete or delete[] operator.
  AT_INSTANTIATE, ///< This node is a template instantiation.
  AT_INSTBYKEY,   ///< This node is a template instantiation with an arg_key.
  AT_USERBEGIN    ///< This is the first user-defined token index.
};

/** Private storage mechanism designed to hold token information and any linkages.
    In general, a node has no linkages, and so we use AST_Node as the base class for
    nodes which do have links.
**/
struct AST_Node {
  AST_TYPE type; ///< The type of this node, as one of the AST_TYPE constants.
  std::string content; ///< The literal, as a string, such as "1234", or the symbol representing the operator, as a symbol for lookup, such as "+=".

  #ifndef NO_ERROR_REPORTING
    std::string filename; ///< The name of the file in which the token was created, for error reporting.
    int linenum; ///< The line on which this token appeared in the file.
    #ifndef NO_ERROR_POSITION
      int pos; ///< The position at which this token appeared in the line.
    #endif
  #endif

  /// Evaluates this node recursively, returning a value containing its result.
  virtual value eval(const ErrorContext &errc) const;
  /// Coerces this node recursively for type, returning a full_type representing it.
  virtual full_type coerce(const ErrorContext &errc) const;

  AST_Node(AST_TYPE type);
  AST_Node(string_view ct, AST_TYPE type);
  virtual ~AST_Node() = default;

  /// Allocates and returns a new type-accurate copy of this node.
  virtual unique_ptr<AST_Node> duplicate() const;
  /// If you hold any references to definitions in this map, update them.
  virtual void remap(const remap_set &n, ErrorContext errc);
  /// Perform some externally defined recursive operation on this AST.
  virtual void operate(ASTOperator *aop, void *p);
  /// Perform some externally defined constant recursive operation on this AST.
  virtual void operate(ConstASTOperator *caop, void *p) const;
  /// Renders this node and its children as a string, recursively.
  virtual string toString() const;
  /// Renders this node and its children as an SVG.
  virtual void toSVG(int x, int y, SVGrenderInfo* svg);
  /// Returns the width in pixels of this node as it will render.
  /// This does not include its children.
  virtual int own_width();
  /// Returns the height in pixels of this node as it will render.
  /// This does not include its children.
  virtual int own_height();
  /// Returns the width which will be used to render this node and all of
  /// its children.
  virtual int width();
  /// Returns the height which will be used to render this node and all of
  /// its children.
  virtual int height();
};

/// Child of AST_Node for unary operators.
struct AST_Node_Unary: AST_Node {
  unique_ptr<AST_Node> operand; ///< The stuff we're operating on.
  bool prefix; ///< True if we are a unary prefix, false otherwise.

  /// Evaluates this node recursively, returning a value containing its result.
  value eval(const ErrorContext &errc) const override;
  /// Coerces this node recursively for type, returning a full_type representing it.
  full_type coerce(const ErrorContext &errc) const override;

  /// Returns true if this node is already completely full, meaning it has
  /// no room for children.
  bool full() const;

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;
  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  int width() override;
  int height() override;

  /// Construct arbitrarily, with a type.
  AST_Node_Unary(AST_TYPE type, unique_ptr<AST_Node> r = nullptr);
  /// Complete constructor, with child node, operator string, prefix bool,
  /// and type.
  AST_Node_Unary(unique_ptr<AST_Node> r, string_view ct, bool pre,
                 AST_TYPE type);
  /// Helper constructor, with child node and operator string, and a boolean
  /// to choose from prefix or postfix.
  AST_Node_Unary(unique_ptr<AST_Node> r, string_view ct, bool pre);
  /// Default destructor. Frees children recursively.
  ~AST_Node_Unary() override = default;
};

/// Child of AST_Node_Unary specifically for sizeof
struct AST_Node_sizeof: AST_Node_Unary {
  /// If this is true, then rather than sizeof, we represent its negation,
  /// is_empty.
  bool negate;

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;

  /// Behaves funny for sizeof; coerces instead, then takes size of result
  /// type.
  value eval(const ErrorContext &errc) const override;
  /// Behaves funny for sizeof; returns unsigned long every time.
  /// For the negation (is_empty), returns bool.
  full_type coerce(const ErrorContext &errc) const override;

  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  AST_Node_sizeof(unique_ptr<AST_Node> param, bool negate);
  ~AST_Node_sizeof() override = default;
};

struct AST_Node_new: AST_Node {
  /// The type to be allocated.
  full_type alloc_type;
  /// A posisition AST node, for placement new.
  unique_ptr<AST_Node> position;
  /// An array bound AST node, for new[].
  unique_ptr<AST_Node> bound;

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;
  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;

  /// Returns a bad value.
  value eval(const ErrorContext &errc) const override;
  /// Returns the contained type, but with another pointer on the ref stack.
  full_type coerce(const ErrorContext &errc) const override;

  AST_Node_new();
  AST_Node_new(const full_type &type, unique_ptr<AST_Node> position,
               unique_ptr<AST_Node> bound);
  ~AST_Node_new() override = default;
};

struct AST_Node_delete: AST_Node_Unary {
  bool array; ///< True if we are delete[] rather than regular delete.

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;
  string toString() const override;

  value eval(const ErrorContext &errc) const override; ///< Throw out a bad value.
  full_type coerce(const ErrorContext &errc) const override; ///< The type returned by the delete operator is void.
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  AST_Node_delete(unique_ptr<AST_Node> param, bool array);
  ~AST_Node_delete() override = default;
};

/// Child of AST_Node_Unary specifically for sizeof
struct AST_Node_Cast: AST_Node_Unary {
  /// Enumerates C++ casting modes, which depend on the keyword used for the cast.
  enum cast_modes {
    CM_CSTYLE,     ///< A C-style cast.
    CM_CONST,      ///< The `const_cast' operator.
    CM_STATIC,     ///< The `static_cast' operator.
    CM_DYNAMIC,    ///< The `dynamic_cast' operator.
    CM_REINTERPRET ///< The `reinterpret_cast' operator.
  };

  full_type cast_type; ///< The type this cast represents.
  cast_modes cast_mode; ///< The mode of this cast; C-style, const, static, dynamic, reinterpret.

  value eval(const ErrorContext &errc) const override; ///< Performs a cast, as it is able.
  full_type coerce(const ErrorContext &errc) const override; ///< Returns \c cast_type.

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;
  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  int height() override;
  int own_height() override;

  /// Construct without consuming a full_type
  AST_Node_Cast(unique_ptr<AST_Node> param, const full_type &ft,
                cast_modes cmode = CM_CSTYLE);
  /// Construct consuming a full_type
  AST_Node_Cast(unique_ptr<AST_Node> param, full_type &ft,
                cast_modes cmode = CM_CSTYLE);
  AST_Node_Cast(unique_ptr<AST_Node> param);
  ~AST_Node_Cast() override = default;
};

/// Child of AST_Node for tokens with an attached \c definition.
struct AST_Node_Definition: AST_Node {
  /// The \c definition of the constant or type this token represents.
  definition *def;

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  value eval(const ErrorContext &errc) const override;
  full_type coerce(const ErrorContext &errc) const override; ///< Returns the type of the given definition, if it has one.
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;
  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  int own_width() override; ///< Returns the height in pixels of this node as it will render. This does not include its children.
  int own_height() override; ///< Returns the height in pixels of this node as it will render. This does not include its children.

  AST_Node_Definition(definition *def, string_view content);
  ~AST_Node_Definition() override = default;
};

/// Child of AST_Node for tokens with an attached \c full_type.
struct AST_Node_Type: AST_Node {
  full_type dec_type; ///< The \c full_type read into this node.

  /// Returns zero; output should never be queried.
  value eval(const ErrorContext &errc) const override;
  /// Returns the type contained, \c dec_type.
  full_type coerce(const ErrorContext &errc) const override;

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;
  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  int own_width() override;
  int own_height() override;

  AST_Node_Type(full_type &ft); ///< Construct consuming a \c full_type.
  ~AST_Node_Type() override = default;
};

/// Child of AST_Node for binary operators.
struct AST_Node_Binary: AST_Node {
  unique_ptr<AST_Node> left; ///< The left-hand side of the expression.
  unique_ptr<AST_Node> right; ///< The right-hand side of the expression.

  /// Evaluates this node recursively, returning a \c value.
  virtual value eval(const ErrorContext &errc) const;
  /// Coerces this node recursively for type, returning a full_type.
  virtual full_type coerce(const ErrorContext &errc) const;

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;
  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  int width() override;
  int height() override;

  AST_Node_Binary(AST_TYPE type = AT_BINARYOP);
  AST_Node_Binary(AST_TYPE type,
                  unique_ptr<AST_Node> left, unique_ptr<AST_Node> right);
  AST_Node_Binary(unique_ptr<AST_Node> left, unique_ptr<AST_Node> right,
                  string_view op, AST_TYPE type = AT_BINARYOP);
  ~AST_Node_Binary() override = default;
};

/// Child of AST_Node for the scope resolution operator, ::.
struct AST_Node_Scope: AST_Node_Binary {
  value eval(const ErrorContext &errc) const override;
  full_type coerce(const ErrorContext &errc) const override;

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;
  string toString() const override;

  AST_Node_Scope(unique_ptr<AST_Node> left, unique_ptr<AST_Node> right,
                 string_view op);
  ~AST_Node_Scope() override = default;
};

/// Child of AST_Node for the ternary operator.
struct AST_Node_Ternary: AST_Node {
  unique_ptr<AST_Node> exp; ///< The expression before the question marks.
  unique_ptr<AST_Node> left; ///< The left-hand (true) result.
  unique_ptr<AST_Node> right; ///< The right-hand (false) result.

  value eval(const ErrorContext &errc) const override;
  full_type coerce(const ErrorContext &errc) const override;

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;
  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  int width() override;
  int height() override;

  AST_Node_Ternary();
  AST_Node_Ternary(unique_ptr<AST_Node> expression,
                   unique_ptr<AST_Node> exp_true,
                   unique_ptr<AST_Node> exp_false, string_view ct);
  ~AST_Node_Ternary() override = default;
};

/// Child of AST_Node for array subscripts.
struct AST_Node_Subscript: AST_Node {
  unique_ptr<AST_Node> left;
  unique_ptr<AST_Node> index;

  /// Evaluates this node recursively, returning a value containing its result.
  value eval(const ErrorContext &errc) const override;
  /// Coerces this node recursively for type, returning a full_type representing it.
  full_type coerce(const ErrorContext &errc) const override;

  bool full(); ///< Returns true if this node is already completely full, meaning it has no room for children.

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;

  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  int width() override;
  int own_width() override;
  int height() override;

  AST_Node_Subscript();
  AST_Node_Subscript(unique_ptr<AST_Node> left_node,
                     unique_ptr<AST_Node> index_node);
  ~AST_Node_Subscript() override = default;
};

struct AST_Node_Array: AST_Node {
  vector<unique_ptr<AST_Node>> elements; ///< Vector of our array elements.

  value eval(const ErrorContext &errc) const override;
  full_type coerce(const ErrorContext &errc) const override;
  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
  void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.

  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  int width() override;
  int height() override;

  AST_Node_Array();
  ~AST_Node_Array() override = default;
};

struct AST_Node_TempInst: AST_Node {
  /// The expression giving the template to be instantiated.
  /// Should be an identifier or scope accessor.
  unique_ptr<AST_Node> temp;
  /// Vector of our template parameters.
  vector<unique_ptr<AST_Node>> params;

  value eval(const ErrorContext &errc) const override;
  full_type coerce(const ErrorContext &errc) const override;
  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;

  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  int width() override;
  int height() override;

  AST_Node_TempInst(definition_template *def);
  AST_Node_TempInst(unique_ptr<AST_Node> tempast, string_view content);
  ~AST_Node_TempInst() override = default;
};

struct AST_Node_TempKeyInst: AST_Node {
  definition_template* temp; ///< The template to be instantiated.
  arg_key key; ///< The \c arg_key which will be used to instantiate \c temp, after some remapping.

  value eval(const ErrorContext &errc) const override;
  full_type coerce(const ErrorContext &errc) const override;
  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;

  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  virtual int width() override;
  virtual int height() override;

  AST_Node_TempKeyInst(definition_template *temp, const arg_key &key);
  ~AST_Node_TempKeyInst() override = default;
};

/// Child of AST_Node for function call parameters.
struct AST_Node_Parameters: AST_Node {
  unique_ptr<AST_Node> func;
  vector<unique_ptr<AST_Node>> params;

  /// Evaluates this node recursively, returning a value containing its result.
  virtual value eval(const ErrorContext &errc) const;
  /// Coerces this node recursively for type, returning a full_type representing it.
  virtual full_type coerce(const ErrorContext &errc) const;

  void setleft(unique_ptr<AST_Node> l); ///< Set the left-hand operand (the function).
  void setright(unique_ptr<AST_Node> r); ///< Set the right-hand operand (adds a parameter).
  bool full(); ///< Returns true if this node is already completely full, meaning it has no room for children.

  unique_ptr<AST_Node> duplicate() const override;
  void remap(const remap_set &n, ErrorContext errc) override;
  void operate(ASTOperator *aop, void *p) override;
  void operate(ConstASTOperator *caop, void *p) const override;

  string toString() const override;
  void toSVG(int x, int y, SVGrenderInfo* svg) override;
  int width() override;
  int height() override;

  AST_Node_Parameters();
  ~AST_Node_Parameters() override = default;
};

/** @class jdi::AST
    General-purpose class designed to take in a series of tokens and generate an abstract syntax tree.
    The generated AST can then be evaluated for a \c value or coerced for a resultant type as a \c definition.
**/
class AST {
 protected:
  friend struct jdi::ASTOperator;
  friend struct jdi::ConstASTOperator;
  friend class jdi::AST_Builder;

  /// The first node in our AST--The last operation that will be performed.
  unique_ptr<AST_Node> root;

  // State flags =============================================================

  /// True if the greater-than symbol is to be interpreted as an operator.
  bool tt_greater_is_op = false;
  /// True if undeclared identifiers are expected and should be treated as 0.
  bool treat_identifiers_as_zero = false;

  /* * Handle a binary operator. Errors if the operator represented by the token cannot
      be used as a binary operator.
      @param  precedence  The precedence which will be given to this operator.
      @param  origin      The token which sparked this call; used to copy over error info.
      @return Returns the node of the operator of lowest precedence (ie, the root node),
              be it the node of the newly allocated node or some other operator. If an
              error occurs, nullptr is returned.
  ** /
  int parse_binary_op(int precedence, jdi::token_t &origin);
  int parse_unary_prefix(int precedence, jdi::token_t &origin);
  int parse_unary_postfix(int precedence, jdi::token_t &origin);
  int parse_ternary(int precedence, jdi::token_t &origin);*/

 public:
  #ifdef DEBUG_MODE
  string expression; ///< The string representation of the expression fed in, for debug purposes.
  #endif

  /// Filter this AST through a definition remap_set, to update references to old definitions.
  void remap(const remap_set &n, ErrorContext errc);

  /// Evaluate the current AST, returning its \c value.
  value eval(const ErrorContext &errc) const;

  /// Coerce the current AST for the type of its result.
  full_type coerce(const ErrorContext &errc) const;

  /// Clear the AST out, effectively creating a new instance of this class
  void clear();

  /// Check if this AST is empty.
  bool empty();

  /// Return a new copy of this entire AST.
  unique_ptr<AST> duplicate() const;

  /// Render the AST as a string: This is a relatively costly operation.
  string toString() const; ///< Renders this node and its children as a string, recursively.
  void operate(jdi::ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
  void operate(jdi::ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.

  /// Render the AST to an SVG file.
  void writeSVG(std::filesystem::path filename);

  /// Use this AST for template parameters
  inline void set_use_for_templates(bool use) { tt_greater_is_op = !use; }

  /// Swap roots with another AST, for efficient transfer
  void swap(AST &ast);

  /// Default constructor. Zeroes some stuff.
  AST();
  /// Construct with a single node: a definition.
  /// @param def   The definition from which to construct a node.
  AST(definition* def);
  /// Construct with a single node: a value.
  /// @param val   The value from which to construct a node.
  AST(value v);
  /// Construct with a single node: a value. Disambiguates pointer/value ctors.
  /// @param val   The integer value from which to construct a node.
  AST(long v);


  // Non-constructor AST factory methods

  /// Constructs an AST with a single template instantiation node (by key).
  /// @param temp  The template to be instantiated.
  /// @param key   The arg_key with which to instantiate the template,
  ///              after some remapping.
  static unique_ptr<AST> create_from_instantiation(definition_template* temp,
                                                   const arg_key& key);

  /// Constructs an AST with a single scope access node node.
  /// @param scope     The scope from which the access will occur, after some
  ///                  remapping.
  /// @param id        The identifier which is to be accessed in the scope.
  /// @param scope_op  The scope resolution operator, probably "::", but "."
  ///                  and "->" are also permitted.
  static unique_ptr<AST> create_from_access(definition_scope* scope,
                                            string_view id,
                                            string_view scope_op);

  /// Move constructor.
  AST(AST &&ast) = default;

  /// Copy constructor to stack a duplicate()—means one fewer alloc.
  AST(const AST &ast, bool):
      AST(ast.root ? ast.root->duplicate() : nullptr) {}

  /// Construct with a root node; this should only be called internally.
  AST(unique_ptr<AST_Node> root);

  /// Default destructor. Deletes the AST.
  ~AST() = default;

  /// Copy constructor would be highly expensive. Not implemented.
  /// Use duplicate(), sparingly.
  AST(const AST &ast) = delete;
};

class AST_Builder {
 protected:
  lexer *lex;
  ErrorHandler *herr;

  // TODO: Remove the legacy cparse instance. Move the behavioral booleans and
  // the expression string out of AST and into AST_Builder.

  context_parser *cparse; ///< Legacy. Handles reading types. Type reading logic is really fucking complicated. It should probably always involve building an AST, instead of trying to rewind time like all the other compilers.
  /// Get this AST builder's context
  inline context_parser *get_context() const { return cparse; }

  definition_scope *search_scope; ///< The scope from which token values will be harvested.

  /// Private method to fetch the next token from the lexer, with or without a scope.
  token_t get_next_token();

  /** Handle a whole expression, stopping at the first unexpected token or when an
      operator is encountered which has a precendence lower than the one specified.
      I.E., passing a precedence of 0 will handle all operators.
      @param  ast         The AST currently being built. See TODO above. [in-out]
      @param  token       The first token to be handled. Will be set to the first unhandled token. [in-out]
      @param  precedence  The lowest precedence of any operators to be handled.
  **/
  unique_ptr<AST_Node> parse_expression(AST *ast, jdi::token_t &token,
                                        int precedence = 0);
  /** Handle anything you'd expect to see after a literal is given.

      This includes binary and ternary operators (to which the literal or enclosing tree
      will be used as the left-hand side, and the right will be read fresh), or a unary
      postfix (which will apply to the latest-read literal or expression).

      @param  ast        The AST currently being built. [in-out]
      @param  token      The first token to be handled. Will be set to the first
                         unhandled token. [in-out]
      @param  left_node  The latest-read literal or expression.
      @param  prec_min   The minimum precedence of operators to handle.
      @return Returns the node of the operator of lowest precedence (ie, the
              root node), or nullptr if an error occurs.
  **/
  unique_ptr<AST_Node> parse_binary_or_unary_post(
      AST *ast, jdi::token_t &token, unique_ptr<AST_Node> left_node,
      int prec_min);
  /** Handle anything you'd expect to see at the start of an expression, being a
      unary prefix operator or a literal.
      @param  ast    The AST currently being built. [in-out]
      @param  token  The first token to be handled. Will be set to the first unhandled token. [in-out]
      @return Returns the node of the operator of lowest precedence (ie, the root
              node), or nullptr if an error occurs.
  **/
  unique_ptr<AST_Node> parse_unary_pre_or_literal(AST *ast,
                                                  jdi::token_t &token);

 public:
  /** Parse in an expression, building an AST, with scope information, starting with the given token.
      @param ast    The AST currently being built. [in]
      @param token  A buffer for the first unhandled token. [out]
      @param scope  The scope from which values of definitions will be read. [in]
      @param prec   The lower-bound precedence.
      @return  Returns 0 if no error has occurred, or nonzero otherwise.        TODO: Return a Status or something.
  **/
  int parse_expression(AST *ast, jdi::token_t &token, definition_scope *scope,
                       int prec);

  /// Construct with a context, lexer, and error reporter.
  /// @param ctp   The context parser that will be used to parse out typenames, etc.
  /// @param lex   A lexer which can be polled for tokens.
  AST_Builder(context_parser *ctp);
};

/// Read and parse an expression from the given lexer. The expression cannot
/// reference anything apart from builtins. The first unconsumed token will
/// be written to the given token. If the given token is null, an error will
/// be logged for unconsumed tokens, instead.
AST parse_expression(lexer *lex, ErrorHandler *herr, token_t *token);

}  // namespace jdi

#include <System/symbols.h>

#endif
