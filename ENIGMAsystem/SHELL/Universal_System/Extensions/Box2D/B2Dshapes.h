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

#ifndef _JOINTS__H
#define _JOINTS__H

#include <vector>
using std::vector;

#include <Box2D/Box2D.h>

struct B2DShape {
  int worldid;
  b2Shape* shape;
  b2PolygonShape* polyshape;
  vector<b2Vec2> vertices;

  B2DShape()
  {
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

namespace enigma_user {

/** description...
@return returns...
**/
int b2d_shape_create();
/** description...
@param id
**/
void b2d_shape_delete(int id);
/** description...
@param id
**/
int b2d_shape_duplicate(int id);
/** description...
@param id
@param halfwidth
@param halfheight
**/
void b2d_shape_box(int id, double halfwidth, double halfheight);
/** description...
@param id
@param radius
**/
void b2d_shape_circle(int id, double radius);
/** description...
@param id
@param x
@param y
**/
void b2d_shape_add_point(int id, double x, double y);
/** description...
@param id
@param pn
**/
void b2d_shape_remove_point(int id, int pn);
/** description...
@param id
@param startn
@param endn
**/
void b2d_shape_remove_points(int id, int startn, int endn);
/** description...
@param id
**/
void b2d_shape_clear_points(int id);
/** description...
@param id
**/
int b2d_shape_count_points(int id);
/** description...
@param id
@param x
@param y
**/
void b2d_shape_test_point(int id, double sx, double sy, double sa, double px, double py);
/** description...
@param id
**/
void b2d_shape_polygon(int id);
/** description...
@param id
**/
void b2d_shape_edge(int id, bool adjstart, bool adjend);
/** description...
@param id
**/
void b2d_shape_chain(int id);
/** description...
@param id
**/
void b2d_shape_loop(int id, bool adjstart, bool adjend);

/** description...
@param bodyid
@param shapeid
@return returns...
**/
int b2d_fixture_create(int bodyid, int shapeid);
/** description...
@param id
**/
void b2d_fixture_delete(int id);
/** description...
@param id
@param group
**/
void b2d_fixture_set_collision_group(int id, int group);
/** description...
@param id
@param density
**/
void b2d_fixture_set_density(int id, double density);
/** description...
@param id
@param friction
**/
void b2d_fixture_set_friction(int id, double friction);
/** description...
@param id
@param restitution
**/
void b2d_fixture_set_restitution(int id, double restitution);
/** description...
@param id
@param state
**/
void b2d_fixture_set_sensor(int id, bool state);
/** description...
@param id
**/
int b2d_fixture_get_shape(int id);
/** description...
@param id
**/
int b2d_fixture_get_body(int id);

}

#endif
