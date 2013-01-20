/**
  @file  module_write_fonts.cpp
  @brief Implements the method in the C++ plugin that writes fonts to a game's
         resource section.
  
  @section License
    Copyright (C) 2008-2013 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
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
int lang_CPP::resource_writer_cpp::module_write_fonts(compile_context &ctex)
{
  // Now we're going to add backgrounds
  edbg << ctex.es->fontCount << " Adding Fonts to Game Module: " << flushl;

  //Magic Number
  fwrite("FNT ",4,1,gameModule);

  //Indicate how many
  int font_count = ctex.es->fontCount;
  writei(font_count,gameModule);

  // For each included font
  for (int i = 0; i < font_count; i++)
  {
    cout << "Iterating included fonts..." << endl;
    
    // Simple allocations and initializations
    const int gc = ctex.es->fonts[i].rangeMax - ctex.es->fonts[i].rangeMin + 1;
    pvrect *boxes = new pvrect[gc];
    list<unsigned int> box_order;
    cout << "Allocated some font stuff" << endl;

    /*cout << "Font name `" << ctex.es->fonts[i].name << "` uses " << ctex.es->fonts[i].fontName << endl;
    cout << "Basic metric info: " << (ctex.es->fonts[i].rangeMax - ctex.es->fonts[i].rangeMin + 1)
         << " glyphs from " << (int)ctex.es->fonts[i].rangeMin << " to " << (int)ctex.es->fonts[i].rangeMax
         << ", glyph pointer is at " << ctex.es->fonts[i].glyphs << "." << endl;
    cout << "Dump of concerned memory: " << endl;

    for (size_t ii = 0; ii < sizeof(Font); ii++)
      printf("%02X%c",((const unsigned char*)(ctex.es->fonts+i))[ii],((ii+1)%16)?' ':'\n');
    cout << endl;
    for (int ii = 0; ii < gc; ii++)
    cout << "The dimensions of glyph " << ii << "are " << flush << ctex.es->fonts[i].glyphs[ii].width << "x" << ctex.es->fonts[i].glyphs[ii].height << "." << endl;
    cout << "End of list." << endl;*/

    // Copy our glyph metrics into it
    for (int ii = 0; ii < gc; ii++)
    //cout << "The dimensions of glyph " << ii << "are " << flush, cout << ctex.es->fonts[i].glyphs[ii].width << "x" << ctex.es->fonts[i].glyphs[ii].height << "." << endl,
      boxes[ii].w = ctex.es->fonts[i].glyphs[ii].width,
      boxes[ii].h = ctex.es->fonts[i].glyphs[ii].height;
    cout << "Copied metrics" << endl;

    // Sort our boxes from largest to smallest in area.
    for (int ii = 0; ii < gc; ii++)
      box_order.push_back((ctex.es->fonts[i].glyphs[ii].width * ctex.es->fonts[i].glyphs[ii].height << 8) + ii); // This reserves only eight bits for the glyph id; unicode will break a little.
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

    unsigned char *bigtex = new unsigned char[w*h];
    for (int ii = 0; ii < w*h; ii++)
      bigtex[ii] = 0;
    struct glyphtexcs { float x,y,x2,y2; };
    glyphtexcs *glyphtexc = new glyphtexcs[gc];

    cout << "Allocated a big texture. Moving font into it..." << endl;
    for (int ii = 0; ii < gc; ii++)
    {
      for (int yy = 0; yy < ctex.es->fonts[i].glyphs[ii].height; yy++)
        for (int xx = 0; xx < ctex.es->fonts[i].glyphs[ii].width; xx++)
          bigtex[w*(boxes[ii].y + yy) + boxes[ii].x + xx] = ctex.es->fonts[i].glyphs[ii].data[yy*ctex.es->fonts[i].glyphs[ii].width + xx];

      glyphtexc[ii].x  = boxes[ii].x / double(w);
      glyphtexc[ii].y  = boxes[ii].y / double(h);
      glyphtexc[ii].x2 = (boxes[ii].x + ctex.es->fonts[i].glyphs[ii].width) / double(w);
      glyphtexc[ii].y2 = (boxes[ii].y + ctex.es->fonts[i].glyphs[ii].height) / double(h);
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

    writei(ctex.es->fonts[i].id,gameModule);
    writei(w,gameModule), writei(h,gameModule);
    fwrite(bigtex,1,w*h,gameModule);
    fwrite("done",1,4,gameModule);
    
    for (int ii = 0; ii < gc; ii++)
      writef(ctex.es->fonts[i].glyphs[ii].advance, gameModule),
      writef(ctex.es->fonts[i].glyphs[ii].baseline,gameModule),
      writef(ctex.es->fonts[i].glyphs[ii].origin, gameModule),
      writei(ctex.es->fonts[i].glyphs[ii].width,  gameModule),
      writei(ctex.es->fonts[i].glyphs[ii].height, gameModule),
      writef(glyphtexc[ii].x,  gameModule),
      writef(glyphtexc[ii].y,  gameModule),
      writef(glyphtexc[ii].x2, gameModule),
      writef(glyphtexc[ii].y2, gameModule);

    fwrite("endf",1,4,gameModule);
    
    delete boxes;
    delete bigtex;
    delete glyphtexc;
    
    cout << "Wrote all data for font " << i << endl;
  }

  edbg << "Done writing fonts." << flushl;
  return 0;
}
