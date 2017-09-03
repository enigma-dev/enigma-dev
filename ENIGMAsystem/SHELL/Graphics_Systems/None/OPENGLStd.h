/** Copyright (C) 2017 Faissal I. Bensefia
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

#include <string>
#include <vector>
#include "../General/GScolors.h"
#include "../General/GSprimitives.h"
#include "../General/GSd3d.h"
#include "../General/GSstdraw.h"
#include "../General/GSblend.h"
#include "../General/GSsurface.h"
#include "../General/GSscreen.h"
#include "Universal_System/scalar.h"

namespace enigma
{
  extern unsigned char currentcolor[4];

  extern int currentblendmode[2];
  extern int currentblendtype;
  extern bool glew_isgo;
  extern bool pbo_isgo;
	unsigned get_texture(int texid);
	void graphics_samplers_apply();

	template<int x> struct intmatch {};
	template<int x> struct uintmatch {};
	template<> struct intmatch<1> {typedef int8_t type;};
	template<> struct intmatch<2> {typedef int16_t type;};
	template<> struct intmatch<4> {typedef int32_t type;};
	template<> struct intmatch<8> {typedef int64_t type;};
	template<> struct uintmatch<1> {typedef uint8_t type;};
	template<> struct uintmatch<2> {typedef uint16_t type;};
	template<> struct uintmatch<4> {typedef uint32_t type;};
	template<> struct uintmatch<8> {typedef uint64_t type;};
	typedef uintmatch<sizeof(gs_scalar)>::type color_t;
	union VertexElement {
		color_t d;
		gs_scalar f;

		VertexElement(gs_scalar v): f(v) {}
		VertexElement(color_t v): d(v) {}
	};
	//TODO: Implement mesh class (so we can manipulate 3D stuffz even without graphics)
	class Mesh
	{
		public:
		unsigned currentPrimitive; //The type of the current primitive being added to the model
		unsigned stride; // Stride computed using GetStride(). After adding custom attributes that involves loops it is better to precalculate

		vector<VertexElement> vertices; // Temporary vertices container for the current primitive until they are batched
	  vector<unsigned> indices; // Temporary indices that can optionally be supplied, otherwise they will get generated by the batcher.
	  vector<VertexElement> triangleVertices; // The vertices added to triangle primitives batched into a single triangle list to be buffered to the GPU
	  vector<VertexElement> triangleIndexedVertices; // The vertices added to indexed triangle primitives batched into a single triangle list to be buffered to the GPU
	  vector<unsigned> triangleIndices; // The triangle indices either concatenated by batching or supplied in the temporary container.
	  vector<VertexElement> lineVertices; // The vertices added to line primitives batched into a single line list to be buffered to the GPU
	  vector<VertexElement> lineIndexedVertices; // The vertices added to indexed line primitives batched into a single line list to be buffered to the GPU
	  vector<unsigned> lineIndices; // The line indices either concatenated by batching or supplied in the temporary container.
	  vector<VertexElement> pointVertices; // The vertices added to point primitives batched into a single point list to be buffered to the GPU
	  vector<VertexElement> pointIndexedVertices; // The vertices added to indexed point primitives batched into a single point list to be buffered to the GPU
	  vector<unsigned> pointIndices; // The point indices either concatenated by batching or supplied in the temporary container.

		unsigned vertexStride; // Whether the vertices are 2D or 3D
	  bool useDepth; // Whether or not the Z-values should be treated as a depth component
	  bool useColors; // If colors have been added to the model
	  bool useTextures; // If texture coordinates have been added
	  bool useNormals; // If normals have been added

	  unsigned pointCount; // The number of indices in the point buffer
	  unsigned triangleCount; // The number of indices in the triangle buffer
	  unsigned triangleVertCount; // The number of vertices in the triangle buffer
	  unsigned lineCount; // The number of indices in the line buffer
	  unsigned lineVertCount; //The number of vertices in the line buffer

	  unsigned indexedoffset; // The number of indexed vertices
	  unsigned pointIndexedCount; // The number of point indices
	  unsigned triangleIndexedCount; // The number of triangle indices
	  unsigned lineIndexedCount; // The number of line indices

		bool vbobuffered; // Whether or not the buffer objects have been generated

		Mesh (int type)
		{
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

	    vbobuffered = false;

	    vertexStride = 0;
	    useDepth = false;
	    useColors = false;
	    useTextures = false;
	    useNormals = false;

	    pointCount = 0;
	    triangleCount = 0;
	    triangleVertCount = 0;
	    lineCount = 0;
	    lineVertCount = 0;

	    indexedoffset = 0;
	    pointIndexedCount = 0;
	    triangleIndexedCount = 0;
	    lineIndexedCount = 0;

	    currentPrimitive = 0;
		}

		~Mesh()
		{
		}

		void ClearData()
		{
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

		void Clear()
		{
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

	    vbobuffered = false;

	    vertexStride = 0;
	    useColors = false;
	    useTextures = false;
	    useNormals = false;

	    pointCount = 0;
	    triangleCount = 0;
	    triangleVertCount = 0;
	    lineCount = 0;
	    lineVertCount = 0;

	    indexedoffset = 0;
	    pointIndexedCount = 0;
	    triangleIndexedCount = 0;
	    lineIndexedCount = 0;
		}

		unsigned GetStride() {
	    unsigned stride = vertexStride;
	    if (useNormals) stride += 3;
	    if (useTextures) stride += 2;
	    if (useColors) stride += 1;
	    return stride;
	  }

		void Begin(int pt)
		{
			vbobuffered = false;
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
			color_t finalcol = col + ((unsigned char)(alpha*255) << 24);
			vertices.push_back(finalcol);
			useColors = true;
		}

		void AddFloat(gs_scalar val)
		{
			vertices.push_back(val);
		}

		void AddFloat2(gs_scalar val, gs_scalar val2)
		{
			vertices.push_back(val); vertices.push_back(val2);
		}

		void AddFloat3(gs_scalar val, gs_scalar val2, gs_scalar val3)
		{
			vertices.push_back(val); vertices.push_back(val2); vertices.push_back(val3);
		}

		void AddFloat4(gs_scalar val, gs_scalar val2, gs_scalar val3, gs_scalar val4)
		{
			vertices.push_back(val); vertices.push_back(val2); vertices.push_back(val3); vertices.push_back(val4);
		}

		void AddUbyte4(uint8_t val, uint8_t val2, uint8_t val3, uint8_t val4)
		{
			uint32_t c = (val << 24) | (val2 << 16) | (val3 << 8) | val4;
			color_t s = color_t(c);
			vertices.push_back(s);
		}

		void End()
		{
			//NOTE: This batching only checks for degenerate primitives on triangle strips and fans since the GPU does not render triangles where the two
	    //vertices are exactly the same, triangle lists could also check for degenerates, it is unknown whether the GPU will render a degenerative
	    //in a line strip primitive.

	    unsigned stride = GetStride();
	    if (vertices.size() == 0) return;

	    // Primitive has ended so now we need to batch the vertices that were given into single lists, eg. line list, triangle list, point list
	    // Indices are optionally supplied, model functions can also be added for the end user to supply the indexing themselves for each primitive
	    // but the batching system does not care either way if they are not supplied it will automatically generate them.
	    switch (currentPrimitive) {
	      case enigma_user::pr_pointlist:
	        if (indices.size() > 0) {
	          pointIndexedVertices.insert(pointIndexedVertices.end(), vertices.begin(), vertices.end());
	          for (std::vector<unsigned>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += pointIndexedCount; }
	          pointIndices.insert(pointIndices.end(), indices.begin(), indices.end());
	        } else {
	          pointVertices.insert(pointVertices.end(), vertices.begin(), vertices.end());
	          pointCount += vertices.size() / stride;
	        }
	        break;
	      case enigma_user::pr_linelist:
	        if (indices.size() > 0) {
	          lineIndexedVertices.insert(lineIndexedVertices.end(), vertices.begin(), vertices.end());
	          for (std::vector<unsigned>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += lineIndexedCount; }
	          lineIndices.insert(lineIndices.end(), indices.begin(), indices.end());
	        } else {
	          lineVertices.insert(lineVertices.end(), vertices.begin(), vertices.end());
	          lineCount += vertices.size() / stride;
	        }
	        break;
	      case enigma_user::pr_linestrip:
	        lineIndexedVertices.insert(lineIndexedVertices.end(), vertices.begin(), vertices.end());
	        if (indices.size() > 0) {
	          for (std::vector<unsigned>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += lineIndexedCount; }
	          for (unsigned i = 0; i < indices.size() - 2; i++) {
	            lineIndices.push_back(indices[i]);
	            lineIndices.push_back(indices[i + 1]);
	          }
	        } else {
	          unsigned offset = (lineIndexedVertices.size() - vertices.size()) / stride;
	          for (unsigned i = 0; i < vertices.size() / stride - 1; i++) {
	            lineIndices.push_back(offset + i);
	            lineIndices.push_back(offset + i + 1);
	          }
	        }
	        break;
	      case enigma_user::pr_trianglelist:
	        if (indices.size() > 0) {
	          triangleIndexedVertices.insert(triangleIndexedVertices.end(), vertices.begin(), vertices.end());
	          for (std::vector<unsigned>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += triangleIndexedCount; }
	          triangleIndices.insert(triangleIndices.end(), indices.begin(), indices.end());
	        } else {
	          triangleVertices.insert(triangleVertices.end(), vertices.begin(), vertices.end());
	          triangleCount += vertices.size() / stride;
	        }
	        break;
	      case enigma_user::pr_trianglestrip:
	        triangleIndexedVertices.insert(triangleIndexedVertices.end(), vertices.begin(), vertices.end());
	        if (indices.size() > 0) {
	          for (std::vector<unsigned>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += triangleIndexedCount; }
	          for (unsigned i = 0; i < indices.size() - 2; i++) {
	            // check for and continue if indexed triangle is degenerate, because the GPU won't render it anyway
	            if (indices[i] == indices[i + 1] || indices[i] == indices[i + 2]  || indices[i + 1] == indices[i + 2] ) { continue; }
	            triangleIndices.push_back(indices[i]);
	            triangleIndices.push_back(indices[i+1]);
	            triangleIndices.push_back(indices[i+2]);
	          }
	        } else {
	          unsigned offset = (triangleIndexedVertices.size() - vertices.size()) / stride;
	          unsigned elements = (vertices.size() / stride - 2);

	          for (unsigned i = 0; i < elements; i++) {
	            if (i % 2) {
	              triangleIndices.push_back(offset + i + 2);
	              triangleIndices.push_back(offset + i + 1);
	              triangleIndices.push_back(offset + i);
	            } else {
	              triangleIndices.push_back(offset + i);
	              triangleIndices.push_back(offset + i + 1);
	              triangleIndices.push_back(offset + i + 2);
	            }
	          }
	        }
	        break;
	      case enigma_user::pr_trianglefan:
	        triangleIndexedVertices.insert(triangleIndexedVertices.end(), vertices.begin(), vertices.end());
	        if (indices.size() > 0) {
	          for (std::vector<unsigned>::iterator it = indices.begin(); it != indices.end(); ++it) { *it += triangleIndexedCount; }
	          for (unsigned i = 1; i < indices.size() - 1; i++) {
	            // check for and continue if indexed triangle is degenerate, because the GPU won't render it anyway
	            if (indices[0] == indices[i] || indices[0] == indices[i + 1]  || indices[i] == indices[i + 1] ) { continue; }
	            triangleIndices.push_back(indices[0]);
	            triangleIndices.push_back(indices[i]);
	            triangleIndices.push_back(indices[i + 1]);
	          }
	        } else {
	          unsigned offset = (triangleIndexedVertices.size() - vertices.size()) / stride;
	          for (unsigned i = 1; i < vertices.size() / stride - 1; i++) {
	            triangleIndices.push_back(offset);
	            triangleIndices.push_back(offset + i);
	            triangleIndices.push_back(offset + i + 1);
	          }
	        }
	        break;
	    }

	    // Clean up the temporary vertex and index containers now that they have been batched efficiently
	    vertices.clear();
	    indices.clear();
		}
	};

	extern vector<Mesh*> meshes;
}

namespace enigma_user
{

enum {
  sh_vertex = 0,
  sh_fragment = 1,
  sh_tesscontrol = 2,
  sh_tessevaluation = 3,
  sh_geometry = 4
};

int glsl_shader_create(int type);
int glsl_shader_load(int id, string fname);
bool glsl_shader_compile(int id);
bool glsl_shader_get_compiled(int id);
string glsl_shader_get_infolog(int id);
void glsl_shader_free(int id);

int glsl_program_create();
bool glsl_program_link(int id);
bool glsl_program_validate(int id);
void glsl_program_attach(int id, int sid);
void glsl_program_detach(int id, int sid);

void glsl_program_set(int id);
void glsl_program_reset();
void glsl_program_free(int id);

int glsl_get_uniform_location(int program, string name);

void glsl_uniformf(int location, float v0);
void glsl_uniformf(int location, float v0, float v1);
void glsl_uniformf(int location, float v0, float v1, float v2);
void glsl_uniformf(int location, float v0, float v1, float v2, float v3);
void glsl_uniformi(int location, int v0);
void glsl_uniformi(int location, int v0, int v1);
void glsl_uniformi(int location, int v0, int v1, int v2);
void glsl_uniformi(int location, int v0, int v1, int v2, int v3);
void glsl_uniformui(int location, unsigned v0);
void glsl_uniformui(int location, unsigned v0, unsigned v1);
void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2);
void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2, unsigned v3);

// Wrap our abstracted version to the useless GayMaker version
#define shader_set            glsl_program_set
#define shader_reset          glsl_program_reset
#define shader_get_uniform    glsl_get_uniform_location
#define shader_get_sampler_index glsl_get_uniform_location
#define shader_set_uniform_f  glsl_uniformf
#define shader_set_uniform_i  glsl_uniformi
}
