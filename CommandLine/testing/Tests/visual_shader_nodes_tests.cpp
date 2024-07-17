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
