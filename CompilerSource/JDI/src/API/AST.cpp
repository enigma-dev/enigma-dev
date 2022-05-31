/**
 * @file  AST.cpp
 * @brief Source implementing a class that handles expression evaluation and type
 *        coercion via an AST.
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

#include "AST.h"
#include <General/parse_basics.h>
#include <Storage/value_funcs.h>
#include <System/builtins.h>
#include <System/symbols.h>
#include <Parser/context_parser.h>
#include <API/compile_settings.h>

#if !defined(__APPLE__) && !defined(__FreeBSD__)
#include <malloc.h>
#endif

#include <cassert>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <map>

using namespace std;
using namespace jdi;

#ifdef DEBUG_MODE
#define track(ct) ast->expression += ct + " "
#else
#define track(ct)
#endif

#include "AST_operator.h"

namespace {

inline string unescape(volatile const char* str, size_t len) {
  string res;
  int pad = len && (*str == '"' || *str == '\'');
  len -= pad;
  //res.reserve(len);
  for (size_t i = pad; i < len; ++i) {
    if (str[i] == '\\')
      switch (str[++i]) {
        case '\\': res.append(1, '\\'); break;
        case '\"': res.append(1, '\"'); break;
        case '\'': res.append(1, '\''); break;
        case 'r':  res.append(1, '\r'); break;
        case 'n':  res.append(1, '\n'); break;
        case 't':  res.append(1, '\t'); break;

        unsigned char accum;
        case '0': case '1':
        case '2': case '3': case '4': case '5': case '6': case '7':
          for (accum = 0; str[i] >= '0' && str[i] <= '7' && accum < 040; ++i) {
            accum <<= 3;
            accum += str[i] - '0';
          }
          res.append(1, (char)accum);
          break;

        case '\n': break;
        case '\r': if (str[++i] != '\n') --i; break;
        default:   res.append(1, '\\');  --i; break;
      }
    else
      res.append(1, str[i]);
  }
  return res;
}
inline long parselong(string s) {
  long res = 0;
  for (size_t i = 0; i < s.length(); ++i)
    res <<= 8, res |= s[i];
  return res;
}

value strtoval(const string &str, int radix) {
  bool is_unsigned = false;
  for (char c : str) if (c == 'u' || c == 'U') {
    is_unsigned = true;
    break;
  }
  return is_unsigned ? value(strtoull(str.c_str(), nullptr, radix))
                     : value(strtoll(str.c_str(),  nullptr, radix));
}

}  // namespace

namespace jdi {

unique_ptr<AST_Node> AST_Builder::parse_expression(AST* ast, token_t &token, int prec_min) {
  string ct;
  unique_ptr<AST_Node> myroot;
  AST_TYPE at = AT_BINARYOP;

  bool read_next = false; // True at the end of this switch if the next token has already been read.
  bool handled_basics = false; // True at the end of this switch if the basic token info was already read in.
  switch (token.type) {
    case TT_DECFLAG: case TT_EXTERN:
    case TT_CLASS: case TT_STRUCT: case TT_ENUM: case TT_UNION: {
        full_type ft = cparse->read_type(token, search_scope); // Read the full set of declarators
        track(ft.toString());
        myroot = make_unique<AST_Node_Type>(ft);
        myroot->filename = token.get_filename();
        myroot->linenum = token.linenum;
        myroot->pos = token.pos;
        handled_basics = read_next = true;
      } break;

    case TT_DECLARATOR: case TT_DEFINITION:
        at = AT_DEFINITION;
        myroot = make_unique<AST_Node_Definition>(token.def, token.content.toString());
        track(myroot->content);
      break;

    case TT_IDENTIFIER: {
        if (search_scope) {
          string n(token.content.toString());
          definition *def = search_scope->look_up(n);
          if (def) {
            myroot = make_unique<AST_Node_Definition>(def, token.content.toString());
            at = AT_DEFINITION;
          }
          else
            myroot = make_unique<AST_Node>(token.content.toString(), at = AT_IDENTIFIER);
        }
        else
          myroot = make_unique<AST_Node>(token.content.toString(), at = AT_IDENTIFIER);

        track(myroot->content);
      } break;

    case TT_OPERATORKW: {
      herr->error(token)
          << "Please refer to operators in their binary form; "
             "explicit use of operator functions not presently supported.";
      break;
    }

    case TT_TYPENAME: {
      token = get_next_token();
      unique_ptr<AST_Node> an = parse_expression(ast, token, precedence::scope);
      if (!an or (an->type != AT_DEFINITION && an->type != AT_SCOPE && an->type != AT_TYPE)) {
        token.report_error(herr, "Expected qualified identifier for type after `typename' token");
        return nullptr;
      }
      // TODO: this token needs marked as a type, somehow. Probably by creating a child
      // of AST_Node_Type which stores an ast, and sets tp.def to arg_key::abstract.
      return an;
    }

    case TT_TEMPLATE:
      token = get_next_token();
      if (token.type != TT_DEFINITION and token.type != TT_IDENTIFIER) {
        token.report_errorf(herr, "Expected identifier to treat as template before %s");
        return nullptr;
      }
      myroot = make_unique<AST_Node>(token.content.toString(), at = AT_TEMPID);
      break;

    case TT_SLASH: case TT_MODULO: case TT_NOT_EQUAL_TO: case TT_EQUAL_TO:
    case TT_LESS_EQUAL: case TT_GREATER_EQUAL: case TT_LSHIFT: case TT_RSHIFT:
    case TT_PIPE: case TT_PIPES: case TT_CARET: case TT_QUESTIONMARK:
    case TT_ARROW: case TT_DOT: case TT_ARROW_STAR: case TT_DOT_STAR:

    case TT_EQUAL: case TT_ADD_ASSIGN: case TT_SUBTRACT_ASSIGN:
    case TT_MULTIPLY_ASSIGN: case TT_DIVIDE_ASSIGN: case TT_MODULO_ASSIGN:
    case TT_LSHIFT_ASSIGN: case TT_RSHIFT_ASSIGN:
    case TT_AND_ASSIGN: case TT_OR_ASSIGN: case TT_XOR_ASSIGN: case TT_NEGATE_ASSIGN:

    // TODO: None of the above can be used as unary operators.
    case TT_PLUS: case TT_MINUS: case TT_INCREMENT: case TT_DECREMENT:
    case TT_NOT: case TT_TILDE: case TT_STAR: case TT_AMPERSAND: case TT_AMPERSANDS: {
      ct = token.content.toString();
      symbol& op = symbols[ct];
      if (not(op.type & ST_UNARY_PRE)) {
        token.report_error(herr, "Operator " + ct
                           + " cannot be used as unary prefix");
        return nullptr;
      }
      track(ct);
      token = get_next_token();
      myroot = make_unique<AST_Node_Unary>(parse_expression(ast, token, op.prec_unary_pre), ct, true);
      read_next = true;
    } break;

    case TT_GREATERTHAN: case TT_LESSTHAN: case TT_COLON:
      herr->error(token) << (ast->tt_greater_is_op
          ? "Expected expression here before greater-than operator"
          : "Expected expression here before closing triangle bracket");
      return nullptr;

    case TT_SCOPE:
        token.report_error(herr, "Unimplemented: global access '::'");
      return nullptr;
    case TT_MEMBER:
        token.report_error(herr, "Unhandled (class::*) pointer");
      return nullptr;

    case TT_LEFTPARENTH:
      track(string("("));
      token = get_next_token();
      myroot = parse_expression(ast, token, 0);
      if (myroot == nullptr) return nullptr;
      if (token.type != TT_RIGHTPARENTH) {
        herr->error(token) << "Expected closing parenthesis here before "
                           << token;
        return nullptr;
      }
      track(string(")"));
      if (myroot->type == AT_TYPE) {
        AST_Node_Type *ad = (AST_Node_Type*) myroot.get();
        token = get_next_token();
        read_next = true;

        auto nr = make_unique<AST_Node_Cast>(
            parse_expression(ast, token, symbols["(cast)"].prec_unary_pre));
        nr->cast_type.swap(ad->dec_type);
        nr->content = nr->cast_type.toString();
        myroot = std::move(nr);
      } else if (myroot->type == AT_DEFINITION &&
          (((AST_Node_Definition*) myroot.get())->def->flags & DEF_TYPENAME)) {
        AST_Node_Definition *ad = (AST_Node_Definition*) myroot.get();
        token = get_next_token();
        read_next = true;

        auto nr = make_unique<AST_Node_Cast>(
            parse_expression(ast, token, symbols["(cast)"].prec_unary_pre));
        nr->cast_type.def = ad->def;
        nr->content = nr->cast_type.toString();
        myroot = std::move(nr);
      }

      handled_basics = true;
      break;

    case TT_LEFTBRACKET:
    case TT_LEFTBRACE: {
      auto array = make_unique<AST_Node_Array>();
      token = get_next_token();
      while (token.type != TT_RIGHTBRACE and token.type != TT_SEMICOLON and token.type != TT_ENDOFCODE) {
        unique_ptr<AST_Node> node =
            parse_expression(ast, token, precedence::comma + 1);
        if (!node) {
          herr->error(token) << "Expected expression for array element";
          FATAL_RETURN(array);
        }
        array->elements.push_back(std::move(node));
        if (token.type != TT_COMMA) {
          if (token.type == TT_RIGHTBRACE) break;
          herr->error(token)
              << "Expected comma to separate array elements before " << token;
          FATAL_RETURN(array); break;
        }
        else token = get_next_token();
      }
      at = AT_ARRAY;
      myroot = std::move(array);
    }
    break;

    case TT_EXTENSION:  // This is GNU warning us shit's about to go down.
      token = get_next_token();
      return parse_expression(ast, token, prec_min);

    case TT_NEW: {
      token = get_next_token();
      auto ann = make_unique<AST_Node_new>();
      if (token.type == TT_LEFTPARENTH) {
        track(string("("));
        ann->position = parse_expression(ast, token, 0);
        if (ann->position == nullptr) return nullptr;
        if (token.type != TT_RIGHTPARENTH) {
          herr->error(token)
              << "Expected closing parenthesis for placement new here before "
              << token;
          return nullptr;
        }
        track(string(")"));
        token = get_next_token();
      }
      ann->alloc_type = cparse->read_type(token, search_scope);

      bool stillgoing = true;
      while (token.type == TT_STAR) {
        ann->alloc_type.refs.push(ref_stack::RT_POINTERTO);
      }

      if (stillgoing and token.type == TT_LEFTBRACKET) {
        track(string("["));
        token = get_next_token();
        ann->bound = parse_expression(ast, token, 0);
        if (ann->bound == nullptr) return nullptr;
        if (token.type != TT_RIGHTBRACKET) {
          herr->error(token)
              << "Expected closing parenthesis for placement new here before "
              << token;
          return nullptr;
        }
        track(string("]"));
        token = get_next_token();
      }

      myroot = std::move(ann);
    } break;

    case TT_DELETE: {
      token = get_next_token();
      bool is_array = token.type == TT_LEFTBRACKET;
      if (is_array) {
        if ((token = get_next_token()).type != TT_RIGHTBRACKET) {
          herr->error(token)
              << "Brackets to operator delete[] should be empty; "
              << "expected right bracket before " << token;
          return nullptr;
        }
      }
      myroot = make_unique<AST_Node_delete>(
          parse_expression(ast, token, precedence::unary_pre), is_array);
    } break;

    case TT_COMMA:
    case TT_SEMICOLON:
      herr->error(token) << "Expected expression before " << token;
      return nullptr;

    case TT_STRINGLITERAL: myroot = make_unique<AST_Node>(unescape(token.content.str, token.content.len), at = AT_STRLITERAL);
                           track(token.content.toString()); break;
    case TT_CHARLITERAL:   myroot = make_unique<AST_Node>(unescape(token.content.str, token.content.len), at = AT_CHRLITERAL);
                           track(token.content.toString()); break;

    case TT_DECLITERAL:
        myroot = make_unique<AST_Node>(token.content.toString(), at = AT_DECLITERAL);
        track(myroot->content);
      break;
    case TT_HEXLITERAL:
        myroot = make_unique<AST_Node>(token.content.toString(), at = AT_HEXLITERAL);
        track(myroot->content);
      break;
    case TT_OCTLITERAL:
        myroot = make_unique<AST_Node>(token.content.toString(), at = AT_OCTLITERAL);
        track(myroot->content);
      break;
    case TT_BINLITERAL:
        myroot = make_unique<AST_Node>(token.content.toString(), at = AT_BINLITERAL);
        track(myroot->content);
      break;
    case TT_TRUE: case TT_FALSE:
        myroot = make_unique<AST_Node>(token.content.toString(), at = AT_BOOLLITERAL);
        track(myroot->content);
      break;

    case TT_DECLTYPE: {
      token_t dt_tok = token;
      token = get_next_token();
      if (token.type != TT_LEFTPARENTH) {
        herr->error(token)
            << "Expected `(` for `decltype` before " << token;
        return nullptr;
      }
      track(string("("));
      token = get_next_token();
      auto coerce_node = parse_expression(ast, token, 0);
      if (!coerce_node) {
        herr->error(token) << "Bad expression to `decltype`";
        return nullptr;
      }
      if (token.type != TT_RIGHTPARENTH) {
        herr->error(token)
            << "Expected closing parenthesis to `decltype` before "
            << token;
      } else {
        track(string(")"));
        token = get_next_token();
      }

      // FIXME: It would be better to store this AST as a sub-AST of
      // some decltype node, but I'm lazy.
      ErrorContext errc(herr, dt_tok);
      full_type ft = coerce_node->coerce(errc);
      myroot = make_unique<AST_Node_Type>(ft);
      at = AT_DEFINITION;
      read_next = true;
      break;
    }

    case TT_TYPEOF:
        herr->error(token) << "Unimplemented: `typeof'.";
      return nullptr;
    case TT_TYPEID:
        herr->error(token) << "Unimplemented: `typeid'.";
      return nullptr;

    case TT_NOEXCEPT:
        herr->error(token) << "Unimplemented: `noexcept'.";
      return nullptr;

    /* ISEMPTY and SIZEOF */ {
        bool not_result;
        if (true) {
          case TT_ISEMPTY: not_result = true; track(string("isempty"));
        } else {
          case TT_SIZEOF: not_result = false; track(string("sizeof"));
        }
        token = get_next_token();
        if (token.type == TT_LEFTPARENTH) {
          token = get_next_token(); track(string("("));
          full_type ft = cparse->read_fulltype(token, search_scope);
          myroot = make_unique<AST_Node_sizeof>(make_unique<AST_Node_Type>(ft), not_result);
          if (token.type != TT_RIGHTPARENTH) {
            herr->error(token)
                << "Expected closing parenthesis to `sizeof` before "
                << token;
          } else {
            track(string(")"));
            token = get_next_token();
          }
        } else {
          myroot = make_unique<AST_Node_sizeof>(parse_expression(ast, token,precedence::unary_pre), not_result);
        }
        at = AT_UNARY_PREFIX;
        read_next = true;
    } break;

    /* Mixed cast types */ {
      AST_Node_Cast::cast_modes mode;
      while (false) {
        case TT_CONST_CAST:        mode = AST_Node_Cast::CM_CONST; break;
        case TT_STATIC_CAST:       mode = AST_Node_Cast::CM_STATIC; break;
        case TT_DYNAMIC_CAST:      mode = AST_Node_Cast::CM_DYNAMIC; break;
        case TT_REINTERPRET_CAST:  mode = AST_Node_Cast::CM_REINTERPRET; break;
      }
      token = get_next_token();
      if (token.type != TT_LESSTHAN) {
        token.report_error(herr, "Expected `< type-id >' following cast mode specifier");
        return nullptr;
      }
      token = get_next_token();
      full_type casttype = cparse->read_type(token, search_scope);
      if (!casttype.def) return nullptr;
      if (token.type != TT_GREATERTHAN){
        token.report_errorf(herr, "Expected closing triangle bracket to cast type before %s");
        return nullptr;
      }
      token = get_next_token();
      if (token.type != TT_LEFTPARENTH) {
        token.report_errorf(herr, "Expected opening parenthesis for cast expression before %s");
        return nullptr;
      }
      token = get_next_token();
      myroot = make_unique<AST_Node_Cast>(parse_expression(ast, token, 0), casttype, mode);
      if (token.type != TT_RIGHTPARENTH) {
        token.report_errorf(herr, "Expected closing parenthesis for cast expression before %s");
        return nullptr;
      }
      break;
    }

    case TT_ALIGNOF:
        token.report_error(herr, "Unimplemented: `alignof'.");
      return nullptr;

    case TT_ELLIPSIS:
    case TT_RIGHTPARENTH: case TT_RIGHTBRACKET: case TT_RIGHTBRACE:
      // Overflow; same error.
    case TT_NAMESPACE: case TT_ENDOFCODE: case TT_ASM: case TT_TYPEDEF:
    case TT_PUBLIC: case TT_PRIVATE: case TT_PROTECTED: case TT_FRIEND:
    case TT_ALIGNAS: case TT_AUTO: case TT_CONSTEXPR: case TT_STATIC_ASSERT:
    case TT_ATTRIBUTE: case TT_USING: case TT_INLINE:
    case TT_THROW:
    #include <User/token_cases.h>
      token.report_errorf(herr, "Expected expression before %s");
      return nullptr;

    case TTM_CONCAT: case TTM_TOSTRING:
    case TTM_WHITESPACE: case TTM_COMMENT: case TTM_NEWLINE:
    token.report_error(herr, "Illogical token type returned!");
      return nullptr;
    case TT_INVALID: default: token.report_error(herr, "Invalid token type returned!");
      return nullptr;
  }
  if (!handled_basics) {
    myroot->type = at;
    myroot->filename = token.get_filename();
    myroot->linenum = token.linenum;
    myroot->pos = token.pos;
  }
  if (!read_next)
    token = get_next_token();

  myroot = parse_binary_or_unary_post(ast, token, std::move(myroot), prec_min);
  return myroot;
}

