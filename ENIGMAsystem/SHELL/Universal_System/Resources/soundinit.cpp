/** Copyright (C) 2010-2011 Josh Ventura
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

#include "Audio_Systems/audio_mandatory.h"
#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "libEGMstd.h"
#include "resinit.h"
#include "Universal_System/zlib.h"
#include "Platforms/General/fileio.h"

#include <cstring>

namespace enigma_user {
  void sound_play(int sound);
}

namespace enigma
{
  void sound_safety_override()
  {

  }

  void exe_loadsounds(FILE_t *exe)
  {
    int nullhere;

    if (!fread_wrapper(&nullhere,4,1,exe)) return;
    if (memcmp(&nullhere, "SND ", sizeof(int)) != 0)
      return;

    // Determine how many sprites we have
    int sndcount;
    if (!fread_wrapper(&sndcount,4,1,exe)) return;

    // Fetch the highest ID we will be using
    int snd_highid;
    if (!fread_wrapper(&snd_highid,4,1,exe)) return;

    for (int i = 0; i < sndcount; i++)
    {
      int id;
      if (!fread_wrapper(&id,1,4,exe)) return;

      unsigned size;
      if (!fread_wrapper(&size,1,4,exe)) return;

      char* fdata = new char[size];
      if (!fread_wrapper(fdata,1,size,exe)) return;

      int e = sound_add_from_buffer(id,fdata,size);
      if (e) DEBUG_MESSAGE("Failed to load sound " + std::to_string(i) + " error " + std::to_string(e), MESSAGE_TYPE::M_ERROR);
      delete[] fdata;
    }
  }
}
