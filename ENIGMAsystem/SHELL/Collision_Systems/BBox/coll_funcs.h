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

bool place_free(double x,double y);
bool place_empty(double x,double y);
bool place_meeting(double x, double y, int object);
bool position_free(double x,double y);
bool position_empty(double x, double y);
bool position_meeting(double x, double y, int object);
void position_destroy_object(double x, double y, int object, bool solid_only = false);
void position_destroy_solid(double x, double y);
void position_destroy(double x, double y);
int instance_place(double x, double y, int object);
int instance_position(double x, double y, int object);
int collision_rectangle(double x1, double y1, double x2, double y2, int obj, bool prec, bool notme);
int collision_line(double x1, double y1, double x2, double y2, int obj, bool prec, bool notme);
int collision_point(double x, double y, int obj, bool prec, bool notme);
int collision_circle(double x, double y, double radius, int obj, bool prec, bool notme);
int collision_ellipse(double x1, double y1, double x2, double y2, int obj, bool prec, bool notme);

double distance_to_object(int object);
double distance_to_point(double x, double y);
