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

#include "Universal_System/Object_Tiers/collisions_object.h"

enigma::object_collisions* const collide_inst_inst(int object, bool solid_only, bool notme, double x, double y);
enigma::object_collisions* const collide_inst_rect(int object, bool solid_only, bool notme, int x1, int y1, int x2, int y2);
enigma::object_collisions* const collide_inst_line(int object, bool solid_only, bool notme, int x1, int y1, int x2, int y2);
enigma::object_collisions* const collide_inst_point(int object, bool solid_only, bool notme, int x1, int y1);
enigma::object_collisions* const collide_inst_circle(int object, bool solid_only, bool notme, int x1, int y1, double r);
enigma::object_collisions* const collide_inst_ellipse(int object, bool solid_only, bool notme, int x1, int y1, double rx, double ry);
void destroy_inst_point(int object, bool solid_only, int x1, int y1);
