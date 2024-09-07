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

#include "visual_shader_nodes.h"

#include <iomanip>
#include <iostream>
#include <sstream>

/*************************************/
/* Vector Base Node                  */
/*************************************/

VisualShaderNodeVectorBase::VisualShaderNodeVectorBase()
    : op_type(VisualShaderNodeVectorBase::OpType::OP_TYPE_VECTOR_3D) {}

VisualShaderNode::PortType VisualShaderNodeVectorBase::get_input_port_type([[maybe_unused]] const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
      return PORT_TYPE_VECTOR_2D;
    case OP_TYPE_VECTOR_3D:
      return PORT_TYPE_VECTOR_3D;
    case OP_TYPE_VECTOR_4D:
      return PORT_TYPE_VECTOR_4D;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

VisualShaderNode::PortType VisualShaderNodeVectorBase::get_output_port_type(const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
      if (get_output_port_count() > 1) {
        return PORT_TYPE_VECTOR_2D;
      }

      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_2D;
        default:
          break;
      }

      break;
    case OP_TYPE_VECTOR_3D:
      if (get_output_port_count() > 1) {
        return PORT_TYPE_VECTOR_3D;
      }

      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_3D;
        default:
          break;
      }

      break;
    case OP_TYPE_VECTOR_4D:
      if (get_output_port_count() > 1) {
        return PORT_TYPE_VECTOR_4D;
      }

      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_4D;
        default:
          break;
      }

      break;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

void VisualShaderNodeVectorBase::set_op_type(const VisualShaderNodeVectorBase::OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeVectorBase::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  this->op_type = op_type;
}

VisualShaderNodeVectorBase::OpType VisualShaderNodeVectorBase::get_op_type() const { return op_type; }

std::vector<std::string> VisualShaderNodeVectorBase::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("op_type");
  return props;
}

/*************************************/
/* CONSTANTS                         */
/*************************************/

VisualShaderNodeConstant::VisualShaderNodeConstant() {}

/*************************************/
/* Float Constant                    */
/*************************************/

VisualShaderNodeFloatConstant::VisualShaderNodeFloatConstant() : constant(0.0f) {  }

std::string VisualShaderNodeFloatConstant::get_caption() const { return "FloatConstant"; }

int VisualShaderNodeFloatConstant::get_input_port_count() const { return 0; }

VisualShaderNode::PortType VisualShaderNodeFloatConstant::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeFloatConstant::get_input_port_name([[maybe_unused]] const int& port) const {
  return std::string();
}

int VisualShaderNodeFloatConstant::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeFloatConstant::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeFloatConstant::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeFloatConstant::generate_code([[maybe_unused]] const int& id,
                                                         [[maybe_unused]] const std::vector<std::string>& input_vars,
                                                         const std::vector<std::string>& output_vars) const {
  std::ostringstream oss;
  oss << "\t" << output_vars.at(0) << " = " << std::fixed << std::setprecision(6) << constant << ";" << std::string("\n");
  return oss.str();
}

void VisualShaderNodeFloatConstant::set_constant(const float& c) {
  if (t_are_almost_equal(constant, c)) {
		return;
	}

  constant = c;
}

float VisualShaderNodeFloatConstant::get_constant() const { return constant; }

std::vector<std::string> VisualShaderNodeFloatConstant::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("constant");
  return props;
}

/*************************************/
/* Int Constant                      */
/*************************************/

VisualShaderNodeIntConstant::VisualShaderNodeIntConstant() : constant(0) {  }

std::string VisualShaderNodeIntConstant::get_caption() const { return "IntConstant"; }

int VisualShaderNodeIntConstant::get_input_port_count() const { return 0; }

VisualShaderNode::PortType VisualShaderNodeIntConstant::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_INT;
}

std::string VisualShaderNodeIntConstant::get_input_port_name([[maybe_unused]] const int& port) const {
  return std::string();
}

int VisualShaderNodeIntConstant::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeIntConstant::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_INT;
}

std::string VisualShaderNodeIntConstant::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeIntConstant::generate_code([[maybe_unused]] const int& id,
                                                       [[maybe_unused]] const std::vector<std::string>& input_vars,
                                                       const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = " + std::to_string(constant) + ";" + std::string("\n");
}

void VisualShaderNodeIntConstant::set_constant(const int& c) {
  if (constant == c) {
    return;
  }

  constant = c;
}

int VisualShaderNodeIntConstant::get_constant() const { return constant; }

std::vector<std::string> VisualShaderNodeIntConstant::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("constant");
  return props;
}

/*************************************/
/* UInt Constant                     */
/*************************************/

VisualShaderNodeUIntConstant::VisualShaderNodeUIntConstant() : constant(0) {  }

std::string VisualShaderNodeUIntConstant::get_caption() const { return "UIntConstant"; }

int VisualShaderNodeUIntConstant::get_input_port_count() const { return 0; }

VisualShaderNode::PortType VisualShaderNodeUIntConstant::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_UINT;
}

std::string VisualShaderNodeUIntConstant::get_input_port_name([[maybe_unused]] const int& port) const {
  return std::string();
}

int VisualShaderNodeUIntConstant::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeUIntConstant::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_UINT;
}

std::string VisualShaderNodeUIntConstant::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeUIntConstant::generate_code([[maybe_unused]] const int& id,
                                                        [[maybe_unused]] const std::vector<std::string>& input_vars,
                                                        const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = " + std::to_string(constant) + "u;" + std::string("\n");
}

void VisualShaderNodeUIntConstant::set_constant(const int& c) {
  if (constant == c) {
    return;
  }

  constant = c;
}

int VisualShaderNodeUIntConstant::get_constant() const { return constant; }

std::vector<std::string> VisualShaderNodeUIntConstant::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("constant");
  return props;
}

/*************************************/
/* Boolean Constant                  */
/*************************************/

VisualShaderNodeBooleanConstant::VisualShaderNodeBooleanConstant() : constant(false) {  }

std::string VisualShaderNodeBooleanConstant::get_caption() const { return "BooleanConstant"; }

int VisualShaderNodeBooleanConstant::get_input_port_count() const { return 0; }

VisualShaderNode::PortType VisualShaderNodeBooleanConstant::get_input_port_type(
    [[maybe_unused]] const int& port) const {
  return PORT_TYPE_BOOLEAN;
}

std::string VisualShaderNodeBooleanConstant::get_input_port_name([[maybe_unused]] const int& port) const {
  return std::string();
}

int VisualShaderNodeBooleanConstant::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeBooleanConstant::get_output_port_type(
    [[maybe_unused]] const int& port) const {
  return PORT_TYPE_BOOLEAN;
}

std::string VisualShaderNodeBooleanConstant::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeBooleanConstant::generate_code([[maybe_unused]] const int& id,
                                                           [[maybe_unused]] const std::vector<std::string>& input_vars,
                                                           const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = " + (constant ? "true" : "false") + ";" + std::string("\n");
}

void VisualShaderNodeBooleanConstant::set_constant(const bool& c) {
  if (constant == c) {
    return;
  }

  constant = c;
}

bool VisualShaderNodeBooleanConstant::get_constant() const { return constant; }

std::vector<std::string> VisualShaderNodeBooleanConstant::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("constant");
  return props;
}

/*************************************/
/* Color Constant                    */
/*************************************/

VisualShaderNodeColorConstant::VisualShaderNodeColorConstant() : constant({1.0f, 1.0f, 1.0f, 1.0f}) {}

std::string VisualShaderNodeColorConstant::get_caption() const { return "ColorConstant"; }

int VisualShaderNodeColorConstant::get_input_port_count() const { return 0; }

VisualShaderNode::PortType VisualShaderNodeColorConstant::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_4D;
}

std::string VisualShaderNodeColorConstant::get_input_port_name([[maybe_unused]] const int& port) const {
  return std::string();
}

int VisualShaderNodeColorConstant::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeColorConstant::get_output_port_type(const int& port) const {
  switch (port) {
    case 0:
      return PORT_TYPE_VECTOR_4D;
    default:
      std::cerr << "VisualShaderNodeColorConstant::get_output_port_type: "
                   "Invalid port: "
                << port << std::endl;
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeColorConstant::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeColorConstant::generate_code([[maybe_unused]] const int& id,
                                                         [[maybe_unused]] const std::vector<std::string>& input_vars,
                                                         const std::vector<std::string>& output_vars) const {
  std::ostringstream oss;
  oss << "\t" << output_vars.at(0) << " = vec4(" << std::fixed << std::setprecision(6) << constant.r << ", "
      << constant.g << ", " << constant.b << ", " << constant.a << ");" << std::string("\n");
  return oss.str();
}

void VisualShaderNodeColorConstant::set_constant(const TColor& c) {
  if (constant.are_almost_equal(c)) {
		return;
	}

  constant = c;
}

TColor VisualShaderNodeColorConstant::get_constant() const { return constant; }

std::vector<std::string> VisualShaderNodeColorConstant::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("constant");
  return props;
}

/*************************************/
/* Vector2 Constant                  */
/*************************************/

VisualShaderNodeVec2Constant::VisualShaderNodeVec2Constant() {  }

std::string VisualShaderNodeVec2Constant::get_caption() const { return "Vector2Constant"; }

int VisualShaderNodeVec2Constant::get_input_port_count() const { return 0; }

VisualShaderNode::PortType VisualShaderNodeVec2Constant::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_2D;
}

std::string VisualShaderNodeVec2Constant::get_input_port_name([[maybe_unused]] const int& port) const {
  return std::string();
}

int VisualShaderNodeVec2Constant::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeVec2Constant::get_output_port_type(const int& port) const {
  switch (port) {
    case 0:
      return PORT_TYPE_VECTOR_2D;
    default:
      std::cerr << "VisualShaderNodeVec2Constant::get_output_port_type: "
                   "Invalid port: "
                << port << std::endl;
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeVec2Constant::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeVec2Constant::generate_code([[maybe_unused]] const int& id,
                                                        [[maybe_unused]] const std::vector<std::string>& input_vars,
                                                        const std::vector<std::string>& output_vars) const {
  std::ostringstream oss;
  oss << "\t" << output_vars.at(0) << " = vec2(" << std::fixed << std::setprecision(6) << constant.x << ", "
      << constant.y << ");" << std::string("\n");
  return oss.str();
}

void VisualShaderNodeVec2Constant::set_constant(const TVector2& c) {
  if (constant.are_almost_equal(c)) {
		return;
	}

  constant = c;
}

TVector2 VisualShaderNodeVec2Constant::get_constant() const { return constant; }

std::vector<std::string> VisualShaderNodeVec2Constant::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("constant");
  return props;
}

/*************************************/
/* Vector3 Constant                  */
/*************************************/

VisualShaderNodeVec3Constant::VisualShaderNodeVec3Constant() {  }

std::string VisualShaderNodeVec3Constant::get_caption() const { return "Vector3Constant"; }

int VisualShaderNodeVec3Constant::get_input_port_count() const { return 0; }

VisualShaderNode::PortType VisualShaderNodeVec3Constant::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_3D;
}

std::string VisualShaderNodeVec3Constant::get_input_port_name([[maybe_unused]] const int& port) const {
  return std::string();
}

int VisualShaderNodeVec3Constant::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeVec3Constant::get_output_port_type(const int& port) const {
  switch (port) {
    case 0:
      return PORT_TYPE_VECTOR_3D;
    default:
      std::cerr << "VisualShaderNodeVec3Constant::get_output_port_type: "
                   "Invalid port: "
                << port << std::endl;
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeVec3Constant::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeVec3Constant::generate_code([[maybe_unused]] const int& id,
                                                        [[maybe_unused]] const std::vector<std::string>& input_vars,
                                                        const std::vector<std::string>& output_vars) const {
  std::ostringstream oss;
  oss << "\t" << output_vars.at(0) << " = vec3(" << std::fixed << std::setprecision(6) << constant.x << ", "
      << constant.y << ", " << constant.z << ");" << std::string("\n");
  return oss.str();
}

void VisualShaderNodeVec3Constant::set_constant(const TVector3& c) {
  if (constant.are_almost_equal(c)) {
		return;
	}

  constant = c;
}

TVector3 VisualShaderNodeVec3Constant::get_constant() const { return constant; }

std::vector<std::string> VisualShaderNodeVec3Constant::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("constant");
  return props;
}

/*************************************/
/* Vector4 Constant                  */
/*************************************/

VisualShaderNodeVec4Constant::VisualShaderNodeVec4Constant() {  }

std::string VisualShaderNodeVec4Constant::get_caption() const { return "Vector4Constant"; }

int VisualShaderNodeVec4Constant::get_input_port_count() const { return 0; }

VisualShaderNode::PortType VisualShaderNodeVec4Constant::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_4D;
}

std::string VisualShaderNodeVec4Constant::get_input_port_name([[maybe_unused]] const int& port) const {
  return std::string();
}

int VisualShaderNodeVec4Constant::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeVec4Constant::get_output_port_type(const int& port) const {
  switch (port) {
    case 0:
      return PORT_TYPE_VECTOR_4D;
    default:
      std::cerr << "VisualShaderNodeVec4Constant::get_output_port_type: "
                   "Invalid port: "
                << port << std::endl;
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeVec4Constant::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeVec4Constant::generate_code([[maybe_unused]] const int& id,
                                                        [[maybe_unused]] const std::vector<std::string>& input_vars,
                                                        const std::vector<std::string>& output_vars) const {
  std::ostringstream oss;
  oss << "\t" << output_vars.at(0) << " = vec4(" << std::fixed << std::setprecision(6) << constant.x << ", "
      << constant.y << ", " << constant.z << ", " << constant.w << ");" << std::string("\n");
  return oss.str();
}

void VisualShaderNodeVec4Constant::set_constant(const TVector4& c) {
  if (constant.are_almost_equal(c)) {
		return;
	}

  constant = c;
}

TVector4 VisualShaderNodeVec4Constant::get_constant() const { return constant; }

std::vector<std::string> VisualShaderNodeVec4Constant::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("constant");
  return props;
}

/*************************************/
/* OPERATORS                         */
/*************************************/

/*************************************/
/* Float Op Node                     */
/*************************************/

VisualShaderNodeFloatOp::VisualShaderNodeFloatOp() : op(VisualShaderNodeFloatOp::Operator::OP_ADD) {
  set_input_port_default_value(0, 0.0f);
  set_input_port_default_value(1, 0.0f);
}

std::string VisualShaderNodeFloatOp::get_caption() const { return "FloatOp"; }

int VisualShaderNodeFloatOp::get_input_port_count() const { return 2; }

VisualShaderNode::PortType VisualShaderNodeFloatOp::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeFloatOp::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "p1";
    case 1:
      return "p2";
    default:
      std::cerr << "VisualShaderNodeFloatOp::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return "";
}

int VisualShaderNodeFloatOp::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeFloatOp::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeFloatOp::get_output_port_name([[maybe_unused]] const int& port) const { return "op"; }

