/*********************************************************************************/
/*                                                                               */
/*  Copyright (C) 2024 Saif Kandil (k0T0z)                                       */
/*                                                                               */
/*  This file is a part of the ENIGMA Development Environment.                   */
/*                                                                               */
/*                                                                               */
/*  ENIGMA is free software: you can redistribute it and/or modify it under the  */
/*  terms of the GNU General Public License as published by the Free Software    */
/*  Foundation, version 3 of the license or any later version.                   */
/*                                                                               */
/*  This application and its source code is distributed AS-IS, WITHOUT ANY       */
/*  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS    */
/*  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more        */
/*  details.                                                                     */
/*                                                                               */
/*  You should have recieved a copy of the GNU General Public License along      */
/*  with this code. If not, see <http://www.gnu.org/licenses/>                   */
/*                                                                               */
/*  ENIGMA is an environment designed to create games and other programs with a  */
/*  high-level, fully compilable language. Developers of ENIGMA or anything      */
/*  associated with ENIGMA are in no way responsible for its users or            */
/*  applications created by its users, or damages caused by the environment      */
/*  or programs made in the environment.                                         */
/*                                                                               */
/*********************************************************************************/

#include <gtest/gtest.h>

#include <iostream>

#include "ResourceTransformations/VisualShader/visual_shader_nodes.h"

TEST(VisualShaderNodesTest, Test_VisualShaderNodeFloatConstant_generate_code) {
  VisualShaderNodeFloatConstant node;
  node.set_constant(1.0);
  std::string code{node.generate_code(0, {}, {"c"})};
  std::string expected_code{"\tc = 1.000000;\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeIntConstant_generate_code) {
  VisualShaderNodeIntConstant node;
  node.set_constant(1);
  std::string code{node.generate_code(0, {}, {"c"})};
  std::string expected_code{"\tc = 1;\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeUIntConstant_generate_code) {
  VisualShaderNodeUIntConstant node;
  node.set_constant(1);
  std::string code{node.generate_code(0, {}, {"c"})};
  std::string expected_code{"\tc = 1u;\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeBooleanConstant_generate_code) {
  VisualShaderNodeBooleanConstant node;
  node.set_constant(true);
  std::string code{node.generate_code(0, {}, {"c"})};
  std::string expected_code{"\tc = true;\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeColorConstant_generate_code) {
  VisualShaderNodeColorConstant node;
  node.set_constant(TColor(1.0, 2.0, 3.0, 0.0));
  std::string code{node.generate_code(0, {}, {"c"})};
  std::string expected_code{"\tc = vec4(1.000000, 2.000000, 3.000000, 0.000000);\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeVec2Constant_generate_code) {
  VisualShaderNodeVec2Constant node;
  node.set_constant(TVector2(1.0, 2.0));
  std::string code{node.generate_code(0, {}, {"c"})};
  std::string expected_code{"\tc = vec2(1.000000, 2.000000);\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeVec3Constant_generate_code) {
  VisualShaderNodeVec3Constant node;
  node.set_constant(TVector3(1.0, 2.0, 3.0));
  std::string code{node.generate_code(0, {}, {"c"})};
  std::string expected_code{"\tc = vec3(1.000000, 2.000000, 3.000000);\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeVec4Constant_generate_code) {
  VisualShaderNodeVec4Constant node;
  node.set_constant(TVector4(1.0, 2.0, 3.0, 4.0));
  std::string code{node.generate_code(0, {}, {"c"})};
  std::string expected_code{"\tc = vec4(1.000000, 2.000000, 3.000000, 4.000000);\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNodesTest, Test_VisualShaderNodeFloatOp_generate_code) {
  VisualShaderNodeFloatOp node;
  std::vector<std::string> input_vars = {"a", "b"};
  std::vector<std::string> output_vars = {"c"};
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{"\tc = a + b;\n"};
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
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{"\tc = a + b;\n"};
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
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{"\tc = a + b;\n"};
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
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{"\tc = a + b;\n"};
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

TEST(VisualShaderNodesTest, Test_VisualShaderNodeFloatFunc_generate_code) {
  VisualShaderNodeFloatFunc node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{"\tb = sin(a);\n"};
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
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{"\tb = sign(a);\n"};
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
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{"\tb = -(a);\n"};
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
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{"\tb = normalize(a);\n"};
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
