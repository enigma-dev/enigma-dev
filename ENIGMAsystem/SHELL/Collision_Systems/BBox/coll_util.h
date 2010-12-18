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

bool collide_rect_line(double rx1, double ry1, double rx2, double ry2,
                       double px1, double py1, double px2, double py2);
bool collide_rect_rect(double r1x1, double r1y1, double r1x2, double r1y2,
                       double r2x1, double r2y1, double r2x2, double r2y2);
bool collide_rect_point(double rx1, double ry1, double rx2, double ry2, double px, double py);

bool collide_bbox_line(const enigma::object_collisions* inst, double ox, double oy, double x1, double y1, double x2, double y2);
bool collide_bbox_bbox(const enigma::object_collisions* inst1, double ox1, double oy1, const enigma::object_collisions* inst2, double ox2, double oy2);
bool collide_bbox_point(const enigma::object_collisions* inst, double ox, double oy, double x, double y);

