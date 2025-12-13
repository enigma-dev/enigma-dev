/** Copyright (C) 2025
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "clang_parser_bridge.h"
#include "declarator.h"
#include <iostream>
#include <sstream>
#include <cstring>

namespace enigma::parsing {

ClangParserBridge::ClangParserBridge(LanguageFrontend* frontend)
    : frontend_(frontend), index_(nullptr) {
  index_ = clang_createIndex(0, 0);
}

ClangParserBridge::~ClangParserBridge() {
  if (index_) {
    clang_disposeIndex(index_);
  }
}

std::string ClangParserBridge::tokens_to_cpp_code(const std::vector<Token>& tokens) {
  std::ostringstream oss;
  for (const auto& token : tokens) {
    oss << token.content << " ";
  }
  return oss.str();
}

AST::PNode ClangParserBridge::parse_declaration_with_clang(const std::vector<Token>& tokens) {
  std::string code = tokens_to_cpp_code(tokens);
  
  // Check cache
  auto it = cache_.find(code);
  if (it != cache_.end() && it->second.node) {
    // Return a copy of the cached node (would need deep copy implementation)
    // For now, skip caching for declarations
  }

  // Wrap code in a function to make it parseable
  std::string wrapped_code = "void __enigma_parse_wrapper() { " + code + " }";
  
  // Parse with Clang
  const char* args[] = {"-std=c++17", "-xc++"};
  CXUnsavedFile unsaved_file;
  unsaved_file.Filename = "__enigma_temp.cpp";
  unsaved_file.Contents = wrapped_code.c_str();
  unsaved_file.Length = wrapped_code.length();
  
  CXTranslationUnit tu = clang_parseTranslationUnit(
      index_, "__enigma_temp.cpp", args, 2, &unsaved_file, 1,
      CXTranslationUnit_None);
  
  if (!tu) {
    std::cerr << "Failed to parse code with Clang: " << code << std::endl;
    return nullptr;
  }

  // Get the cursor for the translation unit
  CXCursor cursor = clang_getTranslationUnitCursor(tu);
  
  AST::PNode result = nullptr;
  
  // Visit children to find our wrapper function
  clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData client_data) {
    if (clang_getCursorKind(c) == CXCursor_FunctionDecl) {
      CXString name = clang_getCursorSpelling(c);
      std::string func_name = clang_getCString(name);
      clang_disposeString(name);
      
      if (func_name == "__enigma_parse_wrapper") {
        // Visit the function body
        clang_visitChildren(c, [](CXCursor body, CXCursor parent, CXClientData client_data) {
          if (clang_getCursorKind(body) == CXCursor_CompoundStmt) {
            // Visit statements inside the body
            clang_visitChildren(body, [](CXCursor stmt, CXCursor parent, CXClientData client_data) {
              auto* bridge = static_cast<ClangParserBridge*>(client_data);
              auto* result_ptr = static_cast<AST::PNode*>(client_data);
              // TODO: Convert statement to AST node
              return CXChildVisit_Break;
            }, client_data);
          }
          return CXChildVisit_Continue;
        }, client_data);
      }
    }
    return CXChildVisit_Continue;
  }, &result);

  clang_disposeTranslationUnit(tu);
  return result;
}

AST::PNode ClangParserBridge::parse_expression_with_clang(const std::vector<Token>& tokens) {
  std::string code = tokens_to_cpp_code(tokens);
  
  // Wrap code in a function with an expression statement
  std::string wrapped_code = "void __enigma_parse_wrapper() { (" + code + "); }";
  
  // Parse with Clang
  const char* args[] = {"-std=c++17", "-xc++"};
  CXUnsavedFile unsaved_file;
  unsaved_file.Filename = "__enigma_temp.cpp";
  unsaved_file.Contents = wrapped_code.c_str();
  unsaved_file.Length = wrapped_code.length();
  
  CXTranslationUnit tu = clang_parseTranslationUnit(
      index_, "__enigma_temp.cpp", args, 2, &unsaved_file, 1,
      CXTranslationUnit_None);
  
  if (!tu) {
    std::cerr << "Failed to parse expression with Clang: " << code << std::endl;
    return nullptr;
  }

  // Get the cursor for the translation unit
  CXCursor cursor = clang_getTranslationUnitCursor(tu);
  
  AST::PNode result = nullptr;
  
  // Visit to find the expression
  clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData client_data) {
    if (clang_getCursorKind(c) == CXCursor_FunctionDecl) {
      clang_visitChildren(c, [](CXCursor body, CXCursor parent, CXClientData client_data) {
        if (clang_getCursorKind(body) == CXCursor_CompoundStmt) {
          clang_visitChildren(body, [](CXCursor stmt, CXCursor parent, CXClientData client_data) {
            auto* result_ptr = static_cast<AST::PNode*>(client_data);
            auto* bridge = reinterpret_cast<ClangParserBridge*>(
                reinterpret_cast<void**>(client_data)[1]);
            
            // Convert the statement to an expression
            *result_ptr = bridge->convert_cursor_to_ast(stmt);
            return CXChildVisit_Break;
          }, client_data);
        }
        return CXChildVisit_Continue;
      }, client_data);
    }
    return CXChildVisit_Continue;
  }, &result);

  clang_disposeTranslationUnit(tu);
  return result;
}

bool ClangParserBridge::parse_type_with_clang(const std::vector<Token>& tokens, FullType& out_type) {
  std::string code = tokens_to_cpp_code(tokens);
  
  // Wrap as a typedef to parse the type
  std::string wrapped_code = "typedef " + code + " __enigma_type_alias;";
  
  // Parse with Clang
  const char* args[] = {"-std=c++17", "-xc++"};
  CXUnsavedFile unsaved_file;
  unsaved_file.Filename = "__enigma_temp.cpp";
  unsaved_file.Contents = wrapped_code.c_str();
  unsaved_file.Length = wrapped_code.length();
  
  CXTranslationUnit tu = clang_parseTranslationUnit(
      index_, "__enigma_temp.cpp", args, 2, &unsaved_file, 1,
      CXTranslationUnit_None);
  
  if (!tu) {
    std::cerr << "Failed to parse type with Clang: " << code << std::endl;
    return false;
  }

  CXCursor cursor = clang_getTranslationUnitCursor(tu);
  
  bool success = false;
  
  // Visit to find the typedef
  clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData client_data) {
    if (clang_getCursorKind(c) == CXCursor_TypedefDecl) {
      auto* data = static_cast<std::pair<ClangParserBridge*, FullType*>*>(client_data);
      auto* bridge = data->first;
      auto* out_type = data->second;
      
      CXType underlying_type = clang_getTypedefDeclUnderlyingType(c);
      *static_cast<bool*>(reinterpret_cast<void*>(
          reinterpret_cast<char*>(client_data) + sizeof(void*)*2)) = 
          bridge->convert_type_to_fulltype(underlying_type, c, *out_type);
      
      return CXChildVisit_Break;
    }
    return CXChildVisit_Continue;
  }, &success);

  clang_disposeTranslationUnit(tu);
  return success;
}

AST::PNode ClangParserBridge::convert_cursor_to_ast(CXCursor cursor) {
  CXCursorKind kind = clang_getCursorKind(cursor);
  
  switch (kind) {
    case CXCursor_BinaryOperator:
      return convert_binary_operator(cursor);
    
    case CXCursor_UnaryOperator:
      return convert_unary_operator(cursor);
    
    case CXCursor_ArraySubscriptExpr:
      return convert_array_subscript(cursor);
    
    case CXCursor_DeclRefExpr: {
      CXString name = clang_getCursorSpelling(cursor);
      std::string identifier = clang_getCString(name);
      clang_disposeString(name);
      
      Token tok;
      tok.type = TT_IDENTIFIER;
      tok.content = identifier;
      return std::make_unique<AST::Literal>(tok);
    }
    
    case CXCursor_IntegerLiteral:
    case CXCursor_FloatingLiteral:
    case CXCursor_StringLiteral: {
      CXSourceRange range = clang_getCursorExtent(cursor);
      CXToken* tokens = nullptr;
      unsigned num_tokens = 0;
      CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);
      clang_tokenize(tu, range, &tokens, &num_tokens);
      
      Token tok;
      if (num_tokens > 0) {
        CXString token_str = clang_getTokenSpelling(tu, tokens[0]);
        tok.content = clang_getCString(token_str);
        clang_disposeString(token_str);
        
        if (kind == CXCursor_IntegerLiteral) {
          tok.type = TT_DECLITERAL;
        } else if (kind == CXCursor_FloatingLiteral) {
          tok.type = TT_DECLITERAL;
        } else {
          tok.type = TT_STRINGLIT;
        }
      }
      
      clang_disposeTokens(tu, tokens, num_tokens);
      return std::make_unique<AST::Literal>(tok);
    }
    
    case CXCursor_ParenExpr: {
      // Extract the inner expression
      AST::PNode inner = nullptr;
      auto visitor_data = std::make_pair(this, &inner);
      clang_visitChildren(cursor, [](CXCursor c, CXCursor /*parent*/, CXClientData client_data) {
        auto* bridge_and_result = static_cast<std::pair<ClangParserBridge*, AST::PNode*>*>(client_data);
        *bridge_and_result->second = bridge_and_result->first->convert_cursor_to_ast(c);
        return CXChildVisit_Break;
      }, &visitor_data);
      
      return std::make_unique<AST::Parenthetical>(std::move(inner));
    }
    
    default:
      std::cerr << "Unhandled cursor kind in convert_cursor_to_ast: " << kind << std::endl;
      return nullptr;
  }
}

