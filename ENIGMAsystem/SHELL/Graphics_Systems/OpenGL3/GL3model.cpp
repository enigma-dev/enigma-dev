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

#include "../General/GSd3d.h"
#include "GL3shapes.h"
#include "../General/GSprimitives.h"
#include "../General/GStextures.h"
#include "GL3model.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include "../General/GLbinding.h"

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

GLenum vbotypes[3] = {
  GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW
};

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
// TODO: Verify that Mesh clearing no longer has memory leaks after latest changes.
class Mesh
{
  public:
  vector<Primitive*> primitives;
  unsigned int currentPrimitive;
  unsigned int basicShapesPrimitive;

  vector<gs_scalar> vertices;
  vector<gs_scalar> textures;
  vector<gs_scalar> normals;
  vector<gs_scalar> colors;
  vector<GLuint> indices;

  /* NOTE: only use 1 VBO per model, this could be
     rewritten to make attributes offset in the single vbo */
  GLuint verticesVBO;
  GLuint texturesVBO;
  GLuint normalsVBO;
  GLuint colorsVBO;
  GLuint indexVBO;
  GLuint maxindice;
  GLuint rebufferOffset;
  bool vbogenerated;
  bool vbobuffered;
  int vbotype;
  bool useColorBuffer; //If color is not used, then it won't bind the color buffer

  unsigned int Begin(int pt)
  {
    vbobuffered = false;
    unsigned int id = primitives.size();
    currentPrimitive = id;
    Primitive* newPrim = new Primitive(pt);
    newPrim->vertstart = vertices.size()/3;
    newPrim->indexstart = indices.size();
    primitives.push_back(newPrim);

    return id;
  }

  Mesh(int vbot = enigma_user::vbo_static)
  {
    vbotype = vbot;
    maxindice = 0;
    vbogenerated = false;
    vbobuffered = false;
    useColorBuffer = false;
    currentPrimitive = 0;
    basicShapesPrimitive = Begin(4);
  }

  ~Mesh()
  {

  }

  void useColor()
  {
      useColorBuffer = true;
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
    for (unsigned int i = 0; i < primitives.size(); i++) {
      delete primitives[i];
    }
    primitives.clear();
    ClearData();
    vbobuffered = false;
  }

  void VertexVector(gs_scalar x, gs_scalar y, gs_scalar z)
  {
    vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
    primitives[currentPrimitive]->vertcount += 1;
  }

  void VertexIndex(GLuint vi)
  {
    if (vi > maxindice) { maxindice = vi; }
    indices.push_back(vi);
    primitives[currentPrimitive]->indexcount += 1;
  }

  void NormalVector(gs_scalar nx, gs_scalar ny, gs_scalar nz)
  {
    normals.push_back(nx); normals.push_back(ny); normals.push_back(nz);
  }

  void TextureVector(gs_scalar tx, gs_scalar ty)
  {
    textures.push_back(tx); textures.push_back(ty);
  }

/* For reference...
    glColor4f(__GETR(color), __GETG(color), __GETB(color), alpha);
    glVertex3d(x,y,z);
    glColor4ubv(enigma::currentcolor);
*/
  void ColorVector(int col, double alpha)
  {
    colors.push_back(__GETR(col)); colors.push_back(__GETG(col)); colors.push_back(__GETB(col)); colors.push_back(alpha);
  }

  void End()
  {

  }

  void BufferGenerate()
  {
    glGenBuffers( 1, &verticesVBO );
    glGenBuffers( 1, &texturesVBO );
    glGenBuffers( 1, &normalsVBO );
    glGenBuffers( 1, &colorsVBO );
    glGenBuffers( 1, &indexVBO );
  }

