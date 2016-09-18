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

#include <string>
#include "var4.h"

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
}

namespace enigma_user
{

int background_add(std::string filename, bool transparent = false, bool smooth = false, bool preload = true, bool mipmap = false);
int background_create_color(unsigned w, unsigned h, int col, bool preload = true);
bool background_replace(int back, std::string filename, bool transparent = false, bool smooth = false, bool preload = true, bool free_texture = true, bool mipmap = false);
void background_save(int back, std::string fname);
void background_delete(int back, bool free_texture = true);
int background_duplicate(int back);
void background_assign(int back, int copy_background, bool free_texture = true);
bool background_exists(int back);
void background_set_alpha_from_background(int back, int copy_background, bool free_texture = true);
int background_get_texture(int backId);
int background_get_width(int backId);
int background_get_height(int backId);
var sprite_get_uvs(int backId);
}

