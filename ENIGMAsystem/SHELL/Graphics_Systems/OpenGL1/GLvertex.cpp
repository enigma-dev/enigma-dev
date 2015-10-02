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

#include "../General/OpenGLHeaders.h"
#include "../General/GSvertex.h"

//GL1 cannot have this stuff!!! Use GL3! Maybe an error here?
namespace enigma_user {

  unsigned vertex_format_create(){
    return 0;
  }

  void vertex_format_destroy(int id){
  }

  bool vertex_format_exists(int id){
	  return false;
  }

  void vertex_format_add(int id, int type, int attribute) {
  }

}
