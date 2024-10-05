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

#include "vs_noise_nodes.h"

#include <iomanip>
#include <sstream>

/*************************************/
/* Value (Simple) Noise              */
/*************************************/

VisualShaderNodeValueNoise::VisualShaderNodeValueNoise() : scale(100.0f) {
  set_input_port_default_value(0, TVector2());
}

std::string VisualShaderNodeValueNoise::get_caption() const { return "ValueNoise"; }

int VisualShaderNodeValueNoise::get_input_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeValueNoise::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_2D;
}

std::string VisualShaderNodeValueNoise::get_input_port_name([[maybe_unused]] const int& port) const { return "uv"; }

int VisualShaderNodeValueNoise::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeValueNoise::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_4D;
}

std::string VisualShaderNodeValueNoise::get_output_port_name([[maybe_unused]] const int& port) const { return "noise"; }

std::string VisualShaderNodeValueNoise::generate_global([[maybe_unused]] const int& id) const {
  std::string code;
  code += "float noise_random_value(vec2 uv) {" + std::string("\n");
  code += std::string("\t") + "return fract(sin(dot(uv, vec2(12.9898, 78.233)))*43758.5453);" + std::string("\n");
  code += std::string("}") + "\n\n";

  code += "float noise_interpolate(float a, float b, float t) {" + std::string("\n");
  code += std::string("\t") + "return (1.0-t)*a + (t*b);" + std::string("\n");
  code += std::string("}") + "\n\n";

  code += "float value_noise(vec2 uv) {" + std::string("\n");
  code += std::string("\t") + "vec2 i = floor(uv);" + std::string("\n");
  code += std::string("\t") + "vec2 f = fract(uv);" + std::string("\n");
  code += std::string("\t") + "f = f * f * (3.0 - 2.0 * f);" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "uv = abs(fract(uv) - 0.5);" + std::string("\n");
  code += std::string("\t") + "vec2 c0 = i + vec2(0.0, 0.0);" + std::string("\n");
  code += std::string("\t") + "vec2 c1 = i + vec2(1.0, 0.0);" + std::string("\n");
  code += std::string("\t") + "vec2 c2 = i + vec2(0.0, 1.0);" + std::string("\n");
  code += std::string("\t") + "vec2 c3 = i + vec2(1.0, 1.0);" + std::string("\n");
  code += std::string("\t") + "float r0 = noise_random_value(c0);" + std::string("\n");
  code += std::string("\t") + "float r1 = noise_random_value(c1);" + std::string("\n");
  code += std::string("\t") + "float r2 = noise_random_value(c2);" + std::string("\n");
  code += std::string("\t") + "float r3 = noise_random_value(c3);" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "float bottom_of_grid = noise_interpolate(r0, r1, f.x);" + std::string("\n");
  code += std::string("\t") + "float top_of_grid = noise_interpolate(r2, r3, f.x);" + std::string("\n");
  code += std::string("\t") + "float t = noise_interpolate(bottom_of_grid, top_of_grid, f.y);" + std::string("\n");
  code += std::string("\t") + "return t;" + std::string("\n");
  code += std::string("}") + "\n\n";

  code +=
      "void generate_value_noise_float(vec2 uv, float scale, out float "
      "out_buffer) {" +
      std::string("\n");
  code += std::string("\t") + "float t = 0.0;" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "float freq = pow(2.0, float(0));" + std::string("\n");
  code += std::string("\t") + "float amp = pow(0.5, float(3-0));" + std::string("\n");
  code += std::string("\t") + "t += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "freq = pow(2.0, float(1));" + std::string("\n");
  code += std::string("\t") + "amp = pow(0.5, float(3-1));" + std::string("\n");
  code += std::string("\t") + "t += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "freq = pow(2.0, float(2));" + std::string("\n");
  code += std::string("\t") + "amp = pow(0.5, float(3-2));" + std::string("\n");
  code += std::string("\t") + "t += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "out_buffer = t;" + std::string("\n");
  code += std::string("}") + "\n\n";

  return code;
}

std::string VisualShaderNodeValueNoise::generate_code([[maybe_unused]] const int& id,
                                                      const std::vector<std::string>& input_vars,
                                                      const std::vector<std::string>& output_vars) const {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(6) << scale;

  std::string code;

  code += std::string("\t") + "// Value Noise" + std::string("\n");
  code += std::string("\t") + "float out_buffer_n" + std::to_string(id) + " = 0.0;" + std::string("\n");
  code += std::string("\t") + "generate_value_noise_float(" + input_vars[0] + ", " + oss.str() + ", out_buffer_n" + std::to_string(id) + ");" +
          std::string("\n");
  code += std::string("\t") + output_vars[0] + " = vec4(out_buffer_n" + std::to_string(id) + ", out_buffer_n" + std::to_string(id) + ", out_buffer_n" + std::to_string(id) + ", 1.0);" + std::string("\n");
  code += std::string("\t") + std::string("\n");

  return code;
}