std::string VisualShaderNodeFloatOp::generate_code([[maybe_unused]] const int& id,
                                                   const std::vector<std::string>& input_vars,
                                                   const std::vector<std::string>& output_vars) const {
  std::string code{std::string("\t") + output_vars.at(0) + " = "};
  switch (op) {
    case VisualShaderNodeFloatOp::Operator::OP_ADD:
      code += input_vars.at(0) + " + " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeFloatOp::Operator::OP_SUB:
      code += input_vars.at(0) + " - " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeFloatOp::Operator::OP_MUL:
      code += input_vars.at(0) + " * " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeFloatOp::Operator::OP_DIV:
      code += input_vars.at(0) + " / " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeFloatOp::Operator::OP_MOD:
      code += "mod(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatOp::Operator::OP_POW:
      code += "pow(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatOp::Operator::OP_MAX:
      code += "max(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatOp::Operator::OP_MIN:
      code += "min(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatOp::Operator::OP_ATAN2:
      code += "atan(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatOp::Operator::OP_STEP:
      code += "step(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    default:
      break;
  }
  return code;
}

void VisualShaderNodeFloatOp::set_operator(const VisualShaderNodeFloatOp::Operator& op) {
  if (op < 0 || op >= OP_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeFloatOp::set_operator: Invalid op: " << op << std::endl;
    return;
  }

  if (this->op == op) {
    return;
  }

  this->op = op;
}

VisualShaderNodeFloatOp::Operator VisualShaderNodeFloatOp::get_operator() const { return op; }

std::vector<std::string> VisualShaderNodeFloatOp::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("operator");
  return props;
}

/*************************************/
/* Int Op Node                       */
/*************************************/

VisualShaderNodeIntOp::VisualShaderNodeIntOp() : op(VisualShaderNodeIntOp::Operator::OP_ADD) {
  set_input_port_default_value(0, 0);
  set_input_port_default_value(1, 0);
}

std::string VisualShaderNodeIntOp::get_caption() const { return "IntOp"; }

int VisualShaderNodeIntOp::get_input_port_count() const { return 2; }

VisualShaderNode::PortType VisualShaderNodeIntOp::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_INT;
}

std::string VisualShaderNodeIntOp::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "p1";
    case 1:
      return "p2";
    default:
      std::cerr << "VisualShaderNodeIntOp::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return "";
}

int VisualShaderNodeIntOp::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeIntOp::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_INT;
}

std::string VisualShaderNodeIntOp::get_output_port_name([[maybe_unused]] const int& port) const { return "op"; }

std::string VisualShaderNodeIntOp::generate_code([[maybe_unused]] const int& id,
                                                 const std::vector<std::string>& input_vars,
                                                 const std::vector<std::string>& output_vars) const {
  std::string code{std::string("\t") + output_vars.at(0) + " = "};
  switch (op) {
    case VisualShaderNodeIntOp::Operator::OP_ADD:
      code += input_vars.at(0) + " + " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeIntOp::Operator::OP_SUB:
      code += input_vars.at(0) + " - " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeIntOp::Operator::OP_MUL:
      code += input_vars.at(0) + " * " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeIntOp::Operator::OP_DIV:
      code += input_vars.at(0) + " / " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeIntOp::Operator::OP_MOD:
      code += input_vars.at(0) + " % " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeIntOp::Operator::OP_MAX:
      code += "max(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeIntOp::Operator::OP_MIN:
      code += "min(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeIntOp::Operator::OP_BITWISE_AND:
      code += input_vars.at(0) + " & " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeIntOp::Operator::OP_BITWISE_OR:
      code += input_vars.at(0) + " | " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeIntOp::Operator::OP_BITWISE_XOR:
      code += input_vars.at(0) + " ^ " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeIntOp::Operator::OP_BITWISE_LEFT_SHIFT:
      code += input_vars.at(0) + " << " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeIntOp::Operator::OP_BITWISE_RIGHT_SHIFT:
      code += input_vars.at(0) + " >> " + input_vars.at(1) + ";" + std::string("\n");
      break;
    default:
      break;
  }

  return code;
}

void VisualShaderNodeIntOp::set_operator(const VisualShaderNodeIntOp::Operator& op) {
  if (op < 0 || op >= OP_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeIntOp::set_operator: Invalid op: " << op << std::endl;
    return;
  }

  if (this->op == op) {
    return;
  }

  this->op = op;
}

VisualShaderNodeIntOp::Operator VisualShaderNodeIntOp::get_operator() const { return op; }

std::vector<std::string> VisualShaderNodeIntOp::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("operator");
  return props;
}

/*************************************/
/* UInt Op Node                      */
/*************************************/

VisualShaderNodeUIntOp::VisualShaderNodeUIntOp() : op(VisualShaderNodeUIntOp::Operator::OP_ADD) {
  set_input_port_default_value(0, 0);
  set_input_port_default_value(1, 0);
}

std::string VisualShaderNodeUIntOp::get_caption() const { return "UIntOp"; }

int VisualShaderNodeUIntOp::get_input_port_count() const { return 2; }

VisualShaderNode::PortType VisualShaderNodeUIntOp::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_UINT;
}

std::string VisualShaderNodeUIntOp::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "p1";
    case 1:
      return "p2";
    default:
      std::cerr << "VisualShaderNodeUIntOp::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return "";
}

int VisualShaderNodeUIntOp::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeUIntOp::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_UINT;
}

std::string VisualShaderNodeUIntOp::get_output_port_name([[maybe_unused]] const int& port) const { return "op"; }

std::string VisualShaderNodeUIntOp::generate_code([[maybe_unused]] const int& id,
                                                  const std::vector<std::string>& input_vars,
                                                  const std::vector<std::string>& output_vars) const {
  std::string code{std::string("\t") + output_vars.at(0) + " = "};
  switch (op) {
    case VisualShaderNodeUIntOp::Operator::OP_ADD:
      code += input_vars.at(0) + " + " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeUIntOp::Operator::OP_SUB:
      code += input_vars.at(0) + " - " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeUIntOp::Operator::OP_MUL:
      code += input_vars.at(0) + " * " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeUIntOp::Operator::OP_DIV:
      code += input_vars.at(0) + " / " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeUIntOp::Operator::OP_MOD:
      code += input_vars.at(0) + " % " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeUIntOp::Operator::OP_MAX:
      code += "max(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeUIntOp::Operator::OP_MIN:
      code += "min(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeUIntOp::Operator::OP_BITWISE_AND:
      code += input_vars.at(0) + " & " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeUIntOp::Operator::OP_BITWISE_OR:
      code += input_vars.at(0) + " | " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeUIntOp::Operator::OP_BITWISE_XOR:
      code += input_vars.at(0) + " ^ " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeUIntOp::Operator::OP_BITWISE_LEFT_SHIFT:
      code += input_vars.at(0) + " << " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeUIntOp::Operator::OP_BITWISE_RIGHT_SHIFT:
      code += input_vars.at(0) + " >> " + input_vars.at(1) + ";" + std::string("\n");
      break;
    default:
      break;
  }

  return code;
}

void VisualShaderNodeUIntOp::set_operator(const VisualShaderNodeUIntOp::Operator& op) {
  if (op < 0 || op >= OP_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeUIntOp::set_operator: Invalid op: " << op << std::endl;
    return;
  }

  if (this->op == op) {
    return;
  }

  this->op = op;
}

VisualShaderNodeUIntOp::Operator VisualShaderNodeUIntOp::get_operator() const { return op; }

std::vector<std::string> VisualShaderNodeUIntOp::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("operator");
  return props;
}

/*************************************/
/* Vector Op Node                    */
/*************************************/

VisualShaderNodeVectorOp::VisualShaderNodeVectorOp() : op(VisualShaderNodeVectorOp::Operator::OP_ADD) {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2());
      set_input_port_default_value(1, TVector2());
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3());
      set_input_port_default_value(1, TVector3());
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4());
      set_input_port_default_value(1, TVector4());
    } break;
    default:
      break;
  }
}

std::string VisualShaderNodeVectorOp::get_caption() const { return "VectorOp"; }

int VisualShaderNodeVectorOp::get_input_port_count() const { return 2; }

std::string VisualShaderNodeVectorOp::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "p1";
    case 1:
      return "p2";
    default:
      std::cerr << "VisualShaderNodeVectorOp::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return "";
}

int VisualShaderNodeVectorOp::get_output_port_count() const { return 1; }

std::string VisualShaderNodeVectorOp::get_output_port_name([[maybe_unused]] const int& port) const { return "op"; }

std::string VisualShaderNodeVectorOp::generate_code([[maybe_unused]] const int& id,
                                                    const std::vector<std::string>& input_vars,
                                                    const std::vector<std::string>& output_vars) const {
  std::string code{std::string("\t") + output_vars.at(0) + " = "};
  switch (op) {
    case VisualShaderNodeVectorOp::Operator::OP_ADD:
      code += input_vars.at(0) + " + " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeVectorOp::Operator::OP_SUB:
      code += input_vars.at(0) + " - " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeVectorOp::Operator::OP_MUL:
      code += input_vars.at(0) + " * " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeVectorOp::Operator::OP_DIV:
      code += input_vars.at(0) + " / " + input_vars.at(1) + ";" + std::string("\n");
      break;
    case VisualShaderNodeVectorOp::Operator::OP_MOD:
      code += "mod(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeVectorOp::Operator::OP_POW:
      code += "pow(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeVectorOp::Operator::OP_MAX:
      code += "max(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeVectorOp::Operator::OP_MIN:
      code += "min(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeVectorOp::Operator::OP_CROSS:
      switch (op_type) {
        case OP_TYPE_VECTOR_2D:  // Not supported.
          code += "vec2(0.0);" + std::string("\n");
          break;
        case OP_TYPE_VECTOR_3D:
          code += "cross(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case OP_TYPE_VECTOR_4D:  // Not supported.
          code += "vec4(0.0);" + std::string("\n");
          break;
        default:
          break;
      }
      break;
    case VisualShaderNodeVectorOp::Operator::OP_ATAN2:
      code += "atan(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeVectorOp::Operator::OP_REFLECT:
      code += "reflect(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    case VisualShaderNodeVectorOp::Operator::OP_STEP:
      code += "step(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
      break;
    default:
      break;
  }

  return code;
}

void VisualShaderNodeVectorOp::set_op_type(const VisualShaderNodeVectorBase::OpType& op_type) {
  if (op_type < 0 || op_type >= VisualShaderNodeVectorBase::OpType::OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeVectorOp::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector2(), get_input_port_default_value(1));
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector3(), get_input_port_default_value(1));
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector4(), get_input_port_default_value(1));
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

void VisualShaderNodeVectorOp::set_operator(const VisualShaderNodeVectorOp::Operator& op) {
  if (op < 0 || op >= OP_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeVectorOp::set_operator: Invalid op: " << op << std::endl;
    return;
  }

  if (this->op == op) {
    return;
  }

  this->op = op;
}

VisualShaderNodeVectorOp::Operator VisualShaderNodeVectorOp::get_operator() const { return op; }

std::vector<std::string> VisualShaderNodeVectorOp::get_editable_properties() const {
  std::vector<std::string> props{VisualShaderNodeVectorBase::get_editable_properties()};
  props.emplace_back("operator");
  return props;
}

std::string VisualShaderNodeVectorOp::get_warning() const {
  bool invalid_type{false};

  if (op_type == OP_TYPE_VECTOR_2D || op_type == OP_TYPE_VECTOR_4D) {
    if (op == OP_CROSS) {
      invalid_type = true;
    }
  }

  if (invalid_type) {
    return "VisualShaderNodeVectorOp::get_warning: Invalid type for "
           "operator: " +
           std::to_string(op);
  }

  return std::string();
}

/*************************************/
/* Color Op Node                     */
/*************************************/

VisualShaderNodeColorOp::VisualShaderNodeColorOp() : op(VisualShaderNodeColorOp::Operator::OP_SCREEN) {
  set_input_port_default_value(0, TVector3());
  set_input_port_default_value(1, TVector3());
}

std::string VisualShaderNodeColorOp::get_caption() const { return "ColorOp"; }

int VisualShaderNodeColorOp::get_input_port_count() const { return 2; }

VisualShaderNode::PortType VisualShaderNodeColorOp::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_3D;
}

std::string VisualShaderNodeColorOp::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "p1";
    case 1:
      return "p2";
    default:
      std::cerr << "VisualShaderNodeColorOp::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return "";
}

