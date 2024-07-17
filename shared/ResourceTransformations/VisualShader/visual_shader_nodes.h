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

#ifndef ENIGMA_VISUAL_SHADER_NODES_H
#define ENIGMA_VISUAL_SHADER_NODES_H

#include "visual_shader.h"

/*************************************/
/* Vector Base Node                  */
/*************************************/

class VisualShaderNodeVectorBase : public VisualShaderNode {
	public:
		enum OpType {
			OP_TYPE_VECTOR_2D,
			OP_TYPE_VECTOR_3D,
			OP_TYPE_VECTOR_4D,
			OP_ENUM_SIZE,
		};

		VisualShaderNodeVectorBase();

		virtual std::string get_caption() const override = 0;

		virtual int get_input_port_count() const override = 0;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override = 0;

		virtual int get_output_port_count() const override = 0;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override = 0;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override = 0;

		virtual void set_op_type(const VisualShaderNodeVectorBase::OpType& op_type);
		VisualShaderNodeVectorBase::OpType get_op_type() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_VECTOR; }

	protected:
		VisualShaderNodeVectorBase::OpType op_type;
};

/*************************************/
/* CONSTANTS                         */
/*************************************/

class VisualShaderNodeConstant : public VisualShaderNode {
public:
    VisualShaderNodeConstant();

	virtual std::string get_caption() const override = 0;

	virtual int get_input_port_count() const override = 0;
	virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override = 0;
	virtual std::string get_input_port_name(const int& port) const override = 0;

	virtual int get_output_port_count() const override = 0;
	virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override = 0;
	virtual std::string get_output_port_name(const int& port) const override = 0;

	virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override = 0;

	inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_INPUT; }
};

/*************************************/
/* Float Constant                    */
/*************************************/

class VisualShaderNodeFloatConstant : public VisualShaderNodeConstant {
	public:
		VisualShaderNodeFloatConstant();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name([[maybe_unused]] const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, [[maybe_unused]] const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_constant(const float& c);
		float get_constant() const;

		virtual std::vector<std::string> get_editable_properties() const override;

	private:
		float constant;
};

/*************************************/
/* Int Constant                      */
/*************************************/

class VisualShaderNodeIntConstant : public VisualShaderNodeConstant {
	public:
		VisualShaderNodeIntConstant();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name([[maybe_unused]] const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, [[maybe_unused]] const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_constant(const int& c);
		int get_constant() const;

		virtual std::vector<std::string> get_editable_properties() const override;

	private:
		int constant;

};

/*************************************/
/* UInt Constant                     */
/*************************************/

class VisualShaderNodeUIntConstant : public VisualShaderNodeConstant {
	public:
		VisualShaderNodeUIntConstant();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name([[maybe_unused]] const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, [[maybe_unused]] const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_constant(const int& c);
		int get_constant() const;

		virtual std::vector<std::string> get_editable_properties() const override;

	private:
		int constant;

};

/*************************************/
/* Boolean Constant                  */
/*************************************/

class VisualShaderNodeBooleanConstant : public VisualShaderNodeConstant {
	public:
		VisualShaderNodeBooleanConstant();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name([[maybe_unused]] const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, [[maybe_unused]] const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_constant(const bool& c);
		bool get_constant() const;

		virtual std::vector<std::string> get_editable_properties() const override;

	private:
		bool constant;

};

/*************************************/
/* Color Constant                    */
/*************************************/

class VisualShaderNodeColorConstant : public VisualShaderNodeConstant {
	public:
		VisualShaderNodeColorConstant();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name([[maybe_unused]] const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, [[maybe_unused]] const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_constant(const TEColor& c);
		TEColor get_constant() const;

		virtual std::vector<std::string> get_editable_properties() const override;

	private:
		TEColor constant;
};

/*************************************/
/* Vector2 Constant                  */
/*************************************/

class VisualShaderNodeVec2Constant : public VisualShaderNodeConstant {
	public:
		VisualShaderNodeVec2Constant();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name([[maybe_unused]] const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, [[maybe_unused]] const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_constant(const TEVector2& c);
		TEVector2 get_constant() const;

		virtual std::vector<std::string> get_editable_properties() const override;

	private:
		TEVector2 constant;	
};

/*************************************/
/* Vector3 Constant                  */
/*************************************/

class VisualShaderNodeVec3Constant : public VisualShaderNodeConstant {
	public:
		VisualShaderNodeVec3Constant();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name([[maybe_unused]] const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, [[maybe_unused]] const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_constant(const TEVector3& c);
		TEVector3 get_constant() const;

		virtual std::vector<std::string> get_editable_properties() const override;

	private:
		TEVector3 constant;
};

/*************************************/
/* Vector4 Constant                  */
/*************************************/

class VisualShaderNodeVec4Constant : public VisualShaderNodeConstant {
	public:
		VisualShaderNodeVec4Constant();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name([[maybe_unused]] const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, [[maybe_unused]] const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_constant(const TEQuaternion& c);
		TEQuaternion get_constant() const;

		virtual std::vector<std::string> get_editable_properties() const override;

	private:
		TEQuaternion constant;
};

/*************************************/
/* OPERATORS                         */
/*************************************/

/*************************************/
/* Float Op Node                     */
/*************************************/

class VisualShaderNodeFloatOp : public VisualShaderNode {
	public:
		enum Operator {
			OP_ADD,
			OP_SUB,
			OP_MUL,
			OP_DIV,
			OP_MOD,
			OP_POW,
			OP_MAX,
			OP_MIN,
			OP_ATAN2,
			OP_STEP,
			OP_ENUM_SIZE,
		};

