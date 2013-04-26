/**
  @file  write_room_data.cpp
  @brief Implements the method in the C++ plugin that writes rooms as plain code
         to the engine.
  
  @section License
    Copyright (C) 2008-2013 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;


#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "parser/object_storage.h"
#include "compiler/compile_common.h"

#include <math.h> //log2 to calculate passes.

#define flushl '\n' << flush
#define flushs flush

#include "languages/lang_CPP.h"

int lang_CPP::compile_writeRoomData(compile_context &ctex)
{
  ofstream wto("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h",ios_base::out);

  wto << gen_license << "namespace enigma {\n"
  << "  int room_loadtimecount = " << ctex.es->roomCount << ";\n";
  int room_highid = 0, room_highinstid = 100000,room_hightileid=10000000;

  for (int i = 0; i < ctex.es->roomCount; i++)
  {
    wto << "  tile tiles_" << ctex.es->rooms[i].id << "[] = {\n";
    for (int ii = 0, modme = 0; ii < ctex.es->rooms[i].tileCount; ii++)
    {
      wto << "{" <<
        ctex.es->rooms[i].tiles[ii].id << "," <<
        ctex.es->rooms[i].tiles[ii].backgroundId << "," <<
        ctex.es->rooms[i].tiles[ii].bgX << "," <<
        ctex.es->rooms[i].tiles[ii].bgY << "," <<
        ctex.es->rooms[i].tiles[ii].depth << "," <<
        ctex.es->rooms[i].tiles[ii].height << "," <<
        ctex.es->rooms[i].tiles[ii].width << "," <<
        ctex.es->rooms[i].tiles[ii].roomX << "," <<
        ctex.es->rooms[i].tiles[ii].roomY << "},";
        if (++modme % 16 == 0) wto << "\n        ";
      if (ctex.es->rooms[i].tiles[ii].id > room_hightileid)
        room_hightileid = ctex.es->rooms[i].tiles[ii].id;
    }
    wto << "  };\n";
  }

  for (int i = 0; i < ctex.es->roomCount; i++)
  {
    wto << "  inst insts_" << ctex.es->rooms[i].id << "[] = {\n";
    for (int ii = 0, modme = 0; ii < ctex.es->rooms[i].instanceCount; ii++)
    {
      wto << "{" <<
        ctex.es->rooms[i].instances[ii].id << "," <<
        ctex.es->rooms[i].instances[ii].objectId << "," <<
        ctex.es->rooms[i].instances[ii].x << "," <<
        ctex.es->rooms[i].instances[ii].y << "},";
        if (++modme % 16 == 0) wto << "\n        ";
      if (ctex.es->rooms[i].instances[ii].id > room_highinstid)
        room_highinstid = ctex.es->rooms[i].instances[ii].id;
    }
    wto << "  };\n";
  }

  wto << "  roomstruct grd_rooms[" << ctex.es->roomCount << "] = {\n";
  for (int i = 0; i < ctex.es->roomCount; i++)
  {
    wto << "    //Room " << ctex.es->rooms[i].id << "\n" <<
    "    { "
    << ctex.es->rooms[i].id << ", " // The ID of this room
    << i << ", \"" // The tree order index of this room
    << ctex.es->rooms[i].name << "\",  \""  // The name of this room
    << ctex.es->rooms[i].caption << "\",\n      " // The caption of this room

    << lgmRoomBGColor(ctex.es->rooms[i].backgroundColor) << ", "//Background color
    << ctex.es->rooms[i].drawBackgroundColor
    << ", roomcreate" << ctex.es->rooms[i].id << ",\n      " // Creation code

    << ctex.es->rooms[i].width << ", " << ctex.es->rooms[i].height << ", " // Width and Height
    << ctex.es->rooms[i].speed << ",  "  // Speed

    << (ctex.es->rooms[i].enableViews ? "true" : "false") << ", {\n"; // Views Enabled

    for (int ii = 0; ii < ctex.es->rooms[i].viewCount; ii++) // For each view
    {
      wto << "      { "
      << ctex.es->rooms[i].views[ii].visible << ",   " // Visible

      << ctex.es->rooms[i].views[ii].viewX << ", " << ctex.es->rooms[i].views[ii].viewY << ", "   // Xview and Yview
      << ctex.es->rooms[i].views[ii].viewW << ", " << ctex.es->rooms[i].views[ii].viewH << ",   " // Wview and Hview

      << ctex.es->rooms[i].views[ii].portX << ", " << ctex.es->rooms[i].views[ii].portY << ", "   // Xport and Yport
      << ctex.es->rooms[i].views[ii].portW << ", " << ctex.es->rooms[i].views[ii].portH << ",   " // Wport and Hport

      << ctex.es->rooms[i].views[ii].objectId << ",   " // Object2Follow

      << ctex.es->rooms[i].views[ii].borderH << ", " << ctex.es->rooms[i].views[ii].borderV << ",   " //Hborder and Vborder
      << ctex.es->rooms[i].views[ii].speedH<< ", " << ctex.es->rooms[i].views[ii].speedV //Hborder and Vborder

      << " },\n";
    }
    //Start of Backgrounds
    wto << "}, {";
     for (int ii = 0; ii < ctex.es->rooms[i].backgroundDefCount; ii++) // For each background
     {
        wto << "      { "
        << (ctex.es->rooms[i].backgroundDefs[ii].visible ? "true" : "false") << ",   " // Visible
        << (ctex.es->rooms[i].backgroundDefs[ii].foreground ? "true" : "false") << ",   " // Foreground
        << ctex.es->rooms[i].backgroundDefs[ii].backgroundId << ",   " // Background
        << ctex.es->rooms[i].backgroundDefs[ii].x << ",   " // X
        << ctex.es->rooms[i].backgroundDefs[ii].y << ",   " // Y
        << ctex.es->rooms[i].backgroundDefs[ii].hSpeed << ",   " // HSpeed
        << ctex.es->rooms[i].backgroundDefs[ii].vSpeed << ",   " // VSpeed
        << (ctex.es->rooms[i].backgroundDefs[ii].tileHoriz ? "true" : "false") << ",   " // tileHor
        << (ctex.es->rooms[i].backgroundDefs[ii].tileVert ? "true" : "false") << ",   " // tileVert
        << (ctex.es->rooms[i].backgroundDefs[ii].stretch ? "true" : "false") // Stretch
        << " },\n";
     }
    wto <<
    "      }," //End of Backgrounds

    << "      " << ctex.es->rooms[i].instanceCount << ", insts_" << ctex.es->rooms[i].id
    << "      ," << ctex.es->rooms[i].tileCount << ", tiles_" << ctex.es->rooms[i].id;

    // End of this room
    wto << "    },\n";

    if (ctex.es->rooms[i].id > room_highid)
      room_highid = ctex.es->rooms[i].id;
  }

  wto << "  };\n  \n"; // End of all rooms
  wto << "  int room_max = " <<  room_highid << " + 1;\n  int maxid = " << room_highinstid << " + 1;\n";

  wto << "} // Namespace enigma\n";

  if (ctex.es->roomCount == 0)
  {
    wto << "int room_first = 0;\n";
    wto << "int room_last = 0;\n";
  }
  else
  {
    wto << "int room_first = " << ctex.es->rooms[0].id << ";\n";
    wto << "int room_last = " << ctex.es->rooms[ctex.es->roomCount-1].id << ";\n";
  }
wto.close();


wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomcreates.h",ios_base::out);
  wto << gen_license;
  for (int i = 0; i < ctex.es->roomCount; i++)
  {
    parsed_room *pr = ctex.parsed_rooms[ctex.es->rooms[i].id];
    if (pr->instance_create_codes.size()) {
      for (map<int,parsed_room::parsed_icreatecode*>::iterator it = pr->instance_create_codes.begin(); it != pr->instance_create_codes.end(); it++)
      {
        wto << "void room_"<< ctex.es->rooms[i].id <<"_instancecreate_" << it->first << "()\n{\n  ";
        print_to_stream(ctex, it->second->code, 2, wto);
        wto << "}\n\n";
      }
    }

    if (pr->instance_create_codes.size() or pr->creation_code)
    {
      wto << "void roomcreate" << ctex.es->rooms[i].id << "()\n{\n  ";
      
      if (pr->creation_code)
        print_to_stream(ctex, *pr->creation_code, 2, wto);

      for (map<int,parsed_room::parsed_icreatecode*>::iterator it = pr->instance_create_codes.begin(); it != pr->instance_create_codes.end(); it++)
        wto << "\n  room_"<< ctex.es->rooms[i].id <<"_instancecreate_" << it->first << "();";

      wto << "\n}\n";
    }
  }
wto.close();

  return 0;
}
