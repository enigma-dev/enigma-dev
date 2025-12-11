/**
 * @file clang_definitions.h
 * @brief Definition structures that mirror JDI interface but use clang
 * 
 * This file provides compatibility structures that match JDI's definition
 * interface but are backed by clang AST nodes.
 */

#ifndef ENIGMA_CLANG_DEFINITIONS_H
#define ENIGMA_CLANG_DEFINITIONS_H

#include <clang-c/Index.h>
#include <string>
#include <map>
#include <memory>
#include <vector>

// Forward declare to match JDI's namespace
namespace jdi {
  // Reuse JDI's flag definitions for compatibility
  enum DEF_FLAGS {
    DEF_TYPENAME =     1 <<  0,
    DEF_NAMESPACE =    1 <<  1,
    DEF_CLASS =        1 <<  2,
    DEF_ENUM =         1 <<  3,
    DEF_UNION =        1 <<  4,
    DEF_SCOPE =        1 <<  5,
    DEF_TYPED =        1 <<  6,
    DEF_FUNCTION =     1 <<  7,
    DEF_OVERLOAD =     1 <<  8,
    DEF_VALUED =       1 <<  9,
    DEF_EXTERN =       1 << 10,
    DEF_TEMPLATE =     1 << 11,
    DEF_TEMPPARAM =    1 << 12,
    DEF_HYPOTHETICAL = 1 << 13,
    DEF_DEPENDENT =    1 << 14,
    DEF_PRIVATE =      1 << 15,
    DEF_PROTECTED =    1 << 16,
    DEF_INCOMPLETE =    1 << 17,
    DEF_ATOMIC =       1 << 18
  };

  // Forward declarations removed - using typedefs instead
}

namespace clang_adapter {

// Base definition class that mirrors JDI's definition structure
struct ClangDefinition {
  unsigned int flags;
  std::string name;
  struct ClangDefinitionScope* parent;
  CXCursor cursor;  // The clang cursor this represents
  
  ClangDefinition(const std::string& n, struct ClangDefinitionScope* p, unsigned int f, CXCursor c)
    : flags(f), name(n), parent(p), cursor(c) {}
  
  virtual ~ClangDefinition() = default;
  
  bool has_all_flags(unsigned flgs) const { return (flags & flgs) == flgs; }
  
  std::string qualified_id() const;
};

// Forward declaration
struct ClangDefinitionScope;

// Forward declaration for shared_ptr
struct ClangDefinitionScope;

// Scope definition (namespace, class, struct, union)
struct ClangDefinitionScope : public ClangDefinition {
  std::map<std::string, std::shared_ptr<ClangDefinition>> members;
  
  ClangDefinitionScope(const std::string& n, ClangDefinitionScope* p, unsigned int f, CXCursor c)
    : ClangDefinition(n, p, f, c) {}
  
  ClangDefinition* look_up(const std::string& name);
  ClangDefinition* find_local(const std::string& name);
};

// Class definition
struct ClangDefinitionClass : public ClangDefinitionScope {
  std::vector<std::pair<ClangDefinitionClass*, int>> ancestors;  // base classes
  
  ClangDefinitionClass(const std::string& n, ClangDefinitionScope* p, unsigned int f, CXCursor c)
    : ClangDefinitionScope(n, p, f, c) {}
};

// Function overload
struct ClangDefinitionOverload : public ClangDefinition {
  // Parameters stored as type definitions
  std::vector<ClangDefinition*> params;
  // Storage for parameter shared_ptrs to keep them alive
  std::vector<std::shared_ptr<ClangDefinition>> owned_params_storage;
  bool is_variadic;
  
  ClangDefinitionOverload(const std::string& n, ClangDefinitionScope* p, unsigned int f, CXCursor c)
    : ClangDefinition(n, p, f, c), is_variadic(false) {}
};

// Function definition (contains overloads)
struct ClangDefinitionFunction : public ClangDefinition {
  std::map<std::string, std::shared_ptr<ClangDefinitionOverload>> overloads;
  std::vector<std::shared_ptr<ClangDefinitionOverload>> template_overloads;
  
  ClangDefinitionFunction(const std::string& n, ClangDefinitionScope* p, unsigned int f, CXCursor c)
    : ClangDefinition(n, p, f, c) {}
};

// Typed definition (variables, typedefs)
struct ClangDefinitionTyped : public ClangDefinition {
  ClangDefinition* type;
  
  ClangDefinitionTyped(const std::string& n, ClangDefinitionScope* p, unsigned int f, CXCursor c, ClangDefinition* t)
    : ClangDefinition(n, p, f, c), type(t) {}
};

// Helper function to convert clang cursor kind to JDI flags
unsigned int cursor_kind_to_flags(CXCursorKind kind);

// Helper function to get cursor name
std::string get_cursor_name(CXCursor cursor);

} // namespace clang_adapter

// Type aliases for compatibility with existing code
namespace jdi {
  // Use clang adapter types as jdi types
  typedef clang_adapter::ClangDefinition definition;
  typedef clang_adapter::ClangDefinitionScope definition_scope;
  typedef clang_adapter::ClangDefinitionClass definition_class;
  typedef clang_adapter::ClangDefinitionFunction definition_function;
  typedef clang_adapter::ClangDefinitionOverload definition_overload;
  typedef clang_adapter::ClangDefinitionTyped definition_typed;
}

#endif // ENIGMA_CLANG_DEFINITIONS_H
