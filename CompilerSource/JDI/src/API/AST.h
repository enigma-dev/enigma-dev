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
 * 
**/

#ifndef _AST__H
#define _AST__H
#define _AST__H__DEBUG // Used in debug_macros.h. Do not rename on a whim.

namespace jdi { class AST; }

#include <string>
#include <System/token.h>
#include <Storage/value.h>
#include <API/lexer_interface.h>
#include <API/error_reporting.h>

namespace jdi {
  /** @class jdi::AST
      General-purpose class designed to take in a series of tokens and generate an abstract syntax tree.
      The generated AST can then be evaluated for a \c value or coerced for a resultant type as a \c definition.
  **/
  class AST
  {
  protected:
    
    /** Enum declaring basic node types for this AST. These include the three types of
        operators and the four types of data.
    **/
    enum AST_TYPE {
      AT_UNARY_PREFIX, ///< This node is some kind of unary prefix operator, such as *, &, ~, or -.
      AT_UNARY_POSTFIX, ///< This node is some kind of unary postfix operator, such as ++, [], or ().
      AT_BINARYOP, ///< This node is a binary operator, like /, *, -, or +.
      AT_TERNARYOP, ///< This node is a ternary operator. Note that ?: is the only one currently supported.
      AT_DECLITERAL, ///< This node is a decimal literal, such as 1337.
      AT_HEXLITERAL, ///< This node is a hexadecimal literal, such as 0x539
      AT_OCTLITERAL, ///< This node is an octal literal, such as 2471.
      AT_CHRLITERAL, ///< This node is a character literal, such as 'a'.
      AT_IDENTIFIER, ///< This node is an identifier that could not be looked up.
      AT_DEFINITION, ///< This node is a definition; an identifier that has been looked up.
      AT_TYPE,       ///< This node is a full type.
      AT_ARRAY,      ///< This node is an array of nodes.
      AT_FUNCCALL    ///< This node is a function call. FIXME: This value is probably never used.
    };
    
    /// Structure containing info for use when rendering SVGs.
    /// This class can export SVG files. Some info needs tossed around to do so.
    struct SVGrenderInfo;
    
    /** Private storage mechanism designed to hold token information and any linkages.
        In general, a node has no linkages, and so we use AST_Node as the base class for
        nodes which do have links.
    **/
    struct AST_Node {
      AST_TYPE type; ///< The type of this node, as one of the AST_TYPE constants.
      AST_Node *parent; ///< The parent of this node, or NULL iff it is the root.
      std::string content; ///< The literal, as a string, such as "1234", or the symbol representing the operator, as a symbol for lookup, such as "+=".
      int precedence; ///< The priority with which this node is executed; 0 = highest precedence.
      
      #ifndef NO_ERROR_REPORTING
        std::string filename; ///< The name of the file in which the token was created, for error reporting.
        int linenum; ///< The line on which this token appeared in the file.
        #ifndef NO_ERROR_POSITION
          int pos; ///< The position at which this token appeared in the line.
        #endif
      #endif
      
      /// Evaluates this node recursively, returning a value containing its result.
      virtual value eval() const;
      /// Coerces this node recursively for type, returning a full_type representing it.
      virtual full_type coerce() const;
      
      AST_Node(); ///< Default constructor.
      AST_Node(string ct); ///< Constructor, with content string.
      virtual ~AST_Node(); ///< Virtual destructor.
      
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
      virtual value eval() const;
      /// Coerces this node recursively for type, returning a full_type representing it.
      virtual full_type coerce() const;
      