int VisualShaderNodeColorOp::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeColorOp::get_output_port_type(const int& port) const {
  switch (port) {
    case 0:
      return PORT_TYPE_VECTOR_3D;
    default:
      std::cerr << "VisualShaderNodeColorOp::get_output_port_type: Invalid port: " << port << std::endl;
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeColorOp::get_output_port_name([[maybe_unused]] const int& port) const { return "op"; }

std::string VisualShaderNodeColorOp::generate_code([[maybe_unused]] const int& id,
                                                   const std::vector<std::string>& input_vars,
                                                   const std::vector<std::string>& output_vars) const {
  std::string code;
  static const char* axisn[3] = {"x", "y", "z"};
  switch (op) {
    case VisualShaderNodeColorOp::Operator::OP_SCREEN: {
      code += std::string("\t") + output_vars.at(0) + " = vec3(1.0) - (vec3(1.0) - " + input_vars.at(0) + ") * (vec3(1.0) - " +
              input_vars.at(1) + ");" + std::string("\n");
    } break;
    case VisualShaderNodeColorOp::Operator::OP_DIFFERENCE: {
      code += std::string("\t") + output_vars.at(0) + " = abs(" + input_vars.at(0) + " - " + input_vars.at(1) + ");" + std::string("\n");
    } break;
    case VisualShaderNodeColorOp::Operator::OP_DARKEN: {
      code += std::string("\t") + output_vars.at(0) + " = min(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
    } break;
    case VisualShaderNodeColorOp::Operator::OP_LIGHTEN: {
      code += std::string("\t") + output_vars.at(0) + " = max(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
    } break;
    case VisualShaderNodeColorOp::Operator::OP_OVERLAY: {
      for (int i{0}; i < 3; i++) {
        code += std::string("\t") + std::string("{") + std::string("\n");
        code += std::string("\t") + std::string("\t") + "float base = " + input_vars.at(0) + "." + axisn[i] + ";" + std::string("\n");
        code += std::string("\t") + std::string("\t") + "float blend = " + input_vars.at(1) + "." + axisn[i] + ";" + std::string("\n");
        code += std::string("\t") + std::string("\t") + "if (base < 0.5) {" + std::string("\n");
        code += std::string("\t") + std::string("\t") + std::string("\t") + output_vars.at(0) + "." + axisn[i] + " = 2.0 * base * blend;" + std::string("\n");
        code += std::string("\t") + std::string("\t") + "} else {" + std::string("\n");
        code += std::string("\t") + std::string("\t") + std::string("\t") + output_vars.at(0) + "." + axisn[i] + " = 1.0 - 2.0 * (1.0 - blend) * (1.0 - base);" + std::string("\n");
        code += std::string("\t") + std::string("\t") + std::string("}") + std::string("\n");
        code += std::string("\t") + std::string("}") + "\n\n";
      }
    } break;
    case VisualShaderNodeColorOp::Operator::OP_DODGE: {
      code += std::string("\t") + output_vars.at(0) + " = (" + input_vars.at(0) + ") / (vec3(1.0) - " + input_vars.at(1) + ");" + std::string("\n");
    } break;
    case VisualShaderNodeColorOp::Operator::OP_BURN: {
      code += std::string("\t") + output_vars.at(0) + " = vec3(1.0) - (vec3(1.0) - " + input_vars.at(0) + ") / (" +
              input_vars.at(1) + ");" + std::string("\n");
    } break;
    case VisualShaderNodeColorOp::Operator::OP_SOFT_LIGHT: {
      for (int i{0}; i < 3; i++) {
        code += std::string("\t") + std::string("{") + std::string("\n");
        code += std::string("\t") + std::string("\t") + "float base = " + input_vars.at(0) + "." + axisn[i] + ";" + std::string("\n");
        code += std::string("\t") + std::string("\t") + "float blend = " + input_vars.at(1) + "." + axisn[i] + ";" + std::string("\n");
        code += std::string("\t") + std::string("\t") + "if (base < 0.5) {" + std::string("\n");
        code += std::string("\t") + std::string("\t") + std::string("\t") + output_vars.at(0) + "." + axisn[i] + " = (base * (blend + 0.5));" + std::string("\n");
        code += std::string("\t") + std::string("\t") + "} else {" + std::string("\n");
        code += std::string("\t") + std::string("\t") + std::string("\t") + output_vars.at(0) + "." + axisn[i] + " = (1.0 - (1.0 - base) * (1.0 - (blend - 0.5)));" + std::string("\n");
        code += std::string("\t") + std::string("\t") + std::string("}") + std::string("\n");
        code += std::string("\t") + std::string("}") + "\n\n";
      }
    } break;
    case VisualShaderNodeColorOp::Operator::OP_HARD_LIGHT: {
      for (int i{0}; i < 3; i++) {
        code += std::string("\t") + std::string("{") + std::string("\n");
        code += std::string("\t") + std::string("\t") + "float base = " + input_vars.at(0) + "." + axisn[i] + ";" + std::string("\n");
        code += std::string("\t") + std::string("\t") + "float blend = " + input_vars.at(1) + "." + axisn[i] + ";" + std::string("\n");
        code += std::string("\t") + std::string("\t") + "if (base < 0.5) {" + std::string("\n");
        code += std::string("\t") + std::string("\t") + std::string("\t") + output_vars.at(0) + "." + axisn[i] + " = (base * (2.0 * blend));" + std::string("\n");
        code += std::string("\t") + std::string("\t") + "} else {" + std::string("\n");
        code +=
            std::string("\t") + std::string("\t") + std::string("\t") + output_vars.at(0) + "." + axisn[i] + " = (1.0 - (1.0 - base) * (1.0 - 2.0 * (blend - 0.5)));" + std::string("\n");
        code += std::string("\t") + std::string("\t") + std::string("}") + std::string("\n");
        code += std::string("\t") + std::string("}") + "\n\n";
      }
    } break;
    default:
      break;
  }

  return code;
}

void VisualShaderNodeColorOp::set_operator(const VisualShaderNodeColorOp::Operator& op) {
  if (op < 0 || op >= OP_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeColorOp::set_operator: Invalid op: " << op << std::endl;
    return;
  }

  if (this->op == op) {
    return;
  }

  switch (op) {
    case OP_SCREEN:
      simple_decl = true;
      break;
    case OP_DIFFERENCE:
      simple_decl = true;
      break;
    case OP_DARKEN:
      simple_decl = true;
      break;
    case OP_LIGHTEN:
      simple_decl = true;
      break;
    case OP_OVERLAY:
      simple_decl = false;
      break;
    case OP_DODGE:
      simple_decl = true;
      break;
    case OP_BURN:
      simple_decl = true;
      break;
    case OP_SOFT_LIGHT:
      simple_decl = false;
      break;
    case OP_HARD_LIGHT:
      simple_decl = false;
      break;
    default:
      break;
  }

  this->op = op;
}

VisualShaderNodeColorOp::Operator VisualShaderNodeColorOp::get_operator() const { return op; }

std::vector<std::string> VisualShaderNodeColorOp::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("operator");
  return props;
}

/*************************************/
/* Float Funcs Node                  */
/*************************************/

VisualShaderNodeFloatFunc::VisualShaderNodeFloatFunc() : func(FUNC_SIN) {
  set_input_port_default_value(0, 0.0f);
}

std::string VisualShaderNodeFloatFunc::get_caption() const { return "FloatFunc"; }

int VisualShaderNodeFloatFunc::get_input_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeFloatFunc::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeFloatFunc::get_input_port_name([[maybe_unused]] const int& port) const { return ""; }

int VisualShaderNodeFloatFunc::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeFloatFunc::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeFloatFunc::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeFloatFunc::generate_code([[maybe_unused]] const int& id,
                                                     const std::vector<std::string>& input_vars,
                                                     const std::vector<std::string>& output_vars) const {
  std::string code;
  switch (func) {
    case VisualShaderNodeFloatFunc::Function::FUNC_SIN:
      code += std::string("\t") + output_vars.at(0) + " = sin(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_COS:
      code += std::string("\t") + output_vars.at(0) + " = cos(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_TAN:
      code += std::string("\t") + output_vars.at(0) + " = tan(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_ASIN:
      code += std::string("\t") + output_vars.at(0) + " = asin(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_ACOS:
      code += std::string("\t") + output_vars.at(0) + " = acos(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_ATAN:
      code += std::string("\t") + output_vars.at(0) + " = atan(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_SINH:
      code += std::string("\t") + output_vars.at(0) + " = sinh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_COSH:
      code += std::string("\t") + output_vars.at(0) + " = cosh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_TANH:
      code += std::string("\t") + output_vars.at(0) + " = tanh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_LOG:
      code += std::string("\t") + output_vars.at(0) + " = log(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_EXP:
      code += std::string("\t") + output_vars.at(0) + " = exp(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_SQRT:
      code += std::string("\t") + output_vars.at(0) + " = sqrt(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_ABS:
      code += std::string("\t") + output_vars.at(0) + " = abs(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_SIGN:
      code += std::string("\t") + output_vars.at(0) + " = sign(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_FLOOR:
      code += std::string("\t") + output_vars.at(0) + " = floor(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_ROUND:
      code += std::string("\t") + output_vars.at(0) + " = round(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_CEIL:
      code += std::string("\t") + output_vars.at(0) + " = ceil(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_FRACT:
      code += std::string("\t") + output_vars.at(0) + " = fract(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_SATURATE:
      code += std::string("\t") + output_vars.at(0) + " = min(max(" + input_vars.at(0) + ", 0.0), 1.0);" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_NEGATE:
      code += std::string("\t") + output_vars.at(0) + " = -(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_ACOSH:
      code += std::string("\t") + output_vars.at(0) + " = acosh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_ASINH:
      code += std::string("\t") + output_vars.at(0) + " = asinh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_ATANH:
      code += std::string("\t") + output_vars.at(0) + " = atanh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_DEGREES:
      code += std::string("\t") + output_vars.at(0) + " = degrees(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_EXP2:
      code += std::string("\t") + output_vars.at(0) + " = exp2(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_INVERSE_SQRT:
      code += std::string("\t") + output_vars.at(0) + " = inversesqrt(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_LOG2:
      code += std::string("\t") + output_vars.at(0) + " = log2(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_RADIANS:
      code += std::string("\t") + output_vars.at(0) + " = radians(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_RECIPROCAL:
      code += std::string("\t") + output_vars.at(0) + " = 1.0 / (" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_ROUNDEVEN:
      code += std::string("\t") + output_vars.at(0) + " = roundEven(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_TRUNC:
      code += std::string("\t") + output_vars.at(0) + " = trunc(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case VisualShaderNodeFloatFunc::Function::FUNC_ONEMINUS:
      code += std::string("\t") + output_vars.at(0) + " = 1.0 - " + input_vars.at(0) + ";" + std::string("\n");
      break;
    default:
      break;
  }
  return code;
}

void VisualShaderNodeFloatFunc::set_function(const Function& func) {
  if (func < 0 || func >= FUNC_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeFloatFunc::set_function: Invalid func: " << func << std::endl;
    return;
  }

  if (this->func == func) {
    return;
  }

  this->func = func;
}

VisualShaderNodeFloatFunc::Function VisualShaderNodeFloatFunc::get_function() const { return func; }

std::vector<std::string> VisualShaderNodeFloatFunc::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("function");
  return props;
}

/*************************************/
/* Int Funcs Node                    */
/*************************************/

VisualShaderNodeIntFunc::VisualShaderNodeIntFunc() : func(FUNC_SIGN) {
  set_input_port_default_value(0, 0);
}

std::string VisualShaderNodeIntFunc::get_caption() const { return "IntFunc"; }

int VisualShaderNodeIntFunc::get_input_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeIntFunc::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_INT;
}

std::string VisualShaderNodeIntFunc::get_input_port_name([[maybe_unused]] const int& port) const { return ""; }

int VisualShaderNodeIntFunc::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeIntFunc::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_INT;
}

std::string VisualShaderNodeIntFunc::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeIntFunc::generate_code([[maybe_unused]] const int& id,
                                                   const std::vector<std::string>& input_vars,
                                                   const std::vector<std::string>& output_vars) const {
  std::string code;
  switch (func) {
    case FUNC_ABS:
      code += std::string("\t") + output_vars.at(0) + " = abs(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_NEGATE:
      code += std::string("\t") + output_vars.at(0) + " = -(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_SIGN:
      code += std::string("\t") + output_vars.at(0) + " = sign(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_BITWISE_NOT:
      code += std::string("\t") + output_vars.at(0) + " = ~(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    default:
      break;
  }
  return code;
}

void VisualShaderNodeIntFunc::set_function(const Function& func) {
  if (func < 0 || func >= FUNC_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeFloatFunc::set_function: Invalid func: " << func << std::endl;
    return;
  }

  if (this->func == func) {
    return;
  }

  this->func = func;
}

VisualShaderNodeIntFunc::Function VisualShaderNodeIntFunc::get_function() const { return func; }

std::vector<std::string> VisualShaderNodeIntFunc::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("function");
  return props;
}

/*************************************/
/* UInt Funcs Node                   */
/*************************************/

VisualShaderNodeUIntFunc::VisualShaderNodeUIntFunc() : func(FUNC_NEGATE) {
  set_input_port_default_value(0, 0);
}

std::string VisualShaderNodeUIntFunc::get_caption() const { return "UIntFunc"; }

int VisualShaderNodeUIntFunc::get_input_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeUIntFunc::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_UINT;
}

std::string VisualShaderNodeUIntFunc::get_input_port_name([[maybe_unused]] const int& port) const { return ""; }

int VisualShaderNodeUIntFunc::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeUIntFunc::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR_UINT;
}

std::string VisualShaderNodeUIntFunc::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeUIntFunc::generate_code([[maybe_unused]] const int& id,
                                                    const std::vector<std::string>& input_vars,
                                                    const std::vector<std::string>& output_vars) const {
  std::string code;
  switch (func) {
    case FUNC_NEGATE:
      code += std::string("\t") + output_vars.at(0) + " = -(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_BITWISE_NOT:
      code += std::string("\t") + output_vars.at(0) + " = ~(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    default:
      break;
  }
  return code;
}

void VisualShaderNodeUIntFunc::set_function(const Function& func) {
  if (func < 0 || func >= FUNC_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeFloatFunc::set_function: Invalid func: " << func << std::endl;
    return;
  }

  if (this->func == func) {
    return;
  }

  this->func = func;
}

VisualShaderNodeUIntFunc::Function VisualShaderNodeUIntFunc::get_function() const { return func; }

std::vector<std::string> VisualShaderNodeUIntFunc::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("function");
  return props;
}

/*************************************/
/* Vector Funcs Node                 */
/*************************************/

VisualShaderNodeVectorFunc::VisualShaderNodeVectorFunc() : func(FUNC_NORMALIZE) {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2());
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3());
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4());
    } break;
    default:
      break;
  }
}

std::string VisualShaderNodeVectorFunc::get_caption() const { return "VectorFunc"; }

int VisualShaderNodeVectorFunc::get_input_port_count() const { return 1; }

std::string VisualShaderNodeVectorFunc::get_input_port_name([[maybe_unused]] const int& port) const { return ""; }

int VisualShaderNodeVectorFunc::get_output_port_count() const { return 1; }

std::string VisualShaderNodeVectorFunc::get_output_port_name([[maybe_unused]] const int& port) const {
  return "result";
}

