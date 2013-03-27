/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, DatZach, Polygone
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

#include "OpenGL3Headers.h"
#include "GL3d3d.h"
#include "GL3vertexbuffer.h"
#include "GL3textures.h"
#include "GL3mesh.h"
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

extern GLenum ptypes_by_id[16];
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

class Mesh
{
  public:
  vector<Primitive*> primitives;
  unsigned int currentPrimitive;

  vector<GLfloat> vertices;
  vector<GLfloat> textures;
  vector<GLfloat> normals;
  vector<GLfloat> colors;
  vector<GLuint> indices;

  GLuint verticesVBO;
  GLuint texturesVBO;
  GLuint maxindice;

  bool vbogenerated;
  int vbotype;

  Mesh(int vbot = vbo_static)
  {
    currentPrimitive = 0;
    vbotype = vbot;
    maxindice = 0;
    vbogenerated = false;
  }

  ~Mesh()
  {

  }

  void ClearData() 
  {
    vertices.clear();
    textures.clear();
    normals.clear();
    colors.clear();
    indices.clear();
  }

  void Clear()
  {
    primitives.clear();
    ClearData();
    // TODO: Rebuffer
  }

  void Begin(int pt)
  {
    vbogenerated = false;
    unsigned int id = primitives.size();
    currentPrimitive = id;
    Primitive* newPrim = new Primitive(pt);
    newPrim->vertstart = vertices.size()/3;
    newPrim->indexstart = indices.size();
    primitives.push_back(newPrim);
  }

  void VertexVector(float x, float y, float z)
  {
    vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
    primitives[currentPrimitive]->vertcount += 1;
  }

  void VertexIndex(int vi)
  {
    if (vi > maxindice) { maxindice = vi; }
    indices.push_back(vi);
    primitives[currentPrimitive]->indexcount += 1;
  }

  void NormalVector(float nx, float ny, float nz)
  {
    normals.push_back(nx); normals.push_back(ny); normals.push_back(nz);
  }

  void TextureVector(float tx, float ty)
  {
    textures.push_back(tx); textures.push_back(ty);
  }

  void ColorVector(double c, double a)
  {

  }

  void End()
  {

  }

  void BufferData()
  {
    // Generate And Bind The Vertex Buffer
    glGenBuffers( 1, &verticesVBO );                  // Get A Valid Name
    glBindBuffer( GL_ARRAY_BUFFER, verticesVBO );         // Bind The Buffer
    // Send the data to the GPU
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW );

    // Generate And Bind The Texture Coordinate Buffer
    glGenBuffers( 1, &texturesVBO );                 // Get A Valid Name
    glBindBuffer( GL_ARRAY_BUFFER, texturesVBO );        // Bind The Buffer
    // Send the data to the GPU
    glBufferData( GL_ARRAY_BUFFER, textures.size() * sizeof(GLfloat), &textures[0], GL_STATIC_DRAW );

    // Clean up the data from RAM it is now safe on VRAM
    ClearData();
  }

  void Draw()
  {
    if (!vbogenerated) {
      vbogenerated = true;
      BufferData();
    }
    glBindBufferARB( GL_ARRAY_BUFFER, verticesVBO );
    glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );       // Set The Vertex Pointer To The Vertex Buffer
    glBindBufferARB( GL_ARRAY_BUFFER, texturesVBO );
    glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );     // Set The TexCoord Pointer To The TexCoord Buffer

    //glNormalPointer(GL_FLOAT, 0, &normals[0]);

    for (int i = 0; i < primitives.size(); i++)
    {

      if (indices.size() > 0) {
        glDrawRangeElements(ptypes_by_id[primitives[i]->type], primitives[i]->indexstart, maxindice + 1, primitives[i]->indexcount, GL_UNSIGNED_INT, &indices[0]);
      } else {
        glDrawArrays(ptypes_by_id[primitives[i]->type], primitives[i]->vertstart, primitives[i]->vertcount);
      }
    }
  }
};

vector<Mesh*> meshes;

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
    glTranslatef(x, y, z);
    meshes[id]->Draw();
    glTranslatef(-x, -y, -z);
}

void d3d_model_draw(const unsigned int id, double x, double y, double z, int texId)
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

void d3d_model_open(const unsigned int id, int start)
{

}

void d3d_model_close(const unsigned int id)
{

}

void d3d_model_vertex(const unsigned int id, double x, double y, double z)
{
  meshes[id]->VertexVector(x, y, z);
}

void d3d_model_normal(const unsigned int id, double nx, double ny, double nz)
{
  meshes[id]->NormalVector(nx, ny, nz);
}

void d3d_model_texture(const unsigned int id, double tx, double ty)
{
  meshes[id]->TextureVector(tx, ty);
}

void d3d_model_color(const unsigned int id, int col, double alpha)
{

}

void d3d_model_index(const unsigned int id, int in)
{
  meshes[id]->VertexIndex(in);
}

void d3d_model_vertex_color(const unsigned int id, double x, double y, double z, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
}

void d3d_model_vertex_texture(const unsigned int id, double x, double y, double z, double tx, double ty)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->TextureVector(tx, ty);
}

void d3d_model_vertex_texture_color(const unsigned int id, double x, double y, double z, double tx, double ty, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->TextureVector(tx, ty);
}

void d3d_model_vertex_normal(const unsigned int id, double x, double y, double z, double nx, double ny, double nz)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
}

void d3d_model_vertex_normal_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
}

void d3d_model_vertex_normal_texture(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
  meshes[id]->TextureVector(tx, ty);
}

void d3d_model_vertex_normal_texture_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
  meshes[id]->TextureVector(tx, ty);
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
