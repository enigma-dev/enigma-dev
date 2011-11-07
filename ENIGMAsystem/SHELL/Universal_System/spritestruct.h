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

#ifndef ENIGMA_SPRITESTRUCT
#define ENIGMA_SPRITESTRUCT

namespace enigma
{
  struct sprite
  {
    int width,height,subcount,xoffset,yoffset,id;
    unsigned int *texturearray; //Each subimage has a texture
    void **colldata; // Each subimage has collision data

    //void*  *pixeldata;
    double texbordx, texbordy;
    int bbox_bottom, bbox_left, bbox_right, bbox_top;
    bool where,smooth;

    sprite();
    sprite(unsigned int);
  };
  extern sprite** spritestructarray;
  extern size_t sprite_idmax;
}

//int sprite_add(string filename,double imgnumb,double precise,double transparent,double smooth,double preload,double x_offset,double y_offset);

namespace enigma
{
  //Adds an empty sprite to the list
  int sprite_new_empty(unsigned sprid, unsigned subc, int w, int h, int x, int y, int bbt, int bbb, int bbl, int bbr, bool pl, bool sm);

  //Adds a subimage to an existing sprite from the exe
  void sprite_set_subimage(int sprid, int imgindex, int x, int y, unsigned int w,unsigned int h,unsigned char*chunk);
}

extern int sprite_get_width  (int sprite);
extern int sprite_get_height (int sprite);

extern int sprite_get_bbox_bottom (int sprite);
extern int sprite_get_bbox_left   (int sprite);
extern int sprite_get_bbox_right  (int sprite);
extern int sprite_get_bbox_top    (int sprite);
extern int sprite_get_bbox_mode   (int sprite);

extern int sprite_get_bbox_bottom_relative (int sprite);
extern int sprite_get_bbox_left_relative   (int sprite);
extern int sprite_get_bbox_right_relative  (int sprite);
extern int sprite_get_bbox_top_relative    (int sprite);

extern int sprite_get_number  (int sprite);
extern int sprite_get_texture (int sprite, int subimage);
extern int sprite_get_xoffset (int sprite);
extern int sprite_get_yoffset (int sprite);

int sprite_add(string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset);

#endif // ENIGMA_SPRITESTRUCT

