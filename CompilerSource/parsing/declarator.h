/** Copyright (C) 2022 Dhruv Chawla
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
#ifndef ENIGMA_COMPILER_PARSING_DECLARATOR_h
#define ENIGMA_COMPILER_PARSING_DECLARATOR_h

#include <JDI/src/Storage/references.h>

#include <deque>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

/* Defines structs to store the structure of a declarator */
namespace enigma::parsing {
struct FullType;
struct Declarator;

struct PointerNode {
  bool is_const{};
  bool is_volatile{};
  jdi::definition_class *class_def{};
};

struct ReferenceNode {};

struct ArrayBoundNode {
  std::size_t size{};
  static constexpr std::size_t nsize = -1;
};

struct FunctionParameterNode {
  struct Parameter {
    bool is_variadic = false;
    void *default_value = nullptr;
    std::unique_ptr<FullType> type;

    Parameter() noexcept = default;
    Parameter(bool is_variadic, void *default_value, std::unique_ptr<FullType> type);

    Parameter(Parameter &&) noexcept = default;
    Parameter &operator=(Parameter &&) noexcept = default;

    ~Parameter();
  };

  using Parameters = std::vector<Parameter>;
  Parameters parameters{};
};

struct NestedNode {
  std::unique_ptr<Declarator> contained{};
};

struct DeclaratorNode {
  enum class Kind {
    POINTER_TO,
    MEMBER_POINTER,

    REFERENCE,
    RVAL_REFERENCE,

    ARRAY_BOUND,
    FUNCTION,
    NESTED,
  } kind;

  using ValueType = std::variant<PointerNode, ReferenceNode, ArrayBoundNode, FunctionParameterNode, NestedNode>;
  ValueType value{};

  DeclaratorNode(Kind kind, ValueType value): kind{kind}, value{std::move(value)} {}
  DeclaratorNode(jdi::definition_class *class_def, PointerNode ptr) {
    if (class_def != nullptr) {
      kind = Kind::MEMBER_POINTER;
    } else {
      kind = Kind::POINTER_TO;
    }
    value = std::move(ptr);
  }
  DeclaratorNode(Kind kind, ReferenceNode ref): kind{kind}, value{ref} {}
  explicit DeclaratorNode(ArrayBoundNode array): kind{Kind::ARRAY_BOUND}, value{array} {}
  explicit DeclaratorNode(FunctionParameterNode function): kind{Kind::FUNCTION}, value{std::move(function)} {}
  explicit DeclaratorNode(NestedNode node): kind{Kind::NESTED}, value{std::move(node)} {}

  template <typename T>
  [[nodiscard]] bool is() const noexcept { return std::holds_alternative<T>(value); }

  [[nodiscard]] bool is(Kind kind_) const noexcept { return kind == kind_; }

  template <typename T>
  const T &as() { return std::get<T>(value); }
};

struct Declarator {
  /**
   * @brief The individual components (*, &, &&, ::* etc.) making up the declarator
   */
  std::vector<DeclaratorNode> components{};

  /**
   * @brief The name of the variable being defined by the declarator (the @c x in <tt> **(*x)[10] </tt>)
   */
  std::string name;

  /**
   * @brief The definition that this declarator comes from
   */
  jdi::definition *ndef;

  /**
   * @brief The nested declarator (i.e. the <tt> <noptr-declarator> ::= ( <ptr-declarator> ) </tt> rule)
   */
  std::size_t nested_declarator{};
  bool has_nested_declarator = false;

  void add_pointer(jdi::definition_class *class_def, bool is_const, bool is_volatile);
  void add_reference(DeclaratorNode::Kind kind);
  void add_array_bound(std::size_t bound);
  void add_function_params(std::vector<FunctionParameterNode::Parameter> params);
  void add_nested(std::unique_ptr<Declarator> node);

  void *to_expression();

  /**
   * @brief Convert a @c Declarator to a <tt> jdi::ref_stack </tt>.
   *
   * This method ignores @c const and @c volatile modifiers present on pointers.
   *
   * @param result The @c ref_stack created from the @c Declarator
   */
  void to_jdi_refstack(jdi::ref_stack &result);
};

}

#endif