std::string VisualShaderNodeVectorFunc::generate_code([[maybe_unused]] const int& id,
                                                      const std::vector<std::string>& input_vars,
                                                      const std::vector<std::string>& output_vars) const {
  std::string code;
  switch (func) {
    case FUNC_NORMALIZE:
      code += std::string("\t") + output_vars.at(0) + " = normalize(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_SATURATE:
      switch (op_type) {
        case OP_TYPE_VECTOR_2D:
          code += std::string("\t") + output_vars.at(0) + " = " + "max(min(" + input_vars.at(0) + ", vec2(1.0)), vec2(0.0));" + std::string("\n");
          break;
        case OP_TYPE_VECTOR_3D:
          code += std::string("\t") + output_vars.at(0) + " = " + "max(min(" + input_vars.at(0) + ", vec3(1.0)), vec3(0.0));" + std::string("\n");
          break;
        case OP_TYPE_VECTOR_4D:
          code += std::string("\t") + output_vars.at(0) + " = " + "max(min(" + input_vars.at(0) + ", vec4(1.0)), vec4(0.0));" + std::string("\n");
          break;
        default:
          break;
      }
      break;
    case FUNC_NEGATE:
      code += std::string("\t") + output_vars.at(0) + " = -(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_RECIPROCAL:
      code += std::string("\t") + output_vars.at(0) + " = 1.0 / (" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_ABS:
      code += std::string("\t") + output_vars.at(0) + " = abs(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_ACOS:
      code += std::string("\t") + output_vars.at(0) + " = acos(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_ACOSH:
      code += std::string("\t") + output_vars.at(0) + " = acosh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_ASIN:
      code += std::string("\t") + output_vars.at(0) + " = asin(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_ASINH:
      code += std::string("\t") + output_vars.at(0) + " = asinh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_ATAN:
      code += std::string("\t") + output_vars.at(0) + " = atan(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_ATANH:
      code += std::string("\t") + output_vars.at(0) + " = atanh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_CEIL:
      code += std::string("\t") + output_vars.at(0) + " = ceil(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_COS:
      code += std::string("\t") + output_vars.at(0) + " = cos(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_COSH:
      code += std::string("\t") + output_vars.at(0) + " = cosh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_DEGREES:
      code += std::string("\t") + output_vars.at(0) + " = degrees(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_EXP:
      code += std::string("\t") + output_vars.at(0) + " = exp(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_EXP2:
      code += std::string("\t") + output_vars.at(0) + " = exp2(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_FLOOR:
      code += std::string("\t") + output_vars.at(0) + " = floor(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_FRACT:
      code += std::string("\t") + output_vars.at(0) + " = fract(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_INVERSE_SQRT:
      code += std::string("\t") + output_vars.at(0) + " = inversesqrt(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_LOG:
      code += std::string("\t") + output_vars.at(0) + " = log(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_LOG2:
      code += std::string("\t") + output_vars.at(0) + " = log2(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_RADIANS:
      code += std::string("\t") + output_vars.at(0) + " = radians(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_ROUND:
      code += std::string("\t") + output_vars.at(0) + " = round(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_ROUNDEVEN:
      code += std::string("\t") + output_vars.at(0) + " = roundEven(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_SIGN:
      code += std::string("\t") + output_vars.at(0) + " = sign(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_SIN:
      code += std::string("\t") + output_vars.at(0) + " = sin(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_SINH:
      code += std::string("\t") + output_vars.at(0) + " = sinh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_SQRT:
      code += std::string("\t") + output_vars.at(0) + " = sqrt(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_TAN:
      code += std::string("\t") + output_vars.at(0) + " = tan(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_TANH:
      code += std::string("\t") + output_vars.at(0) + " = tanh(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_TRUNC:
      code += std::string("\t") + output_vars.at(0) + " = trunc(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_ONEMINUS:
      switch (op_type) {
        case OP_TYPE_VECTOR_2D:
          code += std::string("\t") + output_vars.at(0) + " = vec2(1.0) - " + input_vars.at(0) + ";" + std::string("\n");
          break;
        case OP_TYPE_VECTOR_3D:
          code += std::string("\t") + output_vars.at(0) + " = vec3(1.0) - " + input_vars.at(0) + ";" + std::string("\n");
          break;
        case OP_TYPE_VECTOR_4D:
          code += std::string("\t") + output_vars.at(0) + " = vec4(1.0) - " + input_vars.at(0) + ";" + std::string("\n");
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  return code;
}

void VisualShaderNodeVectorFunc::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeVectorFunc::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2(), get_input_port_default_value(0));
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3(), get_input_port_default_value(0));
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4(), get_input_port_default_value(0));
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

void VisualShaderNodeVectorFunc::set_function(const Function& func) {
  if (func < 0 || func >= FUNC_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeFloatFunc::set_function: Invalid func: " << func << std::endl;
    return;
  }

  if (this->func == func) {
    return;
  }

  this->func = func;
}

VisualShaderNodeVectorFunc::Function VisualShaderNodeVectorFunc::get_function() const { return func; }

std::vector<std::string> VisualShaderNodeVectorFunc::get_editable_properties() const {
  std::vector<std::string> props = VisualShaderNodeVectorBase::get_editable_properties();
  props.emplace_back("function");
  return props;
}

/*************************************/
/* Color Funcs Node                  */
/*************************************/

VisualShaderNodeColorFunc::VisualShaderNodeColorFunc() : func(FUNC_GRAYSCALE) {
  simple_decl = false;
  set_input_port_default_value(0, TVector3());
}

std::string VisualShaderNodeColorFunc::get_caption() const { return "ColorFunc"; }

int VisualShaderNodeColorFunc::get_input_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeColorFunc::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_3D;
}

std::string VisualShaderNodeColorFunc::get_input_port_name([[maybe_unused]] const int& port) const { return ""; }

int VisualShaderNodeColorFunc::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeColorFunc::get_output_port_type(const int& port) const {
  switch (port) {
    case 0:
      return PORT_TYPE_VECTOR_3D;
    default:
      std::cerr << "VisualShaderNodeColorFunc::get_output_port_type: Invalid port: " << port << std::endl;
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeColorFunc::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeColorFunc::generate_code([[maybe_unused]] const int& id,
                                                     const std::vector<std::string>& input_vars,
                                                     const std::vector<std::string>& output_vars) const {
  std::string code;

  switch (func) {
    case FUNC_GRAYSCALE:
      code += std::string("\t") + std::string("{") + std::string("\n");
      code += std::string("\t") + std::string("\t") + "vec3 c = " + input_vars.at(0) + ";" + std::string("\n");
      code += std::string("\t") + std::string("\t") + "float max1 = max(c.r, c.g);" + std::string("\n");
      code += std::string("\t") + std::string("\t") + "float max2 = max(max1, c.b);" + std::string("\n");
      code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = vec3(max2, max2, max2);" + std::string("\n");
      code += std::string("\t") + std::string("}") + std::string("\n");
      break;
    case FUNC_HSV2RGB:
      code += std::string("\t") + std::string("{") + std::string("\n");
      code += std::string("\t") + std::string("\t") + "vec3 c = " + input_vars.at(0) + ";" + std::string("\n");
      code += std::string("\t") + std::string("\t") + "vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);" + std::string("\n");
      code += std::string("\t") + std::string("\t") + "vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);" + std::string("\n");
      code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);" + std::string("\n");
      code += std::string("\t") + std::string("}") + std::string("\n");
      break;
    case FUNC_RGB2HSV:
      code += std::string("\t") + std::string("{") + std::string("\n");
      code += std::string("\t") + std::string("\t") + "vec3 c = " + input_vars.at(0) + ";" + std::string("\n");
      code += std::string("\t") + std::string("\t") + "vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);" + std::string("\n");
      code +=
          std::string("\t") + std::string("\t") + "vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, "
          "c.g));" + std::string("\n");
      code +=
          std::string("\t") + std::string("\t") + "vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, "
          "c.r));" + std::string("\n");
      code += std::string("\t") + std::string("\t") + "float d = q.x - min(q.w, q.y);" + std::string("\n");
      code += std::string("\t") + std::string("\t") + "float e = 1.0e-10;" + std::string("\n");
      code += std::string("\t") + std::string("\t") + output_vars.at(0) +
              " = vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), "
              "q.x);" + std::string("\n");
      code += std::string("\t") + std::string("}") + std::string("\n");
      break;
    case FUNC_SEPIA:
      code += std::string("\t") + std::string("{") + std::string("\n");
      code += std::string("\t") + std::string("\t") + "vec3 c = " + input_vars.at(0) + ";" + std::string("\n");
      code += std::string("\t") + std::string("\t") + "float r = (c.r * .393) + (c.g *.769) + (c.b * .189);" + std::string("\n");
      code += std::string("\t") + std::string("\t") + "float g = (c.r * .349) + (c.g *.686) + (c.b * .168);" + std::string("\n");
      code += std::string("\t") + std::string("\t") + "float b = (c.r * .272) + (c.g *.534) + (c.b * .131);" + std::string("\n");
      code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = vec3(r, g, b);" + std::string("\n");
      code += std::string("\t") + std::string("}") + std::string("\n");
      break;
    default:
      break;
  }

  return code;
}

void VisualShaderNodeColorFunc::set_function(const Function& func) {
  if (func < 0 || func >= FUNC_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeFloatFunc::set_function: Invalid func: " << func << std::endl;
    return;
  }

  if (this->func == func) {
    return;
  }

  this->func = func;
}

VisualShaderNodeColorFunc::Function VisualShaderNodeColorFunc::get_function() const { return func; }

std::vector<std::string> VisualShaderNodeColorFunc::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("function");
  return props;
}

/*************************************/
/* Dot Node                          */
/*************************************/

VisualShaderNodeDotProduct::VisualShaderNodeDotProduct() {
  set_input_port_default_value(0, TVector3());
  set_input_port_default_value(1, TVector3());
}

std::string VisualShaderNodeDotProduct::get_caption() const { return "DotProduct"; }

int VisualShaderNodeDotProduct::get_input_port_count() const { return 2; }

VisualShaderNode::PortType VisualShaderNodeDotProduct::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_VECTOR_3D;
}

std::string VisualShaderNodeDotProduct::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "p1";
    case 1:
      return "p2";
    default:
      std::cerr << "VisualShaderNodeDotProduct::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return "";
}

int VisualShaderNodeDotProduct::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeDotProduct::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeDotProduct::get_output_port_name([[maybe_unused]] const int& port) const { return "dot"; }

std::string VisualShaderNodeDotProduct::generate_code([[maybe_unused]] const int& id,
                                                      const std::vector<std::string>& input_vars,
                                                      const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = dot(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
}

/*************************************/
/* Length Node                       */
/*************************************/

VisualShaderNodeVectorLen::VisualShaderNodeVectorLen() {
  set_input_port_default_value(0, TVector3(0.0f, 0.0f, 0.0f));
}

std::string VisualShaderNodeVectorLen::get_caption() const { return "VectorLen"; }

int VisualShaderNodeVectorLen::get_input_port_count() const { return 1; }

std::string VisualShaderNodeVectorLen::get_input_port_name([[maybe_unused]] const int& port) const { return ""; }

int VisualShaderNodeVectorLen::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeVectorLen::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeVectorLen::get_output_port_name([[maybe_unused]] const int& port) const { return "length"; }

void VisualShaderNodeVectorLen::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeVectorLen::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2(), get_input_port_default_value(0));
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3(), get_input_port_default_value(0));
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4(), get_input_port_default_value(0));
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

std::string VisualShaderNodeVectorLen::generate_code([[maybe_unused]] const int& id,
                                                     const std::vector<std::string>& input_vars,
                                                     const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = length(" + input_vars.at(0) + ");" + std::string("\n");
}

/*************************************/
/* Clamp Node                        */
/*************************************/

VisualShaderNodeClamp::VisualShaderNodeClamp() : op_type(OP_TYPE_FLOAT) {
  set_input_port_default_value(0, 0.0f);
  set_input_port_default_value(1, 0.0f);
  set_input_port_default_value(2, 1.0f);
}

std::string VisualShaderNodeClamp::get_caption() const { return "Clamp"; }

int VisualShaderNodeClamp::get_input_port_count() const { return 3; }

VisualShaderNode::PortType VisualShaderNodeClamp::get_input_port_type([[maybe_unused]] const int& port) const {
  switch (op_type) {
    case OP_TYPE_INT:
      return PORT_TYPE_SCALAR_INT;
    case OP_TYPE_UINT:
      return PORT_TYPE_SCALAR_UINT;
    case OP_TYPE_VECTOR_2D:
      return PORT_TYPE_VECTOR_2D;
    case OP_TYPE_VECTOR_3D:
      return PORT_TYPE_VECTOR_3D;
    case OP_TYPE_VECTOR_4D:
      return PORT_TYPE_VECTOR_4D;
    default:
      break;
  }
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeClamp::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "x";
    case 1:
      return "min";
    case 2:
      return "max";
    default:
      std::cerr << "VisualShaderNodeClamp::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return "";
}

int VisualShaderNodeClamp::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeClamp::get_output_port_type(const int& port) const {
  switch (op_type) {
    case OP_TYPE_INT:
      return PORT_TYPE_SCALAR_INT;
    case OP_TYPE_UINT:
      return PORT_TYPE_SCALAR_UINT;
    case OP_TYPE_VECTOR_2D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_2D;
        default:
          std::cerr << "VisualShaderNodeClamp::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_3D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_3D;
        default:
          std::cerr << "VisualShaderNodeClamp::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_4D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_4D;
        default:
          std::cerr << "VisualShaderNodeClamp::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeClamp::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeClamp::generate_code([[maybe_unused]] const int& id,
                                                 const std::vector<std::string>& input_vars,
                                                 const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = clamp(" + input_vars.at(0) + ", " + input_vars.at(1) + ", " + input_vars.at(2) +
         ");" + std::string("\n");
}

void VisualShaderNodeClamp::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeClamp::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_FLOAT:
      set_input_port_default_value(0, 0.0f, get_input_port_default_value(0));
      set_input_port_default_value(1, 0.0f, get_input_port_default_value(1));
      set_input_port_default_value(2, 0.0f, get_input_port_default_value(2));
      break;
    case OP_TYPE_UINT:
    case OP_TYPE_INT:
      set_input_port_default_value(0, 0, get_input_port_default_value(0));
      set_input_port_default_value(1, 0, get_input_port_default_value(1));
      set_input_port_default_value(2, 0, get_input_port_default_value(2));
      break;
    case OP_TYPE_VECTOR_2D:
      set_input_port_default_value(0, TVector2(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector2(), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector2(), get_input_port_default_value(2));
      break;
    case OP_TYPE_VECTOR_3D:
      set_input_port_default_value(0, TVector3(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector3(), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector3(), get_input_port_default_value(2));
      break;
    case OP_TYPE_VECTOR_4D:
      set_input_port_default_value(0, TVector4(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector4(), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector4(), get_input_port_default_value(2));
      break;
    default:
      break;
  }

  this->op_type = op_type;
}

VisualShaderNodeClamp::OpType VisualShaderNodeClamp::get_op_type() const { return op_type; }

std::vector<std::string> VisualShaderNodeClamp::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("op_type");
  return props;
}

/*************************************/
/* Derivative Func Node              */
/*************************************/

VisualShaderNodeDerivativeFunc::VisualShaderNodeDerivativeFunc()
    : op_type(OP_TYPE_SCALAR), func(FUNC_SUM), precision(PRECISION_NONE) {
  set_input_port_default_value(0, 0.0f);
}

std::string VisualShaderNodeDerivativeFunc::get_caption() const { return "DerivativeFunc"; }

int VisualShaderNodeDerivativeFunc::get_input_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeDerivativeFunc::get_input_port_type([[maybe_unused]] const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
      return PORT_TYPE_VECTOR_2D;
    case OP_TYPE_VECTOR_3D:
      return PORT_TYPE_VECTOR_3D;
    case OP_TYPE_VECTOR_4D:
      return PORT_TYPE_VECTOR_4D;
    default:
      break;
  }
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeDerivativeFunc::get_input_port_name([[maybe_unused]] const int& port) const { return "p"; }

int VisualShaderNodeDerivativeFunc::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeDerivativeFunc::get_output_port_type(
    [[maybe_unused]] const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_2D;
        default:
          std::cerr << "VisualShaderNodeDerivativeFunc::get_output_port_type: "
                       "Invalid port: "
                    << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_3D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_3D;
        default:
          std::cerr << "VisualShaderNodeDerivativeFunc::get_output_port_type: "
                       "Invalid port: "
                    << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_4D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_4D;
        default:
          std::cerr << "VisualShaderNodeDerivativeFunc::get_output_port_type: "
                       "Invalid port: "
                    << port << std::endl;
          break;
      }
      break;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeDerivativeFunc::get_output_port_name([[maybe_unused]] const int& port) const {
  return "result";
}

std::string VisualShaderNodeDerivativeFunc::generate_code([[maybe_unused]] const int& id,
                                                          const std::vector<std::string>& input_vars,
                                                          const std::vector<std::string>& output_vars) const {
  std::string code;
  switch (func) {
    case FUNC_SUM:
      switch (precision) {
        case PRECISION_NONE:  // Set to none in case of gl_compatibility
                              // profile.
          code += std::string("\t") + output_vars.at(0) + " = fwidth(" + input_vars.at(0) + ");" + std::string("\n");
          break;
        case PRECISION_COARSE:
          code += std::string("\t") + output_vars.at(0) + " = fwidthCoarse(" + input_vars.at(0) + ");" + std::string("\n");
          break;
        case PRECISION_FINE:
          code += std::string("\t") + output_vars.at(0) + " = fwidthFine(" + input_vars.at(0) + ");" + std::string("\n");
          break;
        default:
          break;
      }
      break;
    case FUNC_X:
      switch (precision) {
        case PRECISION_NONE:  // Set to none in case of gl_compatibility
                              // profile.
          code += std::string("\t") + output_vars.at(0) + " = dFdx(" + input_vars.at(0) + ");" + std::string("\n");
          break;
        case PRECISION_COARSE:
          code += std::string("\t") + output_vars.at(0) + " = dFdxCoarse(" + input_vars.at(0) + ");" + std::string("\n");
          break;
        case PRECISION_FINE:
          code += std::string("\t") + output_vars.at(0) + " = dFdxFine(" + input_vars.at(0) + ");" + std::string("\n");
          break;
        default:
          break;
      }
      break;
    case FUNC_Y:
      switch (precision) {
        case PRECISION_NONE:  // Set to none in case of gl_compatibility
                              // profile.
          code += std::string("\t") + output_vars.at(0) + " = dFdy(" + input_vars.at(0) + ");" + std::string("\n");
          break;
        case PRECISION_COARSE:
          code += std::string("\t") + output_vars.at(0) + " = dFdyCoarse(" + input_vars.at(0) + ");" + std::string("\n");
          break;
        case PRECISION_FINE:
          code += std::string("\t") + output_vars.at(0) + " = dFdyFine(" + input_vars.at(0) + ");" + std::string("\n");
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

  return code;
}

void VisualShaderNodeDerivativeFunc::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeDerivativeFunc::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_SCALAR: {
      set_input_port_default_value(0, 0.0f, get_input_port_default_value(0));
    } break;
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2(), get_input_port_default_value(0));
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3(), get_input_port_default_value(0));
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4(), get_input_port_default_value(0));
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

VisualShaderNodeDerivativeFunc::OpType VisualShaderNodeDerivativeFunc::get_op_type() const { return op_type; }

void VisualShaderNodeDerivativeFunc::set_function(const Function& func) {
  if (func < 0 || func >= FUNC_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeDerivativeFunc::set_function: Invalid func: " << func << std::endl;
    return;
  }

  if (this->func == func) {
    return;
  }

  this->func = func;
}

VisualShaderNodeDerivativeFunc::Function VisualShaderNodeDerivativeFunc::get_function() const { return func; }

void VisualShaderNodeDerivativeFunc::set_precision(const Precision& precision) {
  if (precision < 0 || precision >= PRECISION_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeDerivativeFunc::set_precision: Invalid precision: " << precision << std::endl;
    return;
  }

  if (this->precision == precision) {
    return;
  }

  this->precision = precision;
}

VisualShaderNodeDerivativeFunc::Precision VisualShaderNodeDerivativeFunc::get_precision() const { return precision; }

std::vector<std::string> VisualShaderNodeDerivativeFunc::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("op_type");
  props.emplace_back("function");
  props.emplace_back("precision");
  return props;
}

