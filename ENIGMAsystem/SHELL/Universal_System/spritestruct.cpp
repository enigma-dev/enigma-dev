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

#include <map>
#include <string>
using namespace std;

#include "spritestruct.h"
#include "../libEGMstd.h"

namespace enigma {
  sprite** spritestructarray;
	extern size_t sprite_idmax;
	sprite::sprite(): texturearray(NULL) {}
  sprite::sprite(unsigned int x): texturearray(new unsigned int[x]) {}
}

#define DEBUGMODE 1

int sprite_get_width(int sprite)
{
	#ifdef DEBUGMODE
  if (!enigma::spritestructarray[sprite]) {
	  show_error("Trying to retrieve width of non-existing sprite "+toString(sprite),0);
	  return 0;
  }
	#endif
	return enigma::spritestructarray[sprite]->width;
}

int sprite_get_height(int sprite)
{
	#ifdef DEBUGMODE
  if (!enigma::spritestructarray[sprite]) {
	  show_error("Trying to retrieve width of non-existing sprite "+toString(sprite),0);
	  return 0;
  }
	#endif
	return enigma::spritestructarray[sprite]->height;
}


int sprite_get_bbox_bottom(int sprite)
{
	#ifdef DEBUGMODE
  if (!enigma::spritestructarray[sprite]) {
	  show_error("Trying to retrieve bounding box of non-existing sprite "+toString(sprite),0);
	  return 0;
  }
	#endif
	return enigma::spritestructarray[sprite]->bbox_bottom;
}
int sprite_get_bbox_left(int sprite)
{
	#ifdef DEBUGMODE
  if (!enigma::spritestructarray[sprite]) {
	  show_error("Trying to retrieve bounding box of non-existing sprite "+toString(sprite),0);
	  return 0;
  }
	#endif
	return enigma::spritestructarray[sprite]->bbox_left;
}
int sprite_get_bbox_right(int sprite)
{
	#ifdef DEBUGMODE
  if (!enigma::spritestructarray[sprite]) {
	  show_error("Trying to retrieve bounding box of non-existing sprite "+toString(sprite),0);
	  return 0;
  }
	#endif
	return enigma::spritestructarray[sprite]->bbox_right;
}
int sprite_get_bbox_top(int sprite)
{
	#ifdef DEBUGMODE
  if (!enigma::spritestructarray[sprite]) {
	  show_error("Trying to retrieve bounding box of non-existing sprite "+toString(sprite),0);
	  return 0;
  }
	#endif
	return enigma::spritestructarray[sprite]->bbox_top;
}

//sprite_get_bbox_mode
//sprite_get_name
//sprite_get_precise
//sprite_get_preload
//sprite_get_smooth
//sprite_get_transparent

int sprite_get_number(int sprite)
{
	#ifdef DEBUGMODE
  if (!enigma::spritestructarray[sprite]) {
	  show_error("Trying to retrieve subimage count of non-existing sprite "+toString(sprite),0);
	  return 0;
  }
	#endif
	return enigma::spritestructarray[sprite]->subcount;
}

int sprite_get_texture(int sprite,int subimage)
{
  enigma::sprite* ps = enigma::spritestructarray[sprite];
	#ifdef DEBUGMODE
  if (!ps) {
	  show_error("Trying to retrieve bounding box of non-existing sprite "+toString(sprite),0);
	  return 0;
  }
	#endif
	return ps->texturearray[subimage % ps->subcount];
}

int sprite_get_xoffset(int sprite){
	enigma::sprite* fnd = enigma::spritestructarray[sprite];
	#ifdef DEBUGMODE
  if (!fnd) {
	  show_error("Trying to retrieve x origin of non-existing sprite "+toString(sprite),0);
	  return 0;
  }
	#endif
	return fnd->xoffset;
}

int sprite_get_yoffset(int sprite){
	enigma::sprite* fnd = enigma::spritestructarray[sprite];
	#ifdef DEBUGMODE
  if (!fnd) {
	  show_error("Trying to retrieve x origin of non-existing sprite "+toString(sprite),0);
	  return 0;
  }
	#endif
	return fnd->yoffset;
}
