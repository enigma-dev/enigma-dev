/*********************************************************************************\
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
\*********************************************************************************/

/*
int sprite_get_width(double sprite)
int sprite_get_height(double sprite)
int sprite_get_bbox_bottom(double sprite)
int sprite_get_bbox_left(double sprite)
int sprite_get_bbox_right(double sprite)
int sprite_get_bbox_top(double sprite)
int sprite_get_bbox_mode(double sprite)
int sprite_get_number(double sprite)
int sprite_get_texture(double sprite,double subimage)
int sprite_get_xoffset(double sprite)
int sprite_get_yoffset(double sprite)
*/


namespace enigma
{
  extern int currentspriteind;
  struct sprite
  {
       int width, height, subcount;
       int xoffset, yoffset;
       int id;
       
       std::map<int,unsigned int> texturearray;       //Each subimage has a texture
       #if COLLIGMA
       std::map<int,collCustom*> bitmask;     //Each subimage gets a custom mask 
       #endif
       //void*  *pixeldata;
       
       double texbordx, texbordy;
       int bbox_bottom, bbox_left, bbox_right, bbox_top;
       bool where,smooth;
  };
  
  extern std::map<int,sprite*> spritestructarray;
  extern std::map<int,sprite*>::iterator spriteiter;
}

extern int sprite_get_width(double sprite);
extern int sprite_get_height(double sprite);
extern int sprite_get_bbox_bottom(double sprite);
extern int sprite_get_bbox_left(double sprite);
extern int sprite_get_bbox_right(double sprite);
extern int sprite_get_bbox_top(double sprite);
extern int sprite_get_bbox_mode(double sprite);
extern int sprite_get_number(double sprite);
extern int sprite_get_texture(double sprite,double subimage);
extern int sprite_get_xoffset(double sprite);
extern int sprite_get_yoffset(double sprite);



