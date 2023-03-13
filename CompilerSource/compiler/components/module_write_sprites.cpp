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

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;


#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/GameData.h"
#include "parser/object_storage.h"
#include "compiler/compile_common.h"

#include "backend/ideprint.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}

#include "languages/lang_CPP.h"
int lang_CPP::module_write_sprites(const GameData &game, FILE *gameModule)
{
  // Now we're going to add sprites
  edbg << game.sprites.size() << " Adding Sprites to Game Module: " << flushl;

  //Magic Number
  fwrite("SPR ",4,1,gameModule);

  //Indicate how many
  int sprite_count = game.sprites.size();
  fwrite(&sprite_count,4,1,gameModule);

  int sprite_maxid = 0;
  for (int i = 0; i < sprite_count; i++)
    if (game.sprites[i].id() > sprite_maxid)
      sprite_maxid = game.sprites[i].id();
  fwrite(&sprite_maxid,4,1,gameModule);

  for (int i = 0; i < sprite_count; i++)
  {
    writei(game.sprites[i].id(), gameModule); //id

    // Track how many subImages we're copying
    int subCount = game.sprites[i].image_data.size();

    int swidth = 0, sheight = 0;
    for (int ii = 0; ii < subCount; ii++)
    {
      if (!swidth and !sheight) {
        swidth =  game.sprites[i].image_data[ii].width;
        sheight = game.sprites[i].image_data[ii].height;
      }
      else if (swidth != game.sprites[i].image_data[ii].width
           || sheight != game.sprites[i].image_data[ii].height) {
        user << "Subimages of sprite `" << game.sprites[i].name << "' vary in dimensions; do not want." << flushl;
        return 14;
      }
    }
    if (!(swidth and sheight and subCount)) {
      user << "Subimages of sprite `" << game.sprites[i].name << "' have zero size." << flushl;
      return 14;
    }

    writei(swidth, gameModule); //width
    writei(sheight,gameModule); //height
    writei(game.sprites[i]->origin_x(),  gameModule); // xorig
    writei(game.sprites[i]->origin_y(),  gameModule); // yorig
    writei(game.sprites[i]->bbox_top(),    gameModule); // BBox Top
    writei(game.sprites[i]->bbox_bottom(), gameModule); // BBox Bottom
    writei(game.sprites[i]->bbox_left(),   gameModule); // BBox Left
    writei(game.sprites[i]->bbox_right(),  gameModule); // BBox Right
    writei(game.sprites[i]->bbox_mode(),   gameModule); // BBox Mode
    writei(game.sprites[i]->shape(),    gameModule); // Mask shape

    writei(subCount,gameModule); //subimages

    for (int ii = 0;ii < subCount; ii++)
    {
      //strans = game.sprites[i].image_data[ii].transColor, fwrite(&idttrans,4,1,exe); //Transparent color
      writei(swidth * sheight * 4, gameModule); // size when unpacked
      writei(game.sprites[i].image_data[ii].pixels.size(), gameModule);  // size
      fwrite(game.sprites[i].image_data[ii].pixels.data(), 1,
             game.sprites[i].image_data[ii].pixels.size(), gameModule);  // data
      writei(0,gameModule);
    }
  }

  edbg << "Done writing sprites." << flushl;
  return 0;
}
