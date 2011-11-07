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
#include <stdio.h>
using namespace std;

#include "backgroundstruct.h"
#include "../Graphics_Systems/graphics_mandatory.h"
#include "../Platforms/platforms_mandatory.h"
#include "../Widget_Systems/widgets_mandatory.h"
#include "fontstruct.h"
#include "../libEGMstd.h"
#include "zlib.h"
#include "resinit.h"

namespace enigma
{
  void exe_loadfonts(FILE *exe)
  {
    int nullhere;
	  unsigned fontcount, fntid, twid, thgt, gwid, ghgt;
	  float advance, baseline, origin, gtx, gty, gtx2, gty2;

    if (!fread(&nullhere,4,1,exe)) return;
    if (nullhere != *(int*)"FNT ")
      return;

    if (!fread(&fontcount,4,1,exe)) return;
    if ((int)fontcount != rawfontcount) {
      show_error("Resource data does not match up with game metrics. Unable to improvise.",0);
      return;
    }

	  fontstructarray = (new font*[rawfontmaxid + 2]) + 1;

	  for (int rf = 0; rf < rawfontcount; rf++)
	  {
		  // int unpacked;
		  if (!fread(&fntid, 4,1,exe)) return;
		  if (!fread(&twid, 4,1,exe)) return;
		  if (!fread(&thgt,4,1,exe)) return;
		  const int i = fntid;

		  fontstructarray[i] = new font;

		  fontstructarray[i]->name = rawfontdata[rf].name;
		  fontstructarray[i]->fontname = rawfontdata[rf].fontname;
		  fontstructarray[i]->fontsize = rawfontdata[rf].fontsize;
		  fontstructarray[i]->bold = rawfontdata[rf].bold;
		  fontstructarray[i]->italic = rawfontdata[rf].italic;

		  fontstructarray[i]->glyphstart = rawfontdata[rf].glyphstart;
		  fontstructarray[i]->glyphcount = rawfontdata[rf].glyphcount;

		  fontstructarray[i]->height = 0;

		  fontstructarray[i]->glyphs = new fontglyph[fontstructarray[i]->glyphcount];

		  const unsigned int size = twid*thgt;

		  int* pixels=new int[size+1]; //FYI: This variable was once called "cpixels." When you do compress them, change it back.

		  unsigned int sz2;
		  for (sz2 = 0; !feof(exe) and sz2 < size; sz2++)
		    pixels[sz2] = 0x00FFFFFF | ((unsigned char)fgetc(exe) << 24);

		  if (size!=sz2) {
			  show_error("Failed to load font: Data is truncated before exe end. Read "+toString(sz2)+" out of expected "+toString(size),0);
			  return;
		  }
      if (!fread(&nullhere,4,1,exe)) return;
      if (nullhere != *(int*)"done")
      {
        printf("Unexpected end; eof:%s\n",feof(exe)?"true":"false");
        return;
      }
		  //unpacked = width*height*4;
		  /*unsigned char* pixels=new unsigned char[unpacked+1];
		  if (zlib_decompress(cpixels,size,unpacked,pixels) != unpacked)
		  {
			  show_error("Background load error: Background does not match expected size",0);
			  continue;
		  }
		  delete[] cpixels;*/

		  int ymin=100, ymax=-100;
		  for (int gi = 0; gi < enigma::fontstructarray[i]->glyphcount; gi++)
		  {
		    if (!fread(&advance,4,1,exe)) return;
        if (!fread(&baseline,4,1,exe)) return;
        if (!fread(&origin,4,1,exe)) return;
        if (!fread(&gwid,4,1,exe)) return;
        if (!fread(&ghgt,4,1,exe)) return;
        if (!fread(&gtx,4,1,exe)) return;
        if (!fread(&gty,4,1,exe)) return;
        if (!fread(&gtx2,4,1,exe)) return;
        if (!fread(&gty2,4,1,exe)) return;

        fontstructarray[i]->glyphs[gi].x = int(origin + .5);
        fontstructarray[i]->glyphs[gi].y = int(baseline + .5);
        fontstructarray[i]->glyphs[gi].x2 = int(origin + .5) + gwid;
        fontstructarray[i]->glyphs[gi].y2 = int(baseline + .5) + ghgt;
        fontstructarray[i]->glyphs[gi].tx = gtx;
        fontstructarray[i]->glyphs[gi].ty = gty;
        fontstructarray[i]->glyphs[gi].tx2 = gtx2;
        fontstructarray[i]->glyphs[gi].ty2 = gty2;
        fontstructarray[i]->glyphs[gi].xs = advance + .5;

        if (fontstructarray[i]->glyphs[gi].y < ymin)
          ymin = fontstructarray[i]->glyphs[gi].y;
        if (fontstructarray[i]->glyphs[gi].y2 > ymax)
          ymax = fontstructarray[i]->glyphs[gi].y2;
        //printf("fntid%d, twid%d, thgt%d, advance%f, baseline%f, origin%f, gwid%d, ghgt%d, gtx%f, gty%f, gtx2%f, gty2%f\n", fntid, twid, thgt, advance, baseline, origin, gwid, ghgt, gtx, gty, gtx2, gty2);
		  }
		  fontstructarray[i]->height = ymax - ymin + 2;
		  fontstructarray[i]->yoffset = - ymin + 1;

		  fontstructarray[i]->texture = graphics_create_texture(twid,thgt,pixels);
		  fontstructarray[i]->twid = twid;
		  fontstructarray[i]->thgt = thgt;

      /*int sss = 'A' - fontstructarray[i]->glyphstart;
      fontstructarray[i]->glyphs[sss].x = 0;
      fontstructarray[i]->glyphs[sss].y = 0;
      fontstructarray[i]->glyphs[sss].x2 = twid;
      fontstructarray[i]->glyphs[sss].y2 = thgt;
      fontstructarray[i]->glyphs[sss].tx = 0;
      fontstructarray[i]->glyphs[sss].ty = 0;
      fontstructarray[i]->glyphs[sss].tx2 = 1;
      fontstructarray[i]->glyphs[sss].ty2 = 1;*/

		  delete[] pixels;

      if (!fread(&nullhere,4,1,exe)) return;
      if (nullhere != *(int*)"endf")
        return;
	  }
  }
}
