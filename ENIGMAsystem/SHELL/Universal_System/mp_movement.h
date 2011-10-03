/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Polygone                                                 **
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
#include "collisions_object.h"
//#include "instance_system.h"
#include <cmath>

void mp_potential_settings(double maxrot, double rotstep, double ahead, double onspot);
bool mp_potential_step_object(const double x, const double y, const double stepsize, const int object, const bool solid_only = false);

inline bool mp_potential_step(const double x, const double y, const double stepsize, const bool checkall) {
    return mp_potential_step_object(x, y, stepsize, all, !checkall);
}

bool mp_potential_path_object(int path, const double x, const double y, const double stepsize, double factor, const int object, const bool solid_only = false);

inline bool mp_potential_path(int path, const double x, const double y, const double stepsize, double factor, const bool checkall, const bool solid_only = false) {
    return mp_potential_path_object(path, x, y, stepsize, factor, all, checkall ? false : true);
}

bool mp_linear_step_object(const double x, const double y, const double stepsize, const int object, const bool solid_only = false);

inline bool mp_linear_step(const double x, const double y, const double stepsize, const bool checkall) {
    return mp_linear_step_object(x, y, stepsize, all, !checkall);
}

bool mp_linear_path_object(int path, const double x, const double y, const double stepsize, const int object, const bool solid_only = false);

inline bool mp_linear_path(int path, const double x, const double y, const double stepsize, const bool checkall) {
    return mp_linear_path_object(path, x, y, stepsize, all, !checkall);
}
