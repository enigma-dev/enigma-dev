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
			OP_TYPE_ENUM_SIZE,
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

/*************************************/
/* Float Funcs Node                  */
/*************************************/

class VisualShaderNodeFloatFunc : public VisualShaderNode {
	public:
		enum Function {
			FUNC_SIN,
			FUNC_COS,
			FUNC_TAN,
			FUNC_ASIN,
			FUNC_ACOS,
			FUNC_ATAN,
			FUNC_SINH,
			FUNC_COSH,
			FUNC_TANH,
			FUNC_LOG,
			FUNC_EXP,
			FUNC_SQRT,
			FUNC_ABS,
			FUNC_SIGN,
			FUNC_FLOOR,
			FUNC_ROUND,
			FUNC_CEIL,
			FUNC_FRACT,
			FUNC_SATURATE,
			FUNC_NEGATE,
			FUNC_ACOSH,
			FUNC_ASINH,
			FUNC_ATANH,
			FUNC_DEGREES,
			FUNC_EXP2,
			FUNC_INVERSE_SQRT,
			FUNC_LOG2,
			FUNC_RADIANS,
			FUNC_RECIPROCAL,
			FUNC_ROUNDEVEN,
			FUNC_TRUNC,
			FUNC_ONEMINUS,
			FUNC_ENUM_SIZE,
		};

		VisualShaderNodeFloatFunc();
		
		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_function(const VisualShaderNodeFloatFunc::Function& func);
		VisualShaderNodeFloatFunc::Function get_function() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_SCALAR; }

	protected:
		VisualShaderNodeFloatFunc::Function func;
};

/*************************************/
/* Int Funcs Node                    */
/*************************************/

class VisualShaderNodeIntFunc : public VisualShaderNode {
	public:
		enum Function {
			FUNC_ABS,
			FUNC_NEGATE,
			FUNC_SIGN,
			FUNC_BITWISE_NOT,
			FUNC_ENUM_SIZE,
		};

		VisualShaderNodeIntFunc();
		
		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_function(const VisualShaderNodeIntFunc::Function& func);
		VisualShaderNodeIntFunc::Function get_function() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_SCALAR; }

	protected:
		VisualShaderNodeIntFunc::Function func;
};

/*************************************/
/* UInt Funcs Node                   */
/*************************************/

class VisualShaderNodeUIntFunc : public VisualShaderNode {
	public:
		enum Function {
			FUNC_NEGATE,
			FUNC_BITWISE_NOT,
			FUNC_ENUM_SIZE,
		};


		VisualShaderNodeUIntFunc();
		
		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_function(const VisualShaderNodeUIntFunc::Function& func);
		VisualShaderNodeUIntFunc::Function get_function() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_SCALAR; }

	protected:
		VisualShaderNodeUIntFunc::Function func = FUNC_NEGATE;
};

/*************************************/
/* Vector Funcs Node                 */
/*************************************/

class VisualShaderNodeVectorFunc : public VisualShaderNodeVectorBase {
	public:
		enum Function {
			FUNC_NORMALIZE,
			FUNC_SATURATE,
			FUNC_NEGATE,
			FUNC_RECIPROCAL,
			FUNC_ABS,
			FUNC_ACOS,
			FUNC_ACOSH,
			FUNC_ASIN,
			FUNC_ASINH,
			FUNC_ATAN,
			FUNC_ATANH,
			FUNC_CEIL,
			FUNC_COS,
			FUNC_COSH,
			FUNC_DEGREES,
			FUNC_EXP,
			FUNC_EXP2,
			FUNC_FLOOR,
			FUNC_FRACT,
			FUNC_INVERSE_SQRT,
			FUNC_LOG,
			FUNC_LOG2,
			FUNC_RADIANS,
			FUNC_ROUND,
			FUNC_ROUNDEVEN,
			FUNC_SIGN,
			FUNC_SIN,
			FUNC_SINH,
			FUNC_SQRT,
			FUNC_TAN,
			FUNC_TANH,
			FUNC_TRUNC,
			FUNC_ONEMINUS,
			FUNC_ENUM_SIZE,
		};

		VisualShaderNodeVectorFunc();
		
		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		virtual void set_op_type(const OpType& op_type) override;

		void set_function(const VisualShaderNodeVectorFunc::Function& func);
		VisualShaderNodeVectorFunc::Function get_function() const;

		virtual std::vector<std::string> get_editable_properties() const override;

	protected:
		VisualShaderNodeVectorFunc::Function func;
};

/*************************************/
/* Color Funcs Node                  */
/*************************************/

