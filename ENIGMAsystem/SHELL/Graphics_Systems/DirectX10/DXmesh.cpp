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
#include "DXd3d.h"
#include "DXvertexbuffer.h"
#include "DXtextures.h"
#include "DXmesh.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include "binding.h"

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
  vector<float> vertices;
  vector<float> textures;
  vector<float> normals;
  vector<float> colors;
  vector<unsigned int> indices;
  unsigned int maxindice = 0;

  unsigned int verticesVBO;
  unsigned int texturesVBO;

  int type;
  int vbotype;

  Primitive(int pt, int vbt)
  {
    type = pt;
    vbotype = vbt;
  }

  ~Primitive()
  {

  }

  void Clear()
  {
    vertices.clear();
    textures.clear();
    normals.clear();
    colors.clear();
    indices.clear();
  }

  void Begin()
  {

  }

  void VertexVector(float x, float y, float z)
  {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
  }

  void VertexIndex(int vi)
  {
    if (vi > maxindice) { maxindice = vi; }
    indices.push_back(vi);
  }

  void NormalVector(float nx, float ny, float nz)
  {
    normals.push_back(nx); normals.push_back(ny); normals.push_back(nz);
  }

  void TextureVector(float tx, float ty)
  {
    textures.push_back(tx); textures.push_back(ty);
  }

  void ColorVector()
  {

  }
 
  void End()
  {

  }

  void Draw()
  {

  }
};

class Mesh
{
  public:
  vector<Primitive*> primitives;
  unsigned int currentPrimitive = 0;

  Mesh()
  {

  }

  ~Mesh()
  {

  }

  void Clear()
  {
    for (int i = 0; i < primitives.size(); i++) 
    {
      primitives[i]->Clear();
      delete primitives[i];
      primitives.clear();
    }
  }

  void Begin(int pt, int vbt)
  {
    unsigned int id = primitives.size();
    currentPrimitive = id;
    Primitive* newPrim = new Primitive(pt, vbt);
    primitives.push_back(newPrim);
  }

  void VertexVector(float x, float y, float z)
  {
    primitives[currentPrimitive]->VertexVector(x,y,z);
  }

  void VertexIndex(int vi) 
  {
    primitives[currentPrimitive]->VertexIndex(vi);
  }

  void NormalVector(float nx, float ny, float nz)
  {
    primitives[currentPrimitive]->NormalVector(nx,ny,nz);
  }

  void TextureVector(float tx, float ty)
  {
    primitives[currentPrimitive]->TextureVector(tx,ty);
  }

  void ColorVector()
  {
    primitives[currentPrimitive]->ColorVector();
  }
 
  void End()
  {
    primitives[currentPrimitive]->End();
  }

  void Draw()
  {
    for (int i = 0; i < primitives.size(); i++) 
    {
      primitives[i]->Draw();
    }
  }
};

vector<Mesh*> meshes;

unsigned int d3d_model_create()
{
  unsigned int id = meshes.size();
  meshes.push_back(new Mesh());
  return id;
}

void d3d_model_destroy(const unsigned int id)
{
  meshes[id]->Clear();
  delete meshes[id];
}

void d3d_model_copy(const unsigned int id, const unsigned int source)
{
  meshes[id] = meshes[source];
}

unsigned int d3d_model_duplicate(const unsigned int source)
{
  //TODO: Write duplicate code meshes
}

bool d3d_model_exists(const unsigned int id)
{
  return (id > 0 && id < meshes.size());
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

void d3d_model_draw(const unsigned int id, double x, double y, double z) // overload for no additional texture call's
{

}

void d3d_model_draw(const unsigned int id, double x, double y, double z, int texId)
{

}

void d3d_model_primitive_begin(const unsigned int id, int kind)
{

}

void d3d_model_primitive_end(const unsigned int id)
{

}

void d3d_model_index(const unsigned int id, int in)
{

}

void d3d_model_vertex(const unsigned int id, double x, double y, double z)
{

}

void d3d_model_vertex_color(const unsigned int id, double x, double y, double z, int col, double alpha)
{

}

void d3d_model_vertex_texture(const unsigned int id, double x, double y, double z, double tx, double ty)
{

}

void d3d_model_vertex_texture_color(const unsigned int id, double x, double y, double z, double tx, double ty, int col, double alpha)
{

}

void d3d_model_vertex_normal(const unsigned int id, double x, double y, double z, double nx, double ny, double nz)
{

}

void d3d_model_vertex_normal_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, int col, double alpha)
{

}

void d3d_model_vertex_normal_texture(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty)
{

}

void d3d_model_vertex_normal_texture_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int col, double alpha)
{

}

void d3d_model_block(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed)
{

}

void d3d_model_cylinder(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps)
{

}

void d3d_model_cone(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps)
{

}

void d3d_model_ellipsoid(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, int steps)
{

}

void d3d_model_icosahedron(const unsigned int id)
{

}

void d3d_model_torus(const unsigned int id, double x1, double y1, double z1, int hrep, int vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI)
{

}

void d3d_model_wall(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
{

}

void d3d_model_floor(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
{
 
}
