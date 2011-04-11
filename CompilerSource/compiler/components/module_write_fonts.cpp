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
#include <list>

using namespace std;

#include "../../externs/externs.h"
#include "../../syntax/syncheck.h"
#include "../../parser/parser.h"

#include "../../backend/EnigmaStruct.h" //LateralGM interface structures
#include "../../parser/object_storage.h"
#include "../compile_common.h"

#include "../../backend/ideprint.h"

#include "../reshandlers/rectpack.h"

inline void writei(int x, FILE *f) {
  fwrite(&x,4,1,f);
}
inline void writef(float x, FILE *f) {
  fwrite(&x,4,1,f);
}

using namespace rect_packer;
int module_write_fonts(EnigmaStruct *es, FILE *gameModule)
{
  // Now we're going to add backgrounds
  edbg << es->fontCount << " Adding Fonts to Game Module: " << flushl;

  //Magic Number
  fwrite("rfnt",4,1,gameModule);

  //Indicate how many
  int font_count = es->fontCount;
  writei(font_count,gameModule);
  
  // For each included font
  for (int i = 0; i < font_count; i++)
  {
    cout << "Iterating included fonts..." << endl;
    // Simple allocations and initializations
    const int gc = es->fonts[i].rangeMax - es->fonts[i].rangeMin + 1;
    pvrect boxes[gc]; list<unsigned int> box_order;
    cout << "Allocated some shit" << endl;
    
    /*cout << "Font name `" << es->fonts[i].name << "` uses " << es->fonts[i].fontName << endl;
    cout << "Basic metric info: " << (es->fonts[i].rangeMax - es->fonts[i].rangeMin + 1)
         << " glyphs from " << (int)es->fonts[i].rangeMin << " to " << (int)es->fonts[i].rangeMax
         << ", glyph pointer is at " << es->fonts[i].glyphs << "." << endl;
    cout << "Dump of concerned memory: " << endl;*/
    
    for (size_t ii = 0; ii < sizeof(Font); ii++)
      printf("%02X%c",((const unsigned char*)(es->fonts+i))[ii],((ii+1)%16)?' ':'\n');
    cout << endl;
    for (int ii = 0; ii < gc; ii++)
    cout << "The dimensions of glyph " << ii << "are " << flush << es->fonts[i].glyphs[ii].width << "x" << es->fonts[i].glyphs[ii].height << "." << endl;
    cout << "End of list." << endl;
    
    // Copy our glyph metrics into it
    for (int ii = 0; ii < gc; ii++)
    cout << "The dimensions of glyph " << ii << "are " << flush, cout << es->fonts[i].glyphs[ii].width << "x" << es->fonts[i].glyphs[ii].height << "." << endl,
      boxes[ii].w = es->fonts[i].glyphs[ii].width,
      boxes[ii].h = es->fonts[i].glyphs[ii].height;
    cout << "Copied metrics" << endl;
    
    // Sort our boxes from largest to smallest in area.
    for (int ii = 0; ii < gc; ii++)
      box_order.push_back((es->fonts[i].glyphs[ii].width * es->fonts[i].glyphs[ii].height << 8) + ii); // This reserves only eight bits for the glyph id; unicode will break a little.
    box_order.sort(); // In actuality, unicode will only cause the area sort to be inaccurate, leading to an inefficient pack.
    cout << "Sorted out some shit" << endl;
    
    // Now we actually pack the mothers. We'll iterate our area-sorted list backwards (largest to smallest)
    int w = 64, h = 64;
    rectpnode *rectplane = new rectpnode(0,0,w,h);
    for (list<unsigned int>::reverse_iterator ii = box_order.rbegin(); ii != box_order.rend(); )
    {
      printf("Add rectangle %d, which is of size %d x %d, to main cell of size %d x %d (%d, %d)\n", *ii & 255, boxes[*ii & 255].w, boxes[*ii & 255].h, rectplane->wid, rectplane->hgt, w, h);
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
    
    cout << "Finished packing shit." << endl;
    
    unsigned char bigtex[w*h];
    for (int ii = 0; ii < w*h; ii++)
      bigtex[ii] = 0;
    struct { float x,y,x2,y2; } glyphtexc[gc];
    
    cout << "Allocated a big texture. Moving shit into it..." << endl;
    for (int ii = 0; ii < gc; ii++)
    {
      for (int yy = 0; yy < boxes[ii].h; yy++)
        for (int xx = 0; xx < boxes[ii].w; xx++)
          bigtex[w*(boxes[ii].y + yy) + boxes[ii].x + xx] = es->fonts[i].glyphs[ii].data[yy*boxes[ii].w + xx];
      
      glyphtexc[ii].x  = boxes[ii].x / double(w);
      glyphtexc[ii].y  = boxes[ii].y / double(h);
      glyphtexc[ii].x2 = (boxes[ii].x + boxes[ii].w) / double(w);
      glyphtexc[ii].y2 = (boxes[ii].y + boxes[ii].h) / double(h);
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
    
    for (int ii = 0; ii < gc; ii++)
      writef(es->fonts[i].glyphs[ii].advance, gameModule),
      writef(es->fonts[i].glyphs[ii].baseline,gameModule),
      writef(es->fonts[i].glyphs[ii].origin, gameModule),
      writei(es->fonts[i].glyphs[ii].width,  gameModule),
      writei(es->fonts[i].glyphs[ii].height, gameModule),
      writef(glyphtexc[ii].x,  gameModule),
      writef(glyphtexc[ii].y,  gameModule),
      writef(glyphtexc[ii].x2, gameModule),
      writef(glyphtexc[ii].y2, gameModule);
  }

  edbg << "Done writing fonts." << flushl;
  return 0;
}
