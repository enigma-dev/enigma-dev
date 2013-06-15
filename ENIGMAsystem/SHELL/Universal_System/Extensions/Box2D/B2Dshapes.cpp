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

#include "B2Dshapes.h"
#include "B2Dfunctions.h"
vector<B2DShape*> b2dshapes;
vector<B2DFixture*> b2dfixtures;

#include <cmath>
#include <cstdlib>
#include <string>
using std::string;

#ifdef DEBUG_MODE
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_shaper(s,id,r) \
    if (unsigned(id) >= b2dshapes.size() || id < 0) { \
      show_error("Cannot access Box2D physics shape with id " + toString(id), false); \
      return r; \
    } B2DShape* s = b2dshapes[id];
  #define get_shape(s,id) \
    if (unsigned(id) >= b2dshapes.size() || id < 0) { \
      show_error("Cannot access Box2D physics shape with id " + toString(id), false); \
      return; \
    } B2DShape* s = b2dshapes[id];
  #define get_fixturer(f,id,r) \
    if (unsigned(id) >= b2dfixtures.size() || id < 0) { \
      show_error("Cannot access Box2D physics fixture with id " + toString(id), false); \
      return r; \
    } B2DFixture* f = b2dfixtures[id];
  #define get_fixture(f,id) \
    if (unsigned(id) >= b2dfixtures.size() || id < 0) { \
      show_error("Cannot access Box2D physics fixture with id " + toString(id), false); \
      return; \
    } B2DFixture* f = b2dfixtures[id];
#else
  #define get_shaper(s,id,r) \
    B2DShape* s = b2dshapes[id];
  #define get_shape(s,id) \
    B2DShape* s = b2dshapes[id];
  #define get_fixturer(f,id,r) \
    B2DFixture* f = b2dfixtures[id];
  #define get_fixture(f,id) \
    B2DFixture* f = b2dfixtures[id];
#endif

