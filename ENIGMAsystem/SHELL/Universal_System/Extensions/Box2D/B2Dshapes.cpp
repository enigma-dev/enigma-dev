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

int b2d_shape_create()
{
  int i = b2dshapes.size();
  B2DShape* b2dshape = new B2DShape();
  b2dshapes.push_back(b2dshape);
  return i;
}

void b2d_shape_delete(int id)
{
  if (unsigned(id) >= b2dshapes.size() || id < 0)
  {
    return;
  }
  else
  {
    delete b2dshapes[id];
  }
}

void b2d_shape_box(int id, double halfwidth, double halfheight)
{
  if (unsigned(id) >= b2dshapes.size() || id < 0)
  {
    return;
  }
  else
  {
    b2PolygonShape* shape = new b2PolygonShape();
    shape->SetAsBox(halfwidth, halfheight);
    b2dshapes[id]->shape = shape;
  }
}

void b2d_shape_circle(int id, double radius)
{
  if (unsigned(id) >= b2dshapes.size() || id < 0)
  {
    return;
  }
  else
  {
    b2CircleShape* shape = new b2CircleShape();
    shape->m_radius = radius;
    b2dshapes[id]->shape = shape;
  }
}

void b2d_shape_add_point(int id, double x, double y)
{
  if (unsigned(id) >= b2dshapes.size() || id < 0)
  {
    return;
  }
  else
  {
    b2dshapes[id]->vertices.push_back(b2Vec2(x, y));
  }
}

void b2d_shape_test_point(int id, double sx, double sy, double sa, double px, double py)
{
  if (unsigned(id) >= b2dshapes.size() || id < 0)
  {
    return;
  }
  else
  {
    b2dshapes[id]->shape->TestPoint(b2Transform(b2Vec2(sx, sy), b2Rot(sa)), b2Vec2(px, py));
  }
}

void b2d_shape_polygon(int id)
{
  if (unsigned(id) >= b2dshapes.size() || id < 0)
  {
    return;
  }
  else
  {
    b2PolygonShape* shape = new b2PolygonShape();

    shape->Set(&b2dshapes[id]->vertices[0],  b2dshapes[id]->vertices.size());
    b2dshapes[id]->shape = shape;
  }
}

void b2d_shape_edge(int id)
{
  if (unsigned(id) >= b2dshapes.size() || id < 0)
  {
    return;
  }
  else
  {
    b2EdgeShape* shape = new b2EdgeShape();

    //shape->Set(&b2dshapes[id]->vertices[0],  &b2dshapes[id]->vertices[b2dshapes[id]->vertices.size()]);
    b2dshapes[id]->shape = shape;
  }
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
  if (unsigned(id) >= b2dfixtures.size() || id < 0)
  {
    return;
  }
  else
  {
    delete b2dfixtures[id];
  }
}

void b2d_fixture_set_collision_group(int id, int group)
{
  if (unsigned(id) >= b2dfixtures.size() || id < 0)
  {
    return;
  }
  else
  {
    // sets the collision group used to make parts of things not collide, like a ragdoll for
    // instance should not collide with itself
    b2Filter newfilter;
    newfilter.groupIndex = group;
    b2dfixtures[id]->fixture->SetFilterData(newfilter);
  }
}

int b2d_fixture_get_shape(int id)
{
  if (unsigned(id) >= b2dfixtures.size() || id < 0)
  {
    return -1;
  }
  else
  {
    return b2dfixtures[id]->shapeid;
  }
}

int b2d_fixture_get_body(int id)
{
  if (unsigned(id) >= b2dfixtures.size() || id < 0)
  {
    return -1;
  }
  else
  {
    return b2dfixtures[id]->bodyid;
  }
}

void b2d_fixture_set_density(int id, double density)
{
  if (unsigned(id) >= b2dfixtures.size() || id < 0)
  {
    return;
  }
  else
  {
    b2dfixtures[id]->fixture->SetDensity(density);
  }
}

void b2d_fixture_set_friction(int id, double friction)
{
  if (unsigned(id) >= b2dfixtures.size() || id < 0)
  {
    return;
  }
  else
  {
    b2dfixtures[id]->fixture->SetFriction(friction);
  }
}

void b2d_fixture_set_restitution(int id, double restitution)
{
  if (unsigned(id) >= b2dfixtures.size() || id < 0)
  {
    return;
  }
  else
  {
    b2dfixtures[id]->fixture->SetRestitution(restitution);
  }
}

void b2d_fixture_set_sensor(int id, bool state)
{
  if (unsigned(id) >= b2dfixtures.size() || id < 0)
  {
    return;
  }
  else
  {
    b2dfixtures[id]->fixture->SetSensor(state);
  }
}
