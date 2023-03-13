/** Copyright (C) 2008-2013 IsmAvatar <ismavatar@gmail.com>
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

#include "Universal_System/scalar.h"

#ifndef COLL_UTIL_H
#define COLL_UTIL_H

bool collide_rect_line(cs_scalar rx1, cs_scalar ry1, cs_scalar rx2, cs_scalar ry2,
                       cs_scalar px1, cs_scalar py1, cs_scalar px2, cs_scalar py2);
bool collide_rect_rect(cs_scalar r1x1, cs_scalar r1y1, cs_scalar r1x2, cs_scalar r1y2,
                       cs_scalar r2x1, cs_scalar r2y1, cs_scalar r2x2, cs_scalar r2y2);
bool collide_rect_point(cs_scalar rx1, cs_scalar ry1, cs_scalar rx2, cs_scalar ry2, cs_scalar px, cs_scalar py);

#include "Universal_System/Object_Tiers/collisions_object.h"

bool collide_bbox_rect(const enigma::object_collisions* inst, cs_scalar ox, cs_scalar oy, cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2);
bool collide_bbox_line(const enigma::object_collisions* inst, cs_scalar ox, cs_scalar oy, cs_scalar x1, cs_scalar y1, cs_scalar x2, cs_scalar y2);
bool collide_bbox_bbox(const enigma::object_collisions* inst1, cs_scalar ox1, cs_scalar oy1, const enigma::object_collisions* inst2, cs_scalar ox2, cs_scalar oy2);
bool collide_bbox_point(const enigma::object_collisions* inst, cs_scalar ox, cs_scalar oy, cs_scalar x, cs_scalar y);
bool collide_bbox_circle(const enigma::object_collisions* inst, cs_scalar ox, cs_scalar oy, cs_scalar x, cs_scalar y, double r);
bool collide_bbox_ellipse(const enigma::object_collisions* inst, cs_scalar ox, cs_scalar oy, cs_scalar x, cs_scalar y, cs_scalar rx, cs_scalar ry);
void destroy_bbox_point(const enigma::object_collisions* inst, cs_scalar ox, cs_scalar oy, cs_scalar x, cs_scalar y);

#endif
