/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
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

#include "Universal_System/instance_system_base.h"

extern bool argument_relative;

inline void action_linear_step(double x, double y, double stepsize, bool solid_only)
{
    if (argument_relative)
    {
        mp_linear_step_object(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+x, ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+y, stepsize,all, solid_only);
    }
    else
    {
        mp_linear_step_object(x, y, stepsize,all, solid_only);
	}
}

inline void action_potential_step(double x, double y, double stepsize, bool solid_only)
{
    if (argument_relative)
    {
        mp_potential_step_object(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+x, ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+y, stepsize,all, solid_only);
    }
    else
    {
        mp_potential_step_object(x, y, stepsize,all, solid_only);
	}
}