bool ClangParserBridge::convert_type_to_fulltype(CXType type, CXCursor /*cursor*/, FullType& out_type) {
  // Extract type flags (currently unused in ENIGMA)
  out_type.flags = 0;
  
  // TODO: Handle const/volatile qualifiers if needed
  // The current ENIGMA type system doesn't use flags for qualifiers
  
  // Get the base type (strip qualifiers and pointers)
  CXType base_type = type;
  while (base_type.kind == CXType_Pointer || 
         base_type.kind == CXType_ConstantArray ||
         base_type.kind == CXType_IncompleteArray) {
    CXType pointee = clang_getPointeeType(base_type);
    if (pointee.kind != CXType_Invalid) {
      base_type = pointee;
    } else {
      pointee = clang_getArrayElementType(base_type);
      if (pointee.kind != CXType_Invalid) {
        base_type = pointee;
      } else {
        break;
      }
    }
  }
  
  // Look up the definition for the base type
  CXString type_name = clang_getTypeSpelling(base_type);
  std::string type_str = clang_getCString(type_name);
  clang_disposeString(type_name);
  
  // Use the frontend to find the type definition
  out_type.def = frontend_->find_typename(type_str);
  
  // Extract declarator components
  extract_declarator_components(type, out_type.decl);
  
  return out_type.def != nullptr;
}

