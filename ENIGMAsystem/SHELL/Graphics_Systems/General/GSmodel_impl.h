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

#include <vector>
using std::vector;

namespace enigma {

struct Primitive {
  int type; // one of the enigma_user primitive type constants (e.g, pr_trianglelist)
  int format; // index of the user vertex format that describes the vertex data of this primitive
  bool format_defined; // if the format has been guessed yet from how vertex data is being specified
  unsigned vertex_start, vertex_count; // range of vertices in the vertex buffer this primitive specified

  // NOTE: format may not exist until d3d_model_primitive_end is called
  // NOTE: when format_defined is true the format may still not exist yet

  Primitive(int type, int format, bool format_defined, unsigned start):
    type(type), format(format), format_defined(format_defined), vertex_start(start), vertex_count(0) {}
};

struct Model {
  int type; // one of the enigma_user model type constants (e.g, model_static is the default)
  int vertex_buffer; // index of the user vertex buffer this model uses to buffer its vertex data
  Primitive* current_primitive; // pointer to the current primitive being specified by the user
  bool vertex_started; // whether the user has begun specifying the model by starting a primitive
  vector<Primitive> primitives; // all primitives the user has finished specifying for this model

  // NOTE: vertex_buffer should always exist but not outlive the model
  // NOTE: current_primitive does not exist outside of the begin/end calls
  // NOTE: vertex_started is true until the user attempts to draw the model

  Model(int type): type(type), vertex_buffer(-1), current_primitive(0), vertex_started(false) {}
};


extern vector<Model*> models;

}

#endif
