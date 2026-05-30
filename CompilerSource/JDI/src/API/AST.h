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

#ifndef _AST__H
#define _AST__H
#define _AST__H__DEBUG // Used in debug_macros.h. Do not rename on a whim.

#include "AST_forward.h"

#include <string>
#include <Storage/arg_key.h>
#include <System/token.h>
#include <Storage/value.h>
#include <API/lexer_interface.h>
#include <API/error_reporting.h>
#include <API/error_context.h>
#include <Storage/definition.h>
#include <API/context.h>

namespace jdip
{
  using namespace jdi;
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
      virtual value eval(const error_context &errc) const;
      /// Coerces this node recursively for type, returning a full_type representing it.
      virtual full_type coerce(const error_context &errc) const;
      
      AST_Node(AST_TYPE type); ///< Default constructor.
      AST_Node(string ct, AST_TYPE type); ///< Constructor, with content string.
      virtual ~AST_Node(); ///< Virtual destructor.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int own_width(); ///< Returns the width in pixels of this node as it will render. This does not include its children.
      virtual int own_height(); ///< Returns the height in pixels of this node as it will render. This does not include its children.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
    };
    /// Child of AST_Node for unary operators.
    struct AST_Node_Unary: AST_Node {
      AST_Node *operand; ///< The stuff we're operating on.
      bool prefix; ///< True if we are a unary prefix, false otherwise.
      
      /// Evaluates this node recursively, returning a value containing its result.
      virtual value eval(const error_context &errc) const;
      /// Coerces this node recursively for type, returning a full_type representing it.
      virtual full_type coerce(const error_context &errc) const;
      
      AST_Node_Unary(AST_TYPE type, AST_Node* r = NULL); ///< Construct arbitrarily, with a type.
      AST_Node_Unary(AST_Node* r, string ct, bool pre, AST_TYPE type); ///< Complete constructor, with child node, operator string, prefix bool, and type.
      AST_Node_Unary(AST_Node* r, string ct, bool pre); ///< Helper constructor, with child node and operator string, and a boolean to choose from prefix or postfix.
      ~AST_Node_Unary(); ///< Default destructor. Frees children recursively.
      bool full(); ///< Returns true if this node is already completely full, meaning it has no room for children.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
    };
    /// Child of AST_Node_Unary specifically for sizeof
    struct AST_Node_sizeof: AST_Node_Unary {
      bool negate; ///< If this is true, then rather than sizeof, we represent its negation, is_empty.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual value eval(const error_context &errc) const; ///< Behaves funny for sizeof; coerces instead, then takes size of result type.
      virtual full_type coerce(const error_context &errc) const; ///< Behaves funny for sizeof; returns unsigned long every time.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      AST_Node_sizeof(AST_Node* param, bool negate);
    };
    struct AST_Node_new: AST_Node {
      full_type alloc_type; ///< The type to be allocated.
      AST_Node *position; ///< A posisition AST node, for placement new.
      AST_Node *bound; ///< An array bound AST node, for new[].
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual value eval(const error_context &errc) const; ///< Throw out a bad value.
      virtual full_type coerce(const error_context &errc) const; ///< Returns the contained type, with another pointer on the ref stack.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      
      AST_Node_new(const full_type &type, AST_Node *position, AST_Node *bound);
      AST_Node_new();
    };
    struct AST_Node_delete: AST_Node_Unary {
      bool array; ///< True if we are delete[] rather than regular delete.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual value eval(const error_context &errc) const; ///< Throw out a bad value.
      virtual full_type coerce(const error_context &errc) const; ///< The type returned by the delete operator is void.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      AST_Node_delete(AST_Node* param, bool array);
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
      
      virtual value eval(const error_context &errc) const; ///< Performs a cast, as it is able.
      virtual full_type coerce(const error_context &errc) const; ///< Returns \c cast_type.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
      virtual int own_height(); ///< Returns the height in pixels of this node as it will render. This does not include its children.
      
      /// Construct without consuming a full_type
      AST_Node_Cast(AST_Node* param, const full_type &ft, cast_modes cmode = CM_CSTYLE);
      /// Construct consuming a full_type
      AST_Node_Cast(AST_Node* param, full_type &ft, cast_modes cmode = CM_CSTYLE);
      AST_Node_Cast(AST_Node* param);
    };
    /// Child of AST_Node for tokens with an attached \c definition.
    struct AST_Node_Definition: AST_Node {
      definition *def; ///< The \c definition of the constant or type this token represents.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual value eval(const error_context &errc) const; ///< Evaluates this node recursively, returning a value containing its result.
      virtual full_type coerce(const error_context &errc) const; ///< Returns the type of the given definition, if it has one.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int own_width(); ///< Returns the height in pixels of this node as it will render. This does not include its children.
      virtual int own_height(); ///< Returns the height in pixels of this node as it will render. This does not include its children.
      
      AST_Node_Definition(definition *def, string content); ///< Construct with a definition
    };
    /// Child of AST_Node for tokens with an attached \c full_type.
    struct AST_Node_Type: AST_Node {
      full_type dec_type; ///< The \c full_type read into this node.
      
      virtual value eval(const error_context &errc) const; ///< Returns zero; output should never be queried.
      virtual full_type coerce(const error_context &errc) const; ///< Returns the type contained, \c dec_type.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int own_width(); ///< Returns the height in pixels of this node as it will render. This does not include its children.
      virtual int own_height(); ///< Returns the height in pixels of this node as it will render. This does not include its children.
      
      AST_Node_Type(full_type &ft); ///< Construct consuming a \c full_type.
    };
    /// Child of AST_Node for binary operators.
    struct AST_Node_Binary: AST_Node {
      AST_Node *left, ///< The left-hand side of the expression.
               *right; ///< The right-hand side of the expression.
      
      /// Evaluates this node recursively, returning a value containing its result.
      virtual value eval(const error_context &errc) const;
      /// Coerces this node recursively for type, returning a full_type representing it.
      virtual full_type coerce(const error_context &errc) const;
      
      AST_Node_Binary(AST_TYPE type = AT_BINARYOP, AST_Node* left=NULL, AST_Node* right=NULL); ///< Construct arbitrarily.
      AST_Node_Binary(AST_Node* left, AST_Node* right, string op, AST_TYPE type = AT_BINARYOP); ///< Full constructor.
      ~AST_Node_Binary(); ///< Default destructor. Frees children recursively.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
    };
    /// Child of AST_Node for the scope resolution operator, ::.
    struct AST_Node_Scope: AST_Node_Binary {
      virtual value eval(const error_context &errc) const; ///< Evaluates this node recursively, returning a value containing its result.
      virtual full_type coerce(const error_context &errc) const; ///< Coerces this node recursively for type, returning a full_type representing it.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      
      AST_Node_Scope(AST_Node* left, AST_Node* right, string op); ///< The one and only know-what-you're-doing constructor.
    };
    /// Child of AST_Node for the ternary operator.
    struct AST_Node_Ternary: AST_Node {
      AST_Node *exp; ///< The expression before the question marks.
      AST_Node *left; ///< The left-hand (true) result.
      AST_Node *right; ///< The right-hand (false) result.
      
      virtual value eval(const error_context &errc) const; ///< Evaluates this node recursively, returning a value containing its result.
      virtual full_type coerce(const error_context &errc) const; ///< Coerces this node recursively for type, returning a full_type representing it.
      
      AST_Node_Ternary(AST_Node *expression = NULL, AST_Node *exp_true = NULL, AST_Node *exp_false = NULL); ///< Default constructor. Sets children to NULL.
      AST_Node_Ternary(AST_Node *expression, AST_Node *exp_true, AST_Node *exp_false, string ct); ///< Complete constructor, with children and a content string.
      ~AST_Node_Ternary(); ///< Default destructor. Frees children recursively.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
    };
    /// Child of AST_Node for array subscripts.
    struct AST_Node_Subscript: AST_Node {
      AST_Node *left;
      AST_Node *index;
      
      /// Evaluates this node recursively, returning a value containing its result.
      virtual value eval(const error_context &errc) const;
      /// Coerces this node recursively for type, returning a full_type representing it.
      virtual full_type coerce(const error_context &errc) const;
      
      AST_Node_Subscript(); ///< Default constructor. Sets children to NULL.
      AST_Node_Subscript(AST_Node *l, AST_Node *ind); ///< Default constructor. Sets children to NULL.
      ~AST_Node_Subscript(); ///< Default destructor. Frees children recursively.
      void setleft(AST_Node* l); ///< Set the left-hand operand (the container).
      void setright(AST_Node* r); ///< Set the right-hand operand (the index expression).
      bool full(); ///< Returns true if this node is already completely full, meaning it has no room for children.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int own_width(); ///< Returns the width in pixels of this node as it will render. This does not include its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
    };
    struct AST_Node_Array: AST_Node {
      vector<AST_Node*> elements; ///< Vector of our array elements.
      
      virtual value eval(const error_context &errc) const; ///< Evaluates this node recursively, returning a value containing its result.
      virtual full_type coerce(const error_context &errc) const; ///< Coerces this node recursively for type, returning a full_type representing it.
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
      
      AST_Node_Array();
      virtual ~AST_Node_Array();
    };
    struct AST_Node_TempInst: AST_Node {
      AST_Node* temp; ///< The expression giving the template to be instantiated.
      vector<AST_Node*> params; ///< Vector of our template parameters.
      
      virtual value eval(const error_context &errc) const; ///< Evaluates this node recursively, returning a value containing its result.
      virtual full_type coerce(const error_context &errc) const; ///< Coerces this node recursively for type, returning a full_type representing it.
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
      
      AST_Node_TempInst(definition_template *def);
      AST_Node_TempInst(AST_Node *tempast, string content);
      virtual ~AST_Node_TempInst();
    };
    struct AST_Node_TempKeyInst: AST_Node {
      definition_template* temp; ///< The template to be instantiated.
      arg_key key; ///< The \c arg_key which will be used to instantiate \c temp, after some remapping.
      
      virtual value eval(const error_context &errc) const; ///< Evaluates this node recursively, returning a value containing its result.
      virtual full_type coerce(const error_context &errc) const; ///< Coerces this node recursively for type, returning a full_type representing it.
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
      
      AST_Node_TempKeyInst(definition_template *temp, const arg_key &key);
      virtual ~AST_Node_TempKeyInst();
    };
    /// Child of AST_Node for function call parameters.
    struct AST_Node_Parameters: AST_Node {
      AST_Node *func;
      vector<AST_Node*> params;
      
      /// Evaluates this node recursively, returning a value containing its result.
      virtual value eval(const error_context &errc) const;
      /// Coerces this node recursively for type, returning a full_type representing it.
      virtual full_type coerce(const error_context &errc) const;
      
      AST_Node_Parameters(); ///< Default constructor. Sets children to NULL.
      ~AST_Node_Parameters(); ///< Default destructor. Frees children recursively.
      void setleft(AST_Node* l); ///< Set the left-hand operand (the function).
      void setright(AST_Node* r); ///< Set the right-hand operand (adds a parameter).
      bool full(); ///< Returns true if this node is already completely full, meaning it has no room for children.
      
      virtual AST_Node* duplicate() const; ///< Duplicate this AST node, returning a new pointer to a copy.
      virtual void remap(const remap_set &n); ///< If you hold any references to definitions in this map, update them.
      virtual void operate(ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
      virtual void operate(ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
    };
}

namespace jdi {
  /** @class jdi::AST
      General-purpose class designed to take in a series of tokens and generate an abstract syntax tree.
      The generated AST can then be evaluated for a \c value or coerced for a resultant type as a \c definition.
  **/
  class AST
  {
  protected:
    friend struct jdip::ASTOperator;
    friend struct jdip::ConstASTOperator;
    friend class jdip::AST_Builder;
    
    void report_error(const jdip::token_t &token, string err);
    void report_errorf(const jdip::token_t &token, string err);
    void report_warning(const jdip::token_t &token, string err);
    
    jdip::AST_Node *root; ///< The first node in our AST--The last operation that will be performed.
    
    // State flags
    bool tt_greater_is_op; ///< True if the greater-than symbol is to be interpreted as an operator.
    
    /* * Handle a binary operator. Errors if the operator represented by the token cannot
        be used as a binary operator.
        @param  precedence  The precedence which will be given to this operator.
        @param  origin      The token which sparked this call; used to copy over error info.
        @return Returns the node of the operator of lowest precedence (ie, the root node),
                be it the node of the newly allocated node or some other operator. If an
                error occurs, NULL is returned.
    ** /
    int parse_binary_op(int precedence, jdip::token_t &origin);
    int parse_unary_prefix(int precedence, jdip::token_t &origin);
    int parse_unary_postfix(int precedence, jdip::token_t &origin);
    int parse_ternary(int precedence, jdip::token_t &origin);*/
    
    public:
    
    #ifdef DEBUG_MODE
    string expression; ///< The string representation of the expression fed in, for debug purposes.
    #endif
    
    /// Filter this AST through a definition remap_set, to update references to old definitions.
    void remap(const remap_set &n);
    
    /// Evaluate the current AST, returning its \c value.
    value eval(const error_context &errc) const;
    
    /// Coerce the current AST for the type of its result.
    full_type coerce(const error_context &errc) const;
    
    /// Clear the AST out, effectively creating a new instance of this class
    void clear();
    
    /// Check if this AST is empty.
    bool empty();
    
    /// Return a new copy of this entire AST.
    AST* duplicate() const;
    
    /// Render the AST as a string: This is a relatively costly operation.
    string toString() const; ///< Renders this node and its children as a string, recursively.
    void operate(jdip::ASTOperator *aop, void *p); ///< Perform some externally defined recursive operation on this AST.
    void operate(jdip::ConstASTOperator *caop, void *p) const; ///< Perform some externally defined constant recursive operation on this AST.
    
    /// Render the AST to an SVG file.
    void writeSVG(const char* filename);
    
    /// Use this AST for template parameters
    inline void set_use_for_templates(bool use) { tt_greater_is_op = !use; }
    
    /// Swap roots with another AST, for efficient transfer
    void swap(AST &ast);
    
    /// Default constructor. Zeroes some stuff.
    AST();
    /// Construct with a single node
    /// @param def   The definition from which to construct a node.
    AST(definition* def);
    /// Construct with a single node
    /// @param val   The value from which to construct a node.
    AST(value v);
    
    
    // Non-constructor AST factory methods
    
    /// Construct with a single template instantiation node (by key)
    /// @param temp  The template to be instantiated.
    /// @param key   The arg_key with which to instantiate the template, after some remapping.
    static AST* create_from_instantiation(definition_template* temp, const arg_key& key);
    
    /// Construct with a single template instantiation node (by key)
    /// @param scope     The scope from which the access will occur, after some remapping.
    /// @param id        The identifier which is to be accessed in the scope.
    /// @param scope_op  The scope resolution operator, probably "::", but "." and "->" are also permitted.
    static AST* create_from_access(definition_scope* scope, string id, string scope_op);
    
    /// Default destructor. Deletes the AST.
    ~AST();
    
    protected:
      /// Construct with a root node; this will invariably be called internally.
      AST(jdip::AST_Node* root);
    
    private:
      /// Copy constructor; highly expensive. Not implemented. Use duplicate(), sparingly.
      AST(const AST& ast);
  };
}

namespace jdip
{
  class AST_Builder
  {
  protected:
    context_parser *cparse; ///< A context parser to poll for tokens and handle error checking
    inline context_parser *get_context() const { return cparse; } /// Get this AST builder's context
    
    definition_scope *search_scope; ///< The scope from which token values will be harvested.
    
    /// Private method to fetch the next token from the lexer, with or without a scope.
    token_t get_next_token();
    
    /** Handle a whole expression, stopping at the first unexpected token or when an
        operator is encountered which has a precendence lower than the one specified.
        I.E., passing a precedence of 0 will handle all operators.
        @param  token       The first token to be handled. Will be set to the first unhandled token. [in-out]
        @param  precedence  The lowest precedence of any operators to be handled.
    **/
    AST_Node* parse_expression(AST* ast, jdip::token_t &token, int precedence = 0);
    /** Handle anything you'd expect to see after a literal is given.
        
        This includes binary and ternary operators (to which the literal or enclosing tree
        will be used as the left-hand side, and the right will be read fresh), or a unary
        postfix (which will apply to the latest-read literal or expression).
        
        @param  token  The first token to be handled. Will be set to the first unhandled token. [in-out]
        @param  left_node  The latest-read literal or expression.
        @param  prec_min   The minimum precedence of operators to handle.
        @return Returns the node of the operator of lowest precedence (ie, the root
                node), or NULL if an error occurs.
    **/
    AST_Node* parse_binary_or_unary_post(AST* ast, jdip::token_t &token, AST_Node *left_node, int prec_min);
    /** Handle anything you'd expect to see at the start of an expression, being a
        unary prefix operator or a literal.
        @param  token  The first token to be handled. Will be set to the first unhandled token. [in-out]
        @return Returns the node of the operator of lowest precedence (ie, the root
                node), or NULL if an error occurs.
    **/
    AST_Node* parse_unary_pre_or_literal(AST* ast, jdip::token_t& token);
    
    public:
    /** Parse in an expression, building an AST.
        @param prec  The lower-bound precedence; default is 0 (precedence::all)
        @return  This function will return 0 if no error has occurred, or nonzero otherwise.
    **/
    int parse_expression(AST* ast, int prec = 0);
    
    /** Parse in an expression, building an AST, with scope information, starting with the given token.
        @param token  A buffer for the first unhandled token. [out]
        @param scope  The scope from which values of definitions will be read. [in]
        @param prec   The lower-bound precedence.
        @return  This function will return 0 if no error has occurred, or nonzero otherwise.
    **/
    int parse_expression(AST* ast, jdip::token_t &token, definition_scope *scope, int prec);
    
    /// Construct with a context, lexer, and error reporter.
    /// @param ctp   The context parser that will be used to parse out typenames, etc.
    /// @param lex   A lexer which can be polled for tokens.
    AST_Builder(context_parser *ctp);
  };
}

#include <System/symbols.h>

#endif