void ClangParserBridge::extract_declarator_components(CXType type, Declarator& decl) {
  decl.components.clear();
  decl.has_nested_declarator = false;
  
  // Traverse the type to extract components
  CXType current_type = type;
  
  while (current_type.kind != CXType_Invalid) {
    switch (current_type.kind) {
      case CXType_Pointer: {
        PointerNode ptr_node;
        ptr_node.is_const = false;
        ptr_node.is_volatile = false;
        ptr_node.class_def = nullptr;
        decl.components.push_back(DeclaratorNode{nullptr, ptr_node});
        current_type = clang_getPointeeType(current_type);
        break;
      }
      
      case CXType_ConstantArray: {
        long long size = clang_getArraySize(current_type);
        ArrayBoundNode array_node;
        array_node.size = static_cast<std::size_t>(size);
        array_node.outside_nested = false;
        decl.components.push_back(DeclaratorNode{array_node});
        current_type = clang_getArrayElementType(current_type);
        break;
      }
      
      case CXType_IncompleteArray: {
        ArrayBoundNode array_node;
        array_node.size = 0;  // Size 0 for incomplete array
        array_node.outside_nested = false;
        decl.components.push_back(DeclaratorNode{array_node});
        current_type = clang_getArrayElementType(current_type);
        break;
      }
      
      case CXType_FunctionProto:
      case CXType_FunctionNoProto: {
        // TODO: Extract function parameters
        FunctionParameterNode func_node;
        func_node.parameters = FunctionParameterNode::ParameterList{};
        func_node.outside_nested = false;
        decl.components.push_back(DeclaratorNode{std::move(func_node)});
        current_type.kind = CXType_Invalid;  // Stop traversal
        break;
      }
      
      default:
        // Base type reached
        current_type.kind = CXType_Invalid;
        break;
    }
  }
}

