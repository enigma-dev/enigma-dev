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
#include "Platforms/General/fileio.h"

#include <cstring>
#include <string>

namespace enigma {

void exe_loadfonts(FILE_t* exe) {
  int nullhere, fntid;
  unsigned fontcount, twid, thgt, gwid, ghgt;
  float advance, baseline, origin, gtx, gty, gtx2, gty2;

  if (!fread_wrapper(&nullhere, 4, 1, exe)) return;
  if (memcmp(&nullhere, "FNT ", sizeof(int)) != 0) return;

  if (!fread_wrapper(&fontcount, 4, 1, exe)) return;
  if ((int)fontcount != rawfontcount) {
    DEBUG_MESSAGE("Resource data does not match up with game metrics. Unable to improvise.", MESSAGE_TYPE::M_ERROR);
    return;
  }

  sprite_fonts.resize(rawfontmaxid+1);

  for (int rf = 0; rf < rawfontcount; rf++) {
    // int unpacked;
    if (!fread_wrapper(&fntid, 4, 1, exe)) return;
    if (!fread_wrapper(&twid, 4, 1, exe)) return;
    if (!fread_wrapper(&thgt, 4, 1, exe)) return;

    SpriteFont font;

    font.name = rawfontdata[rf].name;
    font.fontname = rawfontdata[rf].fontname;
    font.fontsize = rawfontdata[rf].fontsize;
    font.bold = rawfontdata[rf].bold;
    font.italic = rawfontdata[rf].italic;

    font.height = 0;

    const unsigned int size = twid * thgt;
    unsigned char* mono = new unsigned char[size];
    if (!fread_wrapper(&mono[0], sizeof(char), size, exe)) return;
    
    unsigned char* pixels = mono_to_rgba(mono, twid, thgt);
    delete[] mono;

    if (!fread_wrapper(&nullhere, 4, 1, exe)) return;
    if (memcmp(&nullhere, "done", sizeof(int)) != 0) {
      DEBUG_MESSAGE(std::string("Unexpected end; eof: ") + ((feof_wrapper(exe) == 0) ? "true" : "false"), MESSAGE_TYPE::M_ERROR);
      return;
    }

    int ymin = 100, ymax = -100;
    for (size_t gri = 0; gri < rawfontdata[rf].glyphRangeCount; gri++) {
      fontglyphrange fgr;

      unsigned strt, cnt;
      if (!fread_wrapper(&strt, 4, 1, exe)) return;
      if (!fread_wrapper(&cnt, 4, 1, exe)) return;

      fgr.glyphstart = strt;

      for (unsigned gi = 0; gi < cnt; gi++) {
        if (!fread_wrapper(&advance, 4, 1, exe)) return;
        if (!fread_wrapper(&baseline, 4, 1, exe)) return;
        if (!fread_wrapper(&origin, 4, 1, exe)) return;
        if (!fread_wrapper(&gwid, 4, 1, exe)) return;
        if (!fread_wrapper(&ghgt, 4, 1, exe)) return;
        if (!fread_wrapper(&gtx, 4, 1, exe)) return;
        if (!fread_wrapper(&gty, 4, 1, exe)) return;
        if (!fread_wrapper(&gtx2, 4, 1, exe)) return;
        if (!fread_wrapper(&gty2, 4, 1, exe)) return;
        fontglyph fg;

        fg.x = round(origin);
        fg.y = round(baseline);
        fg.x2 = round(origin) + gwid;
        fg.y2 = round(baseline) + ghgt;
        fg.tx = gtx;
        fg.ty = gty;
        fg.tx2 = gtx2;
        fg.ty2 = gty2;
        fg.xs = advance;

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

    if (!fread_wrapper(&nullhere, 4, 1, exe)) return;
    if (memcmp(&nullhere, "endf", sizeof(int)) != 0) return;
  }
}
}  //namespace enigma
