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

namespace enigma {

	struct GPUProfilerBatch{
		bool color_enabled;
		bool texture_enabled;
		bool normals_enabled;

		int triangles;
		int triangles_indexed;
		int lines;
		int lines_indexed;
		int points;
		int points_indexed;

		int drawcalls;
		int texture;

    int vertices() { return triangles+lines+points; }
    int indecies() { return triangles_indexed+lines_indexed+points_indexed; }
	};

	class GPUProfiler {
		std::vector<GPUProfilerBatch> BatchesRenders;

		public:
			int drawn_vertex_number;
			int drawn_drawcall_number;
			int drawn_vbo_number;

			int texture_switches;

			void reset_frame() {
				drawn_vertex_number = 0;
				drawn_drawcall_number = 0;
				drawn_vbo_number = 0;
        BatchesRenders.clear();
			}

      void end_frame() {
        reset_frame();
        for (unsigned int i=0; i<BatchesRenders.size(); ++i){
          drawn_vertex_number += BatchesRenders[i].vertices();
          drawn_drawcall_number += BatchesRenders[i].drawcalls;
        }
        drawn_vbo_number = BatchesRenders.size();
      }

			GPUProfilerBatch& add_drawcall(){
				BatchesRenders.push_back( GPUProfilerBatch() );
				return BatchesRenders.back();
			}

			GPUProfilerBatch& last_drawcall(){
				return BatchesRenders.back();
			}

			GPUProfiler() : drawn_vertex_number(0), drawn_drawcall_number(0), drawn_vbo_number(0){ }
	};

	extern GPUProfiler gpuprof;
}

namespace enigma_user{
	int profiler_get_vertex_count();
	int profiler_get_drawcall_count();
	int profiler_get_vbo_count();
}

#endif
