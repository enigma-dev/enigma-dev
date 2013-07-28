/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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
#include "../General/DirectXHeaders.h"
#include "../General/GSbackground.h"
#include "../General/GSscreen.h"
#include "../General/GSd3d.h"
#include "../General/DXbinding.h"

using namespace std;

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

//Fuck whoever did this to the spec
#ifndef GL_BGR
  #define GL_BGR 0x80E0
#endif

using namespace enigma;

namespace enigma_user {
  extern int window_get_width();
  extern int window_get_height();
}

static inline void draw_back()
{
    using enigma_user::background_x;
    using enigma_user::background_y;
    using enigma_user::background_visible;
    using enigma_user::background_alpha;
    using enigma_user::background_xscale;
    using enigma_user::background_yscale;
    using enigma_user::background_htiled;
    using enigma_user::background_vtiled;
    using enigma_user::background_index;
    using enigma_user::background_coloring;
    using enigma_user::draw_background_tiled_ext;
    using enigma_user::draw_background_ext;
    //Draw backgrounds
    for (int back_current=0; back_current<7; back_current++)
    {
        if (background_visible[back_current] == 1)
        {
            if (background_htiled[back_current] || background_vtiled[back_current])
                draw_background_tiled_ext(background_index[back_current], background_x[back_current], background_y[back_current], background_xscale[back_current], background_xscale[back_current], background_coloring[back_current], background_alpha[back_current]);
            else
                draw_background_ext(background_index[back_current], background_x[back_current], background_y[back_current], background_xscale[back_current], background_xscale[back_current], 0, background_coloring[back_current], background_alpha[back_current]);
        }
    }
}

namespace enigma
{
    extern bool d3dHidden;
    extern std::map<int,roomstruct*> roomdata;
    particles_implementation* particles_impl;
    void set_particles_implementation(particles_implementation* part_impl)
    {
        particles_impl = part_impl;
    }
}

#include "Bridges/General/DXDevice.h"