  void BufferData()
  {
    // Bind The Vertex Buffer
    glBindBuffer( GL_ARRAY_BUFFER, verticesVBO );
    // Send the data to the GPU
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], vbotypes[vbotype] );

    // Bind The Texture Coordinate Buffer
    glBindBuffer( GL_ARRAY_BUFFER, texturesVBO );
    // Send the data to the GPU
    glBufferData( GL_ARRAY_BUFFER, textures.size() * sizeof(GLfloat), &textures[0], vbotypes[vbotype] );

    // Bind The Normal Coordinate Buffer
    glBindBuffer( GL_ARRAY_BUFFER, normalsVBO );
    // Send the data to the GPU
    glBufferData( GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], vbotypes[vbotype] );

    if (useColorBuffer == true){
        // Bind The Colors Buffer
        glBindBuffer( GL_ARRAY_BUFFER, colorsVBO );
        // Send the data to the GPU
        glBufferData( GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), &colors[0], vbotypes[vbotype] );
    }

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexVBO );        // Bind The Buffer
    // Send the data to the GPU
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], vbotypes[vbotype] );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // Clean up the data from RAM it is now safe on VRAM
    ClearData();
  }

  void BufferSubData(GLint offset)
  {
    if (!vbobuffered) { return; }
    glBindBuffer( GL_ARRAY_BUFFER, verticesVBO );         // Bind The Buffer
    // Send the data to the GPU
    glBufferSubData( GL_ARRAY_BUFFER, offset * 3 * sizeof(GLfloat), vertices.size(), &vertices[0] );

    glBindBuffer( GL_ARRAY_BUFFER, texturesVBO );        // Bind The Buffer
    // Send the data to the GPU
    glBufferSubData( GL_ARRAY_BUFFER, offset * 2 * sizeof(GLfloat), textures.size(), &textures[0] );

    glBindBuffer( GL_ARRAY_BUFFER, normalsVBO );        // Bind The Buffer
    // Send the data to the GPU
    glBufferSubData( GL_ARRAY_BUFFER, offset * 3 * sizeof(GLfloat), normals.size(), &normals[0] );

    if (useColorBuffer == true){
        glBindBuffer( GL_ARRAY_BUFFER, colorsVBO );        // Bind The Buffer
        // Send the data to the GPU
        glBufferSubData( GL_ARRAY_BUFFER, offset * 4 * sizeof(GLfloat), colors.size(), &colors[0] );
    }

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexVBO );        // Bind The Buffer
    // Send the data to the GPU
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(GLuint), indices.size(), &indices[0] );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // Clean up the data from RAM it is now safe on VRAM
    ClearData();
  }

  void Open(int offset)
  {
    rebufferOffset = offset;
  }

  void Close()
  {
    BufferSubData(rebufferOffset);
  }

  void Draw()
  {
    if (!vbogenerated) {
      vbogenerated = true;
      BufferGenerate();
    }
    if (!vbobuffered) {
      vbobuffered = true;
      BufferData();
    }

    // enable vertex array's for fast vertex processing
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer( GL_ARRAY_BUFFER, verticesVBO );
    glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );       // Set The Vertex Pointer To The Vertex Buffer

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBuffer( GL_ARRAY_BUFFER, texturesVBO );
    glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );     // Set The TexCoord Pointer To The TexCoord Buffer

    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer( GL_ARRAY_BUFFER, normalsVBO );
    glNormalPointer( GL_FLOAT, 0, (char *) NULL );     // Set The Normal Pointer To The Normal Buffer

    if (useColorBuffer == true){
        glEnableClientState(GL_COLOR_ARRAY);
        glBindBuffer( GL_ARRAY_BUFFER, colorsVBO );
        glColorPointer( 4, GL_FLOAT, 0, (char *) NULL );     // Set The Color Pointer To The Color Buffer
    }

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexVBO );

    for (unsigned int i = 0; i < primitives.size(); i++)
    {
      if (primitives[i]->indexcount > 0) {
        glDrawRangeElements(ptypes_by_id[primitives[i]->type], primitives[i]->indexstart, maxindice + 1, primitives[i]->indexcount, GL_UNSIGNED_INT, 0);
      } else {
        glDrawArrays(ptypes_by_id[primitives[i]->type], primitives[i]->vertstart, primitives[i]->vertcount);
      }
    }

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    if (useColorBuffer == true) { glDisableClientState(GL_COLOR_ARRAY); }
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
    glTranslatef(x, y, z);
    meshes[id]->Draw();
    glTranslatef(-x, -y, -z);
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
  int col = enigma::currentcolor[0] | (enigma::currentcolor[1] << 8) | (enigma::currentcolor[2] << 16);
  float alpha = (float)enigma::currentcolor[3] / 255.0;
  meshes[id]->ColorVector(col, alpha);
}