AST::PNode ClangParserBridge::convert_binary_operator(CXCursor cursor) {
  // Extract left and right operands
  AST::PNode left = nullptr, right = nullptr;
  int child_index = 0;
  
  auto visitor_data = std::make_tuple(this, &left, &right, &child_index);
  clang_visitChildren(cursor, [](CXCursor c, CXCursor /*parent*/, CXClientData client_data) {
    auto* data = static_cast<std::tuple<ClangParserBridge*, AST::PNode*, AST::PNode*, int*>*>(client_data);
    auto* bridge = std::get<0>(*data);
    auto* left = std::get<1>(*data);
    auto* right = std::get<2>(*data);
    auto* index = std::get<3>(*data);
    
    if (*index == 0) {
      *left = bridge->convert_cursor_to_ast(c);
    } else if (*index == 1) {
      *right = bridge->convert_cursor_to_ast(c);
    }
    (*index)++;
    
    return CXChildVisit_Continue;
  }, &visitor_data);
  
  TokenType op_type = get_binary_operator_token_type(cursor);
  AST::Operation op(op_type, "");  // TODO: Get actual operator string
  
  return std::make_unique<AST::BinaryExpression>(std::move(left), std::move(right), op);
}

AST::PNode ClangParserBridge::convert_unary_operator(CXCursor cursor) {
  // Extract operand
  AST::PNode operand = nullptr;
  
  auto visitor_data = std::make_pair(this, &operand);
  clang_visitChildren(cursor, [](CXCursor c, CXCursor /*parent*/, CXClientData client_data) {
    auto* data = static_cast<std::pair<ClangParserBridge*, AST::PNode*>*>(client_data);
    *data->second = data->first->convert_cursor_to_ast(c);
    return CXChildVisit_Break;
  }, &visitor_data);
  
  TokenType op_type = get_unary_operator_token_type(cursor);
  AST::Operation op(op_type, "");  // TODO: Get actual operator string
  
  // TODO: Determine if prefix or postfix
  return std::make_unique<AST::UnaryPrefixExpression>(std::move(operand), op);
}

AST::PNode ClangParserBridge::convert_array_subscript(CXCursor cursor) {
  // Array subscript is represented as a binary expression with TT_BEGINBRACKET
  AST::PNode array = nullptr, index = nullptr;
  int child_index = 0;
  
  auto visitor_data = std::make_tuple(this, &array, &index, &child_index);
  clang_visitChildren(cursor, [](CXCursor c, CXCursor /*parent*/, CXClientData client_data) {
    auto* data = static_cast<std::tuple<ClangParserBridge*, AST::PNode*, AST::PNode*, int*>*>(client_data);
    auto* bridge = std::get<0>(*data);
    auto* array = std::get<1>(*data);
    auto* index = std::get<2>(*data);
    auto* child_idx = std::get<3>(*data);
    
    if (*child_idx == 0) {
      *array = bridge->convert_cursor_to_ast(c);
    } else if (*child_idx == 1) {
      *index = bridge->convert_cursor_to_ast(c);
    }
    (*child_idx)++;
    
    return CXChildVisit_Continue;
  }, &visitor_data);
  
  AST::Operation op(TT_BEGINBRACKET, "[");
  return std::make_unique<AST::BinaryExpression>(std::move(array), std::move(index), op);
}

TokenType ClangParserBridge::get_binary_operator_token_type(CXCursor /*cursor*/) {
  // TODO: Extract actual operator from cursor
  // For now, return a default
  return TT_PLUS;
}

TokenType ClangParserBridge::get_unary_operator_token_type(CXCursor /*cursor*/) {
  // TODO: Extract actual operator from cursor
  return TT_STAR;  // Dereference operator as default
}

} // namespace enigma::parsing
