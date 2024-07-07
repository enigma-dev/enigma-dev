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

#include "visual_shader.h"

const VisualShaderNodeInput::Port VisualShaderNodeInput::ports[] = {

	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D, "fragcoord", "FRAGCOORD" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D, "uv", "UV" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D, "color", "COLOR" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D, "screen_uv", "SCREEN_UV" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D, "texture_pixel_size", "TEXTURE_PIXEL_SIZE" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D, "screen_pixel_size", "SCREEN_PIXEL_SIZE" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D, "point_coord", "POINT_COORD" },
	{ VisualShaderNode::PortType::PORT_TYPE_SCALAR, "time", "TIME" },
	{ VisualShaderNode::PortType::PORT_TYPE_BOOLEAN, "at_light_pass", "AT_LIGHT_PASS" },
	{ VisualShaderNode::PortType::PORT_TYPE_SAMPLER, "texture", "TEXTURE" },
	{ VisualShaderNode::PortType::PORT_TYPE_SAMPLER, "normal_texture", "NORMAL_TEXTURE" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D, "specular_shininess", "SPECULAR_SHININESS" },
	{ VisualShaderNode::PortType::PORT_TYPE_SAMPLER, "specular_shininess_texture", "SPECULAR_SHININESS_TEXTURE" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D, "vertex", "VERTEX" },

	{ VisualShaderNode::PORT_TYPE_MAX, "", "" }, // End of list.
    
};

const VisualShaderNodeInput::Port VisualShaderNodeInput::preview_ports[] = {

	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D, "fragcoord", "FRAGCOORD" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D, "uv", "UV" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_4D, "color", "vec4(1.0)" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D, "screen_uv", "UV" },
	{ VisualShaderNode::PortType::PORT_TYPE_SCALAR, "time", "TIME" },

	{ VisualShaderNode::PORT_TYPE_MAX, "", "" }, // End of list.

};

const VisualShaderNodeOutput::Port VisualShaderNodeOutput::ports[] = {
	
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D, "Color", "COLOR.rgb" },
	{ VisualShaderNode::PortType::PORT_TYPE_SCALAR, "Alpha", "COLOR.a" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D, "Normal", "NORMAL" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D, "Normal Map", "NORMAL_MAP" },
	{ VisualShaderNode::PortType::PORT_TYPE_SCALAR, "Normal Map Depth", "NORMAL_MAP_DEPTH" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_3D, "Light Vertex", "LIGHT_VERTEX" },
	{ VisualShaderNode::PortType::PORT_TYPE_VECTOR_2D, "Shadow Vertex", "SHADOW_VERTEX" },

	{ VisualShaderNode::PortType::PORT_TYPE_MAX, "", "" },
};
