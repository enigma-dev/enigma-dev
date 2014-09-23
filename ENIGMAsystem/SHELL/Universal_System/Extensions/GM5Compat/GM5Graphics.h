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


#ifndef ENIGMA_GM5COMPAT_GRAPHICS__H
#define ENIGMA_GM5COMPAT_GRAPHICS__H


///This file contains graphics-related functionality for GM5.


namespace enigma_user
{

//"exclusive" mode is ignored
void set_graphics_mode(bool exclusive, int horres, int coldepth, int freq, bool fullscreen, int winscale, int fullscale);

}

#endif // ENIGMA_GM5COMPAT_GRAPHICS__H

