/** Copyright (C) 2013 Robert B. Colton
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

#ifndef ENIGMA_BOX2D_SHAPE_H
#define ENIGMA_BOX2D_SHAPE_H

#include <vector>
using std::vector;

#include <Box2D/Box2D.h>

struct B2DShape {
  int type;
  int worldid;
  b2Shape* shape;
  bool shapeBuilt;
  vector<b2Vec2> vertices;

  B2DShape()
  {
    shapeBuilt = false;
  }

  ~B2DShape()
  {

  }

};
extern vector<B2DShape*> b2dshapes;

struct B2DFixture {
  int worldid;
  int bodyid;
  int shapeid;
  b2Fixture* fixture;

  B2DFixture()
  {
  }

  ~B2DFixture()
  {

  }

};
extern vector<B2DFixture*> b2dfixtures;

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_shaper(s,id,r) \
    if (unsigned(id) >= b2dshapes.size() || id < 0) { \
      enigma_user::show_error("Cannot access Box2D physics shape with id " + toString(id), false); \
      return r; \
    } B2DShape* s = b2dshapes[id];
  #define get_shape(s,id) \
    if (unsigned(id) >= b2dshapes.size() || id < 0) { \
      enigma_user::show_error("Cannot access Box2D physics shape with id " + toString(id), false); \
      return; \
    } B2DShape* s = b2dshapes[id];
  #define get_fixturer(f,id,r) \
    if (unsigned(id) >= b2dfixtures.size() || id < 0) { \
      enigma_user::show_error("Cannot access Box2D physics fixture with id " + toString(id), false); \
      return r; \
    } B2DFixture* f = b2dfixtures[id];
  #define get_fixture(f,id) \
    if (unsigned(id) >= b2dfixtures.size() || id < 0) { \
      enigma_user::show_error("Cannot access Box2D physics fixture with id " + toString(id), false); \
      return; \
    } B2DFixture* f = b2dfixtures[id];
  #define check_cast(obj, shapeid, failv) { \
    if ((obj)->shape != shapeid) { \
      enigma_user::show_error("Invalid cast of Box2D shape.", false); return failv; \
    } }
#else
  #define get_shaper(s,id,r) \
    B2DShape* s = b2dshapes[id];
  #define get_shape(s,id) \
    B2DShape* s = b2dshapes[id];
  #define get_fixturer(f,id,r) \
    B2DFixture* f = b2dfixtures[id];
  #define get_fixture(f,id) \
    B2DFixture* f = b2dfixtures[id];
  #define check_cast(obj, shapeid, failv)
#endif

#endif // ENIGMA_BOX2D_SHAPE_H
