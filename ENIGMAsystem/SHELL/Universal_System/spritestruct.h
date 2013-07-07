/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <stdlib.h>
#include <string>

#include "Collision_Systems/collision_types.h"

#ifndef ENIGMA_SPRITESTRUCT
#define ENIGMA_SPRITESTRUCT

struct bbox_rect_t {
  int bottom, left, right, top;
};
namespace enigma
{
  struct sprite
  {
    int width,height,subcount,xoffset,yoffset,id;
    int *texturearray; //Each subimage has a texture
    double *texbordxarray, *texbordyarray;
    void **colldata; // Each subimage has collision data

    //void*  *pixeldata;
    bbox_rect_t bbox, bbox_relative;
    bool where,smooth;

    sprite();
    sprite(int);
  };
  extern sprite** spritestructarray; // INVARIANT: Should only be modified inside spritestruct.cpp.
  extern size_t sprite_idmax;
}

//int sprite_add(string filename,double imgnumb,double precise,double transparent,double smooth,double preload,double x_offset,double y_offset);

namespace enigma
{
  /// Called at game start.
  void sprites_init(); /// This should allocate room for sprites and perform any other necessary actions.

  int sprite_new_empty(unsigned sprid, unsigned subc, int w, int h, int x, int y, int bbt, int bbb, int bbl, int bbr, bool pl, bool sm);
  void sprite_add_to_index(sprite *ns, std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, int x_offset, int y_offset);
  void sprite_add_copy(sprite *spr, sprite *spr_copy);

  //Adds a subimage to an existing sprite from the exe
  void sprite_set_subimage(int sprid, int imgindex, int x, int y, unsigned int w,unsigned int h,unsigned char*chunk, unsigned char*collision_data, collision_type ct);

  void spritestructarray_reallocate();
}

namespace enigma_user
{

extern int sprite_get_width  (int sprite);
extern int sprite_get_height (int sprite);
extern double sprite_get_texture_width_factor(int sprite, int subimg);
extern double sprite_get_texture_height_factor(int sprite, int subimg);

extern int sprite_get_bbox_bottom (int sprite);
extern int sprite_get_bbox_left   (int sprite);
extern int sprite_get_bbox_right  (int sprite);
extern int sprite_get_bbox_top    (int sprite);
extern int sprite_get_bbox_mode   (int sprite);

}

extern int sprite_get_bbox_bottom_relative (int sprite);
extern int sprite_get_bbox_left_relative   (int sprite);
extern int sprite_get_bbox_right_relative  (int sprite);
extern int sprite_get_bbox_top_relative    (int sprite);

extern const bbox_rect_t &sprite_get_bbox(int sprite);
extern const bbox_rect_t &sprite_get_bbox_relative(int sprite);

namespace enigma_user
{

extern int sprite_get_number  (int sprite);
extern int sprite_get_texture (int sprite, int subimage);
extern int sprite_get_xoffset (int sprite);
extern int sprite_get_yoffset (int sprite);

int sprite_add(std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset);
int sprite_add(std::string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset);  //GM7+ compatible
bool sprite_replace(int ind, std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset, bool free_texture = true);
bool sprite_replace(int ind, std::string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset, bool free_texture = true);   //GM7+ compatible
void sprite_delete(int ind, bool free_texture = true);
int sprite_duplicate(int ind);
void sprite_assign(int ind, int copy_sprite, bool free_texture = true);
void sprite_merge(int ind, int copy_sprite);
void sprite_set_offset(int ind, int xoff, int yoff);
void sprite_set_alpha_from_sprite(int ind, int copy_sprite, bool free_texture=true);
void sprite_set_bbox(int sprite, int left, int top, int right, int bottom);

}

#endif // ENIGMA_SPRITESTRUCT

