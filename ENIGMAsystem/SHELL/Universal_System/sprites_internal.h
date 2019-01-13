/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2015 Harijs Grinbergs
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

//FIXME: this should be ifdef shellmain but enigmas in a sorry state
#ifdef JUST_DEFINE_IT_RUN
#error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_SPRITESTRUCT
#define ENIGMA_SPRITESTRUCT

#include "Collision_Systems/collision_types.h"
#include "var4.h"

#include <string>
#include <vector>

namespace enigma {
struct bbox_rect_t {
  int bottom, left, right, top;
};

struct sprite {
  int width, height, subcount, xoffset, yoffset, id;

  std::vector<int> texturearray;      //Each subimage has a texture
  std::vector<double> texturexarray;  //Offset in texture (used in texture atlas, otherwise 0)
  std::vector<double> textureyarray;
  std::vector<double> texturewarray;  //width of valid texture space
  std::vector<double> textureharray;  //ditto height
  std::vector<void *> colldata;       // Each subimage has collision data

  //void*  *pixeldata;
  bbox_rect_t bbox, bbox_relative;
  int bbox_mode = 0;  //Default is automatic
  bool where, smooth = false;

  sprite();
  sprite(int);
};
extern sprite **spritestructarray;  // INVARIANT: Should only be modified inside spritestruct.cpp.
extern size_t sprite_idmax;

/// Called at game start.
void sprites_init();  /// This should allocate room for sprites and perform any other necessary actions.

int sprite_new_empty(unsigned sprid, unsigned subc, int w, int h, int x, int y, int bbt, int bbb, int bbl, int bbr,
                     bool pl, bool sm);
void sprite_add_to_index(sprite *ns, std::string filename, int imgnumb, bool precise, bool transparent, bool smooth,
                         int x_offset, int y_offset, bool mipmap);
void sprite_add_copy(sprite *spr, sprite *spr_copy);

//Sets the subimage
void sprite_set_subimage(int sprid, int imgindex, unsigned int w, unsigned int h, unsigned char *chunk,
                         unsigned char *collision_data, collision_type ct);
//Appends a subimage
void sprite_add_subimage(int sprid, unsigned int w, unsigned int h, unsigned char *chunk, unsigned char *collision_data,
                         collision_type ct);
void spritestructarray_reallocate();

extern const bbox_rect_t &sprite_get_bbox(int sprid);
extern const bbox_rect_t &sprite_get_bbox_relative(int sprid);
}  //namespace enigma

#endif  // ENIGMA_SPRITESTRUCT