/*************************************/
/* Face Forward Node                 */
/*************************************/

VisualShaderNodeFaceForward::VisualShaderNodeFaceForward() {
  set_input_port_default_value(0, TVector3(0.0f, 0.0f, 0.0f));
  set_input_port_default_value(1, TVector3(0.0f, 0.0f, 0.0f));
  set_input_port_default_value(2, TVector3(0.0f, 0.0f, 0.0f));
}

std::string VisualShaderNodeFaceForward::get_caption() const { return "FaceForward"; }

int VisualShaderNodeFaceForward::get_input_port_count() const { return 3; }

std::string VisualShaderNodeFaceForward::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "N";
    case 1:
      return "I";
    case 2:
      return "Nref";
    default:
      break;
  }
  return "";
}

int VisualShaderNodeFaceForward::get_output_port_count() const { return 1; }

std::string VisualShaderNodeFaceForward::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

void VisualShaderNodeFaceForward::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeFaceForward::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector2(), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector2(), get_input_port_default_value(2));
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector3(), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector3(), get_input_port_default_value(2));
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector4(), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector4(), get_input_port_default_value(2));
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

std::string VisualShaderNodeFaceForward::generate_code([[maybe_unused]] const int& id,
                                                       const std::vector<std::string>& input_vars,
                                                       const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = faceforward(" + input_vars.at(0) + ", " + input_vars.at(1) + ", " +
         input_vars.at(2) + ");" + std::string("\n");
}

/*************************************/
/* Step Node                         */
/*************************************/

VisualShaderNodeStep::VisualShaderNodeStep() : op_type(OP_TYPE_SCALAR) {
  set_input_port_default_value(0, 0.0f);
  set_input_port_default_value(1, 0.0f);
}

std::string VisualShaderNodeStep::get_caption() const { return "Step"; }

int VisualShaderNodeStep::get_input_port_count() const { return 2; }

VisualShaderNode::PortType VisualShaderNodeStep::get_input_port_type(const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
      return PORT_TYPE_VECTOR_2D;
    case OP_TYPE_VECTOR_2D_SCALAR:
      switch (port) {
        case 1:
          return PORT_TYPE_VECTOR_2D;
        default:
          break;
      }
      break;
    case OP_TYPE_VECTOR_3D:
      return PORT_TYPE_VECTOR_3D;
    case OP_TYPE_VECTOR_3D_SCALAR:
      switch (port) {
        case 1:
          return PORT_TYPE_VECTOR_3D;
        default:
          break;
      }
      break;
    case OP_TYPE_VECTOR_4D:
      return PORT_TYPE_VECTOR_4D;
    case OP_TYPE_VECTOR_4D_SCALAR:
      switch (port) {
        case 1:
          return PORT_TYPE_VECTOR_4D;
        default:
          break;
      }
      break;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeStep::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "edge";
    case 1:
      return "x";
    default:
      std::cerr << "VisualShaderNodeStep::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return std::string();
}

int VisualShaderNodeStep::get_default_input_port([[maybe_unused]] const PortType& type) const { return 1; }

int VisualShaderNodeStep::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeStep::get_output_port_type(const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
    case OP_TYPE_VECTOR_2D_SCALAR:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_2D;
        default:
          std::cerr << "VisualShaderNodeStep::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_3D:
    case OP_TYPE_VECTOR_3D_SCALAR:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_3D;
        default:
          std::cerr << "VisualShaderNodeStep::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_4D:
    case OP_TYPE_VECTOR_4D_SCALAR:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_4D;
        default:
          std::cerr << "VisualShaderNodeStep::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    default:
      break;
  }
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeStep::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

void VisualShaderNodeStep::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeStep::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_SCALAR: {
      set_input_port_default_value(0, 0.0f, get_input_port_default_value(0));
      set_input_port_default_value(1, 0.0f, get_input_port_default_value(1));
    } break;
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector2(), get_input_port_default_value(1));
    } break;
    case OP_TYPE_VECTOR_2D_SCALAR: {
      set_input_port_default_value(0, 0.0f, get_input_port_default_value(0));
      set_input_port_default_value(1, TVector2(), get_input_port_default_value(1));
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector3(), get_input_port_default_value(1));
    } break;
    case OP_TYPE_VECTOR_3D_SCALAR: {
      set_input_port_default_value(0, 0.0f, get_input_port_default_value(0));
      set_input_port_default_value(1, TVector3(), get_input_port_default_value(1));
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector4(), get_input_port_default_value(1));
    } break;
    case OP_TYPE_VECTOR_4D_SCALAR: {
      set_input_port_default_value(0, 0.0f, get_input_port_default_value(0));
      set_input_port_default_value(1, TVector4(), get_input_port_default_value(1));
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

VisualShaderNodeStep::OpType VisualShaderNodeStep::get_op_type() const { return op_type; }

std::string VisualShaderNodeStep::generate_code([[maybe_unused]] const int& id,
                                                const std::vector<std::string>& input_vars,
                                                const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = step(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
}

std::vector<std::string> VisualShaderNodeStep::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("op_type");
  return props;
}

/*************************************/
/* Smooth Step Node                  */
/*************************************/

VisualShaderNodeSmoothStep::VisualShaderNodeSmoothStep() : op_type(OP_TYPE_SCALAR) {
  set_input_port_default_value(0, 0.0f);  // edge0
  set_input_port_default_value(1, 1.0f);  // edge1
  set_input_port_default_value(2, 0.5f);  // x
}

std::string VisualShaderNodeSmoothStep::get_caption() const { return "SmoothStep"; }

int VisualShaderNodeSmoothStep::get_input_port_count() const { return 3; }

VisualShaderNode::PortType VisualShaderNodeSmoothStep::get_input_port_type(const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
      return PORT_TYPE_VECTOR_2D;
    case OP_TYPE_VECTOR_2D_SCALAR:
      switch (port) {
        case 2:
          return PORT_TYPE_VECTOR_2D;  // x
        default:
          break;
      }
      break;
    case OP_TYPE_VECTOR_3D:
      return PORT_TYPE_VECTOR_3D;
    case OP_TYPE_VECTOR_3D_SCALAR:
      switch (port) {
        case 2:
          return PORT_TYPE_VECTOR_3D;  // x
        default:
          break;
      }
      break;
    case OP_TYPE_VECTOR_4D:
      return PORT_TYPE_VECTOR_4D;
    case OP_TYPE_VECTOR_4D_SCALAR:
      switch (port) {
        case 2:
          return PORT_TYPE_VECTOR_4D;  // x
        default:
          break;
      }
      break;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeSmoothStep::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "edge0";
    case 1:
      return "edge1";
    case 2:
      return "x";
    default:
      std::cerr << "VisualShaderNodeSmoothStep::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return std::string();
}

int VisualShaderNodeSmoothStep::get_default_input_port([[maybe_unused]] const PortType& type) const { return 2; }

int VisualShaderNodeSmoothStep::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeSmoothStep::get_output_port_type(const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
    case OP_TYPE_VECTOR_2D_SCALAR:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_2D;
        default:
          std::cerr << "VisualShaderNodeSmoothStep::get_output_port_type: "
                       "Invalid port: "
                    << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_3D:
    case OP_TYPE_VECTOR_3D_SCALAR:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_3D;
        default:
          std::cerr << "VisualShaderNodeSmoothStep::get_output_port_type: "
                       "Invalid port: "
                    << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_4D:
    case OP_TYPE_VECTOR_4D_SCALAR:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_4D;
        default:
          std::cerr << "VisualShaderNodeSmoothStep::get_output_port_type: "
                       "Invalid port: "
                    << port << std::endl;
          break;
      }
      break;
    default:
      break;
  }
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeSmoothStep::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

void VisualShaderNodeSmoothStep::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeSmoothStep::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_SCALAR:
      set_input_port_default_value(0, 0.0f,
                                   get_input_port_default_value(0));  // edge0
      set_input_port_default_value(1, 0.0f,
                                   get_input_port_default_value(1));  // edge1
      set_input_port_default_value(2, 0.0f,
                                   get_input_port_default_value(2));  // x
      break;
    case OP_TYPE_VECTOR_2D:
      set_input_port_default_value(0, TVector2(),
                                   get_input_port_default_value(0));  // edge0
      set_input_port_default_value(1, TVector2(),
                                   get_input_port_default_value(1));  // edge1
      set_input_port_default_value(2, TVector2(),
                                   get_input_port_default_value(2));  // x
      break;
    case OP_TYPE_VECTOR_2D_SCALAR:
      set_input_port_default_value(0, 0.0f,
                                   get_input_port_default_value(0));  // edge0
      set_input_port_default_value(1, 0.0f,
                                   get_input_port_default_value(1));  // edge1
      set_input_port_default_value(2, TVector2(),
                                   get_input_port_default_value(2));  // x
      break;
    case OP_TYPE_VECTOR_3D:
      set_input_port_default_value(0, TVector3(),
                                   get_input_port_default_value(0));  // edge0
      set_input_port_default_value(1, TVector3(),
                                   get_input_port_default_value(1));  // edge1
      set_input_port_default_value(2, TVector3(),
                                   get_input_port_default_value(2));  // x
      break;
    case OP_TYPE_VECTOR_3D_SCALAR:
      set_input_port_default_value(0, 0.0f,
                                   get_input_port_default_value(0));  // edge0
      set_input_port_default_value(1, 0.0f,
                                   get_input_port_default_value(1));  // edge1
      set_input_port_default_value(2, TVector3(),
                                   get_input_port_default_value(2));  // x
      break;
    case OP_TYPE_VECTOR_4D:
      set_input_port_default_value(0, TVector4(),
                                   get_input_port_default_value(0));  // edge0
      set_input_port_default_value(1, TVector4(),
                                   get_input_port_default_value(1));  // edge1
      set_input_port_default_value(2, TVector4(),
                                   get_input_port_default_value(2));  // x
      break;
    case OP_TYPE_VECTOR_4D_SCALAR:
      set_input_port_default_value(0, 0.0f,
                                   get_input_port_default_value(0));  // edge0
      set_input_port_default_value(1, 0.0f,
                                   get_input_port_default_value(1));  // edge1
      set_input_port_default_value(2, TVector4(),
                                   get_input_port_default_value(2));  // x
      break;
    default:
      break;
  }

  this->op_type = op_type;
}

VisualShaderNodeSmoothStep::OpType VisualShaderNodeSmoothStep::get_op_type() const { return op_type; }

std::string VisualShaderNodeSmoothStep::generate_code([[maybe_unused]] const int& id,
                                                      const std::vector<std::string>& input_vars,
                                                      const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = smoothstep(" + input_vars.at(0) + ", " + input_vars.at(1) + ", " +
         input_vars.at(2) + ");" + std::string("\n");
}

std::vector<std::string> VisualShaderNodeSmoothStep::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("op_type");
  return props;
}

/*************************************/
/* Distance Node                     */
/*************************************/

VisualShaderNodeVectorDistance::VisualShaderNodeVectorDistance() {
  set_input_port_default_value(0, TVector3(0.0f, 0.0f, 0.0f));  // a
  set_input_port_default_value(1, TVector3(0.0f, 0.0f, 0.0f));  // b
}

std::string VisualShaderNodeVectorDistance::get_caption() const { return "Distance"; }

int VisualShaderNodeVectorDistance::get_input_port_count() const { return 2; }

