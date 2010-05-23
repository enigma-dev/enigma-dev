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

//#include "Preprocessor_Environment_Editable/IDE_EDIT_objectdata.h"
mtrandom_seed(enigma::Random_Seed=time(0));
#if BUILDMODE
	buildmode::buildinit();
#endif
graphicssystem_initialize();
#if ENIGMA_WS_WINDOWS!=0
	enigma::init_fonts();
#endif

enigma::instance_event_iterator = enigma::instance_list.begin();

//Clear the input arrays
for(int i=0;i<3;i++){
	enigma::last_mousestatus[i]=0;
	enigma::mousestatus[i]=0;
}
for(int i=0;i<256;i++){
	enigma::last_keybdstatus[i]=0;
	enigma::keybdstatus[i]=0;
}

//Take care of sprites;
enigma::sprite_idmax = 0;
enigma::exe_loadsprs();

//Load rooms
#include "Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h"
int instarpos=0;
for (enigma::roomiter=enigma::roomdata.begin();
     enigma::roomiter!=enigma::roomdata.end(); enigma::roomiter++){
	int troc=(*enigma::roomiter).second.instancecount;
	for(int i=0;i<troc;i++)
	{
    int idn = instdata[instarpos];
		(*enigma::roomiter).second.instances[i].id=idn;
		if (idn >= enigma::maxid) enigma::maxid=idn+1;
		(*enigma::roomiter).second.instances[i].obj=instdata[instarpos+1];
		(*enigma::roomiter).second.instances[i].x=instdata[instarpos+2];
		(*enigma::roomiter).second.instances[i].y=instdata[instarpos+3];
		instarpos+=4;
	}
}

//Go to the first room
room_goto_absolute(0);
/*
FILE* a=fopen("shit!.txt","wb");
if (a)
{
  fprintf(a,"Listing of all instances:\r\n");
  for (enigma::instance_iterator=enigma::instance_list.begin();
  enigma::instance_iterator!=enigma::instance_list.end();
  enigma::instance_iterator++)
  {
    fprintf(a,"IND: %d | ID: %d   | obj: %3d   | x: %3d   | y: %3d\r\n",
      (int)(*enigma::instance_iterator).first,
      (int)(*enigma::instance_iterator).second->id,
      (int)(*enigma::instance_iterator).second->object_index,
      (int)(*enigma::instance_iterator).second->x,
      (int)(*enigma::instance_iterator).second->y);
  }
  fclose(a);
}

system("shit!.txt");*/

