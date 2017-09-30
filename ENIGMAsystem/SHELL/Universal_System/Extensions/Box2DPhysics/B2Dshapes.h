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

#ifndef _B2D_SHAPES__H
#define _B2D_SHAPES__H

#include <Box2D/Box2D.h>

namespace enigma_user {

enum {
  b2d_none,
  b2d_unknown,
  b2d_box,
  b2d_circle,
  b2d_edge,
  b2d_chain,
  b2d_loop,
  b2d_polygon
};

int b2d_shape_create();
void b2d_shape_delete(int id);
int b2d_shape_duplicate(int id);
void b2d_shape_box(int id, double halfwidth, double halfheight);
void b2d_shape_circle(int id, double radius);
void b2d_shape_add_point(int id, double x, double y);
void b2d_shape_remove_point(int id, int pn);
void b2d_shape_remove_points(int id, int startn, int endn);
void b2d_shape_clear_points(int id);
int b2d_shape_count_points(int id);
void b2d_shape_test_point(int id, double sx, double sy, double sa, double px, double py);
void b2d_shape_set_point(int id, int pn, double x, double y);
double b2d_shape_get_point_x(int id, int pn);
double b2d_shape_get_point_y(int id, int pn);
double b2d_shape_get_edge_point_x(int id, int en, int pn);
double b2d_shape_get_edge_point_y(int id, int en, int pn);
int b2d_shape_get_type(int id);
void b2d_shape_polygon(int id);
void b2d_shape_edge(int id, bool adjstart, bool adjend);
void b2d_shape_chain(int id);
void b2d_shape_loop(int id, bool adjstart, bool adjend);

int b2d_fixture_create(int bodyid, int shapeid);
void b2d_fixture_delete(int id);
void b2d_fixture_set_collision_group(int id, int group);
void b2d_fixture_set_density(int id, double density);
void b2d_fixture_set_friction(int id, double friction);
void b2d_fixture_set_restitution(int id, double restitution);
void b2d_fixture_set_sensor(int id, bool state);
int b2d_fixture_get_shape(int id);
int b2d_fixture_get_body(int id);

}

#endif // _ENIGMA_B2D_SHAPES__H

