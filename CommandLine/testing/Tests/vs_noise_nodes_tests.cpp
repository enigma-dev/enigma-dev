/********************************************************************************\
** **
**  Copyright (C) 2024 Saif Kandil (k0T0z) **
** **
**  This file is a part of the ENIGMA Development Environment. **
** **
** **
**  ENIGMA is free software: you can redistribute it and/or modify it under the
**
**  terms of the GNU General Public License as published by the Free Software **
**  Foundation, version 3 of the license or any later version. **
** **
**  This application and its source code is distributed AS-IS, WITHOUT ANY **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more **
**  details. **
** **
**  You should have recieved a copy of the GNU General Public License along **
**  with this code. If not, see <http://www.gnu.org/licenses/> **
** **
**  ENIGMA is an environment designed to create games and other programs with a
**
**  high-level, fully compilable language. Developers of ENIGMA or anything **
**  associated with ENIGMA are in no way responsible for its users or **
**  applications created by its users, or damages caused by the environment **
**  or programs made in the environment. **
** **
\********************************************************************************/

#include <gtest/gtest.h>

#include <iostream>

#include "ResourceTransformations/VisualShader/vs_noise_nodes.h"

TEST(VisualShaderNoiseNodesTest, Test_VisualShaderNodeValueNoise_generate_code) {
  VisualShaderNodeValueNoise node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{
      "\t{ // Value Noise\n"
      "\t\tfloat out_buffer = 0.0;\n"
      "\t\tgenerate_simple_noise_float(a, 100.000000, out_buffer);\n"
      "\t\tb = vec4(out_buffer, out_buffer, out_buffer, 1.0);\n"
      "\t}\n"};
  EXPECT_EQ(code, expected_code);
}