unique_ptr<AST_Node> AST_Builder::parse_binary_or_unary_post(
    AST* ast, token_t &token, unique_ptr<AST_Node> left_node, int prec_min) {
  switch (token.type) {
    case TT_DECLARATOR: case TT_DECFLAG: case TT_CLASS: case TT_STRUCT:
    case TT_ENUM: case TT_UNION: case TT_EXTERN: case TT_INLINE:
      return left_node;

    case TT_IDENTIFIER: case TT_DEFINITION:
      return left_node;

    case TT_TYPENAME:
      token.report_error(herr, "Unimplemented: typename.");
      return nullptr;

    case TT_COLON:
      return left_node;

    case TT_GREATERTHAN:
      if (!ast->tt_greater_is_op)
        return left_node;
      goto case_TT_OPERATOR;

    case TT_SCOPE: {
      track(string("::"));
      token = get_next_token();
      unique_ptr<AST_Node> right =
          parse_expression(ast, token, precedence::scope + 1);
      if (!right) {
        herr->error(token) << "Expected qualified-id for scope access";
        return left_node;
      }
      left_node = make_unique<AST_Node_Scope>(std::move(left_node),
                                              std::move(right), "::");
      break;
    }
    case TT_MEMBER:
      track(string("::*"));
      herr->error(token) << "Not handled: (class::*) pointers";
      return nullptr;

    case TT_LESSTHAN:
      if (left_node->type == AT_SCOPE ||
          left_node->type == AT_DEFINITION ||
          left_node->type == AT_TEMPID) {
        if (precedence::scope < prec_min)
          return left_node;
        definition *def = left_node->coerce(ErrorContext(herr, token)).def;
        if ((def && (def->flags & DEF_TEMPLATE)) ||
            (left_node->type == AT_TEMPID) ||
            (left_node->type == AT_SCOPE &&
               ((AST_Node_Scope*) left_node.get())->right &&
               ((AST_Node_Scope*) left_node.get())->right->type == AT_TEMPID)) {
          track(string("<"));
          auto ti = make_unique<AST_Node_TempInst>(std::move(left_node),
                                                   "()<>");
          token = get_next_token();

          bool gtio = ast->tt_greater_is_op;
          ast->tt_greater_is_op = false;
          for (;;) {
            unique_ptr<AST_Node> node =
                parse_expression(ast, token, precedence::comma + 1);
            if (node == nullptr) break;
            ti->params.push_back(std::move(node));
            if (token.type == TT_GREATERTHAN) {
              token = get_next_token();
              break;
            }
            if (token.type == TT_COMMA) {
              token = get_next_token();
              continue;
            }
            herr->error(token) << "Expected closing triangle bracket before "
                               << token;
            break;
          }
          track(string(">"));
          ast->tt_greater_is_op = gtio;
          left_node = std::move(ti);
          break;
        }
      }
      // Fallthrough

    case TT_PLUS: case TT_MINUS: case TT_STAR: case TT_SLASH: case TT_MODULO:
    case TT_INCREMENT: case TT_DECREMENT: case TT_NOT: case TT_TILDE:
    case TT_LSHIFT: case TT_RSHIFT:

    case TT_EQUAL_TO: case TT_NOT_EQUAL_TO: case TT_LESS_EQUAL: case TT_GREATER_EQUAL:
    case TT_AMPERSAND: case TT_AMPERSANDS: case TT_PIPE: case TT_PIPES: case TT_CARET:
    case TT_ARROW: case TT_DOT: case TT_ARROW_STAR: case TT_DOT_STAR: case TT_QUESTIONMARK:

    case TT_EQUAL: case TT_ADD_ASSIGN: case TT_SUBTRACT_ASSIGN: case TT_MULTIPLY_ASSIGN:
    case TT_DIVIDE_ASSIGN: case TT_MODULO_ASSIGN: case TT_LSHIFT_ASSIGN: case TT_RSHIFT_ASSIGN:
    case TT_AND_ASSIGN: case TT_OR_ASSIGN: case TT_XOR_ASSIGN: case TT_NEGATE_ASSIGN:
    case TT_THROW:
    case_TT_OPERATOR: {
        string op(token.content.toString());
        auto b = symbols.find(op);
        if (b == symbols.end()) {
          token.report_error(herr, "Operator `" + token.content.toString() + "' not defined");
          return nullptr;
        }
        if (left_node->type == AT_DEFINITION) {
          AST_Node_Definition *ad = (AST_Node_Definition*) left_node.get();
          if (ad->def->flags & DEF_TYPENAME) {
            full_type cme(ad->def);
            left_node = make_unique<AST_Node_Type>(cme);
          }
        }
        if (left_node->type == AT_TYPE) {
          if (token.content.len == 1) {
            if (*token.content.str == '*') {
              ((AST_Node_Type*) left_node.get())->dec_type
                  .refs.push(ref_stack::RT_POINTERTO);
              token = get_next_token();
              break;
            }
            if (*token.content.str == '&') {
              ((AST_Node_Type*) left_node.get())->dec_type
                  .refs.push(ref_stack::RT_REFERENCE);
              token = get_next_token();
              break;
            }
          }
          herr->error(token)
              << "Cannot operate on type "
              << PQuote(((AST_Node_Type*) left_node.get())->dec_type);
          return nullptr;
        }
        symbol &s = b->second;
        if (s.type & ST_BINARY) {
          if (s.prec_binary < prec_min)
            return left_node;
          token = get_next_token();
          track(op);
          unique_ptr<AST_Node> right = parse_expression(ast, token, s.prec_binary + !(s.type & ST_RTL_PARSED));
          if (!right) {
            herr->error(token)
                << "Expected secondary expression after binary operator "
                << PQuote(op);
            return left_node;
          }
          left_node = make_unique<AST_Node_Binary>(std::move(left_node),
                                                   std::move(right), op);
        } else if (s.type & ST_TERNARY) {
          if (s.prec_binary < prec_min)
            return left_node;
          string ct(token.content.toString());
          track(ct);

          token = get_next_token();
          unique_ptr<AST_Node> exptrue = parse_expression(ast, token, 0);
          if (!exptrue) return nullptr;
          if (token.type != TT_COLON) {
            herr->error(token) << "Colon expected to separate ternary operands";
            return nullptr;
          }
          track(string(":"));

          token = get_next_token();
          unique_ptr<AST_Node> expfalse = parse_expression(ast, token, 0);
          if (!expfalse) return nullptr;

          left_node = make_unique<AST_Node_Ternary>(
              std::move(left_node), std::move(exptrue), std::move(expfalse),
              ct);
        } else if (s.type & ST_UNARY_POST) {
          if (s.prec_unary_post < prec_min)
            return left_node;
          left_node = make_unique<AST_Node_Unary>(std::move(left_node),
                                                  op, false);
          token = get_next_token();
        } else {
          if (s.type & ST_UNARY_PRE) {
            herr->error(token) << "Expected binary operator before unary "
                               << PQuote(op) << " operator...";
          } else {
            herr->error(token) << "Symbol " << PQuote(op)
                               << " is known, but has no operator type...";
          }
          return left_node;
        }
      }
      break;

    case TT_LEFTPARENTH:
        if (left_node->type == AT_DEFINITION ||
            left_node->type == AT_TYPE ||
            left_node->type == AT_SCOPE) {
          full_type ltype = left_node->coerce(ErrorContext(herr, token));
          if (ltype.def and ltype.def->flags & DEF_TYPENAME)
          {
            AST_Node_Type *ant;
            full_type ft_stacked, *ft_annoying;
            if (left_node->type == AT_TYPE)
              ft_annoying = &(ant = (AST_Node_Type*) left_node.get())->dec_type;
            else
              ant = nullptr, ft_annoying = &ft_stacked;
            full_type &ft = *ft_annoying;

            lexer::look_ahead lb(lex);
            bool is_cast = true; // True if the contents of these parentheses are part of the cast;
            // For example, in bool(*)(), the (*) is part of the cast. In bool(10), (10) is not part of the cast.

            lb.push(token);
            for (int depth = 1;;) {
              token_t tk = get_next_token();
              if (tk.type == TT_RIGHTPARENTH) {
                  if (!--depth) { token = tk; break; }
              }
              else if (tk.type == TT_LEFTPARENTH)
                ++depth;
              else if (tk.type == TT_ENDOFCODE)
                break;
              else if (tk.type != TT_STAR && tk.type != TT_AMPERSAND)
                is_cast = false;
            }
            if (token.type != TT_RIGHTPARENTH) {
              token.report_errorf(herr, "Expected closing parenthesis to cast here before %s");
              return nullptr;
            }

            lb.rewind();
            token = get_next_token();
            if (is_cast) {
              cparse->read_referencers(ft.refs, ft, token, search_scope); // Read all referencers
              track(ft.refs.toString());
              if (!ant) {
                auto uant = make_unique<AST_Node_Type>(ft);
                ant = uant.get();
                left_node = std::move(uant);
                left_node->filename = token.get_filename();
                left_node->linenum = token.linenum;
                left_node->pos = token.pos;
              }
            }
            else {
              if (token.type != TT_LEFTPARENTH) {
                herr->error(token)
                    << "INTERNAL ERROR: " << token
                    << " is not a left parenthesis. This should not happen.";
              }
              track(string("("));
              token = get_next_token();
              auto nr = make_unique<AST_Node_Cast>(
                  parse_expression(ast, token, 0), ft);
              if (token.type == TT_RIGHTPARENTH) {
                token = get_next_token();
              } else {
                herr->error(token)
                    << "Expected closing parenthesis to cast before "
                    << token;
              }
              nr->content = nr->cast_type.toString();

              left_node = std::move(nr);
              left_node->filename = token.get_filename();
              left_node->linenum = token.linenum;
              left_node->pos = token.pos;
              track(string(")"));
            }
            break;
          }
          track(string("("));
          token = get_next_token();

          bool gtio = ast->tt_greater_is_op; ast->tt_greater_is_op = true;
          unique_ptr<AST_Node> params = parse_expression(ast, token, precedence::all);
          ast->tt_greater_is_op = gtio;
          if (!params) {
            token.report_error(herr, "Expected secondary expression after binary operator");
            return left_node;
          }
          if (token.type != TT_RIGHTPARENTH) {
            token.report_errorf(herr, "Expected closing parenthesis here before %s");
            FATAL_RETURN(left_node);
          }
          left_node = make_unique<AST_Node_Binary>(std::move(left_node),
                                                   std::move(params), "");
          token = get_next_token(); // Skip that closing paren
          track(string(")"));
          break;
        }
      return left_node;

    case TT_COMMA: {
        if (precedence::comma < prec_min)
          return left_node;
        token = get_next_token();
        string op(","); track(op);
        unique_ptr<AST_Node> right = parse_expression(ast, token, precedence::comma);
        if (!right) {
          token.report_error(herr, "Expected secondary expression after comma");
          return left_node;
        }
        left_node = make_unique<AST_Node_Binary>(std::move(left_node),
                                                 std::move(right), op);
      } break;

    case TT_LEFTBRACKET: {
        if (precedence::unary_post < prec_min)
          return left_node;
        token = get_next_token();
        string op("["); track(op);
        unique_ptr<AST_Node> indx = parse_expression(ast, token, precedence::comma);
        if (!indx) {
          token.report_error(herr, "Expected index for array subscript");
          return left_node;
        }
        left_node = make_unique<AST_Node_Subscript>(std::move(left_node),
                                                    std::move(indx));
        if (token.type != TT_RIGHTBRACKET) {
          token.report_errorf(herr, "Expected closing bracket to array subscript before %s");
          return left_node;
        } track(string("]"));
        token = get_next_token();
      } break;

    case TT_LEFTBRACE:
    case TT_SEMICOLON:

    case TT_STRINGLITERAL: case TT_CHARLITERAL:
    case TT_DECLITERAL: case TT_HEXLITERAL: case TT_OCTLITERAL:
    case TT_BINLITERAL: case TT_TRUE: case TT_FALSE:

    case TT_ELLIPSIS:

    case TT_RIGHTPARENTH: case TT_RIGHTBRACKET: case TT_RIGHTBRACE: return left_node;

    case TT_TEMPLATE: case TT_NAMESPACE: case TT_ENDOFCODE: case TT_TYPEDEF:
    case TT_USING: case TT_PUBLIC: case TT_PRIVATE: case TT_PROTECTED: case TT_FRIEND:
    case TT_ALIGNAS: case TT_AUTO: case TT_CONSTEXPR: case TT_STATIC_ASSERT:

    case TT_ASM: case TT_OPERATORKW: case TT_SIZEOF: case TT_ISEMPTY:
    case TT_ALIGNOF: case TT_DECLTYPE: case TT_TYPEID: case TT_TYPEOF:
    case TT_CONST_CAST: case TT_STATIC_CAST: case TT_DYNAMIC_CAST: case TT_REINTERPRET_CAST:
    case TT_NEW: case TT_DELETE:
    case TT_NOEXCEPT: case TT_ATTRIBUTE: case TT_EXTENSION:
    #include <User/token_cases.h>
    return left_node;

    case TTM_CONCAT: case TTM_TOSTRING: case TTM_COMMENT:
    case TTM_WHITESPACE:case TTM_NEWLINE: case TT_INVALID:
    default: return left_node;
  }
  return parse_binary_or_unary_post(ast, token, std::move(left_node), prec_min);
}

