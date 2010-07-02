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

int compile_writeRoomData(EnigmaStruct* es)
{
  ofstream wto("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h",ios_base::out);
  
  wto << license;
  int room_highid = 0, room_highinstid = 100000;
  for (int i = 0; i < es->roomCount; i++) 
  {
    wto << "//Room " << es->rooms[i].id << "\n" <<
    "  enigma::roomdata[" << es->rooms[i].id << "].name = \"" << es->rooms[i].name << "\";\n"
    "  enigma::roomdata[" << es->rooms[i].id << "].cap = \"" << es->rooms[i].caption << "\";\n"
    "  enigma::roomdata[" << es->rooms[i].id << "].backcolor = " << lgmRoomBGColor(es->rooms[i].backgroundColor) << ";\n"
    "  enigma::roomdata[" << es->rooms[i].id << "].spd = " << es->rooms[i].speed << ";\n"
    "  enigma::roomdata[" << es->rooms[i].id << "].width = " << es->rooms[i].width << ";\n"
    "  enigma::roomdata[" << es->rooms[i].id << "].height = " << es->rooms[i].height << ";\n"
    "  enigma::roomdata[" << es->rooms[i].id << "].views_enabled = " << es->rooms[i].enableViews << ";\n";
    for (int ii = 0; ii < es->rooms[i].viewCount; ii++)
    {
      wto << 
      "    enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].start_vis = 0;\n"
      "    enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].area_x = 0; enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].area_y = 0;"
         " enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].area_w = 640; enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].area_h = 480;\n"
      "    enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].port_x = 0;   enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].port_y = 0;"
         " enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].port_w = 640; enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].port_h = 480;\n"
      "    enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].object2follow = 0;\n"
      "    enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].hborder=32; enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].vborder = 32;"
         " enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].hspd = -1;  enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].vspd = -1;\n";
    }
    wto << 
    "  enigma::roomdata[" << es->rooms[i].id << "].instancecount = " << es->rooms[i].instanceCount << ";\n"
    "  enigma::roomdata[" << es->rooms[i].id << "].createcode = (void(*)())roomcreate" << es->rooms[i].id << ";";
    
    if (es->rooms[i].id > room_highid)
      room_highid = es->rooms[i].id;
  }
  
  wto << "int instdata[] = {";
  for (int i = 0; i < es->roomCount; i++)
    for (int ii = 0; ii < es->rooms[i].instanceCount; ii++) {
      wto << 
        es->rooms[i].instances[ii].id << "," << 
        es->rooms[i].instances[ii].objectId << "," << 
        es->rooms[i].instances[ii].x << "," << 
        es->rooms[i].instances[ii].y << ",";
      if (es->rooms[i].instances[ii].id > room_highinstid)
        room_highinstid = es->rooms[i].instances[ii].id;
    }
  wto << "};\n\n";
  wto << "enigma::room_max = " <<  room_highid << " + 1;\nenigma::maxid = " << room_highinstid << " + 1;\n";
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
    wto << parser_main(cme);
    wto << "\n}\n";
  }
  
  wto.close();
  return 0;
}
