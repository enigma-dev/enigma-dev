/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
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

#include "backgrounds_internal.h"
#include "fonts_internal.h"
#include "libEGMstd.h"
#include "resinit.h"
#include "Universal_System/zlib.h"

#include "Graphics_Systems/graphics_mandatory.h"
#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <cstring>
#include <cstdio>
#include <string>

namespace enigma {

void exe_loadfonts(FILE* exe) {
  int nullhere, fntid;
  unsigned fontcount, twid, thgt, gwid, ghgt;
  float advance, baseline, origin, gtx, gty, gtx2, gty2;

  if (!fread(&nullhere, 4, 1, exe)) return;
  if (memcmp(&nullhere, "FNT ", sizeof(int)) != 0) return;

  if (!fread(&fontcount, 4, 1, exe)) return;
  if ((int)fontcount != rawfontcount) {
    DEBUG_MESSAGE("Resource data does not match up with game metrics. Unable to improvise.", MESSAGE_TYPE::M_ERROR);
    return;
  }

  sprite_fonts.resize(rawfontmaxid+1);

  for (int rf = 0; rf < rawfontcount; rf++) {
    // int unpacked;
    if (!fread(&fntid, 4, 1, exe)) return;
    if (!fread(&twid, 4, 1, exe)) return;
    if (!fread(&thgt, 4, 1, exe)) return;

    SpriteFont font;

    font.name = rawfontdata[rf].name;
    font.fontname = rawfontdata[rf].fontname;
    font.fontsize = rawfontdata[rf].fontsize;
    font.bold = rawfontdata[rf].bold;
    font.italic = rawfontdata[rf].italic;

    font.height = 0;

    const unsigned int size = twid * thgt;
    unsigned char* mono = new unsigned char[size];
    if (!fread(&mono[0], sizeof(char), size, exe)) return;
    
    unsigned char* pixels = mono_to_rgba(mono, twid, thgt);
    delete[] mono;

    if (!fread(&nullhere, 4, 1, exe)) return;
    if (memcmp(&nullhere, "done", sizeof(int)) != 0) {
      DEBUG_MESSAGE(std::string("Unexpected end; eof: ") + ((feof(exe) == 0) ? "true" : "false"), MESSAGE_TYPE::M_ERROR);
      return;
    }

    int ymin = 100, ymax = -100;
    for (size_t gri = 0; gri < rawfontdata[rf].glyphRangeCount; gri++) {
      fontglyphrange fgr;

      unsigned strt, cnt;
      if (!fread(&strt, 4, 1, exe)) return;
      if (!fread(&cnt, 4, 1, exe)) return;

      fgr.glyphstart = strt;

      for (unsigned gi = 0; gi < cnt; gi++) {
        if (!fread(&advance, 4, 1, exe)) return;
        if (!fread(&baseline, 4, 1, exe)) return;
        if (!fread(&origin, 4, 1, exe)) return;
        if (!fread(&gwid, 4, 1, exe)) return;
        if (!fread(&ghgt, 4, 1, exe)) return;
        if (!fread(&gtx, 4, 1, exe)) return;
        if (!fread(&gty, 4, 1, exe)) return;
        if (!fread(&gtx2, 4, 1, exe)) return;
        if (!fread(&gty2, 4, 1, exe)) return;
        fontglyph fg;

        fg.x = int(origin + .5);
        fg.y = int(baseline + .5);
        fg.x2 = int(origin + .5) + gwid;
        fg.y2 = int(baseline + .5) + ghgt;
        fg.tx = gtx;
        fg.ty = gty;
        fg.tx2 = gtx2;
        fg.ty2 = gty2;
        fg.xs = advance + .5;

        if (fg.y < ymin) ymin = fg.y;
        if (fg.y2 > ymax) ymax = fg.y2;

        fgr.glyphs.push_back(fg);
      }

      font.glyphRanges.push_back(std::move(fgr));
    }

    font.height = ymax - ymin + 2;
    font.yoffset = -ymin + 1;

    font.texture = graphics_create_texture(RawImage(pixels, twid, thgt), false);
    font.twid = twid;
    font.thgt = thgt;

    sprite_fonts[fntid] = std::move(font);

    if (!fread(&nullhere, 4, 1, exe)) return;
    if (memcmp(&nullhere, "endf", sizeof(int)) != 0) return;
  }
}
}  //namespace enigma