token_t AST_Builder::get_next_token() {
  return search_scope
      ? lex->get_token_in_scope(search_scope)
      : lex->get_token();
}

//===========================================================================================================================
//=: Evaluators :============================================================================================================
//===========================================================================================================================

value AST::eval(const ErrorContext &errc) const {
  if (!root) {
    errc.error("Evaluating a broken expression");
    return value();
  }
  return root->eval(errc);
}

value AST_Node::eval(const ErrorContext &errc) const {
  if (type == AT_DECLITERAL) {
    dec_literal:
    if (is_letter(content[content.length() - 1])) {
      bool is_float = false;
      char *number = (char*)alloca(content.length());
      memcpy(number, content.c_str(), content.length() * sizeof(char));
      // This block will fail if the number is all letters--but we know it isn't.
      for (char *i = number + content.length() - 1; is_letter(*i); --i) {
        if (*i == 'f' or *i == 'd' or  *i == 'F' or *i == 'D')
          is_float = true;
        *i = 0;
      }
      if (!is_float)
        for (size_t i = 0; i < content.length(); i++)
          if (content[i] == '.' or  content[i] == 'E' or content[i] == 'e')
            is_float = true;
      if (is_float)
        return value(atof(number));
      return value(atol(number));
    }
    if (content.find('.', 0) != string::npos)
      return value(atof(content.c_str()));
    return strtoval(content, 10);
  }
  if (type == AT_OCTLITERAL) {
    if (content.length() == 1)
      goto dec_literal; // A single octal digit is no different from a decimal digit
    return strtoval(content, 16);
  }
  if (type == AT_HEXLITERAL)
    return strtoval(content, 16);
  if (type == AT_STRLITERAL)
    return value(content);
  if (type == AT_CHRLITERAL)
    return value(parselong(content));
  if (type == AT_IDENTIFIER) {
    errc.error() << "Evaluation error: `" << content << "` is undeclared";
    return value();
  }
  errc.error() << "Evaluating unknown node type " << type;
  return value();
}
value AST_Node_Definition::eval(const ErrorContext &errc) const {
  if (def) {
    if (def->flags & DEF_VALUED)
      return ((definition_valued*)def)->value_of;
    if (def->flags & DEF_TEMPPARAM)
      return value(VT_DEPENDENT);
  }
  errc.error("Evaluating null definition");
  return value();
}
value AST_Node_Ternary::eval(const ErrorContext &errc) const {
  if (!exp) {
    errc.error("Bad ternary expression: cannot be evaluated");
    return value();
  }
  if (exp->eval(errc)) {
    if (left) return left->eval(errc);
    errc.error("Bad ternary left operand: cannot be evaluated");
  } else {
    if (right) return right->eval(errc);
    errc.error("Bad ternary right operand: cannot be evaluated");
  }
  return value();
}
value AST_Node_Binary::eval(const ErrorContext &errc) const {
  if (!left or !right) {
    errc.error("Invalid (null) operands");
    return value();
  }
  symbol_iter si = symbols.find(content);
  if (si == symbols.end()) {
    errc.error("Internal error! Operator not found!");
    return value();
  }
  symbol &s = si->second;
  if (!s.operate) {
    errc.error("Internal error: Operator has no definition");
    return value();
  }
  value l = left->eval(errc), r = right->eval(errc);
  value res = s.operate(l, r);
  return res;
}
value AST_Node_Scope::eval(const ErrorContext &errc) const {
  full_type res = left->coerce(errc);
  if (res.def == arg_key::abstract)
    return value(VT_DEPENDENT);
  if (!res.def or !(res.def->flags & DEF_SCOPE)) {
    if (res.def) errc.error() << "Invalid scope `" << res.def->name << "'";
    else errc.error() << "Invalid (nonsensical) scope to `::` operator";
    return value();
  }

  definition* d = ((definition_scope*)res.def)->find_local(right->content);
  if (!d or not(d->flags & DEF_VALUED)) {
    if (res.def->flags & DEF_DEPENDENT)
      return value(VT_DEPENDENT);
    errc.error() << "AST evaluation failure: No `" << right->content
                 << "' found in " << res.def->kind() << " " << res.def->name;
    return value(long(0));
  }
  return ((definition_valued*)d)->value_of;
}
value AST_Node_Unary::eval(const ErrorContext &errc) const {
  if (!operand) {
    errc.error() << "No operand to unary `operator" << content << "`!";
    return value();
  }
  if (prefix) {
    if (!symbols[content].operate_unary_pre) {
      errc.error() << "No method to unary `operator" << content << "`!";
      return value();
    }
    value b4 = operand->eval(errc);
    value after = symbols[content].operate_unary_pre(b4);
    return after;
  }
  else {
    if (!symbols[content].operate_unary_post) {
      errc.error() << "No method to unary `operator" << content << "`!";
      return value();
    }
    value b4 = operand->eval(errc);
    value after = symbols[content].operate_unary_post(b4);
    return after;
  }
}
/*value AST_Node_Group::eval() {
  return root?root->eval(errc):value();
}*/
value AST_Node_Parameters::eval(const ErrorContext &errc) const {
  errc.error("Evaluating a function call is not supported.");
  return value(); // We can't evaluate a function call ;_;
}
value AST_Node_Type::eval(const ErrorContext &errc) const {
  errc.error("Evaluating a type for value");
  return value();
}
value AST_Node_sizeof::eval(const ErrorContext &errc) const {
  if (!operand) {
    errc.error("Computing size of invalid type");
    return 0L;
  }
  definition *cd = operand->coerce(errc).def;
  return cd? (long)cd->size_of(errc) : 0;
}
value AST_Node_Cast::eval(const ErrorContext &errc) const {
  if (!operand || !cast_type.def) {
    errc.error("Invalid cast type");
    return value();
  }
  if (cast_type.def == builtin_type__int) {
    if (builtin_flag__short->Matches(cast_type.flags)) {
        if (cast_type.flags & builtin_flag__unsigned->mask)
          return value((long)(unsigned short)(long) operand->eval(errc));
        else
          return value((long)(short)(long) operand->eval(errc));
    } else if (builtin_flag__long->Matches(cast_type.flags) ||
               builtin_flag__long_long->Matches(cast_type.flags)) {
      if (cast_type.flags & builtin_flag__unsigned->mask)
        return value((long) operand->eval(errc));
      else
        return value((long)(int) operand->eval(errc));
    } else {
      if (cast_type.flags & builtin_flag__unsigned->mask)
        return value((long)(unsigned int)(long) operand->eval(errc));
      else
        return value((long)(int)(long) operand->eval(errc));
    }
  }
  else if (cast_type.def == builtin_type__float)
    return value((double)(float)(double)operand->eval(errc));
  else if (cast_type.def == builtin_type__double)
    return value((double)operand->eval(errc));
  else if (cast_type.def == builtin_type__char)
    return value((long)(char)(long)operand->eval(errc));
  else if (cast_type.def == builtin_type__bool)
    return value((long)(bool)operand->eval(errc));
  errc.error() << "Attempt to cast to `" << cast_type.def->name << "'";
  return value();
}
value AST_Node_Array::eval(const ErrorContext &errc) const {
  return elements.size()? elements.front()->eval(errc) : value(0l);
}
value AST_Node_new::eval(const ErrorContext &errc) const {
  if (!position) {
    errc.error("Cannot perform allocation in AST evaluation: not safe");
    return value();
  }
  return position->eval(errc);
}
value AST_Node_delete::eval(const ErrorContext &errc) const {
  errc.error("Cannot delete pointers in AST evaluation: unsafe");
  return value();
}
value AST_Node_Subscript::eval(const ErrorContext &errc) const
{
  value iv = index->eval(errc);
  if (iv.type != VT_INTEGER)
    return value();
  size_t ivl = (size_t)(long)iv;

  if (left->type == AT_ARRAY)
  {
    AST_Node_Array* la = (AST_Node_Array*) left.get();
    if (ivl >= la->elements.size()) {
      errc.error() << "Out of bounds read: accessing element "
                   << value(long(ivl)) <<  " of array of size "
                   << value(long(la->elements.size()));
      return value();
    }
    return la->elements[ivl]->eval(errc);
  }
  else
  {
    value av = left->eval(errc);
    if (av.type == VT_NONE)
      return av;
    if (av.type != VT_STRING)
      return value();
    string str = (string)av;
    if (ivl > str.length())
      return value();
    if (ivl >= str.length())
      return value(0L);
    return value(long(str[ivl]));
  }
}
value AST_Node_TempInst::eval(const ErrorContext &errc) const {
  // Evaluating a template instantiatoin, eg, basic_string<char>. Derp.
  // Shouldn't happen in parser code.
  errc.error("Attempting to evaluate a template instantiation.");
  return value();
}
value AST_Node_TempKeyInst::eval(const ErrorContext &errc) const {
  // Evaluating a template instantiatoin, eg, basic_string<char>. Derp.
  // Shouldn't happen in parser code.
  errc.error("Attempting to evaluate a template instantiation.");
  return value();
}

