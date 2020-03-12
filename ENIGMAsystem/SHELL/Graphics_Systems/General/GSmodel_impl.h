/** Copyright (C) 2013 Robert Colton, Adriano Tumminelli
*** Copyright (C) 2014 Seth N. Hetu, Josh Ventura, Harijs Grinbergs
*** Copyright (C) 2015 Harijs Grinbergs
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

#ifdef INCLUDED_FROM_SHELLMAIN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_GSMODEL_IMPL_H
#define ENIGMA_GSMODEL_IMPL_H

#include "GSmodel.h"
#include "GScolors.h"
#include "Universal_System/scalar.h"
#include "Universal_System/Resources/AssetArray.h"
using enigma::AssetArray;

#include <vector>
using std::vector;
// this is for GLM's transition to radians only
// let's define it just in case some enigma
// user's package manager happens to be old
#define GLM_FORCE_RADIANS
// first one is deprecated but define it
// anyway for the same reasons as above
#define GLM_FORCE_LEFT_HANDED

namespace enigma {

struct Primitive {
  int type; // one of the enigma_user primitive type constants (e.g, pr_trianglelist)
  int format; // index of the user vertex format that describes the vertex data of this primitive
  bool format_started; // if the format has been started yet from specifying any vertex data
  bool format_defined; // if the format has been guessed yet from how vertex data is being specified
  unsigned vertex_offset; // byte offset into the vertex buffer where this primitive's data begins
  unsigned vertex_count; // number of vertices this primitive is composed of

  // NOTE: format may not exist until d3d_model_primitive_end is called
  // NOTE: when format_defined is true the format may still not exist yet

  Primitive(): type(0), format(-1), format_started(false), format_defined(false), vertex_offset(0), vertex_count(0) {}
  Primitive(int type, int format, bool format_exists, unsigned offset):
    type(type), format(format), format_started(format_exists), format_defined(format_exists), vertex_offset(offset), vertex_count(0) {}
};

class Model {
  bool destroyed;
 public:
  int type; // one of the enigma_user model type constants (e.g, model_static is the default)
  int vertex_buffer; // index of the user vertex buffer this model uses to buffer its vertex data
  Primitive current_primitive; // the current primitive being specified by the user
  bool vertex_started; // whether the user has begun specifying the model by starting a primitive
  vector<Primitive> primitives; // all primitives the user has finished specifying for this model

  bool use_draw_color; // whether to store the current color per-vertex when no format given
  bool vertex_colored; // whether the last vertex specified color or not
  int vertex_color; // the color that was set when the last vertex was added
  gs_scalar vertex_alpha; // the alpha that was set when the last vertex was added

  // NOTE: vertex_buffer should always exist but not outlive the model
  // NOTE: current_primitive is not reset until the next begin call
  // NOTE: current_primitive is not a pointer (same allocation) to avoid allocation overhead
  // NOTE: vertex_started is true until the user attempts to draw the model
  // NOTE: vertex_colored waits until the next vertex or primitive end to add the color
  //       because GM has always specified color as the last argument on its vertex formats

  Model(int type = enigma_user::model_static, bool use_draw_color = false):
    destroyed(false), type(type), vertex_buffer(-1), current_primitive(), vertex_started(false),
    use_draw_color(use_draw_color), vertex_colored(true), vertex_color(enigma_user::c_white), vertex_alpha(1.0) {}

  void destroy() {
    destroyed = true;
    // this will give us 0 size and 0 capacity
    vector<Primitive>().swap(primitives);
  }

  bool isDestroyed() const { return destroyed; }

  static const char* getAssetTypeName() { return "model"; }
};

namespace enigma {

extern glm::mat4 world, view, projection;

} // namespace enigma

#endif // ENIGMA_GSMATRIX_IMPL_H
