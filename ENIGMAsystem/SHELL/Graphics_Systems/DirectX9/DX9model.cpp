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

#include "DX9d3d.h"
#include "DX9shapes.h"
#include "../General/GSprimitives.h"
#include "DX9vertexbuffer.h"
#include "../General/GStextures.h"
#include "DX9model.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include "../General/DXbinding.h"

using namespace std;

#define __GETR(x) ((x & 0x0000FF))/255.0
#define __GETG(x) ((x & 0x00FF00)>>8)/255.0
#define __GETB(x) ((x & 0xFF0000)>>16)/255.0

#include <iostream>
#include <map>
#include <list>
#include "Universal_System/fileio.h"
#include "Universal_System/estring.h"

#include <vector>
using std::vector;

namespace enigma {
  extern unsigned char currentcolor[4];
}

struct Primitive
{
  unsigned int vertcount;
  unsigned int vertstart;
  unsigned int indexcount;
  unsigned int indexstart;
  int type;

  Primitive(int pt)
  {
    type = pt;
    vertcount = 0;
    vertstart = 0;
    indexcount = 0;
    indexstart = 0;
  }

  ~Primitive()
  {

  }
};

/* Mesh clearing has a memory leak */
class Mesh
{
  public:

  unsigned int Begin(int pt)
  {

  }

  Mesh(int vbot = enigma_user::vbo_static)
  {

  }

  ~Mesh()
  {

  }

  void ClearData() 
  {

  }

  void Clear()
  {

  }

  void VertexVector(gs_scalar x, gs_scalar y, gs_scalar z)
  {

  }

  void VertexIndex(GLuint vi)
  {

  }

  void NormalVector(gs_scalar nx, gs_scalar ny, gs_scalar nz)
  {

  }

  void TextureVector(gs_scalar tx, gs_scalar ty)
  {

  }

  void ColorVector(int col, double alpha)
  {
  
  }

  void End()
  {
 
  }

  void BufferGenerate()
  {

  }

  void BufferData()
  {

  }

  void BufferSubData(GLint offset)
  {

  }

  void Open(int offset) 
  {

  }

  void Close()
  {

  }

  void Draw()
  {

  }
};

vector<Mesh*> meshes;

