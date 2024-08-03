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

#include "vs_noise_nodes.h"

#include <iomanip>
#include <sstream>

/*************************************/
/* Value Noise                       */
/*************************************/

VisualShaderNodeValueNoise::VisualShaderNodeValueNoise() : scale(100.0f) {
  NAME_ID = "node_value_noise";

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
  code += "float noise_random_value(vec2 uv) {\n";
  code += "\treturn fract(sin(dot(uv, vec2(12.9898, 78.233)))*43758.5453);\n";
  code += "}\n\n";

  code += "float noise_interpolate(float a, float b, float t) {\n";
  code += "\treturn (1.0-t)*a + (t*b);\n";
  code += "}\n\n";

  code += "float value_noise(vec2 uv) {\n";
  code += "\tvec2 i = floor(uv);\n";
  code += "\tvec2 f = fract(uv);\n";
  code += "\tf = f * f * (3.0 - 2.0 * f);\n";
  code += "\t\n";
  code += "\tuv = abs(fract(uv) - 0.5);\n";
  code += "\tvec2 c0 = i + vec2(0.0, 0.0);\n";
  code += "\tvec2 c1 = i + vec2(1.0, 0.0);\n";
  code += "\tvec2 c2 = i + vec2(0.0, 1.0);\n";
  code += "\tvec2 c3 = i + vec2(1.0, 1.0);\n";
  code += "\tfloat r0 = noise_random_value(c0);\n";
  code += "\tfloat r1 = noise_random_value(c1);\n";
  code += "\tfloat r2 = noise_random_value(c2);\n";
  code += "\tfloat r3 = noise_random_value(c3);\n";
  code += "\t\n";
  code += "\tfloat bottomOfGrid = noise_interpolate(r0, r1, f.x);\n";
  code += "\tfloat topOfGrid = noise_interpolate(r2, r3, f.x);\n";
  code += "\tfloat t = noise_interpolate(bottomOfGrid, topOfGrid, f.y);\n";
  code += "\treturn t;\n";
  code += "}\n\n";

  code +=
      "void generate_value_noise_float(vec2 uv, float scale, out float "
      "out_buffer) {\n";
  code += "\tfloat t = 0.0;\n";
  code += "\t\n";
  code += "\tfloat freq = pow(2.0, float(0));\n";
  code += "\tfloat amp = pow(0.5, float(3-0));\n";
  code += "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n";
  code += "\t\n";
  code += "\tfreq = pow(2.0, float(1));\n";
  code += "\tamp = pow(0.5, float(3-1));\n";
  code += "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n";
  code += "\t\n";
  code += "\tfreq = pow(2.0, float(2));\n";
  code += "\tamp = pow(0.5, float(3-2));\n";
  code += "\tt += value_noise(vec2(uv.x*scale/freq, uv.y*scale/freq))*amp;\n";
  code += "\t\n";
  code += "\tout_buffer = t;\n";
  code += "}\n\n";

  return code;
}

std::string VisualShaderNodeValueNoise::generate_code([[maybe_unused]] const int& id,
                                                      const std::vector<std::string>& input_vars,
                                                      const std::vector<std::string>& output_vars) const {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(6) << scale;

  std::string code;

  code += "\t{ // Value Noise\n";
  code += "\t\tfloat out_buffer = 0.0;\n";
  code += "\t\tgenerate_value_noise_float(" + input_vars[0] + ", " + oss.str() + ", out_buffer);\n";
  code += "\t\t" + output_vars[0] + " = vec4(out_buffer, out_buffer, out_buffer, 1.0);\n";
  code += "\t}\n";

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