//===========================================================================================================================
//=: Coercers :==============================================================================================================
//===========================================================================================================================

full_type AST::coerce(const ErrorContext &errc) const {
  return root? root->coerce(errc) : full_type();
}

full_type AST_Node::coerce(const ErrorContext &errc) const {
  full_type res;
  unsigned long flag_mask = 0;
  res.def = builtin_type__int;
  res.flags = 0;
  for (size_t i = content.length(); i and is_letter(content[i]); --i) {
    if (content[i] == 'l' or content[i] == 'L') {
      builtin_flag__long->Apply(&flag_mask, &res.flags, errc);
    } else if (content[i] == 'u' or content[i] == 'U') {
      builtin_flag__unsigned->Apply(&flag_mask, &res.flags, errc);
    }
  }
  if (type == AT_DECLITERAL) {
    for (size_t i = 0; i < content.length(); ++i) {
      if (content[i] == '.' or content[i] == 'e' or content[i] == 'E') {
        res.def = builtin_type__double;
        while (++i < content.length()) {
          if (content[i] == 'f' or content[i] == 'F')
            res.def = builtin_type__float;
        }
        break;
      }
    }
  }
  return res;
}

full_type AST_Node_Binary::coerce(const ErrorContext &errc) const {
  //TODO: Implement using operator() functions.
  if (!left || !right) return {};
  return left->coerce(errc);
}

