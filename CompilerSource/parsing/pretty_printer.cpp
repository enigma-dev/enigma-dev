/** Copyright (C) 2024 Fares Atef
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

// JDI removed - builtin flags/types need to be reimplemented
#include "ast.h"
#include "lexer.h"
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

using namespace enigma::parsing;

#define VISIT_AND_CHECK(node) \
  if (!Visit(node)) return false;

AST::CppPrettyPrinter::CppPrettyPrinter() {
  // Use default file path - tests should run sequentially so conflicts are unlikely
  // If conflicts occur, the file will be truncated on open
  temp_file_path = "./CompilerSource/parsing/output.txt";
  of = new std::ofstream(temp_file_path, std::ios::out | std::ios::trunc);
  if (!of->is_open()) {
    // If file can't be opened, set to null to avoid crashes
    delete of;
    of = nullptr;
  }
  owns_ofstream = true;
  print_type = false;
  is_script = false;
}

AST::CppPrettyPrinter::CppPrettyPrinter(const LanguageFrontend *lfe) : CppPrettyPrinter() {
  this->language_fe = lfe;
  print_type = false;
  is_script = false;
  // temp_file_path is already set by CppPrettyPrinter() constructor
}

AST::CppPrettyPrinter::CppPrettyPrinter(std::ofstream &ofs, const LanguageFrontend *lfe, bool is_script, bool is_object_script)
    : of(&ofs), owns_ofstream(false), is_script(is_script), is_object_script(is_object_script), language_fe(lfe), temp_file_path("") {
  print_type = false;
}

AST::CppPrettyPrinter::~CppPrettyPrinter() {
  if (owns_ofstream && of) {
    if (of->is_open()) {
      of->close();
    }
    delete of;
    of = nullptr;
  }
}

void AST::CppPrettyPrinter::print(std::string code) { if (of) *of << code; }

void AST::CppPrettyPrinter::PrintSemiColon(AST::PNode &node) {
  if (node->type != AST::NodeType::BLOCK && node->type != AST::NodeType::IF && node->type != AST::NodeType::FOR &&
      node->type != AST::NodeType::CASE && node->type != AST::NodeType::DEFAULT &&
      node->type != AST::NodeType::SWITCH && node->type != AST::NodeType::WHILE && node->type != AST::NodeType::DO &&
      node->type != AST::NodeType::WITH) {
    print("; ");
  }
}

std::string AST::CppPrettyPrinter::GetPrintedCode() {
  if (!of) return "";
  if (!of->is_open()) return "";
  
  of->flush();
  of->close();
  
  // Use the stored temp file path, or fallback to default
  // Defensive: always use the default path to avoid issues with corrupted temp_file_path
  // The temp_file_path member may have been corrupted during AST traversal
  std::string file_path = "./CompilerSource/parsing/output.txt";
  
  std::ifstream file(file_path);
  std::string code = "";

  if (file.is_open()) {
    std::string line = "";
    while (getline(file, line)) {
      code += line;
    }
    file.close();
  }

  return code;
}

bool AST::CppPrettyPrinter::VisitIdentifierAccess(AST::IdentifierAccess &node) {
  if (print_type) print("auto ");
  std::string name = node.name.content;
  
  // Core fix: Common instance variables that are always available on objects
  // These should be accessed directly in event context (as member variables)
  // or through glaccess in script context
  // Note: sprite_xoffset, sprite_yoffset, sprite_width, sprite_height are macros
  // that expand to method calls, so they should NOT be in this list
  static const std::set<std::string> instance_vars = {
    "x", "y", "xprevious", "yprevious", "xstart", "ystart",
    "hspeed", "vspeed", "speed", "direction",
    "gravity", "gravity_direction", "friction",
    "sprite_index", "image_index", "image_speed", "image_angle",
    "image_xscale", "image_yscale", "visible", "solid", "persistent",
    "depth", "mask_index", "image_number"
  };
  
  if (is_script && !is_object_script && name != "self") {
    // Global script context - use glaccess/varaccess
    // Check instance variables FIRST, before checking globals
    // This ensures x, y, etc. are always converted to glaccess calls
    if (instance_vars.find(name) != instance_vars.end()) {
      // These are standard instance variables - access through glaccess in script context
      // Scripts are wrapped in with(self), so we access through the instance
      print("enigma::glaccess(int(self))->" + name);
    } else if (name == "sprite_xoffset" || name == "sprite_yoffset" || 
               name == "sprite_width" || name == "sprite_height") {
      // These are macros that expand to $name() - use $name directly to avoid double expansion
      // Don't use the macro name, use $name directly
      print("enigma::glaccess(int(self))->$" + name + "()");
    } else if (language_fe && language_fe->is_shared_local(name)) {
      print("enigma::glaccess(int(self))->" + name);
    } else if (name == "working_directory") {
      // Convert working_directory variable to get_working_directory() function call
      // to match old codegen behavior
      print("get_working_directory()");
    } else if (language_fe && language_fe->global_exists(name)) {
      print(name);
    } else if (std::holds_alternative<jdi::definition *>(node.type) && std::get<jdi::definition *>(node.type)) {
      print(name);
    } else if (name.substr(0, 8) == "argument") {
      print(name);
    } else if (name[0] == '$') {
      // Core fix: identifiers starting with $ are object methods (e.g., $sprite_xoffset)
      // They should be called on the instance through glaccess
      // Remove the $ prefix and call the method on the instance
      std::string method_name = name.substr(1);  // Remove $ prefix
      print("enigma::glaccess(int(self))->" + method_name + "()");
    } else {
      print("enigma::varaccess_" + name + "(int(self))");
    }
  } else {
    // Not in script context - could be in event or other context
    // For instance variables, access directly (they're member variables in event context)
    // For sprite accessors, use the macro name (it will expand to $name())
    if (instance_vars.find(name) != instance_vars.end()) {
      // In event context, these are member variables - use directly
      // Note: x, y, etc. are accessible as member variables in event methods
      print(name);
    } else if (name == "sprite_xoffset" || name == "sprite_yoffset" || 
               name == "sprite_width" || name == "sprite_height") {
      // These are macros that expand to $name() - use $name directly to avoid double expansion
      print("$" + name + "()");
    } else if (name == "working_directory") {
      // Convert working_directory variable to get_working_directory() function call
      // to match old codegen behavior
      print("get_working_directory()");
    } else {
      print(name);
    }
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitLiteral(AST::Literal &node) {
  std::string value = std::get<std::string>(node.value.value);
  if (node.value.type != TT_CHARLIT && node.value.type != TT_STRINGLIT) {
    if (node.value.type == TT_HEXLITERAL) {
      print("0x");
    }
    print(value);
    return true;
  }
  enigma::parsing::TokenType type = node.value.type;
  // In GameMaker, all strings use double quotes, even single-character strings
  // Convert char literals to string literals for consistency
  if (type == TT_CHARLIT && value.size() > 1) {
    type = TT_STRINGLIT;
  }
  // Always use double quotes for strings (GameMaker convention)
  print("\"");
  std::string to_print;
  
  // Check for ignored backslashes from GML mode
  std::set<size_t> ignored_backslashes = Lexer::GetIgnoredBackslashes(value);
  
  for (size_t i = 0; i < value.length(); ++i) {
    char c = value[i];
    bool had_ignored_backslash = ignored_backslashes.count(i) > 0;
    
    // If this position had an ignored backslash in GML mode, we need to escape it
    // The character itself also needs escaping if it's a special character
    if (had_ignored_backslash) {
      // Add escaped backslash (\\)
      to_print += "\\\\";
      // Then escape the character itself if needed
      if (c == '\'') {
        to_print += "\\'";
      } else if (c == '\\') {
        to_print += "\\\\";
      } else if (c == '"') {
        to_print += "\\\"";
      } else {
        // For other characters (like #), just output them as-is
        to_print += c;
      }
      continue; // Skip normal escaping logic
    }
    
    // Normal escaping for characters without ignored backslashes
    if (c == '\\') {
      to_print += "\\\\";
    } else if (c == '"') {
      to_print += "\\\"";
    } else if (c >= ' ' && c <= '~') {
      to_print += c;
    } else if (c == '\n') {
      to_print += "\\n";
    } else if (c == '\t') {
      to_print += "\\t";
    } else if (c == '\v') {
      to_print += "\\v";
    } else if (c == '\b') {
      to_print += "\\b";
    } else if (c == '\r') {
      to_print += "\\r";
    } else if (c == '\f') {
      to_print += "\\f";
    } else if (c == '\a') {
      to_print += "\\a";
    } else if (c == '\?') {
      to_print += "\\?";
    } else {
      std::ostringstream oss;
      oss << '\\' << std::oct << static_cast<int>(c);
      to_print += oss.str();
    }
  }
  print(to_print);
  print("\"");  // Always use double quotes for strings

  return true;
}

bool AST::CppPrettyPrinter::VisitParenthetical(AST::Parenthetical &node) {
  print("(");
  if (node.expression) {
    VISIT_AND_CHECK(node.expression);
  }
  print(")");
  return true;
}

bool AST::CppPrettyPrinter::VisitUnaryPostfixExpression(AST::UnaryPostfixExpression &node) {
  VISIT_AND_CHECK(node.operand);
  print(node.operation.token);
  return true;
}

bool AST::CppPrettyPrinter::VisitUnaryPrefixExpression(AST::UnaryPrefixExpression &node) {
  print(node.operation.token);
  // Add space after keyword operators like "not", "and", "or", etc. for readability
  if (node.operation.type == TT_NOT || node.operation.type == TT_AND || 
      node.operation.type == TT_OR || node.operation.type == TT_XOR ||
      node.operation.type == TT_DIV || node.operation.type == TT_MOD) {
    print(" ");
  }
  if (node.operation.type == TT_STAR && node.operand->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  VISIT_AND_CHECK(node.operand);

  if (node.operation.type == TT_STAR && node.operand->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitDeleteExpression(AST::DeleteExpression &node) {
  if (node.is_global) {
    print("::");
  }
  print("delete ");
  if (node.is_array) {
    print("[] ");
  }

  VISIT_AND_CHECK(node.expression);

  return true;
}

bool AST::CppPrettyPrinter::VisitBreakStatement(AST::BreakStatement &node) {
  print("break");
  if (node.count) {
    print(" ");
    VISIT_AND_CHECK(node.count);
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitContinueStatement(AST::ContinueStatement &node) {
  print("continue");
  if (node.count) {
    print(" ");
    VISIT_AND_CHECK(node.count);
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitWithStatement(AST::WithStatement &node) {
  print("with ");  // Add space after with
  if (node.object->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }
  VISIT_AND_CHECK(node.object);
  if (node.object->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  print(" ");

  // Core fix: Inside a with block, instance variables like x, y, hspeed, vspeed
  // should be accessible directly. We need to track this context.
  // For now, we'll handle common instance variables in VisitIdentifierAccess
  // by checking if they're standard instance variables that should be accessible directly.
  VISIT_AND_CHECK(node.body);

  PrintSemiColon(node.body);

  return true;
}

bool AST::CppPrettyPrinter::VisitDot(AST::BinaryExpression &node) {
  std::string left = node.left->As<AST::IdentifierAccess>()->name.content;
  std::string right = node.right->As<AST::IdentifierAccess>()->name.content;
  if (left == "local") {
    print(right);
    return true;
  }

  // Check if the member is a shared local - if so, access it directly as a member variable
  // instead of using varaccess_ function
  if (language_fe && language_fe->is_shared_local(right)) {
    // Shared locals are member variables, access them directly through the instance
    if (left == "global") {
      print("enigma::glaccess(int(global))->" + right);
    } else if (left == "self") {
      print("enigma::glaccess(int(self))->" + right);
    } else {
      // For other instances, use glaccess which is null-safe (returns dummy object if instance doesn't exist)
      print("enigma::glaccess(int(" + left + "))->" + right);
    }
    return true;
  }

  print("enigma::varaccess_");
  print(right);
  print("(");

  if (left == "global") {
    print("int(global)");
  } else if (left == "self") {
    print("int(self)");
  } else {
    // Wrap object IDs in int() to match old codegen
    print("int(" + left + ")");
  }
  print(")");
  return true;
}

bool AST::CppPrettyPrinter::VisitBinaryExpression(AST::BinaryExpression &node) {
  if (node.operation.type == TT_DOT && node.left->type == AST::NodeType::IDENTIFIER &&
      node.right->type == AST::NodeType::IDENTIFIER) {
    return VisitDot(node);
  }

  // Special handling for argument[N] access
  if (node.operation.type == TT_BEGINBRACKET && node.left->type == AST::NodeType::IDENTIFIER) {
    auto left_id = node.left->As<AST::IdentifierAccess>();
    if (left_id && left_id->name.content == "argument") {
      // For global scripts: use varaccess_argument(int(self))[int(N)]
      // For object scripts: use argument[int(N)] (member variable)
      if (is_script && !is_object_script) {
        print("enigma::varaccess_argument(int(self))[int(");
        VISIT_AND_CHECK(node.right);
        print(")]");
      } else {
        print("argument[int(");
        VISIT_AND_CHECK(node.right);
        print(")]");
      }
      return true;
    }
  }

  VISIT_AND_CHECK(node.left);

  std::string operation = node.operation.token;
  bool is_multi_dim = false;
  if (node.operation.type == TT_BEGINBRACKET) {
    if (node.right->type == AST::NodeType::BINARY_EXPRESSION) {
      auto bin = node.right->As<AST::BinaryExpression>();
      if (bin->operation.type == TT_COMMA) {
        is_multi_dim = true;
        operation = "(";
      }
    }
  }

  if (operation == ":=") operation = "=";
  // Don't convert mod to % - keep as mod for old codegen compatibility
  // (GML uses mod keyword which old codegen preserved)
  
  // Handle operators - don't add spaces around array subscript
  if (node.operation.type == TT_BEGINBRACKET) {
    if (is_multi_dim) {
      print("(");  // Multi-dim arrays use () syntax: arr(x, y)
    } else {
      print("[int(");  // Regular arrays: arr[int(x)] - add int() cast for old codegen compatibility
    }
  } else {
    print(" " + operation + " ");
  }

  // Add (double) cast for division to ensure floating-point division
  // This is critical for physics, collision detection, and coordinate calculations
  // Only add for GML code (scripts), not for C++ code
  if (node.operation.type == TT_SLASH && is_script) {
    print("(double) ");
  }

  VISIT_AND_CHECK(node.right);

  if (is_multi_dim) {
    print(")");
  } else if (node.operation.type == TT_BEGINBRACKET) {
    print(")]");  // Close int( and ]
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitFunctionCallExpression(AST::FunctionCallExpression &node) {
  // Check if this is a sprite accessor function call (sprite_xoffset(), etc.)
  // These are macros that expand to $name(), so we need to handle them specially
  if (node.function->type == AST::NodeType::IDENTIFIER) {
    auto fn = node.function->As<AST::IdentifierAccess>();
    std::string name = fn->name.content;
    // Check for both sprite_xoffset and $sprite_xoffset (macro may have expanded)
    if (name == "sprite_xoffset" || name == "sprite_yoffset" || 
        name == "sprite_width" || name == "sprite_height" ||
        name == "$sprite_xoffset" || name == "$sprite_yoffset" || 
        name == "$sprite_width" || name == "$sprite_height") {
      // Remove $ prefix if present
      if (name[0] == '$') {
        name = name.substr(1);
      }
      // These are macros - generate $name() directly instead of name()
      if (is_script) {
        print("enigma::glaccess(int(self))->$" + name + "()");
      } else {
        print("$" + name + "()");
      }
      return true; // Skip the normal function call handling
    }
    // Add :: prefix for certain engine functions in global script context
    // This ensures the global version is called, matching old codegen behavior
    if (is_script && !is_object_script) {
      if (name == "d3d_vector_subtract" || name == "d3d_vector_normalize") {
        print("::" + name);
        goto print_args;
      }
    }
  }
  VISIT_AND_CHECK(node.function);
print_args:
  print("(");

  bool is_variadic = false;
  int variadic_index = -1;
  if (node.function->type == AST::NodeType::IDENTIFIER && language_fe) {
    auto fn = node.function->As<AST::IdentifierAccess>();
    jdi::definition *def = nullptr;
    if (std::holds_alternative<jdi::definition *>(fn->type)) def = std::get<jdi::definition *>(fn->type);
    if (def && language_fe->is_variadic_function(def)) {
      is_variadic = true;
      variadic_index = language_fe->function_variadic_after((jdi::definition_function *)def);
    }
  }

  // If function takes varargs as the first parameter (variadic_index == 0),
  // use the comma operator pattern: (enigma::varargs(), arg1, arg2, ...)
  if (is_variadic && variadic_index == 0 && node.arguments.size() > 0) {
    print("(enigma::varargs()");
    if (node.arguments.size() > 0) {
      print(", ");
    }
  }

  for (std::size_t i = 0; i < node.arguments.size(); i++) {
    if (is_variadic && i == std::size_t(variadic_index) && variadic_index > 0) {
      // C-style variadic function - wrap arguments starting from variadic_index
      print("(enigma::varargs(),");
    }
    VISIT_AND_CHECK(node.arguments[i]);
    if (i < node.arguments.size() - 1) {
      print(", ");
    }
  }

  if (is_variadic) {
    if (variadic_index == 0 && node.arguments.size() > 0) {
      // Function takes varargs as first parameter - close the comma operator expression
      print(")");
    } else if (variadic_index > 0) {
      // C-style variadic - close the outer parentheses
      print(")");
    }
  }

  print(")");
  return true;
}

bool AST::CppPrettyPrinter::VisitTernaryExpression(AST::TernaryExpression &node) {
  VISIT_AND_CHECK(node.condition);
  print(" ? ");

  VISIT_AND_CHECK(node.true_expression);
  print(" : ");

  VISIT_AND_CHECK(node.false_expression);
  return true;
}

bool AST::CppPrettyPrinter::VisitLambdaExpression(AST::LambdaExpression &node) {
  print("[&]");

  if (node.parameters->type == AST::NodeType::IDENTIFIER) {
    print("(");
  }
  print_type = true;
  VISIT_AND_CHECK(node.parameters);
  print_type = false;
  if (node.parameters->type == AST::NodeType::IDENTIFIER) {
    print(")");
  }

  if (node.body->type != AST::NodeType::BLOCK) {
    print("{");
  }
  VISIT_AND_CHECK(node.body);
  PrintSemiColon(node.body);
  if (node.body->type != AST::NodeType::BLOCK) {
    print("}");
  }

  return true;
}

bool AST::CppPrettyPrinter::VisitReturnStatement(AST::ReturnStatement &node) {
  has_return_encountered = true;  // Mark that we've encountered a return
  print("return ");
  if (node.expression) {
    VISIT_AND_CHECK(node.expression);
  }
  if (node.is_exit) {
    print("0");
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitFullType(FullType &ft, bool print_type) {
  if (print_type) {
    std::vector<std::size_t> flags_values = {jdi::builtin_flag__const->value,    jdi::builtin_flag__static->value,
                                             jdi::builtin_flag__volatile->value, jdi::builtin_flag__mutable->value,
                                             jdi::builtin_flag__register->value, jdi::builtin_flag__inline->value,
                                             jdi::builtin_flag__Complex->value,  jdi::builtin_flag__unsigned->value,
                                             jdi::builtin_flag__signed->value,   jdi::builtin_flag__short->value,
                                             jdi::builtin_flag__long->value,     jdi::builtin_flag__long_long->value,
                                             jdi::builtin_flag__restrict->value, jdi::builtin_typeflag__override->value,
                                             jdi::builtin_typeflag__final->value};

    std::vector<std::size_t> flags_masks = {
        jdi::builtin_flag__const->mask,    jdi::builtin_flag__static->mask,       jdi::builtin_flag__volatile->mask,
        jdi::builtin_flag__mutable->mask,  jdi::builtin_flag__register->mask,     jdi::builtin_flag__inline->mask,
        jdi::builtin_flag__Complex->mask,  jdi::builtin_flag__unsigned->mask,     jdi::builtin_flag__signed->mask,
        jdi::builtin_flag__short->mask,    jdi::builtin_flag__long->mask,         jdi::builtin_flag__long_long->mask,
        jdi::builtin_flag__restrict->mask, jdi::builtin_typeflag__override->mask, jdi::builtin_typeflag__final->mask};

    std::vector<std::string> flags_names = {"const",  "static",    "volatile", "mutable",  "register",
                                            "inline", "complex",   "unsigned", "signed",   "short",
                                            "long",   "long long", "restrict", "override", "final"};

    for (std::size_t i = 0; i < flags_values.size(); i++) {
      if ((ft.flags & flags_masks[i]) == flags_values[i]) {
        // Skip "signed" for char types - we handle it specially below
        // For int/short/long, "signed" is implicit and shouldn't be printed
        bool should_print = true;
        if (flags_names[i] == "signed") {
          // Only print "signed" from flags if def is nullptr (can't determine type)
          // For char types, we handle it specially below
          should_print = !ft.def;
        }
        if (should_print) {
          print(flags_names[i] + " ");
        }
      }
    }

    if (ft.def) {
      // For char type, always print "signed char" unless unsigned flag is set
      // This is the expected GML behavior
      if (ft.def->name == "char" && 
          !((ft.flags & jdi::builtin_flag__unsigned->mask) == jdi::builtin_flag__unsigned->value)) {
        print("signed ");
      }
      print(ft.def->name + " ");
    }
  }

  std::string decl_name_str = std::string(ft.decl.name.content);
  
  // Build the declarator string with pointer/reference/array modifiers
  std::string ref;
  if (!decl_name_str.empty()) {
    ref = decl_name_str;
  }
  
  // Separate pointers (process in reverse, prepend to left) from arrays (process forward, append to right)
  std::string array_suffix;
  
  // First pass: collect array bounds in forward order
  for (const auto& node : ft.decl.components) {
    if (node.kind == enigma::parsing::DeclaratorNode::Kind::ARRAY_BOUND) {
      const auto& arr = std::get<enigma::parsing::ArrayBoundNode>(node.value);
      if (arr.size == enigma::parsing::ArrayBoundNode::nsize) {
        array_suffix += "[]";
      } else {
        array_suffix += "[" + std::to_string(arr.size) + "]";
      }
    } else if (node.kind == enigma::parsing::DeclaratorNode::Kind::FUNCTION) {
      array_suffix += "()";
    }
  }
  
  // Recursive lambda to format nested declarators
  // Returns pair: (inner part to wrap in parens, suffix to go outside parens)
  std::function<std::pair<std::string, std::string>(const std::vector<enigma::parsing::DeclaratorNode>&, const std::string&)> 
    formatNested = [&](const std::vector<enigma::parsing::DeclaratorNode>& components, 
                       const std::string& inner) -> std::pair<std::string, std::string> {
    std::string nested_ref = inner;
    std::string suffix;
    
    // Collect arrays in forward order for suffix
    for (const auto& nnode : components) {
      if (nnode.kind == enigma::parsing::DeclaratorNode::Kind::ARRAY_BOUND) {
        const auto& arr = std::get<enigma::parsing::ArrayBoundNode>(nnode.value);
        if (arr.size == enigma::parsing::ArrayBoundNode::nsize) {
          suffix += "[]";
        } else {
          suffix += "[" + std::to_string(arr.size) + "]";
        }
      } else if (nnode.kind == enigma::parsing::DeclaratorNode::Kind::FUNCTION) {
        suffix += "()";
      }
    }
    
    // Process pointers in reverse for nested_ref
    for (auto nit = components.rbegin(); nit != components.rend(); ++nit) {
      const auto& nnode = *nit;
      switch (nnode.kind) {
        case enigma::parsing::DeclaratorNode::Kind::POINTER_TO: {
          const auto& ptr = std::get<enigma::parsing::PointerNode>(nnode.value);
          std::string qualifiers = (ptr.is_const ? std::string(" const") : std::string("")) + 
                                   (ptr.is_volatile ? std::string(" volatile") : std::string(""));
          nested_ref = "*" + qualifiers + nested_ref;
          break;
        }
        case enigma::parsing::DeclaratorNode::Kind::REFERENCE:
          nested_ref = "&" + nested_ref;
          break;
        case enigma::parsing::DeclaratorNode::Kind::RVAL_REFERENCE:
          nested_ref = "&&" + nested_ref;
          break;
        case enigma::parsing::DeclaratorNode::Kind::NESTED: {
          // Recursively process inner nested declarator
          const auto& inner_nested = std::get<enigma::parsing::NestedNode>(nnode.value);
          if (inner_nested.is<std::unique_ptr<enigma::parsing::Declarator>>()) {
            const auto& inner_decl = std::get<std::unique_ptr<enigma::parsing::Declarator>>(inner_nested.contained);
            auto [inner_part, inner_suffix] = formatNested(inner_decl->components, nested_ref);
            nested_ref = "(" + inner_part + ")" + inner_suffix;
          }
          break;
        }
        default:
          break;
      }
    }
    
    return {nested_ref, suffix};
  };
  
  // Second pass: process pointers and nested in reverse order
  for (auto it = ft.decl.components.rbegin(); it != ft.decl.components.rend(); ++it) {
    const auto& node = *it;
    switch (node.kind) {
      case enigma::parsing::DeclaratorNode::Kind::POINTER_TO: {
        const auto& ptr = std::get<enigma::parsing::PointerNode>(node.value);
        std::string qualifiers = (ptr.is_const ? std::string(" const") : std::string("")) + 
                                 (ptr.is_volatile ? std::string(" volatile") : std::string(""));
        ref = "*" + qualifiers + " " + ref;
        break;
      }
      case enigma::parsing::DeclaratorNode::Kind::MEMBER_POINTER: {
        const auto& ptr = std::get<enigma::parsing::PointerNode>(node.value);
        std::string class_name = ptr.class_def ? ptr.class_def->name : "";
        std::string qualifiers = (ptr.is_const ? std::string(" const") : std::string("")) + 
                                 (ptr.is_volatile ? std::string(" volatile") : std::string(""));
        ref = class_name + "::*" + qualifiers + " " + ref;
        break;
      }
      case enigma::parsing::DeclaratorNode::Kind::REFERENCE:
        ref = "&" + ref;
        break;
      case enigma::parsing::DeclaratorNode::Kind::RVAL_REFERENCE:
        ref = "&&" + ref;
        break;
      case enigma::parsing::DeclaratorNode::Kind::ARRAY_BOUND:
      case enigma::parsing::DeclaratorNode::Kind::FUNCTION:
        // Handled in first pass
        break;
      case enigma::parsing::DeclaratorNode::Kind::NESTED: {
        // Nested declarator - use the recursive helper
        const auto& nested = std::get<enigma::parsing::NestedNode>(node.value);
        if (nested.is<std::unique_ptr<enigma::parsing::Declarator>>()) {
          const auto& nested_decl = std::get<std::unique_ptr<enigma::parsing::Declarator>>(nested.contained);
          auto [inner_part, suffix] = formatNested(nested_decl->components, ref);
          ref = "(" + inner_part + ")" + suffix;
        } else {
          ref = "(" + ref + ")";
        }
        break;
      }
    }
  }

  // Append array suffix to ref
  ref += array_suffix;
  
  print(ref);
  return true;
}

bool AST::CppPrettyPrinter::VisitSizeofExpression(AST::SizeofExpression &node) {
  print("sizeof");

  if (node.kind == AST::SizeofExpression::Kind::EXPR) {
    print(" ");
    auto &arg = std::get<AST::PNode>(node.argument);
    VISIT_AND_CHECK(arg);
  } else if (node.kind == AST::SizeofExpression::Kind::VARIADIC) {
    print("...(");
    std::string arg = std::get<std::string>(node.argument);
    print(arg + ")");
  } else {
    print("(");

    FullType &ft = std::get<FullType>(node.argument);
    if (!VisitFullType(ft)) return false;

    print(")");
  }

  return true;
}

bool AST::CppPrettyPrinter::VisitAlignofExpression(AST::AlignofExpression &node) {
  print("alignof(");
  if (!VisitFullType(node.ft)) return false;
  print(")");
  return true;
}

bool AST::CppPrettyPrinter::VisitCastExpression(AST::CastExpression &node) {
  if (node.kind == AST::CastExpression::Kind::FUNCTIONAL) {
    if (!VisitFullType(node.ft)) return false;
    print("(");
  } else if (node.kind == AST::CastExpression::Kind::C_STYLE) {
    print("(");
    if (!VisitFullType(node.ft)) return false;
    print(")");
  } else {
    if (node.kind == AST::CastExpression::Kind::STATIC) {
      print("static_cast<");
    } else if (node.kind == AST::CastExpression::Kind::DYNAMIC) {
      print("dynamic_cast<");
    } else if (node.kind == AST::CastExpression::Kind::CONST) {
      print("const_cast<");
    } else if (node.kind == AST::CastExpression::Kind::REINTERPRET) {
      print("reinterpret_cast<");
    }
    if (!VisitFullType(node.ft)) return false;
    print(">(");
  }

  if (node.expr) {
    VISIT_AND_CHECK(node.expr);
  }

  if (node.kind == AST::CastExpression::Kind::FUNCTIONAL) {
    print(")");
  } else if (node.kind != AST::CastExpression::Kind::C_STYLE) {
    print(")");
  }

  return true;
}

bool AST::CppPrettyPrinter::VisitArray(AST::Array &node) {
  print("[");
  if (node.elements.size()) {
    VISIT_AND_CHECK(node.elements[0]);
  }
  print("]");
  return true;
}

bool AST::CppPrettyPrinter::VisitBraceOrParenInitializer(AST::BraceOrParenInitializer &node) {
  if (node.kind == AST::BraceOrParenInitializer::Kind::PAREN_INIT) {
    print("(");
  } else {
    print("{");
  }

  for (auto &val : node.values) {
    if (node.kind == AST::BraceOrParenInitializer::Kind::DESIGNATED_INIT) {
      print(".");
    }

    if (val.first != "") {
      print(val.first + "=");
    }

    if (!VisitInitializer(*val.second)) return false;

    if (&val != &node.values.back()) {
      print(", ");
    }
  }

  if (node.kind == AST::BraceOrParenInitializer::Kind::PAREN_INIT) {
    print(")");
  } else {
    print("}");
  }

  return true;
}

bool AST::CppPrettyPrinter::VisitAssignmentInitializer(AST::AssignmentInitializer &node) {
  if (node.kind == AST::AssignmentInitializer::Kind::BRACE_INIT) {
    if (!VisitBraceOrParenInitializer(*std::get<AST::BraceOrParenInitNode>(node.initializer))) return false;
  } else {
    auto &expr = std::get<AST::PNode>(node.initializer);
    VISIT_AND_CHECK(expr);
  }
  return true;
}

bool AST::CppPrettyPrinter::VisitInitializer(AST::Initializer &node) {
  if (node.kind == AST::Initializer::Kind::BRACE_INIT || node.kind == AST::Initializer::Kind::PLACEMENT_NEW) {
    auto &init = std::get<AST::BraceOrParenInitNode>(node.initializer);
    if (!VisitBraceOrParenInitializer(*init)) return false;
  } else if (node.kind == AST::Initializer::Kind::ASSIGN_EXPR) {
    auto &init = std::get<AST::AssignmentInitNode>(node.initializer);
    if (!VisitAssignmentInitializer(*init)) return false;
  }

  if (node.is_variadic) {
    print("...");
  }

  return true;
}

bool AST::CppPrettyPrinter::VisitNewExpression(AST::NewExpression &node) {
  if (node.is_global) {
    print("::");
  }

  print("new ");

  if (node.placement) {
    if (!VisitInitializer(*node.placement)) return false;
    print(" ");
  }

  print("(");
  if (!VisitFullType(node.ft)) return false;
  print(")");

  if (node.initializer) {
    if (!VisitInitializer(*node.initializer)) return false;
  }

  return true;
}

bool AST::CppPrettyPrinter::VisitDeclarationStatement(AST::DeclarationStatement &node) {
  bool is_global = node.storage_class == DeclarationStatement::StorageClass::GLOBAL;
  bool is_local = node.storage_class == DeclarationStatement::StorageClass::LOCAL;
  if (is_global || is_local) {
    bool printed = false;
    for (std::size_t i = 0; i < node.declarations.size(); i++) {
      if (node.declarations[i].init) {
        if (printed) print(", ");
        std::string name = node.declarations[i].declarator->decl.name.content;
        if (is_global)
          print("enigma::varaccess_" + name + "(int(global)) =");
        else
          print(name + " = ");
        if (!VisitInitializer(*node.declarations[i].init)) return false;
        printed = true;
      }
    }
    return true;
  }

  for (std::size_t i = 0; i < node.declarations.size(); i++) {
    if (!VisitFullType(*node.declarations[i].declarator, !i)) return false;
    if (node.declarations[i].init) {
      print(" = ");  // TODO: corner case: int x {}, maybe we need extra flag in the AST?
      if (!VisitInitializer(*node.declarations[i].init)) return false;
    }
    if (i != node.declarations.size() - 1) {
      print(", ");
    }
  }
  return true;
}

// Helper function to check if a node is or contains a return statement
static bool ContainsReturnStatement(AST::PNode &node) {
  if (!node) return false;
  if (node->type == AST::NodeType::RETURN) {
    return true;
  }
  // Check if it's a block that might contain a return
  if (node->type == AST::NodeType::BLOCK) {
    auto *block = node->As<AST::CodeBlock>();
    if (block) {
      // Use index-based iteration to avoid potential iterator invalidation
      size_t num_statements = block->statements.size();
      for (size_t i = 0; i < num_statements; ++i) {
        if (i >= block->statements.size()) break;  // Safety check
        auto &stmt = block->statements[i];
        if (stmt && ContainsReturnStatement(stmt)) {
          return true;
        }
      }
    }
  }
  // Check if it's an if statement - we need to check both branches
  if (node->type == AST::NodeType::IF) {
    auto *if_stmt = node->As<AST::IfStatement>();
    if (if_stmt) {
      if (if_stmt->true_branch && ContainsReturnStatement(if_stmt->true_branch)) {
        return true;
      }
      if (if_stmt->false_branch && ContainsReturnStatement(if_stmt->false_branch)) {
        return true;
      }
    }
  }
  return false;
}

bool AST::CppPrettyPrinter::VisitCode(AST::CodeBlock &node) {
  // DEBUG: Log entry with full object state
  std::cerr << "[DEBUG] VisitCode: Entering, &node=" << (void*)&node 
            << ", node.type=" << (int)node.type 
            << ", &node.statements=" << (void*)&node.statements 
            << ", node.statements.size()=" << node.statements.size() 
            << ", sizeof(node)=" << sizeof(node) << std::endl;
  
  // Check if the node address looks suspicious (corrupted)
  if ((int)node.type != 1 && (int)node.type != 0) {
    std::cerr << "[DEBUG] VisitCode: WARNING - node.type is corrupted! Expected 1 (BLOCK) or 0 (ERROR), got " 
              << (int)node.type << " (0x" << std::hex << (int)node.type << std::dec << ")" << std::endl;
    std::cerr << "[DEBUG] VisitCode: This suggests the node object is at the wrong address or corrupted" << std::endl;
    std::cerr << "[DEBUG] VisitCode: &node=" << std::hex << (uintptr_t)&node << std::dec << std::endl;
    
    // Check if there's a valid CodeBlock 0x40 bytes before this address (the original location)
    void* potential_original = (char*)&node - 0x40;
    std::cerr << "[DEBUG] VisitCode: Checking potential original address " << std::hex << (uintptr_t)potential_original << std::dec << std::endl;
    
    // Use ASAN to check if the address is valid before accessing
    #ifdef __has_feature
    #if __has_feature(address_sanitizer)
    if (!__asan_address_is_poisoned(potential_original)) {
      AST::CodeBlock* potential_block = reinterpret_cast<AST::CodeBlock*>(potential_original);
      if (potential_block->type == AST::NodeType::BLOCK) {
        std::cerr << "[DEBUG] VisitCode: FOUND valid CodeBlock 0x40 bytes before! This suggests object slicing or copy issue" << std::endl;
        std::cerr << "[DEBUG] VisitCode: Original block at " << std::hex << (uintptr_t)potential_block 
                  << ", statements.size()=" << potential_block->statements.size() << std::dec << std::endl;
      }
    } else {
      std::cerr << "[DEBUG] VisitCode: Potential original address is ASAN poisoned" << std::endl;
    }
    #else
    // Without ASAN, try to check more carefully
    AST::CodeBlock* potential_block = reinterpret_cast<AST::CodeBlock*>(potential_original);
    if (potential_block->type == AST::NodeType::BLOCK) {
      std::cerr << "[DEBUG] VisitCode: FOUND valid CodeBlock 0x40 bytes before! This suggests object slicing or copy issue" << std::endl;
    }
    #endif
    #endif
  }
  
  // ASAN: Check if node is poisoned
  #ifdef __has_feature
  #if __has_feature(address_sanitizer)
  if (__asan_address_is_poisoned(&node)) {
    std::cerr << "[DEBUG] VisitCode: ERROR - node is ASAN poisoned!" << std::endl;
  }
  if (__asan_address_is_poisoned(&node.statements)) {
    std::cerr << "[DEBUG] VisitCode: ERROR - node.statements is ASAN poisoned!" << std::endl;
  }
  #endif
  #endif
  
  // Save previous state for nested blocks
  bool prev_return_state = has_return_encountered;
  has_return_encountered = false;
  
  // Store size to avoid issues if vector is modified during iteration
  size_t num_statements = node.statements.size();
  std::cerr << "[DEBUG] VisitCode: num_statements=" << num_statements << std::endl;
  
  for (size_t i = 0; i < num_statements; ++i) {
    std::cerr << "[DEBUG] VisitCode: Loop iteration i=" << i 
              << ", node.statements.size()=" << node.statements.size() << std::endl;
    
    // ASAN: Check if the vector element address is poisoned before accessing
    #ifdef __has_feature
    #if __has_feature(address_sanitizer)
    void *elem_addr = &node.statements[i];
    if (__asan_address_is_poisoned(elem_addr)) {
      std::cerr << "[DEBUG] VisitCode: ERROR - statements[" << i << "] address is ASAN poisoned at " << elem_addr << std::endl;
      break;
    }
    #endif
    #endif
    
    std::cerr << "[DEBUG] VisitCode: About to access statements[" << i << "] at " << (void*)&node.statements[i] << std::endl;
    auto &stmt = node.statements[i];
    std::cerr << "[DEBUG] VisitCode: Got reference to statements[" << i << "], &stmt=" << (void*)&stmt << std::endl;
    
    // This is where it crashes - the unique_ptr object itself is corrupted
    std::cerr << "[DEBUG] VisitCode: About to check if stmt is null, stmt.get()=" << (void*)stmt.get() << std::endl;
    if (!stmt) continue;  // Skip null statements
    
    // Check if we've already encountered a return in this block
    // Exception: In switch statements, case/default labels are independent execution paths
    // so we should not skip them even if a previous case had a return
    if (has_return_encountered) {
      // Check if this is a case or default statement (independent execution paths in switch)
      AST::NodeType stmt_type = stmt->type;
      if (stmt_type == AST::NodeType::CASE || stmt_type == AST::NodeType::DEFAULT) {
        // For case/default, reset the flag since they're independent execution paths
        has_return_encountered = false;
      } else {
        // Skip unreachable code after return statement
        continue;
      }
    }
    
    print("    ");
    VISIT_AND_CHECK(stmt);
    PrintSemiColon(stmt);
    print("\n");
    
    // Check if this statement is or contains a return AFTER visiting
    // We check after so we print the return statement itself, but skip subsequent ones
    // Store the node type before it might become invalid
    AST::NodeType stmt_type = stmt->type;
    bool is_return = (stmt_type == AST::NodeType::RETURN);
    if (!is_return) {
      // Check recursively for nested returns (e.g., in if statements)
      is_return = ContainsReturnStatement(stmt);
    }
    
    if (is_return) {
      has_return_encountered = true;
    }
  }
  
  // If we encountered a return in this block, keep the flag set for parent blocks
  // Otherwise restore previous state
  if (!has_return_encountered) {
    has_return_encountered = prev_return_state;
  }
  
  return true;
}

bool AST::CppPrettyPrinter::VisitCodeBlock(AST::CodeBlock &node) {
  print("{\n");
  if (!VisitCode(node)) return false;
  print("}");
  return true;
}

bool AST::CppPrettyPrinter::VisitIfStatement(AST::IfStatement &node) {
  print("if");
  if (node.not_condition) print("(!");
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  VISIT_AND_CHECK(node.condition);

  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  if (node.not_condition) print(")");

  print(" ");
  if (node.true_branch) {
    VISIT_AND_CHECK(node.true_branch);
    PrintSemiColon(node.true_branch);
  } else {
    print(";");
  }
  print(" ");

  if (node.false_branch) {
    print("else ");
    VISIT_AND_CHECK(node.false_branch);
    PrintSemiColon(node.false_branch);
    print(" ");
  }

  return true;
}

bool AST::CppPrettyPrinter::VisitForLoop(AST::ForLoop &node) {
  // Normal for loop output
  print("for(");

  VISIT_AND_CHECK(node.assignment);
  print("; ");

  VISIT_AND_CHECK(node.condition);
  print("; ");

  VISIT_AND_CHECK(node.increment);
  print(") ");

  if (node.body) {
    VISIT_AND_CHECK(node.body);
    PrintSemiColon(node.body);
  } else {
    print(";");
  }
  print(" ");

  return true;
}

bool AST::CppPrettyPrinter::VisitCaseStatement(AST::CaseStatement &node) {
  print("case ");
  VISIT_AND_CHECK(node.value);

  print(": ");
  if (!VisitCodeBlock(*node.statements->As<AST::CodeBlock>())) return false;
  print(" ");

  return true;
}

bool AST::CppPrettyPrinter::VisitDefaultStatement(AST::DefaultStatement &node) {
  print("default: ");
  if (!VisitCodeBlock(*node.statements->As<AST::CodeBlock>())) return false;
  print(" ");
  return true;
}

bool AST::CppPrettyPrinter::VisitSwitchStatement(AST::SwitchStatement &node) {
  print("switch(int(");
  VISIT_AND_CHECK(node.expression);
  print(")) ");

  if (!VisitCodeBlock(*node.body->As<AST::CodeBlock>())) return false;
  print(" ");

  return true;
}

bool AST::CppPrettyPrinter::VisitWhileLoop(AST::WhileLoop &node) {
  if (node.kind == AST::WhileLoop::Kind::REPEAT) {
    // repeat is a macro, so output it as-is (not converted to for/while loop)
    // The macro will be expanded by the C++ preprocessor at build time
    print("repeat");
    if (node.condition->type != AST::NodeType::PARENTHETICAL) {
      print("(");
    }
  } else {
    print("while");
    if (node.condition->type != AST::NodeType::PARENTHETICAL) {
      print("(");
    }

    if (node.kind == AST::WhileLoop::Kind::UNTIL) {
      if (node.condition->type == AST::NodeType::PARENTHETICAL) {
        print("(!");
      } else {
        print("!(");
      }
    }
  }

  VISIT_AND_CHECK(node.condition);

  if (node.kind == AST::WhileLoop::Kind::REPEAT) {
    if (node.condition->type != AST::NodeType::PARENTHETICAL) {
      print(")");
    }
  } else {
    if (node.kind == AST::WhileLoop::Kind::UNTIL) {
      print(")");
    }

    if (node.condition->type != AST::NodeType::PARENTHETICAL) {
      print(")");
    }
  }

  print(" ");
  VISIT_AND_CHECK(node.body);
  PrintSemiColon(node.body);

  return true;
}

bool AST::CppPrettyPrinter::VisitDoLoop(AST::DoLoop &node) {
  print("do");

  if (node.body->type != AST::NodeType::BLOCK) {
    print("{");
  }

  VISIT_AND_CHECK(node.body);
  PrintSemiColon(node.body);

  if (node.body->type != AST::NodeType::BLOCK) {
    print("}");
  }

  print("while");
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  if (node.is_until) {
    if (node.condition->type == AST::NodeType::PARENTHETICAL) {
      print("(!");
    } else {
      print("!(");
    }
  }

  VISIT_AND_CHECK(node.condition);

  if (node.is_until) {
    print(")");
  }
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  print(";");

  return true;
}
