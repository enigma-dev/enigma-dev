/** Copyright (C) 2008-2013 polygone
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

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_background(bck2d,back)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#else
  #define get_background(bck2d,back)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#endif

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

// Tile system
#include "GLtiles.h"
#include "Universal_System/depth_draw.h"
#include <algorithm>
#include "GLbackground.h"
#include "Universal_System/backgroundstruct.h"
#include "GLtextures.h"
#include "binding.h"
#include "OpenGLHeaders.h"
namespace enigma
{
    void draw_tile(int back,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha)
    {
        get_background(bck2d,back);
        bind_texture(GmTextures[bck2d->texture]->gltex);

        glColor4ub(__GETR(color),__GETG(color),__GETB(color),char(alpha*255));

        float tbw = bck2d->width/(float)bck2d->texbordx, tbh = bck2d->height/(float)bck2d->texbordy,
              xvert1 = x, xvert2 = xvert1 + width*xscale,
              yvert1 = y, yvert2 = yvert1 + height*yscale,
              tbx1 = left/tbw, tbx2 = tbx1 + width/tbw,
              tby1 = top/tbh, tby2 = tby1 + height/tbh;

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(tbx1,tby1);
        glVertex2f(xvert1,yvert1);
        glTexCoord2f(tbx2,tby1);
        glVertex2f(xvert2,yvert1);
        glTexCoord2f(tbx1,tby2);
        glVertex2f(xvert1,yvert2);
        glTexCoord2f(tbx2,tby2);
        glVertex2f(xvert2,yvert2);
        glEnd();
    }

    void load_tiles()
    {
        glPushAttrib(GL_CURRENT_BIT);
        for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++)
        {
            if (dit->second.tiles.size())
            {
                sort(dit->second.tiles.begin(), dit->second.tiles.end(), bkinxcomp);
                int index = int(glGenLists(1));
                drawing_depths[dit->second.tiles[0].depth].tilelist = index;
                glNewList(index, GL_COMPILE);
                for(std::vector<tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                {
                    tile t = dit->second.tiles[i];
                    draw_background_part_ext(t.bckid, t.bgx, t.bgy, t.width, t.height, t.roomX, t.roomY, t.xscale, t.yscale, t.color, t.alpha);
                }
                glEndList();
            }
        }
        glPopAttrib();
    }
}
