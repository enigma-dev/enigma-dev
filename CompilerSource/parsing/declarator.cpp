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

FunctionParameterNode::FunctionParameterNode(std::vector<Parameter> params, bool outside_nested):
  kind{Kind::DECLARATOR}, parameters{std::move(params)}, outside_nested{outside_nested} {}
FunctionParameterNode::FunctionParameterNode(Parameters params, bool outside_nested):
  kind{Kind::DECLARATOR}, parameters{std::move(params)}, outside_nested{outside_nested} {}
FunctionParameterNode::FunctionParameterNode(void *expression, bool outside_nested):
  kind{Kind::EXPRESSION}, parameters{expression}, outside_nested{outside_nested} {}
FunctionParameterNode::~FunctionParameterNode() {
  if (is<void *>()) {
    delete reinterpret_cast<AST::FunctionCallExpression *>(std::get<void *>(parameters));
  }
}

NestedNode::NestedNode(std::unique_ptr<Declarator> decl):
  kind{Kind::DECLARATOR}, contained{std::move(decl)} {}
NestedNode::NestedNode(void *expr):
  kind{Kind::EXPRESSION}, contained{expr} {}

NestedNode::NestedNode(NestedNode &&node) noexcept: kind{node.kind} {
  *this = std::move(node);
}

NestedNode &NestedNode::operator=(NestedNode &&node) noexcept {
  if (&node != this) {
    kind = node.kind;
    contained = std::move(node.contained);
    switch (kind) {
      case Kind::EXPRESSION:
        node.as<void *>() = nullptr;
        break;
      case Kind::DECLARATOR:
        break;
    }
  }

  return *this;
}

NestedNode::~NestedNode() noexcept {
  if (is<void *>()) {
    auto node = reinterpret_cast<AST::Node *>(as<void *>());
    delete node;
  }
}

void Declarator::add_pointer(jdi::definition_class *class_def, bool is_const, bool is_volatile) {
  components.emplace_back(class_def, PointerNode{is_const, is_volatile, class_def});
}

void Declarator::add_reference(DeclaratorNode::Kind kind) {
  components.emplace_back(kind, ReferenceNode{});
}

void Declarator::add_array_bound(std::size_t bound, bool outside_nested) {
  components.emplace_back(ArrayBoundNode{bound, outside_nested});
}

void Declarator::add_function_params(std::vector<FunctionParameterNode::Parameter> params, bool outside_nested) {
  components.emplace_back(FunctionParameterNode{std::move(params), outside_nested});
}

void Declarator::add_function_params(FunctionParameterNode::Parameters params, bool outside_nested) {
  components.emplace_back(FunctionParameterNode{std::move(params), outside_nested});
}

void Declarator::add_function_params(FunctionParameterNode params) {
  components.emplace_back(std::move(params));
}

void Declarator::add_nested(std::unique_ptr<Declarator> node) {
  nested_declarator = components.size();
  has_nested_declarator = true;
  components.emplace_back(NestedNode{std::move(node)});
}

void Declarator::add_nested(void *expr) {
  nested_declarator = components.size();
  has_nested_declarator = true;
  components.emplace_back(NestedNode{expr});
}

void make_array_subscript_expression(ArrayBoundNode &node, std::unique_ptr<AST::Node> &expr, std::deque<std::string> &token_contents) {
  token_contents.emplace_back(std::to_string(node.size));
  CodeSnippet location = {token_contents.back(), 0, 0};
  Token index = {TT_DECLITERAL, location};
  expr = std::make_unique<AST::BinaryExpression>(std::move(expr), std::make_unique<AST::Literal>(index), TT_BEGINBRACKET);
}

void make_function_call_expression(FunctionParameterNode &node, std::unique_ptr<AST::Node> &expr) {
  if (node.kind == FunctionParameterNode::Kind::EXPRESSION) {
    auto call = reinterpret_cast<AST::FunctionCallExpression *>(node.as<void *>());
    node.as<void *>() = nullptr;
    call->function = std::move(expr);
    expr = std::unique_ptr<AST::Node>(call);
  } else {
    std::cerr << "Internal error: This part of Declarator::to_expression (" << __FILE__ << ":" << __LINE__ << ") should be unreachable";
    auto call = std::make_unique<AST::FunctionCallExpression>(
        std::move(expr), std::vector<std::unique_ptr<AST::Node>>{});
    for (auto &param : node.as<FunctionParameterNode::ParameterList>()) {
      auto decl_expr = std::unique_ptr<AST::Node>(reinterpret_cast<AST::Node *>(param.type->decl.to_expression()));
      if (param.default_value != nullptr) {
        decl_expr = std::make_unique<AST::BinaryExpression>(
            std::move(decl_expr), std::unique_ptr<AST::Node>(reinterpret_cast<AST::Node *>(param.default_value)),
            TT_EQUALS);
        param.default_value = nullptr;
      }
      call->arguments.emplace_back(std::move(decl_expr));
    }
    expr = std::move(call);
  }
}