		VisualShaderNodeFloatOp();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_operator(const VisualShaderNodeFloatOp::Operator& op);
		VisualShaderNodeFloatOp::Operator get_operator() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_SCALAR; }

	private:
		VisualShaderNodeFloatOp::Operator op;
};

/*************************************/
/* Int Op Node                       */
/*************************************/

class VisualShaderNodeIntOp : public VisualShaderNode {
	public:
		enum Operator {
			OP_ADD,
			OP_SUB,
			OP_MUL,
			OP_DIV,
			OP_MOD,
			OP_MAX,
			OP_MIN,
			OP_BITWISE_AND,
			OP_BITWISE_OR,
			OP_BITWISE_XOR,
			OP_BITWISE_LEFT_SHIFT,
			OP_BITWISE_RIGHT_SHIFT,
			OP_ENUM_SIZE,
		};

		VisualShaderNodeIntOp();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_operator(const VisualShaderNodeIntOp::Operator& op);
		VisualShaderNodeIntOp::Operator get_operator() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_SCALAR; }

	private:
		VisualShaderNodeIntOp::Operator op;
};

/*************************************/
/* UInt Op Node                      */
/*************************************/

class VisualShaderNodeUIntOp : public VisualShaderNode {
	public:
		enum Operator {
			OP_ADD,
			OP_SUB,
			OP_MUL,
			OP_DIV,
			OP_MOD,
			OP_MAX,
			OP_MIN,
			OP_BITWISE_AND,
			OP_BITWISE_OR,
			OP_BITWISE_XOR,
			OP_BITWISE_LEFT_SHIFT,
			OP_BITWISE_RIGHT_SHIFT,
			OP_ENUM_SIZE,
		};

		VisualShaderNodeUIntOp();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_operator(const VisualShaderNodeUIntOp::Operator& op);
		VisualShaderNodeUIntOp::Operator get_operator() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_SCALAR; }

	private:
		VisualShaderNodeUIntOp::Operator op;
};

/*************************************/
/* Vector Op Node                    */
/*************************************/

class VisualShaderNodeVectorOp : public VisualShaderNodeVectorBase {
	public:
		enum Operator {
			OP_ADD,
			OP_SUB,
			OP_MUL,
			OP_DIV,
			OP_MOD,
			OP_POW,
			OP_MAX,
			OP_MIN,
			OP_CROSS,
			OP_ATAN2,
			OP_REFLECT,
			OP_STEP,
			OP_ENUM_SIZE,
		};

		VisualShaderNodeVectorOp();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		virtual void set_op_type(const VisualShaderNodeVectorBase::OpType& op_type) override;

		void set_operator(const VisualShaderNodeVectorOp::Operator& op);
		VisualShaderNodeVectorOp::Operator get_operator() const;

		virtual std::vector<std::string> get_editable_properties() const override;
		
		std::string get_warning() const override;

	private:
		VisualShaderNodeVectorOp::Operator op;
};

/*************************************/
/* Color Op Node                     */
/*************************************/

class VisualShaderNodeColorOp : public VisualShaderNode {
	public:
		enum Operator {
			OP_SCREEN,
			OP_DIFFERENCE,
			OP_DARKEN,
			OP_LIGHTEN,
			OP_OVERLAY,
			OP_DODGE,
			OP_BURN,
			OP_SOFT_LIGHT,
			OP_HARD_LIGHT,
			OP_ENUM_SIZE,
		};

		VisualShaderNodeColorOp();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type([[maybe_unused]] const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name([[maybe_unused]] const int& port) const override;

		virtual std::string generate_code([[maybe_unused]] const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_operator(const VisualShaderNodeColorOp::Operator& op);
		VisualShaderNodeColorOp::Operator get_operator() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_COLOR; }

	private:
		VisualShaderNodeColorOp::Operator op;
};

#endif //ENIGMA_VISUAL_SHADER_NODES_H
