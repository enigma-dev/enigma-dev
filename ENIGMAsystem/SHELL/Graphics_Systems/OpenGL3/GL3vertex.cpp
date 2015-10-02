/** Copyright (C) 2015 Harijs Grinbergs
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
#include <vector>
#include <map>
#include <utility>
using std::vector;
using std::pair;

#include "../General/GSvertex.h"
#include "../General/GStextures.h"

namespace enigma {
  vector<VertexFormat*> vertexFormats(0);    
}

namespace enigma_user {

  unsigned vertex_format_create(){
    unsigned id = enigma::vertexFormats.size();
    enigma::vertexFormats.push_back(new enigma::VertexFormat());
    return id;
  }

  void vertex_format_destroy(int id){
    delete enigma::vertexFormats[id];
    enigma::vertexFormats[id] = nullptr;
  }

  bool vertex_format_exists(int id){
    return (id >= 0 && (unsigned)id < enigma::vertexFormats.size() &&  enigma::vertexFormats[id] != nullptr);
  }

  void vertex_format_add(int id, int type, int attribute) {
    enigma::vertexFormats[id]->AddAttribute(type, attribute);
  }

}