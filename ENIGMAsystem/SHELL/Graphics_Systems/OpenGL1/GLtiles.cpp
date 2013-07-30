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

// Tile system
#include "Universal_System/depth_draw.h"
#include <algorithm>
#include "../General/GSbackground.h"
#include "Universal_System/backgroundstruct.h"
#include "../General/GStextures.h"
#include "../General/GLTextureStruct.h"
#include "../General/GStiles.h"
#include "../General/GLtilestruct.h"
#include "../General/GLbinding.h"
#include "../General/OpenGLHeaders.h"

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

namespace enigma
{
    static void draw_tile(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
    {
        if (!enigma_user::background_exists(back)) return;
        get_background(bck2d,back);
        texture_use(GmTextures[bck2d->texture]->gltex);

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
            if (dit->second.tiles.size())
            {
                texture_reset();
                sort(dit->second.tiles.begin(), dit->second.tiles.end(), bkinxcomp);
                int index = int(glGenLists(1));
                drawing_depths[dit->second.tiles[0].depth].tilelist = index;
                glNewList(index, GL_COMPILE);
                for(std::vector<tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                {
                    tile t = dit->second.tiles[i];
                    draw_tile(t.bckid, t.bgx, t.bgy, t.width, t.height, t.roomX, t.roomY, t.xscale, t.yscale, t.color, t.alpha);
                }
                glEndList();
            }
        glPopAttrib();
    }

    void delete_tiles()
    {
        for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++)
            if (dit->second.tiles.size())
                glDeleteLists(drawing_depths[dit->second.tiles[0].depth].tilelist, 1);
    }

    void rebuild_tile_layer(int layer_depth)
    {
        glPushAttrib(GL_CURRENT_BIT);
        texture_reset();
        for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++)
            if (dit->second.tiles.size())
            {
                if (dit->second.tiles[0].depth != layer_depth)
                    continue;

                texture_reset();
                glDeleteLists(drawing_depths[dit->second.tiles[0].depth].tilelist, 1);
                int index = int(glGenLists(1));
                drawing_depths[dit->second.tiles[0].depth].tilelist = index;
                glNewList(index, GL_COMPILE);
                for(std::vector<tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                {
                    tile t = dit->second.tiles[i];
                    draw_tile(t.bckid, t.bgx, t.bgy, t.width, t.height, t.roomX, t.roomY, t.xscale, t.yscale, t.color, t.alpha);
                }
                glEndList();
            }
        glPopAttrib();
    }
}

namespace enigma_user
{

int tile_add(int background, int left, int top, int width, int height, int x, int y, int depth, int alpha, int color)
{
    enigma::tile *ntile = new enigma::tile;
    ntile->id = enigma::maxtileid++;
    ntile->bckid = background;
    ntile->bgx = left;
    ntile->bgy = top;
    ntile->width = width;
    ntile->height = height;
    ntile->roomX = x;
    ntile->roomY = y;
    ntile->depth = depth;
    ntile->alpha = alpha;
    ntile->color = color;
    enigma::drawing_depths[ntile->depth].tiles.push_back(*ntile);
    enigma::rebuild_tile_layer(ntile->depth);
    return ntile->id;
}

bool tile_delete(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
            {
                enigma::tile t = dit->second.tiles[i];
                if (t.id == id)
                {
                    enigma::drawing_depths[t.depth].tiles.erase(enigma::drawing_depths[t.depth].tiles.begin() + i);
                    enigma::rebuild_tile_layer(t.depth);
                    return true;
                }
            }
    return false;
}

bool tile_exists(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return true;
    return false;
}

double tile_get_alpha(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].alpha;
    return 0;
}

int tile_get_background(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].bckid;
    return 0;
}

int tile_get_blend(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].color;
    return 0;
}

int tile_get_depth(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].depth;
    return 0;
}

int tile_get_height(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].height;
    return 0;
}

int tile_get_left(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].bgx;
    return 0;
}

int tile_get_top(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].bgy;
    return 0;
}

double tile_get_visible(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return (dit->second.tiles[i].alpha > 0);
    return 0;
}

bool tile_get_width(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].width;
    return 0;
}

int tile_get_x(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].roomX;
    return 0;
}

int tile_get_xscale(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].xscale;
    return 0;
}

int tile_get_y(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].roomY;
    return 0;
}

int tile_get_yscale(int id)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                    return dit->second.tiles[i].yscale;
    return 0;
}

bool tile_set_alpha(int id, double alpha)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                {
                    dit->second.tiles[i].alpha = alpha;
                    enigma::rebuild_tile_layer(dit->second.tiles[i].depth);
                    return true;
                }
    return false;
}

bool tile_set_background(int id, int background)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                {
                    dit->second.tiles[i].bckid = background;
                    enigma::rebuild_tile_layer(dit->second.tiles[i].depth);
                    return true;
                }
    return false;
}

