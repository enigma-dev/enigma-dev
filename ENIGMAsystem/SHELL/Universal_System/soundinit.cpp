/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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

#include <string>
#include <stdio.h>
using namespace std;

#include "../Graphics_Systems/OpenGL/GSspriteadd.h"
#include "../Audio_Systems/audio_mandatory.h"
#include "../Platforms/platforms_mandatory.h"
#include "../libEGMstd.h"
#include "compression.h"

void sound_play(int sound);

namespace enigma
{
  void exe_loadsounds(FILE *exe)
  { 
    int nullhere;
    
    fread(&nullhere,4,1,exe);
    if (nullhere != *(int*)"sndn")
      return;
    
    // Determine how many sprites we have
    int sndcount;
    fread(&sndcount,4,1,exe);
    
    // Fetch the highest ID we will be using
    int snd_highid;
    fread(&snd_highid,4,1,exe);
    
    for (int i = 0; i < sndcount; i++)
    {
      int id;
      fread(&id,1,4,exe);
      
      unsigned size;
      fread(&size,1,4,exe);
      
      char fdata[size];
      fread(fdata,1,size,exe);
      
      printf("Read %d bytes; first several are %s",size, string(fdata,50).c_str());
      
      if (sound_add_from_buffer(id,fdata,size))
        printf("Failed to load sound%d\n",i);
    }
  }
}