class VisualShaderNodeColorFunc : public VisualShaderNode {
	public:
		enum Function {
			FUNC_GRAYSCALE,
			FUNC_HSV2RGB,
			FUNC_RGB2HSV,
			FUNC_SEPIA,
			FUNC_ENUM_SIZE,
		};

		VisualShaderNodeColorFunc();
		
		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_function(const VisualShaderNodeColorFunc::Function& func);
		VisualShaderNodeColorFunc::Function get_function() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_COLOR; }

	protected:
		VisualShaderNodeColorFunc::Function func;
};

/*************************************/
/* UV Funcs Node                     */
/*************************************/

class VisualShaderNodeUVFunc : public VisualShaderNode {
	public:
		enum Function {
			FUNC_PANNING,
			FUNC_SCALING,
			FUNC_ENUM_SIZE,
		};

		VisualShaderNodeUVFunc();
		
		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;
		virtual bool is_input_port_default(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual bool is_show_prop_names() const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_function(const VisualShaderNodeUVFunc::Function& func);
		VisualShaderNodeUVFunc::Function get_function() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_TEXTURES; }

	protected:
		VisualShaderNodeUVFunc::Function func;
};

/*************************************/
/* UV PolarCoord Funcs Node          */
/*************************************/

class VisualShaderNodeUVPolarCoord : public VisualShaderNode {
	public:
		VisualShaderNodeUVPolarCoord();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;
		virtual bool is_input_port_default(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_TEXTURES; }

};

/*************************************/
/* Dot Node                          */
/*************************************/

class VisualShaderNodeDotProduct : public VisualShaderNode {
	public:
		VisualShaderNodeDotProduct();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_VECTOR; }

};

/*************************************/
/* Length Node                       */
/*************************************/

class VisualShaderNodeVectorLen : public VisualShaderNodeVectorBase {
	public:
		VisualShaderNodeVectorLen();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual void set_op_type(const OpType& op_type) override;
		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

};

/*************************************/
/* Clamp Node                        */
/*************************************/

class VisualShaderNodeClamp : public VisualShaderNode {
	public:
		enum OpType {
			OP_TYPE_FLOAT,
			OP_TYPE_INT,
			OP_TYPE_UINT,
			OP_TYPE_VECTOR_2D,
			OP_TYPE_VECTOR_3D,
			OP_TYPE_VECTOR_4D,
			OP_TYPE_ENUM_SIZE,
		};

		VisualShaderNodeClamp();
		
		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		void set_op_type(const OpType& op_type);
		OpType get_op_type() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		inline virtual VisualShaderNode::Category get_category() const override {
			switch (op_type) {
				case OP_TYPE_FLOAT:
				case OP_TYPE_INT:
				case OP_TYPE_UINT:
					return VisualShaderNode::Category::CATEGORY_SCALAR;
				default:
					break;
			}
			return VisualShaderNode::Category::CATEGORY_VECTOR;
		}

	protected:
		OpType op_type;
};

/*************************************/
/* Derivative Func Node              */
/*************************************/

class VisualShaderNodeDerivativeFunc : public VisualShaderNode {
	public:
		enum OpType {
			OP_TYPE_SCALAR,
			OP_TYPE_VECTOR_2D,
			OP_TYPE_VECTOR_3D,
			OP_TYPE_VECTOR_4D,
			OP_TYPE_ENUM_SIZE,
		};

		enum Function {
			FUNC_SUM,
			FUNC_X,
			FUNC_Y,
			FUNC_ENUM_SIZE,
		};

		enum Precision {
			PRECISION_NONE,
			PRECISION_COARSE,
			PRECISION_FINE,
			PRECISION_ENUM_SIZE,
		};

		VisualShaderNodeDerivativeFunc();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_op_type(const OpType& op_type);
		OpType get_op_type() const;

		void set_function(const Function& func);
		Function get_function() const;

		void set_precision(const Precision& precision);
		Precision get_precision() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_UTILITY; }

	protected:
		OpType op_type;
		Function func;
		Precision precision;
};

/*************************************/
/* Face Forward Node                 */
/*************************************/

class VisualShaderNodeFaceForward : public VisualShaderNodeVectorBase {
	public:
		VisualShaderNodeFaceForward();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual void set_op_type(const OpType& op_type) override;
		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

};

/*************************************/
/* Step Node                         */
/*************************************/

// TODO: Add a custom node to say "for values 0-0.5, use this filter; for values 0.5-1, use this one".

