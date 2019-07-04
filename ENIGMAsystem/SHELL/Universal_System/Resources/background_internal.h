/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
*** Copyright (C) 2013 Robert B. Colton
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

#ifdef INCLUDED_FROM_SHELLMAIN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_BACKGROUND_INTERNAL_H
#define ENIGMA_BACKGROUND_INTERNAL_H

#include "Universal_System/var4.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <string>

namespace enigma
{
  extern size_t background_idmax;
  struct background
  {
    int width, height;
    int texture;

    bool transparent;
    bool smooth;
    bool preload;
    double texturex = 0.0, texturey = 0.0;
    double texturew, textureh;

    bool tileset;

    background();
    background(bool);
    background(int w,int h,int tex,bool trans,bool smth,bool prel);
    background(bool,int w,int h,int tex,bool trans,bool smth,bool prel);
  };
  struct background_tileset: background
  {
    int tileWidth;
    int tileHeight;
    int hOffset;
    int vOffset;
    int hSep;
    int vSep;

    background_tileset();
    background_tileset(int tw, int th, int ho, int vo, int hs, int vs);
    background_tileset(int w,int h,int tex,bool trans,bool smth,bool prel,int tw, int th, int ho, int vo, int hs, int vs);
  };

  extern background** backgroundstructarray;
  void background_new(int bkgid, unsigned w, unsigned h, unsigned char* chunk, bool transparent, bool smoothEdges, bool preload, bool useAsTileset, int tileWidth, int tileHeight, int hOffset, int vOffset, int hSep, int vSep);
  void background_add_to_index(background *nb, std::string filename, bool transparent, bool smoothEdges, bool preload, bool mipmap);
  void background_add_copy(background *bak, background *bck_copy);
  void backgrounds_init();
  void backgroundstructarray_reallocate();
} //namespace enigma

#ifdef DEBUG_MODE
  #include "Widget_Systems/widgets_mandatory.h"
  #include "libEGMstd.h"
  #define get_background(bck2d,back)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      DEBUG_MESSAGE("Attempting to draw non-existing background " + toString(back), MESSAGE_TYPE::M_USER_ERROR);\
      return;\
    }\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      DEBUG_MESSAGE("Attempting to draw non-existing background " + toString(back), MESSAGE_TYPE::M_USER_ERROR);\
      return r;\
    }\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
#else
  #define get_background(bck2d,back)\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    enigma::background *bck2d = enigma::backgroundstructarray[back];
#endif

#include "Graphics_Systems/General/GScolor_macros.h"

#endif //ENIGMA_BACKGROUND_INTERNAL_H