void VisualShaderNodeValueNoise::set_scale(const float& s) {
  if (this->scale == s) {
    return;
  }

  this->scale = s;
}

float VisualShaderNodeValueNoise::get_scale() const { return scale; }

std::vector<std::string> VisualShaderNodeValueNoise::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("scale");
  return props;
}

/*************************************/
/* Perlin (Gradient) Noise           */
/*************************************/

VisualShaderNodePerlinNoise::VisualShaderNodePerlinNoise() : scale(10.0f) {
  set_input_port_default_value(0, TVector2());
}

std::string VisualShaderNodePerlinNoise::get_caption() const { return "PerlinNoise"; }

int VisualShaderNodePerlinNoise::get_input_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodePerlinNoise::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_2D;
}

std::string VisualShaderNodePerlinNoise::get_input_port_name([[maybe_unused]] const int& port) const { return "uv"; }

int VisualShaderNodePerlinNoise::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodePerlinNoise::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_4D;
}

std::string VisualShaderNodePerlinNoise::get_output_port_name([[maybe_unused]] const int& port) const {
  return "noise";
}

std::string VisualShaderNodePerlinNoise::generate_global([[maybe_unused]] const int& id) const {
  std::string code;

  code += "vec2 perlin_noise_dir(vec2 p) {" + std::string("\n");
  code += std::string("\t") + "p = mod(p, 289.0);" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "float x = mod((34.0 * p.x + 1.0) * p.x, 289.0) + p.y;" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "x = mod((34.0 * x + 1.0) * x, 289.0);" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "x = fract(x / 41.0) * 2.0 - 1.0;" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "return normalize(vec2(x - floor(x + 0.5), abs(x) - 0.5));" + std::string("\n");
  code += std::string("}") + "\n\n";

  code += "float perlin_noise(vec2 p) {" + std::string("\n");
  code += std::string("\t") + "vec2 ip = floor(p);" + std::string("\n");
  code += std::string("\t") + "vec2 fp = fract(p);" + std::string("\n");
  code += std::string("\t") + "float d00 = dot(perlin_noise_dir(ip), fp);" + std::string("\n");
  code +=
      std::string("\t") + "float d01 = dot(perlin_noise_dir(ip + vec2(0, 1)), fp - vec2(0, 1));" + std::string("\n");
  code +=
      std::string("\t") + "float d10 = dot(perlin_noise_dir(ip + vec2(1, 0)), fp - vec2(1, 0));" + std::string("\n");
  code +=
      std::string("\t") + "float d11 = dot(perlin_noise_dir(ip + vec2(1, 1)), fp - vec2(1, 1));" + std::string("\n");
  code += std::string("\t") + "fp = fp * fp * fp * (fp * (fp * 6.0 - 15.0) + 10.0);" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "return mix(mix(d00, d01, fp.y), mix(d10, d11, fp.y), fp.x);" + std::string("\n");
  code += std::string("}") + "\n\n";

  code += "void generate_perlin_noise_float(vec2 uv, float scale, out float out_buffer) {" + std::string("\n");
  code += std::string("\t") + "out_buffer = perlin_noise(uv * scale) + 0.5;" + std::string("\n");
  code += std::string("}") + "\n\n";

  return code;
}

std::string VisualShaderNodePerlinNoise::generate_code([[maybe_unused]] const int& id,
                                                       const std::vector<std::string>& input_vars,
                                                       const std::vector<std::string>& output_vars) const {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(6) << scale;

  std::string code;

  code += std::string("\t") + "// Perlin Noise" + std::string("\n");
  code += std::string("\t") + "float out_buffer_n" + std::to_string(id) + " = 0.0;" + std::string("\n");
  code += std::string("\t") + "generate_perlin_noise_float(" + input_vars[0] + ", " + oss.str() + ", out_buffer_n" + std::to_string(id) + ");" +
          std::string("\n");
  code += std::string("\t") + output_vars[0] + " = vec4(out_buffer_n" + std::to_string(id) + ", out_buffer_n" + std::to_string(id) + ", out_buffer_n" + std::to_string(id) + ", 1.0);" + std::string("\n");
  code += std::string("\t") + std::string("\n");

  return code;
}

void VisualShaderNodePerlinNoise::set_scale(const float& s) {
  if (this->scale == s) {
    return;
  }

  this->scale = s;
}

float VisualShaderNodePerlinNoise::get_scale() const { return scale; }

std::vector<std::string> VisualShaderNodePerlinNoise::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("scale");
  return props;
}

/*************************************/
/* Voronoi (Worley) Noise            */
/*************************************/

VisualShaderNodeVoronoiNoise::VisualShaderNodeVoronoiNoise() : angle_offset(10.0f), cell_density(10.0f) {
  set_input_port_default_value(0, TVector2());
}

std::string VisualShaderNodeVoronoiNoise::get_caption() const { return "VoronoiNoise"; }

int VisualShaderNodeVoronoiNoise::get_input_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeVoronoiNoise::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_2D;
}

std::string VisualShaderNodeVoronoiNoise::get_input_port_name([[maybe_unused]] const int& port) const { return "uv"; }