namespace enigma_user {

int b2d_shape_create()
{
  int i = b2dshapes.size();
  B2DShape* b2dshape = new B2DShape();
  b2dshapes.push_back(b2dshape);
  return i;
}

void b2d_shape_delete(int id)
{
  get_shape(b2dshape, id);
  delete b2dshape;
}

int b2d_shape_duplicate(int id)
{
  get_shape(b2dshape, id);
  int i = b2dshapes.size();
  B2DShape* nb2dshape = new B2DShape();
  nb2dshape->shape = b2dshape->shape->Clone(new b2BlockAllocator());
  copy(b2dshape->vertices.begin(),b2dshape->vertices.end(),nb2dshape->vertices.begin());
  b2dshapes.push_back(nb2dshape);
  return i;
}

void b2d_shape_box(int id, double halfwidth, double halfheight)
{
  get_shape(b2dshape, id);
  b2PolygonShape* shape = new b2PolygonShape();
  shape->SetAsBox(halfwidth, halfheight);
  b2dshape->shape = shape;
}

void b2d_shape_circle(int id, double radius)
{
  get_shape(b2dshape, id);
  b2CircleShape* shape = new b2CircleShape();
  shape->m_radius = radius;
  b2dshape->shape = shape;
}

void b2d_shape_add_point(int id, double x, double y)
{
  get_shape(b2dshape, id);
  b2dshape->vertices.push_back(b2Vec2(x, y));
}

void b2d_shape_remove_point(int id, int pn)
{
  get_shape(b2dshape, id);
  b2dshape->vertices.erase(b2dshape->vertices.begin() + pn);
}

void b2d_shape_remove_points(int id, int startn, int endn)
{
  get_shape(b2dshape, id);
  b2dshape->vertices.erase(b2dshape->vertices.begin() + startn, b2dshape->vertices.begin() + endn);
}

void b2d_shape_clear_points(int id)
{
  get_shape(b2dshape, id);
  b2dshape->vertices.clear();
}

int b2d_shape_count_points(int id)
{
  get_shape(b2dshape, id);
  return b2dshape->vertices.size();
}

void b2d_shape_test_point(int id, double sx, double sy, double sa, double px, double py)
{
  get_shape(b2dshape, id);
  b2dshape->shape->TestPoint(b2Transform(b2Vec2(sx, sy), b2Rot(sa)), b2Vec2(px, py));
}

void b2d_shape_polygon(int id)
{
  get_shape(b2dshape, id);
  b2PolygonShape* shape = new b2PolygonShape();

  shape->Set(&b2dshape->vertices[0],  b2dshape->vertices.size());
  b2dshape->shape = shape;
}

void b2d_shape_edge(int id, bool adjstart, bool adjend)
{
  get_shape(b2dshape, id);
  b2EdgeShape* shape = new b2EdgeShape();

  int vid = 0;
  if (adjstart) {
    shape->m_hasVertex0 = true;
    shape->m_vertex0 = b2dshape->vertices[vid];
    vid += 1;
  }
  shape->Set(b2dshape->vertices[vid],  b2dshape->vertices[vid + 1]);
  vid += 2;
  if (adjend) {
    shape->m_hasVertex3 = true;
    shape->m_vertex3 = b2dshape->vertices[vid];
  }
  b2dshape->shape = shape;
}

void b2d_shape_chain(int id)
{
  get_shape(b2dshape, id);
  b2ChainShape* shape = new b2ChainShape();

  shape->CreateChain(&b2dshape->vertices[0],  b2dshape->vertices.size());
  b2dshape->shape = shape;
}

void b2d_shape_loop(int id, bool adjstart, bool adjend)
{
  get_shape(b2dshape, id);
  b2ChainShape* shape = new b2ChainShape();

  if (adjstart) {
    shape->SetPrevVertex(b2dshape->vertices[0]);
  }
  shape->CreateLoop(&b2dshape->vertices[adjstart],  b2dshape->vertices.size() - adjstart - adjend);
  if (adjend) {
    shape->SetNextVertex(b2dshape->vertices[b2dshape->vertices.size() - 1]);
  }
  b2dshape->shape = shape;
}

int b2d_fixture_create(int bodyid, int shapeid)
{
  if (unsigned(bodyid) >= b2dbodies.size() || bodyid < 0 || 
	unsigned(shapeid) >= b2dshapes.size() || shapeid < 0)
  {
    return -1;
  }
  else
  {
    B2DFixture* b2dfixture = new B2DFixture();
    b2FixtureDef fixtureDef;
    fixtureDef.shape = b2dshapes[shapeid]->shape;
    b2dfixture->fixture = b2dbodies[bodyid]->body->CreateFixture(&fixtureDef);
    b2dfixture->bodyid = bodyid;
    b2dfixture->shapeid = shapeid;
    int i = b2dfixtures.size();
    b2dbodies[bodyid]->fixtures.push_back(i);
    b2dfixtures.push_back(b2dfixture);
    return i;
  }
}

void b2d_fixture_delete(int id)
{
  get_fixture(b2dfixture, id);
  delete b2dfixture;
}

void b2d_fixture_set_collision_group(int id, int group)
{
  get_fixture(b2dfixture, id);
  // sets the collision group used to make parts of things not collide, like a ragdoll for
  // instance should not collide with itself
  b2Filter newfilter;
  newfilter.groupIndex = group;
  b2dfixture->fixture->SetFilterData(newfilter);
}

int b2d_fixture_get_shape(int id)
{
  get_fixturer(b2dfixture, id, -1);
  return b2dfixture->shapeid;
}

int b2d_fixture_get_body(int id)
{
  get_fixturer(b2dfixture, id, -1);
  return b2dfixture->bodyid;
}

void b2d_fixture_set_density(int id, double density)
{
  get_fixture(b2dfixture, id);
  b2dfixture->fixture->SetDensity(density);
}

void b2d_fixture_set_friction(int id, double friction)
{
  get_fixture(b2dfixture, id);
  b2dfixture->fixture->SetFriction(friction);
}

void b2d_fixture_set_restitution(int id, double restitution)
{
  get_fixture(b2dfixture, id);
  b2dfixture->fixture->SetRestitution(restitution);
}

void b2d_fixture_set_sensor(int id, bool state)
{
  get_fixture(b2dfixture, id);
  b2dfixture->fixture->SetSensor(state);
}

}