class VisualShaderNodeStep : public VisualShaderNode {
	public:
		enum OpType {
			OP_TYPE_SCALAR,
			OP_TYPE_VECTOR_2D,
			OP_TYPE_VECTOR_2D_SCALAR,
			OP_TYPE_VECTOR_3D,
			OP_TYPE_VECTOR_3D_SCALAR,
			OP_TYPE_VECTOR_4D,
			OP_TYPE_VECTOR_4D_SCALAR,
			OP_TYPE_ENUM_SIZE,
		};

		VisualShaderNodeStep();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;
		virtual int get_default_input_port(const PortType& type) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		void set_op_type(const OpType& op_type);
		OpType get_op_type() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		inline virtual VisualShaderNode::Category get_category() const override {
			switch (op_type) {
				case OP_TYPE_SCALAR:
					return VisualShaderNode::Category::CATEGORY_SCALAR;
				default:
					break;
			}

			return VisualShaderNode::Category::CATEGORY_VECTOR;
		}

	protected:
		OpType op_type;
};

/*************************************/
/* Smooth Step Node                  */
/*************************************/

class VisualShaderNodeSmoothStep : public VisualShaderNode {
	public:
		enum OpType {
			OP_TYPE_SCALAR,
			OP_TYPE_VECTOR_2D,
			OP_TYPE_VECTOR_2D_SCALAR,
			OP_TYPE_VECTOR_3D,
			OP_TYPE_VECTOR_3D_SCALAR,
			OP_TYPE_VECTOR_4D,
			OP_TYPE_VECTOR_4D_SCALAR,
			OP_TYPE_ENUM_SIZE,
		};

		VisualShaderNodeSmoothStep();
		
		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;
		virtual int get_default_input_port(const PortType& type) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		void set_op_type(const OpType& op_type);
		OpType get_op_type() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		inline virtual VisualShaderNode::Category get_category() const override {
			switch (op_type) {
				case OP_TYPE_SCALAR:
					return VisualShaderNode::Category::CATEGORY_SCALAR;
				default:
					break;
			}

			return VisualShaderNode::Category::CATEGORY_VECTOR;
		}

	protected:
		OpType op_type;
};

/*************************************/
/* Distance Node                     */
/*************************************/

class VisualShaderNodeVectorDistance : public VisualShaderNodeVectorBase {
	public:
		VisualShaderNodeVectorDistance();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual void set_op_type(const OpType& op_type) override;
		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

};

/*************************************/
/* Refract Node                      */
/*************************************/

class VisualShaderNodeVectorRefract : public VisualShaderNodeVectorBase {
	public:
		VisualShaderNodeVectorRefract();
		
		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;
		virtual void set_op_type(const OpType& op_type) override;

};

/*************************************/
/* Mix Node                          */
/*************************************/

class VisualShaderNodeMix : public VisualShaderNode {
	public:
		enum OpType {
			OP_TYPE_SCALAR,
			OP_TYPE_VECTOR_2D,
			OP_TYPE_VECTOR_2D_SCALAR,
			OP_TYPE_VECTOR_3D,
			OP_TYPE_VECTOR_3D_SCALAR,
			OP_TYPE_VECTOR_4D,
			OP_TYPE_VECTOR_4D_SCALAR,
			OP_TYPE_ENUM_SIZE,
		};

		VisualShaderNodeMix();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		void set_op_type(const OpType& op_type);
		OpType get_op_type() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		inline virtual VisualShaderNode::Category get_category() const override {
			switch (op_type) {
				case OP_TYPE_SCALAR:
					return VisualShaderNode::Category::CATEGORY_SCALAR;
				default:
					break;
			}

			return VisualShaderNode::Category::CATEGORY_VECTOR;
		}

	protected:
		OpType op_type;
};

/*************************************/
/* Vector Compose Node               */
/*************************************/

class VisualShaderNodeVectorCompose : public VisualShaderNodeVectorBase {
	public:
		VisualShaderNodeVectorCompose();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual void set_op_type(const OpType& op_type) override;
		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

};

/*************************************/
/* Vector Decompose Node             */
/*************************************/

class VisualShaderNodeVectorDecompose : public VisualShaderNodeVectorBase {
	public:
		VisualShaderNodeVectorDecompose();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual void set_op_type(const OpType& op_type) override;
		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

};

/*************************************/
/* If Node                           */
/*************************************/

class VisualShaderNodeIf : public VisualShaderNode {
	public:
		VisualShaderNodeIf();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_CONDITIONAL; }

};

/*************************************/
/* Switch Node                       */
/*************************************/