full_type AST_Node_Scope::coerce(const ErrorContext &errc) const {
  full_type res = left->coerce(errc);
  if (!res.def)
    return errc.error("Scope to access is nullptr!"), full_type();
  if (res.def == arg_key::abstract)
    return res.def;
  if (!(res.def->flags & DEF_SCOPE)) {
    res.reduce();
    if (!res.def)
      return errc.error("Scope is typedef to nullptr!"), full_type();
    if (!(res.def->flags & DEF_SCOPE)) {
      errc.error() << "Bad scope `" << res.def->name << "` ("
                   << flagnames(res.def->flags) << ")";
      return full_type();
    }
  }
  if (res.def == arg_key::abstract)
    return arg_key::abstract;
  definition_scope *scp = (definition_scope*)res.def;
  res.def = scp->find_local(right->content);
  if (!res.def) {
    if (scp->flags & DEF_DEPENDENT)
      return arg_key::abstract;
    errc.error()
        << "Scope " << PQuote(scp->name) << " (" << PQuote(left->toString())
        << ") has no member " << PQuote(right->content);
    scp->find_local(right->content);
  }
  return res;
}

full_type AST_Node_Cast::coerce(const ErrorContext &) const {
  return cast_type;
}

full_type AST_Node_Definition::coerce(const ErrorContext &) const {
  full_type res;
  if (def->flags & DEF_TYPED) {
    res.def = ((definition_typed*)def)->type;
    res.refs.copy(((definition_typed*)def)->referencers);
    res.flags = ((definition_typed*)def)->modifiers;
  }
  else
    res.def = def;
  return res;
}

