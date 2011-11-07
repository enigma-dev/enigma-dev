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

#include <string>
#include <stdio.h>
using namespace std;

#include "../Audio_Systems/audio_mandatory.h"
#include "../Platforms/platforms_mandatory.h"
#include "../libEGMstd.h"
#include "resinit.h"
#include "zlib.h"

void sound_play(int sound);

namespace enigma
{
  void sound_safety_override()
  {
    
  }
  
  void exe_loadsounds(FILE *exe)
  { 
    int nullhere;
    
    if (!fread(&nullhere,4,1,exe)) return;
    if (nullhere != *(int*)"SND ")
      return;
    
    // Determine how many sprites we have
    int sndcount;
    if (!fread(&sndcount,4,1,exe)) return;
    
    // Fetch the highest ID we will be using
    int snd_highid;
    if (!fread(&snd_highid,4,1,exe)) return;
    
    for (int i = 0; i < sndcount; i++)
    {
      int id;
      if (!fread(&id,1,4,exe)) return;
      
      unsigned size;
      if (!fread(&size,1,4,exe)) return;
      
      char* fdata = new char[size];
      if (!fread(fdata,1,size,exe)) return;
      
      if (sound_add_from_buffer(id,fdata,size))
        printf("Failed to load sound%d\n",i);
      delete fdata;
    }
  }
}