bool tile_set_blend(int id, int color)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                {
                    dit->second.tiles[i].color = color;
                    enigma::rebuild_tile_layer(dit->second.tiles[i].depth);
                    return true;
                }
    return false;
}

bool tile_set_position(int id, int x, int y)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                {
                    dit->second.tiles[i].roomX = x;
                    dit->second.tiles[i].roomY = y;
                    enigma::rebuild_tile_layer(dit->second.tiles[i].depth);
                    return true;
                }
    return false;
}

bool tile_set_region(int id, int left, int top, int width, int height)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                {
                    dit->second.tiles[i].bgx = left;
                    dit->second.tiles[i].bgy = top;
                    dit->second.tiles[i].width = width;
                    dit->second.tiles[i].height = height;
                    enigma::rebuild_tile_layer(dit->second.tiles[i].depth);
                    return true;
                }
    return false;
}

bool tile_set_scale(int id, int xscale, int yscale)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                {
                    dit->second.tiles[i].xscale = xscale;
                    dit->second.tiles[i].yscale = yscale;
                    enigma::rebuild_tile_layer(dit->second.tiles[i].depth);
                    return true;
                }
    return false;
}

bool tile_set_visible(int id, bool visible)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
                if (dit->second.tiles[i].id == id)
                {
                    dit->second.tiles[i].alpha = visible?1:0;
                    enigma::rebuild_tile_layer(dit->second.tiles[i].depth);
                    return true;
                }
    return false;
}

bool tile_set_depth(int id, int depth)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
            {
                enigma::tile t = dit->second.tiles[i];
                if (t.id == id)
                {
                    enigma::drawing_depths[t.depth].tiles.erase(enigma::drawing_depths[t.depth].tiles.begin() + i);
                    enigma::rebuild_tile_layer(t.depth);
                    t.depth = depth;
                    enigma::drawing_depths[t.depth].tiles.push_back(t);
                    enigma::rebuild_tile_layer(t.depth);
                    return true;
                }
            }
    return false;
}

bool tile_layer_delete(int layer_depth)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
        {
            if (dit->second.tiles[0].depth != layer_depth)
                continue;
            glDeleteLists(enigma::drawing_depths[dit->second.tiles[0].depth].tilelist, 1);
            dit->second.tiles.clear();
            return true;
        }
    return false;
}

bool tile_layer_delete_at(int layer_depth, int x, int y)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
        {
            if (dit->second.tiles[0].depth != layer_depth)
                continue;
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
            {
                enigma::tile t = dit->second.tiles[i];
                if (t.roomX == x && t.roomY == y)
                    enigma::drawing_depths[t.depth].tiles.erase(enigma::drawing_depths[t.depth].tiles.begin() + i);
            }
            enigma::rebuild_tile_layer(layer_depth);
            return true;
        }
    return false;
}

bool tile_layer_depth(int layer_depth, int depth)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
        {
            if (dit->second.tiles[0].depth != layer_depth)
                continue;
            enigma::drawing_depths[depth].tilelist = enigma::drawing_depths[layer_depth].tilelist;
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
            {
                enigma::tile t = dit->second.tiles[i];
                t.depth = depth;
                enigma::drawing_depths[t.depth].tiles.push_back(t);
            }
            enigma::drawing_depths[layer_depth].tiles.clear();
            enigma::rebuild_tile_layer(depth);
            return true;
        }
    return false;
}

int tile_layer_find(int layer_depth, int x, int y)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
        {
            if (dit->second.tiles[0].depth != layer_depth)
                continue;
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
            {
                enigma::tile t = dit->second.tiles[i];
                if (t.roomX == x && t.roomY == y)
                    return t.id;
            }
        }
    return -1;
}

bool tile_layer_hide(int layer_depth)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
        {
            if (dit->second.tiles[0].depth != layer_depth)
                continue;
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
            {
                enigma::tile &t = dit->second.tiles[i];
                t.alpha = 0;
            }
            return true;
        }
    return false;
}

bool tile_layer_show(int layer_depth)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
        {
            if (dit->second.tiles[0].depth != layer_depth)
                continue;
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
            {
                enigma::tile &t = dit->second.tiles[i];
                t.alpha = 1;
            }
            return true;
        }
    return false;
}

bool tile_layer_shift(int layer_depth, int x, int y)
{
    for (enigma::diter dit = enigma::drawing_depths.rbegin(); dit != enigma::drawing_depths.rend(); dit++)
        if (dit->second.tiles.size())
        {
            if (dit->second.tiles[0].depth != layer_depth)
                continue;
            for(std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
            {
                enigma::tile &t = dit->second.tiles[i];
                t.roomX += x;
                t.roomY += y;
            }
            return true;
        }
    return false;
}

}

