/** Copyright (C) 2008-2013 Robert B. Colton, Josh Ventura, DatZach, Polygone
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

#include "../General/OpenGLHeaders.h"
#include "../General/GSd3d.h"
#include "GL3shapes.h"
#include "../General/GSprimitives.h"
#include "../General/GSmodel.h"
#include "../General/GStextures.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include <stdio.h>

#include "GL3binding.h"
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

unsigned get_texture(int texid);

extern GLenum ptypes_by_id[16];
namespace enigma {
  extern unsigned char currentcolor[4];
}

//NOTE: This class handles batching, indexing, and other optimization for you and is very very efficient.
class Mesh
{
  public:
  unsigned currentPrimitive; //The type of the current primitive being added to the model

  vector<gs_scalar> vertices; // Temporary vertices container for the current primitive until they are batched
  vector<GLuint> indices; // Temporary indices that can optionally be supplied, otherwise they will get generated by the batcher.
  vector<gs_scalar> triangleVertices; // The vertices added to all triangle primitives batched into a single triangle list to be buffered to the GPU
  vector<GLuint> triangleIndices; // The triangle indices either concatenated by batching or supplied in the temporary container.
  vector<gs_scalar> lineVertices; // The vertices added to all line primitives batched into a single line list to be buffered to the GPU 
  vector<GLuint> lineIndices; // The line indices either concatenated by batching or supplied in the temporary container.
  vector<gs_scalar> pointVertices; // The vertices added to all point primitives batched into a single point list to be buffered to the GPU
  vector<GLuint> pointIndices; // The point indices either concatenated by batching or supplied in the temporary container.
  
  bool useColors; // If colors have been added to the model
  bool useTextures; // If texture coordinates have been added
  bool useNormals; // If normals have been added
  
  unsigned pointCount; // The number of vertices in the point buffer but after batching it becomes the number of indices
  unsigned triangleCount; // The number of vertices in the triangle buffer but after batching it becomes the number of indices
  unsigned lineCount; // The number of vertices in the line buffer but after batching it becomes the number of indices
  
  GLuint vertexBuffer; // Interleaved vertex buffer object TRIANGLES|LINES|POINTS with triangles first since they are most likely to be used
  GLuint indexBuffer; // Interleaved index buffer object TRIANGLES|LINES|POINTS with triangles first since they are most likely to be used
  
  bool vbogenerated; // Whether or not the buffer objects have been generated
  bool vbobuffered; // Whether or not the buffer objects have been buffered
  
  Mesh()
  {
    vbogenerated = false;
	
    useColors = false;
    useTextures = false;
    useNormals = false;
	
	pointCount = 0;
	triangleCount = 0;
	lineCount = 0;
	
    currentPrimitive = 0;
  }

  ~Mesh()
  {
	glDeleteBuffersARB(1, &vertexBuffer);
	glDeleteBuffersARB(1, &indexBuffer);
  }

  void ClearData()
  {
    triangleVertices.clear();
	pointVertices.clear();
	lineVertices.clear();
	triangleIndices.clear();
	pointIndices.clear();
	lineIndices.clear();
  }

  void Clear()
  {
    ClearData();
	
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	
	vbogenerated = false;
	
	useColors = false;
    useTextures = false;
    useNormals = false;
	
	pointCount = 0;
	triangleCount = 0;
	lineCount = 0;
  }
  
  void Begin(int pt)
  {
    vbobuffered = false;
    currentPrimitive = pt;
  }

  void AddVertex(gs_scalar x, gs_scalar y, gs_scalar z)
  {
    vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
  }

  void AddNormal(gs_scalar nx, gs_scalar ny, gs_scalar nz)
  {
    vertices.push_back(nx); vertices.push_back(ny); vertices.push_back(nz);
	useNormals = true;
  }

  void AddTexture(gs_scalar tx, gs_scalar ty)
  {
    vertices.push_back(tx); vertices.push_back(ty);
	useTextures = true;
  }

  // NOTE: The vertex format for this class should really only be handling 4 integers not 4 floats.
  void AddColor(int col, double alpha)
  {
    vertices.push_back(__GETR(col)); vertices.push_back(__GETG(col)); vertices.push_back(__GETB(col)); vertices.push_back(alpha);
	useColors = true;
  }

  void End()
  {
	//NOTE: This batching does not check for degenerate primitives or remove duplicate vertices.
	
	unsigned stride = 3;
    if (useNormals) stride += 3;
	if (useTextures) stride += 2;
    if (useColors) stride += 4;
	
	// Primitive has ended so now we need to batch the vertices that were given into single lists, eg. line list, triangle list, point list
	// Indices are optionally supplied, model functions can also be added for the end user to supply the indexing themselves for each primitive
	// but the batching system does not care either way if they are not supplied it will automatically generate them.
	switch (currentPrimitive) {
		case enigma_user::pr_pointlist:
			pointVertices.insert(pointVertices.end(), vertices.begin(), vertices.end());
			if (indices.size() > 0) {
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += pointCount; }
				pointIndices.insert(pointIndices.end(), indices.begin(), indices.end());
			} else {
				for (unsigned i = 0; i < vertices.size() / stride; i++) {
					pointIndices.push_back(pointCount + i);
				}
			}
			pointCount += vertices.size() / stride;
			break;
		case enigma_user::pr_linestrip:
			lineVertices.insert(lineVertices.end(), vertices.begin(), vertices.end());
			if (indices.size() > 0) {
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += lineCount; }
				lineIndices.insert(lineIndices.end(), indices.begin(), indices.end());
			} else {
				for (unsigned i = 0; i < vertices.size() / stride - 1; i++) {
					lineIndices.push_back(lineCount + i);
					lineIndices.push_back(lineCount + i + 1);
				}
			}
			lineCount += vertices.size() / stride;
			break;
		case enigma_user::pr_linelist:
			lineVertices.insert(lineVertices.end(), vertices.begin(), vertices.end());
			if (indices.size() > 0) {
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += lineCount; }
				lineIndices.insert(lineIndices.end(), indices.begin(), indices.end());
			} else {
				for (unsigned i = 0; i < vertices.size() / stride; i++) {
					lineIndices.push_back(lineCount + i);
				}
			}
			lineCount += vertices.size() / stride;
			break;
		case enigma_user::pr_trianglestrip:
			triangleVertices.insert(triangleVertices.end(), vertices.begin(), vertices.end());
			if (indices.size() > 0) {
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += triangleCount; }
				triangleIndices.insert(triangleIndices.end(), indices.begin(), indices.end());
			} else {
				for (unsigned i = 0; i < vertices.size() / stride - 2; i++) {
					if (i % 2) {
						triangleIndices.push_back(triangleCount + i + 2);
						triangleIndices.push_back(triangleCount + i + 1);
						triangleIndices.push_back(triangleCount + i);
					} else {
						triangleIndices.push_back(triangleCount + i);
						triangleIndices.push_back(triangleCount + i + 1);
						triangleIndices.push_back(triangleCount + i + 2);
					}
				}
			}
			triangleCount += vertices.size() / stride;
			break;
		case enigma_user::pr_trianglelist:
			triangleVertices.insert(triangleVertices.end(), vertices.begin(), vertices.end());
			if (indices.size() > 0) {
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += triangleCount; }
				triangleIndices.insert(triangleIndices.end(), indices.begin(), indices.end());
			} else {
				for (unsigned i = 0; i < vertices.size() / stride; i++) {
					triangleIndices.push_back(triangleCount + i);
				}
			}
			triangleCount += vertices.size() / stride;
			break;
		case enigma_user::pr_trianglefan:
			triangleVertices.insert(triangleVertices.end(), vertices.begin(), vertices.end());
			if (indices.size() > 0) {
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += triangleCount; }
				triangleIndices.insert(triangleIndices.end(), indices.begin(), indices.end());
			} else {
				for (unsigned i = 1; i < vertices.size() / stride - 1; i++) {
					triangleIndices.push_back(triangleCount);
					triangleIndices.push_back(triangleCount + i);
					triangleIndices.push_back(triangleCount + i + 1);
				}
			}
			triangleCount += vertices.size() / stride;
			break;
	}

	// Clean up the temporary vertex and index containers now that they have been batched efficiently
	vertices.clear();
	indices.clear();
  }

  void BufferGenerate(bool subdata)
  {
	vector<gs_scalar> vdata;
	vector<GLuint> idata;
	
	unsigned interleave = 0;
		
	if (triangleCount > 0) {
		vdata.insert(vdata.begin(), triangleVertices.begin(), triangleVertices.end());
		idata.insert(idata.begin(), triangleIndices.begin(), triangleIndices.end());
		triangleCount = triangleIndices.size();
	}
	
	if (lineCount > 0) {
		vdata.insert(vdata.begin(), lineVertices.begin(), lineVertices.end());
		interleave += triangleVertices.size();
		for (std::vector<GLuint>::iterator it = lineIndices.begin(); it != lineIndices.end(); ++it) { *it += interleave; }
		idata.insert(idata.begin(), lineIndices.begin(), lineIndices.end());
		lineCount = lineIndices.size();
	}
	
	if (pointCount > 0) {
		vdata.insert(vdata.begin(), pointVertices.begin(), pointVertices.end());
		interleave += lineVertices.size();
		for (std::vector<GLuint>::iterator it = lineIndices.begin(); it != lineIndices.end(); ++it) { *it += interleave; }
		idata.insert(idata.begin(), pointIndices.begin(), pointIndices.end());
		pointCount = pointIndices.size();
	}
	
	glGenBuffersARB( 1, &vertexBuffer );
	glGenBuffersARB( 1, &indexBuffer );
	// Bind The Vertex Buffer
	glBindBufferARB( GL_ARRAY_BUFFER, vertexBuffer );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
	
	// Send the data to the GPU
	if (subdata) {
		
	} else {
		glBufferDataARB( GL_ARRAY_BUFFER, vdata.size() * sizeof(gs_scalar), &vdata[0], GL_STATIC_DRAW );
		glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER, idata.size() * sizeof(GLuint), &idata[0], GL_STATIC_DRAW );
	}

	// Unbind the buffers we do not need them anymore
	glBindBufferARB( GL_ARRAY_BUFFER, 0 );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, 0 );
	// Clean up temporary interleaved data
	vdata.clear();
	idata.clear();
    // Clean up the data from RAM it is now safe on VRAM
    ClearData();
  }

  void Draw()
  {
    if (!vbogenerated) {
      vbogenerated = true;
	  vbobuffered = true;
      BufferGenerate(false);
    } else if (!vbobuffered) {
	  vbobuffered = true;
	  BufferGenerate(true);
	}
  
	GLsizei stride = 3;
    if (useNormals) stride += 3;
	if (useTextures) stride += 2;
    if (useColors) stride += 4;
	stride = stride * sizeof( gs_scalar );
	
	#define OFFSET( P )  ( ( const GLvoid * ) ( sizeof( gs_scalar ) * ( P         ) ) )
	GLsizei STRIDE = stride;

	// Enable vertex array's for fast vertex processing
	glBindBufferARB( GL_ARRAY_BUFFER, vertexBuffer );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

	glEnableClientState(GL_VERTEX_ARRAY);
	int offset = 0;
	glVertexPointer( 3, GL_FLOAT, STRIDE, OFFSET(offset) ); // Set the vertex pointer to the offset in the buffer
	offset += 3;
	
    if (useNormals){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer( GL_FLOAT, STRIDE, OFFSET(offset) ); // Set the normal pointer to the offset in the buffer
		offset += 3;
    }

	if (useTextures){
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 2, GL_FLOAT, STRIDE, OFFSET(offset) ); // Set the texture pointer to the offset in the buffer
		offset += 2;
	}
	
    if (useColors){
		glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer( 4, GL_FLOAT, STRIDE, OFFSET(offset)); // Set The Color Pointer To The Color Buffer
    }

	#define OFFSETE( P )  ( ( const GLvoid * ) ( sizeof( GLuint ) * ( P         ) ) )
	// Draw the batched triangle list
	if (triangleCount > 0) { 
		glDrawElements(GL_TRIANGLES, triangleCount, GL_UNSIGNED_INT, OFFSETE(0));
	}
	
	// Draw the batched line list
	if (lineCount > 0) {
		glDrawElements(GL_LINES, lineCount, GL_UNSIGNED_INT, OFFSETE(triangleCount));
	}
	
	// Draw the batched point list
	if (pointCount > 0) {
		glDrawElements(GL_POINTS, pointCount, GL_UNSIGNED_INT, OFFSETE(lineCount + triangleCount));
	}
	
	glBindBufferARB( GL_ARRAY_BUFFER, 0 );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, 0 );
	
	glDisableClientState(GL_VERTEX_ARRAY);
    if (useTextures) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (useNormals) glDisableClientState(GL_NORMAL_ARRAY);
    if (useColors) glDisableClientState(GL_COLOR_ARRAY);
  }
};

vector<Mesh*> meshes;

namespace enigma_user
{

unsigned d3d_model_create()
{
  unsigned id = meshes.size();
  meshes.push_back(new Mesh());
  return id;
}

void d3d_model_destroy(int id)
{
  meshes[id]->Clear();
  delete meshes[id];
}

bool d3d_model_exists(int id)
{
  return (id >= 0 && (unsigned)id < meshes.size());
}

void d3d_model_clear(int id)
{
  meshes[id]->Clear();
}

void d3d_model_save(int id, string fname)
{
  //TODO: Write save code for meshes
}

bool d3d_model_load(int id, string fname)
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

void d3d_model_draw(int id) // overload for no additional texture or transformation call's
{
    meshes[id]->Draw();
}

void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z) // overload for no additional texture call's
{
    glTranslatef(x, y, z);
    meshes[id]->Draw();
    glTranslatef(-x, -y, -z);
}

void d3d_model_draw(int id, int texId)
{
    texture_use(get_texture(texId));
    meshes[id]->Draw();
}

void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int texId)
{
    texture_use(get_texture(texId));
    d3d_model_draw(id, x, y, z);
}

void d3d_model_primitive_begin(int id, int kind)
{
  meshes[id]->Begin(kind);
}

void d3d_model_primitive_end(int id)
{
  meshes[id]->End();
}

void d3d_model_vertex(int id, gs_scalar x, gs_scalar y, gs_scalar z)
{
  meshes[id]->AddVertex(x, y, z);
}

void d3d_model_vertex_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, int col, double alpha)
{
  meshes[id]->AddVertex(x, y, z);
  meshes[id]->AddColor(col, alpha);
}

void d3d_model_vertex_texture(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty)
{
  meshes[id]->AddVertex(x, y, z);
  meshes[id]->AddTexture(tx, ty);
}

void d3d_model_vertex_texture_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int col, double alpha)
{
  meshes[id]->AddVertex(x, y, z);
  meshes[id]->AddTexture(tx, ty);
  meshes[id]->AddColor(col, alpha);
}

void d3d_model_vertex_normal(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz)
{
  meshes[id]->AddVertex(x, y, z);
  meshes[id]->AddNormal(nx, ny, nz);
}

void d3d_model_vertex_normal_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int col, double alpha)
{
  meshes[id]->AddVertex(x, y, z);
  meshes[id]->AddNormal(nx, ny, nz);
  meshes[id]->AddColor(col, alpha);
}

void d3d_model_vertex_normal_texture(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty)
{
  meshes[id]->AddVertex(x, y, z);
  meshes[id]->AddNormal(nx, ny, nz);
  meshes[id]->AddTexture(tx, ty);
}

void d3d_model_vertex_normal_texture_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int col, double alpha)
{
  meshes[id]->AddVertex(x, y, z);
  meshes[id]->AddNormal(nx, ny, nz);
  meshes[id]->AddTexture(tx, ty);
  meshes[id]->AddColor(col, alpha);
}

void d3d_model_block(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed)
{
  GLfloat verts[] = { x1,y1,z1, x1,y1,z2, x1,y2,z1, x1,y2,z2, x2,y2,z1, x2,y2,z2, x2,y1,z1, x2,y1,z2, // sides
                     x1,y1,z1, x2,y1,z1, x1,y2,z1, x2,y2,z1,  // bottom
                     x1,y1,z2, x2,y1,z2, x1,y2,z2, x2,y2,z2 }, // top
          texts[] = { 0,vrep, 0,0, hrep,vrep, hrep,0,
					  0,vrep, 0,0, hrep,vrep, hrep,0,
                      0,0, hrep,0, 0,vrep, hrep,vrep,
                      0,0, hrep,0, 0,vrep, hrep,vrep },
		  norms[] = { -0.5,-0.5,-0.5, -0.5,-0.5,0.5, -0.5,0.5,-0.5, -0.5,0.5,0.5,
                       0.5,0.5,-0.5, 0.5,0.5,0.5, 0.5,-0.5,-0.5, 0.5,-0.5,0.5,
                      -0.5,-0.5,-0.5, 0.5,-0.5,-0.5, -0.5,0.5,-0.5, 0.5,0.5,-0.5, // bottom
                      -0.5,-0.5,0.5, 0.5,-0.5,0.5, -0.5,0.5,0.5, 0.5,0.5,0.5 }; // top
  GLuint inds[] = { 1,3,0, 3,2,0, 3,5,2, 5,4,2, 5,6,4, 5,7,6, 7,1,6, 0,6,1, // sides
                    11,9,8, 10,11,8, 12,13,15, 12,15,14 }; // top and bottom
  
  d3d_model_primitive_begin(id, pr_trianglestrip);
  
  for (int i = 0; i < 16; i++) {
	copy(&verts[i * 3], &verts[i * 3 + 3], back_inserter(meshes[id]->vertices));
	copy(&norms[i * 3], &norms[i * 3 + 3], back_inserter(meshes[id]->vertices));
	copy(&texts[i * 2], &texts[i * 2 + 2], back_inserter(meshes[id]->vertices));
	
	//meshes[id]->useColors = true;
    meshes[id]->useTextures = true;
    meshes[id]->useNormals = true;
  }
  
  // Supply the indices, the batcher will automatically offset them for us
  if (closed) {
    meshes[id]->indices.insert(meshes[id]->indices.end(), inds, inds + 36);
  } else {
    meshes[id]->indices.insert(meshes[id]->indices.end(), inds, inds + 24);
  }
  
  d3d_model_primitive_end(id);
}

void d3d_model_cylinder(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
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
        }

        if (closed)
        {
            v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
            t[k][0] = 0; t[k][1] = vrep;
            k++;
            for (int i = 0; i < steps*2; i+=2)
            {
                d3d_model_vertex_normal_texture(id, cx, cy, z1, 0, 0, -1, 0, vrep);
                d3d_model_vertex_normal_texture(id, v[i+3][0], v[i+3][1], v[i+3][2], 0, 0, -1, t[i+2][0], t[i+2][1]);
                d3d_model_vertex_normal_texture(id, v[i+1][0], v[i+1][1], v[i+1][2], 0, 0, -1, t[i][0], t[i][1]);
            }

            v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
            t[k][0] = 0; t[k][1] = vrep;
            k++;
            for (int i = 0; i < steps*2; i+=2)
            {
                d3d_model_vertex_normal_texture(id, cx, cy, z2, 0, 0, -1, 0, vrep);
                d3d_model_vertex_normal_texture(id, v[i][0], v[i][1], v[i][2], 0, 0, -1, t[i][0], t[i][1]);
                d3d_model_vertex_normal_texture(id, v[i+2][0], v[i+2][1], v[i+2][2], 0, 0, -1, t[i+2][0], t[i+2][1]);
            }
        }
}

void d3d_model_cone(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
  steps = min(max(steps, 3), 48);
  const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
  double a, px, py, tp;
  int k = 0;
  d3d_model_primitive_begin(id, pr_trianglefan);
  d3d_model_vertex_texture(id, cx, cy, z2, 0, 0);
  k++;
  a = 0; px = cx+rx; py = cy; tp = 0;
  for (int i = 0; i <= steps; i++)
  {
    d3d_model_vertex_texture(id, px, py, z1, tp, vrep);
    k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
  }
  d3d_model_primitive_end(id);
  if (closed)
  {
    d3d_model_primitive_begin(id, pr_trianglefan);
    d3d_model_vertex_texture(id, cx, cy, z1, 0, vrep);
    k++;
    tp = 0;
    for (int i = 0; i <= steps + 1; i++)
    {
      d3d_model_vertex_texture(id, px, py, z1, tp, 0);
      k++; a -= pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
    }
    d3d_model_primitive_end(id);
  }
}

void d3d_model_ellipsoid(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, int steps)
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
  d3d_model_primitive_begin(id, pr_trianglefan);
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
 d3d_model_primitive_end(id);
 for (int ii = 0; ii < zsteps - 2; ii++)
 {
    b += qr;
    cosb = cos(b);
	pz = rz*sin(b);
    tzp -= invstep2;
    d3d_model_primitive_begin(id, pr_trianglestrip);
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
    d3d_model_primitive_end(id);
  }
  d3d_model_primitive_begin(id, pr_trianglefan);
  v[k][0] = cx; v[k][1] = cy; v[k][2] = cz + rz;
  t[k][0] = 0; t[k][1] = 0;
  d3d_model_vertex_texture(id, cx, cy, cz + rz, 0, 0);
  k++;
  for (int i = k - steps - 2; i <= k - 2; i++)
  {
  d3d_model_vertex_texture(id, v[i][0], v[i][1], v[i][2], t[i][0], t[i][1]);
  }
}

void d3d_model_icosahedron(int id)
{
  //TODO: Write this shit
}

void d3d_model_torus(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI)
{
  int numc = csteps, numt = tsteps;

  for (int i = 0; i < numc; i++) {
    d3d_model_primitive_begin(id, pr_trianglestrip); // quads are deprecated all basic shapes on models are batched into primitive 0 as triangle list
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
    d3d_model_primitive_end(id);
  }
}

void d3d_model_wall(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep)
{
  float xd = x2-x1, yd = y2-y1, zd = z2-z1;
  float normal[3] = {xd*zd, zd*yd, 0};
  float mag = hypot(normal[0], normal[1]);
  normal[0] /= mag;
  normal[1] /= mag;

  d3d_model_primitive_begin(id, pr_trianglestrip);
  d3d_model_vertex_normal_texture(id, x1, y1, z1, normal[0], normal[1], normal[2], 0, 0);
  d3d_model_vertex_normal_texture(id, x2, y2, z1, normal[0], normal[1], normal[2], hrep, 0);
  d3d_model_vertex_normal_texture(id, x1, y1, z2, normal[0], normal[1], normal[2], 0, vrep);
  d3d_model_vertex_normal_texture(id, x2, y2, z2, normal[0], normal[1], normal[2], hrep, vrep);
  d3d_model_primitive_end(id);
}

void d3d_model_floor(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep)
{
  GLfloat normal[] = {0, 0, 1};

  d3d_model_primitive_begin(id, pr_trianglestrip);
  d3d_model_vertex_normal_texture(id, x1, y1, z1, normal[0], normal[1], normal[2], 0, 0);
  d3d_model_vertex_normal_texture(id, x2, y1, z2, normal[0], normal[1], normal[2], 0, vrep);
  d3d_model_vertex_normal_texture(id, x1, y2, z1, normal[0], normal[1], normal[2], hrep, 0);
  d3d_model_vertex_normal_texture(id, x2, y2, z2, normal[0], normal[1], normal[2], hrep, vrep);
  d3d_model_primitive_end(id);
}

}