full_type AST_Node_Parameters::coerce(const ErrorContext &errc) const {
  #ifdef DEBUG_MODE
    if (func->type != AT_DEFINITION ||
        !((AST_Node_Definition*) func.get())->def ||
        !(((AST_Node_Definition*)func.get())->def->flags & DEF_FUNCTION)) {
      errc.error("Left-hand of parameter list not a function");
      return full_type();
    }
  #endif
  vector<full_type> param_types;
  param_types.reserve(params.size());
  for (auto it = params.begin(); it != params.end(); ++it)
    param_types.push_back((*it)->coerce(errc));
  //definition_function* df = (definition_function*)((AST_Node_Definition*)func)->def;
  // TODO: Overload resolution
  // FIXME: Overload resolution!
  full_type res;
  res.def = builtin_type__int;
  //res.refs.copy(df->referencers);
  //res.refs.pop();
  //res.flags = df->flags;
  return res;
}

full_type AST_Node_sizeof::coerce(const ErrorContext &errc) const {
  full_type res;
  unsigned long mask = 0;
  res.def = builtin_type__int;
  builtin_flag__unsigned->Apply(&mask, &res.flags, errc);
  builtin_flag__long->Apply(&mask, &res.flags, errc);
  return res;
}

full_type AST_Node_Ternary::coerce(const ErrorContext &errc) const {
  full_type t1 = left->coerce(errc), t2 = right->coerce(errc);
  // FIXME: introduce common type resolution here using whatever casts_to() is used in overload resolution
  if (t1 != t2)
    errc.error("ERROR: Operands to ternary operator differ in type.");
  return t1;
}

full_type AST_Node_Type::coerce(const ErrorContext &) const {
  full_type ret;
  ret.copy(dec_type);
  return ret;
}

