/** Copyright (C) 2008-2013 Josh Ventura, Robert Colton
*** Copyright (C) 2018 Robert Colton
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
#include "GSstdraw.h"
#include "GSmodel.h"
#include "GStextures.h"

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h"
#endif

namespace {

// the batching mode is initialized to the default here
int draw_batch_mode = enigma_user::batch_flush_deferred;
// whether a batch has been started but not flushed yet
bool draw_batch_dirty = false;
// lazy create the batch stream that we use for combining primitives
int draw_get_batch_stream() {
  static int draw_batch_stream = -1;
  if (!enigma_user::d3d_model_exists(draw_batch_stream))
    draw_batch_stream = enigma_user::d3d_model_create(enigma_user::model_stream, true);
  return draw_batch_stream;
}
// helper function for beginning a deferred batch to determine when texture swap occurs
// one goal of the function is to ensure the render states are current when a batch begins
void draw_batch_begin_deferred(int texId) {
  // if we want to use a different texture, set it now
  // this marks the state as dirty only if the texture is different
  if (enigma_user::texture_get() != texId) {
    enigma_user::texture_set(texId);
  }
  // if the draw state is dirty, flush the new state
  // this will flush any previous batch first before
  // actually flushing our texture set to the device
  if (enigma::draw_get_state_dirty()) {
    enigma_user::draw_state_flush();
  }
  draw_batch_dirty = true;
}

} // anonymous namespace

namespace enigma_user
{

unsigned draw_primitive_count(int kind, unsigned vertex_count) {
  switch (kind) {
    case pr_pointlist: return vertex_count;
    case pr_linelist: return vertex_count / 2;
    case pr_linestrip: if (vertex_count > 1) return vertex_count - 1; break;
    case pr_trianglelist: return vertex_count / 3;
    case pr_trianglestrip: if (vertex_count > 2) return vertex_count - 2; break;
    case pr_trianglefan: if (vertex_count > 2) return vertex_count - 2; break;
    #ifdef DEBUG_MODE
    default: DEBUG_MESSAGE("Unknown primitive kind: " + std::to_string(kind), MESSAGE_TYPE::M_USER_ERROR);
    #endif
  }
  return 0;
}

void draw_batch_flush(int kind) {
  static bool flushing = false;

  // return if the kind of flush being requested
  // is not the mode of flushing we have enabled
  if (draw_batch_mode != kind) return;

  // guard against infinite recursion in case this flush
  // leads to other state changes that trigger another flush
  if (flushing || !draw_batch_dirty) return;
  flushing = true;

  // the never flush mode means the batch isn't drawn
  // we must still clear it from memory to avoid leaks
  if (draw_batch_mode != batch_flush_never) {
    // when we start a batch using draw_batch_begin_deferred
    // the render state is actually made current immediately
    // along with the texture that the batch uses so we don't
    // actually want any state flush to occur while we draw
    // this batch and then we can restore the dirty state for
    // the next batch or vertex submit to flush the new state
    bool wasStateDirty = enigma::draw_get_state_dirty();
    enigma::draw_set_state_dirty(false);
    d3d_model_draw(draw_get_batch_stream());
    enigma::draw_set_state_dirty(wasStateDirty);
  }
  d3d_model_clear(draw_get_batch_stream());

  flushing = false;
  draw_batch_dirty = false;
}

void draw_set_batch_mode(int mode) {
  if (draw_batch_mode != batch_flush_immediate &&
      draw_batch_mode != mode) {
    // if we were not doing immediate flushing before
    // we need to flush now before we switch modes or
    // the batch may never be drawn
    draw_batch_flush(draw_batch_mode);
  }
  draw_batch_mode = mode;
}

int draw_get_batch_mode() {
  return draw_batch_mode;
}

void draw_primitive_begin(int kind, int format)
{
  draw_batch_begin_deferred(-1);
  d3d_model_primitive_begin(draw_get_batch_stream(), kind, format);
}

void draw_primitive_begin_texture(int kind, int texId, int format)
{
  draw_batch_begin_deferred(texId);
  d3d_model_primitive_begin(draw_get_batch_stream(), kind, format);
}

void draw_primitive_end()
{
  d3d_model_primitive_end(draw_get_batch_stream());
  draw_batch_flush(batch_flush_immediate);
}

void draw_vertex(gs_scalar x, gs_scalar y)
{
  d3d_model_vertex(draw_get_batch_stream(), x, y);
}

void draw_vertex_color(gs_scalar x, gs_scalar y, int col, float alpha)
{
  d3d_model_vertex_color(draw_get_batch_stream(), x, y, col, alpha);
}

void draw_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_texture(draw_get_batch_stream(), x, y, tx, ty);
}

void draw_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, float alpha)
{
  d3d_model_vertex_texture_color(draw_get_batch_stream(), x, y, tx, ty, col, alpha);
}

void d3d_primitive_begin(int kind, int format)
{
  draw_primitive_begin(kind, format);
}

void d3d_primitive_begin_texture(int kind, int texId, int format)
{
  draw_primitive_begin_texture(kind, texId, format);
}

void d3d_primitive_end()
{
  draw_primitive_end();
}

void d3d_vertex(gs_scalar x, gs_scalar y, gs_scalar z)
{
  d3d_model_vertex(draw_get_batch_stream(), x, y, z);
}

void d3d_vertex_color(gs_scalar x, gs_scalar y, gs_scalar z, int color, double alpha)
{
  d3d_model_vertex_color(draw_get_batch_stream(), x, y, z, color, alpha);
}

void d3d_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_texture(draw_get_batch_stream(), x, y, z, tx, ty);
}

void d3d_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int color, double alpha)
{
  d3d_model_vertex_texture_color(draw_get_batch_stream(), x, y, z, tx, ty, color, alpha);
}

void d3d_vertex_normal(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz)
{
  d3d_model_vertex_normal(draw_get_batch_stream(), x, y, z, nx, ny, nz);
}

void d3d_vertex_normal_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int color, double alpha)
{
  d3d_model_vertex_normal_color(draw_get_batch_stream(), x, y, z, nx, ny, nz, color, alpha);
}

void d3d_vertex_normal_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_normal_texture(draw_get_batch_stream(), x, y, z, nx, ny, nz, tx, ty);
}

void d3d_vertex_normal_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int color, double alpha)
{
  d3d_model_vertex_normal_texture_color(draw_get_batch_stream(), x, y, z, nx, ny, nz, tx, ty, color, alpha);
}

void d3d_draw_floor(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep)
{
  texture_set_repeat(true);
  draw_batch_begin_deferred(texId);
  d3d_model_floor(draw_get_batch_stream(), x1, y1, z1, x2, y2, z2, hrep, vrep);
  draw_batch_flush(batch_flush_immediate);
}

void d3d_draw_wall(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep)
{
  texture_set_repeat(true);
  draw_batch_begin_deferred(texId);
  d3d_model_wall(draw_get_batch_stream(), x1, y1, z1, x2, y2, z2, hrep, vrep);
  draw_batch_flush(batch_flush_immediate);
}

void d3d_draw_block(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed)
{
  texture_set_repeat(true);
  draw_batch_begin_deferred(texId);
  d3d_model_block(draw_get_batch_stream(), x1, y1, z1, x2, y2, z2, hrep, vrep, closed);
  draw_batch_flush(batch_flush_immediate);
}

void d3d_draw_cylinder(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
  texture_set_repeat(true);
  draw_batch_begin_deferred(texId);
  d3d_model_cylinder(draw_get_batch_stream(), x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
  draw_batch_flush(batch_flush_immediate);
}

void d3d_draw_cone(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
  texture_set_repeat(true);
  draw_batch_begin_deferred(texId);
  d3d_model_cone(draw_get_batch_stream(), x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
  draw_batch_flush(batch_flush_immediate);
}

void d3d_draw_ellipsoid(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps)
{
  texture_set_repeat(true);
  draw_batch_begin_deferred(texId);
  d3d_model_ellipsoid(draw_get_batch_stream(), x1, y1, z1, x2, y2, z2, hrep, vrep, steps);
  draw_batch_flush(batch_flush_immediate);
}

void d3d_draw_icosahedron(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps) {
}

void d3d_draw_torus(gs_scalar x1, gs_scalar y1, gs_scalar z1, int texId, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius) {
  texture_set_repeat(true);
  draw_batch_begin_deferred(texId);
  d3d_model_torus(draw_get_batch_stream(), x1, y1, z1, hrep, vrep, csteps, tsteps, radius, tradius);
  draw_batch_flush(batch_flush_immediate);
}

}
