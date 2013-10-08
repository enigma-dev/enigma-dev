/** Copyright (C) 2008-2013 Josh Ventura, Harijs Grinbergs, Robert B. Colton
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
#include "../General/OpenGLHeaders.h"
#include "../General/GSbackground.h"
#include "../General/GSscreen.h"
#include "../General/GSd3d.h"
#include "GL3binding.h"

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
namespace enigma
{
    vector<gs_scalar> globalVBO_data;
    vector<unsigned int> globalVBO_indices;
    GLuint globalVBO; //The buffer itself
    unsigned int globalVBO_texture;
    unsigned int globalVBO_datCount; //Length of the VBO data buffer
    unsigned int globalVBO_verCount; //Number of vertices on this buffer
    unsigned int globalVBO_indCount; //Number of indices
    unsigned int globalVBO_maxBSize; //Maximum buffer size. Buffer will not be regenerated if the current buffer size is smaller than the maximum
    unsigned int bound_framebuffer = 0; //Shows the bound framebuffer, so glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo); don't need to be called (they are very slow)
}

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
	extern bool d3dZWriteEnable;
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

void draw_globalVBO()
{
    if (globalVBO_verCount>0){
        //int fbo;
        //glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
        //printf("RENDERING THIS - Verts = %i, inds = %i and fbo = %i, data size = %i, index size = %i\n",globalVBO_verCount,globalVBO_indCount,fbo,globalVBO_data.size(),globalVBO_indices.size() );
        //glBindTexture(GL_TEXTURE_2D,0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glBindBufferARB(GL_ARRAY_BUFFER, globalVBO);
		
		// Textures should be clamped when rendering 2D sprites and stuff, so memorize it.
		GLint wrapr, wraps, wrapt;
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, &wrapr);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &wraps);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &wrapt);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        if (globalVBO_verCount>globalVBO_maxBSize) glBufferDataARB(GL_ARRAY_BUFFER, globalVBO_datCount * sizeof(gs_scalar), &globalVBO_data[0], GL_DYNAMIC_DRAW), globalVBO_maxBSize = globalVBO_verCount;
        else glBufferSubDataARB(GL_ARRAY_BUFFER, 0, globalVBO_datCount * sizeof(gs_scalar), &globalVBO_data[0]);
        glVertexPointer( 2, GL_FLOAT, sizeof(gs_scalar) * 8, NULL );
        glTexCoordPointer( 2, GL_FLOAT, sizeof(gs_scalar) * 8, (void*)(sizeof(gs_scalar) * 2) );
        glColorPointer( 4, GL_FLOAT, sizeof(gs_scalar) * 8, (void*)(sizeof(gs_scalar) * 4) );
		

		// this sprite batching mechanism does not allow one to apply transformations to sprites or text
		// like is possible with the Direct3D 9 sprite batcher or traditionally in Game Maker.
		if (d3dZWriteEnable) {
		  glDepthMask(false);
		}
        glDrawElements(GL_TRIANGLES, globalVBO_indCount, GL_UNSIGNED_INT, &globalVBO_indices[0] );
		if (d3dZWriteEnable) {
		  glDepthMask(true);
		}
		
		// And now reset the textures repetition
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt);
		
        glDisableClientState( GL_COLOR_ARRAY );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        glDisableClientState( GL_VERTEX_ARRAY );
		
		glBindBufferARB(GL_ARRAY_BUFFER, 0);

        globalVBO_datCount = globalVBO_verCount = globalVBO_indCount = 0;
    }
}

namespace enigma_user
{

void screen_redraw()
{
	// Clean up any textures that ENIGMA may still think are binded but actually are not
	texture_reset();
	
    if (!view_enabled)
    {
        glViewport(0, 0, window_get_region_width_scaled(), window_get_region_height_scaled());
        glLoadIdentity();
        glScalef(1, (bound_framebuffer==0?-1:1), 1);
        glOrtho(0, room_width, 0, room_height, 0, 1);
        glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
        glMultMatrixd(transformation_matrix);

        int clear_bits = 0;
        if (background_showcolor)
        {
            int clearcolor = ((int)background_color) & 0x00FFFFFF;
            glClearColor(__GETR(clearcolor) / 255.0, __GETG(clearcolor) / 255.0, __GETB(clearcolor) / 255.0, 1);
            clear_bits |= GL_COLOR_BUFFER_BIT;
        }

        // Clear the depth buffer if 3d mode is on at the beginning of the draw step.
        if (enigma::d3dMode)
            clear_bits |= GL_DEPTH_BUFFER_BIT;

        if (clear_bits)
            glClear(clear_bits);

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
                glCallList(drawing_depths[dit->second.tiles[0].depth].tilelist);

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

                glViewport(view_xport[vc], view_yport[vc], window_get_region_width_scaled() - view_xport[vc], window_get_region_height_scaled() - view_yport[vc]);
                glLoadIdentity();
                glScalef(1, (bound_framebuffer==0?-1:1), 1);

                glOrtho(view_xview[vc], view_wview[vc] + view_xview[vc], view_yview[vc], view_hview[vc] + view_yview[vc], 0, 1);
                glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
                glMultMatrixd(transformation_matrix);

                int clear_bits = 0;
                if (background_showcolor && view_current == 0)
                {
                    int clearcolor = ((int)background_color) & 0x00FFFFFF;
                    glClearColor(__GETR(clearcolor) / 255.0, __GETG(clearcolor) / 255.0, __GETB(clearcolor) / 255.0, 1);
                    clear_bits |= GL_COLOR_BUFFER_BIT;
                }

				// Clear the depth buffer if 3d mode is on at the beginning of the draw step.
				// Always clear the depth buffer for every view, this was Game Maker 8.1 behaviour
				if (enigma::d3dMode)
					clear_bits |= GL_DEPTH_BUFFER_BIT;

				if (clear_bits)
					glClear(clear_bits);

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
                        glCallList(drawing_depths[dit->second.tiles[0].depth].tilelist);

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

	draw_globalVBO();
			
	// Now process the sub event of draw called draw gui
	// It is for drawing GUI elements without view scaling and transformation
    if (enigma::gui_used)
    {
	    glViewport(0, 0, window_get_region_width_scaled(), window_get_region_height_scaled());
        glLoadIdentity();
        glScalef(1, (bound_framebuffer==0?-1:1), 1);
        glOrtho(0, enigma::gui_width, 0, enigma::gui_height, 0, 1);
        glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
        glMultMatrixd(transformation_matrix);
		
		// Clear the depth buffer if hidden surface removal is on at the beginning of the draw step.
        if (enigma::d3dMode)
			glClear(GL_DEPTH_BUFFER_BIT);
		
		int culling = d3d_get_culling();
		d3d_set_culling(rs_none);
		
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
		draw_globalVBO();
		
		// reset the culling
		d3d_set_culling(culling);
    }
		
    screen_refresh();
}

void screen_init()
{
	enigma::gui_width = window_get_region_width_scaled();
	enigma::gui_height = window_get_region_height_scaled();
	
    glGenBuffersARB(1, &globalVBO);
    texture_reset();
    if (!view_enabled)
    {
        glMatrixMode(GL_PROJECTION);
          glClearColor(0,0,0,0);
          glLoadIdentity();
          gluPerspective(0, 1, 0, 1);
        glMatrixMode(GL_MODELVIEW);
          glViewport(0, 0, window_get_region_width_scaled(), window_get_region_height_scaled());
          glLoadIdentity();
          glScalef(1, -1, 1);
          glOrtho(0, room_width, 0, room_height, 0, 1);
          glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
          glMultMatrixd(transformation_matrix);
          glClearColor(0,0,0,0);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          glDisable(GL_DEPTH_TEST);
          glEnable(GL_BLEND);
          glEnable(GL_ALPHA_TEST);
          glEnable(GL_TEXTURE_2D);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          glAlphaFunc(GL_ALWAYS,0);
          glColor4f(0,0,0,1);
          texture_reset();
    }
    else
    {
        for (view_current = 0; view_current < 7; view_current++)
        {
            if (view_visible[(int)view_current])
            {
                int vc = (int)view_current;
                glMatrixMode(GL_PROJECTION);
                  glClearColor(0,0,0,0);
                  glLoadIdentity();
                  gluPerspective(0, 1, 0, 1);
                glMatrixMode(GL_MODELVIEW);
                  glViewport(view_xport[vc], view_yport[vc], view_wport[vc], view_hport[vc]);
                  glLoadIdentity();
                  glScalef(1, -1, 1);
                  glOrtho(view_xview[vc], view_wview[vc] + view_xview[vc], view_yview[vc], view_hview[vc] + view_yview[vc], 0, 1);
                  glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
                  glMultMatrixd(transformation_matrix);
                  glClearColor(0,0,0,0);
                  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                  glDisable(GL_DEPTH_TEST);
                  glEnable(GL_BLEND);
                  glEnable(GL_ALPHA_TEST);
                  glEnable(GL_TEXTURE_2D);
                  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                  glAlphaFunc(GL_ALWAYS,0);
                  glColor4f(0,0,0,1);
                  texture_reset();
                break;
            }
        }
    }
}

int screen_save(string filename) //Assumes native integers are little endian
{
	unsigned int w=window_get_width(),h=window_get_height(),sz=w*h;
	FILE *bmp = fopen(filename.c_str(),"wb");
	if(!bmp) return -1;

	char *scrbuf = new char[sz*3];
	glReadPixels(0,0,w,h,GL_BGR,GL_UNSIGNED_BYTE,scrbuf);

	fwrite("BM",2,1,bmp);
	sz <<= 2;

	fwrite(&sz,4,1,bmp);
	fwrite("\0\0\0\0\x36\0\0\0\x28\0\0",12,1,bmp);
	fwrite(&w,4,1,bmp);
	fwrite(&h,4,1,bmp);
	fwrite("\1\0\x18\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",28,1,bmp);

	if (w & 3)
	{
		size_t pad=w&3;
		w*=3;
		sz-=sz>>2;
		for(unsigned int i=0;i<sz;i+=w)
		{
			fwrite(scrbuf+i,w,1,bmp);
			fwrite("\0\0",pad,1,bmp);
		}
	} else fwrite(scrbuf,w*3,h,bmp);
	fclose(bmp);
	delete[] scrbuf;
	return 0;
}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h) //Assumes native integers are little endian
{
	unsigned sz = w * h;
	FILE *bmp=fopen(filename.c_str(), "wb");
	if (!bmp) return -1;
	fwrite("BM", 2, 1, bmp);

	char *scrbuf = new char[sz*3];
	glReadPixels(x, y, w, h, GL_BGR, GL_UNSIGNED_BYTE, scrbuf);

	sz <<= 2;
	fwrite(&sz,4,1,bmp);
	fwrite("\0\0\0\0\x36\0\0\0\x28\0\0",12,1,bmp);
	fwrite(&w,4,1,bmp);
	fwrite(&h,4,1,bmp);
	fwrite("\1\0\x18\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",28,1,bmp);

	if (w & 3)
	{
		size_t pad = w & 3;
		w *= 3;
		sz -= sz >> 2;
		for(unsigned i = 0; i < sz; i += w)
		{
			fwrite(scrbuf+i,w,1,bmp);
			fwrite("\0\0",pad,1,bmp);
		}
	}
	else fwrite(scrbuf, w*3, h, bmp);

	fclose(bmp);
	delete[] scrbuf;
	return 0;
}

void display_set_gui_size(unsigned width, unsigned height) {
	enigma::gui_width = width;
	enigma::gui_height = height;
}

}