full_type AST_Node_Unary::coerce(const ErrorContext &errc) const {
  if (content.empty() || !operand) return {};
  switch (content[0]) {
    case '+': case '-': case '~': {
      return operand->coerce(errc);
    }
    case '*': {
      full_type res = operand->coerce(errc);
      res.refs.pop();
      return res;
    }
    case '&': {
      full_type res = operand->coerce(errc);
      res.refs.push(ref_stack::RT_POINTERTO);
      return res;
    }
    case '!': {
      return builtin_type__bool;
    }
    default: {
      errc.error() << "ERROR: Unknown coercion pattern for unary operator "
                   << PQuote(content);
      return operand->coerce(errc);
    }
  }
}

full_type AST_Node_Array::coerce(const ErrorContext &errc) const {
  if (elements.size()) {
    full_type res = elements[0]->coerce(errc);
    res.refs.push_array(elements.size());
    return res;
  }
  full_type res(builtin_type__int);
  res.refs.push_array(0);
  return res;
}

full_type AST_Node_new::coerce(const ErrorContext &errc) const {
  full_type res = alloc_type;
  res.refs.push_array(bound? (long)bound->eval(errc) : 0);
  return res;
}

full_type AST_Node_delete::coerce(const ErrorContext &errc) const {
  errc.warning("Type of delete expression not ignored");
  return builtin_type__void;
}

full_type AST_Node_Subscript::coerce(const ErrorContext &errc) const {
  if (!left || !index) {
    errc.error("Invalid operands to []");
    return full_type();
  }
  full_type res = left->coerce(errc);
  if (!res.refs.size()) {
    // FIXME: Look up operator[] here; check for return type
    if (index->type != AT_ARRAY) {
      errc.error("Left operand of array indices is not an array");
      return full_type();
    }
    res = index->coerce(errc);
  }
  res.refs.pop(); // Dereference that bitch
  return res;
}

full_type AST_Node_TempInst::coerce(const ErrorContext &errc) const
{
  full_type tpd = temp->coerce(errc);

  if (tpd.def == arg_key::abstract)
    return arg_key::abstract;
  if (!tpd.def or not(tpd.def->flags & DEF_TEMPLATE)) {
    errc.error() << "Bad template type "
                 << (tpd.def? tpd.def->name : "(nullptr)");
    return full_type();
  }

  definition_template *tplate = (definition_template*)tpd.def;
  arg_key k(tplate->params.size());
  k.mirror_types(tplate);
  if (tplate->params.size() < params.size()) {
    errc.error() << "Too many parameters to template instantiation; "
                 << PQuote(*temp) << " accepts only " << tplate->params.size()
                 << " parameters, but " << params.size() << " were given";
    return full_type();
  }

  for (size_t i = 0; i < params.size(); ++i) {
    if (tplate->params[i]->flags & DEF_TYPENAME) {
      full_type t = params[i]->coerce(errc);
      if (!t.def)
        return full_type();
      if (t.def == arg_key::abstract || t.def->flags & DEF_DEPENDENT)
        return arg_key::abstract;
      k.put_type(i, t);
    }
    else {
      value v = params[i]->eval(errc);
      // Bail out returning void if we have dependent parameters
      if (v.type == VT_DEPENDENT)
        return arg_key::abstract;
      k.put_value(i, v);
    }
  }
  check_read_template_parameters(k, params.size(), tplate, errc);
  return full_type(tplate->instantiate(k, errc));
}
full_type AST_Node_TempKeyInst::coerce(const ErrorContext &errc) const {
  if (key.is_abstract())
    return arg_key::abstract;
  return full_type(temp->instantiate(key, errc));
}

//===========================================================================================================================
//=: Constructors :==========================================================================================================
//===========================================================================================================================

static const string kStrSizeof("sizeof");
static const string kStrCast("cast");

AST_Node::AST_Node(AST_TYPE tp): type(tp) {}

AST_Node::AST_Node(string_view ct, AST_TYPE tp): type(tp), content(ct) {}

AST_Node_Definition::AST_Node_Definition(definition* d, string_view ct):
    AST_Node(ct, AT_DEFINITION), def(d) {}

AST_Node_Scope::AST_Node_Scope(unique_ptr<AST_Node> l, unique_ptr<AST_Node> r,
                               string_view op):
    AST_Node_Binary(std::move(l), std::move(r), op, AT_SCOPE) {}

AST_Node_Type::AST_Node_Type(full_type &ft):
    AST_Node(AT_TYPE) {
  dec_type.swap(ft);
}

AST_Node_Unary::AST_Node_Unary(AST_TYPE tp, unique_ptr<AST_Node> r):
    AST_Node(tp), operand(std::move(r)) {}

AST_Node_Unary::AST_Node_Unary(unique_ptr<AST_Node> r, string_view ct, bool pre,
                               AST_TYPE tp):
    AST_Node(ct, tp), operand(std::move(r)), prefix(pre) {}

AST_Node_Unary::AST_Node_Unary(unique_ptr<AST_Node> r, string_view ct,
                               bool pre):
    AST_Node(ct, pre? AT_UNARY_PREFIX : AT_UNARY_POSTFIX),
    operand(std::move(r)), prefix(pre) {}

AST_Node_sizeof::AST_Node_sizeof(unique_ptr<AST_Node> param, bool n):
    AST_Node_Unary(std::move(param), kStrSizeof, true, AT_SIZEOF), negate(n) {}

AST_Node_Cast::AST_Node_Cast(unique_ptr<AST_Node> param, const full_type& ft,
                             cast_modes cmode):
    AST_Node_Unary(std::move(param), kStrCast, true, AT_CAST),
    cast_mode(cmode) {
  cast_type.copy(ft);
}

AST_Node_Cast::AST_Node_Cast(unique_ptr<AST_Node> param, full_type& ft,
                             cast_modes cmode):
    AST_Node_Unary(std::move(param), kStrCast, true, AT_CAST),
    cast_mode(cmode) {
  cast_type.swap(ft);
}

AST_Node_Cast::AST_Node_Cast(unique_ptr<AST_Node> param):
    AST_Node_Unary(std::move(param), kStrCast, true, AT_CAST) {}

AST_Node_Binary::AST_Node_Binary(AST_TYPE tp, unique_ptr<AST_Node> l,
                                 unique_ptr<AST_Node> r):
    AST_Node(tp), left(std::move(l)), right(std::move(r)) {
  assert(type == AT_BINARYOP);  // wtf?
}

AST_Node_Binary::AST_Node_Binary(unique_ptr<AST_Node> l, unique_ptr<AST_Node> r,
                                 string_view op, AST_TYPE tp):
    AST_Node(op, tp), left(std::move(l)), right(std::move(r)) {
  assert(type == tp);
}

AST_Node_Ternary::AST_Node_Ternary(unique_ptr<AST_Node> expression,
                                   unique_ptr<AST_Node> exp_true,
                                   unique_ptr<AST_Node> exp_false,
                                   string_view ct):
    AST_Node(ct, AT_TERNARYOP), exp(std::move(expression)),
    left(std::move(exp_true)), right(std::move(exp_false)) {
  assert(type == AT_TERNARYOP);
}

AST_Node_Parameters::AST_Node_Parameters():
    AST_Node(AT_PARAMLIST), func() {}

AST_Node_new::AST_Node_new(const full_type &t, unique_ptr<AST_Node> p,
                           unique_ptr<AST_Node> b):
    AST_Node(AT_NEW), alloc_type(t),
    position(std::move(p)), bound(std::move(b)) {}

AST_Node_new::AST_Node_new():
    AST_Node(AT_NEW), alloc_type(), position(), bound() {}

AST_Node_delete::AST_Node_delete(unique_ptr<AST_Node> param, bool arr):
    AST_Node_Unary(std::move(param), arr? "delete" : "delete[]",
                   true, AT_DELETE),
    array(arr) {}