//Are these functions really needed? They just break stuff!
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
//Functions questioning ends here!

void d3d_model_vertex_color(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->ColorVector(col, alpha);
  meshes[id]->useColor();
}

void d3d_model_vertex_texture(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->TextureVector(tx, ty);
  int col = enigma::currentcolor[0] | (enigma::currentcolor[1] << 8) | (enigma::currentcolor[2] << 16);
  float alpha = (float)enigma::currentcolor[3] / 255.0;
  meshes[id]->ColorVector(col, alpha);
}

void d3d_model_vertex_texture_color(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->TextureVector(tx, ty);
  meshes[id]->ColorVector(col, alpha);
  meshes[id]->useColor();
}

void d3d_model_vertex_normal(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
  int col = enigma::currentcolor[0] | (enigma::currentcolor[1] << 8) | (enigma::currentcolor[2] << 16);
  float alpha = (float)enigma::currentcolor[3] / 255.0;
  meshes[id]->ColorVector(col, alpha);
}

void d3d_model_vertex_normal_color(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
  meshes[id]->ColorVector(col, alpha);
  meshes[id]->useColor();
}

void d3d_model_vertex_normal_texture(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
  meshes[id]->TextureVector(tx, ty);
  int col = enigma::currentcolor[0] | (enigma::currentcolor[1] << 8) | (enigma::currentcolor[2] << 16);
  float alpha = (float)enigma::currentcolor[3] / 255.0;
  meshes[id]->ColorVector(col, alpha);
}

void d3d_model_vertex_normal_texture_color(const unsigned int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int col, double alpha)
{
  meshes[id]->VertexVector(x, y, z);
  meshes[id]->NormalVector(nx, ny, nz);
  meshes[id]->TextureVector(tx, ty);
  meshes[id]->ColorVector(col, alpha);
  meshes[id]->useColor();
}


void d3d_model_block(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed)
{
  GLfloat verts[] = {x1,y1,z1, x1,y1,z2, x1,y2,z1, x1,y2,z2, x2,y2,z1, x2,y2,z2, x2,y1,z1, x2,y1,z2, // sides
                     x1,y1,z1, x2,y1,z1, x1,y2,z1, x2,y2,z1,  // bottom
                     x1,y1,z2, x2,y1,z2, x1,y2,z2, x2,y2,z2}, // top
          texts[] = {0,vrep, 0,0, hrep,vrep, hrep,0,
		     0,vrep, 0,0, hrep,vrep, hrep,0,
                     0,0, hrep,0, 0,vrep, hrep,vrep,
                     0,0, hrep,0, 0,vrep, hrep,vrep},
	  norms[] = {-0.5,-0.5,-0.5, -0.5,-0.5,0.5, -0.5,0.5,-0.5, -0.5,0.5,0.5,
                     0.5,0.5,-0.5, 0.5,0.5,0.5, 0.5,-0.5,-0.5, 0.5,-0.5,0.5,
                     -0.5,-0.5,-0.5, 0.5,-0.5,-0.5, -0.5,0.5,-0.5, 0.5,0.5,-0.5, // bottom
                     -0.5,-0.5,0.5, 0.5,-0.5,0.5, -0.5,0.5,0.5, 0.5,0.5,0.5}; // top
 GLuint inds[] = {1,3,0, 3,2,0, 3,5,2, 5,4,2, 5,6,4, 5,7,6, 7,1,6, 0,6,1, // sides
                   11,9,8, 10,11,8, 12,13,15, 12,15,14}; // top and bottom

  unsigned int indoff = meshes[id]->maxindice + (meshes[id]->maxindice > 0);
  for (int ix = 0; ix < 36; ix++) {
    inds[ix] += indoff;
    if (inds[ix] > meshes[id]->maxindice) {
      meshes[id]->maxindice = inds[ix];
    }
  }

  meshes[id]->vertices.insert(meshes[id]->vertices.end(), verts, verts + 48);
  meshes[id]->primitives[0]->vertcount += 16;
  meshes[id]->normals.insert(meshes[id]->normals.end(), norms, norms + 48);
  meshes[id]->textures.insert(meshes[id]->textures.end(), texts, texts + 32);

  if (closed) {
    meshes[id]->indices.insert(meshes[id]->indices.end(), inds, inds + 36);
    meshes[id]->primitives[0]->indexcount += 36;
  } else {
    meshes[id]->indices.insert(meshes[id]->indices.end(), inds, inds + 24);
    meshes[id]->primitives[0]->indexcount += 24;
  }
}