std::string VisualShaderNodeVectorDistance::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "p1";
    case 1:
      return "p2";
  }
  return std::string();
}

int VisualShaderNodeVectorDistance::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeVectorDistance::get_output_port_type(
    [[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeVectorDistance::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

void VisualShaderNodeVectorDistance::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeVectorDistance::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2(),
                                   get_input_port_default_value(0));  // a
      set_input_port_default_value(1, TVector2(),
                                   get_input_port_default_value(1));  // b
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3(),
                                   get_input_port_default_value(0));  // a
      set_input_port_default_value(1, TVector3(),
                                   get_input_port_default_value(1));  // b
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4(),
                                   get_input_port_default_value(0));  // a
      set_input_port_default_value(1, TVector4(),
                                   get_input_port_default_value(1));  // b
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

std::string VisualShaderNodeVectorDistance::generate_code([[maybe_unused]] const int& id,
                                                          const std::vector<std::string>& input_vars,
                                                          const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = distance(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
}

/*************************************/
/* Refract Node                      */
/*************************************/

VisualShaderNodeVectorRefract::VisualShaderNodeVectorRefract() {
  set_input_port_default_value(0, TVector3(0.0f, 0.0f, 0.0f));
  set_input_port_default_value(1, TVector3(0.0f, 0.0f, 0.0f));
  set_input_port_default_value(2, 0.0f);
}

std::string VisualShaderNodeVectorRefract::get_caption() const { return "Refract"; }

int VisualShaderNodeVectorRefract::get_input_port_count() const { return 3; }

std::string VisualShaderNodeVectorRefract::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "I";
    case 1:
      return "N";
    case 2:
      return "eta";
  }
  return std::string();
}

int VisualShaderNodeVectorRefract::get_output_port_count() const { return 1; }

std::string VisualShaderNodeVectorRefract::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeVectorRefract::generate_code([[maybe_unused]] const int& id,
                                                         const std::vector<std::string>& input_vars,
                                                         const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = refract(" + input_vars.at(0) + ", " + input_vars.at(1) + ", " +
         input_vars.at(2) + ");" + std::string("\n");
}

void VisualShaderNodeVectorRefract::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeVectorRefract::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector2(), get_input_port_default_value(1));
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector3(), get_input_port_default_value(1));
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector4(), get_input_port_default_value(1));
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

/*************************************/
/* Mix Node                          */
/*************************************/

VisualShaderNodeMix::VisualShaderNodeMix() : op_type(OP_TYPE_SCALAR) {
  set_input_port_default_value(0, 0.0f);  // a
  set_input_port_default_value(1, 1.0f);  // b
  set_input_port_default_value(2, 0.5f);  // weight
}

std::string VisualShaderNodeMix::get_caption() const { return "Mix"; }

int VisualShaderNodeMix::get_input_port_count() const { return 3; }

VisualShaderNode::PortType VisualShaderNodeMix::get_input_port_type(const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
      return PORT_TYPE_VECTOR_2D;
    case OP_TYPE_VECTOR_2D_SCALAR:
      switch (port) {
        case 0:
        case 1:
          return PORT_TYPE_VECTOR_2D;
        case 2:
          break;
        default:
          std::cerr << "VisualShaderNodeMix::get_input_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_3D:
      return PORT_TYPE_VECTOR_3D;
    case OP_TYPE_VECTOR_3D_SCALAR:
      switch (port) {
        case 0:
        case 1:
          return PORT_TYPE_VECTOR_3D;
        case 2:
          break;
        default:
          std::cerr << "VisualShaderNodeMix::get_input_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_4D:
      return PORT_TYPE_VECTOR_4D;
    case OP_TYPE_VECTOR_4D_SCALAR:
      switch (port) {
        case 0:
        case 1:
          return PORT_TYPE_VECTOR_4D;
        case 2:
          break;
        default:
          std::cerr << "VisualShaderNodeMix::get_input_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeMix::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "p1";
    case 1:
      return "p2";
    case 2:
      return "weight";
    default:
      std::cerr << "VisualShaderNodeMix::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return std::string();
}

int VisualShaderNodeMix::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeMix::get_output_port_type(const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
    case OP_TYPE_VECTOR_2D_SCALAR:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_2D;
        default:
          std::cerr << "VisualShaderNodeMix::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_3D:
    case OP_TYPE_VECTOR_3D_SCALAR:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_3D;
        default:
          std::cerr << "VisualShaderNodeMix::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_4D:
    case OP_TYPE_VECTOR_4D_SCALAR:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_4D;
        default:
          std::cerr << "VisualShaderNodeMix::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeMix::get_output_port_name([[maybe_unused]] const int& port) const { return "mix"; }

void VisualShaderNodeMix::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeMix::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_SCALAR: {
      set_input_port_default_value(0, 0.0f,
                                   get_input_port_default_value(0));  // a
      set_input_port_default_value(1, 0.0f,
                                   get_input_port_default_value(1));  // b
      set_input_port_default_value(2, 0.0f,
                                   get_input_port_default_value(2));  // weight
    } break;
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2(),
                                   get_input_port_default_value(0));  // a
      set_input_port_default_value(1, TVector2(),
                                   get_input_port_default_value(1));  // b
      set_input_port_default_value(2, TVector2(),
                                   get_input_port_default_value(2));  // weight
    } break;
    case OP_TYPE_VECTOR_2D_SCALAR: {
      set_input_port_default_value(0, TVector2(),
                                   get_input_port_default_value(0));  // a
      set_input_port_default_value(1, TVector2(),
                                   get_input_port_default_value(1));  // b
      set_input_port_default_value(2, 0.0f,
                                   get_input_port_default_value(2));  // weight
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3(),
                                   get_input_port_default_value(0));  // a
      set_input_port_default_value(1, TVector3(),
                                   get_input_port_default_value(1));  // b
      set_input_port_default_value(2, TVector3(),
                                   get_input_port_default_value(2));  // weight
    } break;
    case OP_TYPE_VECTOR_3D_SCALAR: {
      set_input_port_default_value(0, TVector3(),
                                   get_input_port_default_value(0));  // a
      set_input_port_default_value(1, TVector3(),
                                   get_input_port_default_value(1));  // b
      set_input_port_default_value(2, 0.0f,
                                   get_input_port_default_value(2));  // weight
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4(),
                                   get_input_port_default_value(0));  // a
      set_input_port_default_value(1, TVector4(),
                                   get_input_port_default_value(1));  // b
      set_input_port_default_value(2, TVector4(),
                                   get_input_port_default_value(2));  // weight
    } break;
    case OP_TYPE_VECTOR_4D_SCALAR: {
      set_input_port_default_value(0, TVector4(),
                                   get_input_port_default_value(0));  // a
      set_input_port_default_value(1, TVector4(),
                                   get_input_port_default_value(1));  // b
      set_input_port_default_value(2, 0.0f,
                                   get_input_port_default_value(2));  // weight
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

VisualShaderNodeMix::OpType VisualShaderNodeMix::get_op_type() const { return op_type; }

std::string VisualShaderNodeMix::generate_code([[maybe_unused]] const int& id,
                                               const std::vector<std::string>& input_vars,
                                               const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = mix(" + input_vars.at(0) + ", " + input_vars.at(1) + ", " + input_vars.at(2) +
         ");" + std::string("\n");
}

std::vector<std::string> VisualShaderNodeMix::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("op_type");
  return props;
}

/*************************************/
/* Vector Compose Node               */
/*************************************/

VisualShaderNodeVectorCompose::VisualShaderNodeVectorCompose() {
  set_input_port_default_value(0, 0.0f);
  set_input_port_default_value(1, 0.0f);
  set_input_port_default_value(2, 0.0f);
}

std::string VisualShaderNodeVectorCompose::get_caption() const { return "VectorCompose"; }

int VisualShaderNodeVectorCompose::get_input_port_count() const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
      return 2;
    case OP_TYPE_VECTOR_3D:
      return 3;
    case OP_TYPE_VECTOR_4D:
      return 4;
    default:
      break;
  }
  return 0;
}

VisualShaderNode::PortType VisualShaderNodeVectorCompose::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeVectorCompose::get_input_port_name(const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      switch (port) {
        case 0:
          return "x";
        case 1:
          return "y";
      }
    } break;
    case OP_TYPE_VECTOR_3D: {
      switch (port) {
        case 0:
          return "x";
        case 1:
          return "y";
        case 2:
          return "z";
      }
    } break;
    case OP_TYPE_VECTOR_4D: {
      switch (port) {
        case 0:
          return "x";
        case 1:
          return "y";
        case 2:
          return "z";
        case 3:
          return "w";
      }
    } break;
    default:
      break;
  }
  return std::string();
}

int VisualShaderNodeVectorCompose::get_output_port_count() const { return 1; }

std::string VisualShaderNodeVectorCompose::get_output_port_name([[maybe_unused]] const int& port) const {
  return "vec";
}

void VisualShaderNodeVectorCompose::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeVectorCompose::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  float p1{std::get<float>(get_input_port_default_value(0))};
  float p2{std::get<float>(get_input_port_default_value(1))};

  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, p1);
      set_input_port_default_value(1, p2);
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, p1);
      set_input_port_default_value(1, p2);
      set_input_port_default_value(2, 0.0f);
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, p1);
      set_input_port_default_value(1, p2);
      set_input_port_default_value(2, 0.0f);
      set_input_port_default_value(3, 0.0f);
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

std::string VisualShaderNodeVectorCompose::generate_code([[maybe_unused]] const int& id,
                                                         const std::vector<std::string>& input_vars,
                                                         const std::vector<std::string>& output_vars) const {
  std::string code;
  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      code += std::string("\t") + output_vars.at(0) + " = vec2(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
    } break;
    case OP_TYPE_VECTOR_3D: {
      code += std::string("\t") + output_vars.at(0) + " = vec3(" + input_vars.at(0) + ", " + input_vars.at(1) + ", " +
              input_vars.at(2) + ");" + std::string("\n");
    } break;
    case OP_TYPE_VECTOR_4D: {
      code += std::string("\t") + output_vars.at(0) + " = vec4(" + input_vars.at(0) + ", " + input_vars.at(1) + ", " +
              input_vars.at(2) + ", " + input_vars.at(3) + ");" + std::string("\n");
    } break;
    default:
      break;
  }
  return code;
}

/*************************************/
/* Vector Decompose Node             */
/*************************************/

VisualShaderNodeVectorDecompose::VisualShaderNodeVectorDecompose() {
  set_input_port_default_value(0, TVector3(0.0f, 0.0f, 0.0f));
}

std::string VisualShaderNodeVectorDecompose::get_caption() const { return "VectorDecompose"; }

int VisualShaderNodeVectorDecompose::get_input_port_count() const { return 1; }

std::string VisualShaderNodeVectorDecompose::get_input_port_name([[maybe_unused]] const int& port) const {
  return "vec";
}

int VisualShaderNodeVectorDecompose::get_output_port_count() const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
      return 2;
    case OP_TYPE_VECTOR_3D:
      return 3;
    case OP_TYPE_VECTOR_4D:
      return 4;
    default:
      break;
  }
  return 0;
}

VisualShaderNode::PortType VisualShaderNodeVectorDecompose::get_output_port_type(
    [[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeVectorDecompose::get_output_port_name(const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      switch (port) {
        case 0:
          return "x";
        case 1:
          return "y";
      }
    } break;
    case OP_TYPE_VECTOR_3D: {
      switch (port) {
        case 0:
          return "x";
        case 1:
          return "y";
        case 2:
          return "z";
      }
    } break;
    case OP_TYPE_VECTOR_4D: {
      switch (port) {
        case 0:
          return "x";
        case 1:
          return "y";
        case 2:
          return "z";
        case 3:
          return "w";
      }
    } break;
    default:
      break;
  }
  return std::string();
}

void VisualShaderNodeVectorDecompose::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeVectorDecompose::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2(), get_input_port_default_value(0));
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3(), get_input_port_default_value(0));
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4(), get_input_port_default_value(0));
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

std::string VisualShaderNodeVectorDecompose::generate_code([[maybe_unused]] const int& id,
                                                           const std::vector<std::string>& input_vars,
                                                           const std::vector<std::string>& output_vars) const {
  std::string code;
  switch (op_type) {
    case OP_TYPE_VECTOR_2D: {
      code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + ".x;" + std::string("\n");
      code += std::string("\t") + output_vars.at(1) + " = " + input_vars.at(0) + ".y;" + std::string("\n");
    } break;
    case OP_TYPE_VECTOR_3D: {
      code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + ".x;" + std::string("\n");
      code += std::string("\t") + output_vars.at(1) + " = " + input_vars.at(0) + ".y;" + std::string("\n");
      code += std::string("\t") + output_vars.at(2) + " = " + input_vars.at(0) + ".z;" + std::string("\n");
    } break;
    case OP_TYPE_VECTOR_4D: {
      code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + ".x;" + std::string("\n");
      code += std::string("\t") + output_vars.at(1) + " = " + input_vars.at(0) + ".y;" + std::string("\n");
      code += std::string("\t") + output_vars.at(2) + " = " + input_vars.at(0) + ".z;" + std::string("\n");
      code += std::string("\t") + output_vars.at(3) + " = " + input_vars.at(0) + ".w;" + std::string("\n");
    } break;
    default:
      break;
  }

  return code;
}

/*************************************/
/* If Node                           */
/*************************************/

VisualShaderNodeIf::VisualShaderNodeIf() {
  simple_decl = false;
  set_input_port_default_value(0, 0.0f);
  set_input_port_default_value(1, 0.0f);
  set_input_port_default_value(2, std::numeric_limits<float>::epsilon());
  set_input_port_default_value(3, TVector3(0.0f, 0.0f, 0.0f));
  set_input_port_default_value(4, TVector3(0.0f, 0.0f, 0.0f));
  set_input_port_default_value(5, TVector3(0.0f, 0.0f, 0.0f));
}

std::string VisualShaderNodeIf::get_caption() const { return "If"; }

int VisualShaderNodeIf::get_input_port_count() const { return 6; }

VisualShaderNode::PortType VisualShaderNodeIf::get_input_port_type(const int& port) const {
  switch (port) {
    case 0:
    case 1:
    case 2:
      return PORT_TYPE_SCALAR;
    case 3:
    case 4:
    case 5:
      return PORT_TYPE_VECTOR_3D;
    default:
      std::cerr << "VisualShaderNodeIf::get_input_port_type: Invalid port: " << port << std::endl;
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeIf::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "p1";
    case 1:
      return "p2";
    case 2:
      return "tolerance";
    case 3:
      return "p1 == p2";
    case 4:
      return "p1 > p2";
    case 5:
      return "p1 < p2";
    default:
      break;
  }
  return "";
}

