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

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "../../externs/externs.h"
#include "../../syntax/syncheck.h"
#include "../../parser/parser.h"

#include "../../backend/EnigmaStruct.h" //LateralGM interface structures
#include "../../parser/object_storage.h"
#include "../compile_common.h"

#include <math.h> //log2 to calculate passes.

#define flushl '\n' << flush
#define flushs flush

int compile_writeRoomData(EnigmaStruct* es, parsed_object *EGMglobal)
{
  ofstream wto("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h",ios_base::out);

  wto << license << "namespace enigma {\n"
  << "  int room_loadtimecount = " << es->roomCount << ";\n"
  << "  roomstruct grd_rooms[" << es->roomCount << "] = {\n";
  int room_highid = 0, room_highinstid = 100000;
  for (int i = 0; i < es->roomCount; i++)
  {
    wto << "    //Room " << es->rooms[i].id << "\n" <<
    "    { "
    << es->rooms[i].id << ", \"" // The ID of this room
    << es->rooms[i].name << "\",  \""  // The name of this room
    << es->rooms[i].caption << "\",\n      " // The caption of this room

    << lgmRoomBGColor(es->rooms[i].backgroundColor) //Background color
    << ", roomcreate" << es->rooms[i].id << ",\n      " // Creation code

    << es->rooms[i].width << ", " << es->rooms[i].height << ", " // Width and Height
    << es->rooms[i].speed << ",  "  // Speed

    << (es->rooms[i].enableViews ? "true" : "false") << ", {\n"; // Views Enabled

    for (int ii = 0; ii < es->rooms[i].viewCount; ii++) // For each view
    {
      wto << "      { "
      << es->rooms[i].views[ii].visible << ",   " // Visible

      << es->rooms[i].views[ii].viewX << ", " << es->rooms[i].views[ii].viewY << ", "   // Xview and Yview
      << es->rooms[i].views[ii].viewW << ", " << es->rooms[i].views[ii].viewH << ",   " // Wview and Hview

      << es->rooms[i].views[ii].portX << ", " << es->rooms[i].views[ii].portY << ", "   // Xport and Yport
      << es->rooms[i].views[ii].portW << ", " << es->rooms[i].views[ii].portH << ",   " // Wport and Hport

      << es->rooms[i].views[ii].objectId << ",   " // Object2Follow

      << es->rooms[i].views[ii].borderH << ", " << es->rooms[i].views[ii].borderV << ",   " //Hborder and Vborder
      << es->rooms[i].views[ii].speedH<< ", " << es->rooms[i].views[ii].speedV //Hborder and Vborder

      << " },\n";
    }
    //Start of Backgrounds
    wto << "}, {";
     for (int ii = 0; ii < es->rooms[i].backgroundDefCount; ii++) // For each background
     {
        wto << "      { "
        << (es->rooms[i].backgroundDefs[ii].visible ? "true" : "false") << ",   " // Visible
        << (es->rooms[i].backgroundDefs[ii].foreground ? "true" : "false") << ",   " // Foreground
        << es->rooms[i].backgroundDefs[ii].backgroundId << ",   " // Background
        << es->rooms[i].backgroundDefs[ii].x << ",   " // X
        << es->rooms[i].backgroundDefs[ii].y << ",   " // Y
        << es->rooms[i].backgroundDefs[ii].hSpeed << ",   " // HSpeed
        << es->rooms[i].backgroundDefs[ii].vSpeed << ",   " // VSpeed
        << (es->rooms[i].backgroundDefs[ii].tileHoriz ? "true" : "false") << ",   " // tileHor
        << (es->rooms[i].backgroundDefs[ii].tileVert ? "true" : "false") << ",   " // tileVert
        << (es->rooms[i].backgroundDefs[ii].stretch ? "true" : "false") // Stretch
        << " },\n";
     }
    wto <<
    "      },"; //End of Backgrounds
    wto <<
    "      " << es->rooms[i].instanceCount << ", "
    "      (enigma::inst*)(int[]){";
      int modme = 0;
      for (int ii = 0; ii < es->rooms[i].instanceCount; ii++) {
        wto <<
          es->rooms[i].instances[ii].id << "," <<
          es->rooms[i].instances[ii].objectId << "," <<
          es->rooms[i].instances[ii].x << "," <<
          es->rooms[i].instances[ii].y << ",";
          if (++modme % 16 == 0) wto << "\n        ";
        if (es->rooms[i].instances[ii].id > room_highinstid)
          room_highinstid = es->rooms[i].instances[ii].id;
      }
    wto << "  0,0,0,0}\n"; // End of the instances

    // End of this room
    wto << "    },\n";

    if (es->rooms[i].id > room_highid)
      room_highid = es->rooms[i].id;
  }

  wto << "  };\n  \n"; // End of all rooms
  wto << "  int room_max = " <<  room_highid << " + 1;\n  int maxid = " << room_highinstid << " + 1;\n";

  wto << "} // Namespace enigma\n";
wto.close();


wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomcreates.h",ios_base::out);
  wto << license;
  for (int i = 0; i < es->roomCount; i++)
  {
    wto << "void roomcreate" << es->rooms[i].id << "()\n{\n  ";
    string cme = es->rooms[i].creationCode;
    int a = syncheck::syntacheck(cme);
    if (a != -1) {
      cout << "Syntax error in room creation code for room " << es->rooms[i].id << " (`" << es->rooms[i].name << "'):" << endl << syncheck::error << flushl;
      return E_ERROR_SYNTAX;
    }
    parsed_object par;
    parsed_event ev(&par);
    parser_main(cme,&ev);
    parser_secondary(ev.code,ev.synt,EGMglobal,&par);
    print_to_file(ev.code,ev.synt,ev.strc,ev.strs,2,wto);
    wto << "\n}\n";
  }

  wto.close();
  return 0;
}
