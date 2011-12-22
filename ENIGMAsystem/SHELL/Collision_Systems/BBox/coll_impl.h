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

#include "../../Universal_System/collisions_object.h"

enigma::object_collisions* const collide_inst_rect(int object, bool solid_only, bool notme, double x1, double y1, double x2, double y2);
enigma::object_collisions* const collide_inst_line(int object, bool solid_only, bool notme, double x1, double y1, double x2, double y2);
enigma::object_collisions* const collide_inst_inst(int object, bool solid_only, bool notme, double x, double y);
enigma::object_collisions* const collide_inst_point(int object, bool solid_only, bool notme, double x, double y);
enigma::object_collisions* const collide_inst_circle(int object, bool solid_only, bool notme, double x, double y, double r);
enigma::object_collisions* const collide_inst_ellipse(int object, bool solid_only, bool notme, double x, double y, double rx, double ry);
void destroy_inst_point(int object, bool solid_only, double x, double y);
