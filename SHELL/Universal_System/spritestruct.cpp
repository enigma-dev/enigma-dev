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

#include <map>
#include <string>
#include "spritestruct.h"

namespace enigma
{
  int currentspriteind;

  std::map<int,sprite*> spritestructarray;
  std::map<int,sprite*>::iterator spriteiter;
}


int sprite_get_width(double sprite)
{
  enigma::sprite* fnd=enigma::spritestructarray[(int)sprite]; if (fnd!=NULL)
    return fnd->width;
  #if SHOWERRORS
  else show_error("Trying to retrieve width of unexisting sprite "+string(sprite),0);
  #endif
  return 0;
}

int sprite_get_height(double sprite)
{
  enigma::sprite* fnd=enigma::spritestructarray[(int)sprite]; if (fnd!=NULL)
    return fnd->height;
  #if SHOWERRORS
  else show_error("Trying to retrieve width of unexisting sprite "+string(sprite),0);
  #endif
  return 0;
}


int sprite_get_bbox_bottom(double sprite)
{

  enigma::sprite* fnd=enigma::spritestructarray[(int)sprite]; if (fnd!=NULL)
    return fnd->height;
  #if SHOWERRORS
  else show_error("Trying to retrieve width of unexisting sprite "+string(sprite),0);
  #endif
  return 0;
}
int sprite_get_bbox_left(double sprite)
{
  enigma::sprite* fnd=enigma::spritestructarray[(int)sprite]; if (fnd!=NULL)
    return 0;//fnd->height;
  #if SHOWERRORS
  else show_error("Trying to retrieve width of unexisting sprite "+string(sprite),0);
  #endif
  return 0;
}
int sprite_get_bbox_right(double sprite)
{
  enigma::sprite* fnd=enigma::spritestructarray[(int)sprite]; if (fnd!=NULL)
    return fnd->width;
  #if SHOWERRORS
  else show_error("Trying to retrieve width of unexisting sprite "+string(sprite),0);
  #endif
  return 0;
}
int sprite_get_bbox_top(double sprite)
{
  enigma::sprite* fnd=enigma::spritestructarray[(int)sprite]; if (fnd!=NULL)
    return 0;//fnd->height;
  #if SHOWERRORS
  else show_error("Trying to retrieve width of unexisting sprite "+string(sprite),0);
  #endif
  return 0;
}

int sprite_get_bbox_mode(double sprite)
{
  /*enigma::sprite* fnd=enigma::spritestructarray[(int)sprite]; if (fnd!=NULL)
    return fnd->height;
  #if SHOWERRORS
  else show_error("Trying to retrieve width of unexisting sprite "+string(sprite),0);
  #endif*/
  return 0;
}


//sprite_get_name

int sprite_get_number(double sprite)
{
  enigma::sprite* fnd=enigma::spritestructarray[(int)sprite]; if (fnd!=NULL)
    return fnd->subcount;
  #if SHOWERRORS
  else show_error("Trying to retrieve width of unexisting sprite "+string(sprite),0);
  #endif
  return 0;
}


//sprite_get_precise
//sprite_get_preload
//sprite_get_smooth

int sprite_get_texture(double sprite,double subimage)
{
  enigma::sprite* fnd=enigma::spritestructarray[(int)sprite]; if (fnd!=NULL)
    return fnd->texturearray[(int)subimage];
  #if SHOWERRORS
  else show_error("Trying to retrieve width of unexisting sprite "+string(sprite),0);
  #endif
  return 0;
}

//sprite_get_transparent

int sprite_get_xoffset(double sprite)
{
  enigma::sprite* fnd=enigma::spritestructarray[(int)sprite]; if (fnd!=NULL)
    return fnd->xoffset;
  #if SHOWERRORS
  else show_error("Trying to retrieve width of unexisting sprite "+string(sprite),0);
  #endif
  return 0;
}
int sprite_get_yoffset(double sprite)
{
  enigma::sprite* fnd=enigma::spritestructarray[(int)sprite]; if (fnd!=NULL)
    return fnd->yoffset;
  #if SHOWERRORS
  else show_error("Trying to retrieve width of unexisting sprite "+string(sprite),0);
  #endif
  return 0;
}

