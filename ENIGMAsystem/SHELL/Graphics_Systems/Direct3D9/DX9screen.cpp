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
#include "Direct3D9Headers.h"
#include "../General/GSbackground.h"
#include "../General/GSscreen.h"
#include "../General/GSd3d.h"
#include "DX9binding.h"

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
#include "Platforms/General/PFwindow.h"
#include "Universal_System/CallbackArrays.h"
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
	extern bool d3dMode;
    extern bool d3dHidden;
	extern int d3dCulling;
    extern std::map<int,roomstruct*> roomdata;
    particles_implementation* particles_impl;
    void set_particles_implementation(particles_implementation* part_impl)
    {
        particles_impl = part_impl;
    }
	
	unsigned gui_width;
	unsigned gui_height;
}

#include "Bridges/General/DX9Device.h"

namespace enigma_user
{

void screen_redraw()
{
	// Should implement extended lost device checking
	//if (d3ddev == NULL ) return;
	
	// Clean up any textures that ENIGMA may still think are binded but actually are not
	texture_reset();

    d3ddev->BeginScene();    // begins the 3D scene
	dsprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);
		
	if (!view_enabled)
    {
		D3DVIEWPORT9 pViewport = { 0, 0, (DWORD)window_get_region_width_scaled(), (DWORD)window_get_region_height_scaled(), 0, 1.0f };
		d3ddev->SetViewport(&pViewport);
		
		D3DXMATRIX matTrans, matScale;

		// Calculate a translation matrix
		D3DXMatrixTranslation(&matTrans, -0.5, -room_height - 0.5, 0);
		D3DXMatrixScaling(&matScale, 1, -1, 1);
		
		// Calculate our world matrix by multiplying the above (in the correct order)
		D3DXMATRIX matWorld = matTrans * matScale;

		// Set the matrix to be applied to anything we render from now on
		d3ddev->SetTransform( D3DTS_VIEW, &matWorld);
	
		D3DXMATRIX matProjection;    // the projection transform matrix
		D3DXMatrixOrthoOffCenterLH(&matProjection,
							0,
							(FLOAT)room_width,   
							0, 
							(FLOAT)room_height,   
							0.0f,    // the near view-plane
							1.0f);    // the far view-plane
		d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection transform
		
		if (background_showcolor)
		{
			int clearcolor = ((int)background_color) & 0x00FFFFFF;
			// clear the window to the background color
			d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(__GETR(clearcolor), __GETG(clearcolor), __GETB(clearcolor)), 1.0f, 0);
			// clear the depth buffer
		}

		// Clear the depth buffer if 3d mode is on at the beginning of the draw step.
        if (enigma::d3dMode)
			d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		
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
		//return;
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
				
				D3DVIEWPORT9 pViewport = { (DWORD)view_xport[vc], (DWORD)view_yport[vc], 
					(DWORD)(window_get_region_width_scaled() - view_xport[vc]), (DWORD)(window_get_region_height_scaled() - view_yport[vc]), 0, 1.0f };
				d3ddev->SetViewport(&pViewport);
		
				D3DXMATRIX matTrans, matScale;

				// Calculate a translation matrix
				D3DXMatrixTranslation(&matTrans, -view_xview[vc] - 0.5, -view_yview[vc] - room_height - 0.5, 0);
				D3DXMatrixScaling(&matScale, 1, -1, 1);
		
				// Calculate our world matrix by multiplying the above (in the correct order)
				D3DXMATRIX matWorld = matTrans * matScale;

				// Set the matrix to be applied to anything we render from now on
				d3ddev->SetTransform( D3DTS_VIEW, &matWorld);
	
				D3DXMATRIX matProjection;    // the projection transform matrix
				D3DXMatrixOrthoOffCenterLH(&matProjection,
							0,
							(FLOAT)view_wview[vc],   
							0, 
							(FLOAT)view_hview[vc],   
							0.0f,    // the near view-plane
							1.0f);    // the far view-plane
				d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection transform
				
