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

#include <cstddef>

#include <math.h>
#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSbackground.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/spritestruct.h"

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_background(bck2d,back)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return r;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#else
  #define get_background(bck2d,back)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#endif

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma_user {
  extern int room_width, room_height;
}
namespace enigma {
  extern size_t background_idmax;
  D3DCOLOR get_currentcolor();
}


#include <string.h> // needed for querying ARB extensions

#include "Bridges/General/DX9Context.h"
#include "DX9TextureStruct.h"

namespace enigma_user
{

void draw_background(int back, gs_scalar x, gs_scalar y)
{
  get_background(bck2d, back);

}

void draw_background_stretched(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height)
{
	get_background(bck2d, back);

}

void draw_background_part(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{
	get_background(bck2d, back);

}

void draw_background_tiled(int back, gs_scalar x, gs_scalar y)
{
    get_background(bck2d,back);

    x = bck2d->width-fmod(x,bck2d->width);
    y = bck2d->height-fmod(y,bck2d->height);

    const float
    tbx = bck2d->texbordx,tby=bck2d->texbordy;

    const int
    hortil = int (ceil(room_width/(bck2d->width*tbx))) + 1,
    vertil = int (ceil(room_height/(bck2d->height*tby))) + 1;

}

void draw_background_tiled_area(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2)
{
	get_background(bck2d,back);

    const float tbx=bck2d->texbordx,tby=bck2d->texbordy;
    float sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = bck2d->width;
    sh = bck2d->height;

    i = x1-(fmod(x1,sw) - fmod(x,sw)) - sw*(fmod(x1,sw)<fmod(x,sw));
    j = y1-(fmod(y1,sh) - fmod(y,sh)) - sh*(fmod(y1,sh)<fmod(y,sh));
    jj = j;

}

void draw_background_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int color, gs_scalar alpha)
{
	get_background(bck2d, back);

}

void draw_background_stretched_ext(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
	get_background(bck2d, back);

}

void draw_background_part_ext(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
	get_background(bck2d, back);

}

void draw_background_tiled_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha, bool htiled, bool vtiled)
{

}

void draw_background_tiled_area_ext(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{

}

void draw_background_general(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a1, gs_scalar a2, gs_scalar a3, gs_scalar a4)
{

}

double background_get_texture_width_factor(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texbordx;
}

double background_get_texture_height_factor(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texbordy;
}

int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload)
{

}

}

