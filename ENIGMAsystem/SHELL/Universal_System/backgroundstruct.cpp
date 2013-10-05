/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
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

#include <string>
#include <cstring>
using namespace std;

#include "Graphics_Systems/graphics_mandatory.h"
#include "libEGMstd.h"
#include "backgroundstruct.h"
#include "IMGloading.h"

#ifdef DEBUG_MODE
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_background(bck2d,back)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return;\
    }\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return r;\
    }\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
#else
  #define get_background(bck2d,back)\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
#endif

namespace enigma {
  background** backgroundstructarray;
  extern size_t background_idmax;
}


/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */
#include "nlpo2.h"

namespace enigma
{
  background::background():
    tileset(false) {}
  background::background(bool ts):
    tileset(ts) {}
  background::background(int w,int h,int tex,bool trans,bool smth,bool prel):
    width(w), height(h), texture(tex), transparent(trans), smooth(smth), preload(prel), tileset(false) {}
  background::background(bool ts,int w,int h,int tex,bool trans,bool smth,bool prel):
    width(w), height(h), texture(tex), transparent(trans), smooth(smth), preload(prel), tileset(ts) {}

  background_tileset::background_tileset():
    background(true) {}
  background_tileset::background_tileset(int tw, int th, int ho, int vo, int hs, int vs):
    background(true), tileWidth(tw), tileHeight(th), hOffset(ho), vOffset(vo), hSep(hs), vSep(vs) {}
  background_tileset::background_tileset(int w,int h,int tex,bool trans,bool smth,bool prel,int tw, int th, int ho, int vo, int hs, int vs):
    background(true, w, h, tex, trans, smth, prel), tileWidth(tw), tileHeight(th), hOffset(ho), vOffset(vo), hSep(hs), vSep(vs) {}

  //Adds a subimage to an existing sprite from the exe
  void background_new(int bkgid, unsigned w, unsigned h, unsigned char* chunk, bool transparent, bool smoothEdges, bool preload, bool useAsTileset, int tileWidth, int tileHeight, int hOffset, int vOffset, int hSep, int vSep)
  {
    unsigned int fullwidth = nlpo2dc(w)+1, fullheight = nlpo2dc(h)+1;
    char *imgpxdata = new char[4*fullwidth*fullheight+1], *imgpxptr = imgpxdata;
    unsigned int rowindex,colindex;
    for (rowindex = 0; rowindex < h; rowindex++)
    {
      for(colindex = 0; colindex < w; colindex++)
      {
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
        *imgpxptr++ = *chunk++;
      }
      memset(imgpxptr, 0, (fullwidth-colindex) << 2);
      imgpxptr += (fullwidth-colindex) << 2;
    }
    memset(imgpxptr,0,(fullheight-h) * fullwidth);

    int texture = graphics_create_texture(fullwidth,fullheight,imgpxdata,false);
    delete[] imgpxdata;

    backgroundstructarray[bkgid] = useAsTileset ? new background(w,h,texture,transparent,smoothEdges,preload) : new background_tileset(w,h,texture,transparent,smoothEdges,preload,tileWidth, tileHeight, hOffset, vOffset, hSep, vSep);
    background *bak = backgroundstructarray[bkgid];
    bak->texbordx  = (double) w/fullwidth;
    bak->texbordy  = (double) h/fullheight;
  }

  void background_add_to_index(background *bak, string filename, bool transparent, bool smoothEdges, bool preload)
  {
    unsigned int w, h, fullwidth, fullheight;

    char *pxdata = load_bitmap(filename,&w,&h,&fullwidth,&fullheight);
    unsigned texture = graphics_create_texture(fullwidth, fullheight, pxdata, false);
    delete[] pxdata;

    bak->width = w;
    bak->height = h;
    bak->transparent = transparent;
    bak->smooth = smoothEdges;
    bak->preload = preload;
    bak->tileset = false;
    bak->texbordx = (double) w/fullwidth;
    bak->texbordy = (double) h/fullheight;
    bak->texture = texture;
  }

  void background_add_copy(background *bak, background *bck_copy)
  {
    bak->width = bck_copy->width;
    bak->height = bck_copy->height;
    bak->transparent = bck_copy->transparent;
    bak->smooth = bck_copy->smooth;
    bak->preload = bck_copy->preload;
    bak->tileset = bck_copy->tileset;
    bak->texbordx = bck_copy->texbordx;
    bak->texbordy = bck_copy->texbordy;
    bak->texture = graphics_duplicate_texture(bck_copy->texture);
  }

  //Allocates and zero-fills the array at game start
  void backgrounds_init()
  {
    backgroundstructarray = new background*[background_idmax+1];
    for (unsigned i = 0; i < background_idmax; i++)
      backgroundstructarray[i] = NULL;
  }
}

#include "estring.h"

namespace enigma_user
{
  int background_add(string filename, bool transparent, bool smooth, bool preload)
  {
    enigma::background *bck = enigma::backgroundstructarray[enigma::background_idmax] = new enigma::background;
    enigma::background_add_to_index(bck, filename, transparent, smooth, preload);
    return enigma::background_idmax++;
  }

  bool background_replace(int back, string filename, bool transparent, bool smooth, bool preload, bool free_texture)
  {
    get_backgroundnv(bck,back,false);
    if (free_texture)
        enigma::graphics_delete_texture(bck->texture);

    enigma::background_add_to_index(bck, filename, transparent, smooth, preload);
    return true;
  }

  void background_delete(int back, bool free_texture)
  {
    get_background(bck,back);
    if (free_texture)
        enigma::graphics_delete_texture(bck->texture);

    delete enigma::backgroundstructarray[back];
    enigma::backgroundstructarray[back] = NULL;
  }

  int background_duplicate(int back)
  {
    get_backgroundnv(bck_copy,back,-1);
    enigma::background *bck = enigma::backgroundstructarray[enigma::background_idmax] = new enigma::background;
    enigma::background_add_copy(bck, bck_copy);
    return enigma::background_idmax++;
  }

  void background_assign(int back, int copy_background, bool free_texture)
  {
    get_background(bck,back);
    get_background(bck_copy,copy_background);
    if (free_texture)
      enigma::graphics_delete_texture(bck->texture);

    enigma::background_add_copy(bck, bck_copy);
  }

  bool background_exists(int back)
  {
    return unsigned(back) < enigma::background_idmax && bool(enigma::backgroundstructarray) && bool(enigma::backgroundstructarray[back]);
  }

  void background_set_alpha_from_background(int back, int copy_background, bool free_texture)
  {
    get_background(bck,back);
    get_background(bck_copy,copy_background);
    enigma::graphics_replace_texture_alpha_from_texture(bck->texture, bck_copy->texture);
  }
}

