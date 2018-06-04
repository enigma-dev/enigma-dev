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
#include "Graphics_Systems/General/OpenGLHeaders.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSbackground.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Universal_System/image_formats.h"
#include "Universal_System/background.h"
#include "Universal_System/var4.h"
#include "Universal_System/estring.h"

#include "Universal_System/roomsystem.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"
#include "Universal_System/depth_draw.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/platforms_mandatory.h"
#include "Graphics_Systems/graphics_mandatory.h"

#include <string>
#include <cstdio>
#include <limits>

using namespace enigma;
using namespace enigma_user;

using namespace std;

//Fuck whoever did this to the spec
#ifndef GL_BGR
  #define GL_BGR 0x80E0
#endif

namespace enigma_user {
  extern int window_get_width();
  extern int window_get_height();
  extern int window_get_region_width();
  extern int window_get_region_height();
}

namespace enigma
{
  extern GLuint msaa_fbo;
  extern bool d3dMode;
  extern int d3dCulling;
  particles_implementation* particles_impl;
  void set_particles_implementation(particles_implementation* part_impl)
  {
      particles_impl = part_impl;
  }

	unsigned gui_width;
	unsigned gui_height;
	unsigned int bound_framebuffer = 0; //Shows the bound framebuffer, so glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo); don't need to be called (they are very slow)
  int viewport_x, viewport_y, viewport_w, viewport_h; //These are used by surfaces, to set back the viewport
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

static inline void draw_insts()
{
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
}

/**
  Handles tile drawing for a view; returns whether to break the view loop.
  @return Returns 0 if all is well, or non-zero if the view loop should be broken.
*/
static inline int draw_tiles()
{
  for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++)
  {
    if (dit->second.tiles.size())
    {
        glCallList(drawing_depths[dit->second.tiles[0].depth].tilelist);
        texture_reset();
    }
    enigma::inst_iter* push_it = enigma::instance_event_iterator;
    //loop instances
    for (enigma::instance_event_iterator = dit->second.draw_events->next; enigma::instance_event_iterator != NULL; enigma::instance_event_iterator = enigma::instance_event_iterator->next) {
      enigma::object_graphics* inst = ((object_graphics*)enigma::instance_event_iterator->inst);
      if (inst->myevent_draw_subcheck())
        inst->myevent_draw();
      if (enigma::room_switching_id != -1)
        return 1;
    }
    enigma::instance_event_iterator = push_it;
    //particles
    if (enigma::particles_impl != NULL) {
      const double high = dit->first;
      dit++;
      const double low = dit != drawing_depths.rend() ? dit->first : -numeric_limits<double>::max();
      dit--;
      (enigma::particles_impl->draw_particlesystems)(high, low);
    }
  }
  return 0;
}

void clear_view(float x, float y, float w, float h, float angle, bool showcolor)
{
  d3d_set_projection_ortho(x, y, w, h, angle);

  int clear_bits = 0;
  if (showcolor)
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
}

static inline void draw_gui()
{
  int culling = d3d_get_culling();
  bool hidden = d3d_get_hidden();
  d3d_set_culling(rs_none);
  d3d_set_hidden(false);

  bool stop_loop = false;
  for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++)
  {
    enigma::inst_iter* push_it = enigma::instance_event_iterator;
    //loop instances
    for (enigma::instance_event_iterator = dit->second.draw_events->next; enigma::instance_event_iterator != NULL; enigma::instance_event_iterator = enigma::instance_event_iterator->next) {
      enigma::object_graphics* inst = ((object_graphics*)enigma::instance_event_iterator->inst);
      if (inst->myevent_drawgui_subcheck())
        inst->myevent_drawgui();
      if (enigma::room_switching_id != -1) {
        stop_loop = true;
        break;
      }
    }
    enigma::instance_event_iterator = push_it;
    if (stop_loop) break;
  }

  // reset the culling
  d3d_set_culling(culling);
  // only restore hidden if the user didn't change it
  if (!d3d_get_hidden()) {
    d3d_set_hidden(hidden);
  }
}

namespace enigma_user
{

void screen_redraw()
{
  // Clean up any textures that ENIGMA may still think are binded but actually are not
  d3d_set_zwriteenable(true);
  if (!view_enabled)
  {
    if (bound_framebuffer != 0) //This fixes off-by-one error when rendering on surfaces. This should be checked to see if other GPU's have the same effect
      screen_set_viewport(1, 1, window_get_region_width(), window_get_region_height());
    else
      screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());

    clear_view(0, 0, window_get_region_width(), window_get_region_height(), 0, background_showcolor);
    draw_back();
    draw_insts();
    draw_tiles();
  }
  else
  {
    //TODO: Possibly implement view option from Stupido to control which view clears the background
    // Only clear the background on the first visible view by checking if it hasn't been cleared yet
    bool draw_backs = true;
    bool background_allviews = true; // FIXME: Create a setting for this.
    for (view_current = 0; view_current < 8; view_current++)
    {
      int vc = (int)view_current;
      if (!view_visible[vc])
        continue;

      int vob = (int)view_object[vc];
      if (vob != -1)
        follow_object(vob, vc);

      if (bound_framebuffer != 0) //This fixes off-by-one error when rendering on surfaces. This should be checked to see if other GPU's have the same effect
        screen_set_viewport(view_xport[vc]+1, view_yport[vc]+1, view_wport[vc], view_hport[vc]);
      else
        //printf("%d %d %d %d\n", (int)view_xport[vc], (int)view_yport[vc], (int)view_wport[vc], (int)view_hport[vc]),
        screen_set_viewport(view_xport[vc], view_yport[vc], view_wport[vc], view_hport[vc]);

      clear_view(view_xview[vc], view_yview[vc], view_wview[vc], view_hview[vc], view_angle[vc], background_showcolor && draw_backs);

      if (draw_backs)
        draw_back();

      draw_insts();

      if (draw_tiles())
        break;
      draw_backs = background_allviews;
    }
    // In Studio this variable is not reset until the next iteration of views as is actually 7 in the draw_gui event, in 8.1 however view_current will always be 0 in the step event.
    view_current = 0;
  }