int VisualShaderNodeIf::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeIf::get_output_port_type(const int& port) const {
  switch (port) {
    case 0:
      return PORT_TYPE_VECTOR_3D;
    default:
      std::cerr << "VisualShaderNodeIf::get_output_port_type: Invalid port: " << port << std::endl;
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeIf::get_output_port_name([[maybe_unused]] const int& port) const { return "result"; }

std::string VisualShaderNodeIf::generate_code([[maybe_unused]] const int& id,
                                              const std::vector<std::string>& input_vars,
                                              const std::vector<std::string>& output_vars) const {
  std::string code;
  code += std::string("\t") + "if(abs(" + input_vars.at(0) + " - " + input_vars.at(1) + ") < " + input_vars.at(2) +
          ") {" + std::string("\n");  // abs(p1 - p2) < tolerance eg. p1 == p2
  code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = " + input_vars.at(3) + ";" + std::string("\n");
  code += std::string("\t") + "} else if(" + input_vars.at(0) + " < " + input_vars.at(1) + ") {" + std::string("\n");  // p1 < p2
  code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = " + input_vars.at(5) + ";" + std::string("\n");
  code += std::string("\t") + "} else {" + std::string("\n");  // p1 > p2 (or p1 >= p2 if abs(p1 - p2) < tolerance is false)
  code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = " + input_vars.at(4) + ";" + std::string("\n");
  code += std::string("\t") + std::string("}") + std::string("\n");
  return code;
}

/*************************************/
/* Switch Node                       */
/*************************************/

VisualShaderNodeSwitch::VisualShaderNodeSwitch() : op_type(OP_TYPE_FLOAT) {
  simple_decl = false;
  set_input_port_default_value(0, false);
  set_input_port_default_value(1, 1.0f);
  set_input_port_default_value(2, 0.0f);
}

std::string VisualShaderNodeSwitch::get_caption() const { return "Switch"; }

int VisualShaderNodeSwitch::get_input_port_count() const { return 3; }

VisualShaderNode::PortType VisualShaderNodeSwitch::get_input_port_type(const int& port) const {
  switch (port) {
    case 0:
      return PORT_TYPE_BOOLEAN;
    case 1:
    case 2:
      switch (op_type) {
        case OP_TYPE_INT:
          return PORT_TYPE_SCALAR_INT;
        case OP_TYPE_UINT:
          return PORT_TYPE_SCALAR_UINT;
        case OP_TYPE_VECTOR_2D:
          return PORT_TYPE_VECTOR_2D;
        case OP_TYPE_VECTOR_3D:
          return PORT_TYPE_VECTOR_3D;
        case OP_TYPE_VECTOR_4D:
          return PORT_TYPE_VECTOR_4D;
        case OP_TYPE_BOOLEAN:
          return PORT_TYPE_BOOLEAN;
        default:
          break;
      }
      break;
    default:
      std::cerr << "VisualShaderNodeSwitch::get_input_port_type: Invalid port: " << port << std::endl;
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeSwitch::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "value";
    case 1:
      return "true";
    case 2:
      return "false";
    default:
      break;
  }
  return "";
}

int VisualShaderNodeSwitch::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeSwitch::get_output_port_type(const int& port) const {
  switch (op_type) {
    case OP_TYPE_INT:
      return PORT_TYPE_SCALAR_INT;
    case OP_TYPE_UINT:
      return PORT_TYPE_SCALAR_UINT;
    case OP_TYPE_VECTOR_2D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_2D;
        default:
          std::cerr << "VisualShaderNodeSwitch::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_3D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_3D;
        default:
          std::cerr << "VisualShaderNodeSwitch::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_4D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_4D;
        default:
          std::cerr << "VisualShaderNodeSwitch::get_output_port_type: Invalid port: " << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_BOOLEAN:
      return PORT_TYPE_BOOLEAN;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeSwitch::get_output_port_name([[maybe_unused]] const int& port) const { return "result"; }

void VisualShaderNodeSwitch::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeSwitch::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_FLOAT:
      set_input_port_default_value(1, 1.0f, get_input_port_default_value(1));
      set_input_port_default_value(2, 0.0f, get_input_port_default_value(2));
      break;
    case OP_TYPE_UINT:
    case OP_TYPE_INT:
      set_input_port_default_value(1, 1, get_input_port_default_value(1));
      set_input_port_default_value(2, 0, get_input_port_default_value(2));
      break;
    case OP_TYPE_VECTOR_2D:
      set_input_port_default_value(1, TVector2(1.0f, 1.0f), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector2(0.0f, 0.0f), get_input_port_default_value(2));
      break;
    case OP_TYPE_VECTOR_3D:
      set_input_port_default_value(1, TVector3(1.0f, 1.0f, 1.0f), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector3(0.0f, 0.0f, 0.0f), get_input_port_default_value(2));
      break;
    case OP_TYPE_VECTOR_4D:
      set_input_port_default_value(1, TVector4(1.0f, 1.0f, 1.0f, 1.0f), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector4(0.0f, 0.0f, 0.0f, 0.0f), get_input_port_default_value(2));
      break;
    case OP_TYPE_BOOLEAN:
      set_input_port_default_value(1, true);
      set_input_port_default_value(2, false);
      break;
    default:
      break;
  }

  this->op_type = op_type;
}

VisualShaderNodeSwitch::OpType VisualShaderNodeSwitch::get_op_type() const { return op_type; }

std::vector<std::string> VisualShaderNodeSwitch::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("op_type");
  return props;
}

std::string VisualShaderNodeSwitch::generate_code([[maybe_unused]] const int& id,
                                                  const std::vector<std::string>& input_vars,
                                                  const std::vector<std::string>& output_vars) const {
  bool use_mix{false};

  switch (op_type) {
    case OP_TYPE_FLOAT:
    case OP_TYPE_VECTOR_2D:
    case OP_TYPE_VECTOR_3D:
    case OP_TYPE_VECTOR_4D:
      use_mix = true;
      break;
    default:
      break;
  }

  std::string code;
  if (use_mix) {
    code += std::string("\t") + output_vars.at(0) + " = mix(" + input_vars.at(2) + ", " + input_vars.at(1) + ", float(" +
            input_vars.at(0) + "));" + std::string("\n");
  } else {
    code += std::string("\t") + "if (" + input_vars.at(0) + ") {" + std::string("\n");
    code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = " + input_vars.at(1) + ";" + std::string("\n");
    code += std::string("\t") + "} else {" + std::string("\n");
    code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = " + input_vars.at(2) + ";" + std::string("\n");
    code += std::string("\t") + std::string("}") + std::string("\n");
  }
  return code;
}

/*************************************/
/* Is Node                           */
/*************************************/

VisualShaderNodeIs::VisualShaderNodeIs() : func(FUNC_IS_INF) {
  set_input_port_default_value(0, 0.0f);
}

std::string VisualShaderNodeIs::get_caption() const { return "Is"; }

int VisualShaderNodeIs::get_input_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeIs::get_input_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeIs::get_input_port_name([[maybe_unused]] const int& port) const { return ""; }

int VisualShaderNodeIs::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeIs::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_BOOLEAN;
}

std::string VisualShaderNodeIs::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeIs::generate_code([[maybe_unused]] const int& id,
                                              const std::vector<std::string>& input_vars,
                                              const std::vector<std::string>& output_vars) const {
  std::string code;
  switch (func) {
    case FUNC_IS_INF:
      code += std::string("\t") + output_vars.at(0) + " = isinf(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    case FUNC_IS_NAN:
      code += std::string("\t") + output_vars.at(0) + " = isnan(" + input_vars.at(0) + ");" + std::string("\n");
      break;
    default:
      break;
  }
  return code;
}

void VisualShaderNodeIs::set_function(const Function& func) {
  if (func < 0 || func >= FUNC_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeIs::set_function: Invalid func: " << func << std::endl;
    return;
  }

  if (this->func == func) {
    return;
  }

  this->func = func;
}

VisualShaderNodeIs::Function VisualShaderNodeIs::get_function() const { return func; }

std::vector<std::string> VisualShaderNodeIs::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("function");
  return props;
}

/*************************************/
/* Compare Node                      */
/*************************************/

VisualShaderNodeCompare::VisualShaderNodeCompare()
    : comparison_type(CMP_TYPE_SCALAR), func(FUNC_EQUAL), condition(COND_ALL) {
  set_input_port_default_value(0, 0.0f);
  set_input_port_default_value(1, 0.0f);
  set_input_port_default_value(2, std::numeric_limits<float>::epsilon());
}

std::string VisualShaderNodeCompare::get_caption() const { return "Compare"; }

int VisualShaderNodeCompare::get_input_port_count() const {
  switch (comparison_type) {
    case CMP_TYPE_SCALAR:
      switch (func) {
        case FUNC_EQUAL:
        case FUNC_NOT_EQUAL:
          return 3;
        default:
          break;
      }
      break;
    default:
      break;
  }

  return 2;
}

VisualShaderNode::PortType VisualShaderNodeCompare::get_input_port_type([[maybe_unused]] const int& port) const {
  switch (comparison_type) {
    case CMP_TYPE_SCALAR:
      return PORT_TYPE_SCALAR;
    case CMP_TYPE_SCALAR_INT:
      return PORT_TYPE_SCALAR_INT;
    case CMP_TYPE_SCALAR_UINT:
      return PORT_TYPE_SCALAR_UINT;
    case CMP_TYPE_VECTOR_2D:
      return PORT_TYPE_VECTOR_2D;
    case CMP_TYPE_VECTOR_3D:
      return PORT_TYPE_VECTOR_3D;
    case CMP_TYPE_VECTOR_4D:
      return PORT_TYPE_VECTOR_4D;
    case CMP_TYPE_BOOLEAN:
      return PORT_TYPE_BOOLEAN;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeCompare::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "p1";
    case 1:
      return "p2";
    case 2:
      return "tolerance";
    default:
      std::cerr << "VisualShaderNodeCompare::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return "";
}

int VisualShaderNodeCompare::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeCompare::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_BOOLEAN;
}

std::string VisualShaderNodeCompare::get_output_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "result";
    default:
      std::cerr << "VisualShaderNodeCompare::get_output_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return "";
}

std::string VisualShaderNodeCompare::get_warning() const {
  switch (comparison_type) {
    case CMP_TYPE_BOOLEAN:
      if (func > FUNC_NOT_EQUAL) {
        return "Invalid comparison function for that type.";
      }
      break;
    default:
      break;
  }

  return "";
}

