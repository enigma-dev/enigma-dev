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

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "parser/object_storage.h"
#include "compiler/compile_common.h"

#include "backend/ideprint.h"

#include "compiler/reshandlers/rectpack.h"
#include "languages/lang_CPP.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}
inline void writef(float x, FILE *f) {
  fwrite(&x,4,1,f);
}

using namespace rect_packer;
int lang_CPP::module_write_fonts(EnigmaStruct *es, FILE *gameModule)
{
  // Now we're going to add backgrounds
  edbg << es->fontCount << " Adding Fonts to Game Module: " << flushl;

  //Magic Number
  fwrite("FNT ",4,1,gameModule);

  //Indicate how many
  int font_count = es->fontCount;
  writei(font_count,gameModule);

  // For each included font
  for (int i = 0; i < font_count; i++)
  {
    cout << "Iterating included fonts..." << endl;
    // Simple allocations and initializations
    size_t gc = 0;
    for (int ii = 0; ii < es->fonts[i].glyphRangeCount; ii++) {
      GlyphRange &glyphRange = es->fonts[i].glyphRanges[ii];
      gc += glyphRange.rangeMax - glyphRange.rangeMin + 1 + 1;
    }
    pvrect* boxes = new pvrect[gc];
    list<unsigned int> box_order;
    cout << "Allocated some font stuff" << endl;

    /*cout << "Font name `" << es->fonts[i].name << "` uses " << es->fonts[i].fontName << endl;
    cout << "Basic metric info: " << (es->fonts[i].rangeMax - es->fonts[i].rangeMin + 1)
         << " glyphs from " << (int)es->fonts[i].rangeMin << " to " << (int)es->fonts[i].rangeMax
         << ", glyph pointer is at " << es->fonts[i].glyphs << "." << endl;
    cout << "Dump of concerned memory: " << endl;

    for (size_t ii = 0; ii < sizeof(Font); ii++)
      printf("%02X%c",((const unsigned char*)(es->fonts+i))[ii],((ii+1)%16)?' ':'\n');
    cout << endl;
    for (int ii = 0; ii < gc; ii++)
    cout << "The dimensions of glyph " << ii << "are " << flush << es->fonts[i].glyphs[ii].width << "x" << es->fonts[i].glyphs[ii].height << "." << endl;
    cout << "End of list." << endl;*/
	
	

    // Copy our glyph metrics into it
    size_t ib = 0;
    for (int ii = 0; ii < es->fonts[i].glyphRangeCount; ii++) {
      GlyphRange &glyphRange = es->fonts[i].glyphRanges[ii];
      for (int ig = 0; ig < glyphRange.rangeMax - glyphRange.rangeMin + 1; ig++) {
        Glyph &glyph = es->fonts[i].glyphRanges[ii].glyphs[ig];
        boxes[ib].w = glyph.width,
        boxes[ib].h = glyph.height;
        ib++;
      }
    }
    cout << "Copied metrics" << endl;

    // Sort our boxes from largest to smallest in area.
    size_t bo = 0;
    for (int ii = 0; ii < es->fonts[i].glyphRangeCount; ii++) {
      GlyphRange &glyphRange = es->fonts[i].glyphRanges[ii];
      for (int ig = 0; ig < glyphRange.rangeMax - glyphRange.rangeMin + 1; ig++) {
        Glyph &glyph = es->fonts[i].glyphRanges[ii].glyphs[ig];
        box_order.push_back((glyph.width * glyph.height << 8) + bo); // This reserves only eight bits for the glyph id; unicode will break a little.
        bo++;
      }
    }
    box_order.sort(); // In actuality, unicode will only cause the area sort to be inaccurate, leading to an inefficient pack.
    cout << "Sorted out some font stuff" << endl;

    // Now we actually pack the mothers. We'll iterate our area-sorted list backwards (largest to smallest)
    int w = 64, h = 64;
    rectpnode *rectplane = new rectpnode(0,0,w,h);
    for (list<unsigned int>::reverse_iterator ii = box_order.rbegin(); ii != box_order.rend(); )
    {
      rectpnode *nn = rninsert(rectplane, *ii & 0xFF, boxes);
      if (nn)
        rncopy(nn, boxes, *ii & 0xFF),
        ii++;
      else
      {
        w > h ? h <<= 1 : w <<= 1,
        rectplane = expand(rectplane, w, h);
        printf("Expanded to %d by %d\n", w, h);
        if (!w or !h) return -1;
      }
    }

    cout << "Finished packing font stuff." << endl;

    unsigned char bigtex[w*h];
    for (int ii = 0; ii < w*h; ii++)
      bigtex[ii] = 0;
    struct { float x,y,x2,y2; } glyphtexc[gc];

    cout << "Allocated a big texture. Moving font into it..." << endl;
    size_t igt = 0;
    for (int ii = 0; ii < es->fonts[i].glyphRangeCount; ii++) {
      GlyphRange &glyphRange = es->fonts[i].glyphRanges[ii];
      for (int ig = 0; ig < glyphRange.rangeMax - glyphRange.rangeMin + 1; ig++) {
        Glyph &glyph = es->fonts[i].glyphRanges[ii].glyphs[ig];
        
        for (int yy = 0; yy < glyph.height; yy++)
          for (int xx = 0; xx < glyph.width; xx++)
            bigtex[w*(boxes[igt].y + yy) + boxes[igt].x + xx] = glyph.data[yy * glyph.width + xx];

        glyphtexc[igt].x  = boxes[igt].x / double(w);
        glyphtexc[igt].y  = boxes[igt].y / double(h);
        glyphtexc[igt].x2 = (boxes[igt].x + glyph.width) / double(w);
        glyphtexc[igt].y2 = (boxes[igt].y + glyph.height) / double(h);
        igt++;
      }
    }

    /*cout << "Populated. Debugging..." << endl;
    FILE* sex = fopen("/home/josh/Desktop/lol.txt","wb");
    char grades[] = " =COBM";
    for (int yy = 0; yy < h; yy++)
    {
      for (int xx = 0; xx < w; xx++)
        fputc(grades[bigtex[(yy*w+xx)]*5/255],sex);
      fputc('\n',sex);
    }
    fclose(sex);*/

    writei(es->fonts[i].id,gameModule);
    writei(w,gameModule), writei(h,gameModule);
    fwrite(bigtex,1,w*h,gameModule);
    fwrite("done",1,4,gameModule);
	
    igt = 0;
    for (int ii = 0; ii < es->fonts[i].glyphRangeCount; ii++) {
      GlyphRange &glyphRange = es->fonts[i].glyphRanges[ii];
      writei(glyphRange.rangeMin, gameModule);
      unsigned gc = glyphRange.rangeMax - glyphRange.rangeMin + 1;
      writei(gc, gameModule);
      for (unsigned ig = 0; ig < gc; ig++) {
        Glyph &glyph = glyphRange.glyphs[ig];
        writef(glyph.advance, gameModule);
        writef(glyph.baseline,gameModule);
        writef(glyph.origin, gameModule);
        writei(glyph.width,  gameModule);
        writei(glyph.height, gameModule);
        
        writef(glyphtexc[igt].x,  gameModule),
        writef(glyphtexc[igt].y,  gameModule),
        writef(glyphtexc[igt].x2, gameModule),
        writef(glyphtexc[igt].y2, gameModule);
        igt++;
      }
    }


    fwrite("endf",1,4,gameModule);
    cout << "Wrote all data for font " << i << endl;
    delete[] boxes;
  }

  edbg << "Done writing fonts." << flushl;
  return 0;
}
