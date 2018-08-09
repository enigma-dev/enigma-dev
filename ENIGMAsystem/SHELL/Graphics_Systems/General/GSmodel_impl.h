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
  int type, format;
  unsigned vertex_start, vertex_count;

  Primitive(int type, int format, unsigned start): type(type), format(format), vertex_start(start), vertex_count(0) {}
};

struct Model {
  int type, vertex_buffer;
  Primitive* current_primitive;
  bool vertex_started;
  vector<Primitive> primitives;

  Model(int type): type(type), vertex_buffer(-1), current_primitive(0), vertex_started(false) {}
};


extern vector<Model*> models;

}

#endif