std::string VisualShaderNodeCompare::generate_code([[maybe_unused]] const int& id,
                                                   const std::vector<std::string>& input_vars,
                                                   const std::vector<std::string>& output_vars) const {
  std::string code;
  switch (comparison_type) {
    case CMP_TYPE_SCALAR: {
      switch (func) {
        case FUNC_EQUAL:
          code += std::string("\t") + output_vars.at(0) + " = (abs(" + input_vars.at(0) + " - " + input_vars.at(1) + ") < " +
                  input_vars.at(2) + ");" + std::string("\n");
          break;
        case FUNC_NOT_EQUAL:
          code += std::string("\t") + output_vars.at(0) + " = !(abs(" + input_vars.at(0) + " - " + input_vars.at(1) + ") < " +
                  input_vars.at(2) + ");" + std::string("\n");
          break;
        case FUNC_GREATER_THAN:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " > " + input_vars.at(1) + ";" + std::string("\n");
          break;
        case FUNC_GREATER_THAN_EQUAL:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " >= " + input_vars.at(1) + ";" + std::string("\n");
          break;
        case FUNC_LESS_THAN:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " < " + input_vars.at(1) + ";" + std::string("\n");
          break;
        case FUNC_LESS_THAN_EQUAL:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " <= " + input_vars.at(1) + ";" + std::string("\n");
          break;
        default:
          break;
      }
    } break;
    case CMP_TYPE_SCALAR_UINT:
    case CMP_TYPE_SCALAR_INT: {
      switch (func) {
        case FUNC_EQUAL:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " == " + input_vars.at(1) + ";" + std::string("\n");
          break;
        case FUNC_NOT_EQUAL:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " != " + input_vars.at(1) + ";" + std::string("\n");
          break;
        case FUNC_GREATER_THAN:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " > " + input_vars.at(1) + ";" + std::string("\n");
          break;
        case FUNC_GREATER_THAN_EQUAL:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " >= " + input_vars.at(1) + ";" + std::string("\n");
          break;
        case FUNC_LESS_THAN:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " < " + input_vars.at(1) + ";" + std::string("\n");
          break;
        case FUNC_LESS_THAN_EQUAL:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " <= " + input_vars.at(1) + ";" + std::string("\n");
          break;
        default:
          break;
      }
    } break;
    case CMP_TYPE_VECTOR_2D: {
      code += std::string("\t") + std::string("{") + std::string("\n");
      switch (func) {
        case FUNC_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec2 _bv = equal(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_NOT_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec2 _bv = notEqual(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_GREATER_THAN:
          code += std::string("\t") + std::string("\t") + "bvec2 _bv = greaterThan(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_GREATER_THAN_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec2 _bv = greaterThanEqual(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_LESS_THAN:
          code += std::string("\t") + std::string("\t") + "bvec2 _bv = lessThan(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_LESS_THAN_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec2 _bv = lessThanEqual(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        default:
          break;
      }

      switch (condition) {
        case COND_ALL:
          code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = all(_bv);" + std::string("\n");
          break;
        case COND_ANY:
          code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = any(_bv);" + std::string("\n");
          break;
        default:
          break;
      }

      code += std::string("\t") + std::string("}") + std::string("\n");
    } break;
    case CMP_TYPE_VECTOR_3D: {
      code += std::string("\t") + std::string("{") + std::string("\n");

      switch (func) {
        case FUNC_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec3 _bv = equal(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_NOT_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec3 _bv = notEqual(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_GREATER_THAN:
          code += std::string("\t") + std::string("\t") + "bvec3 _bv = greaterThan(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_GREATER_THAN_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec3 _bv = greaterThanEqual(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_LESS_THAN:
          code += std::string("\t") + std::string("\t") + "bvec3 _bv = lessThan(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_LESS_THAN_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec3 _bv = lessThanEqual(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        default:
          break;
      }

      switch (condition) {
        case COND_ALL:
          code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = all(_bv);" + std::string("\n");
          break;
        case COND_ANY:
          code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = any(_bv);" + std::string("\n");
          break;
        default:
          break;
      }

      code += std::string("\t") + std::string("}") + std::string("\n");
    } break;
    case CMP_TYPE_VECTOR_4D: {
      code += std::string("\t") + std::string("{") + std::string("\n");

      switch (func) {
        case FUNC_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec4 _bv = equal(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_NOT_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec4 _bv = notEqual(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_GREATER_THAN:
          code += std::string("\t") + std::string("\t") + "bvec4 _bv = greaterThan(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_GREATER_THAN_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec4 _bv = greaterThanEqual(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_LESS_THAN:
          code += std::string("\t") + std::string("\t") + "bvec4 _bv = lessThan(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        case FUNC_LESS_THAN_EQUAL:
          code += std::string("\t") + std::string("\t") + "bvec4 _bv = lessThanEqual(" + input_vars.at(0) + ", " + input_vars.at(1) + ");" + std::string("\n");
          break;
        default:
          break;
      }

      switch (condition) {
        case COND_ALL:
          code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = all(_bv);" + std::string("\n");
          break;
        case COND_ANY:
          code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = any(_bv);" + std::string("\n");
          break;
        default:
          break;
      }

      code += std::string("\t") + std::string("}") + std::string("\n");
    } break;
    case CMP_TYPE_BOOLEAN: {
      switch (func) {
        case FUNC_EQUAL:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " == " + input_vars.at(1) + ";" + std::string("\n");
          break;
        case FUNC_NOT_EQUAL:
          code += std::string("\t") + output_vars.at(0) + " = " + input_vars.at(0) + " != " + input_vars.at(1) + ";" + std::string("\n");
          break;
        case FUNC_GREATER_THAN:
        case FUNC_GREATER_THAN_EQUAL:
        case FUNC_LESS_THAN:
        case FUNC_LESS_THAN_EQUAL:
          code += std::string("\t") + output_vars.at(0) + " = false;" + std::string("\n");
          break;
        default:
          break;
      }
    } break;
    default:
      break;
  }
  return code;
}

void VisualShaderNodeCompare::set_comparison_type(const ComparisonType& type) {
  if (type < 0 || type >= CMP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeCompare::set_comparison_type: Invalid type: " << type << std::endl;
    return;
  }

  if (comparison_type == type) {
    return;
  }

  switch (type) {
    case CMP_TYPE_SCALAR:
      simple_decl = true;
      set_input_port_default_value(0, 0.0f, get_input_port_default_value(0));
      set_input_port_default_value(1, 0.0f, get_input_port_default_value(1));
      break;
    case CMP_TYPE_SCALAR_UINT:
    case CMP_TYPE_SCALAR_INT:
      simple_decl = true;
      set_input_port_default_value(0, 0, get_input_port_default_value(0));
      set_input_port_default_value(1, 0, get_input_port_default_value(1));
      break;
    case CMP_TYPE_VECTOR_2D:
      simple_decl = false;
      set_input_port_default_value(0, TVector2(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector2(), get_input_port_default_value(1));
      break;
    case CMP_TYPE_VECTOR_3D:
      simple_decl = false;
      set_input_port_default_value(0, TVector3(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector3(), get_input_port_default_value(1));
      break;
    case CMP_TYPE_VECTOR_4D:
      simple_decl = false;
      set_input_port_default_value(0, TVector4(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector4(), get_input_port_default_value(1));
      break;
    case CMP_TYPE_BOOLEAN:
      simple_decl = true;
      set_input_port_default_value(0, false);
      set_input_port_default_value(1, false);
      break;
    default:
      break;
  }

  comparison_type = type;
}

VisualShaderNodeCompare::ComparisonType VisualShaderNodeCompare::get_comparison_type() const { return comparison_type; }

void VisualShaderNodeCompare::set_function(const Function& func) {
  if (func < 0 || func >= FUNC_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeFloatFunc::set_function: Invalid func: " << func << std::endl;
    return;
  }

  if (this->func == func) {
    return;
  }

  this->func = func;
}

VisualShaderNodeCompare::Function VisualShaderNodeCompare::get_function() const { return func; }

void VisualShaderNodeCompare::set_condition(const Condition& cond) {
  if (cond < 0 || cond >= COND_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeCompare::set_condition: Invalid cond: " << cond << std::endl;
    return;
  }

  if (condition == cond) {
    return;
  }

  condition = cond;
}

VisualShaderNodeCompare::Condition VisualShaderNodeCompare::get_condition() const { return condition; }

std::vector<std::string> VisualShaderNodeCompare::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("type");
  props.emplace_back("function");

  switch (comparison_type) {
    case CMP_TYPE_VECTOR_2D:
    case CMP_TYPE_VECTOR_3D:
    case CMP_TYPE_VECTOR_4D:
      props.emplace_back("condition");
      break;
    default:
      break;
  }

  return props;
}

/*************************************/
/* Multiply Add Node                 */
/*************************************/

VisualShaderNodeMultiplyAdd::VisualShaderNodeMultiplyAdd() : op_type(OP_TYPE_SCALAR) {
  set_input_port_default_value(0, 0.0f);
  set_input_port_default_value(1, 1.0f);
  set_input_port_default_value(2, 0.0f);
}

std::string VisualShaderNodeMultiplyAdd::get_caption() const { return "MultiplyAdd"; }

int VisualShaderNodeMultiplyAdd::get_input_port_count() const { return 3; }

VisualShaderNode::PortType VisualShaderNodeMultiplyAdd::get_input_port_type([[maybe_unused]] const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
      return PORT_TYPE_VECTOR_2D;
    case OP_TYPE_VECTOR_3D:
      return PORT_TYPE_VECTOR_3D;
    case OP_TYPE_VECTOR_4D:
      return PORT_TYPE_VECTOR_4D;
    default:
      break;
  }
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeMultiplyAdd::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "p1";
    case 1:
      return "p2 (*)";
    case 2:
      return "p3 (+)";
    default:
      std::cerr << "VisualShaderNodeMultiplyAdd::get_input_port_name: Invalid port: " << port << std::endl;
      break;
  }

  return "";
}

int VisualShaderNodeMultiplyAdd::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeMultiplyAdd::get_output_port_type(const int& port) const {
  switch (op_type) {
    case OP_TYPE_VECTOR_2D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_2D;
        default:
          std::cerr << "VisualShaderNodeMultiplyAdd::get_output_port_type: "
                       "Invalid port: "
                    << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_3D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_3D;
        default:
          std::cerr << "VisualShaderNodeMultiplyAdd::get_output_port_type: "
                       "Invalid port: "
                    << port << std::endl;
          break;
      }
      break;
    case OP_TYPE_VECTOR_4D:
      switch (port) {
        case 0:
          return PORT_TYPE_VECTOR_4D;
        default:
          std::cerr << "VisualShaderNodeMultiplyAdd::get_output_port_type: "
                       "Invalid port: "
                    << port << std::endl;
          break;
      }
      break;
    default:
      break;
  }
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeMultiplyAdd::get_output_port_name([[maybe_unused]] const int& port) const { return ""; }

std::string VisualShaderNodeMultiplyAdd::generate_code([[maybe_unused]] const int& id,
                                                       const std::vector<std::string>& input_vars,
                                                       const std::vector<std::string>& output_vars) const {
  // TODO: Can use fma()?
  return std::string("\t") + output_vars.at(0) + " = (" + input_vars.at(0) + " * " + input_vars.at(1) + ") + " + input_vars.at(2) +
         ";" + std::string("\n");
}

void VisualShaderNodeMultiplyAdd::set_op_type(const OpType& op_type) {
  if (op_type < 0 || op_type >= OP_TYPE_ENUM_SIZE) {
    std::cerr << "VisualShaderNodeMultiplyAdd::set_op_type: Invalid op_type: " << op_type << std::endl;
    return;
  }

  if (this->op_type == op_type) {
    return;
  }

  switch (op_type) {
    case OP_TYPE_SCALAR: {
      set_input_port_default_value(0, 0.0f, get_input_port_default_value(0));
      set_input_port_default_value(1, 1.0f, get_input_port_default_value(1));
      set_input_port_default_value(2, 0.0f, get_input_port_default_value(2));
    } break;
    case OP_TYPE_VECTOR_2D: {
      set_input_port_default_value(0, TVector2(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector2(1.0f, 1.0f), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector2(), get_input_port_default_value(2));
    } break;
    case OP_TYPE_VECTOR_3D: {
      set_input_port_default_value(0, TVector3(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector3(1.0f, 1.0f, 1.0f), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector3(), get_input_port_default_value(2));
    } break;
    case OP_TYPE_VECTOR_4D: {
      set_input_port_default_value(0, TVector4(), get_input_port_default_value(0));
      set_input_port_default_value(1, TVector4(1.0f, 1.0f, 1.0f, 1.0f), get_input_port_default_value(1));
      set_input_port_default_value(2, TVector4(), get_input_port_default_value(2));
    } break;
    default:
      break;
  }

  this->op_type = op_type;
}

VisualShaderNodeMultiplyAdd::OpType VisualShaderNodeMultiplyAdd::get_op_type() const { return op_type; }

std::vector<std::string> VisualShaderNodeMultiplyAdd::get_editable_properties() const {
  std::vector<std::string> props;
  props.emplace_back("op_type");
  return props;
}

/*************************************/
/* DistanceFade Node                 */
/*************************************/

VisualShaderNodeDistanceFade::VisualShaderNodeDistanceFade() {
  set_input_port_default_value(0, 0.0f);
  set_input_port_default_value(1, 10.0f);
}

std::string VisualShaderNodeDistanceFade::get_caption() const { return "DistanceFade"; }

int VisualShaderNodeDistanceFade::get_input_port_count() const { return 2; }

VisualShaderNode::PortType VisualShaderNodeDistanceFade::get_input_port_type(const int& port) const {
  switch (port) {
    case 0:
      return PORT_TYPE_SCALAR;
    case 1:
      return PORT_TYPE_SCALAR;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeDistanceFade::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "min";
    case 1:
      return "max";
    default:
      break;
  }

  return "";
}

int VisualShaderNodeDistanceFade::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeDistanceFade::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeDistanceFade::get_output_port_name([[maybe_unused]] const int& port) const {
  return "amount";
}

bool VisualShaderNodeDistanceFade::has_output_port_preview([[maybe_unused]] const int& port) const { return false; }

std::string VisualShaderNodeDistanceFade::generate_code([[maybe_unused]] const int& id,
                                                        const std::vector<std::string>& input_vars,
                                                        const std::vector<std::string>& output_vars) const {
  std::string code;
  code += std::string("\t") + output_vars.at(0) + " = clamp(smoothstep(" + input_vars.at(0) + ", " + input_vars.at(1) +
          ", length(VERTEX)), 0.0, 1.0);" + std::string("\n");
  return code;
}

/*************************************/
/* Random Range Node                 */
/*************************************/

VisualShaderNodeRandomRange::VisualShaderNodeRandomRange() {
	set_input_port_default_value(0, TVector3(1.0f, 1.0f, 1.0f));
	set_input_port_default_value(1, 0.0f);
	set_input_port_default_value(2, 1.0f);
}

std::string VisualShaderNodeRandomRange::get_caption() const {
	return "RandomRange";
}

int VisualShaderNodeRandomRange::get_input_port_count() const {
	return 3;
}

VisualShaderNodeRandomRange::PortType VisualShaderNodeRandomRange::get_input_port_type(const int& port) const {
	switch (port) {
		case 0:
			return PORT_TYPE_VECTOR_3D;
		case 1:
			return PORT_TYPE_SCALAR;
		case 2:
			return PORT_TYPE_SCALAR;
		default:
			break;
	}

	return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeRandomRange::get_input_port_name(const int& port) const {
	switch (port) {
		case 0:
			return "seed";
		case 1:
			return "min";
		case 2:
			return "max";
		default:
			break;
	}

	return "";
}

int VisualShaderNodeRandomRange::get_output_port_count() const {
	return 1;
}

VisualShaderNodeRandomRange::PortType VisualShaderNodeRandomRange::get_output_port_type([[maybe_unused]] const int& port) const {
	return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeRandomRange::get_output_port_name([[maybe_unused]] const int& port) const {
	return "value";
}

std::string VisualShaderNodeRandomRange::generate_global_per_node([[maybe_unused]] const int& id) const {
	std::string code;

	code += "// 3D Noise with friendly permission by Inigo Quilez" + std::string("\n");
	code += "vec3 hash_noise_range( vec3 p ) {" + std::string("\n");
	code += std::string("\t") + "p *= mat3(vec3(127.1, 311.7, -53.7), vec3(269.5, 183.3, 77.1), vec3(-301.7, 27.3, 215.3));" + std::string("\n");
	code += std::string("\t") + "return 2.0 * fract(fract(p)*4375.55) -1.;" + std::string("\n");
	code += std::string("}") + "\n\n";

	return code;
}

std::string VisualShaderNodeRandomRange::generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars,
                                    const std::vector<std::string>& output_vars) const {
  return std::string("\t") + output_vars.at(0) + " = mix(" + input_vars.at(1) + ", " + input_vars.at(2) + ", hash_noise_range(" + input_vars.at(0) + ").x);" + std::string("\n");
}

/*************************************/
/* Remap Node                        */
/*************************************/

VisualShaderNodeRemap::VisualShaderNodeRemap() {
  simple_decl = false;
  set_input_port_default_value(1, 0.0f);
  set_input_port_default_value(2, 1.0f);
  set_input_port_default_value(3, 0.0f);
  set_input_port_default_value(4, 1.0f);
}

std::string VisualShaderNodeRemap::get_caption() const { return "Remap"; }

int VisualShaderNodeRemap::get_input_port_count() const { return 5; }

VisualShaderNode::PortType VisualShaderNodeRemap::get_input_port_type(const int& port) const {
  switch (port) {
    case 0:
      return PORT_TYPE_SCALAR;
    case 1:
      return PORT_TYPE_SCALAR;
    case 2:
      return PORT_TYPE_SCALAR;
    case 3:
      return PORT_TYPE_SCALAR;
    case 4:
      return PORT_TYPE_SCALAR;
    default:
      break;
  }

  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeRemap::get_input_port_name(const int& port) const {
  switch (port) {
    case 0:
      return "value";
    case 1:
      return "input min";
    case 2:
      return "input max";
    case 3:
      return "output min";
    case 4:
      return "output max";
    default:
      break;
  }

  return "";
}

int VisualShaderNodeRemap::get_output_port_count() const { return 1; }

VisualShaderNode::PortType VisualShaderNodeRemap::get_output_port_type([[maybe_unused]] const int& port) const {
  return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeRemap::get_output_port_name([[maybe_unused]] const int& port) const { return "value"; }

std::string VisualShaderNodeRemap::generate_code([[maybe_unused]] const int& id,
                                                 const std::vector<std::string>& input_vars,
                                                 const std::vector<std::string>& output_vars) const {
  std::string code;
  code += std::string("\t") + std::string("{") + std::string("\n");
  code += std::string("\t") + std::string("\t") + "float __input_range = " + input_vars.at(2) + " - " + input_vars.at(1) + ";" + std::string("\n");
  code += std::string("\t") + std::string("\t") + "float __output_range = " + input_vars.at(4) + " - " + input_vars.at(3) + ";" + std::string("\n");
  code += std::string("\t") + std::string("\t") + output_vars.at(0) + " = " + input_vars.at(3) + " + __output_range * ((" + input_vars.at(0) + " - " +
          input_vars.at(1) + ") / __input_range);" + std::string("\n");
  code += std::string("\t") + std::string("}") + std::string("\n");
  return code;
}