				if (background_showcolor && view_current == 0)
				{
					int clearcolor = ((int)background_color) & 0x00FFFFFF;
					// clear the window to the background color
					d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(__GETR(clearcolor), __GETG(clearcolor), __GETB(clearcolor)), 1.0f, 0);
				}
				
				// Clear the depth buffer if 3d mode is on at the beginning of the draw step.
                if (enigma::d3dMode)
                    d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

                if (view_current == 0) {
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
	
	int culling = d3d_get_culling();
	d3d_set_culling(rs_none);

	// Now process the sub event of draw called draw gui
	// It is for drawing GUI elements without view scaling and transformation
    if (enigma::gui_used)
    {
		// Now process the sub event of draw called draw gui 
		// It is for drawing GUI elements without view scaling and transformation
		D3DVIEWPORT9 pViewport = { 0, 0, window_get_region_width_scaled(), window_get_region_height_scaled(), 0, 1.0f };
		d3ddev->SetViewport(&pViewport);
		
		D3DXMATRIX matTrans, matScale;

		// Calculate a translation matrix
		D3DXMatrixTranslation(&matTrans, -0.5, -room_height - 0.5, 0);
		D3DXMatrixScaling(&matScale, 1, -1, 1);
		
		// Calculate our world matrix by multiplying the above (in the correct order)
		D3DXMATRIX matWorld = matTrans * matScale;

		// Set the matrix to be applied to anything we render from now on
		d3ddev->SetTransform( D3DTS_VIEW, &matWorld);
	
		D3DXMATRIX matProjection;    // the projection transform matrix
		D3DXMatrixOrthoOffCenterLH(&matProjection,
							0,
							(FLOAT)enigma::gui_width,   
							0, 
							(FLOAT)enigma::gui_height,   
							0.0f,    // the near view-plane
							1.0f);    // the far view-plane
		d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection transform

		//dsprite->SetWorldViewRH(NULL, &matWorld);

		// Clear the depth buffer if hidden surface removal is on at the beginning of the draw step.
        if (enigma::d3dMode)
			d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			
		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			
        bool stop_loop = false;
        for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++)
        {
            enigma::inst_iter* push_it = enigma::instance_event_iterator;
            //loop instances
            for (enigma::instance_event_iterator = dit->second.draw_events->next; enigma::instance_event_iterator != NULL; enigma::instance_event_iterator = enigma::instance_event_iterator->next) {
				enigma::instance_event_iterator->inst->myevent_drawgui();
                if (enigma::room_switching_id != -1) {
                    stop_loop = true;
                    break;
                }
            }
            enigma::instance_event_iterator = push_it;
            if (stop_loop) break;
        }
		
	}
	
	// Textures should be clamped when rendering 2D sprites and stuff, so memorize it.
	DWORD wrapu, wrapv, wrapw;
	d3ddev->GetSamplerState( 0, D3DSAMP_ADDRESSU, &wrapu );
	d3ddev->GetSamplerState( 0, D3DSAMP_ADDRESSV, &wrapv );
	d3ddev->GetSamplerState( 0, D3DSAMP_ADDRESSW, &wrapw );
	
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP );
	// The D3D sprite batcher uses clockwise face culling which is default but can't tell if 
	// this here should memorize it and force it to CW all the time and then reset what the user had
	// or not.
	//d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	dsprite->End();
	// And now reset the texture repetition.
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, wrapu );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, wrapv );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, wrapw );
	
	// reset the culling
	d3d_set_culling(culling);
    d3ddev->EndScene();    // ends the 3D scene
		
	screen_refresh();
}

void screen_init()
{
	enigma::gui_width = window_get_region_width_scaled();
	enigma::gui_height = window_get_region_height_scaled();
    if (!view_enabled)
    {
        //glMatrixMode(GL_PROJECTION);
          //gluPerspective(0, 1, 0, 1);
        //glMatrixMode(GL_MODELVIEW);

		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		
		D3DVIEWPORT9 pViewport = { 0, 0, (DWORD)window_get_region_width_scaled(), (DWORD)window_get_region_height_scaled(), 0, 1.0f };
		d3ddev->SetViewport(&pViewport);
		
		D3DXMATRIX matTrans, matScale;
						
		// Calculate a translation matrix
		D3DXMatrixTranslation(&matTrans, -0.5, -room_height - 0.5, 0);
		D3DXMatrixScaling(&matScale, 1, -1, 1);
		
		// Calculate our world matrix by multiplying the above (in the correct order)
		D3DXMATRIX matWorld = matTrans * matScale;

		// Set the matrix to be applied to anything we render from now on
		d3ddev->SetTransform( D3DTS_VIEW, &matWorld);
	
		D3DXMATRIX matProjection;    // the projection transform matrix
		D3DXMatrixOrthoOffCenterLH(&matProjection,
							0,
							(FLOAT)room_width,   
							0, 
							(FLOAT)room_height,   
							0.0f,    // the near view-plane
							1.0f);    // the far view-plane
		d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection transform
				
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
				
		d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);
		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        //glEnable(GL_BLEND);
        //glEnable(GL_TEXTURE_2D);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        //glAlphaFunc(GL_ALWAYS,0);
        //glColor4f(0,0,0,1);
    }
    else
    {
		for (view_current = 0; view_current < 7; view_current++)
        {
            if (view_visible[(int)view_current])
            {
                int vc = (int)view_current;
                  //gluPerspective(0, 1, 0, 1);
				  
				D3DVIEWPORT9 pViewport = { (DWORD)view_xport[vc], (DWORD)view_yport[vc], 
					(DWORD)(window_get_region_width_scaled() - view_xport[vc]), (DWORD)(window_get_region_height_scaled() - view_yport[vc]), 0, 1.0f };
				d3ddev->SetViewport(&pViewport);
		
				D3DXMATRIX matTrans, matScale;

				// Calculate a translation matrix
				D3DXMatrixTranslation(&matTrans, -view_xview[vc] - 0.5, -view_yview[vc] - room_height - 0.5, 0);
				D3DXMatrixScaling(&matScale, 1, -1, 1);
		
				// Calculate our world matrix by multiplying the above (in the correct order)
				D3DXMATRIX matWorld = matTrans * matScale;

				// Set the matrix to be applied to anything we render from now on
				d3ddev->SetTransform( D3DTS_VIEW, &matWorld);
	
				D3DXMATRIX matProjection;    // the projection transform matrix
				D3DXMatrixOrthoOffCenterLH(&matProjection,
							0,
							(FLOAT)view_wview[vc],   
							0, 
							(FLOAT)view_hview[vc],   
							0.0f,    // the near view-plane
							1.0f);    // the far view-plane
				d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection transform
				  
				d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
				d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
				
				d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);
				d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                //glEnable(GL_BLEND);
                //glEnable(GL_TEXTURE_2D);
                //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
                //glAlphaFunc(GL_ALWAYS,0);
                //glColor4f(0,0,0,1);
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

void display_set_gui_size(unsigned width, unsigned height) {
	enigma::gui_width = width;
	enigma::gui_height = height;
}

}

