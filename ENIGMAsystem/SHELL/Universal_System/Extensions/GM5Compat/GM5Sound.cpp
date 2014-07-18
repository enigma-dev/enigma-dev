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

#include "GM5Sound.h"

#include "Graphics_Systems/General/GSprimitives.h"
#include "Audio_Systems/General/ASbasic.h"


namespace enigma_user
{

//buffers states how many buffers to use to store the sound. If it's set to, e.g., 5, then the sound can only be played 5 times simultaneously. We ignore it.
//useEffects allows sound_volume, sound_pan, and sound_frequency to be used. We assume it is always on.
//loadOnUse means whether the sound "should be stored in internal memory". We should (I think) assume it is always on.
int sound_add(string fName, int buffers, bool useEffects, bool loadOnUse)
{
  //We just dispatch to the normal sound_add function. 0="normal" sound.
  return sound_add(fName, 0, true);
}

void sound_frequency(int sound, float value)
{
  //This is a rough approximation, but is seems consistent with GM5.
  sound_pitch(sound, value*10);
}



}

