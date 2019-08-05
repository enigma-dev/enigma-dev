/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton
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

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <list>

using namespace std;

#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/GameData.h"
#include "parser/object_storage.h"
#include "compiler/compile_common.h"

#include "backend/ideprint.h"
#include "languages/lang_CPP.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}
inline void writef(float x, FILE *f) {
  fwrite(&x,4,1,f);
}

int lang_CPP::module_write_fonts(const GameData &game, FILE *gameModule) {
  // Now we're going to add backgrounds
  edbg << game.fonts.size() << " Adding Fonts to Game Module: " << flushl;

  //Magic Number
  fwrite("FNT ",4,1,gameModule);

  //Indicate how many
  int font_count = game.fonts.size();
  writei(font_count,gameModule);

  // For each included font
  for (const auto &font : game.fonts) {
    cout << "Iterating included fonts..." << endl;

    const ImageData& img = font.image_data;
    writei(img.pixels.size(), gameModule);
    fwrite(img.pixels.data(), 1, img.pixels.size(), gameModule);

    cout << "Wrote all data for font " << font.id() << endl;
  }

  edbg << "Done writing fonts." << flushl;
  return 0;
}
