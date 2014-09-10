// 
// Copyright (C) 2014 Seth N. Hetu
// 
// This file is a part of the ENIGMA Development Environment.
// 
// ENIGMA is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, version 3 of the license or any later version.
// 
// This application and its source code is distributed AS-IS, WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with this code. If not, see <http://www.gnu.org/licenses/>
//

#include "GM5Graphics.h"

#include "Platforms/platforms_mandatory.h"

namespace enigma_user
{

void set_graphics_mode(bool exclusive, int horres, int coldepth, int freq, bool fullscreen, int winscale, int fullscale)
{
  //Exclusive mode seems to have died with GM5.
  //The horizontal resolution can be 320, 640, 800, 1024, 1280, 1600; or 0 not to change it. For now we ignore it.
  //The color depth can be 16 or 32 (or probably 0 to not change); for now, we ignore it.
  //The frequency can be 60, 70, 75, 80, 100, 120 (or probably 0 to not change); for now we ignore it.
  //Winscale and fullscale are the scaling factors (1 = no scaling; 0 *might* mean stretch-to-fit) for windowing and full-screen mode. We ignore both for now.

  //Fullscreen determines whether or not we should be running in full-screen mode.
  window_set_fullscreen(fullscreen);
}


}