  // Now process the sub event of draw called draw gui
  // It is for drawing GUI elements without view scaling and transformation
  if (enigma::gui_used)
  {
    screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());
    d3d_set_projection_ortho(0, 0, enigma::gui_width, enigma::gui_height, 0);

    // Clear the depth buffer if hidden surface removal is on at the beginning of the draw step.
    if (enigma::d3dMode)
      glClear(GL_DEPTH_BUFFER_BIT);

    draw_gui();
  }

  if (enigma::msaa_fbo != 0) {
    GLint fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, enigma::msaa_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //TODO: Change the code below to fix this to size properly to views
    glBlitFramebuffer(0, 0, window_get_region_width_scaled(), window_get_region_height_scaled(), 0, 0, window_get_region_width_scaled(), window_get_region_height_scaled(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    // glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
  }

  if (sprite_exists(cursor_sprite))
    draw_sprite(cursor_sprite, 0, mouse_x, mouse_y);

  ///TODO: screen_refresh() shouldn't be in screen_redraw(). They are separate functions for a reason.
  if (bound_framebuffer==0 || enigma::msaa_fbo != 0) { screen_refresh(); }
}

void screen_init()
{
  enigma::gui_width = window_get_region_width();
  enigma::gui_height = window_get_region_height();

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!view_enabled)
  {
    glClearColor(0,0,0,0);
    screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());
    d3d_set_projection_ortho(0, 0, room_width, room_height, 0);
  } else {
    for (view_current = 0; view_current < 7; view_current++)
    {
      if (view_visible[(int)view_current])
      {
        int vc = (int)view_current;

          glClearColor(0,0,0,0);

          screen_set_viewport(view_xport[vc], view_yport[vc], view_wport[vc], view_hport[vc]);
        d3d_set_projection_ortho(view_xview[vc], view_yview[vc], view_wview[vc], view_hview[vc], view_angle[vc]);
        break;
      }
    }
  }

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glEnable(GL_SCISSOR_TEST);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glAlphaFunc(GL_ALWAYS,0);
  texture_reset();
  draw_set_color(c_white);
}

int screen_save(string filename) { //Assumes native integers are little endian
	unsigned int w=window_get_width(),h=window_get_height(),sz=w*h;

	string ext = enigma::image_get_format(filename);

	unsigned char *rgbdata = new unsigned char[sz*4];
	GLint prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
 	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	glReadPixels(0,0,w,h, GL_BGRA, GL_UNSIGNED_BYTE, rgbdata);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, prevFbo);

	int ret = image_save(filename, rgbdata, w, h, w, h, false);

	delete[] rgbdata;
	return ret;
}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h) { //Assumes native integers are little endian
	unsigned sz = w*h;

	string ext = enigma::image_get_format(filename);

	unsigned char *rgbdata = new unsigned char[sz*4];
	GLint prevFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &prevFbo);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
 	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	glReadPixels(x,window_get_region_height_scaled()-h-y,w,h, GL_BGRA, GL_UNSIGNED_BYTE, rgbdata);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, prevFbo);

	int ret = image_save(filename, rgbdata, w, h, w, h, false);

	delete[] rgbdata;
	return ret;
}

void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height) {
  x = (x / window_get_region_width()) * window_get_region_width_scaled();
  y = (y / window_get_region_height()) * window_get_region_height_scaled();
  width = (width / window_get_region_width()) * window_get_region_width_scaled();
  height = (height / window_get_region_height()) * window_get_region_height_scaled();
  gs_scalar sx, sy;
  sx = (window_get_width() - window_get_region_width_scaled()) / 2;
  sy = (window_get_height() - window_get_region_height_scaled()) / 2;
  viewport_x = sx + x;
  viewport_y = window_get_height() - (sy + y) - height;
  viewport_w = width;
  viewport_h = height;
  //NOTE: OpenGL viewports are bottom left unlike Direct3D viewports which are top left
  glViewport(viewport_x, viewport_y, viewport_w, viewport_h);
  glScissor(viewport_x, viewport_y, viewport_w, viewport_h);
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
