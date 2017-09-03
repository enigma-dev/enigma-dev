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
#include <string>
#include <cstdio>
#include "../General/GStextures.h"
#include "../General/GSbackground.h"
#include "../General/GSscreen.h"
#include "../General/GSd3d.h"
#include "../General/GSmatrix.h"
#include "../General/GScolors.h"

using namespace std;

#include "Universal_System/image_formats.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/var4.h"
#include "Universal_System/estring.h"

#define __GETR(x) (((unsigned int)x & 0x0000FF))
#define __GETG(x) (((unsigned int)x & 0x00FF00) >> 8)
#define __GETB(x) (((unsigned int)x & 0xFF0000) >> 16)

#include "Universal_System/roomsystem.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"
#include "Universal_System/depth_draw.h"
#include "Platforms/platforms_mandatory.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include <limits>

using namespace enigma;
using namespace enigma_user;
namespace enigma_user {
  extern int window_get_width();
  extern int window_get_height();
  extern int window_get_region_width();
  extern int window_get_region_height();
}

namespace enigma
{
	extern bool d3dMode;
	extern int d3dCulling;
	particles_implementation* particles_impl;
	void set_particles_implementation(particles_implementation* part_impl)
	{
			particles_impl = part_impl;
	}
}


static inline void draw_back(){}

static inline void follow_object(int vob, size_t vc)
{
  object_basic *instanceexists = fetch_instance_by_int(vob);

  if (instanceexists)
  {
    object_planar* vobr = (object_planar*)instanceexists;

    double vobx = vobr->x, voby = vobr->y;

    //int bbl=*vobr.x+*vobr.bbox_left,bbr=*vobr.x+*vobr.bbox_right,bbt=*vobr.y+*vobr.bbox_top,bbb=*vobr.y+*vobr.bbox_bottom;
    //if (bbl<view_xview[vc]+view_hbor[vc]) view_xview[vc]=bbl-view_hbor[vc];

    double vbc_h, vbc_v;
    (view_hborder[vc] > view_wview[vc]/2) ? vbc_h = view_wview[vc]/2 : vbc_h = view_hborder[vc];
    (view_vborder[vc] > view_hview[vc]/2) ? vbc_v = view_hview[vc]/2 : vbc_v = view_vborder[vc];

    if (view_hspeed[vc] == -1)
    {
      if (vobx < view_xview[vc] + vbc_h)
        view_xview[vc] = vobx - vbc_h;
      else if (vobx > view_xview[vc] + view_wview[vc] - vbc_h)
        view_xview[vc] = vobx + vbc_h - view_wview[vc];
    }
    else
    {
      if (vobx < view_xview[vc] + vbc_h)
      {
        view_xview[vc] -= view_hspeed[vc];
        if (view_xview[vc] < vobx - vbc_h)
          view_xview[vc] = vobx - vbc_h;
      }
      else if (vobx > view_xview[vc] + view_wview[vc] - vbc_h)
      {
        view_xview[vc] += view_hspeed[vc];
        if (view_xview[vc] > vobx + vbc_h - view_wview[vc])
          view_xview[vc] = vobx + vbc_h - view_wview[vc];
      }
    }

    if (view_vspeed[vc] == -1)
    {
      if (voby < view_yview[vc] + vbc_v)
        view_yview[vc] = voby - vbc_v;
      else if (voby > view_yview[vc] + view_hview[vc] - vbc_v)
        view_yview[vc] = voby + vbc_v - view_hview[vc];
    }
    else
    {
      if (voby < view_yview[vc] + vbc_v)
      {
        view_yview[vc] -= view_vspeed[vc];
        if (view_yview[vc] < voby - vbc_v)
          view_yview[vc] = voby - vbc_v;
      }
      if (voby > view_yview[vc] + view_hview[vc] - vbc_v)
      {
        view_yview[vc] += view_vspeed[vc];
        if (view_yview[vc] > voby + vbc_v - view_hview[vc])
          view_yview[vc] = voby + vbc_v - view_hview[vc];
      }
    }

    if (view_xview[vc] < 0)
      view_xview[vc] = 0;
    else if (view_xview[vc] > room_width - view_wview[vc])
      view_xview[vc] = room_width - view_wview[vc];

    if (view_yview[vc] < 0)
      view_yview[vc] = 0;
    else if (view_yview[vc] > room_height - view_hview[vc])
      view_yview[vc] = room_height - view_hview[vc];
  }
}

static inline void draw_insts(){}
static inline int draw_tiles(){return 0;}
void clear_view(float x, float y, float w, float h, float angle, bool showcolor){}
static inline void draw_gui(){}
