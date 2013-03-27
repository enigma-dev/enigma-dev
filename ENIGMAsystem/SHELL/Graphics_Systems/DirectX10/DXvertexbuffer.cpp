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

#include "DirectXHeaders.h"
#include "DXvertexbuffer.h"
#include <math.h>

// first one is the "Global Vertex Buffer"
vector<VertexBuffer*> vertexbuffers(0);
int current_vertex_buffer = 0;

int vertex_buffer_create(int type)
{
  int id = vertexbuffers.size();
  VertexBuffer* vertbuf = new VertexBuffer();
  vertexbuffers.push_back(vertbuf);
  return id;
}

void vertex_buffer_free(int id)
{
  delete vertexbuffers[id];
}

void vertex_buffer_set(int id)
{
  current_vertex_buffer = id;
}

int vertex_buffer_get()
{
  return current_vertex_buffer;
}

void vertex_buffer_reset()
{
  current_vertex_buffer = 0;
}

void vertex_buffer_draw(int id)
{
  vertexbuffers[id]->Draw();
}

bool vertex_buffer_supported()
{

}