      AST_Node_Unary(AST_Node* r = NULL); ///< Default constructor. Sets children to NULL.
      AST_Node_Unary(AST_Node* r, string ct, bool pre); ///< Complete constructor, with child node and operator string.
      ~AST_Node_Unary(); ///< Default destructor. Frees children recursively.
      bool full(); ///< Returns true if this node is already completely full, meaning it has no room for children.
      
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
    };
    /// Child of AST_Node_Unary specifically for sizeof
    struct AST_Node_sizeof: AST_Node_Unary {
      bool negate; ///< If this is true, then rather than sizeof, we represent its negation, is_empty.
      
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual value eval() const; ///< Behaves funny for sizeof; coerces instead, then takes size of result type.
      virtual full_type coerce() const; ///< Behaves funny for sizeof; returns unsigned long every time.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      AST_Node_sizeof(AST_Node* param, bool negate);
    };
    struct AST_Node_new: AST_Node {
      full_type type; ///< The type to be allocated.
      AST_Node *position; ///< A posisition AST node, for placement new.
      AST_Node *bound; ///< An array bound AST node, for new[].
      
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual value eval() const; ///< Behaves funny for sizeof; coerces instead, then takes size of result type.
      virtual full_type coerce() const; ///< Behaves funny for sizeof; returns unsigned long every time.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      AST_Node_new();
    };
    struct AST_Node_delete: AST_Node_Unary {
      bool array; ///< True if we are delete[] rather than regular delete.
      
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual value eval() const; ///< Behaves funny for sizeof; coerces instead, then takes size of result type.
      virtual full_type coerce() const; ///< Behaves funny for sizeof; returns unsigned long every time.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      AST_Node_delete(AST_Node* param, bool array);
    };
    /// Child of AST_Node_Unary specifically for sizeof
    struct AST_Node_Cast: AST_Node_Unary {
      full_type cast_type; ///< The type this cast represents.
      virtual value eval() const; ///< Performs a cast, as it is able.
      virtual full_type coerce() const; ///< Returns \c cast_type.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
      virtual int own_height(); ///< Returns the height in pixels of this node as it will render. This does not include its children.
      AST_Node_Cast(AST_Node* param, const full_type &ft);
      AST_Node_Cast(AST_Node* param, full_type &ft);
      AST_Node_Cast(AST_Node* param);
    };
    /// Child of AST_Node for tokens with an attached \c definition.
    struct AST_Node_Definition: AST_Node {
      definition *def; ///< The \c definition of the constant or type this token represents.
      virtual value eval() const; ///< Evaluates this node recursively, returning a value containing its result.
      virtual full_type coerce() const; ///< Returns the type of the given definition, if it has one.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      AST_Node_Definition(definition *def); ///< Construct with a definition
    };
    /// Child of AST_Node for tokens with an attached \c full_type.
    struct AST_Node_Type: AST_Node {
      full_type dec_type; ///< The \c full_type read into this node.
      virtual value eval() const; ///< Returns zero; output should never be queried.
      virtual full_type coerce() const; ///< Returns the type contained, \c dec_type.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      AST_Node_Type(full_type &ft); ///< Construct consuming a \c full_type.
    };
    /// Child of AST_Node for binary operators.
    struct AST_Node_Binary: AST_Node {
      AST_Node *left, ///< The left-hand side of the expression.
               *right; ///< The right-hand side of the expression.
      
      /// Evaluates this node recursively, returning a value containing its result.
      virtual value eval() const;
      /// Coerces this node recursively for type, returning a full_type representing it.
      virtual full_type coerce() const;
      
      AST_Node_Binary(AST_Node* left=NULL, AST_Node* right=NULL); ///< Default constructor. Sets children to NULL.
      AST_Node_Binary(AST_Node* left, AST_Node* right, string op); ///< Default constructor. Sets children to NULL.
      ~AST_Node_Binary(); ///< Default destructor. Frees children recursively.
      
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
    };
    /// Child of AST_Node for the scope resolution operator, ::.
    struct AST_Node_Scope: AST_Node_Binary {
      virtual value eval() const; ///< Evaluates this node recursively, returning a value containing its result.
      virtual full_type coerce() const; ///< Coerces this node recursively for type, returning a full_type representing it.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      AST_Node_Scope(AST_Node* left, AST_Node* right, string op); ///< The one and only know-what-you're-doing constructor.
    };
    /// Child of AST_Node for the ternary operator.
    struct AST_Node_Ternary: AST_Node {
      AST_Node *exp; ///< The expression before the question marks.
      AST_Node *left; ///< The left-hand (true) result.
      AST_Node *right; ///< The right-hand (false) result.
      
      virtual value eval() const; ///< Evaluates this node recursively, returning a value containing its result.
      virtual full_type coerce() const; ///< Coerces this node recursively for type, returning a full_type representing it.
      
      AST_Node_Ternary(AST_Node *expression = NULL, AST_Node *exp_true = NULL, AST_Node *exp_false = NULL); ///< Default constructor. Sets children to NULL.
      AST_Node_Ternary(AST_Node *expression, AST_Node *exp_true, AST_Node *exp_false, string ct); ///< Complete constructor, with children and a content string.
      ~AST_Node_Ternary(); ///< Default destructor. Frees children recursively.
      
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
      virtual value eval() const;
      /// Coerces this node recursively for type, returning a full_type representing it.
      virtual full_type coerce() const;
      
      AST_Node_Subscript(); ///< Default constructor. Sets children to NULL.
      ~AST_Node_Subscript(); ///< Default destructor. Frees children recursively.
      void setleft(AST_Node* l); ///< Set the left-hand operand (the container).
      void setright(AST_Node* r); ///< Set the right-hand operand (the index expression).
      bool full(); ///< Returns true if this node is already completely full, meaning it has no room for children.
      
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
    };
    struct AST_Node_Array: AST_Node {
      vector<AST_Node*> elements; ///< Vector of our array elements.
      
      virtual value eval() const; ///< Evaluates this node recursively, returning a value containing its result.
      virtual full_type coerce() const; ///< Coerces this node recursively for type, returning a full_type representing it.
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      
      virtual ~AST_Node_Array();
    };
    /// Child of AST_Node for function call parameters.
    struct AST_Node_Parameters: AST_Node {
      AST_Node *func;
      vector<AST_Node*> params;
      
      /// Evaluates this node recursively, returning a value containing its result.
      virtual value eval() const;
      /// Coerces this node recursively for type, returning a full_type representing it.
      virtual full_type coerce() const;
      