namespace enigma_user
{

void screen_redraw()
{
    d3ddev->BeginScene();    // begins the 3D scene

if (!view_enabled)
    {

        int clear_bits = 0;
		if (background_showcolor)
		{
			int clearcolor = ((int)background_color) & 0x00FFFFFF;
			// clear the window to a deep blue
			d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(__GETR(clearcolor), __GETG(clearcolor), __GETB(clearcolor)), 1.0f, 0);
		}

        draw_back();

        // Apply and clear stored depth changes.
        for (map<int,pair<double,double> >::iterator it = id_to_currentnextdepth.begin(); it != id_to_currentnextdepth.end(); it++)
        {
            enigma::object_graphics* inst_depth = (enigma::object_graphics*)enigma::fetch_instance_by_id((*it).first);
            if (inst_depth != NULL) {
                drawing_depths[(*it).second.first].draw_events->unlink(inst_depth->depth.myiter);
                inst_iter* mynewiter = drawing_depths[(*it).second.second].draw_events->add_inst(inst_depth->depth.myiter->inst);
                if (instance_event_iterator == inst_depth->depth.myiter) {
                    instance_event_iterator = inst_depth->depth.myiter->prev;
                }
                inst_depth->depth.myiter = mynewiter;
            }
        }
        id_to_currentnextdepth.clear();

        if (enigma::particles_impl != NULL) {
            const double high = numeric_limits<double>::max();
            const double low = drawing_depths.rbegin() != drawing_depths.rend() ? drawing_depths.rbegin()->first : -numeric_limits<double>::max();
            (enigma::particles_impl->draw_particlesystems)(high, low);
        }
        bool stop_loop = false;
        for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++)
        {
            if (dit->second.tiles.size())
                //glCallList(drawing_depths[dit->second.tiles[0].depth].tilelist);

            texture_reset();
            enigma::inst_iter* push_it = enigma::instance_event_iterator;
            //loop instances
            for (enigma::instance_event_iterator = dit->second.draw_events->next; enigma::instance_event_iterator != NULL; enigma::instance_event_iterator = enigma::instance_event_iterator->next) {
                enigma::instance_event_iterator->inst->myevent_draw();
                if (enigma::room_switching_id != -1) {
                    stop_loop = true;
                    break;
                }
            }
            enigma::instance_event_iterator = push_it;
            if (stop_loop) break;
            //particles
            if (enigma::particles_impl != NULL) {
                const double high = dit->first;
                dit++;
                const double low = dit != drawing_depths.rend() ? dit->first : -numeric_limits<double>::max();
                dit--;
                (enigma::particles_impl->draw_particlesystems)(high, low);
            }
        }
    }
    else
    {
        bool stop_loop = false;
        for (view_current = 0; view_current < 7; view_current++)
        {
            if (view_visible[(int)view_current])
            {
                int vc = (int)view_current;
                int vob = (int)view_object[vc];

                if (vob != -1)
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

                int clear_bits = 0;
				if (background_showcolor)
				{
					int clearcolor = ((int)background_color) & 0x00FFFFFF;
					// clear the window to a deep blue
					d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(__GETR(clearcolor), __GETG(clearcolor), __GETB(clearcolor)), 1.0f, 0);
				}

                draw_back();

                // Apply and clear stored depth changes.
                for (map<int,pair<double,double> >::iterator it = id_to_currentnextdepth.begin(); it != id_to_currentnextdepth.end(); it++)
                {
                    enigma::object_graphics* inst_depth = (enigma::object_graphics*)enigma::fetch_instance_by_id((*it).first);
                    if (inst_depth != NULL) {
                        drawing_depths[(*it).second.first].draw_events->unlink(inst_depth->depth.myiter);
                        inst_iter* mynewiter = drawing_depths[(*it).second.second].draw_events->add_inst(inst_depth->depth.myiter->inst);
                        if (instance_event_iterator == inst_depth->depth.myiter) {
                            instance_event_iterator = inst_depth->depth.myiter->prev;
                        }
                        inst_depth->depth.myiter = mynewiter;
                    }
                }
                id_to_currentnextdepth.clear();

                if (enigma::particles_impl != NULL) {
                    const double high = numeric_limits<double>::max();
                    const double low = drawing_depths.rbegin() != drawing_depths.rend() ? drawing_depths.rbegin()->first : -numeric_limits<double>::max();
                    (enigma::particles_impl->draw_particlesystems)(high, low);
                }
                for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++)
                {
                    if (dit->second.tiles.size())
                        //glCallList(drawing_depths[dit->second.tiles[0].depth].tilelist);

                    texture_reset();
                    enigma::inst_iter* push_it = enigma::instance_event_iterator;
                    //loop instances
                    for (enigma::instance_event_iterator = dit->second.draw_events->next; enigma::instance_event_iterator != NULL; enigma::instance_event_iterator = enigma::instance_event_iterator->next) {
                        enigma::instance_event_iterator->inst->myevent_draw();
                        if (enigma::room_switching_id != -1) {
                            stop_loop = true;
                            break;
                        }
                    }
                    enigma::instance_event_iterator = push_it;
                    if (stop_loop) break;
                    //particles
                    if (enigma::particles_impl != NULL) {
                        const double high = dit->first;
                        dit++;
                        const double low = dit != drawing_depths.rend() ? dit->first : -numeric_limits<double>::max();
                        dit--;
                        (enigma::particles_impl->draw_particlesystems)(high, low);
                    }
                }
                if (stop_loop) break;
            }
        }
        view_current = 0;
    }

    d3ddev->EndScene();    // ends the 3D scene

    d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
}

void screen_init()
{
    
}

int screen_save(string filename) //Assumes native integers are little endian
{
	
}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h) //Assumes native integers are little endian
{
	
}

}

