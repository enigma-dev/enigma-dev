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
#include "Platforms/General/fileio.h"

#include <cstring>

namespace enigma
{
  void exe_loadbackgrounds(FILE_t *exe)
  {
    int nullhere;
    unsigned bkgid, width, height,transparent,smoothEdges,preload,useAsTileset,tileWidth,tileHeight,hOffset,vOffset,hSep,vSep;

    if (!fread_wrapper(&nullhere, 4, 1, exe)) return;
    if (memcmp(&nullhere, "BKG ", sizeof(int)) != 0) return;

    // Determine how many backgrounds we have
    int bkgcount;
    if (!fread_wrapper(&bkgcount,4,1,exe))
      return;

    // Fetch the highest ID we will be using
    int bkg_highid;
    if (!fread_wrapper(&bkg_highid,4,1,exe))
      return;
    
    if (bkgcount == 0) return;
    backgrounds.resize(bkg_highid+1);

    for (int i = 0; i < bkgcount; i++)
    {
      int unpacked;
      if (!fread_wrapper(&bkgid, 4,1,exe)) return;
      if (!fread_wrapper(&width, 4,1,exe)) return;
      if (!fread_wrapper(&height,4,1,exe)) return;
      if (!fread_wrapper(&transparent,4,1,exe)) return;
      if (!fread_wrapper(&smoothEdges,4,1,exe)) return;
      if (!fread_wrapper(&preload,4,1,exe)) return;
      if (!fread_wrapper(&useAsTileset,4,1,exe)) return;
      if (!fread_wrapper(&tileWidth,4,1,exe)) return;
      if (!fread_wrapper(&tileHeight,4,1,exe)) return;
      if (!fread_wrapper(&hOffset,4,1,exe)) return;
      if (!fread_wrapper(&vOffset,4,1,exe)) return;
      if (!fread_wrapper(&hSep,4,1,exe)) return;
      if (!fread_wrapper(&vSep,4,1,exe)) return;

      unpacked = width*height*4;

      unsigned int size;
      if (!fread_wrapper(&size,4,1,exe)){};
      
      unsigned char* cpixels=new unsigned char[size+1];
      if (!cpixels)
      {
        DEBUG_MESSAGE("Failed to load background: Cannot allocate enough memory " + enigma_user::toString(unpacked), MESSAGE_TYPE::M_ERROR);
        break;
      }
      unsigned int sz2=fread_wrapper(cpixels,1,size,exe);
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

      unsigned fw, fh;
      int texID = graphics_create_texture(RawImage(pixels, width, height), false, &fw, &fh);
      Background bkg(width, height, fw, fh, texID, useAsTileset, tileWidth, tileHeight, hOffset, vOffset, hSep, vSep);
      backgrounds.assign(bkgid, std::move(bkg));
    }
  }
} //namespace enigma
