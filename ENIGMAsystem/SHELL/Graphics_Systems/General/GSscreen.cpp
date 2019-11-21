/** Copyright (C) 2008-2014 Josh Ventura, Harijs Grinbergs, Robert B. Colton
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

#include "GSscreen.h"
#include "GSstdraw.h"
#include "GStextures.h"
#include "GSsprite.h"
#include "GStilestruct.h"
#include "GSbackground.h"
#include "GSd3d.h"
#include "GSmatrix.h"
#include "GSprimitives.h"
#include "GSvertex.h"
#include "GScolors.h"

#include "Universal_System/nlpo2.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/Resources/background.h"
#include "Universal_System/Object_Tiers/graphics_object.h"
#include "Universal_System/depth_draw.h"
#include "Universal_System/Instances/instance_system.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/Resources/background_internal.h"
#include "Universal_System/Resources/sprites_internal.h"
#include "Platforms/General/PFwindow.h"
#include "Graphics_Systems/graphics_mandatory.h"

#include <string>
#include <cstdio>
#include <limits>

using namespace enigma;
using namespace enigma_user;
using namespace std;

namespace {

bool clamp_view(int& x, int& y) {
  if (view_enabled) {
    x = x - enigma_user::view_xview[enigma_user::view_current];
    y = y - enigma_user::view_yview[enigma_user::view_current];
    if (x > enigma_user::view_wview[enigma_user::view_current] || y > enigma_user::view_hview[enigma_user::view_current]) return true;
  } else {
    if (x > enigma_user::room_width || y > enigma_user::room_height) return true;
  }
  if (x < 0) x = 0;
  if (y < 0) y = 0;
  return false;
}

//These are used to reset the screen viewport for surfaces
gs_scalar viewport_x, viewport_y, viewport_w, viewport_h;

} // namespace anonymous

namespace enigma {

std::vector<std::function<void()> > extension_draw_gui_after_hooks;

unsigned gui_width = 0;
unsigned gui_height = 0;

particles_implementation* particles_impl;
void set_particles_implementation(particles_implementation* part_impl)
{
  particles_impl = part_impl;
}

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
            background_yscale[back_current], background_coloring[back_current], background_alpha[back_current], background_htiled[back_current], background_vtiled[back_current]);
        } else {
          draw_background_ext(background_index[back_current], background_x[back_current], background_y[back_current], background_xscale[back_current], background_yscale[back_current], 0, background_coloring[back_current], background_alpha[back_current]);
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
  enigma::load_tiles();
  for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++)
  {
    if (dit->second.tiles.size())
    {
      for (auto &t : tile_layer_metadata[dit->second.tiles[0].depth]) {
        enigma_user::index_submit_range(enigma::tile_index_buffer, enigma::tile_vertex_buffer, enigma_user::pr_trianglelist, t[0], t[1], t[2]);
      }
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
  if (enigma::d3dMode && enigma::d3dPerspective)
    d3d_set_projection_perspective(x, y, w, h, angle);
  else
    d3d_set_projection_ortho(x, y, w, h, angle);

  if (showcolor)
    enigma_user::draw_clear(background_color);

  // Clear the depth buffer if 3d mode is on at the beginning of the draw step.
  // Always clear the depth buffer for every view, this was Game Maker 8.1 behaviour
  if (enigma::d3dMode)
    enigma_user::d3d_clear_depth();
}

static inline void draw_gui()
{
  // turn some state off automatically for the user to draw the GUI
  // this is exactly what GMSv1.4 does
  int culling = enigma::d3dCulling;
  bool hidden = enigma::d3dHidden;
  bool zwrite = enigma::d3dZWriteEnable;
  d3d_set_zwriteenable(false);
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

  // reset the state to what the user had
  d3d_set_culling(culling);
  d3d_set_hidden(hidden);
  d3d_set_zwriteenable(zwrite);
}

static inline void draw_gui_after_hooks() {
  // if any extension wants to draw over the user
  // GUI let's go ahead and call those hooks now
  for (auto draw_gui_after_hook : extension_draw_gui_after_hooks)
    draw_gui_after_hook();
}

namespace enigma_user {

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

void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height) {
  draw_batch_flush(batch_flush_deferred);

  x = (x / window_get_region_width()) * window_get_region_width_scaled();
  y = (y / window_get_region_height()) * window_get_region_height_scaled();
  width = (width / window_get_region_width()) * window_get_region_width_scaled();
  height = (height / window_get_region_height()) * window_get_region_height_scaled();
  gs_scalar sx, sy;
  sx = (window_get_width() - window_get_region_width_scaled()) / 2;
  sy = (window_get_height() - window_get_region_height_scaled()) / 2;
  viewport_x = sx + x;
  viewport_y = sy + y;
  viewport_w = width;
  viewport_h = height;

  screen_reset_viewport();
}

void screen_reset_viewport() {
  graphics_set_viewport(viewport_x, viewport_y, viewport_w, viewport_h);
}

void screen_init() {
  enigma::gui_width = window_get_region_width();
  enigma::gui_height = window_get_region_height();

  draw_clear(c_black);
  d3d_clear_depth(0.0f);

  if (!view_enabled)
  {
    screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());
    d3d_set_projection_ortho(0, 0, room_width, room_height, 0);
  } else {
    for (view_current = 0; view_current < 7; view_current++)
    {
      if (view_visible[(int)view_current])
      {
        int vc = (int)view_current;

        screen_set_viewport(view_xport[vc], view_yport[vc], view_wport[vc], view_hport[vc]);
        d3d_set_projection_ortho(view_xview[vc], view_yview[vc], view_wview[vc], view_hview[vc], view_angle[vc]);
        break;
      }
    }
  }

  enigma::d3dHidden = false;
  enigma::d3dCulling = rs_none;
  enigma::alphaBlend = true;
  texture_reset();

  // make sure all of the default state values are
  // synchronzied at least once with the device/context
  enigma::draw_set_state_dirty();
  draw_state_flush();
}

void screen_refresh() {
  draw_batch_flush(batch_flush_deferred);
  enigma::ScreenRefresh();
}

void screen_redraw()
{
  enigma::scene_begin();

  if (!view_enabled)
  {
    screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());

    clear_view(0, 0, window_get_region_width(), window_get_region_height(), 0, background_showcolor);
    draw_back();
    draw_insts();
    draw_tiles();
  }
  else
  {
    // update object following first in case the user accesses
    // a view other than the current one in the draw event
    for (view_current = 0; view_current < 8; view_current++)
    {
      int vc = (int)view_current;
      if (!view_visible[vc])
        continue;

      int vob = (int)view_object[vc];
      if (vob != -1)
        follow_object(vob, vc);
    }

    //TODO: Possibly implement view option from Stupido to control which view clears the background
    // Only clear the background on the first visible view by checking if it hasn't been cleared yet
    bool draw_backs = true;
    bool background_allviews = true; // FIXME: Create a setting for this.
    for (view_current = 0; view_current < 8; view_current++)
    {
      int vc = (int)view_current;
      if (!view_visible[vc])
        continue;

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

  // normal draw events over, do an implicit flush
  draw_batch_flush(batch_flush_deferred);

  // Now process the sub event of draw called draw gui
  // It is for drawing GUI elements without view scaling and transformation
  screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());
  d3d_set_projection_ortho(0, 0, enigma::gui_width, enigma::gui_height, 0);

  if (enigma::gui_used)
  {
    // Clear the depth buffer if hidden surface removal is on at the beginning of the draw step.
    if (enigma::d3dMode)
      d3d_clear_depth();

    draw_gui();
  }

  // allow extensions to draw after GUI events handled if there are any
  draw_gui_after_hooks();

  // do an implicit flush to catch anything from the draw GUI events
  draw_batch_flush(batch_flush_deferred);

  if (sprite_exists(cursor_sprite)) {
    draw_sprite(cursor_sprite, 0, mouse_x, mouse_y);
    // cursor sprite needs its own flush before this frame ends
    draw_batch_flush(batch_flush_deferred);
  }

  // must do at least one flush for the never mode to prevent leaks
  draw_batch_flush(batch_flush_never);

  enigma::scene_end();

  // GM8.1 manual specifies that screen_redraw should call screen_refresh
  // "The first function redraws the internal image and then refreshes the screen image."
  screen_refresh();
}

int screen_save(string filename) { //Assumes native integers are little endian
  draw_batch_flush(batch_flush_deferred);

  unsigned int fw = 0, fh = 0;
  bool flipped = false;
  unsigned char* rgba = enigma::graphics_copy_screen_pixels(&fw,&fh,&flipped);
  int ret = image_save(filename, rgba, fw, fh, fw, fh, flipped);

  delete[] rgba;
  return ret;
}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h) { //Assumes native integers are little endian
  draw_batch_flush(batch_flush_deferred);

  bool flipped = false;
  unsigned char* rgba = enigma::graphics_copy_screen_pixels(x,y,w,h,&flipped);
  int ret = image_save(filename, rgba, w, h, w, h, flipped);

  delete[] rgba;
  return ret;
}

int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload)
{
  draw_batch_flush(batch_flush_deferred);

  bool flipped = false;
  unsigned char* rgba = enigma::graphics_copy_screen_pixels(x,y,w,h,&flipped);

  if (flipped)
    rgba = enigma::image_flip(rgba, w, h, 4);

  enigma::backgroundstructarray_reallocate();
  int bckid=enigma::background_idmax;
  enigma::background_new(bckid, w, h, &rgba[0], removeback, smooth, preload, false, 0, 0, 0, 0, 0, 0);
  delete[] rgba;
  enigma::background_idmax++;
  return bckid;
}

int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig) {
  draw_batch_flush(batch_flush_deferred);

  bool flipped = false;
  unsigned char* rgba = enigma::graphics_copy_screen_pixels(x,y,w,h,&flipped);

  if (flipped)
    rgba = enigma::image_flip(rgba, w, h, 4);

  enigma::spritestructarray_reallocate();
  int sprid=enigma::sprite_idmax;
  enigma::sprite_new_empty(sprid, 1, w, h, xorig, yorig, 0, h, 0, w, preload, smooth);
  enigma::sprite_set_subimage(sprid, 0, w, h, rgba, rgba, enigma::ct_precise); //TODO: Support toggling of precise.
  delete[] rgba;
  return sprid;
}

int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig) {
  return sprite_create_from_screen(x, y, w, h, removeback, smooth, true, xorig, yorig);
}

void sprite_add_from_screen(int id, int x, int y, int w, int h, bool removeback, bool smooth) {
  draw_batch_flush(batch_flush_deferred);

  bool flipped = false;
  unsigned char* rgba = enigma::graphics_copy_screen_pixels(x,y,w,h,&flipped);

  if (flipped)
    rgba = enigma::image_flip(rgba, w, h, 4);

  enigma::sprite_add_subimage(id, w, h, rgba, rgba, enigma::ct_precise); //TODO: Support toggling of precise.
  delete[] rgba;
}

int draw_getpixel(int x,int y)
{
  if (clamp_view(x,y)) return 0;
  draw_batch_flush(batch_flush_deferred);

  unsigned char* rgba = enigma::graphics_copy_screen_pixels(x,y,1,1);
  int ret = rgba[2] | rgba[1] << 8 | rgba[0] << 16;
  delete[] rgba;
  return ret;
}

int draw_getpixel_ext(int x,int y)
{
  if (clamp_view(x,y)) return 0;
  draw_batch_flush(batch_flush_deferred);

  unsigned char* rgba = enigma::graphics_copy_screen_pixels(x,y,1,1);
  int ret = rgba[2] | rgba[1] << 8 | rgba[0] << 16 | rgba[3] << 24;
  delete[] rgba;
  return ret;
}

}