void d3d_model_cylinder(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
        float v[100][3];
        float t[100][3];
        steps = min(max(steps, 3), 48); // i think 48 should be circle_presicion
        const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
        double a, px, py, tp;
        int k;
        a = 0; px = cx+rx; py = cy; tp = 0; k = 0;

        for (int i = 0; i <= steps; i++)
        {
            v[k][0] = px; v[k][1] = py; v[k][2] = z2;
            t[k][0] = tp; t[k][1] = 0;
            d3d_model_vertex_texture(id, px, py, z2, tp, 0);
            k++;
            v[k][0] = px; v[k][1] = py; v[k][2] = z1;
            t[k][0] = tp; t[k][1] = vrep;
            d3d_model_vertex_texture(id, px, py, z1, tp, vrep);
            k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
            v[k][0] = px; v[k][1] = py; v[k][2] = z2;
            t[k][0] = tp; t[k][1] = 0;
            d3d_model_vertex_texture(id, px, py, z1, tp, vrep);
/*
            v[k][0] = px; v[k][1] = py; v[k][2] = z2;
            t[k][0] = tp; t[k][1] = 0;
            d3d_model_vertex_texture(id, px, py, z2, tp, 0);
            k++;
            v[k][0] = px; v[k][1] = py; v[k][2] = z1;
            t[k][0] = tp; t[k][1] = vrep;
            d3d_model_vertex_texture(id, px, py, z1, tp, vrep);
            k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
            d3d_model_vertex_texture(id, v[k-4][0], v[k-4][1], v[k-4][3], t[k-4][0], t[k-4][1]);*/
        }

        if (closed)
        {
            v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
            t[k][0] = 0; t[k][1] = vrep;
            k++;
            for (int i = 0; i < steps*2; i+=2)
            {
                d3d_model_vertex_texture(id, cx, cy, z1, 0, vrep);
                d3d_model_vertex_texture(id, v[i+3][0], v[i+3][1], v[i+3][2], t[i+2][0], t[i+2][1]);
                d3d_model_vertex_texture(id, v[i+1][0], v[i+1][1], v[i+1][2], t[i][0], t[i][1]);
		d3d_model_normal(id, 0, 0, -1); d3d_model_normal(id, 0, 0, -1); d3d_model_normal(id, 0, 0, -1);
            }

            v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
            t[k][0] = 0; t[k][1] = vrep;
            k++;
            for (int i = 0; i < steps*2; i+=2)
            {
                d3d_model_vertex_texture(id, cx, cy, z2, 0, vrep);
                d3d_model_vertex_texture(id, v[i][0], v[i][1], v[i][2], t[i][0], t[i][1]);
                d3d_model_vertex_texture(id, v[i+2][0], v[i+2][1], v[i+2][2], t[i+2][0], t[i+2][1]);
		d3d_model_normal(id, 0, 0, 1); d3d_model_normal(id, 0, 0, 1); d3d_model_normal(id, 0, 0, 1);
            }
        }
}

