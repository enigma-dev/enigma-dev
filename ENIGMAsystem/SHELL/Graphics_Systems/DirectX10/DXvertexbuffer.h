/** Copyright (C) 2008-2013 Robert B. Colton
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

#ifndef _DXVERTEXBUFFER__H
#define _DXVERTEXBUFFER__H

#include <vector>
using std::vector;

enum {
  vb_static,
  vb_dynamic,
  vb_stream
};

struct VertexBuffer {
  float vertexdata[];
  float colordata[];
  float texturedata[];

  VertexBuffer()
  {
  }
  ~VertexBuffer()
  {
    //delete vertexdata;
    //delete colordata;
    //delete texturedata;
  }
  void Draw()
  {

  }
};

extern vector<VertexBuffer*> vertexbuffers;
extern int current_vertex_buffer;

int vertex_buffer_create(int type); // creates a vertex buffer and returns the id
void vertex_buffer_free(int id); // frees the vertex buffer object and all its vertex/index data
void vertex_buffer_set(int id); // redirects all draw calls to use the given buffer
int vertex_buffer_get(); // gets the currently set vertex buffer
void vertex_buffer_reset(); // redirects draw calls to the global vertex buffer
void vertex_buffer_draw(int id); // manually tells the buffer to upload its vertex/index data and draw immediately
bool vertex_buffer_supported(); // gets whether the present hardware supports vertex buffer objects

#endif
