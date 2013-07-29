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

#ifndef _DX9MATERIAL__H
#define _DX9MATERIAL__H

namespace enigma_user {
int material_create();
void material_load();
void material_save();
void material_add_texture(int id, int tid);
void material_set_texture(int id, int mtid, int tid);
void material_remove_texture(int id, int mtid);
void material_set_shader(int id, int sid);
int material_get_shader(int id);
void material_use(int id);
void material_reset();
void material_free(int id);
}

#endif
