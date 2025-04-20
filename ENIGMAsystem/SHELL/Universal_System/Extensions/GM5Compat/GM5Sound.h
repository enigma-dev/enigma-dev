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


#ifndef ENIGMA_GM5COMPAT_SOUND_H
#define ENIGMA_GM5COMPAT_SOUND_H


///This file contains audio functions in the GM5 style.


#include "Universal_System/var4.h"
#include "Universal_System/scalar.h"

#include <string>

namespace enigma_user
{

//Add a sound (the other parameters are ignored).
int sound_add(std::string fName, int buffers, bool useEffects, bool loadOnUse);

//Modify the sound's frequency (approximated with pitch).
void sound_frequency(int sound, float value);

}

#endif // ENIGMA_GM5COMPAT_SOUND_H
