/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#ifndef ENIGMA_SOUND_CHANNEL_H
#define ENIGMA_SOUND_CHANNEL_H
#include "../General/ASadvanced.h"
#include "XAsystem.h"

#include <vector>
using std::vector;

struct SoundChannel {
  unsigned source;
  int soundIndex;
  double priority;
  int type;
  SoundChannel(int alsource, int sound_id): source(alsource), soundIndex(sound_id) {}
  SoundChannel() {}

void sound_update()
{
  // NOTE: Use starttime, elapsedtime, and lasttime
  // calculate fade

  // calculate falloff
  switch (falloff_model)
  {
    case enigma_user::audio_falloff_exponent_distance:
      // gain = (listener_distance / reference_distance) ^ (-falloff_factor)
      break;
    case enigma_user::audio_falloff_exponent_distance_clamped:
      // distance = clamp(listener_distance, reference_distance, maximum_distance)
      // gain = (distance / reference_distance) ^ (-falloff_factor)
      break;
    case enigma_user::audio_falloff_inverse_distance:
      // gain = reference_distance / (reference_distance + falloff_factor * (listener_distance – reference_distance))
      break;
    case enigma_user::audio_falloff_inverse_distance_clamped:
      // distance = clamp(listener_distance, reference_distance, maximum_distance)
      // gain = reference_distance / (reference_distance + falloff_factor * (distance – reference_distance))
      break;
    case enigma_user::audio_falloff_linear_distance:
      // distance = min(distance, maximum_distance)
      // gain = (1 – falloff_factor * (distance – reference_distance) / (maximum_distance – reference_distance))
      break;
    case enigma_user::audio_falloff_linear_distance_clamped:
      // distance = clamp(listener_distance, reference_distance, maximum_distance)
      // gain = (1 – falloff_factor * (distance – reference_distance) / (maximum_distance – reference_distance))
      break;
    case enigma_user::audio_falloff_none:
      // gain = 1
      break;
    default:
      break;
  }
}

};

extern vector<SoundChannel*> sound_channels;

#endif