void d3d_model_cone(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
  steps = min(max(steps, 3), 48);
  const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
  double a, px, py, tp;
  int k = 0;
  //d3d_model_primitive_begin(id, pr_trianglefan);
  d3d_model_vertex_texture(id, cx, cy, z2, 0, 0);
  k++;
  a = 0; px = cx+rx; py = cy; tp = 0;
  for (int i = 0; i <= steps; i++)
  {
    d3d_model_vertex_texture(id, px, py, z1, tp, vrep);
    k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
  }
  //d3d_model_primitive_end(id);
  if (closed)
  {
    //d3d_model_primitive_begin(id, pr_trianglefan);
    d3d_model_vertex_texture(id, cx, cy, z1, 0, vrep);
    k++;
    tp = 0;
    for (int i = steps + 1; i >= 0; i--)
    {
      d3d_model_vertex_texture(id, cx, cy, z1, tp, 0);
      k++; tp += invstep;
    }
    //d3d_model_primitive_end(id);
  }
}

void d3d_model_ellipsoid(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, int steps)
{

        float v[277][3];
        float t[277][3];
        steps = min(max(steps, 3), 24);
        const int zsteps = ceil(steps/2);
        const double cx = (x1+x2)/2, cy = (y1+y2)/2, cz = (z1+z2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, rz = (z2-z1)/2, invstep = (1.0/steps)*hrep, invstep2 = (1.0/zsteps)*vrep, pr = 2*M_PI/steps, qr = M_PI/zsteps;
        double a, b, px, py, pz, tp, tzp, cosb;
        double cosx[25], siny[25], txp[25];
        a = pr; tp = 0;
        for (int i = 0; i <= steps; i++)
        {
            cosx[i] = cos(a)*rx; siny[i] = sin(a)*ry;
            txp[i] = tp;
            a += pr; tp += invstep;
        }
        int k = 0, kk;
        //d3d_model_primitive_begin(id, pr_trianglefan);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = cz - rz;
        t[k][0] = 0; t[k][1] = vrep;
        d3d_model_vertex_texture(id, cx, cy, cz - rz, 0, vrep);
        k++;
        b = qr-M_PI/2;
        cosb = cos(b);
        pz = rz*sin(b);
        tzp = vrep-invstep2;
        px = cx+rx*cosb; py = cy;
        for (int i = 0; i <= steps; i++)
        {
            v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
            t[k][0] = txp[i]; t[k][1] = tzp;
            d3d_model_vertex_texture(id, px, py, cz + pz, txp[i], tzp);
            k++; px = cx+cosx[i]*cosb; py = cy+siny[i]*cosb;
        }
        //d3d_model_primitive_end(id);
        for (int ii = 0; ii < zsteps - 2; ii++)
        {
            b += qr;
            cosb = cos(b);
            pz = rz*sin(b);
            tzp -= invstep2;
            //d3d_model_primitive_begin(id, pr_trianglestrip);
            px = cx+rx*cosb; py = cy;
            for (int i = 0; i <= steps; i++)
            {
                kk = k - steps - 1;
                d3d_model_vertex_texture(id, v[kk][0], v[kk][1], v[kk][2], t[kk][0], t[kk][1]);
                v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
                t[k][0] = txp[i]; t[k][1] = tzp;
                d3d_model_vertex_texture(id, px, py, cz + pz, txp[i], tzp);
                k++; px = cx+cosx[i]*cosb; py = cy+siny[i]*cosb;
            }
            //d3d_model_primitive_end(id);
        }
        //d3d_model_primitive_begin(id, pr_trianglefan);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = cz + rz;
        t[k][0] = 0; t[k][1] = 0;
        d3d_model_vertex_texture(id, cx, cy, cz + rz, 0, 0);
        k++;
        for (int i = k - steps - 2; i <= k - 2; i++)
        {
            d3d_model_vertex_texture(id, v[i][0], v[i][1], v[i][2], t[i][0], t[i][1]);
        }
        //d3d_model_primitive_end(id);

}

void d3d_model_icosahedron(const unsigned int id)
{

}

void d3d_model_torus(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI)
{
  int numc = csteps, numt = tsteps;

  for (int i = 0; i < numc; i++) {
    //d3d_model_primitive_begin(id, pr_quadstrip);
    for (int j = 0; j <= numt; j++) {
      for (int k = 1; k >= 0; k--) {

        double s = (i + k) % numc + 0.5;
        double t = j % numt;

        double x = (radius + tradius * cos(s * TWOPI / numc)) * cos(t * TWOPI / numt);
        double y = (radius + tradius * cos(s * TWOPI / numc)) * sin(t * TWOPI / numt);
        double z = tradius * sin(s * TWOPI / numc);
	double u = (i + k) / (float)numc;
	double v = t / (float)numt;

	d3d_model_vertex_texture(id, x1 + x, y1 + y, z1 + z, u, v);
      }
    }
    //d3d_model_primitive_end(id);
  }
}

void d3d_model_wall(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep)
{
  if ((x1 == x2 && y1 == y2) || z1 == z2) {
    return;
  }

  float xd = x2-x1, yd = y2-y1, zd = z2-z1;
  float normal[3] = {xd*zd, zd*yd, 0};
  float mag = hypot(normal[0], normal[1]);
  normal[0] /= mag;
  normal[1] /= mag;
  if (x2 < x1) {
    normal[0]=-normal[0]; }
  if (y2 < y1) {
    normal[1]=-normal[1]; }

  //d3d_model_primitive_begin(id, pr_trianglestrip);
  d3d_model_vertex_normal_texture(id, x1, y1, z1, 0, 0, normal[0], normal[1], normal[2]);
  d3d_model_vertex_normal_texture(id, x2, y2, z1, hrep, 0, normal[0], normal[1], normal[2]);
  d3d_model_vertex_normal_texture(id, x1, y1, z2, 0, vrep, normal[0], normal[1], normal[2]);
  d3d_model_vertex_normal_texture(id, x2, y2, z2, hrep, vrep, normal[0], normal[1], normal[2]);

  if (x2>x1 || y2>y1) {
    d3d_model_index(id, 0);
  } else {
    d3d_model_index(id, 3);
  }

  if (x2<x1 || y2<y1) {
    d3d_model_index(id, 2);
  } else {
    d3d_model_index(id, 1);
  }

  if (x2<x1 || y2<y1) {
    d3d_model_index(id, 1);
  } else {
    d3d_model_index(id, 2);
  }

  if (x2>x1 || y2>y1) {
    d3d_model_index(id, 3);
  } else {
    d3d_model_index(id, 0);
  }

  //d3d_model_primitive_end(id);
}

void d3d_model_floor(const unsigned int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep)
{
  GLfloat normal[] = {0, 0, 1};

  //d3d_model_primitive_begin(id, pr_trianglestrip);

  if (x2>x1 || y2>y1) {
    d3d_model_index(id, 0); d3d_model_index(id, 2); d3d_model_index(id, 1); d3d_model_index(id, 3);
    normal[2] = -1;
  } else {
    d3d_model_index(id, 3); d3d_model_index(id, 1); d3d_model_index(id, 2); d3d_model_index(id, 0);
  }

  d3d_model_vertex_normal_texture(id, x1, y1, z1, 0, 0, normal[0], normal[1], normal[2]);
  d3d_model_vertex_normal_texture(id, x2, y1, z2, 0, vrep, normal[0], normal[1], normal[2]);
  d3d_model_vertex_normal_texture(id, x1, y2, z1, hrep, 0, normal[0], normal[1], normal[2]);
  d3d_model_vertex_normal_texture(id, x2, y2, z2, hrep, vrep, normal[0], normal[1], normal[2]);
  //d3d_model_primitive_end(id);
}

}

