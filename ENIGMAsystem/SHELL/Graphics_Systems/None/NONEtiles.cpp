/** Copyright (C) 2017 Faissal I. Bensefia
*** Copyright (C) 2008-2013 Robert B. Colton, Adriano Tumminelli
*** Copyright (C) 2014 Seth N. Hetu
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
#include "Graphics_Systems/General/GSprimitives.h"
#include "Universal_System/depth_draw.h"
#include <algorithm>
#include "../General/GSbackground.h"
#include "Universal_System/backgroundstruct.h"
#include "../General/GStextures.h"
#include "../General/GStiles.h"
#include "../General/GLtilestruct.h"
#include "../General/OpenGLHeaders.h"

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
			ntile->alpha = alpha;
			ntile->color = color;
			ntile->xscale = xscale;
			ntile->yscale = yscale;
			enigma::drawing_depths[ntile->depth].tiles.push_back(*ntile);
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
						t.depth = depth;
						enigma::drawing_depths[t.depth].tiles.push_back(t);
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
				for (std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
				{
					enigma::tile t = dit->second.tiles[i];
					if (t.roomX == x && t.roomY == y)
						enigma::drawing_depths[t.depth].tiles.erase(enigma::drawing_depths[t.depth].tiles.begin() + i);
				}
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
				for (std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
				{
					enigma::tile t = dit->second.tiles[i];
					t.depth = depth;
					enigma::drawing_depths[t.depth].tiles.push_back(t);
				}
				enigma::drawing_depths[layer_depth].tiles.clear();
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
				for (std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
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
				for (std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
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
				for (std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
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
				for (std::vector<enigma::tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++)
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
