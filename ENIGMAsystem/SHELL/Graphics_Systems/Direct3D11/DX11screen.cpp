/** Copyright (C) 2008-2014 Josh Ventura, Robert B. Colton
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

#include "Bridges/General/DX11Context.h"
#include "Direct3D11Headers.h"
#include "Graphics_Systems/General/GSbackground.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GSmodel.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Universal_System/var4.h"
#include "Universal_System/estring.h"
#include "Universal_System/background.h"
#include "Universal_System/background_internal.h"

#include "Universal_System/roomsystem.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"
#include "Universal_System/depth_draw.h"
#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/General/PFmain.h"

#include <limits>
#include <string>
#include <cstdio>

using namespace std;

//Fuck whoever did this to the spec
#ifndef GL_BGR
  #define GL_BGR 0x80E0
#endif

using namespace enigma;

namespace enigma_user {
  extern int window_get_width();
  extern int window_get_height();
  extern int window_get_region_width();
  extern int window_get_region_height();
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
    using enigma_user::background_hspeed;
    using enigma_user::background_vspeed;
    using enigma_user::background_index;
    using enigma_user::background_coloring;
    using enigma_user::draw_background_tiled_ext;
    using enigma_user::draw_background_ext;
    // Draw the rooms backgrounds
    for (int back_current = 0; back_current < 8; back_current++) {
        if (background_visible[back_current] == 1) {
          if (enigma_user::background_exists(background_index[back_current])) {
				//NOTE: This has been double checked with Game Maker 8.1 to work exactly the same, the background_x/y is modified just as object locals are
				//and also just as one would assume the system to work.
				//TODO: This should probably be moved to room system.
				background_x[back_current] += background_hspeed[back_current];
				background_y[back_current] += background_vspeed[back_current];
	            if (background_htiled[back_current] || background_vtiled[back_current]) {
	                draw_background_tiled_ext(background_index[back_current], background_x[back_current], background_y[back_current], background_xscale[back_current],
						background_xscale[back_current], background_coloring[back_current], background_alpha[back_current], background_htiled[back_current], background_vtiled[back_current]);
	            } else {
	                draw_background_ext(background_index[back_current], background_x[back_current], background_y[back_current], background_xscale[back_current], background_xscale[back_current], 0, background_coloring[back_current], background_alpha[back_current]);
				}
        }
		}
    }
}

namespace enigma
{
  extern bool d3dMode;
  extern bool d3dHidden;
  extern int d3dCulling;
  particles_implementation* particles_impl;
  void set_particles_implementation(particles_implementation* part_impl)
  {
      particles_impl = part_impl;
  }

	unsigned gui_width;
	unsigned gui_height;
}

namespace enigma_user
{

void screen_redraw()
{
	//TODO: Should implement extended lost device checking
	//if (d3dmgr == NULL ) return;

	if (!view_enabled)
    {
		screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());
		d3d_set_projection_ortho(0, 0, window_get_region_width(), window_get_region_height(), 0);

		if (background_showcolor)
		{
			draw_clear(background_color);
		}

		// Clear the depth buffer if 3d mode is on at the beginning of the draw step.
        if (enigma::d3dMode)
			m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

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
           // if (dit->second.tiles.size())
                //glCallList(drawing_depths[dit->second.tiles[0].depth].tilelist);

            texture_reset();
            enigma::inst_iter* push_it = enigma::instance_event_iterator;
            //loop instances
            for (enigma::instance_event_iterator = dit->second.draw_events->next; enigma::instance_event_iterator != NULL; enigma::instance_event_iterator = enigma::instance_event_iterator->next) {
                enigma::object_graphics* inst = ((object_graphics*)enigma::instance_event_iterator->inst);
                inst->myevent_draw();
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
		//return;
    }
    else
    {
        bool stop_loop = false;
		//TODO: Possibly implement view option from Stupido to control which view clears the background
		// Only clear the background on the first visible view by checking if it hasn't been cleared yet
		bool view_first = true;
        for (view_current = 0; view_current < 8; view_current++)
        {
            if (!view_visible[(int)view_current]) { continue; }
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
			screen_set_viewport(view_xport[vc], view_yport[vc],
				(window_get_region_width_scaled() - view_xport[vc]), (window_get_region_height_scaled() - view_yport[vc]));
			d3d_set_projection_ortho(view_xview[vc], view_wview[vc] + view_xview[vc], view_yview[vc], view_hview[vc] + view_yview[vc], 0);

			if (background_showcolor && view_first)
			{
				draw_clear(background_color);
			}

			// Clear the depth buffer if 3d mode is on at the beginning of the draw step.
			if (enigma::d3dMode)
				m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

			if (view_first) {
				draw_back();
			}

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
          enigma::object_graphics* inst = ((object_graphics*)enigma::instance_event_iterator->inst);
					inst->myevent_draw();
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
			view_first = false;
			if (stop_loop) break;
        }
        // In Studio this variable is not reset until the next iteration of views as is actually 7 in the draw_gui event, in 8.1 however view_current will always be 0 in the step event.
        view_current = 0;
    }

	int culling = d3d_get_culling();
	d3d_set_culling(rs_none);

	// Now process the sub event of draw called draw gui
	// It is for drawing GUI elements without view scaling and transformation
    if (enigma::gui_used)
    {
		// Now process the sub event of draw called draw gui
		// It is for drawing GUI elements without view scaling and transformation
		screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());
		d3d_set_projection_ortho(0, 0, enigma::gui_width, enigma::gui_height, 0);

		// Clear the depth buffer if 3d mode is on at the beginning of the draw step.
        if (enigma::d3dMode)
			m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		//d3dmgr->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

        bool stop_loop = false;
        for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++)
        {
            enigma::inst_iter* push_it = enigma::instance_event_iterator;
            //loop instances
            for (enigma::instance_event_iterator = dit->second.draw_events->next; enigma::instance_event_iterator != NULL; enigma::instance_event_iterator = enigma::instance_event_iterator->next) {
                enigma::object_graphics* inst = ((object_graphics*)enigma::instance_event_iterator->inst);
                inst->myevent_drawgui();
                if (enigma::room_switching_id != -1) {
                    stop_loop = true;
                    break;
                }
            }
            enigma::instance_event_iterator = push_it;
            if (stop_loop) break;
        }

	}

    d3dmgr->EndShapesBatching();    // ends the 3D scene

	screen_refresh();
}

void screen_init()
{
	enigma::gui_width = window_get_region_width();
	enigma::gui_height = window_get_region_height();

	//d3dmgr->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	//d3dmgr->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    if (!view_enabled)
    {
		//d3dmgr->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

		screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());
		d3d_set_projection_ortho(0, 0, window_get_region_width(), window_get_region_height(), 0);
    } else {
		for (view_current = 0; view_current < 7; view_current++)
        {
            if (view_visible[(int)view_current])
            {
                int vc = (int)view_current;

				screen_set_viewport(view_xport[vc], view_yport[vc],
					(window_get_region_width_scaled() - view_xport[vc]), (window_get_region_height_scaled() - view_yport[vc]));
				d3d_set_projection_ortho(view_xview[vc], view_wview[vc] + view_xview[vc], view_yview[vc], view_hview[vc] + view_yview[vc], 0);
                break;
            }
        }
	}
}

int screen_save(string filename) //Assumes native integers are little endian
{

}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h) //Assumes native integers are little endian
{

}

void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height) {
    x = (x / window_get_region_width()) * window_get_region_width_scaled();
    y = (y / window_get_region_height()) * window_get_region_height_scaled();
    width = (width / window_get_region_width()) * window_get_region_width_scaled();
    height = (height / window_get_region_height()) * window_get_region_height_scaled();
    gs_scalar sx, sy;
    sx = (window_get_width() - window_get_region_width_scaled()) / 2;
    sy = (window_get_height() - window_get_region_height_scaled()) / 2;

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = sx + x;
    viewport.TopLeftY = sy + y;
    viewport.Width = width;
    viewport.Height = height;

    m_deviceContext->RSSetViewports(1, &viewport);
}

//TODO: These need to be in some kind of General
void display_set_gui_size(unsigned int width, unsigned int height) {
	enigma::gui_width = width;
	enigma::gui_height = height;
}

unsigned int display_get_gui_width(){
  return enigma::gui_width;
}

unsigned int display_get_gui_height(){
  return enigma::gui_height;
}

}
