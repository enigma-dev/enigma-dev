/********************************************************************************\
**                                                                              **
**  Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>                          **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "coll_util.h"

////////////////////////////////////
// utility intersection functions - test shape intersection, which is largely API independant
////////////////////////////////////

//because including math and stdlib don't find min/max for some unknown reason...
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

////////////////////////////////////
//rect functions - test if a rectangle and another shape intersect
////////////////////////////////////

bool collide_rect_line(double rx1, double ry1, double rx2, double ry2,
                       double px1, double py1, double px2, double py2)
{
  // Find the intersection of the segment's and rectangle's x-projections
  double minX = max(min(px1,px2),rx1);
  double maxX = min(max(px1,px2),rx2);
  if (minX > maxX)
    return false;

  // Find corresponding min and max Y for min and max X we found before
  double minY = py1;
  double maxY = py2;
  double dx = px2 - px1;

  //do slope check of non vertical lines (dx != 0)
  if ((float)dx)
  {
    double a = (py2 - py1) / dx;
    double b = py1 - a * px1;
    minY = a * minX + b;
    maxY = a * maxX + b;
  }

  if (minY > maxY) //swap
  {
    double tmp = maxY;
    maxY = minY;
    minY = tmp;
  }

  // Find the intersection of the segment's and rectangle's y-projections
  if (maxY > ry2)
    maxY = ry2;
  if (minY < ry1)
    minY = ry1;

  return (minY <= maxY); // If Y-projections do not intersect return false
}

//unused
bool collide_rect_rect(double r1x1, double r1y1, double r1x2, double r1y2,
                       double r2x1, double r2y1, double r2x2, double r2y2)
{
  return (r1x1 < r2x2 && r1x2 > r2x1 && r1y1 < r2y2 && r1y2 > r2y1);
}

//unused
bool collide_rect_point(double rx1, double ry1, double rx2, double ry2, double px, double py)
{
  return (px < rx2 && px > rx1 && py < ry2 && py > ry1);
}


////////////////////////////////////
// bbox functions - tests if an instance's bbox placed at a position will collide with something
////////////////////////////////////

#include "../../Universal_System/collisions_object.h"

bool collide_bbox_rect(const enigma::object_collisions* inst, double ox, double oy, double x1, double y1, double x2, double y2)
{
  const bbox_rect_t &box = inst->$bbox_relative();
  return collide_rect_rect(box.left + ox,
                           box.top + oy,
                           box.right + ox,
                           box.bottom + oy,
                           x1,y1,x2,y2);
}

bool collide_bbox_line(const enigma::object_collisions* inst, double ox, double oy, double x1, double y1, double x2, double y2)
{
  const bbox_rect_t &box = inst->$bbox_relative();
  return collide_rect_line(box.left + ox,
                           box.top + oy,
                           box.right + ox,
                           box.bottom + oy,
                           x1,y1,x2,y2);
}

bool collide_bbox_bbox(const enigma::object_collisions* inst1, double ox1, double oy1, const enigma::object_collisions* inst2, double ox2, double oy2)
{
  const bbox_rect_t &box1 = inst1->$bbox_relative();
  const bbox_rect_t &box2 = inst2->$bbox_relative();
  return (box1.left + ox1 < box2.right + ox2
       && box2.left + ox2 < box1.right + ox1
       && box1.top + oy1  < box2.bottom + oy2
       && box2.top + oy2  < box1.bottom + oy1);
}

bool collide_bbox_point(const enigma::object_collisions* inst, double ox, double oy, double x, double y)
{
  const bbox_rect_t &box = inst->$bbox_relative();
  return (x < box.right + ox
       && x > box.left + ox
       && y < box.bottom + oy
       && y > box.top + oy);
}
