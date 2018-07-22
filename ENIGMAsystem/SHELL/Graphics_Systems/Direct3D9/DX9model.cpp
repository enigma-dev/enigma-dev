/** Copyright (C) 2013 Robert B. Colton, Adriano Tumminelli
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
#include "DX9ModelStruct.h"
#include "Bridges/General/DX9Context.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GSmodel.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include <stdlib.h>

using namespace std;

#include <iostream>
#include <map>
#include <list>
#include "Universal_System/fileio.h"
#include "Universal_System/estring.h"

vector<Mesh*> meshes(0);

namespace enigma {

unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
	unsigned int pos = txt.find( ch );
	unsigned int initialPos = 0;
	strs.clear();

	// Decompose statement
	while( pos != std::string::npos ) {
		strs.push_back( txt.substr( initialPos, pos - initialPos + 1 ) );
		initialPos = pos + 1;

		pos = txt.find( ch, initialPos );
	}

	// Add the last one
	strs.push_back( txt.substr( initialPos, std::min( pos, (unsigned)txt.size() ) - initialPos + 1 ) );

	return strs.size();
}

}

namespace enigma_user
{

unsigned d3d_model_create(int type)
{
  unsigned id = meshes.size();
  meshes.push_back(new Mesh(type));
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

unsigned d3d_model_get_stride(int id) {
	return meshes[id]->GetStride();
}

bool d3d_model_load(int id, string fname)
{
  //TODO: this needs to be rewritten properly not using the file_text functions
  using namespace enigma_user;

  int file = file_text_open_read(fname);

  if (file == -1) {
	return false;
  }

  string fileExt = fname.substr(fname.find_last_of(".") + 1) ;
  if (fileExt == "obj")
  {
	vector< float > vertices;
	vector< float > uvs;
	vector< float > normals;

	int faceCount = 0;
	bool hasTexture = false;
	bool hasNormals = false;
	meshes[id]->Begin(pr_trianglelist);

	while (!file_text_eof(file))
	{
		string line = file_text_read_string(file);
		file_text_readln(file);
		enigma::string_parse(&line);
		if (line.length() > 0)
		{
			if(line[0] == 'v')
			{
				vector<float> floats = enigma::float_split(line, ' ');
				floats.erase( floats.begin());

				int n = 0;
				switch(line[1])
				{
				case ' ':
					n = 0;
					for( vector<float>::const_iterator i = floats.begin(); i != floats.end(); ++i)
					{
						if(n < 3) vertices.push_back(*i);
						n++;
					}
					break;
				case 't':
					n = 0;
					for( vector<float>::const_iterator i = floats.begin(); i != floats.end(); ++i)
					{
						if(n < 2) uvs.push_back(*i);
						n++;
					}
					hasTexture = true;
					break;
				case 'n':
					n = 0;
					for( vector<float>::const_iterator i = floats.begin(); i != floats.end(); ++i)
					{
						if(n < 3) normals.push_back(*i);
						n++;
					}
					hasNormals = true;
					break;
				default:
					break;
				}
			}
			else if(line[0] == 'f')
			{
				faceCount++;
				vector<float> f = enigma::float_split(line, ' ');
				f.erase( f.begin());
				int faceVertices = f.size() / (1 + hasTexture + hasNormals);
				int of = 1 + hasTexture + hasNormals;

				meshes[id]->AddVertex(vertices[(f[0]-1)*3],  vertices[(f[0]-1)*3 +1], vertices[(f[0]-1)*3 +2]);
				if(hasNormals) meshes[id]->AddNormal(normals[(f[2]-1)*3], normals[(f[2]-1)*3 +1], normals[(f[2]-1)*3 +2]);
				if(hasTexture) meshes[id]->AddTexture(uvs[(f[1]-1)*2], 1 - uvs[(f[1]-1)*2 +1]);

				meshes[id]->AddVertex(vertices[(f[1*of]-1)*3],  vertices[(f[1*of]-1)*3 +1] , vertices[(f[1*of]-1)*3 +2]);
				if(hasNormals) meshes[id]->AddNormal(normals[(f[1*of + 2]-1)*3], normals[(f[1*of  + 2]-1)*3 +1], normals[(f[1*of  + 2]-1)*3 +2]);
				if(hasTexture) meshes[id]->AddTexture(uvs[(f[1*of + 1]-1)*2], 1 - uvs[(f[1*of + 1]-1)*2 +1]);

				meshes[id]->AddVertex(vertices[(f[2*of]-1)*3],  vertices[(f[2*of]-1)*3 +1] , vertices[(f[2*of]-1)*3 +2]);
				if(hasNormals) meshes[id]->AddNormal(normals[(f[2*of + 2]-1)*3], normals[(f[2*of  + 2]-1)*3 +1], normals[(f[2*of  + 2]-1)*3 +2]);
				if(hasTexture) meshes[id]->AddTexture(uvs[(f[2*of + 1]-1)*2], 1 - uvs[(f[2*of + 1]-1)*2 +1]);

				//is a quad
				if(faceVertices == 4)
				{
					meshes[id]->AddVertex(vertices[(f[2*of]-1)*3],  vertices[(f[2*of]-1)*3 +1] , vertices[(f[2*of]-1)*3 +2]);
					if(hasNormals) meshes[id]->AddNormal(normals[(f[2*of + 2]-1)*3], normals[(f[2*of  + 2]-1)*3 +1], normals[(f[2*of  + 2]-1)*3 +2]);
					if(hasTexture) meshes[id]->AddTexture(uvs[(f[2*of + 1]-1)*2], 1 - uvs[(f[2*of + 1]-1)*2 +1]);

					meshes[id]->AddVertex( vertices[(f[3*of]-1)*3],  vertices[(f[3*of]-1)*3 +1] , vertices[(f[3*of]-1)*3 +2]);
					if(hasNormals) meshes[id]->AddNormal(normals[(f[3*of + 2]-1)*3], normals[(f[3*of  + 2]-1)*3 +1], normals[(f[3*of  + 2]-1)*3 +2]);
					if(hasTexture) meshes[id]->AddTexture(uvs[(f[3*of + 1]-1)*2], 1 - uvs[(f[3*of + 1]-1)*2 +1]);

					meshes[id]->AddVertex(vertices[(f[0]-1)*3],  vertices[(f[0]-1)*3 +1], vertices[(f[0]-1)*3 +2]);
					if(hasNormals) meshes[id]->AddNormal(normals[(f[2]-1)*3], normals[(f[2]-1)*3 +1], normals[(f[2]-1)*3 +2]);
					if(hasTexture) meshes[id]->AddTexture(uvs[(f[0 + 1]-1)*2], 1 - uvs[(f[0 + 1]-1)*2 +1]);
				}

			}
		}
	}
	meshes[id]->End();

  }
  else
  {
	int something = file_text_read_real(file);
	if (something != 100) {
		return false;
	}
	file_text_readln(file);
	unsigned count = file_text_read_real(file);
	file_text_readln(file);
	int kind;
	float v[3], n[3], t[2];
	double col, alpha;
	for (unsigned i = 0; i < count; i++) {
		string str = file_text_read_string(file);
		std::vector<std::string> dat;
		enigma::split(str, dat, ' ');
		switch (atoi(dat[0].c_str()))
		{
		case  0:
			kind = atoi(dat[1].c_str());
			d3d_model_primitive_begin(id, kind);
			break;
		case  1:
			d3d_model_primitive_end(id);
			break;
		case  2:
			v[0] = atof(dat[1].c_str()); v[1] = atof(dat[2].c_str()); v[2] = atof(dat[3].c_str());
			d3d_model_vertex(id, v[0],v[1],v[2]);
			break;
		case  3:
			v[0] = atof(dat[1].c_str()); v[1] = atof(dat[2].c_str()); v[2] = atof(dat[3].c_str());
			col = atoi(dat[4].c_str()); alpha = atof(dat[5].c_str());
			d3d_model_vertex_color(id, v[0],v[1],v[2],col,alpha);
			break;
		case  4:
			v[0] = atof(dat[1].c_str()); v[1] = atof(dat[2].c_str()); v[2] = atof(dat[3].c_str());
			t[0] = atof(dat[4].c_str()); t[1] = atof(dat[5].c_str());
			d3d_model_vertex_texture(id, v[0],v[1],v[2],t[0],t[1]);
			break;
		case  5:
			v[0] = atof(dat[1].c_str()); v[1] = atof(dat[2].c_str()); v[2] = atof(dat[3].c_str());
			t[0] = atof(dat[4].c_str()); t[1] = atof(dat[5].c_str());
			col = atoi(dat[6].c_str()); alpha = atof(dat[7].c_str());
			d3d_model_vertex_texture_color(id, v[0],v[1],v[2],t[0],t[1],col,alpha);
			break;
		case  6:
			v[0] = atof(dat[1].c_str()); v[1] = atof(dat[2].c_str()); v[2] = atof(dat[3].c_str());
			n[0] = atof(dat[4].c_str()); n[1] = atof(dat[5].c_str()); n[2] = atof(dat[6].c_str());
			d3d_model_vertex_normal(id, v[0],v[1],v[2],n[0],n[1],n[2]);
			break;
		case  7:
			v[0] = atof(dat[1].c_str()); v[1] = atof(dat[2].c_str()); v[2] = atof(dat[3].c_str());
			n[0] = atof(dat[4].c_str()); n[1] = atof(dat[5].c_str()); n[2] = atof(dat[6].c_str());
			col = atoi(dat[7].c_str()); alpha = atof(dat[8].c_str());
			d3d_model_vertex_normal_color(id, v[0],v[1],v[2],n[0],n[1],n[2],col,alpha);
			break;
		case  8:
			v[0] = atof(dat[1].c_str()); v[1] = atof(dat[2].c_str()); v[2] = atof(dat[3].c_str());
			n[0] = atof(dat[4].c_str()); n[1] = atof(dat[5].c_str()); n[2] = atof(dat[6].c_str());
			t[0] = atof(dat[7].c_str()); t[1] = atof(dat[8].c_str());
			d3d_model_vertex_normal_texture(id, v[0],v[1],v[2],n[0],n[1],n[2],t[0],t[1]);
			break;
		case  9:
			v[0] = atof(dat[1].c_str()); v[1] = atof(dat[2].c_str()); v[2] = atof(dat[3].c_str());
			n[0] = atof(dat[4].c_str()); n[1] = atof(dat[5].c_str()); n[2] = atof(dat[6].c_str());
			t[0] = atof(dat[7].c_str()); t[1] = atof(dat[8].c_str());
			col = atoi(dat[9].c_str()); alpha = atof(dat[10].c_str());
			d3d_model_vertex_normal_texture_color(id, v[0],v[1],v[2],n[0],n[1],n[2],t[0],t[1],col,alpha);
			break;
		case  10:
			d3d_model_block(id, atof(dat[1].c_str()), atof(dat[2].c_str()), atof(dat[3].c_str()), atof(dat[4].c_str()), atof(dat[5].c_str()), atof(dat[6].c_str()), atof(dat[7].c_str()), atof(dat[9].c_str()), true);
			break;
		case  11:
			d3d_model_cylinder(id, atof(dat[1].c_str()), atof(dat[2].c_str()), atof(dat[3].c_str()), atof(dat[4].c_str()), atof(dat[5].c_str()), atof(dat[6].c_str()), atof(dat[7].c_str()), atof(dat[9].c_str()), atoi(dat[10].c_str()), atoi(dat[11].c_str()));
			break;
		case  12:
			d3d_model_cone(id, atof(dat[1].c_str()), atof(dat[2].c_str()), atof(dat[3].c_str()), atof(dat[4].c_str()), atof(dat[5].c_str()), atof(dat[6].c_str()), atof(dat[7].c_str()), atof(dat[9].c_str()), atoi(dat[10].c_str()), atoi(dat[11].c_str()));
			break;
		case  13:
			d3d_model_ellipsoid(id, atof(dat[1].c_str()), atof(dat[2].c_str()), atof(dat[3].c_str()), atof(dat[4].c_str()), atof(dat[5].c_str()), atof(dat[6].c_str()), atof(dat[7].c_str()), atof(dat[9].c_str()), atoi(dat[10].c_str()));
			break;
		case  14:
			d3d_model_wall(id, atof(dat[1].c_str()), atof(dat[2].c_str()), atof(dat[3].c_str()), atof(dat[4].c_str()), atof(dat[5].c_str()), atof(dat[6].c_str()), atof(dat[7].c_str()), atof(dat[9].c_str()));
			break;
		case  15:
			d3d_model_floor(id, atof(dat[1].c_str()), atof(dat[2].c_str()), atof(dat[3].c_str()), atof(dat[4].c_str()), atof(dat[5].c_str()), atof(dat[6].c_str()), atof(dat[7].c_str()), atof(dat[9].c_str()));
			break;
		}
		file_text_readln(file);
	}
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
  d3d_transform_add_translation(x, y, z);
  meshes[id]->Draw();
  d3d_transform_add_translation(-x, -y, -z);
}

void d3d_model_draw(int id, int texId)
{
  texture_set(texId);
  meshes[id]->Draw();
}

void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int texId)
{
  texture_set(texId);
  d3d_model_draw(id, x, y, z);
}

void d3d_model_primitive_begin(int id, int kind, int format)
{
  meshes[id]->Begin(kind);
}

void d3d_model_primitive_end(int id)
{
  meshes[id]->End();
}

void d3d_model_index(int id, unsigned ind) {
  meshes[id]->AddIndex(ind);
}

// 2D Functions
void d3d_model_vertex(int id, gs_scalar x, gs_scalar y)
{
  meshes[id]->AddVertex(x, y);
}

void d3d_model_vertex_color(int id, gs_scalar x, gs_scalar y, int col, double alpha)
{
  meshes[id]->AddVertex(x, y);
  meshes[id]->AddColor(col, alpha);
}

void d3d_model_vertex_texture(int id, gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty)
{
  meshes[id]->AddVertex(x, y);
  meshes[id]->AddTexture(tx, ty);
}

void d3d_model_vertex_texture_color(int id, gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, double alpha)
{
  meshes[id]->AddVertex(x, y);
  meshes[id]->AddColor(col, alpha);
  meshes[id]->AddTexture(tx, ty);
}

// 3D Vertex Functions
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
  meshes[id]->AddColor(col, alpha);
  meshes[id]->AddTexture(tx, ty);
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
  meshes[id]->AddColor(col, alpha);
  meshes[id]->AddTexture(tx, ty);
}

}
