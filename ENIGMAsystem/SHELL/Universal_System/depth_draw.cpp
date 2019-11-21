/** Copyright (C) 2011 Josh Ventura
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

/// While the code that manages tiles and drawing is to be declared and managed
/// by the files under Graphics_Systems, this file exists to provide a way to
/// structure layers of depth, for both tiles and instances.

#include <math.h>
#include "depth_draw.h"

namespace enigma {
  depth_layer::depth_layer(): draw_events(new event_iter("Draw")), tilelist(-1) {}
  map<double,depth_layer> drawing_depths;
  map<int,pair<double,double> > id_to_currentnextdepth;
}
