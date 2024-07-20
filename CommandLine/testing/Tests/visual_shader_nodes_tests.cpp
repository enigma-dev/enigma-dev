/********************************************************************************\
**                                                                              **
**  Copyright (C) 2024 Saif Kandil (k0T0z)                                      **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "ResourceTransformations/VisualShader/visual_shader_nodes.h"

#include <gtest/gtest.h>

#include <iostream>

TEST(VisualShaderNodesTest, Test_VisualShaderNodeFloatConstant_generate_code) {
  VisualShaderNodeFloatConstant node;
  node.set_constant(1.0);
  std::string code {node.generate_code(0, {}, {"c"})};
  std::string expected_code {"\tc = 1.000000;\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeIntConstant_generate_code) {
  VisualShaderNodeIntConstant node;
  node.set_constant(1);
  std::string code {node.generate_code(0, {}, {"c"})};
  std::string expected_code {"\tc = 1;\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeUIntConstant_generate_code) {
  VisualShaderNodeUIntConstant node;
  node.set_constant(1);
  std::string code {node.generate_code(0, {}, {"c"})};
  std::string expected_code {"\tc = 1u;\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeBooleanConstant_generate_code) {
  VisualShaderNodeBooleanConstant node;
  node.set_constant(true);
  std::string code {node.generate_code(0, {}, {"c"})};
  std::string expected_code {"\tc = true;\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeColorConstant_generate_code) {
  VisualShaderNodeColorConstant node;
  node.set_constant(TEColor(1.0, 2.0, 3.0, 0.0));
  std::string code {node.generate_code(0, {}, {"c"})};
  std::string expected_code {"\tc = vec4(1.000000, 2.000000, 3.000000, 0.000000);\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeVec2Constant_generate_code) {
  VisualShaderNodeVec2Constant node;
  node.set_constant(TEVector2(1.0, 2.0));
  std::string code {node.generate_code(0, {}, {"c"})};
  std::string expected_code {"\tc = vec2(1.000000, 2.000000);\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeVec3Constant_generate_code) {
  VisualShaderNodeVec3Constant node;
  node.set_constant(TEVector3(1.0, 2.0, 3.0));
  std::string code {node.generate_code(0, {}, {"c"})};
  std::string expected_code {"\tc = vec3(1.000000, 2.000000, 3.000000);\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeVec4Constant_generate_code) {
  VisualShaderNodeVec4Constant node;
  node.set_constant(TEQuaternion(1.0, 2.0, 3.0, 4.0));
  std::string code {node.generate_code(0, {}, {"c"})};
  std::string expected_code {"\tc = vec4(1.000000, 2.000000, 3.000000, 4.000000);\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeFloatOp_generate_code) {
  VisualShaderNodeFloatOp node;
  std::vector<std::string> input_vars = {"a", "b"};
  std::vector<std::string> output_vars = {"c"};
  std::string code {node.generate_code(0, input_vars, output_vars)};
  std::string expected_code {"\tc = a + b;\n"};
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeFloatOp::Operator::OP_SUB);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a - b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeFloatOp::Operator::OP_MUL);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a * b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeFloatOp::Operator::OP_DIV);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a / b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeFloatOp::Operator::OP_MOD);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = mod(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeFloatOp::Operator::OP_POW);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = pow(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeFloatOp::Operator::OP_MAX);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = max(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeFloatOp::Operator::OP_MIN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = min(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeFloatOp::Operator::OP_ATAN2);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = atan(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeFloatOp::Operator::OP_STEP);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = step(a, b);\n";
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeIntOp_generate_code) {
  VisualShaderNodeIntOp node;
  std::vector<std::string> input_vars = {"a", "b"};
  std::vector<std::string> output_vars = {"c"};
  std::string code {node.generate_code(0, input_vars, output_vars)};
  std::string expected_code {"\tc = a + b;\n"};
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeIntOp::Operator::OP_SUB);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a - b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeIntOp::Operator::OP_MUL);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a * b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeIntOp::Operator::OP_DIV);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a / b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeIntOp::Operator::OP_MOD);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a % b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeIntOp::Operator::OP_MAX);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = max(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeIntOp::Operator::OP_MIN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = min(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeIntOp::Operator::OP_BITWISE_AND);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a & b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeIntOp::Operator::OP_BITWISE_OR);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a | b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeIntOp::Operator::OP_BITWISE_XOR);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a ^ b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeIntOp::Operator::OP_BITWISE_LEFT_SHIFT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a << b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeIntOp::Operator::OP_BITWISE_RIGHT_SHIFT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a >> b;\n";
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeUIntOp_generate_code) {
  VisualShaderNodeUIntOp node;
  std::vector<std::string> input_vars = {"a", "b"};
  std::vector<std::string> output_vars = {"c"};
  std::string code {node.generate_code(0, input_vars, output_vars)};
  std::string expected_code {"\tc = a + b;\n"};
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeUIntOp::Operator::OP_SUB);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a - b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeUIntOp::Operator::OP_MUL);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a * b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeUIntOp::Operator::OP_DIV);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a / b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeUIntOp::Operator::OP_MOD);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a % b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeUIntOp::Operator::OP_MAX);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = max(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeUIntOp::Operator::OP_MIN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = min(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeUIntOp::Operator::OP_BITWISE_AND);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a & b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeUIntOp::Operator::OP_BITWISE_OR);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a | b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeUIntOp::Operator::OP_BITWISE_XOR);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a ^ b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeUIntOp::Operator::OP_BITWISE_LEFT_SHIFT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a << b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeUIntOp::Operator::OP_BITWISE_RIGHT_SHIFT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a >> b;\n";
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeVectorOp_generate_code) {
  VisualShaderNodeVectorOp node;
  std::vector<std::string> input_vars = {"a", "b"};
  std::vector<std::string> output_vars = {"c"};
  std::string code {node.generate_code(0, input_vars, output_vars)};
  std::string expected_code {"\tc = a + b;\n"};
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeVectorOp::Operator::OP_SUB);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a - b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeVectorOp::Operator::OP_MUL);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a * b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeVectorOp::Operator::OP_DIV);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = a / b;\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeVectorOp::Operator::OP_MOD);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = mod(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeVectorOp::Operator::OP_POW);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = pow(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeVectorOp::Operator::OP_MAX);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = max(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeVectorOp::Operator::OP_MIN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = min(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeVectorOp::Operator::OP_CROSS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = cross(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeVectorOp::Operator::OP_ATAN2);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = atan(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeVectorOp::Operator::OP_REFLECT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = reflect(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeVectorOp::Operator::OP_STEP);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = step(a, b);\n";
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeColorOp_generate_code) {
  VisualShaderNodeColorOp node;
  std::vector<std::string> input_vars = {"a", "b"};
  std::vector<std::string> output_vars = {"c"};
  std::string code {node.generate_code(0, input_vars, output_vars)};
  std::string expected_code {"\tc = vec3(1.0) - (vec3(1.0) - a) * (vec3(1.0) - b);\n"};
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeColorOp::Operator::OP_DIFFERENCE);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = abs(a - b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeColorOp::Operator::OP_DARKEN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = min(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeColorOp::Operator::OP_LIGHTEN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = max(a, b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeColorOp::Operator::OP_OVERLAY);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\t{\n"
  "\t\tfloat base = a.x;\n"
  "\t\tfloat blend = b.x;\n"
  "\t\tif (base < 0.5) {\n"
  "\t\t\tc.x = 2.0 * base * blend;\n"
  "\t\t} else {\n"
  "\t\t\tc.x = 1.0 - 2.0 * (1.0 - blend) * (1.0 - base);\n"
  "\t\t}\n"
  "\t}\n\n"
  "\t{\n"
  "\t\tfloat base = a.y;\n"
  "\t\tfloat blend = b.y;\n"
  "\t\tif (base < 0.5) {\n"
  "\t\t\tc.y = 2.0 * base * blend;\n"
  "\t\t} else {\n"
  "\t\t\tc.y = 1.0 - 2.0 * (1.0 - blend) * (1.0 - base);\n"
  "\t\t}\n"
  "\t}\n\n"
  "\t{\n"
  "\t\tfloat base = a.z;\n"
  "\t\tfloat blend = b.z;\n"
  "\t\tif (base < 0.5) {\n"
  "\t\t\tc.z = 2.0 * base * blend;\n"
  "\t\t} else {\n"
  "\t\t\tc.z = 1.0 - 2.0 * (1.0 - blend) * (1.0 - base);\n"
  "\t\t}\n"
  "\t}\n\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeColorOp::Operator::OP_DODGE);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = (a) / (vec3(1.0) - b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeColorOp::Operator::OP_BURN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = vec3(1.0) - (vec3(1.0) - a) / (b);\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeColorOp::Operator::OP_SOFT_LIGHT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\t{\n"
  "\t\tfloat base = a.x;\n"
  "\t\tfloat blend = b.x;\n"
  "\t\tif (base < 0.5) {\n"
  "\t\t\tc.x = (base * (blend + 0.5));\n"
  "\t\t} else {\n"
  "\t\t\tc.x = (1.0 - (1.0 - base) * (1.0 - (blend - 0.5)));\n"
  "\t\t}\n"
  "\t}\n\n"
  "\t{\n"
  "\t\tfloat base = a.y;\n"
  "\t\tfloat blend = b.y;\n"
  "\t\tif (base < 0.5) {\n"
  "\t\t\tc.y = (base * (blend + 0.5));\n"
  "\t\t} else {\n"
  "\t\t\tc.y = (1.0 - (1.0 - base) * (1.0 - (blend - 0.5)));\n"
  "\t\t}\n"
  "\t}\n\n"
  "\t{\n"
  "\t\tfloat base = a.z;\n"
  "\t\tfloat blend = b.z;\n"
  "\t\tif (base < 0.5) {\n"
  "\t\t\tc.z = (base * (blend + 0.5));\n"
  "\t\t} else {\n"
  "\t\t\tc.z = (1.0 - (1.0 - base) * (1.0 - (blend - 0.5)));\n"
  "\t\t}\n"
  "\t}\n\n";
  EXPECT_EQ(code, expected_code);
  node.set_operator(VisualShaderNodeColorOp::Operator::OP_HARD_LIGHT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\t{\n"
  "\t\tfloat base = a.x;\n"
  "\t\tfloat blend = b.x;\n"
  "\t\tif (base < 0.5) {\n"
  "\t\t\tc.x = (base * (2.0 * blend));\n"
  "\t\t} else {\n"
  "\t\t\tc.x = (1.0 - (1.0 - base) * (1.0 - 2.0 * (blend - 0.5)));\n"
  "\t\t}\n"
  "\t}\n\n"
  "\t{\n"
  "\t\tfloat base = a.y;\n"
  "\t\tfloat blend = b.y;\n"
  "\t\tif (base < 0.5) {\n"
  "\t\t\tc.y = (base * (2.0 * blend));\n"
  "\t\t} else {\n"
  "\t\t\tc.y = (1.0 - (1.0 - base) * (1.0 - 2.0 * (blend - 0.5)));\n"
  "\t\t}\n"
  "\t}\n\n"
  "\t{\n"
  "\t\tfloat base = a.z;\n"
  "\t\tfloat blend = b.z;\n"
  "\t\tif (base < 0.5) {\n"
  "\t\t\tc.z = (base * (2.0 * blend));\n"
  "\t\t} else {\n"
  "\t\t\tc.z = (1.0 - (1.0 - base) * (1.0 - 2.0 * (blend - 0.5)));\n"
  "\t\t}\n"
  "\t}\n\n";
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeFloatFunc_generate_code) {
  VisualShaderNodeFloatFunc node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code {node.generate_code(0, input_vars, output_vars)};
  std::string expected_code {"\tb = sin(a);\n"};
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_COS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = cos(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_TAN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = tan(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ASIN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = asin(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ACOS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = acos(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ATAN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = atan(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_SINH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = sinh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_COSH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = cosh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_TANH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = tanh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_LOG);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = log(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_EXP);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = exp(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_SQRT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = sqrt(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ABS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = abs(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_SIGN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = sign(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_FLOOR);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = floor(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ROUND);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = round(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_CEIL);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = ceil(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_FRACT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = fract(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_SATURATE);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = min(max(a, 0.0), 1.0);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_NEGATE);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = -(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ACOSH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = acosh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ASINH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = asinh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ATANH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = atanh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_DEGREES);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = degrees(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_EXP2);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = exp2(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_INVERSE_SQRT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = inversesqrt(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_LOG2);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = log2(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_RADIANS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = radians(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_RECIPROCAL);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = 1.0 / (a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ROUNDEVEN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = roundEven(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_TRUNC);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = trunc(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ONEMINUS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = 1.0 - a;\n";
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeIntFunc_generate_code) {
  VisualShaderNodeIntFunc node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code {node.generate_code(0, input_vars, output_vars)};
  std::string expected_code {"\tb = sign(a);\n"};
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeIntFunc::Function::FUNC_ABS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = abs(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeIntFunc::Function::FUNC_NEGATE);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = -(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeIntFunc::Function::FUNC_BITWISE_NOT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = ~(a);\n";
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeUIntFunc_generate_code) {
  VisualShaderNodeUIntFunc node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code {node.generate_code(0, input_vars, output_vars)};
  std::string expected_code {"\tb = -(a);\n"};
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeUIntFunc::Function::FUNC_BITWISE_NOT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = ~(a);\n";
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeVectorFunc_generate_code) {
  VisualShaderNodeVectorFunc node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code {node.generate_code(0, input_vars, output_vars)};
  std::string expected_code {"\tb = normalize(a);\n"};
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_SATURATE);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = max(min(a, vec3(1.0)), vec3(0.0));\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_NEGATE);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = -(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_RECIPROCAL);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = 1.0 / (a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ABS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = abs(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ACOS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = acos(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ACOSH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = acosh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ASIN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = asin(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ASINH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = asinh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ATAN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = atan(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ATANH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = atanh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_CEIL);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = ceil(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_COS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = cos(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_COSH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = cosh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_DEGREES);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = degrees(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_EXP);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = exp(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_EXP2);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = exp2(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_FLOOR);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = floor(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_FRACT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = fract(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_INVERSE_SQRT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = inversesqrt(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_LOG);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = log(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_LOG2);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = log2(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_RADIANS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = radians(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ROUND);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = round(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ROUNDEVEN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = roundEven(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_SIGN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = sign(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_SIN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = sin(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_SINH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = sinh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_SQRT);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = sqrt(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_TAN);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = tan(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_TANH);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = tanh(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_TRUNC);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = trunc(a);\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ONEMINUS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = vec3(1.0) - a;\n";
  EXPECT_EQ(code, expected_code);

  // Change the dimension to 2
  node.set_op_type(VisualShaderNodeVectorBase::OpType::OP_TYPE_VECTOR_2D);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_SATURATE);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = max(min(a, vec2(1.0)), vec2(0.0));\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ONEMINUS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = vec2(1.0) - a;\n";
  EXPECT_EQ(code, expected_code);

  // Change the dimension to 4
  node.set_op_type(VisualShaderNodeVectorBase::OpType::OP_TYPE_VECTOR_4D);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_SATURATE);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = max(min(a, vec4(1.0)), vec4(0.0));\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeVectorFunc::Function::FUNC_ONEMINUS);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tb = vec4(1.0) - a;\n";
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeColorFunc_generate_code) {
  VisualShaderNodeColorFunc node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code {node.generate_code(0, input_vars, output_vars)};
  std::string expected_code {"\t{\n"
  "\t\tvec3 c = a;\n"
  "\t\tfloat max1 = max(c.r, c.g);\n"
  "\t\tfloat max2 = max(max1, c.b);\n"
  "\t\tb = vec3(max2, max2, max2);\n"
  "\t}\n"};
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeColorFunc::Function::FUNC_HSV2RGB);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\t{\n"
  "\t\tvec3 c = a;\n"
  "\t\tvec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n"
  "\t\tvec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);\n"
  "\t\tb = c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);\n"
  "\t}\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeColorFunc::Function::FUNC_RGB2HSV);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\t{\n"
  "\t\tvec3 c = a;\n"
  "\t\tvec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);\n"
  "\t\tvec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));\n"
  "\t\tvec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));\n"
  "\t\tfloat d = q.x - min(q.w, q.y);\n"
  "\t\tfloat e = 1.0e-10;\n"
  "\t\tb = vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);\n"
  "\t}\n";
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeColorFunc::Function::FUNC_SEPIA);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\t{\n"
  "\t\tvec3 c = a;\n"
  "\t\tfloat r = (c.r * .393) + (c.g *.769) + (c.b * .189);\n"
  "\t\tfloat g = (c.r * .349) + (c.g *.686) + (c.b * .168);\n"
  "\t\tfloat b = (c.r * .272) + (c.g *.534) + (c.b * .131);\n"
  "\t\tb = vec3(r, g, b);\n"
  "\t}\n";
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeUVFunc_generate_code) {
  VisualShaderNodeUVFunc node;
  std::vector<std::string> input_vars = {"a", "1.0", "0.0"}; // scale, offset
  std::vector<std::string> output_vars = {"c"};
  std::string code {node.generate_code(0, input_vars, output_vars)};
  std::string expected_code {"\tc = (0.0 * 1.0) + a;\n"};
  EXPECT_EQ(code, expected_code);
  node.set_function(VisualShaderNodeUVFunc::Function::FUNC_SCALING);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = ((a - 0.0) * 1.0) + 0.0;\n";
  EXPECT_EQ(code, expected_code);

  input_vars.at(0) = "";
  
  node.set_function(VisualShaderNodeUVFunc::Function::FUNC_PANNING);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = (0.0 * 1.0) + UV;\n";
  EXPECT_EQ(code, expected_code);

  node.set_function(VisualShaderNodeUVFunc::Function::FUNC_SCALING);
  code = node.generate_code(0, input_vars, output_vars);
  expected_code = "\tc = ((UV - 0.0) * 1.0) + 0.0;\n";
  EXPECT_EQ(code, expected_code);
}