void *Declarator::to_expression() {
  std::unique_ptr<AST::Node> expr = nullptr;
  if (has_nested_declarator && components[nested_declarator].is<NestedNode>()) {
    if (components[nested_declarator].as<NestedNode>().is<std::unique_ptr<Declarator>>()) {
      expr = std::unique_ptr<AST::Node>{
          reinterpret_cast<AST::Node *>(components[nested_declarator]
                                            .as<NestedNode>()
                                            .as<std::unique_ptr<Declarator>>()
                                            ->to_expression())};
    } else if (components[nested_declarator].as<NestedNode>().is<void *>()) {
      if (components[nested_declarator].as<NestedNode>().as<void *>() == nullptr) {
        std::cerr << "Internal error: trying to convert nullptr nested_declarator to expression more than once\n";
        return nullptr;
      } else {
        expr = std::unique_ptr<AST::Node>{
            reinterpret_cast<AST::Node *>(components[nested_declarator].as<NestedNode>().as<void *>())};
        components[nested_declarator].as<NestedNode>().as<void *>() = nullptr;
      }
    } else {
      std::cerr << "This code is unreachable: (" << __FILE__ << ":" << __LINE__ << ")\n";
      return nullptr;
    }
  } else if (!name.content.empty()) {
    expr = std::make_unique<AST::Literal>(name);
  }

  if (!components.empty()) {
    std::size_t ptrs = 0;
    while (ptrs < components.size() && (components[ptrs].is<PointerNode>() || components[ptrs].is<ReferenceNode>())) {
      ptrs++;
    }
    std::vector<DeclaratorNode *> post_modifiers{};
    if (ptrs < components.size() &&
        (components[ptrs].is<ArrayBoundNode>() || components[ptrs].is<FunctionParameterNode>())) {
      std::size_t post_declarators = ptrs;
      while (post_declarators < components.size()) {
        if (components[post_declarators].is<ArrayBoundNode>()) {
          if (components[post_declarators].as<ArrayBoundNode>().outside_nested) {
            post_modifiers.emplace_back(&components[post_declarators]);
          } else {
            make_array_subscript_expression(components[post_declarators].as<ArrayBoundNode>(), expr, token_contents);
          }
        } else {
          auto &params = components[post_declarators].as<FunctionParameterNode>();
          if (params.outside_nested) {
            post_modifiers.emplace_back(&components[post_declarators]);
          } else {
            make_function_call_expression(components[post_declarators].as<FunctionParameterNode>(), expr);
          }
        }
        post_declarators++;
      }
    }

    for (std::int64_t i = ptrs - 1; i >= 0; i--) {
      if (components[i].is<PointerNode>()) {
        // TODO: Implement accessing class members, though idk if its legally allowed
        expr = std::make_unique<AST::UnaryPrefixExpression>(std::move(expr), TT_STAR);
      } else if (components[i].is<ReferenceNode>()) {
        if (components[i].kind == DeclaratorNode::Kind::RVAL_REFERENCE) {
          // TODO: Maybe make this an error, however for now let the compiler deal with it
          expr = std::make_unique<AST::UnaryPrefixExpression>(std::move(expr), TT_AMPERSAND);
          expr = std::make_unique<AST::UnaryPrefixExpression>(std::move(expr), TT_AMPERSAND);
        } else {
          expr = std::make_unique<AST::UnaryPrefixExpression>(std::move(expr), TT_AMPERSAND);
        }
      } else if (components[i].is<NestedNode>()) {
        continue;
      } else {
        std::cerr << "Internal error: This part of Declarator::to_expression (" << __FILE__ << ":" << __LINE__ << ") should be unreachable\n";
      }
    }
    if (!post_modifiers.empty()) {
      expr = std::make_unique<AST::Parenthetical>(std::move(expr));
    }
    for (auto &post : post_modifiers) {
      if (post->is<ArrayBoundNode>()) {
        make_array_subscript_expression(post->as<ArrayBoundNode>(), expr, token_contents);
      } else {
        make_function_call_expression(post->as<FunctionParameterNode>(), expr);
      }
    }
  }

  return reinterpret_cast<void *>(expr.release());
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
        if (component.as<FunctionParameterNode>().is<FunctionParameterNode::ParameterList>()) {
          for (auto &param : component.as<FunctionParameterNode>().as<FunctionParameterNode::ParameterList>()) {
            jdi::ref_stack::parameter jdi_param;
            if (param.default_value != nullptr) {
              std::cerr << "Unimplemented: FUNCTION (Declarator) default value -> jdi::ref_stack";
            }
            jdi_param.variadic = param.is_variadic;
            jdi::full_type ft = param.type->to_jdi_fulltype();
            jdi_param.swap_in(ft);
            params.throw_on(jdi_param);
          }
        } else {
          std::cerr << "Internal error: trying to convert a function call expression to a refstack";
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
    if (components[nested_declarator].as<NestedNode>().is<std::unique_ptr<Declarator>>()) {
      components[nested_declarator]
          .as<NestedNode>()
          .as<std::unique_ptr<Declarator>>()
          ->to_jdi_refstack(nested);
    } else {
      std::cerr << "Internal error: cannot convert nested expression to ref_stack";
    }

    result.append_c(nested);
  }
}
}