namespace enigma_user
{

unsigned int d3d_model_create(int vbot)
{
  unsigned int id = meshes.size();
  meshes.push_back(new Mesh(vbot));
  return id;
}

void d3d_model_destroy(const unsigned int id)
{
  meshes[id]->Clear();
  delete meshes[id];
}

void d3d_model_copy(const unsigned int id, const unsigned int source)
{
  //TODO: Write copy meshes code
}

void d3d_model_merge(const unsigned int id, const unsigned int source)
{
  //TODO: Write merge meshes code
}

unsigned int d3d_model_duplicate(const unsigned int source)
{
  //TODO: Write duplicate meshes code
}

bool d3d_model_exists(const unsigned int id)
{
  return (id >= 0 && id < meshes.size());
}

void d3d_model_clear(const unsigned int id)
{
  meshes[id]->Clear();
}

void d3d_model_save(const unsigned int id, string fname)
{
  //TODO: Write save code for meshes
}

bool d3d_model_load(const unsigned int id, string fname)
{
  //TODO: this needs to be rewritten properly not using the file_text functions
  using namespace enigma_user;
  int file = file_text_open_read(fname);
  if (file == -1) {
    return false;
  }
  int something = file_text_read_real(file);
  if (something != 100) {
    return false;
  }
  file_text_readln(file);
  file_text_read_real(file);  //don't see the use in this value, it doesn't equal the number of calls left exactly
  file_text_readln(file);
  int kind;
  float v[3], n[3], t[2];
  double col, alpha;
  while (!file_text_eof(file))
  {
    switch (int(file_text_read_real(file)))
    {
      case  0:
        kind = file_text_read_real(file);
        d3d_model_primitive_begin(id, kind);
        break;
      case  1:
        d3d_model_primitive_end(id);
        break;
      case  2:
        v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
        d3d_model_vertex(id, v[0],v[1],v[2]);
        break;
      case  3:
        v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
        col = file_text_read_real(file); alpha = file_text_read_real(file);
        d3d_model_vertex_color(id, v[0],v[1],v[2],col,alpha);
        break;
      case  4:
        v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
        t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
        d3d_model_vertex_texture(id, v[0],v[1],v[2],t[0],t[1]);
        break;
      case  5:
        v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
        t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
        col = file_text_read_real(file); alpha = file_text_read_real(file);
        d3d_model_vertex_texture_color(id, v[0],v[1],v[2],t[0],t[1],col,alpha);
        break;
      case  6:
        v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
        n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
        d3d_model_vertex_normal(id, v[0],v[1],v[2],n[0],n[1],n[2]);
        break;
      case  7:
        v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
        n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
        col = file_text_read_real(file); alpha = file_text_read_real(file);
        d3d_model_vertex_normal_color(id, v[0],v[1],v[2],n[0],n[1],n[2],col,alpha);
        break;
      case  8:
        v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
        n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
        t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
        d3d_model_vertex_normal_texture(id, v[0],v[1],v[2],n[0],n[1],n[2],t[0],t[1]);
        break;
      case  9:
        v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
        n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
        t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
        col = file_text_read_real(file); alpha = file_text_read_real(file);
        d3d_model_vertex_normal_texture_color(id, v[0],v[1],v[2],n[0],n[1],n[2],t[0],t[1],col,alpha);
        break;
      case  10:
        d3d_model_block(id, file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file), true);
        break;
      case  11:
        d3d_model_cylinder(id, file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
        break;
      case  12:
        d3d_model_cone(id, file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
        break;
      case  13:
        d3d_model_ellipsoid(id, file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
        break;
      case  14:
        d3d_model_wall(id, file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
        break;
      case  15:
        d3d_model_floor(id, file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
        break;
    }
    file_text_readln(file);
  }

  file_text_close(file);
  return true;
}

void d3d_model_draw(const unsigned int id) // overload for no additional texture or transformation call's
{
    meshes[id]->Draw();
}

void d3d_model_draw(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z) // overload for no additional texture call's
{

}

void d3d_model_draw(const unsigned int id, int texId)
{
    texture_use(get_texture(texId));
    meshes[id]->Draw();
}

void d3d_model_draw(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, int texId)
{
    texture_use(get_texture(texId));
    d3d_model_draw(id, x, y, z);
}

void d3d_model_primitive_begin(const unsigned int id, int kind)
{
  meshes[id]->Begin(kind);
}

void d3d_model_primitive_end(const unsigned int id)
{
  meshes[id]->End();
}

void d3d_model_open(const unsigned int id, int offset)
{
  meshes[id]->Open(offset);
}

void d3d_model_close(const unsigned int id)
{
  meshes[id]->Close();
}

void d3d_model_vertex(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z)
{
  meshes[id]->VertexVector(x, y, z);
}

void d3d_model_normal(const unsigned int id, gs_scalar nx, gs_scalar ny, gs_scalar nz)
{
  meshes[id]->NormalVector(nx, ny, nz);
}

void d3d_model_texture(const unsigned int id, gs_scalar tx, gs_scalar ty)
{
  meshes[id]->TextureVector(tx, ty);
}

void d3d_model_color(const unsigned int id, int col, double alpha)
{
  meshes[id]->ColorVector(col, alpha);
}

void d3d_model_index(const unsigned int id, GLuint in)
{
  meshes[id]->VertexIndex(in);
}

void d3d_model_vertex_color(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
}

void d3d_model_vertex_texture(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->TextureVector(tx, ty);
}

void d3d_model_vertex_texture_color(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->TextureVector(tx, ty);
}

void d3d_model_vertex_normal(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
}

void d3d_model_vertex_normal_color(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
}

void d3d_model_vertex_normal_texture(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
  meshes[id]->TextureVector(tx, ty);
}

void d3d_model_vertex_normal_texture_color(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
  meshes[id]->TextureVector(tx, ty);
}


void d3d_model_block(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed)
{
 
}

void d3d_model_cylinder(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
  
}

void d3d_model_cone(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{

}

void d3d_model_ellipsoid(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, int steps)
{

}

void d3d_model_icosahedron(const unsigned int id)
{

}

void d3d_model_torus(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI)
{

}

void d3d_model_wall(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep)
{
 
}

void d3d_model_floor(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep)
{
 
}

}

