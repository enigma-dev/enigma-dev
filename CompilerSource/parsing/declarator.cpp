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
#include "declarator.h"

#include "ast.h"
#include "full_type.h"

namespace enigma::parsing {
FunctionParameterNode::Parameter::Parameter(bool is_variadic, void *default_value, std::unique_ptr<FullType> type):
  is_variadic{is_variadic}, default_value{default_value}, type{std::move(type)} {}
FunctionParameterNode::Parameter::~Parameter() {
  delete reinterpret_cast<AST::Initializer *>(default_value);
}

void Declarator::add_pointer(jdi::definition_class *class_def, bool is_const, bool is_volatile) {
  components.emplace_back(class_def, PointerNode{is_const, is_volatile});
}

void Declarator::add_reference(DeclaratorNode::Kind kind) {
  components.emplace_back(kind, ReferenceNode{});
}

void Declarator::add_array_bound(std::size_t bound) {
  components.emplace_back(ArrayBoundNode{bound});
}

void Declarator::add_function_params(std::vector<FunctionParameterNode::Parameter> params) {
  components.emplace_back(FunctionParameterNode{std::move(params)});
}

void Declarator::add_nested(std::unique_ptr<Declarator> node) {
  nested_declarator = components.size();
  has_nested_declarator = true;
  components.emplace_back(NestedNode{std::move(node)});
}

void *Declarator::to_expression() {

}

void Declarator::to_jdi_refstack(jdi::ref_stack &result) {
  for (auto &component: components) {
    switch (component.kind) {
      case DeclaratorNode::Kind::POINTER_TO:
        result.push(jdi::ref_stack::RT_POINTERTO);
        break;
      case DeclaratorNode::Kind::MEMBER_POINTER:
        result.push_memptr(component.as<PointerNode>().class_def);
        break;
      case DeclaratorNode::Kind::REFERENCE:
        result.push(jdi::ref_stack::RT_REFERENCE);
        break;
      case DeclaratorNode::Kind::RVAL_REFERENCE:
        // TODO: This thing
        std::cerr << "Unimplemented: RVAL_REFERENCE (Declarator) -> jdi::ref_stack";
        break;
      case DeclaratorNode::Kind::ARRAY_BOUND:
        result.push_array(component.as<ArrayBoundNode>().size);
        break;
      case DeclaratorNode::Kind::FUNCTION: {
        jdi::ref_stack::parameter_ct params;
        for (auto &param : component.as<FunctionParameterNode>().parameters) {
          jdi::ref_stack::parameter jdi_param;
          if (param.default_value != nullptr) {
            std::cerr << "Unimplemented: FUNCTION (Declarator) default value -> jdi::ref_stack";
          }
          jdi_param.variadic = param.is_variadic;
          jdi::full_type ft = param.type->to_jdi_fulltype();
          jdi_param.swap_in(ft);
          params.throw_on(jdi_param);
        }
        result.push_func(params);
        break;
      }
      case DeclaratorNode::Kind::NESTED:
        break; // This case will be handled below
    }
  }

  if (has_nested_declarator && components[nested_declarator].is<NestedNode>()) {
    jdi::ref_stack nested;
    components[nested_declarator].as<NestedNode>().contained->to_jdi_refstack(nested);
    result.append_c(nested);
  }
}
}
