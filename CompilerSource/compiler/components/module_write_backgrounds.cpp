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

#include "parser/object_storage.h"
#include "compiler/compile_common.h"

#include "backend/ideprint.h"
#include "languages/lang_CPP.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}

int lang_CPP::module_write_backgrounds(const GameData &game, FILE *gameModule)
{
  // Now we're going to add backgrounds
  edbg << game.backgrounds.size() << " Adding Backgrounds to Game Module: " << flushl;

  //Magic Number
  fwrite("BKG ",4,1,gameModule);

  //Indicate how many
  int back_count = game.backgrounds.size();
  fwrite(&back_count,4,1,gameModule);

  int back_maxid = 0;
  for (int i = 0; i < back_count; i++)
    if (game.backgrounds[i].id() > back_maxid)
      back_maxid = game.backgrounds[i].id();
  fwrite(&back_maxid,4,1,gameModule);

  for (int i = 0; i < back_count; i++)
  {
    writei(game.backgrounds[i].id(), gameModule);  // id
    writei(game.backgrounds[i].image_data.width,  gameModule);  // width
    writei(game.backgrounds[i].image_data.height, gameModule);  // height

    writei(game.backgrounds[i].legacy_transparency,   gameModule);
    writei(game.backgrounds[i]->smooth_edges(),       gameModule);
    writei(game.backgrounds[i]->preload(),            gameModule);
    writei(game.backgrounds[i]->use_as_tileset(),     gameModule);
    writei(game.backgrounds[i]->tile_width(),         gameModule);
    writei(game.backgrounds[i]->tile_height(),        gameModule);
    writei(game.backgrounds[i]->horizontal_offset(),  gameModule);
    writei(game.backgrounds[i]->vertical_offset(),    gameModule);
    writei(game.backgrounds[i]->horizontal_spacing(), gameModule);
    writei(game.backgrounds[i]->vertical_spacing(),   gameModule);

    const int sz = game.backgrounds[i].image_data.pixels.size();
    writei(sz, gameModule); // size
    fwrite(game.backgrounds[i].image_data.pixels.data(), 1, sz, gameModule); // data
  }

  edbg << "Done writing backgrounds." << flushl;
  return 0;
}
