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

#include "../General/DirectXHeaders.h"
#include "DX9material.h"
#include "../General/DXbinding.h"
#include <math.h>

#include <vector>
using std::vector;

struct Material {
  unsigned int shader;
  vector<unsigned int> textures;

  Material()
  {

  }

  ~Material()
  {

  }
};

vector<Material*> materials;

namespace enigma_user
{

int material_create(int type)
{
  unsigned int id = materials.size();
  materials.push_back(new Material());
  return id;
}

void material_add_texture(int id, int tid)
{
  materials[id]->textures.push_back(tid);
}

void material_set_texture(int id, int mtid, int tid)
{
  materials[id]->textures[mtid] = tid;
}

void material_set_shader(int id, int sid)
{
  materials[id]->shader = sid;
}

int material_get_shader(int id)
{
  return materials[id]->shader;
}

/* Could use global bound material for optimization, just like texture's */
void material_use(int id)
{

}

void material_reset()
{

}

void material_free(int id)
{
  delete materials[id];
}

}

