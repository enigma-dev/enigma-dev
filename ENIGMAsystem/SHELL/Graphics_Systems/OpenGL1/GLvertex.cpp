/** Copyright (C) 2013 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <vector>
#include <map>
using std::vector;
using std::map;

#include "../General/GSvertex.h"

namespace enigma {

struct VertexFormat {
	map<int,int> flags;
	
	VertexFormat() {
	
	}
	
	~VertexFormat() {
	
	}
	
	void AddAttribute(int type, int usage) {
		flags.insert(map<int,int>::value_type(type, usage));
	}
};

struct VertexBuffer {

};

vector<VertexFormat*> vertexFormats;
vector<VertexBuffer*> vertexBuffers;
	
VertexFormat* vertexFormat = 0;

}

namespace enigma_user {

void vertex_format_begin() {
	enigma::vertexFormat = new enigma::VertexFormat();
}

void vertex_format_add_colour() {
	vertex_format_add_custom(vertex_type_colour, vertex_usage_colour);
}

void vertex_format_add_position() {
	vertex_format_add_custom(vertex_type_float3, vertex_usage_position);
}

void vertex_format_add_position_3d() {
	vertex_format_add_custom(vertex_type_float2, vertex_usage_position);
} 

void vertex_format_add_textcoord() {
	vertex_format_add_custom(vertex_type_float2, vertex_usage_textcoord);
}

void vertex_format_add_normal() {
	vertex_format_add_custom(vertex_type_float3, vertex_usage_normal);
}

void vertex_format_add_custom(int type, int usage) {
	enigma::vertexFormat->AddAttribute(type, usage);
}

int vertex_format_end() {
	enigma::vertexFormats.push_back(enigma::vertexFormat);
	return enigma::vertexFormats.size() - 1;
}

}