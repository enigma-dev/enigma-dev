/** Copyright (C) 2014 Harijs Grinbergs
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

#ifndef GL3_PROFILER_H
#define GL3_PROFILER_H

#include <vector>

namespace enigma{

	struct GPUProfilerVBORender{
		bool color_enabled;
		bool texture_enabled;
		bool normals_enabled;
		
		int vertices;
		
		int triangles;
		int triangle_indeces;
		int lines;
		int line_indeces;
		int points;
		int point_indeces;
		
		int drawcalls;
		int texture;
	};

	class GPUProfiler {
		std::vector<GPUProfilerVBORender> VBORenders;
		
		public:
			int drawn_vertex_number;
			int drawn_drawcall_number;
			int drawn_vbo_number;
			
			int texture_switches;
			
			void reset_frame() { 
				drawn_vertex_number = 0;
				drawn_drawcall_number = 0;
				drawn_vbo_number = 0;
			}
			
			GPUProfilerVBORender& add_drawcall(){
				VBORenders.push_back( GPUProfilerVBORender() );
				return VBORenders.back();
			}
			
			GPUProfilerVBORender& last_drawcall(){
				return VBORenders.back();
			}
			
			GPUProfiler() : drawn_vertex_number(0), drawn_drawcall_number(0), drawn_vbo_number(0){ }
	};

}

namespace enigma_user{
	int profiler_get_vertex_count();
	int profiler_get_drawcall_count();
	int profiler_get_vbo_count();
}

#endif