int VisualShaderNodeVoronoiNoise::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeVoronoiNoise::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_4D;
}

std::string VisualShaderNodeVoronoiNoise::get_output_port_name([[maybe_unused]] const int& port) const {
  return "noise";
}

std::string VisualShaderNodeVoronoiNoise::generate_global([[maybe_unused]] const int& id) const {
  std::string code;

  code += "vec2 voronoi_noise_random_vector(vec2 uv, float offset) {" + std::string("\n");
  code += std::string("\t") + "mat2 m = mat2(15.27, 47.63, 99.41, 89.98);" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "uv = fract(sin(m * uv) * 46839.32);" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "return vec2(sin(uv.y*+offset)*0.5+0.5, cos(uv.x*offset)*0.5+0.5);" + std::string("\n");
  code += std::string("}") + "\n\n";

  code +=
      "void generate_voronoi_noise_float(vec2 uv, float angle_offset, float cell_density, out float out_buffer, out "
      "float cells) {" +
      std::string("\n");
  code += std::string("\t") + "vec2 g = floor(uv * cell_density);" + std::string("\n");
  code += std::string("\t") + "vec2 f = fract(uv * cell_density);" + std::string("\n");
  code += std::string("\t") + "float t = 8.0;" + std::string("\n");
  code += std::string("\t") + "vec3 res = vec3(8.0, 0.0, 0.0);" + std::string("\n");
  code += std::string("\t") + std::string("\n");
  code += std::string("\t") + "for(int y=-1; y<=1; y++) {" + std::string("\n");
  code += std::string("\t") + std::string("\t") + "for(int x=-1; x<=1; x++) {" + std::string("\n");
  code += std::string("\t") + std::string("\t") + std::string("\t") + "vec2 lattice = vec2(x,y);" + std::string("\n");
  code += std::string("\t") + std::string("\t") + std::string("\t") +
          "vec2 offset = voronoi_noise_random_vector(lattice + g, angle_offset);" + std::string("\n");
  code += std::string("\t") + std::string("\t") + std::string("\t") + "float d = distance(lattice + offset, f);" +
          std::string("\n");
  code += std::string("\t") + std::string("\t") + std::string("\t") + "if(d < res.x) {" + std::string("\n");
  code += std::string("\t") + std::string("\t") + std::string("\t") + std::string("\t") +
          "res = vec3(d, offset.x, offset.y);" + std::string("\n");
  code += std::string("\t") + std::string("\t") + std::string("\t") + std::string("\t") + "out_buffer = res.x;" +
          std::string("\n");
  code += std::string("\t") + std::string("\t") + std::string("\t") + std::string("\t") + "cells = res.y;" +
          std::string("\n");
  code += std::string("\t") + std::string("\t") + std::string("\t") + std::string("}") + std::string("\n");
  code += std::string("\t") + std::string("\t") + std::string("}") + std::string("\n");
  code += std::string("\t") + std::string("}") + std::string("\n");
  code += std::string("}") + "\n\n";

  return code;
}

std::string VisualShaderNodeVoronoiNoise::generate_code([[maybe_unused]] const int& id,
                                                        const std::vector<std::string>& input_vars,
                                                        const std::vector<std::string>& output_vars) const {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(6) << angle_offset;
  std::ostringstream oss2;
  oss2 << std::fixed << std::setprecision(6) << cell_density;

  std::string code;

  code += std::string("\t") + "// Voronoi Noise" + std::string("\n");
  code += std::string("\t") + "float out_buffer_n" + std::to_string(id) + " = 0.0;" + std::string("\n");
  code += std::string("\t") + "float cells_n" + std::to_string(id) + " = 0.0; // TODO: How we can use this?" + std::string("\n");
  code += std::string("\t") + "generate_voronoi_noise_float(" + input_vars[0] + ", " + oss.str() + ", " + oss2.str() +
          ", out_buffer_n" + std::to_string(id) + ", cells_n" + std::to_string(id) + ");" + std::string("\n");
  code += std::string("\t") + output_vars[0] + " = vec4(out_buffer_n" + std::to_string(id) + ", out_buffer_n" + std::to_string(id) + ", out_buffer_n" + std::to_string(id) + ", 1.0);" + std::string("\n");
  code += std::string("\t") + std::string("\n");

  return code;
}

void VisualShaderNodeVoronoiNoise::set_angle_offset(const float& a) {
  if (this->angle_offset == a) {
    return;
  }

  this->angle_offset = a;
}

float VisualShaderNodeVoronoiNoise::get_angle_offset() const { return angle_offset; }

void VisualShaderNodeVoronoiNoise::set_cell_density(const float& c) {
  if (this->cell_density == c) {
    return;
  }

  this->cell_density = c;
}

float VisualShaderNodeVoronoiNoise::get_cell_density() const { return cell_density; }

std::vector<std::string> VisualShaderNodeVoronoiNoise::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("angle_offset");
  props.emplace_back("cell_density");
  return props;
}
