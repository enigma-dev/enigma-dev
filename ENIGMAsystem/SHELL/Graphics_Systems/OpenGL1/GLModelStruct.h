/** Copyright (C) 2008-2013 Robert B. Colton, Adriano Tumminelli
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
#include "GLshapes.h"
#include "../General/GSprimitives.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


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
  
  //split a string and convert to float
  vector<float> float_split(const string& str, const char& ch) {
    string next;
    vector<float> result;

    for (string::const_iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it == ch)
		{
			if (!next.empty())
			{
				result.push_back(atof(next.c_str()));
				next.clear();
			}
        } else {
            next += *it;
        }
    }
    if (!next.empty())
         result.push_back(atof(next.c_str()));
    return result;
  }

  //obj model parsing functions
  void string_parse( string *s )
  {
	size_t spaces = 0;
	bool trimmed = false;
	bool checknormal = false;
	for (unsigned int i = 0; i < s->size() ; i++)
	{ 
		//comment
		if ((*s)[i] == '#')
		{
			s->erase(i, s->length() - i);
			break;
		}
		else if((*s)[i] == ' ')
		{
			if (!trimmed)
			{
				s->erase(i,1);
				i--;
			}
			else
			{
				if (spaces >= 1)
				{
					s->erase(i,1);
					i--;
				}
				spaces++;
			}
		}
		else
		{
			if((*s)[i] == '/')
			{
				(*s)[i] = ' ';
				if(checknormal)
				{
					s->erase(i, 1);
					checknormal = false;
				}
				else
					checknormal = true;
			}
			else
				checknormal = false;
			spaces = 0;
			trimmed = true;
		}
	}
	//end trim
	if (s->size() > 0) {
		if ((*s)[s->size()-1] == ' ')
		{
			s->erase(s->size()-1, 1);
		}
	}
  }
}

union VertexElement {
	unsigned long d;
	gs_scalar f;
	
	VertexElement(gs_scalar v): f(v) {}
	VertexElement(unsigned long v): d(v) {}
};

class Mesh
{
  public:
  unsigned currentPrimitive; // The type of the current primitive being added to the model

  vector<VertexElement> vertices; // Temporary vertices container for the current primitive until they are batched
  vector<GLuint> indices; // Temporary indices that can optionally be supplied, otherwise they will get generated by the batcher.
  vector<VertexElement> triangleVertices; // The vertices added to triangle primitives batched into a single triangle list to be sent to the GPU
  vector<VertexElement> triangleIndexedVertices; // The vertices added to indexed triangle primitives batched into a single triangle list to be sent to the GPU
  vector<GLuint> triangleIndices; // The triangle indices either concatenated by batching or supplied in the temporary container.
  vector<VertexElement> lineVertices; // The vertices added to line primitives batched into a single line list to be sent to the GPU
  vector<VertexElement> lineIndexedVertices; // The vertices added to indexed line primitives batched into a single line list to be sent to the GPU
  vector<GLuint> lineIndices; // The line indices either concatenated by batching or supplied in the temporary container.
  vector<VertexElement> pointVertices; // The vertices added to point primitives batched into a single point list to be sent to the GPU
  vector<VertexElement> pointIndexedVertices; // The vertices added to indexed point primitives batched into a single point list to be sent to the GPU
  vector<GLuint> pointIndices; // The point indices either concatenated by batching or supplied in the temporary container.
  
  unsigned vertexStride; // whether the vertices are 2D or 3D
  bool useColors; // If colors have been added to the model
  bool useTextures; // If texture coordinates have been added
  bool useNormals; // If normals have been added
  
  unsigned pointCount; // The number of vertices in the point container
  unsigned triangleCount; // The number of vertices in the triangle container
  unsigned lineCount; // The number of vertices in the line container
  
  unsigned pointIndexedCount; // The number of point indices
  unsigned triangleIndexedCount; // The number of triangle indices
  unsigned lineIndexedCount; // The number of line indices
  
  //NOTE: OpenGL 1.1 models are always dynamic since they utilize vertex arrays for software vertex processing and backwards compatibility.
  Mesh (bool dynamic) {
	vertexStride = 0;
    useColors = false;
    useTextures = false;
    useNormals = false;
	
	pointCount = 0;
	triangleCount = 0;
	lineCount = 0;
	
	pointIndexedCount = 0;
	triangleIndexedCount = 0;
	lineIndexedCount = 0;
	
    currentPrimitive = 0;
	
	triangleIndexedVertices.reserve(64000);
	pointIndexedVertices.reserve(64000);
	lineIndexedVertices.reserve(64000);
	pointVertices.reserve(64000);
	pointIndices.reserve(64000);
	lineVertices.reserve(64000);
	lineIndices.reserve(64000);
	triangleVertices.reserve(64000);
	triangleIndices.reserve(64000);
	vertices.reserve(64000);
	indices.reserve(64000);
  }

  ~Mesh() {

  }

  void ClearData() {
    triangleVertices.clear();
	pointVertices.clear();
	lineVertices.clear();
	triangleIndexedVertices.clear();
	pointIndexedVertices.clear();
	lineIndexedVertices.clear();
	triangleIndices.clear();
	pointIndices.clear();
	lineIndices.clear();
  }

  void Clear() {
    ClearData();
	
	triangleIndexedVertices.reserve(64000);
	pointIndexedVertices.reserve(64000);
	lineIndexedVertices.reserve(64000);
	pointVertices.reserve(64000);
	pointIndices.reserve(64000);
	lineVertices.reserve(64000);
	lineIndices.reserve(64000);
	triangleVertices.reserve(64000);
	triangleIndices.reserve(64000);
	vertices.reserve(64000);
	indices.reserve(64000);
	
	vertexStride = 0;
	useColors = false;
    useTextures = false;
    useNormals = false;
	
	pointCount = 0;
	triangleCount = 0;
	lineCount = 0;
	pointIndexedCount = 0;
	triangleIndexedCount = 0;
	lineIndexedCount = 0;
  }
  
  GLsizei GetStride() {
  	GLsizei stride = vertexStride;
    if (useNormals) stride += 3;
	if (useTextures) stride += 2;
    if (useColors) stride += 1;
	return stride;
  }
  
  void Begin(int pt)
  {
    currentPrimitive = pt;
  }
  
  void AddVertex(gs_scalar x, gs_scalar y)
  {
    vertices.push_back(x); vertices.push_back(y);
	vertexStride = 2;
  }

  void AddVertex(gs_scalar x, gs_scalar y, gs_scalar z)
  {
    vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
	vertexStride = 3;
  }
  
  void AddIndex(unsigned ind)
  {
    indices.push_back(ind);
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

  void AddColor(int col, double alpha)
  {               
	unsigned long final = col + ((unsigned char)alpha*255 << 24);
	vertices.push_back(final); 
	useColors = true;
  }
  
  void Translate(gs_scalar x, gs_scalar y, gs_scalar z)
  {
	unsigned int stride = GetStride();
	unsigned int size = triangleVertices.size();
	for (unsigned int i = 0; i < size; i += stride)
	{
		triangleVertices[i].f += x;
		triangleVertices[i+1].f += y;
		triangleVertices[i+2].f += z;
	}
  }
     
  void RotateUV(gs_scalar angle)
  {
	unsigned int stride = GetStride();
	angle *= 3.14159/180.0;
	gs_scalar _cos = cos(angle);
	gs_scalar _sin = sin(angle);
	unsigned int size = triangleVertices.size();
	for (unsigned int i = 0; i < size; i += stride)
	{
		gs_scalar x = triangleVertices[i + 3 + 3*useNormals].f;
		gs_scalar y = triangleVertices[i + 4 + 3*useNormals].f;
		triangleVertices[i + 3 + 3*useNormals].f = x*_cos - y*_sin;
		triangleVertices[i + 4 + 3*useNormals].f = x*_sin - y*_cos;
	}
  }
  
  /*
  void ScaleUV(gs_scalar xscale, gs_scalar yscale)
  {
	unsigned int stride = GetStride();

	for (vector<gs_scalar>::iterator i = triangleVertices.begin(); i != triangleVertices.end(); i += stride)
	{
		*(i + 3 + 3*useNormals) *= xscale;
		*(i + 4 + 3*useNormals) *= yscale;
	}
  }
  */
   
  void RotateX(gs_scalar angle)
  {
	unsigned int stride = GetStride();
	angle *= 3.14159/180.0;
	gs_scalar _cos = cos(angle);
	gs_scalar _sin = sin(angle);
	unsigned int size = triangleVertices.size();
	for (unsigned int i = 0; i < size; i += stride)
	{
		gs_scalar y = triangleVertices[i+1].f;
		gs_scalar z = triangleVertices[i+2].f;
		triangleVertices[i+1].f = y*_cos - z*_sin;
		triangleVertices[i+2].f = y*_sin - z*_cos;
	}
  }
  
  
  void RotateY(gs_scalar angle)
  {
	unsigned int stride = GetStride();
	angle *= 3.14159/180.0;
	gs_scalar _cos = cos(angle);
	gs_scalar _sin = sin(angle);
	unsigned int size = triangleVertices.size();
	for (unsigned int i = 0; i < size; i += stride)
	{
		gs_scalar x = triangleVertices[i].f;
		gs_scalar z = triangleVertices[i+2].f;
		triangleVertices[i].f = z*_sin - x*_cos;
		triangleVertices[i+2].f = z*_cos - x*_sin;
	}
  }
  
  void RotateZ(gs_scalar angle)
  {
	unsigned int stride = GetStride();
	angle *= 3.14159/180.0;
	gs_scalar _cos = cos(angle);
	gs_scalar _sin = sin(angle);
	unsigned int size = triangleVertices.size();
	for (unsigned int i = 0; i < size; i += stride)
	{
		gs_scalar x = triangleVertices[i].f;
		gs_scalar y = triangleVertices[i+1].f;
		triangleVertices[i].f = x*_cos - y*_sin;
		triangleVertices[i+1].f = x*_sin - y*_cos;
	}
  }
  
  /*
  void Scale(gs_scalar xscale, gs_scalar yscale, gs_scalar zscale)
  {
	unsigned int stride = GetStride();

	for (vector<gs_scalar>::iterator i = triangleVertices.begin(); i != triangleVertices.end(); i += stride)
	{
		*(i+0) *= xscale;
		*(i+1) *= yscale;
		*(i+2) *= zscale;
	}
  }
  */
  /*
  bool CalculateNormals(bool smooth, bool invert)
  {
	unsigned int stride = GetStride();
	
	int oft = useNormals * 3;
	int ofc = oft + useTextures * 2 ;
	vector<gs_scalar> tempVertices;

	for (vector<gs_scalar>::const_iterator i = triangleVertices.begin(); i != triangleVertices.end(); i += stride*3)
	{
		gs_scalar x1 = *i;
		gs_scalar y1 = *(i+1);
		gs_scalar z1 = *(i+2);

		gs_scalar x2 = *(i +stride);
		gs_scalar y2 = *(i+1 +stride);
		gs_scalar z2 = *(i+2 +stride);
		
		gs_scalar x3 = *(i +stride*2);
		gs_scalar y3 = *(i+1 +stride*2);
		gs_scalar z3 = *(i+2 +stride*2);
		
		gs_scalar nX = (y2-y1)*(z3-z1)-(y3-y1)*(z2-z1);
		gs_scalar nY = (z2-z1)*(x3-x1)-(z3-z1)*(x2-x1);
		gs_scalar nZ = (x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
		gs_scalar  m = sqrt(nX*nX + nY*nY + nZ*nZ);
		nX /= m;
		nY /= m;
		nZ /= m;
		
		for(int n = 0; n < 3; n++)
		{
			int v = n*stride;
			//add position
			tempVertices.push_back(*(i+0 + v));
			tempVertices.push_back(*(i+1 + v));
			tempVertices.push_back(*(i+2 + v));
			//add normals
			if (invert) {
				tempVertices.push_back(nX * -1);
				tempVertices.push_back(nY * -1);
				tempVertices.push_back(nZ * -1);
			} else {
				tempVertices.push_back(nX);
				tempVertices.push_back(nY);
				tempVertices.push_back(nZ);
			}
			//add texture
			if(useTextures){
				tempVertices.push_back(*(i+3+oft + v));
				tempVertices.push_back(*(i+4+oft + v));
			}
			//add color
			if(useColors){
				tempVertices.push_back(*(i+5+ofc + v));
				tempVertices.push_back(*(i+6+ofc + v));
				tempVertices.push_back(*(i+7+ofc + v));
				tempVertices.push_back(*(i+8+ofc + v));
			}
		}
	}
	triangleVertices = tempVertices;
	useNormals = true;
	if(smooth) SmoothNormals();
	return true;
  }
  */
  /*
  void SmoothNormals()
  {
	unsigned int stride = GetStride();
	
	vector<vector<unsigned int> > groupList;
	unsigned int n = 0;
	//group all vertices
	for (vector<gs_scalar>::const_iterator i = triangleVertices.begin(); i != triangleVertices.end(); i += stride)
	{
		gs_scalar x1 = *(i+0);
		gs_scalar y1 = *(i+1);
		gs_scalar z1 = *(i+2);
		
		bool added = false;
		//check each group 
		if (groupList.size() > 0)
		for (vector< vector<unsigned int> >::iterator ig = groupList.begin(); ig != groupList.end(); ++ig)
		{
			
			//compute first element and add it if has the same position
			unsigned int index = (*ig)[0];
			gs_scalar x2 = triangleVertices[index*stride + 0];
			gs_scalar y2 = triangleVertices[index*stride + 1];
			gs_scalar z2 = triangleVertices[index*stride + 2]; 
			if( x1 == x2 && y1 == y2 && z1 == z2)
			{
				added = true;
				(*ig).push_back(n);
				break;
			}
			 
		}
		if (!added)
		{
			vector<unsigned int> vec = vector<unsigned int>();
			vec.push_back(n);
			groupList.push_back(vec);
		}
		 
		n++;
	}
	
	//add average values
	for (vector< vector<unsigned int> >::iterator ig = groupList.begin(); ig != groupList.end(); ++ig)
	{
		gs_scalar count = 0;
		gs_scalar anx = 0, any = 0, anz = 0;
		for (vector<unsigned int>::iterator i = (*ig).begin(); i != (*ig).end(); ++i)
		{
			anx += triangleVertices[(*i)*stride+3];
			any += triangleVertices[(*i)*stride+4];
			anz += triangleVertices[(*i)*stride+5];
			
			count++;
		}
		anx /= count;
		any /= count;
		anz /= count;
		
		for (vector<unsigned int>::iterator i = (*ig).begin(); i != (*ig).end(); ++i)
		{
			triangleVertices[(*i)*stride+3] = anx;
			triangleVertices[(*i)*stride+4] = any;
			triangleVertices[(*i)*stride+5] = anz;
		}
	}
  }
  */
  
  void End()
  {
	//NOTE: This batching only checks for degenerate primitives on triangle strips and fans since the GPU does not render triangles where the two
	//vertices are exactly the same, triangle lists could also check for degenerates, it is unknown whether the GPU will render a degenerative 
	//in a line strip primitive.
	
	unsigned stride = GetStride();
	
	// Primitive has ended so now we need to batch the vertices that were given into single lists, eg. line list, triangle list, point list
	// Indices are optionally supplied, model functions can also be added for the end user to supply the indexing themselves for each primitive
	// but the batching system does not care either way if they are not supplied it will automatically generate them.
	switch (currentPrimitive) {
		case enigma_user::pr_pointlist:
			if (indices.size() > 0) {
				pointIndexedVertices.insert(pointIndexedVertices.end(), vertices.begin(), vertices.end());
				pointIndexedCount += vertices.size() / stride;
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += pointCount; }
				pointIndices.insert(pointIndices.end(), indices.begin(), indices.end());
			} else {
				pointVertices.insert(pointVertices.end(), vertices.begin(), vertices.end());
				pointCount += vertices.size() / stride;
			}
			break;
		case enigma_user::pr_linelist:
			if (indices.size() > 0) {
				lineIndexedVertices.insert(lineIndexedVertices.end(), vertices.begin(), vertices.end());
				lineIndexedCount += vertices.size() / stride;
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += lineCount; }
				lineIndices.insert(lineIndices.end(), indices.begin(), indices.end());
			} else {
				lineVertices.insert(lineVertices.end(), vertices.begin(), vertices.end());
				lineCount += vertices.size() / stride;
			}
			break;
		case enigma_user::pr_linestrip:
			lineIndexedVertices.insert(lineIndexedVertices.end(), vertices.begin(), vertices.end());
			if (indices.size() > 0) {
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += lineIndexedCount; }
				for (unsigned i = 0; i < indices.size() - 2; i++) {
					lineIndices.push_back(indices[i]);
					lineIndices.push_back(indices[i + 1]);
				}
			} else {
				for (unsigned i = 0; i < vertices.size() / stride - 1; i++) {
					lineIndices.push_back(lineIndexedCount + i);
					lineIndices.push_back(lineIndexedCount + i + 1);
				}
			}
			lineIndexedCount += vertices.size() / stride;
			break;
		case enigma_user::pr_trianglelist:
			if (indices.size() > 0) {
				triangleIndexedVertices.insert(triangleIndexedVertices.end(), vertices.begin(), vertices.end());
				triangleIndexedCount += vertices.size() / stride;
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += triangleCount; }
				triangleIndices.insert(triangleIndices.end(), indices.begin(), indices.end());
			} else {
				triangleVertices.insert(triangleVertices.end(), vertices.begin(), vertices.end());
				triangleCount += vertices.size() / stride;
			}
			break;
		case enigma_user::pr_trianglestrip:
			triangleIndexedVertices.insert(triangleIndexedVertices.end(), vertices.begin(), vertices.end());
			if (indices.size() > 0) {
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += triangleIndexedCount; }
				for (unsigned i = 0; i < indices.size() - 2; i++) {
					// check for and continue if indexed triangle is degenerate, because the GPU won't render it anyway
					if (indices[i] == indices[i + 1] || indices[i] == indices[i + 2]  || indices[i + 1] == indices[i + 2] ) { continue; }
					triangleIndices.push_back(indices[i]);
					triangleIndices.push_back(indices[i+1]);
					triangleIndices.push_back(indices[i+2]);
				}
			} else {
				for (unsigned i = 0; i < vertices.size() / stride - 2; i++) {
					if (i % 2) {
						triangleIndices.push_back(triangleIndexedCount + i + 2);
						triangleIndices.push_back(triangleIndexedCount + i + 1);
						triangleIndices.push_back(triangleIndexedCount + i);
					} else {
						triangleIndices.push_back(triangleIndexedCount + i);
						triangleIndices.push_back(triangleIndexedCount + i + 1);
						triangleIndices.push_back(triangleIndexedCount + i + 2);
					}
				}
			}
			triangleIndexedCount += vertices.size() / stride;
			break;
		case enigma_user::pr_trianglefan:
			triangleIndexedVertices.insert(triangleIndexedVertices.end(), vertices.begin(), vertices.end());
			if (indices.size() > 0) {
				for (std::vector<GLuint>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += triangleIndexedCount; }
				for (unsigned i = 1; i < indices.size() - 1; i++) {
					// check for and continue if indexed triangle is degenerate, because the GPU won't render it anyway
					if (indices[0] == indices[i] || indices[0] == indices[i + 1]  || indices[i] == indices[i + 1] ) { continue; }
					triangleIndices.push_back(indices[0]);
					triangleIndices.push_back(indices[i]);
					triangleIndices.push_back(indices[i + 1]);
				}
			} else {
				for (unsigned i = 1; i < vertices.size() / stride - 1; i++) {
					triangleIndices.push_back(triangleIndexedCount);
					triangleIndices.push_back(triangleIndexedCount + i);
					triangleIndices.push_back(triangleIndexedCount + i + 1);
				}
			}
			triangleIndexedCount += vertices.size() / stride;
			break;
	}

	// Clean up the temporary vertex and index containers now that they have been batched efficiently.
	vertices.clear();
	indices.clear();
  }
  
  void DrawElements(GLenum mode, vector<VertexElement> &verts, vector<GLuint> &inds, GLsizei count) {
	if (!count) {
		return;
	}
  
    // Calculate the number of bytes to get to the next vertex
	GLsizei stride = GetStride() * sizeof( gs_scalar );
	
	#define OFFSET( P )  ( char* ) ( &verts[0] ) + ( ( sizeof( gs_scalar ) * ( P         ) ) ) 
	GLsizei STRIDE = stride;

	// Enable vertex array's for fast vertex processing
	glEnableClientState(GL_VERTEX_ARRAY);
	unsigned offset = 0;
	glVertexPointer( vertexStride, GL_FLOAT, STRIDE, OFFSET(offset) ); // Set the vertex pointer
	offset += vertexStride;
	
    if (useNormals){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer( GL_FLOAT, STRIDE, OFFSET(offset) ); // Set the normal pointer to the offset in the array
		offset += 3;
    }

	if (useTextures){
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 2, GL_FLOAT, STRIDE,  OFFSET(offset) ); // Set the texture pointer to the offset in the array
		offset += 2;
	}
	
    if (useColors){
		glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer( 4, GL_UNSIGNED_BYTE, STRIDE, OFFSET(offset)); // Set the color pointer to the offset in the array
    }
	
	glDrawElements(mode, count, GL_UNSIGNED_INT, &inds[0]);
  }
  
  void DrawArrays(GLenum mode, vector<VertexElement> &verts, GLsizei count) {
	if (!count) {
		return;
	}
  
    // Calculate the number of bytes to get to the next vertex
	GLsizei stride = GetStride() * sizeof( gs_scalar );
	
	#define OFFSET( P )  ( char* ) ( &verts[0] ) + ( ( sizeof( gs_scalar ) * ( P         ) ) ) 
	GLsizei STRIDE = stride;

	// Enable vertex array's for fast vertex processing
	glEnableClientState(GL_VERTEX_ARRAY);
	unsigned offset = 0;
	glVertexPointer( vertexStride, GL_FLOAT, STRIDE, OFFSET(offset) ); // Set the vertex pointer
	offset += vertexStride;
	
    if (useNormals){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer( GL_FLOAT, STRIDE, OFFSET(offset) ); // Set the normal pointer to the offset in the array
		offset += 3;
    }

	if (useTextures){
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 2, GL_FLOAT, STRIDE,  OFFSET(offset) ); // Set the texture pointer to the offset in the array
		offset += 2;
	}
	
    if (useColors){
		glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer( 4, GL_UNSIGNED_BYTE, STRIDE, OFFSET(offset)); // Set the color pointer to the offset in the array
    }
	
	glDrawArrays(mode, 0, count);
  }

  void Draw()
  {
	// Draw the batched and indexed primitives
	if (triangleIndexedCount > 0) { 
		DrawElements(GL_TRIANGLES, triangleIndexedVertices, triangleIndices, triangleIndices.size());
	}
	if (lineIndexedCount > 0) {
		DrawElements(GL_LINES, lineIndexedVertices, lineIndices, lineIndices.size());
	}
	if (pointIndexedCount > 0) {
	    DrawElements(GL_POINTS, pointIndexedVertices, pointIndices, pointIndices.size());
	}
	
	// Draw the unbatched and unindexed primitives
	if (triangleCount > 0) { 
		DrawArrays(GL_TRIANGLES, triangleVertices, triangleCount);
	}
	if (lineCount > 0) {
		DrawArrays(GL_LINES, lineVertices, lineCount);
	}
	if (pointCount > 0) {
	    DrawArrays(GL_POINTS, pointVertices, pointCount);
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
    if (useTextures) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (useNormals) glDisableClientState(GL_NORMAL_ARRAY);
    if (useColors) glDisableClientState(GL_COLOR_ARRAY);
  }
};

extern vector<Mesh*> meshes;