/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008, 2018 Josh Ventura                                       **
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

#include "backend/GameData.h"
#include "parser/object_storage.h"
#include "compiler/compile_common.h"

#include "backend/ideprint.h"
#include "languages/lang_CPP.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}

int lang_CPP::module_write_sounds(const GameData &game, FILE *gameModule)
{
  // Now we're going to add sounds
  edbg << game.sounds.size() << " Sounds:" << flushl;
  for (size_t i = 0; i < game.sounds.size(); i++) {
    edbg << " " << game.sounds[i].name << flushl;
    fflush(stdout);
  }

  //Magic number
  fwrite("SND ",4,1,gameModule);

  //Indicate how many
  size_t sound_count = game.sounds.size();
  fwrite(&sound_count,4,1,gameModule);

  int sound_maxid = 0;
  for (size_t i = 0; i < sound_count; i++)
    if (game.sounds[i].id() > sound_maxid)
      sound_maxid = game.sounds[i].id();
  fwrite(&sound_maxid,4,1,gameModule);

  for (size_t i = 0; i < sound_count; i++) {
    const size_t sndsz = game.sounds[i].audio.size();
    if (!sndsz) {
      user << "Sound `" << game.sounds[i].name << "' has no size. "
              "It will be omitted from the game." << flushl;
      continue;
    }

    writei(game.sounds[i].id(), gameModule); // ID
    writei(sndsz, gameModule); // Size
    fwrite(game.sounds[i].audio.data(), 1, sndsz, gameModule); // Data
  }

  edbg << "Done writing sounds." << flushl;
  return 0;
}
