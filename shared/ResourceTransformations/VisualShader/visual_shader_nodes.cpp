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

#include "visual_shader_nodes.h"

#include <iostream>

/*************************************/
/* Vector Base Node                  */
/*************************************/

VisualShaderNodeVectorBase::VisualShaderNodeVectorBase() : op_type(VisualShaderNodeVectorBase::OpType::OP_TYPE_VECTOR_3D) {}

VisualShaderNodeVectorBase::PortType VisualShaderNodeVectorBase::get_input_port_type([[maybe_unused]] const int& port) const {
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

VisualShaderNodeVectorBase::PortType VisualShaderNodeVectorBase::get_output_port_type(const int& port) const {
	switch (op_type) {
		case OP_TYPE_VECTOR_2D:
			return port == 0 || get_output_port_count() > 1 ? PORT_TYPE_VECTOR_2D : PORT_TYPE_SCALAR;
		case OP_TYPE_VECTOR_3D:
			return port == 0 || get_output_port_count() > 1 ? PORT_TYPE_VECTOR_3D : PORT_TYPE_SCALAR;
		case OP_TYPE_VECTOR_4D:
			return port == 0 || get_output_port_count() > 1 ? PORT_TYPE_VECTOR_4D : PORT_TYPE_SCALAR;
		default:
			break;
	}
	return PORT_TYPE_SCALAR;
}

void VisualShaderNodeVectorBase::set_op_type(const VisualShaderNodeVectorBase::OpType& op_type) {
    if (op_type < 0 || op_type >= OP_ENUM_SIZE) {
        std::cerr << "VisualShaderNodeVectorBase::set_op_type: Invalid op_type: " << op_type << std::endl;
        return;
    }

	if (this->op_type == op_type) {
		return;
	}

	this->op_type = op_type;
}

VisualShaderNodeVectorBase::OpType VisualShaderNodeVectorBase::get_op_type() const {
	return op_type;
}

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
/* OPERATORS                         */
/*************************************/

/*************************************/
/* Float Op Node                     */
/*************************************/

VisualShaderNodeFloatOp::VisualShaderNodeFloatOp() : op(VisualShaderNodeFloatOp::Operator::OP_ADD) {
	set_input_port_default_value(0, 0.0f);
	set_input_port_default_value(1, 0.0f);
}

std::string VisualShaderNodeFloatOp::get_caption() const {
	return "FloatOp";
}

int VisualShaderNodeFloatOp::get_input_port_count() const {
	return 2;
}

VisualShaderNodeFloatOp::PortType VisualShaderNodeFloatOp::get_input_port_type([[maybe_unused]] const int& port) const {
	return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeFloatOp::get_input_port_name(const int& port) const {
	return port == 0 ? "p1" : "p2";
}

int VisualShaderNodeFloatOp::get_output_port_count() const {
	return 1;
}

VisualShaderNodeFloatOp::PortType VisualShaderNodeFloatOp::get_output_port_type([[maybe_unused]] const int& port) const {
	return PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeFloatOp::get_output_port_name([[maybe_unused]] const int& port) const {
	return "op";
}

std::string VisualShaderNodeFloatOp::generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const {
	std::string code {"\t" + output_vars.at(0) + " = "};
	switch (op) {
		case VisualShaderNodeFloatOp::Operator::OP_ADD:
			code += input_vars.at(0) + " + " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeFloatOp::Operator::OP_SUB:
			code += input_vars.at(0) + " - " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeFloatOp::Operator::OP_MUL:
			code += input_vars.at(0) + " * " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeFloatOp::Operator::OP_DIV:
			code += input_vars.at(0) + " / " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeFloatOp::Operator::OP_MOD:
			code += "mod(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeFloatOp::Operator::OP_POW:
			code += "pow(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeFloatOp::Operator::OP_MAX:
			code += "max(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeFloatOp::Operator::OP_MIN:
			code += "min(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeFloatOp::Operator::OP_ATAN2:
			code += "atan(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeFloatOp::Operator::OP_STEP:
			code += "step(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
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

VisualShaderNodeFloatOp::Operator VisualShaderNodeFloatOp::get_operator() const {
	return op;
}

std::vector<std::string> VisualShaderNodeFloatOp::get_editable_properties() const {
	std::vector<std::string> props;
	props.push_back("operator");
	return props;
}

/*************************************/
/* Int Op Node                       */
/*************************************/

VisualShaderNodeIntOp::VisualShaderNodeIntOp() : op(VisualShaderNodeIntOp::Operator::OP_ADD) {
	set_input_port_default_value(0, 0);
	set_input_port_default_value(1, 0);
}

std::string VisualShaderNodeIntOp::get_caption() const {
	return "IntOp";
}

int VisualShaderNodeIntOp::get_input_port_count() const {
	return 2;
}

VisualShaderNodeIntOp::PortType VisualShaderNodeIntOp::get_input_port_type([[maybe_unused]] const int& port) const {
	return PORT_TYPE_SCALAR_INT;
}

std::string VisualShaderNodeIntOp::get_input_port_name(const int& port) const {
	return port == 0 ? "p1" : "p2";
}

int VisualShaderNodeIntOp::get_output_port_count() const {
	return 1;
}

VisualShaderNodeIntOp::PortType VisualShaderNodeIntOp::get_output_port_type([[maybe_unused]] const int& port) const {
	return PORT_TYPE_SCALAR_INT;
}

std::string VisualShaderNodeIntOp::get_output_port_name([[maybe_unused]] const int& port) const {
	return "op";
}

std::string VisualShaderNodeIntOp::generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const {
	std::string code {"\t" + output_vars.at(0) + " = "};
	switch (op) {
		case VisualShaderNodeIntOp::Operator::OP_ADD:
			code += input_vars.at(0) + " + " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeIntOp::Operator::OP_SUB:
			code += input_vars.at(0) + " - " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeIntOp::Operator::OP_MUL:
			code += input_vars.at(0) + " * " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeIntOp::Operator::OP_DIV:
			code += input_vars.at(0) + " / " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeIntOp::Operator::OP_MOD:
			code += input_vars.at(0) + " % " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeIntOp::Operator::OP_MAX:
			code += "max(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeIntOp::Operator::OP_MIN:
			code += "min(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeIntOp::Operator::OP_BITWISE_AND:
			code += input_vars.at(0) + " & " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeIntOp::Operator::OP_BITWISE_OR:
			code += input_vars.at(0) + " | " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeIntOp::Operator::OP_BITWISE_XOR:
			code += input_vars.at(0) + " ^ " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeIntOp::Operator::OP_BITWISE_LEFT_SHIFT:
			code += input_vars.at(0) + " << " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeIntOp::Operator::OP_BITWISE_RIGHT_SHIFT:
			code += input_vars.at(0) + " >> " + input_vars.at(1) + ";\n";
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

VisualShaderNodeIntOp::Operator VisualShaderNodeIntOp::get_operator() const {
	return op;
}

std::vector<std::string> VisualShaderNodeIntOp::get_editable_properties() const {
	std::vector<std::string> props;
	props.push_back("operator");
	return props;
}

/*************************************/
/* UInt Op Node                      */
/*************************************/

VisualShaderNodeUIntOp::VisualShaderNodeUIntOp() : op(VisualShaderNodeUIntOp::Operator::OP_ADD) {
	set_input_port_default_value(0, 0);
	set_input_port_default_value(1, 0);
}

std::string VisualShaderNodeUIntOp::get_caption() const {
	return "UIntOp";
}

int VisualShaderNodeUIntOp::get_input_port_count() const {
	return 2;
}

VisualShaderNodeUIntOp::PortType VisualShaderNodeUIntOp::get_input_port_type([[maybe_unused]] const int& port) const {
	return PORT_TYPE_SCALAR_UINT;
}

std::string VisualShaderNodeUIntOp::get_input_port_name(const int& port) const {
	return port == 0 ? "p1" : "p2";
}

int VisualShaderNodeUIntOp::get_output_port_count() const {
	return 1;
}

VisualShaderNodeUIntOp::PortType VisualShaderNodeUIntOp::get_output_port_type([[maybe_unused]] const int& port) const {
	return PORT_TYPE_SCALAR_UINT;
}

std::string VisualShaderNodeUIntOp::get_output_port_name([[maybe_unused]] const int& port) const {
	return "op";
}

std::string VisualShaderNodeUIntOp::generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const {
	std::string code {"\t" + output_vars.at(0) + " = "};
	switch (op) {
		case VisualShaderNodeUIntOp::Operator::OP_ADD:
			code += input_vars.at(0) + " + " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeUIntOp::Operator::OP_SUB:
			code += input_vars.at(0) + " - " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeUIntOp::Operator::OP_MUL:
			code += input_vars.at(0) + " * " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeUIntOp::Operator::OP_DIV:
			code += input_vars.at(0) + " / " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeUIntOp::Operator::OP_MOD:
			code += input_vars.at(0) + " % " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeUIntOp::Operator::OP_MAX:
			code += "max(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeUIntOp::Operator::OP_MIN:
			code += "min(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeUIntOp::Operator::OP_BITWISE_AND:
			code += input_vars.at(0) + " & " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeUIntOp::Operator::OP_BITWISE_OR:
			code += input_vars.at(0) + " | " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeUIntOp::Operator::OP_BITWISE_XOR:
			code += input_vars.at(0) + " ^ " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeUIntOp::Operator::OP_BITWISE_LEFT_SHIFT:
			code += input_vars.at(0) + " << " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeUIntOp::Operator::OP_BITWISE_RIGHT_SHIFT:
			code += input_vars.at(0) + " >> " + input_vars.at(1) + ";\n";
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

VisualShaderNodeUIntOp::Operator VisualShaderNodeUIntOp::get_operator() const {
	return op;
}

std::vector<std::string> VisualShaderNodeUIntOp::get_editable_properties() const {
	std::vector<std::string> props;
	props.push_back("operator");
	return props;
}

/*************************************/
/* Vector Op Node                    */
/*************************************/

VisualShaderNodeVectorOp::VisualShaderNodeVectorOp() : op(VisualShaderNodeVectorOp::Operator::OP_ADD) {
	switch (op_type) {
		case OP_TYPE_VECTOR_2D: {
			set_input_port_default_value(0, TEVector2());
			set_input_port_default_value(1, TEVector2());
		} break;
		case OP_TYPE_VECTOR_3D: {
			set_input_port_default_value(0, TEVector3());
			set_input_port_default_value(1, TEVector3());
		} break;
		case OP_TYPE_VECTOR_4D: {
			set_input_port_default_value(0, TEQuaternion());
			set_input_port_default_value(1, TEQuaternion());
		} break;
		default:
			break;
	}
}

std::string VisualShaderNodeVectorOp::get_caption() const {
	return "VectorOp";
}

int VisualShaderNodeVectorOp::get_input_port_count() const {
	return 2;
}

std::string VisualShaderNodeVectorOp::get_input_port_name(const int& port) const {
	return port == 0 ? "p1" : "p2";
}

int VisualShaderNodeVectorOp::get_output_port_count() const {
	return 1;
}

std::string VisualShaderNodeVectorOp::get_output_port_name([[maybe_unused]] const int& port) const {
	return "op";
}

std::string VisualShaderNodeVectorOp::generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const {
	std::string code {"\t" + output_vars.at(0) + " = "};
	switch (op) {
		case VisualShaderNodeVectorOp::Operator::OP_ADD:
			code += input_vars.at(0) + " + " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeVectorOp::Operator::OP_SUB:
			code += input_vars.at(0) + " - " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeVectorOp::Operator::OP_MUL:
			code += input_vars.at(0) + " * " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeVectorOp::Operator::OP_DIV:
			code += input_vars.at(0) + " / " + input_vars.at(1) + ";\n";
			break;
		case VisualShaderNodeVectorOp::Operator::OP_MOD:
			code += "mod(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeVectorOp::Operator::OP_POW:
			code += "pow(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeVectorOp::Operator::OP_MAX:
			code += "max(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeVectorOp::Operator::OP_MIN:
			code += "min(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeVectorOp::Operator::OP_CROSS:
            switch (op_type) {
                case OP_TYPE_VECTOR_2D: // Not supported.
                    code += "vec2(0.0);\n";
                    break;
                case OP_TYPE_VECTOR_3D:
                    code += "cross(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
                    break;
                case OP_TYPE_VECTOR_4D: // Not supported.
                    code += "vec4(0.0);\n";
                    break;
                default:
                    break;
            }
			break;
		case VisualShaderNodeVectorOp::Operator::OP_ATAN2:
			code += "atan(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeVectorOp::Operator::OP_REFLECT:
			code += "reflect(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		case VisualShaderNodeVectorOp::Operator::OP_STEP:
			code += "step(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
			break;
		default:
			break;
	}

	return code;
}

void VisualShaderNodeVectorOp::set_op_type(const VisualShaderNodeVectorBase::OpType& op_type) {
    if (op_type < 0 || op_type >= VisualShaderNodeVectorBase::OpType::OP_ENUM_SIZE) {
        std::cerr << "VisualShaderNodeVectorOp::set_op_type: Invalid op_type: " << op_type << std::endl;
        return;
    }

	if (this->op_type == op_type) {
		return;
	}

	switch (op_type) {
		case OP_TYPE_VECTOR_2D: {
			set_input_port_default_value(0, TEVector2(), get_input_port_default_value(0));
			set_input_port_default_value(1, TEVector2(), get_input_port_default_value(1));
		} break;
		case OP_TYPE_VECTOR_3D: {
			set_input_port_default_value(0, TEVector3(), get_input_port_default_value(0));
			set_input_port_default_value(1, TEVector3(), get_input_port_default_value(1));
		} break;
		case OP_TYPE_VECTOR_4D: {
			set_input_port_default_value(0, TEQuaternion(), get_input_port_default_value(0));
			set_input_port_default_value(1, TEQuaternion(), get_input_port_default_value(1));
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

VisualShaderNodeVectorOp::Operator VisualShaderNodeVectorOp::get_operator() const {
	return op;
}

std::vector<std::string> VisualShaderNodeVectorOp::get_editable_properties() const {
	std::vector<std::string> props {VisualShaderNodeVectorBase::get_editable_properties()};
	props.push_back("operator");
	return props;
}

std::string VisualShaderNodeVectorOp::get_warning() const {
	bool invalid_type {false};

	if (op_type == OP_TYPE_VECTOR_2D || op_type == OP_TYPE_VECTOR_4D) {
		if (op == OP_CROSS) {
			invalid_type = true;
		}
	}

	if (invalid_type) {
		return "VisualShaderNodeVectorOp::get_warning: Invalid type for operator: " + std::to_string(op);
	}

	return std::string();
}

/*************************************/
/* Color Op Node                     */
/*************************************/

VisualShaderNodeColorOp::VisualShaderNodeColorOp() : op(VisualShaderNodeColorOp::Operator::OP_SCREEN) {
	set_input_port_default_value(0, TEVector3());
	set_input_port_default_value(1, TEVector3());
}

std::string VisualShaderNodeColorOp::get_caption() const {
	return "ColorOp";
}

int VisualShaderNodeColorOp::get_input_port_count() const {
	return 2;
}

VisualShaderNodeColorOp::PortType VisualShaderNodeColorOp::get_input_port_type([[maybe_unused]] const int& port) const {
	return PORT_TYPE_VECTOR_3D;
}

std::string VisualShaderNodeColorOp::get_input_port_name(const int& port) const {
	return port == 0 ? "p1" : "p2";
}

int VisualShaderNodeColorOp::get_output_port_count() const {
	return 1;
}

VisualShaderNodeColorOp::PortType VisualShaderNodeColorOp::get_output_port_type(const int& port) const {
	return port == 0 ? PORT_TYPE_VECTOR_3D : PORT_TYPE_SCALAR;
}

std::string VisualShaderNodeColorOp::get_output_port_name([[maybe_unused]] const int& port) const {
	return "op";
}

std::string VisualShaderNodeColorOp::generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const {
	std::string code;
	static const char* axisn[3] = { "x", "y", "z" };
	switch (op) {
		case VisualShaderNodeColorOp::Operator::OP_SCREEN: {
			code += "\t" + output_vars.at(0) + " = vec3(1.0) - (vec3(1.0) - " + input_vars.at(0) + ") * (vec3(1.0) - " + input_vars.at(1) + ");\n";
		} break;
		case VisualShaderNodeColorOp::Operator::OP_DIFFERENCE: {
			code += "\t" + output_vars.at(0) + " = abs(" + input_vars.at(0) + " - " + input_vars.at(1) + ");\n";
		} break;
		case VisualShaderNodeColorOp::Operator::OP_DARKEN: {
			code += "\t" + output_vars.at(0) + " = min(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";
		} break;
		case VisualShaderNodeColorOp::Operator::OP_LIGHTEN: {
			code += "\t" + output_vars.at(0) + " = max(" + input_vars.at(0) + ", " + input_vars.at(1) + ");\n";

		} break;
		case VisualShaderNodeColorOp::Operator::OP_OVERLAY: {
			for (int i = 0; i < 3; i++) {
				code += "\t{\n";
				code += "\t\tfloat base = " + input_vars.at(0) + "." + axisn[i] + ";\n";
				code += "\t\tfloat blend = " + input_vars.at(1) + "." + axisn[i] + ";\n";
				code += "\t\tif (base < 0.5) {\n";
				code += "\t\t\t" + output_vars.at(0) + "." + axisn[i] + " = 2.0 * base * blend;\n";
				code += "\t\t} else {\n";
				code += "\t\t\t" + output_vars.at(0) + "." + axisn[i] + " = 1.0 - 2.0 * (1.0 - blend) * (1.0 - base);\n";
				code += "\t\t}\n";
				code += "\t}\n";
			}

		} break;
		case VisualShaderNodeColorOp::Operator::OP_DODGE: {
			code += "\t" + output_vars.at(0) + " = (" + input_vars.at(0) + ") / (vec3(1.0) - " + input_vars.at(1) + ");\n";

		} break;
		case VisualShaderNodeColorOp::Operator::OP_BURN: {
			code += "\t" + output_vars.at(0) + " = vec3(1.0) - (vec3(1.0) - " + input_vars.at(0) + ") / (" + input_vars.at(1) + ");\n";
		} break;
		case VisualShaderNodeColorOp::Operator::OP_SOFT_LIGHT: {
			for (int i = 0; i < 3; i++) {
				code += "\t{\n";
				code += "\t\tfloat base = " + input_vars.at(0) + "." + axisn[i] + ";\n";
				code += "\t\tfloat blend = " + input_vars.at(1) + "." + axisn[i] + ";\n";
				code += "\t\tif (base < 0.5) {\n";
				code += "\t\t\t" + output_vars.at(0) + "." + axisn[i] + " = (base * (blend + 0.5));\n";
				code += "\t\t} else {\n";
				code += "\t\t\t" + output_vars.at(0) + "." + axisn[i] + " = (1.0 - (1.0 - base) * (1.0 - (blend - 0.5)));\n";
				code += "\t\t}\n";
				code += "\t}\n";
			}

		} break;
		case VisualShaderNodeColorOp::Operator::OP_HARD_LIGHT: {
			for (int i = 0; i < 3; i++) {
				code += "\t{\n";
				code += "\t\tfloat base = " + input_vars.at(0) + "." + axisn[i] + ";\n";
				code += "\t\tfloat blend = " + input_vars.at(1) + "." + axisn[i] + ";\n";
				code += "\t\tif (base < 0.5) {\n";
				code += "\t\t\t" + output_vars.at(0) + "." + axisn[i] + " = (base * (2.0 * blend));\n";
				code += "\t\t} else {\n";
				code += "\t\t\t" + output_vars.at(0) + "." + axisn[i] + " = (1.0 - (1.0 - base) * (1.0 - 2.0 * (blend - 0.5)));\n";
				code += "\t\t}\n";
				code += "\t}\n";
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

VisualShaderNodeColorOp::Operator VisualShaderNodeColorOp::get_operator() const {
	return op;
}

std::vector<std::string> VisualShaderNodeColorOp::get_editable_properties() const {
	std::vector<std::string> props;
	props.push_back("operator");
	return props;
}
