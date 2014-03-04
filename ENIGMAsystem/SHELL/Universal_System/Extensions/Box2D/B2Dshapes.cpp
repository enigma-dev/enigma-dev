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

#include "Box2DShape.h"
#include "Box2DWorld.h"
#include "B2Dshapes.h"
#include "B2Dfunctions.h"
vector<B2DShape*> b2dshapes;
vector<B2DFixture*> b2dfixtures;

#include <cmath>
#include <cstdlib>
#include <string>
using std::string;

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
  get_shaper(b2dshape, id, -1);
  int i = b2dshapes.size();
  B2DShape* nb2dshape = new B2DShape();
  nb2dshape->shape = b2dshape->shape->Clone(new b2BlockAllocator());
  nb2dshape->type = b2dshape->type;
  nb2dshape->shapeBuilt = true;
  nb2dshape->worldid = b2dshape->worldid;
  copy(b2dshape->vertices.begin(),b2dshape->vertices.end(),nb2dshape->vertices.begin());
  b2dshapes.push_back(nb2dshape);
  return i;
}

void b2d_shape_box(int id, double halfwidth, double halfheight)
{
  get_shape(b2dshape, id);
  b2PolygonShape* shape;
  if (b2dshape->shapeBuilt) {
    if (b2dshape->shape->GetType() != b2Shape::e_polygon) {
      delete b2dshape->shape;
      shape = new b2PolygonShape();
    } else {
      shape = (b2PolygonShape*)b2dshape->shape;
    }
  } else {
    b2dshape->shapeBuilt = true;
    shape = new b2PolygonShape();
  }
  shape->SetAsBox(halfwidth, halfheight);
  b2dshape->type = b2d_box;
  b2dshape->shape = shape;
}

void b2d_shape_circle(int id, double radius)
{
  get_shape(b2dshape, id);
  b2CircleShape* shape;
  if (b2dshape->shapeBuilt) {
    if (b2dshape->shape->GetType() != b2Shape::e_circle) {
      delete b2dshape->shape;
      shape = new b2CircleShape();
    } else {
      shape = (b2CircleShape*)b2dshape->shape;
    }
  } else {
    b2dshape->shapeBuilt = true;
    shape = new b2CircleShape();
  }
  shape->m_radius = radius;
  b2dshape->type = b2d_circle;
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
  get_shaper(b2dshape, id, -1);
  return b2dshape->vertices.size();
}

void b2d_shape_test_point(int id, double sx, double sy, double sa, double px, double py)
{
  get_shape(b2dshape, id);
  b2dshape->shape->TestPoint(b2Transform(b2Vec2(sx, sy), b2Rot(sa)), b2Vec2(px, py));
}

void b2d_shape_set_point(int id, int pn, double x, double y)
{
  get_shape(b2dshape, id);
  b2dshape->vertices[pn].Set(x, y);
}

double b2d_shape_get_point_x(int id, int pn)
{
  get_shaper(b2dshape, id, -1);
  return b2dshape->vertices[pn].x;
}

double b2d_shape_get_point_y(int id, int pn)
{
  get_shaper(b2dshape, id, -1);
  return b2dshape->vertices[pn].y;
}

double b2d_shape_get_edge_point_x(int id, int en, int pn)
{
  get_shaper(b2dshape, id, -1);
  b2EdgeShape edge;
  //check_cast(b2dshape, b2ChainShape*, -1);
  b2ChainShape* shape = (b2ChainShape*)b2dshape->shape;
  shape->GetChildEdge(&edge, en);
  switch (pn) {
    case 0:
      return edge.m_vertex0.x;
      break;
    case 1:
      return edge.m_vertex1.x;
      break;
    case 2:
      return edge.m_vertex2.x;
      break;
    case 3:
      return edge.m_vertex3.x;
      break;
  }
}

double b2d_shape_get_edge_point_y(int id, int en, int pn)
{
  get_shaper(b2dshape, id, -1);
  b2EdgeShape edge;
  //check_cast(b2dshape, b2ChainShape*, -1);
  b2ChainShape* shape = (b2ChainShape*)b2dshape->shape;
  shape->GetChildEdge(&edge, en);
  switch (pn) {
    case 0:
      return edge.m_vertex0.y;
      break;
    case 1:
      return edge.m_vertex1.y;
      break;
    case 2:
      return edge.m_vertex2.y;
      break;
    case 3:
      return edge.m_vertex3.y;
      break;
  }
}

int b2d_shape_get_type(int id)
{
  get_shaper(b2dshape, id, -1);
  return b2dshape->type;
}

void b2d_shape_polygon(int id)
{
  get_shape(b2dshape, id);
  b2PolygonShape* shape;
  if (b2dshape->shapeBuilt) {
    if (b2dshape->shape->GetType() != b2Shape::e_polygon) {
      delete b2dshape->shape;
      shape = new b2PolygonShape();
    } else {
      shape = (b2PolygonShape*)b2dshape->shape;
    }
  } else {
    b2dshape->shapeBuilt = true;
    shape = new b2PolygonShape();
  }
  shape->Set(&b2dshape->vertices[0],  b2dshape->vertices.size());
  b2dshape->type = b2d_polygon;
  b2dshape->shape = shape;
}

void b2d_shape_edge(int id, bool adjstart, bool adjend)
{
  get_shape(b2dshape, id);
  b2EdgeShape* shape;
  if (b2dshape->shapeBuilt) {
    if (b2dshape->shape->GetType() != b2Shape::e_edge) {
      delete b2dshape->shape;
      shape = new b2EdgeShape();
    } else {
      shape = (b2EdgeShape*)b2dshape->shape;
    }
  } else {
    b2dshape->shapeBuilt = true;
    shape = new b2EdgeShape();
  }
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
  b2dshape->type = b2d_edge;
  b2dshape->shape = shape;
}

void b2d_shape_chain(int id)
{
  get_shape(b2dshape, id);
  b2ChainShape* shape;
  if (b2dshape->shapeBuilt) {
    if (b2dshape->shape->GetType() != b2Shape::e_chain) {
      delete b2dshape->shape;
      shape = new b2ChainShape();
    } else {
      shape = (b2ChainShape*)b2dshape->shape;
    }
  } else {
    b2dshape->shapeBuilt = true;
    shape = new b2ChainShape();
  }
  shape->CreateChain(&b2dshape->vertices[0],  b2dshape->vertices.size());
  b2dshape->type = b2d_chain;
  b2dshape->shape = shape;
}

void b2d_shape_loop(int id, bool adjstart, bool adjend)
{
  get_shape(b2dshape, id);
  b2ChainShape* shape;
  if (b2dshape->shapeBuilt) {
    if (b2dshape->shape->GetType() != b2Shape::e_chain) {
      delete b2dshape->shape;
      shape = new b2ChainShape();
    } else {
      shape = (b2ChainShape*)b2dshape->shape;
    }
  } else {
    b2dshape->shapeBuilt = true;
    shape = new b2ChainShape();
  }
  if (adjstart) {
    shape->SetPrevVertex(b2dshape->vertices[0]);
  }
  shape->CreateLoop(&b2dshape->vertices[adjstart],  b2dshape->vertices.size() - adjstart - adjend);
  if (adjend) {
    shape->SetNextVertex(b2dshape->vertices[b2dshape->vertices.size() - 1]);
  }
  b2dshape->type = b2d_loop;
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
