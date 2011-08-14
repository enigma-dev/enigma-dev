/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Harijs Grînbergs                                         **
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
bool path_start(unsigned pathid,double speed,unsigned endaction,bool absolute);
bool path_exists(unsigned pathid);
void path_delete(unsigned pathid);
void path_assign(unsigned pathid,unsigned path);
void path_append(unsigned pathid,unsigned path);
int path_add();
int path_duplicate(unsigned pathid);
int path_get_number(unsigned pathid);
double path_get_point_x(unsigned pathid, unsigned n);
double path_get_point_y(unsigned pathid, unsigned n);
double path_get_point_length(unsigned pathid, unsigned n);
double path_get_point_speed(unsigned pathid, unsigned n);
double path_get_center_x(unsigned pathid);
double path_get_center_y(unsigned pathid);
int path_get_precision(unsigned pathid);
double path_get_length(unsigned pathid);
bool path_get_kind(unsigned pathid);
bool path_get_closed(unsigned pathid);
void path_set_kind(unsigned pathid, bool val);
void path_set_closed(unsigned pathid, bool val);
void path_set_precision(unsigned pathid, int prec);
void path_clear_points(unsigned pathid);
void path_add_point(unsigned pathid,double x,double y,double speed);
void path_insert_point(unsigned pathid,unsigned n,double x,double y,double speed);
void path_change_point(unsigned pathid,unsigned n,double x,double y,double speed);
void path_reverse(unsigned pathid);
void path_shift(unsigned pathid,double xshift,double yshift);
void path_flip(unsigned pathid);
void path_mirror(unsigned pathid);
void path_scale(unsigned pathid,double xscale,double yscale);
void path_rotate(unsigned pathid,double angle);
double path_get_x(unsigned pathid, double t);
double path_get_y(unsigned pathid, double t);
double path_get_speed(unsigned pathid, double t);
double path_get_direction(unsigned pathid, double t);
void draw_path(unsigned path,double x,double y,bool absolute);
