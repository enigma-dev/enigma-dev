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

// NOTES:
// 1) box2d uses an inversed y-axis, thus why b2d_body_get_angle() returns -radianstodegrees(angle)
// 2) static objects when hit appear to me to move about one pixel at times, I can't tell if this is just my eyes playin tricks
// 3) leave the option to continue allowing you to manually update your world
// 4) box2d's manual also states you should blend previous timesteps for updating the world with the current timestep
//    in order to make the simulation run smoother
// 5) box2d manual is available here... http://www.box2d.org/manual.html
// 6) I made joints bind fixtures, where as studio's joints bind instances together, that's stupid, fuck that
// 7) box2d's classes allow you to set a b2Shape for instance to a b2CircleShape or b2PolygonShape
//    that is why I wrote the classes to use an abstracted pointer reference such as b2Shape and b2Joint

#include "B2Dfunctions.h"
#include "B2Djoints.h"
#include "B2Dshapes.h"
