/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
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
    double texbordx, texbordy;

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
  void background_add_to_index(background *nb, std::string filename, bool transparent, bool smoothEdges, bool preload);
  void background_add_copy(background *bak, background *bck_copy);
  void backgrounds_init();
}

namespace enigma_user
{

int background_add(std::string filename, bool transparent = false, bool smooth = false, bool preload = false);
bool background_replace(int back, std::string filename, bool transparent = false, bool smooth = false, bool preload = false, bool free_texture = true);
void background_delete(int back, bool free_texture = true);
int background_duplicate(int back);
void background_assign(int back, int copy_background, bool free_texture = true);
bool background_exists(int back);
void background_set_alpha_from_background(int back, int copy_background, bool free_texture = true);

}

