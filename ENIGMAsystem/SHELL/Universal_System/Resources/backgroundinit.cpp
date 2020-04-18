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

#include "backgrounds_internal.h"
#include "libEGMstd.h"
#include "resinit.h"
#include "Universal_System/zlib.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/platforms_mandatory.h"

#include <cstring>
#include <cstdio>

namespace enigma
{
  void exe_loadbackgrounds(FILE *exe)
  {
    int nullhere;
    unsigned bkgid, width, height,transparent,smoothEdges,preload,useAsTileset,tileWidth,tileHeight,hOffset,vOffset,hSep,vSep;

    if (!fread(&nullhere, 4, 1, exe)) return;
    if (memcmp(&nullhere, "BKG ", sizeof(int)) != 0) return;

    // Determine how many backgrounds we have
    int bkgcount;
    if (!fread(&bkgcount,4,1,exe) || bkgcount == 0)
      return;

    // Fetch the highest ID we will be using
    int bkg_highid;
    if (!fread(&bkg_highid,4,1,exe))
      return;
      
    backgrounds.resize(bkg_highid+1);

    for (int i = 0; i < bkgcount; i++)
    {
      int unpacked;
      if (!fread(&bkgid, 4,1,exe)) return;
      if (!fread(&width, 4,1,exe)) return;
      if (!fread(&height,4,1,exe)) return;
      if (!fread(&transparent,4,1,exe)) return;
      if (!fread(&smoothEdges,4,1,exe)) return;
      if (!fread(&preload,4,1,exe)) return;
      if (!fread(&useAsTileset,4,1,exe)) return;
      if (!fread(&tileWidth,4,1,exe)) return;
      if (!fread(&tileHeight,4,1,exe)) return;
      if (!fread(&hOffset,4,1,exe)) return;
      if (!fread(&vOffset,4,1,exe)) return;
      if (!fread(&hSep,4,1,exe)) return;
      if (!fread(&vSep,4,1,exe)) return;

      unpacked = width*height*4;

      unsigned int size;
      if (!fread(&size,4,1,exe)){};
      
      unsigned char* cpixels=new unsigned char[size+1];
      if (!cpixels)
      {
        DEBUG_MESSAGE("Failed to load background: Cannot allocate enough memory " + enigma_user::toString(unpacked), MESSAGE_TYPE::M_ERROR);
        break;
      }
      unsigned int sz2=fread(cpixels,1,size,exe);
      if (size!=sz2) {
        DEBUG_MESSAGE("Failed to load background: Data is truncated before exe end. Read " + enigma_user::toString(sz2) + " out of expected " + enigma_user::toString(size), MESSAGE_TYPE::M_ERROR);
        return;
      }
      unsigned char* pixels=new unsigned char[unpacked+1];
      if (zlib_decompress(cpixels,size,unpacked,pixels) != unpacked)
      {
        DEBUG_MESSAGE("Background load error: Background does not match expected size", MESSAGE_TYPE::M_ERROR);
        continue;
      }
      delete[] cpixels;

      RawImage img = image_pad(pixels, width, height, nlpo2dc(width)+1, nlpo2dc(height)+1);
      int texID = graphics_create_texture(width, height, img.w, img.h, img.pxdata, false);
      Background bkg(width, height, texID, useAsTileset, tileWidth, tileHeight, hOffset, vOffset, hSep, vSep);
      backgrounds.assign(bkgid, std::move(bkg));

      delete[] pixels;
    }
  }
} //namespace enigma
