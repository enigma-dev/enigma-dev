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
#include <chrono>  // For timing

#include "ResourceTransformations/VisualShader/visual_shader.h"
#include "ResourceTransformations/VisualShader/visual_shader_nodes.h"
#include "ResourceTransformations/VisualShader/vs_noise_nodes.h"

TEST(VisualShaderTest, Test_generate_shader) {
  // Create a time input
  VisualShaderNodeInput vsni;
  vsni.set_input_name("time");
  std::shared_ptr<VisualShaderNode> vsni_ptr = std::make_shared<VisualShaderNodeInput>(vsni);

  // Create a sin func
  VisualShaderNodeFloatFunc vsnff;
  std::shared_ptr<VisualShaderNode> vsnff_ptr = std::make_shared<VisualShaderNodeFloatFunc>(vsnff);

  // Create a divide operator
  VisualShaderNodeFloatOp vsnfo;
  vsnfo.set_operator(VisualShaderNodeFloatOp::Operator::OP_DIV);
  std::shared_ptr<VisualShaderNode> vsnfo_ptr = std::make_shared<VisualShaderNodeFloatOp>(vsnfo);

  // Create a UV input
  VisualShaderNodeInput vsni2;
  vsni2.set_input_name("uv");
  std::shared_ptr<VisualShaderNode> vsni2_ptr = std::make_shared<VisualShaderNodeInput>(vsni2);

  // Create a Value Noise node
  VisualShaderNodeValueNoise vsnvn;
  std::shared_ptr<VisualShaderNode> vsnvn_ptr = std::make_shared<VisualShaderNodeValueNoise>(vsnvn);

  // Create a subtract operator
  VisualShaderNodeFloatOp vsnfo2;
  vsnfo2.set_operator(VisualShaderNodeFloatOp::Operator::OP_SUB);
  std::shared_ptr<VisualShaderNode> vsnfo2_ptr = std::make_shared<VisualShaderNodeFloatOp>(vsnfo2);

  // Create a float func
  VisualShaderNodeFloatFunc vsnff2;
  vsnff2.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ROUND);
  std::shared_ptr<VisualShaderNode> vsnff2_ptr = std::make_shared<VisualShaderNodeFloatFunc>(vsnff2);

  // Create the graph.
  VisualShader vs;

  // Populate the graph.
  int id{vs.get_valid_node_id()};
  EXPECT_EQ(id, 1);
  vs.add_node(vsni_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 2);
  vs.add_node(vsnff_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 3);
  vs.add_node(vsnfo_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 4);
  vs.add_node(vsni2_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 5);
  vs.add_node(vsnvn_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 6);
  vs.add_node(vsnfo2_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 7);
  vs.add_node(vsnff2_ptr, {0.0f, 0.0f}, id);

  // Connect the nodes.

  // Connect `output port 0` of time input to `input port 0` of sin func.
  int lookup_id1{vs.find_node_id(vsni_ptr)};
  int lookup_id2{vs.find_node_id(vsnff_ptr)};
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of sin func to `input port 0` of divide operator.
  lookup_id1 = vs.find_node_id(vsnff_ptr);
  lookup_id2 = vs.find_node_id(vsnfo_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of divide operator to `input port 1` of subtract
  // operator.
  lookup_id1 = vs.find_node_id(vsnfo_ptr);
  lookup_id2 = vs.find_node_id(vsnfo2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 1);

  // Connect `output port 0` of UV input to `input port 0` of value noise node.
  lookup_id1 = vs.find_node_id(vsni2_ptr);
  lookup_id2 = vs.find_node_id(vsnvn_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of value noise node to `input port 0` of subtract
  // operator.
  lookup_id1 = vs.find_node_id(vsnvn_ptr);
  lookup_id2 = vs.find_node_id(vsnfo2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of subtract operator to `input port 0` of float
  // func.
  lookup_id1 = vs.find_node_id(vsnfo2_ptr);
  lookup_id2 = vs.find_node_id(vsnff2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of float func to `input port 0` of output node.
  lookup_id1 = vs.find_node_id(vsnff2_ptr);
  vs.connect_nodes(lookup_id1, 0, 0, 0);

  auto start_time {std::chrono::high_resolution_clock::now()};

  // Generate the shader.
  bool status{vs.generate_shader()};
  EXPECT_EQ(status, true);

  auto end_time {std::chrono::high_resolution_clock::now()};
    
  // Calculate the duration in microseconds
  auto duration {std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count()};
  
  EXPECT_LE(duration, 500);

  // Get the shader.
  std::string generated_code{vs.get_code()};
  std::string expected_code{
    "in vec2 FragCoord;\n"

    "uniform float uTime;\n"

    "float noise_random_value(vec2 uv) {\n"
    "\treturn fract(sin(dot(uv, vec2(12.9898, 78.233)))*43758.5453);\n"
    "}\n\n"

    "float noise_interpolate(float a, float b, float t) {\n"
    "\treturn (1.0-t)*a + (t*b);\n"
    "}\n\n"

    "float value_noise(vec2 uv) {\n"
    "\tvec2 i = floor(uv);\n"
    "\tvec2 f = fract(uv);\n"
    "\tf = f * f * (3.0 - 2.0 * f);\n"
    "\t\n"
    "\tuv = abs(fract(uv) - 0.5);\n"
    "\tvec2 c0 = i + vec2(0.0, 0.0);\n"
    "\tvec2 c1 = i + vec2(1.0, 0.0);\n"
    "\tvec2 c2 = i + vec2(0.0, 1.0);\n"
    "\tvec2 c3 = i + vec2(1.0, 1.0);\n"
    "\tfloat r0 = noise_random_value(c0);\n"
    "\tfloat r1 = noise_random_value(c1);\n"
    "\tfloat r2 = noise_random_value(c2);\n"
    "\tfloat r3 = noise_random_value(c3);\n"
    "\t\n"
    "\tfloat bottom_of_grid = noise_interpolate(r0, r1, f.x);\n"
    "\tfloat top_of_grid = noise_interpolate(r2, r3, f.x);\n"
    "\tfloat t = noise_interpolate(bottom_of_grid, top_of_grid, f.y);\n"
    "\treturn t;\n"
    "}\n\n"

    "void generate_value_noise_float(vec2 uv, float scale, out float "
    "out_buffer) {\n"
    "\tfloat t = 0.0;\n"
    "\t\n"
    "\tfloat freq = pow(2.0, float(0));\n"
    "\tfloat amp = pow(0.5, float(3-0));\n"
    "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n"
    "\t\n"
    "\tfreq = pow(2.0, float(1));\n"
    "\tamp = pow(0.5, float(3-1));\n"
    "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n"
    "\t\n"
    "\tfreq = pow(2.0, float(2));\n"
    "\tamp = pow(0.5, float(3-2));\n"
    "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n"
    "\t\n"
    "\tout_buffer = t;\n"
    "}\n\n"

    "out vec4 FragColor;\n"

    "\nvoid main() {\n"
    "// Input:4\n"
    "\tvec2 var_from_n4_p0 = FragCoord;\n\n\n"
    "// ValueNoise:5\n"
    "\t// Value Noise\n"
    "\tfloat out_buffer_n5 = 0.0;\n"
    "\tgenerate_value_noise_float(var_from_n4_p0, 100.000000, out_buffer_n5);\n"
    "\tvec4 var_from_n5_p0 = vec4(out_buffer_n5, out_buffer_n5, out_buffer_n5, 1.0);\n"
    "\t\n\n\n"
    "// Input:1\n"
    "\tfloat var_from_n1_p0 = uTime;\n\n\n"
    "// FloatFunc:2\n"
    "\tfloat var_from_n2_p0 = sin(var_from_n1_p0);\n\n\n"
    "// FloatOp:3\n"
    "\tfloat var_to_n3_p1 = 0.00000;\n"
    "\tfloat var_from_n3_p0 = var_from_n2_p0 / var_to_n3_p1;\n\n\n"
    "// FloatOp:6\n"
    "\tfloat var_from_n6_p0 = var_from_n5_p0.x - var_from_n3_p0;\n\n\n"
    "// FloatFunc:7\n"
    "\tfloat var_from_n7_p0 = round(var_from_n6_p0);\n\n\n"
    "// Output:0\n"
    "\tFragColor = vec4(var_from_n7_p0);\n\n\n"
    "}\n\n"};
  EXPECT_EQ(generated_code, expected_code);
}

TEST(VisualShaderTest, Test_generate_preview_shader) {
  // Create a time input
  VisualShaderNodeInput vsni;
  vsni.set_input_name("time");
  std::shared_ptr<VisualShaderNode> vsni_ptr = std::make_shared<VisualShaderNodeInput>(vsni);

  // Create a sin func
  VisualShaderNodeFloatFunc vsnff;
  std::shared_ptr<VisualShaderNode> vsnff_ptr = std::make_shared<VisualShaderNodeFloatFunc>(vsnff);

  // Create a divide operator
  VisualShaderNodeFloatOp vsnfo;
  vsnfo.set_operator(VisualShaderNodeFloatOp::Operator::OP_DIV);
  std::shared_ptr<VisualShaderNode> vsnfo_ptr = std::make_shared<VisualShaderNodeFloatOp>(vsnfo);

  // Create a UV input
  VisualShaderNodeInput vsni2;
  vsni2.set_input_name("uv");
  std::shared_ptr<VisualShaderNode> vsni2_ptr = std::make_shared<VisualShaderNodeInput>(vsni2);

  // Create a Value Noise node
  VisualShaderNodeValueNoise vsnvn;
  std::shared_ptr<VisualShaderNode> vsnvn_ptr = std::make_shared<VisualShaderNodeValueNoise>(vsnvn);

  // Create a subtract operator
  VisualShaderNodeFloatOp vsnfo2;
  vsnfo2.set_operator(VisualShaderNodeFloatOp::Operator::OP_SUB);
  std::shared_ptr<VisualShaderNode> vsnfo2_ptr = std::make_shared<VisualShaderNodeFloatOp>(vsnfo2);

  // Create a float func
  VisualShaderNodeFloatFunc vsnff2;
  vsnff2.set_function(VisualShaderNodeFloatFunc::Function::FUNC_ROUND);
  std::shared_ptr<VisualShaderNode> vsnff2_ptr = std::make_shared<VisualShaderNodeFloatFunc>(vsnff2);

  // Create the graph.
  VisualShader vs;

  // Populate the graph.
  int id{vs.get_valid_node_id()};
  EXPECT_EQ(id, 1);
  vs.add_node(vsni_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 2);
  vs.add_node(vsnff_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 3);
  vs.add_node(vsnfo_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 4);
  vs.add_node(vsni2_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 5);
  vs.add_node(vsnvn_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 6);
  vs.add_node(vsnfo2_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 7);
  vs.add_node(vsnff2_ptr, {0.0f, 0.0f}, id);

  // Connect the nodes.

  // Connect `output port 0` of time input to `input port 0` of sin func.
  int lookup_id1{vs.find_node_id(vsni_ptr)};
  int lookup_id2{vs.find_node_id(vsnff_ptr)};
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of sin func to `input port 0` of divide operator.
  lookup_id1 = vs.find_node_id(vsnff_ptr);
  lookup_id2 = vs.find_node_id(vsnfo_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of divide operator to `input port 1` of subtract
  // operator.
  lookup_id1 = vs.find_node_id(vsnfo_ptr);
  lookup_id2 = vs.find_node_id(vsnfo2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 1);

  // Connect `output port 0` of UV input to `input port 0` of value noise node.
  lookup_id1 = vs.find_node_id(vsni2_ptr);
  lookup_id2 = vs.find_node_id(vsnvn_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of value noise node to `input port 0` of subtract
  // operator.
  lookup_id1 = vs.find_node_id(vsnvn_ptr);
  lookup_id2 = vs.find_node_id(vsnfo2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of subtract operator to `input port 0` of float
  // func.
  lookup_id1 = vs.find_node_id(vsnfo2_ptr);
  lookup_id2 = vs.find_node_id(vsnff2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of float func to `input port 0` of output node.
  lookup_id1 = vs.find_node_id(vsnff2_ptr);
  vs.connect_nodes(lookup_id1, 0, 0, 0);

  // Send the sin func node.
  std::string generated_code{vs.generate_preview_shader(vs.find_node_id(vsni_ptr), 0)};
  std::string expected_code {
    "in vec2 FragCoord;\n"
    "uniform float uTime;\n"
    "out vec4 FragColor;\n"
    "\nvoid main() {\n"
    "// Input:1\n"
    "\tfloat var_from_n1_p0 = uTime;\n\n\n"
    "\tFragColor = vec4(vec3(var_from_n1_p0), 1.0);\n"
    "}\n\n"
  };

  EXPECT_EQ(generated_code, expected_code);

  generated_code = vs.generate_preview_shader(vs.find_node_id(vsnff_ptr), 0);
  expected_code = 
    "in vec2 FragCoord;\n"
    "uniform float uTime;\n"
    "out vec4 FragColor;\n"
    "\nvoid main() {\n"
    "// Input:1\n"
    "\tfloat var_from_n1_p0 = uTime;\n\n\n"
    "// FloatFunc:2\n"
    "\tfloat var_from_n2_p0 = sin(var_from_n1_p0);\n\n\n"
    "\tFragColor = vec4(vec3(var_from_n2_p0), 1.0);\n"
    "}\n\n";

  EXPECT_EQ(generated_code, expected_code);

  generated_code = vs.generate_preview_shader(vs.find_node_id(vsnvn_ptr), 0);
  expected_code = 
    "in vec2 FragCoord;\n"
    "uniform float uTime;\n"

    "float noise_random_value(vec2 uv) {\n"
    "\treturn fract(sin(dot(uv, vec2(12.9898, 78.233)))*43758.5453);\n"
    "}\n\n"

    "float noise_interpolate(float a, float b, float t) {\n"
    "\treturn (1.0-t)*a + (t*b);\n"
    "}\n\n"

    "float value_noise(vec2 uv) {\n"
    "\tvec2 i = floor(uv);\n"
    "\tvec2 f = fract(uv);\n"
    "\tf = f * f * (3.0 - 2.0 * f);\n"
    "\t\n"
    "\tuv = abs(fract(uv) - 0.5);\n"
    "\tvec2 c0 = i + vec2(0.0, 0.0);\n"
    "\tvec2 c1 = i + vec2(1.0, 0.0);\n"
    "\tvec2 c2 = i + vec2(0.0, 1.0);\n"
    "\tvec2 c3 = i + vec2(1.0, 1.0);\n"
    "\tfloat r0 = noise_random_value(c0);\n"
    "\tfloat r1 = noise_random_value(c1);\n"
    "\tfloat r2 = noise_random_value(c2);\n"
    "\tfloat r3 = noise_random_value(c3);\n"
    "\t\n"
    "\tfloat bottom_of_grid = noise_interpolate(r0, r1, f.x);\n"
    "\tfloat top_of_grid = noise_interpolate(r2, r3, f.x);\n"
    "\tfloat t = noise_interpolate(bottom_of_grid, top_of_grid, f.y);\n"
    "\treturn t;\n"
    "}\n\n"

    "void generate_value_noise_float(vec2 uv, float scale, out float "
    "out_buffer) {\n"
    "\tfloat t = 0.0;\n"
    "\t\n"
    "\tfloat freq = pow(2.0, float(0));\n"
    "\tfloat amp = pow(0.5, float(3-0));\n"
    "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n"
    "\t\n"
    "\tfreq = pow(2.0, float(1));\n"
    "\tamp = pow(0.5, float(3-1));\n"
    "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n"
    "\t\n"
    "\tfreq = pow(2.0, float(2));\n"
    "\tamp = pow(0.5, float(3-2));\n"
    "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n"
    "\t\n"
    "\tout_buffer = t;\n"
    "}\n\n"

    "out vec4 FragColor;\n"

    "\nvoid main() {\n"
    "// Input:4\n"
    "\tvec2 var_from_n4_p0 = FragCoord;\n\n\n"
    "// ValueNoise:5\n"
    "\t// Value Noise\n"
    "\tfloat out_buffer_n5 = 0.0;\n"
    "\tgenerate_value_noise_float(var_from_n4_p0, 100.000000, out_buffer_n5);\n"
    "\tvec4 var_from_n5_p0 = vec4(out_buffer_n5, out_buffer_n5, out_buffer_n5, 1.0);\n"
    "\t\n\n\n"
    "\tFragColor = vec4(var_from_n5_p0.xyz, 1.0);\n"
    "}\n\n";

  EXPECT_EQ(generated_code, expected_code);
}

TEST(VisualShaderTest, Test_find_node_id) {
  VisualShader vs;

  VisualShaderNodeInput vsni;
  std::shared_ptr<VisualShaderNode> vsni_ptr = std::make_shared<VisualShaderNodeInput>(vsni);
  vs.add_node(vsni_ptr, {0.0f, 0.0f}, vs.get_valid_node_id());

  VisualShaderNodeInput vsni2;
  std::shared_ptr<VisualShaderNode> vsni2_ptr = std::make_shared<VisualShaderNodeInput>(vsni2);
  vs.add_node(vsni2_ptr, {0.0f, 0.0f}, vs.get_valid_node_id());

  int lookup_id1{vs.find_node_id(vsni_ptr)};
  int lookup_id2{vs.find_node_id(vsni2_ptr)};
  EXPECT_EQ(lookup_id1, 1);
  EXPECT_EQ(lookup_id2, 2);
}

TEST(VisualShaderTest, Test_remove_node) {
  // Add 10 nodes to the graph.
  VisualShader vs;

  for (int i = 1; i <= 10; i++) {
    VisualShaderNodeInput vsni;
    std::shared_ptr<VisualShaderNode> vsni_ptr = std::make_shared<VisualShaderNodeInput>(vsni);
    vs.add_node(vsni_ptr, {0.0f, 0.0f}, i);
  }

  // Remove the nodes from the graph.
  for (int i = 1; i <= 10; i++) {
    vs.remove_node(i);
  }

  // Get a valid node id.
  int id{vs.get_valid_node_id()};  // This should be 1 as the previous addition
                                   // was failed due to id 0 already being taken
                                   // (output node id is always 0).
  EXPECT_EQ(id, 1);
}

TEST(VisualShaderTest, Test_VisualShader) {
  // Create a node to insert into the graph.
  VisualShaderNodeInput vsni;
  std::shared_ptr<VisualShaderNode> vsni_ptr = std::make_shared<VisualShaderNodeInput>(vsni);

  VisualShader vs;

  // Add the node to the graph with id 0.
  vs.add_node(vsni_ptr, {0.0f, 0.0f}, 0);

  int id{vs.get_valid_node_id()};  // This should be 1 as the previous addition
                                   // was failed due to id 0 already being taken
                                   // (output node id is always 0).
  EXPECT_EQ(id, 1);

  // Add the node to the graph with id 1.
  vs.add_node(vsni_ptr, {0.0f, 0.0f}, 1);

  // The node should be in the graph.
  EXPECT_EQ(vs.get_node(1), vsni_ptr);

  // Remove the node from the graph.
  vs.remove_node(1);

  // The node should not be in the graph.
  EXPECT_EQ(vs.get_node(1), nullptr);
}

TEST(VisualShaderTest, Test_add_node) {
  // Create a node to insert into the graph.
  VisualShaderNodeInput vsni;
  std::shared_ptr<VisualShaderNode> vsni_ptr = std::make_shared<VisualShaderNodeInput>(vsni);

  VisualShader vs;

  // Add the node to the graph.
  vs.add_node(vsni_ptr, {0.0f, 0.0f}, 1);

  // The node should be in the graph.
  EXPECT_EQ(vs.get_node(1), vsni_ptr);

  // Add another node to the graph.
  vs.add_node(vsni_ptr, {0.0f, 0.0f}, 2);

  // The node should be in the graph.
  EXPECT_EQ(vs.get_node(2), vsni_ptr);
}

TEST(VisualShaderTest, Test_get_valid_node_id) {
  // Create nodes to insert into the graph.
  VisualShaderNodeInput vsni;
  std::shared_ptr<VisualShaderNode> vsni_ptr = std::make_shared<VisualShaderNodeInput>(vsni);

  VisualShaderNodeInput vsni2;
  std::shared_ptr<VisualShaderNode> vsni2_ptr = std::make_shared<VisualShaderNodeInput>(vsni2);

  VisualShader vs;

  // The id should be 1 since the output node is always created with id 0.
  int id{vs.get_valid_node_id()};
  EXPECT_EQ(id, 1);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 1);

  // Add a node to the graph.
  vs.add_node(vsni_ptr, {0.0f, 0.0f}, id);

  // The id should be 2 now.
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 2);

  // Add another node to the graph.
  vs.add_node(vsni2_ptr, {0.0f, 0.0f}, id);

  // The id should be 3 now.
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 3);

  // Remove the node with id 1: `vsni2_ptr`.
  vs.remove_node(1);

  // The id should still be 3 since we the generated id is always the maximum id
  // + 1.
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 3);

  // Remove the node with id 2: `vsni_ptr`.
  vs.remove_node(2);

  // The id should be 1 now as both nodes with id 1 and 2 are removed.
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 1);
}

TEST(VisualShaderTest, Test_add_multiple_noise_nodes) {
  // Create a UV input
  VisualShaderNodeInput vsni;
  vsni.set_input_name("uv");
  std::shared_ptr<VisualShaderNode> vsni_ptr = std::make_shared<VisualShaderNodeInput>(vsni);

  // Create a Value Noise node
  VisualShaderNodeValueNoise vsnvn;
  std::shared_ptr<VisualShaderNode> vsnvn_ptr = std::make_shared<VisualShaderNodeValueNoise>(vsnvn);

  // Create another Value Noise node
  VisualShaderNodeValueNoise vsnvn2;
  std::shared_ptr<VisualShaderNode> vsnvn2_ptr = std::make_shared<VisualShaderNodeValueNoise>(vsnvn2);

  // Create another Value Noise node
  VisualShaderNodeValueNoise vsnvn3;
  std::shared_ptr<VisualShaderNode> vsnvn3_ptr = std::make_shared<VisualShaderNodeValueNoise>(vsnvn3);

  // Create a Vector3 for Value Noise node
  VisualShaderNodeVectorCompose vsnvc;
  std::shared_ptr<VisualShaderNode> vsnvc_ptr = std::make_shared<VisualShaderNodeVectorCompose>(vsnvc);

  // Create a Perlin Noise node
  VisualShaderNodePerlinNoise vsnpn;
  std::shared_ptr<VisualShaderNode> vsnpn_ptr = std::make_shared<VisualShaderNodePerlinNoise>(vsnpn);

  // Create another Perlin Noise node
  VisualShaderNodePerlinNoise vsnpn2;
  std::shared_ptr<VisualShaderNode> vsnpn2_ptr = std::make_shared<VisualShaderNodePerlinNoise>(vsnpn2);

  // Create another Perlin Noise node
  VisualShaderNodePerlinNoise vsnpn3;
  std::shared_ptr<VisualShaderNode> vsnpn3_ptr = std::make_shared<VisualShaderNodePerlinNoise>(vsnpn3);

  // Create a Vector3 for Perlin Noise node
  VisualShaderNodeVectorCompose vsnvc2;
  std::shared_ptr<VisualShaderNode> vsnvc2_ptr = std::make_shared<VisualShaderNodeVectorCompose>(vsnvc2);

  // Create a Worley Noise node
  VisualShaderNodeVoronoiNoise vsnwn;
  std::shared_ptr<VisualShaderNode> vsnwn_ptr = std::make_shared<VisualShaderNodeVoronoiNoise>(vsnwn);

  // Create another Worley Noise node
  VisualShaderNodeVoronoiNoise vsnwn2;
  std::shared_ptr<VisualShaderNode> vsnwn2_ptr = std::make_shared<VisualShaderNodeVoronoiNoise>(vsnwn2);

  // Create another Worley Noise node
  VisualShaderNodeVoronoiNoise vsnwn3;
  std::shared_ptr<VisualShaderNode> vsnwn3_ptr = std::make_shared<VisualShaderNodeVoronoiNoise>(vsnwn3);

  // Create a Vector3 for Worley Noise node
  VisualShaderNodeVectorCompose vsnvc3;
  std::shared_ptr<VisualShaderNode> vsnvc3_ptr = std::make_shared<VisualShaderNodeVectorCompose>(vsnvc3);

  // Create the graph.
  VisualShader vs;

  // Populate the graph.
  int id{vs.get_valid_node_id()};
  EXPECT_EQ(id, 1);
  vs.add_node(vsni_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 2);
  vs.add_node(vsnvn_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 3);
  vs.add_node(vsnvn2_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 4);
  vs.add_node(vsnvn3_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 5);

  vs.add_node(vsnvc_ptr, {0.0f, 0.0f}, id);

  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 6);
  vs.add_node(vsnpn_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 7);
  vs.add_node(vsnpn2_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 8);
  vs.add_node(vsnpn3_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 9);

  vs.add_node(vsnvc2_ptr, {0.0f, 0.0f}, id);

  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 10);
  vs.add_node(vsnwn_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 11);
  vs.add_node(vsnwn2_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 12);
  vs.add_node(vsnwn3_ptr, {0.0f, 0.0f}, id);
  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 13);

  vs.add_node(vsnvc3_ptr, {0.0f, 0.0f}, id);

  // Connect the nodes.

  int lookup_id1{vs.find_node_id(vsni_ptr)};

  // Connect `output port 0` of UV input to `input port 0` of value noise node.
  int lookup_id2{vs.find_node_id(vsnvn_ptr)};
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of UV input to `input port 0` of second value noise node.
  lookup_id2 = vs.find_node_id(vsnvn2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of UV input to `input port 0` of third value noise node.
  lookup_id2 = vs.find_node_id(vsnvn3_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of UV input to `input port 0` of perlin noise node.
  lookup_id2 = vs.find_node_id(vsnpn_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of UV input to `input port 0` of second perlin noise node.
  lookup_id2 = vs.find_node_id(vsnpn2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of UV input to `input port 0` of third perlin noise node.
  lookup_id2 = vs.find_node_id(vsnpn3_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of UV input to `input port 0` of worley noise node.
  lookup_id2 = vs.find_node_id(vsnwn_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of UV input to `input port 0` of second worley noise node.
  lookup_id2 = vs.find_node_id(vsnwn2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of UV input to `input port 0` of third worley noise node.
  lookup_id2 = vs.find_node_id(vsnwn3_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of value noise node to `input port 0` of vector compose node.
  // Connect `output port 0` of second value noise node to `input port 1` of vector compose node.
  // Connect `output port 0` of third value noise node to `input port 2` of vector compose node.
  lookup_id1 = vs.find_node_id(vsnvn_ptr);
  lookup_id2 = vs.find_node_id(vsnvc_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);
  lookup_id1 = vs.find_node_id(vsnvn2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 1);
  lookup_id1 = vs.find_node_id(vsnvn3_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 2);

  // Connect `output port 0` of perlin noise node to `input port 0` of vector compose node.
  // Connect `output port 0` of second perlin noise node to `input port 1` of vector compose node.
  // Connect `output port 0` of third perlin noise node to `input port 2` of vector compose node.
  lookup_id1 = vs.find_node_id(vsnpn_ptr);
  lookup_id2 = vs.find_node_id(vsnvc2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);
  lookup_id1 = vs.find_node_id(vsnpn2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 1);
  lookup_id1 = vs.find_node_id(vsnpn3_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 2);

  // Connect `output port 0` of worley noise node to `input port 0` of vector compose node.
  // Connect `output port 0` of second worley noise node to `input port 1` of vector compose node.
  // Connect `output port 0` of third worley noise node to `input port 2` of vector compose node.
  lookup_id1 = vs.find_node_id(vsnwn_ptr);
  lookup_id2 = vs.find_node_id(vsnvc3_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);
  lookup_id1 = vs.find_node_id(vsnwn2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 1);
  lookup_id1 = vs.find_node_id(vsnwn3_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 2);

  // Create a Vector Operation node and add it to the graph.
  VisualShaderNodeVectorOp vsnvo;
  std::shared_ptr<VisualShaderNode> vsnvo_ptr = std::make_shared<VisualShaderNodeVectorOp>(vsnvo);

  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 14);
  vs.add_node(vsnvo_ptr, {0.0f, 0.0f}, id);

  // Connect `output port 0` of Value Noise Vector Compose node to `input port 0` of Vector Operation node.
  lookup_id1 = vs.find_node_id(vsnvc_ptr);
  lookup_id2 = vs.find_node_id(vsnvo_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of Perlin Noise Vector Compose node to `input port 1` of Vector Operation node.
  lookup_id1 = vs.find_node_id(vsnvc2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 1);

  // Create a Vector Operation node and add it to the graph.
  VisualShaderNodeVectorOp vsnvo2;
  std::shared_ptr<VisualShaderNode> vsnvo2_ptr = std::make_shared<VisualShaderNodeVectorOp>(vsnvo2);

  id = vs.get_valid_node_id();
  EXPECT_EQ(id, 15);
  vs.add_node(vsnvo2_ptr, {0.0f, 0.0f}, id);

  // Connect `output port 0` of first Vector Operation node to `input port 0` of second Vector Operation node.
  lookup_id1 = vs.find_node_id(vsnvo_ptr);
  lookup_id2 = vs.find_node_id(vsnvo2_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

  // Connect `output port 0` of Worley Noise Vector Compose node to `input port 1` of second Vector Operation node.
  lookup_id1 = vs.find_node_id(vsnvc3_ptr);
  vs.connect_nodes(lookup_id1, 0, lookup_id2, 1);

  // Connect `output port 0` of second Vector Operation node to `input port 0` of output node.
  lookup_id1 = vs.find_node_id(vsnvo2_ptr);
  vs.connect_nodes(lookup_id1, 0, 0, 0);

  auto start_time {std::chrono::high_resolution_clock::now()};

  // Generate the shader.
  bool status{vs.generate_shader()};
  EXPECT_EQ(status, true);

  auto end_time {std::chrono::high_resolution_clock::now()};
    
  // Calculate the duration in microseconds
  auto duration {std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count()};
  
  EXPECT_LE(duration, 500);

  // Get the shader.
  std::string generated_code{vs.get_code()};
  std::string expected_code{
    "in vec2 FragCoord;\n"
    "uniform float uTime;\n"
    "float noise_random_value(vec2 uv) {\n"
    "\treturn fract(sin(dot(uv, vec2(12.9898, 78.233)))*43758.5453);\n"
    "}\n\n"
    "float noise_interpolate(float a, float b, float t) {\n"
    "\treturn (1.0-t)*a + (t*b);\n"
    "}\n\n"
    "float value_noise(vec2 uv) {\n"
    "\tvec2 i = floor(uv);\n"
    "\tvec2 f = fract(uv);\n"
    "\tf = f * f * (3.0 - 2.0 * f);\n"
    "\t\n"
    "\tuv = abs(fract(uv) - 0.5);\n"
    "\tvec2 c0 = i + vec2(0.0, 0.0);\n"
    "\tvec2 c1 = i + vec2(1.0, 0.0);\n"
    "\tvec2 c2 = i + vec2(0.0, 1.0);\n"
    "\tvec2 c3 = i + vec2(1.0, 1.0);\n"
    "\tfloat r0 = noise_random_value(c0);\n"
    "\tfloat r1 = noise_random_value(c1);\n"
    "\tfloat r2 = noise_random_value(c2);\n"
    "\tfloat r3 = noise_random_value(c3);\n"
    "\t\n"
    "\tfloat bottom_of_grid = noise_interpolate(r0, r1, f.x);\n"
    "\tfloat top_of_grid = noise_interpolate(r2, r3, f.x);\n"
    "\tfloat t = noise_interpolate(bottom_of_grid, top_of_grid, f.y);\n"
    "\treturn t;\n"
    "}\n\n"
    "void generate_value_noise_float(vec2 uv, float scale, out float "
    "out_buffer) {\n"
    "\tfloat t = 0.0;\n"
    "\t\n"
    "\tfloat freq = pow(2.0, float(0));\n"
    "\tfloat amp = pow(0.5, float(3-0));\n"
    "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n"
    "\t\n"
    "\tfreq = pow(2.0, float(1));\n"
    "\tamp = pow(0.5, float(3-1));\n"
    "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n"
    "\t\n"
    "\tfreq = pow(2.0, float(2));\n"
    "\tamp = pow(0.5, float(3-2));\n"
    "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n"
    "\t\n"
    "\tout_buffer = t;\n"
    "}\n\n"
    "vec2 perlin_noise_dir(vec2 p) {\n"
    "\tp = mod(p, 289.0);\n"
    "\t\n"
    "\tfloat x = mod((34.0 * p.x + 1.0) * p.x, 289.0) + p.y;\n"
    "\t\n"
    "\tx = mod((34.0 * x + 1.0) * x, 289.0);\n"
    "\t\n"
    "\tx = fract(x / 41.0) * 2.0 - 1.0;\n"
    "\t\n"
    "\treturn normalize(vec2(x - floor(x + 0.5), abs(x) - 0.5));\n"
    "}\n\n"
    "float perlin_noise(vec2 p) {\n"
    "\tvec2 ip = floor(p);\n"
    "\tvec2 fp = fract(p);\n"
    "\tfloat d00 = dot(perlin_noise_dir(ip), fp);\n"
    "\tfloat d01 = dot(perlin_noise_dir(ip + vec2(0, 1)), fp - vec2(0, 1));\n"
    "\tfloat d10 = dot(perlin_noise_dir(ip + vec2(1, 0)), fp - vec2(1, 0));\n"
    "\tfloat d11 = dot(perlin_noise_dir(ip + vec2(1, 1)), fp - vec2(1, 1));\n"
    "\tfp = fp * fp * fp * (fp * (fp * 6.0 - 15.0) + 10.0);\n"
    "\t\n"
    "\treturn mix(mix(d00, d01, fp.y), mix(d10, d11, fp.y), fp.x);\n"
    "}\n\n"
    "void generate_perlin_noise_float(vec2 uv, float scale, out float out_buffer) {\n"
    "\tout_buffer = perlin_noise(uv * scale) + 0.5;\n"
    "}\n\n"
    "vec2 voronoi_noise_random_vector(vec2 uv, float offset) {\n"
    "\tmat2 m = mat2(15.27, 47.63, 99.41, 89.98);\n"
    "\t\n"
    "\tuv = fract(sin(m * uv) * 46839.32);\n"
    "\t\n"
    "\treturn vec2(sin(uv.y*+offset)*0.5+0.5, cos(uv.x*offset)*0.5+0.5);\n"
    "}\n\n"
    "void generate_voronoi_noise_float(vec2 uv, float angle_offset, float cell_density, out float out_buffer, out float cells) {\n"
    "\tvec2 g = floor(uv * cell_density);\n"
    "\tvec2 f = fract(uv * cell_density);\n"
    "\tfloat t = 8.0;\n"
    "\tvec3 res = vec3(8.0, 0.0, 0.0);\n"
    "\t\n"
    "\tfor(int y=-1; y<=1; y++) {\n"
    "\t\tfor(int x=-1; x<=1; x++) {\n"
    "\t\t\tvec2 lattice = vec2(x,y);\n"
    "\t\t\tvec2 offset = voronoi_noise_random_vector(lattice + g, angle_offset);\n"
    "\t\t\tfloat d = distance(lattice + offset, f);\n"
    "\t\t\tif(d < res.x) {\n"
    "\t\t\t\tres = vec3(d, offset.x, offset.y);\n"
    "\t\t\t\tout_buffer = res.x;\n"
    "\t\t\t\tcells = res.y;\n"
    "\t\t\t}\n"
    "\t\t}\n"
    "\t}\n"
    "}\n\n"

    "out vec4 FragColor;\n"

    "\nvoid main() {\n"
    "// Input:1\n"

    "\tvec2 var_from_n1_p0 = FragCoord;\n\n\n"

    "// ValueNoise:2\n"

    "\t// Value Noise\n"
    "\tfloat out_buffer_n2 = 0.0;\n"
    "\tgenerate_value_noise_float(var_from_n1_p0, 100.000000, out_buffer_n2);\n"
    "\tvec4 var_from_n2_p0 = vec4(out_buffer_n2, out_buffer_n2, out_buffer_n2, 1.0);\n"
    "\t\n\n\n"

    "// ValueNoise:3\n"
    "\t// Value Noise\n"
    "\tfloat out_buffer_n3 = 0.0;\n"
    "\tgenerate_value_noise_float(var_from_n1_p0, 100.000000, out_buffer_n3);\n"
    "\tvec4 var_from_n3_p0 = vec4(out_buffer_n3, out_buffer_n3, out_buffer_n3, 1.0);\n"
    "\t\n\n\n"

    "// ValueNoise:4\n"
    "\t// Value Noise\n"
    "\tfloat out_buffer_n4 = 0.0;\n"
    "\tgenerate_value_noise_float(var_from_n1_p0, 100.000000, out_buffer_n4);\n"
    "\tvec4 var_from_n4_p0 = vec4(out_buffer_n4, out_buffer_n4, out_buffer_n4, 1.0);\n"
    "\t\n\n\n"

    "// VectorCompose:5\n"
    "\tvec3 var_from_n5_p0 = vec3(var_from_n2_p0.x, var_from_n3_p0.x, var_from_n4_p0.x);\n"
    "\n\n"

    "// PerlinNoise:6\n"
    "\t// Perlin Noise\n"
    "\tfloat out_buffer_n6 = 0.0;\n"
    "\tgenerate_perlin_noise_float(var_from_n1_p0, 10.000000, out_buffer_n6);\n"
    "\tvec4 var_from_n6_p0 = vec4(out_buffer_n6, out_buffer_n6, out_buffer_n6, 1.0);\n"
    "\t\n\n\n"

    "// PerlinNoise:7\n"
    "\t// Perlin Noise\n"
    "\tfloat out_buffer_n7 = 0.0;\n"
    "\tgenerate_perlin_noise_float(var_from_n1_p0, 10.000000, out_buffer_n7);\n"
    "\tvec4 var_from_n7_p0 = vec4(out_buffer_n7, out_buffer_n7, out_buffer_n7, 1.0);\n"
    "\t\n\n\n"

    "// PerlinNoise:8\n"
    "\t// Perlin Noise\n"
    "\tfloat out_buffer_n8 = 0.0;\n"
    "\tgenerate_perlin_noise_float(var_from_n1_p0, 10.000000, out_buffer_n8);\n"
    "\tvec4 var_from_n8_p0 = vec4(out_buffer_n8, out_buffer_n8, out_buffer_n8, 1.0);\n"
    "\t\n\n\n"

    "// VectorCompose:9\n"
    "\tvec3 var_from_n9_p0 = vec3(var_from_n6_p0.x, var_from_n7_p0.x, var_from_n8_p0.x);\n"
    "\n\n"

    "// VectorOp:14\n"
    "\tvec3 var_from_n14_p0 = var_from_n5_p0 + var_from_n9_p0;\n"
    "\n\n"

    "// VoronoiNoise:10\n"
    "\t// Voronoi Noise\n"
    "\tfloat out_buffer_n10 = 0.0;\n"
    "\tfloat cells_n10 = 0.0; // TODO: How we can use this?\n"
    "\tgenerate_voronoi_noise_float(var_from_n1_p0, 10.000000, 10.000000, out_buffer_n10, cells_n10);\n"
    "\tvec4 var_from_n10_p0 = vec4(out_buffer_n10, out_buffer_n10, out_buffer_n10, 1.0);\n"
    "\t\n\n\n"

    "// VoronoiNoise:11\n"
    "\t// Voronoi Noise\n"
    "\tfloat out_buffer_n11 = 0.0;\n"
    "\tfloat cells_n11 = 0.0; // TODO: How we can use this?\n"
    "\tgenerate_voronoi_noise_float(var_from_n1_p0, 10.000000, 10.000000, out_buffer_n11, cells_n11);\n"
    "\tvec4 var_from_n11_p0 = vec4(out_buffer_n11, out_buffer_n11, out_buffer_n11, 1.0);\n"
    "\t\n\n\n"

    "// VoronoiNoise:12\n"
    "\t// Voronoi Noise\n"
    "\tfloat out_buffer_n12 = 0.0;\n"
    "\tfloat cells_n12 = 0.0; // TODO: How we can use this?\n"
    "\tgenerate_voronoi_noise_float(var_from_n1_p0, 10.000000, 10.000000, out_buffer_n12, cells_n12);\n"
    "\tvec4 var_from_n12_p0 = vec4(out_buffer_n12, out_buffer_n12, out_buffer_n12, 1.0);\n"
    "\t\n\n\n"

    "// VectorCompose:13\n"
    "\tvec3 var_from_n13_p0 = vec3(var_from_n10_p0.x, var_from_n11_p0.x, var_from_n12_p0.x);\n"
    "\n\n"

    "// VectorOp:15\n"
    "\tvec3 var_from_n15_p0 = var_from_n14_p0 + var_from_n13_p0;\n"
    "\n\n"

    "// Output:0\n"
    "\tFragColor = vec4(var_from_n15_p0, 1.0);\n\n\n"
    "}\n\n"};
  EXPECT_EQ(generated_code, expected_code);
}