      AST_Node_Parameters(); ///< Default constructor. Sets children to NULL.
      ~AST_Node_Parameters(); ///< Default destructor. Frees children recursively.
      void setleft(AST_Node* l); ///< Set the left-hand operand (the function).
      void setright(AST_Node* r); ///< Set the right-hand operand (adds a parameter).
      bool full(); ///< Returns true if this node is already completely full, meaning it has no room for children.
      
      virtual string toString() const; ///< Renders this node and its children as a string, recursively.
      virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
      virtual int width(); ///< Returns the width which will be used to render this node and all its children.
      virtual int height(); ///< Returns the height which will be used to render this node and all its children.
    };
    
    AST_Node *root; ///< The first node in our AST--The last operation that will be performed.
    error_handler *herr; ///< The error handler which will receive any error messages.
    lexer *lex; ///< The lexer from which tokens will be read.
    definition_scope *search_scope; ///< The scope from which token values will be harvested.
    
    /// Private method to fetch the next token from the lexer, with or without a scope.
    jdip::token_t get_next_token();
    
    // State flags
    bool tt_greater_is_op; ///< True if the greater-than symbol is to be interpreted as an operator.
    
    /** Handle a whole expression, stopping at the first unexpected token or when an
        operator is encountered which has a precendence lower than the one specified.
        I.E., passing a precedence of 0 will handle all operators.
        @param  token       The first token to be handled. Will be set to the first unhandled token. [in-out]
        @param  precedence  The lowest precedence of any operators to be handled.
    **/
    AST_Node* parse_expression(jdip::token_t &token, int precedence = 0);
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
    AST_Node* parse_binary_or_unary_post(jdip::token_t &token, AST_Node *left_node, int prec_min);
    /** Handle anything you'd expect to see at the start of an expression, being a
        unary prefix operator or a literal.
        @param  token  The first token to be handled. Will be set to the first unhandled token. [in-out]
        @return Returns the node of the operator of lowest precedence (ie, the root
                node), or NULL if an error occurs.
    **/
    AST_Node* parse_unary_pre_or_literal(jdip::token_t& token);
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
    
    /** Parse in an expression, building an AST.
        @param lex    The lexer which will be polled for tokens.
        @param herr   The error handler which will receive any warning or error messages.
        @return  This function will return 0 if no error has occurred, or nonzero otherwise.
    **/
    int parse_expression(lexer *lex, error_handler *herr = def_error_handler);
    
    /** Parse in an expression, building an AST, returning a token as well; DO NOT confuse with
        the sister overload which does not utilize the passed token.
        
        @param token  The first token to handle. Will be overwritten with the first unhandled token. [in-out]
        @param lex    The lexer which will be polled for tokens.
        @param prec   The lower-bound precedence.
        @param herr   The error handler which will receive any warning or error messages.
        @return  This function will return 0 if no error has occurred, or nonzero otherwise.
    **/
    int parse_expression(jdip::token_t& token, lexer *lex, int prec, error_handler *herr = def_error_handler);
    
    /** Parse in an expression, building an AST, returning a token as well; DO NOT confuse with
        the sister overload which utilizes the passed token.
        @param lex    The lexer which will be polled for tokens.
        @param token  A buffer for the first unhandled token. [out]
        @param prec   The lower-bound precedence.
        @param herr   The error handler which will receive any warning or error messages.
        @return  This function will return 0 if no error has occurred, or nonzero otherwise.
    **/
    int parse_expression(lexer *lex, jdip::token_t& token, int prec, error_handler *herr = def_error_handler);
    /** Parse in an expression, building an AST, with scope information, starting with the given token.
        @param lex    The lexer which will be polled for tokens.
        @param token  A buffer for the first unhandled token. [out]
        @param scope  The scope from which values of definitions will be read. [in]
        @param prec   The lower-bound precedence.
        @param herr   The error handler which will receive any warning or error messages.
        @return  This function will return 0 if no error has occurred, or nonzero otherwise.
    **/
    int parse_expression(jdip::token_t &token, lexer *lex, definition_scope *scope, int prec, error_handler *uherr);
    
    /// Evaluate the current AST, returning its \c value.
    value eval() const;
    
    /// Coerce the current AST for the type of its result.
    full_type coerce() const;
    
    /// Clear the AST out, effectively creating a new instance of this class
    void clear();
    
    /// Render the AST as a string: This is a relatively costly operation.
    string toString() const; ///< Renders this node and its children as a string, recursively.
    
    /// Render the AST to an SVG file.
    void writeSVG(const char* filename);
    
    /// Use this AST for template parameters
    inline void set_use_for_templates(bool use) { tt_greater_is_op = !use; }
    
    /// Default constructor. Zeroes some stuff.
    AST();
    /// Construct with a single node
    AST(definition* def);
    
    /// Default destructor. Deletes the AST.
    ~AST();
  };
}

#include <System/symbols.h>

#endif