AST_Node_Subscript::AST_Node_Subscript(unique_ptr<AST_Node> l,
                                       unique_ptr<AST_Node> ind):
    AST_Node(AT_SUBSCRIPT), left(std::move(l)), index(std::move(ind)) {}

AST_Node_Subscript::AST_Node_Subscript():
    AST_Node(AT_SUBSCRIPT), left(nullptr), index(nullptr) {}

AST_Node_TempInst::AST_Node_TempInst(definition_template* d):
    AST_Node(d->name, AT_INSTANTIATE),
    temp(make_unique<AST_Node_Definition>(d, d->name)) {}

AST_Node_TempInst::AST_Node_TempInst(unique_ptr<AST_Node> d, string_view c):
    AST_Node(c, AT_INSTANTIATE), temp(std::move(d)) {}

AST_Node_TempKeyInst::AST_Node_TempKeyInst(definition_template* t,
                                           const arg_key &k):
    AST_Node(t->name, AT_INSTBYKEY), temp(t), key(k) {}

AST_Node_Array::AST_Node_Array(): AST_Node(AT_ARRAY) {}


//===========================================================================================================================
//=: ASTOperator Class :=====================================================================================================
//===========================================================================================================================

void AST_Node            ::operate(ASTOperator *aop, void *param) { aop->operate            (this, param); }
void AST_Node_Definition ::operate(ASTOperator *aop, void *param) { aop->operate_Definition (this, param); }
void AST_Node_Scope      ::operate(ASTOperator *aop, void *param) { aop->operate_Scope      (this, param); }
void AST_Node_Type       ::operate(ASTOperator *aop, void *param) { aop->operate_Type       (this, param); }
void AST_Node_Unary      ::operate(ASTOperator *aop, void *param) { aop->operate_Unary      (this, param); }
void AST_Node_sizeof     ::operate(ASTOperator *aop, void *param) { aop->operate_sizeof     (this, param); }
void AST_Node_Cast       ::operate(ASTOperator *aop, void *param) { aop->operate_Cast       (this, param); }
void AST_Node_Binary     ::operate(ASTOperator *aop, void *param) { aop->operate_Binary     (this, param); }
void AST_Node_Ternary    ::operate(ASTOperator *aop, void *param) { aop->operate_Ternary    (this, param); }
void AST_Node_Parameters ::operate(ASTOperator *aop, void *param) { aop->operate_Parameters (this, param); }
void AST_Node_Array      ::operate(ASTOperator *aop, void *param) { aop->operate_Array      (this, param); }
void AST_Node_new        ::operate(ASTOperator *aop, void *param) { aop->operate_new        (this, param); }
void AST_Node_delete     ::operate(ASTOperator *aop, void *param) { aop->operate_delete     (this, param); }
void AST_Node_Subscript  ::operate(ASTOperator *aop, void *param) { aop->operate_Subscript  (this, param); }
void AST_Node_TempInst   ::operate(ASTOperator *aop, void *param) { aop->operate_TempInst   (this, param); }
void AST_Node_TempKeyInst::operate(ASTOperator *aop, void *param) { aop->operate_TempKeyInst(this, param); }

void AST_Node            ::operate(ConstASTOperator *aop, void *param) const { aop->operate            (this, param); }
void AST_Node_Definition ::operate(ConstASTOperator *aop, void *param) const { aop->operate_Definition (this, param); }
void AST_Node_Scope      ::operate(ConstASTOperator *aop, void *param) const { aop->operate_Scope      (this, param); }
void AST_Node_Type       ::operate(ConstASTOperator *aop, void *param) const { aop->operate_Type       (this, param); }
void AST_Node_Unary      ::operate(ConstASTOperator *aop, void *param) const { aop->operate_Unary      (this, param); }
void AST_Node_sizeof     ::operate(ConstASTOperator *aop, void *param) const { aop->operate_sizeof     (this, param); }
void AST_Node_Cast       ::operate(ConstASTOperator *aop, void *param) const { aop->operate_Cast       (this, param); }
void AST_Node_Binary     ::operate(ConstASTOperator *aop, void *param) const { aop->operate_Binary     (this, param); }
void AST_Node_Ternary    ::operate(ConstASTOperator *aop, void *param) const { aop->operate_Ternary    (this, param); }
void AST_Node_Parameters ::operate(ConstASTOperator *aop, void *param) const { aop->operate_Parameters (this, param); }
void AST_Node_Array      ::operate(ConstASTOperator *aop, void *param) const { aop->operate_Array      (this, param); }
void AST_Node_new        ::operate(ConstASTOperator *aop, void *param) const { aop->operate_new        (this, param); }
void AST_Node_delete     ::operate(ConstASTOperator *aop, void *param) const { aop->operate_delete     (this, param); }
void AST_Node_Subscript  ::operate(ConstASTOperator *aop, void *param) const { aop->operate_Subscript  (this, param); }
void AST_Node_TempInst   ::operate(ConstASTOperator *aop, void *param) const { aop->operate_TempInst   (this, param); }
void AST_Node_TempKeyInst::operate(ConstASTOperator *aop, void *param) const { aop->operate_TempKeyInst(this, param); }

void AST::remap(const remap_set& n, ErrorContext errc) {
  if (root)
    root->remap(n, errc);
}

//===========================================================================================================================
//=: Everything else :=======================================================================================================
//===========================================================================================================================

int AST_Builder::parse_expression(AST *ast, token_t &token,
                                  definition_scope *scope, int precedence) {
  search_scope = scope;
  return !(ast->root = parse_expression(ast, token, precedence));
}

void AST::clear() {
  #ifdef DEBUG_MODE
    expression.clear();
  #endif
  root.reset();
}
bool AST::empty() {
  return !root;
}

void AST::swap(AST& o) {
  o.root.swap(root);
}

AST::AST(): root(nullptr), tt_greater_is_op(true) {}
AST::AST(unique_ptr<AST_Node> r): root(std::move(r)), tt_greater_is_op(true) {}
AST::AST(definition* d):
    root(make_unique<AST_Node_Definition>(d, d->name)),
    tt_greater_is_op(true) {}
AST::AST(value v): tt_greater_is_op(true) {
  root = make_unique<AST_Node>(v.toString(), v.type == VT_STRING? AT_STRLITERAL : AT_DECLITERAL);
}
AST::AST(long v): tt_greater_is_op(true) {
  root = make_unique<AST_Node>(value(v).toString(), AT_DECLITERAL);
}

unique_ptr<AST> AST::create_from_instantiation(definition_template* temp, const arg_key &key) {
  return make_unique<AST>(make_unique<AST_Node_TempKeyInst>(temp, key));
}
unique_ptr<AST> AST::create_from_access(definition_scope* scope, string_view id, string_view scope_op) {
  return make_unique<AST>(
    make_unique<AST_Node_Scope>(
        make_unique<AST_Node_Definition>(
            scope,
            scope->name),
        make_unique<AST_Node>(
            id,
            AT_IDENTIFIER),
        scope_op)
  );
}

AST_Builder::AST_Builder(context_parser *ctp):
    lex(ctp->lex), herr(ctp->herr), cparse(ctp), search_scope(nullptr) {}

AST parse_expression(lexer *lex, ErrorHandler *herr, token_t *token) {
  Context ctex;
  context_parser ctp(&ctex, lex);
  AST_Builder ab(&ctp);
  AST res;
  token_t tok = lex->get_token();
  ab.parse_expression(&res, tok, nullptr, 0);
  if (token)
    *token = tok;
  else if (tok.type != TT_ENDOFCODE) {
    herr->error(tok, "Extra tokens at end of expression: unhandled %s",
                tok.to_string());
  }
  return res;
}

}  // namespace jdi
