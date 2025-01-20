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

#include "ResourceTransformations/VisualShader/vs_noise_nodes.h"

TEST(VisualShaderNoiseNodesTest, Test_VisualShaderNodeValueNoise_generate_code) {
  VisualShaderNodeValueNoise node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{
      "\t// Value Noise\n"
      "\tfloat out_buffer_n0 = 0.0;\n"
      "\tgenerate_value_noise_float(a, 100.000000, out_buffer_n0);\n"
      "\tb = vec4(out_buffer_n0, out_buffer_n0, out_buffer_n0, 1.0);\n"
      "\t\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNoiseNodesTest, Test_VisualShaderNodeValueNoise_generate_global) {
  VisualShaderNodeValueNoise node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code{node.generate_global(0)};
  std::string expected_code{
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
    "void generate_value_noise_float(vec2 uv, float scale, out float out_buffer) {\n"
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
    "}\n\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNoiseNodesTest, Test_VisualShaderNodePerlinNoise_generate_code) {
  VisualShaderNodePerlinNoise node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{
      "\t// Perlin Noise\n"
      "\tfloat out_buffer_n0 = 0.0;\n"
      "\tgenerate_perlin_noise_float(a, 10.000000, out_buffer_n0);\n"
      "\tb = vec4(out_buffer_n0, out_buffer_n0, out_buffer_n0, 1.0);\n"
      "\t\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNoiseNodesTest, Test_VisualShaderNodePerlinNoise_generate_global) {
  VisualShaderNodePerlinNoise node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code{node.generate_global(0)};
  std::string expected_code{
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
    "}\n\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNoiseNodesTest, Test_VisualShaderNodeVoronoiNoise_generate_code) {
  VisualShaderNodeVoronoiNoise node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code{node.generate_code(0, input_vars, output_vars)};
  std::string expected_code{
      "\t// Voronoi Noise\n"
      "\tfloat out_buffer_n0 = 0.0;\n"
      "\tfloat cells_n0 = 0.0; // TODO: How we can use this?\n"
      "\tgenerate_voronoi_noise_float(a, 10.000000, 10.000000, out_buffer_n0, cells_n0);\n"
      "\tb = vec4(out_buffer_n0, out_buffer_n0, out_buffer_n0, 1.0);\n"
      "\t\n"};
  EXPECT_EQ(code, expected_code);
}

TEST(VisualShaderNoiseNodesTest, Test_VisualShaderNodeVoronoiNoise_generate_global) {
  VisualShaderNodeVoronoiNoise node;
  std::vector<std::string> input_vars = {"a"};
  std::vector<std::string> output_vars = {"b"};
  std::string code{node.generate_global(0)};
  std::string expected_code{
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
    "}\n\n"};
  EXPECT_EQ(code, expected_code);
}