class VisualShaderNodeSwitch : public VisualShaderNode {
	public:
		enum OpType {
			OP_TYPE_FLOAT,
			OP_TYPE_INT,
			OP_TYPE_UINT,
			OP_TYPE_VECTOR_2D,
			OP_TYPE_VECTOR_3D,
			OP_TYPE_VECTOR_4D,
			OP_TYPE_BOOLEAN,
			OP_TYPE_ENUM_SIZE,
		};

		VisualShaderNodeSwitch();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		void set_op_type(const OpType& op_type);
		OpType get_op_type() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_CONDITIONAL; }

	protected:
		OpType op_type;
};

/*************************************/
/* Is Node                           */
/*************************************/

class VisualShaderNodeIs : public VisualShaderNode {
	public:
		enum Function {
			FUNC_IS_INF,
			FUNC_IS_NAN,
			FUNC_ENUM_SIZE,
		};

		VisualShaderNodeIs();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_function(const Function& func);
		Function get_function() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_CONDITIONAL; }

	protected:
		Function func;
};

/*************************************/
/* Compare Node                      */
/*************************************/

class VisualShaderNodeCompare : public VisualShaderNode {
	public:
		enum ComparisonType {
			CMP_TYPE_SCALAR,
			CMP_TYPE_SCALAR_INT,
			CMP_TYPE_SCALAR_UINT,
			CMP_TYPE_VECTOR_2D,
			CMP_TYPE_VECTOR_3D,
			CMP_TYPE_VECTOR_4D,
			CMP_TYPE_BOOLEAN,
			CMP_TYPE_ENUM_SIZE,
		};

		enum Function {
			FUNC_EQUAL,
			FUNC_NOT_EQUAL,
			FUNC_GREATER_THAN,
			FUNC_GREATER_THAN_EQUAL,
			FUNC_LESS_THAN,
			FUNC_LESS_THAN_EQUAL,
			FUNC_ENUM_SIZE,
		};

		enum Condition {
			COND_ALL,
			COND_ANY,
			COND_ENUM_SIZE,
		};

		VisualShaderNodeCompare();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_comparison_type(const ComparisonType& type);
		ComparisonType get_comparison_type() const;

		void set_function(const Function& func);
		Function get_function() const;

		void set_condition(const Condition& cond);
		Condition get_condition() const;

		virtual std::vector<std::string> get_editable_properties() const override;
		virtual std::string get_warning() const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_CONDITIONAL; }

	protected:
		ComparisonType comparison_type;
		Function func;
		Condition condition;
};

/*************************************/
/* Multiply Add Node                 */
/*************************************/

class VisualShaderNodeMultiplyAdd : public VisualShaderNode {
	public:
		enum OpType {
			OP_TYPE_SCALAR,
			OP_TYPE_VECTOR_2D,
			OP_TYPE_VECTOR_3D,
			OP_TYPE_VECTOR_4D,
			OP_TYPE_ENUM_SIZE,
		};

		VisualShaderNodeMultiplyAdd();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		void set_op_type(const OpType& op_type);
		OpType get_op_type() const;

		virtual std::vector<std::string> get_editable_properties() const override;

		inline virtual VisualShaderNode::Category get_category() const override {
			switch (op_type) {
				case OP_TYPE_SCALAR:
					return VisualShaderNode::Category::CATEGORY_SCALAR;
				default:
					break;
			}

			return VisualShaderNode::Category::CATEGORY_VECTOR;
		}

	protected:
		OpType op_type;
};

/*************************************/
/* DistanceFade Node                 */
/*************************************/

class VisualShaderNodeDistanceFade : public VisualShaderNode {
	public:
		VisualShaderNodeDistanceFade();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;
		virtual bool has_output_port_preview(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_UTILITY; }

};

/*************************************/
/* Remap Node                        */
/*************************************/

class VisualShaderNodeRemap : public VisualShaderNode {
	public:
		VisualShaderNodeRemap();

		virtual std::string get_caption() const override;

		virtual int get_input_port_count() const override;
		virtual VisualShaderNode::PortType get_input_port_type(const int& port) const override;
		virtual std::string get_input_port_name(const int& port) const override;

		virtual int get_output_port_count() const override;
		virtual VisualShaderNode::PortType get_output_port_type(const int& port) const override;
		virtual std::string get_output_port_name(const int& port) const override;

		virtual std::string generate_code(const int& id, const std::vector<std::string>& input_vars, const std::vector<std::string>& output_vars) const override;

		inline virtual VisualShaderNode::Category get_category() const override { return VisualShaderNode::Category::CATEGORY_UTILITY; }

};

#endif //ENIGMA_VISUAL_SHADER_NODES_H
