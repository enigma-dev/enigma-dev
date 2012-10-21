/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008-2011 Josh Ventura                                        **
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

#include "Collision_Systems/collision_mandatory.h"
#include "Universal_System/nlpo2.h"

#include <iostream>

namespace enigma
{
  //A non-NULL pointer is a sprite mask, a NULL pointer means bbox should be used.
  void *get_collision_mask(sprite* spr, unsigned char* input_data, collision_type ct) // It is called for every subimage of every sprite loaded.
  {
    switch (ct)
    {
      case ct_precise:
        {
          const unsigned int w = spr->width, h = spr->height;
          unsigned char* colldata = new unsigned char[w*h];

          for (unsigned int rowindex = 0; rowindex < h; rowindex++)
          {
            for(unsigned int colindex = 0; colindex < w; colindex++)
            {
              colldata[rowindex*w + colindex] = (input_data[4*(rowindex*w + colindex) + 3] != 0) ? 1 : 0; // If alpha != 0 then 1 else 0.
            }
          }
    
          return colldata;
        }
      default: return 0;
      //TODO: Generate masks for circle, ellipse and diamond, and treat them otherwise like precise.
    };
  }

  void free_collision_mask(void* mask)
  {
    if (mask != 0) {
      delete[] (unsigned char*)mask;
    }
  }
};

