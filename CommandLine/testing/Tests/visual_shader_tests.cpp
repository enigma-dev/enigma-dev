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

#include "ResourceTransformations/VisualShader/visual_shader.h"
#include "ResourceTransformations/VisualShader/visual_shader_nodes.h"
#include "ResourceTransformations/VisualShader/vs_noise_nodes.h"

#include <gtest/gtest.h>

#include <iostream>

TEST(VisualShaderTest, Test_generate_shader) {
    // Create a time input
    VisualShaderNodeInput vsni;
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
    int id {vs.get_valid_node_id()};
    vs.add_node(vsni_ptr, {0.0f, 0.0f}, id);
    id = vs.get_valid_node_id();
    vs.add_node(vsnff_ptr, {0.0f, 0.0f}, id);
    id = vs.get_valid_node_id();
    vs.add_node(vsnfo_ptr, {0.0f, 0.0f}, id);
    id = vs.get_valid_node_id();
    vs.add_node(vsni2_ptr, {0.0f, 0.0f}, id);
    id = vs.get_valid_node_id();
    vs.add_node(vsnvn_ptr, {0.0f, 0.0f}, id);
    id = vs.get_valid_node_id();
    vs.add_node(vsnfo2_ptr, {0.0f, 0.0f}, id);
    id = vs.get_valid_node_id();
    vs.add_node(vsnff2_ptr, {0.0f, 0.0f}, id);

    // Connect the nodes.
    
    // Connect `output port 0` of time input to `input port 0` of sin func.
    int lookup_id1 {vs.find_node_id(vsni_ptr)};
    int lookup_id2 {vs.find_node_id(vsnff_ptr)};
    vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

    // Connect `output port 0` of sin func to `input port 0` of divide operator.
    lookup_id1 = vs.find_node_id(vsnff_ptr);
    lookup_id2 = vs.find_node_id(vsnfo_ptr);
    vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

    // Connect `output port 0` of divide operator to `input port 1` of subtract operator.
    lookup_id1 = vs.find_node_id(vsnfo_ptr);
    lookup_id2 = vs.find_node_id(vsnfo2_ptr);
    vs.connect_nodes(lookup_id1, 0, lookup_id2, 1);

    // Connect `output port 0` of UV input to `input port 0` of value noise node.
    lookup_id1 = vs.find_node_id(vsni2_ptr);
    lookup_id2 = vs.find_node_id(vsnvn_ptr);
    vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

    // Connect `output port 0` of value noise node to `input port 0` of subtract operator.
    lookup_id1 = vs.find_node_id(vsnvn_ptr);
    lookup_id2 = vs.find_node_id(vsnfo2_ptr);
    vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

    // Connect `output port 0` of subtract operator to `input port 0` of float func.
    lookup_id1 = vs.find_node_id(vsnfo2_ptr);
    lookup_id2 = vs.find_node_id(vsnff2_ptr);
    vs.connect_nodes(lookup_id1, 0, lookup_id2, 0);

    // Connect `output port 0` of float func to `input port 0` of output node.
    lookup_id1 = vs.find_node_id(vsnff2_ptr);
    vs.connect_nodes(lookup_id1, 0, 0, 0);

    // Generate the shader.
    std::string shader {vs.generate_shader()};
    std::cout << shader << std::endl;
}

TEST(VisualShaderTest, Test_find_node_id) {
    VisualShader vs;

    VisualShaderNodeInput vsni;
    std::shared_ptr<VisualShaderNode> vsni_ptr = std::make_shared<VisualShaderNodeInput>(vsni);
    vs.add_node(vsni_ptr, {0.0f, 0.0f}, vs.get_valid_node_id());

    VisualShaderNodeInput vsni2;
    std::shared_ptr<VisualShaderNode> vsni2_ptr = std::make_shared<VisualShaderNodeInput>(vsni2);
    vs.add_node(vsni2_ptr, {0.0f, 0.0f}, vs.get_valid_node_id());

    int lookup_id1 {vs.find_node_id(vsni_ptr)};
    int lookup_id2 {vs.find_node_id(vsni2_ptr)};
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
    int id {vs.get_valid_node_id()}; // This should be 1 as the previous addition was failed due to id 0 already being taken (output node id is always 0).
    EXPECT_EQ(id, 1);
}

TEST(VisualShaderTest, Test_VisualShader) {
    // Create a node to insert into the graph.
    VisualShaderNodeInput vsni;
    std::shared_ptr<VisualShaderNode> vsni_ptr = std::make_shared<VisualShaderNodeInput>(vsni);

    VisualShader vs;

    // Add the node to the graph with id 0.
    vs.add_node(vsni_ptr, {0.0f, 0.0f}, 0);

    int id {vs.get_valid_node_id()}; // This should be 1 as the previous addition was failed due to id 0 already being taken (output node id is always 0).
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
    int id {vs.get_valid_node_id()};
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

    // The id should still be 3 since we the generated id is always the maximum id + 1.
    id = vs.get_valid_node_id();
    EXPECT_EQ(id, 3);

    // Remove the node with id 2: `vsni_ptr`.
    vs.remove_node(2);

    // The id should be 1 now as both nodes with id 1 and 2 are removed.
    id = vs.get_valid_node_id();
    EXPECT_EQ(id, 1);
}
