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

#include "GSprimitives.h"
#include "GSbackground.h"
#include "GStextures.h"
#include "GSvertex.h"
#include "GStiles.h"
#include "GStilestruct.h"
#include "GSvertex_impl.h"

#include "Universal_System/depth_draw.h"
#include "Universal_System/background.h"
#include "Universal_System/background_internal.h"

#define INCLUDED_FROM_SHELLMAIN Not really.
// make an exception just for point_in_rectangle
#include "Universal_System/mathnc.h"
#undef INCLUDED_FROM_SHELLMAIN

#include <algorithm>

namespace {

bool tiles_are_dirty = true;

struct bkinxop
{
  bool operator() (const enigma::tile& a, const enigma::tile& b) {return (a.bckid < b.bckid);}
} bkinxcomp;

} // anonymous namespace

namespace enigma
{
    int tile_vertex_buffer = -1, tile_index_buffer = -1;
    //Tile vector holds several values, like number of vertices to render, texture to use and so on
    //The structure is like this [render batch][batch info]
    //batch info - 0 = texture to use, 1 = vertices to render,
    std::map<int,std::vector<std::vector<int> > > tile_layer_metadata;

    static void draw_tile(int &ind, int index, int vertex, int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
    {
      if (!enigma_user::background_exists(back)) return;
      get_background(bck2d,back);

      const gs_scalar tbx = bck2d->texturex, tby = bck2d->texturey,
                      tbw = bck2d->width/(gs_scalar)bck2d->texturew, tbh = bck2d->height/(gs_scalar)bck2d->textureh,
                      xvert1 = x, xvert2 = xvert1 + width*xscale,
                      yvert1 = y, yvert2 = yvert1 + height*yscale,
                      tbx1 = tbx+left/tbw, tbx2 = tbx1 + width/tbw,
                      tby1 = tby+top/tbh, tby2 = tby1 + height/tbh;

      enigma_user::vertex_position(vertex, xvert1, yvert1);
      enigma_user::vertex_texcoord(vertex, tbx1, tby1);
      enigma_user::vertex_color(vertex, color, alpha);

      enigma_user::vertex_position(vertex, xvert2, yvert1);
      enigma_user::vertex_texcoord(vertex, tbx2, tby1);
      enigma_user::vertex_color(vertex, color, alpha);

      enigma_user::vertex_position(vertex, xvert1, yvert2);
      enigma_user::vertex_texcoord(vertex, tbx1, tby2);
      enigma_user::vertex_color(vertex, color, alpha);

      enigma_user::vertex_position(vertex, xvert2, yvert2);
      enigma_user::vertex_texcoord(vertex, tbx2, tby2);
      enigma_user::vertex_color(vertex, color, alpha);

      IndexBuffer* indexBuffer = indexBuffers[index];
      int indices[] = {ind + 0, ind + 1, ind + 2, ind + 2, ind + 1, ind + 3};
      indexBuffer->indices.insert(indexBuffer->indices.end(), indices, indices + 6);
      ind += 4;
    }

    void load_tiles()
    {
        if (!tiles_are_dirty) return;
        tiles_are_dirty = false;
        tile_layer_metadata.clear();

        static int vertexFormat = -1;
        if (!enigma_user::vertex_format_exists(vertexFormat)) {
            enigma_user::vertex_format_begin();
            enigma_user::vertex_format_add_position();
            enigma_user::vertex_format_add_textcoord();
            enigma_user::vertex_format_add_color();
            vertexFormat = enigma_user::vertex_format_end();
        }
        // Create a vertex buffer or clear the existing one
        if (!enigma_user::vertex_exists(tile_vertex_buffer))
            tile_vertex_buffer = enigma_user::vertex_create_buffer();
        else
            enigma_user::vertex_clear(tile_vertex_buffer);
        // Create an index buffer or clear the existing one
        if (!enigma_user::index_exists(tile_vertex_buffer))
            tile_index_buffer = enigma_user::index_create_buffer();
        else
            enigma_user::index_clear(tile_index_buffer);

        enigma_user::vertex_begin(tile_vertex_buffer, vertexFormat);
        enigma_user::index_begin(tile_index_buffer, enigma_user::index_type_ushort);

        int prev_bkid;
        int index_start = 0;
        int index_count = 0;
        int vertex_ind = 0;
        for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++){
            if (dit->second.tiles.size())
            {
                //TODO: Should they really be sorted by background? This may help batching, but breaks compatiblity. Nothing texture atlas wouldn't solve.
                sort(dit->second.tiles.begin(), dit->second.tiles.end(), bkinxcomp);
                for(std::vector<tile>::size_type i = 0; i != dit->second.tiles.size(); ++i)
                {
                    tile t = dit->second.tiles[i];
                    if (i==0){ prev_bkid = t.bckid; }
                    draw_tile(vertex_ind, tile_index_buffer, tile_vertex_buffer, t.bckid, t.bgx, t.bgy, t.width, t.height, t.roomX, t.roomY, t.xscale, t.yscale, t.color, t.alpha);
                    index_count += 6;
                    if (prev_bkid != t.bckid || i == dit->second.tiles.size()-1){ //Texture switch has happened. Create new batch
                        get_background(bck2d,prev_bkid);
                        tile_layer_metadata[dit->second.tiles[0].depth].push_back( std::vector< int >(3) );
                        tile_layer_metadata[dit->second.tiles[0].depth].back()[0] = bck2d->texture;
                        tile_layer_metadata[dit->second.tiles[0].depth].back()[1] = index_start;
                        tile_layer_metadata[dit->second.tiles[0].depth].back()[2] = index_count;

                        index_start += index_count;
                        index_count = 0;

                        prev_bkid = t.bckid;
                    }
                }
            }
        }

        enigma_user::vertex_end(tile_vertex_buffer);
        enigma_user::index_end(tile_index_buffer);
        enigma_user::vertex_freeze(tile_vertex_buffer);
        enigma_user::index_freeze(tile_index_buffer);
    }

    void delete_tiles()
    {
        tiles_are_dirty = true;
    }

    void rebuild_tile_layer(int layer_depth)
    {
        // Yes, this does the same as delete_tiles for now, but is being left
        // because tiles could be futher optimized by only rebuilding a single
        // layer instead of all the layers when changed. This requires the
        // vertex_* functions be extended with the ability to rewrite only part
        // of their buffers.
        tiles_are_dirty = true;
    }
}

namespace enigma_user
{

int tile_add(int background, int left, int top, int width, int height, int x, int y, int depth, double xscale, double yscale, double alpha, int color)
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
    ntile->xscale = xscale;
    ntile->yscale = yscale;
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
            enigma::rebuild_tile_layer(layer_depth);
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
                if (point_in_rectangle(x, y, t.roomX, t.roomY, t.roomX + t.width - 1, t.roomY + t.height - 1))
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
