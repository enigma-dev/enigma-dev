/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
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

#include "makedir.h"
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

int lang_CPP::compile_writeRoomData(EnigmaStruct* es, parsed_object *EGMglobal, int mode)
{
  ofstream wto((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h").c_str(),ios_base::out);

  wto << license << "namespace enigma {\n"
  << "  int room_loadtimecount = " << es->roomCount << ";\n";
  int room_highid = 0, room_highinstid = 100000,room_hightileid=10000000;

  for (int i = 0; i < es->roomCount; i++)
  {
    wto << "  tile tiles_" << es->rooms[i].id << "[] = {\n";
    for (int ii = 0, modme = 0; ii < es->rooms[i].tileCount; ii++)
    {
      wto << "{" <<
        es->rooms[i].tiles[ii].id << "," <<
        es->rooms[i].tiles[ii].backgroundId << "," <<
        es->rooms[i].tiles[ii].bgX << "," <<
        es->rooms[i].tiles[ii].bgY << "," <<
        es->rooms[i].tiles[ii].depth << "," <<
        es->rooms[i].tiles[ii].height << "," <<
        es->rooms[i].tiles[ii].width << "," <<
        es->rooms[i].tiles[ii].roomX << "," <<
        es->rooms[i].tiles[ii].roomY << "," <<
        /*es->rooms[i].tiles[ii].xscale*/1 << "," <<
        /*es->rooms[i].tiles[ii].yscale*/1 << "," <<
        /*es->rooms[i].tiles[ii].alpha*/"1.0" << "," <<
        /*es->rooms[i].tiles[ii].color*/0xFFFFFF << "},";
        if (++modme % 16 == 0) wto << "\n        ";
      if (es->rooms[i].tiles[ii].id > room_hightileid)
        room_hightileid = es->rooms[i].tiles[ii].id;
    }
    wto << "  };\n";
  }

  for (int i = 0; i < es->roomCount; i++)
  {
    wto << "  inst insts_" << es->rooms[i].id << "[] = {\n";
    for (int ii = 0, modme = 0; ii < es->rooms[i].instanceCount; ii++)
    {
      wto << "{" <<
        es->rooms[i].instances[ii].id << "," <<
        es->rooms[i].instances[ii].objectId << "," <<
        es->rooms[i].instances[ii].x << "," <<
        es->rooms[i].instances[ii].y << "},";
        if (++modme % 16 == 0) wto << "\n        ";
      if (es->rooms[i].instances[ii].id > room_highinstid)
        room_highinstid = es->rooms[i].instances[ii].id;
    }
    wto << "  };\n";
  }

  wto << "  roomstruct grd_rooms[" << es->roomCount << "] = {\n";
  for (int i = 0; i < es->roomCount; i++)
  {
    wto << "    //Room " << es->rooms[i].id << "\n" <<
    "    { "
    << es->rooms[i].id << ", " // The ID of this room
    << i << ", \"" // The tree order index of this room
    << es->rooms[i].name << "\",  \""  // The name of this room
    << es->rooms[i].caption << "\",\n      " // The caption of this room

    << javaColor(es->rooms[i].backgroundColor) << ", "//Background color
    << (es->rooms[i].drawBackgroundColor ? "true" : "false")
    << ", roomcreate" << es->rooms[i].id << ",\n      " // Creation code
    << "roomprecreate" << es->rooms[i].id << ",\n      " // PreCreation code
    
    << es->rooms[i].width << ", " << es->rooms[i].height << ", " // Width and Height
    << es->rooms[i].speed << ",  "  // Speed
    << (es->rooms[i].persistent ? "true" : "false") << ",  "  // Persistent

    << (es->rooms[i].enableViews ? "true" : "false") << ", {\n"; // Views Enabled

    for (int ii = 0; ii < es->rooms[i].viewCount; ii++) // For each view
    {
      wto << "      { "
      << (es->rooms[i].views[ii].visible ? "true" : "false") << ",   " // Visible

      << es->rooms[i].views[ii].viewX << ", " << es->rooms[i].views[ii].viewY << ", "   // Xview and Yview
      << es->rooms[i].views[ii].viewW << ", " << es->rooms[i].views[ii].viewH << ",   " // Wview and Hview

      << es->rooms[i].views[ii].portX << ", " << es->rooms[i].views[ii].portY << ", "   // Xport and Yport
      << es->rooms[i].views[ii].portW << ", " << es->rooms[i].views[ii].portH << ",   " // Wport and Hport

      << es->rooms[i].views[ii].objectId << ",   " // Object2Follow

      << es->rooms[i].views[ii].borderH << ", " << es->rooms[i].views[ii].borderV << ",   " //Hborder and Vborder
      << es->rooms[i].views[ii].speedH<< ", " << es->rooms[i].views[ii].speedV //Hspeed and Vspeed

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
        << (es->rooms[i].backgroundDefs[ii].stretch ? "true" : "false") << ",   " // Stretch
        << /*es->rooms[i].backgroundDefs[ii].alpha*/1 << ",   " // Alpha
        << /*es->rooms[i].backgroundDefs[ii].color*/0xFFFFFF << " },\n";  // Color
     }
    wto <<
    "      }," //End of Backgrounds

    << "      " << es->rooms[i].instanceCount << ", insts_" << es->rooms[i].id
    << "      ," << es->rooms[i].tileCount << ", tiles_" << es->rooms[i].id;

    // End of this room
    wto << "    },\n";

    if (es->rooms[i].id > room_highid)
      room_highid = es->rooms[i].id;
    
    (void)EGMglobal; // No need to know globals, here.
  }

  wto << "  };\n  \n"; // End of all rooms
  wto << "  int maxid = " << room_highinstid << " + 1;\n  int maxtileid = " << room_hightileid << " + 1;\n";

  wto << "} // Namespace enigma\n";

  if (es->roomCount == 0)
  {
    wto << "int room_first = 0;\n";
    wto << "int room_last = 0;\n";
  }
  else
  {
    wto << "int room_first = " << es->rooms[0].id << ";\n";
    wto << "int room_last = " << es->rooms[es->roomCount-1].id << ";\n";
  }
wto.close();


wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_roomcreates.h").c_str(),ios_base::out);
  wto << license;
  for (int i = 0; i < es->roomCount; i++)
  {
    parsed_room *pr = parsed_rooms[es->rooms[i].id];
    for (map<int,parsed_room::parsed_icreatecode>::iterator it = pr->instance_create_codes.begin(); it != pr->instance_create_codes.end(); it++)
    {
      wto << "variant room_"<< es->rooms[i].id <<"_instancecreate_" << it->first << "()\n{\n  ";
      if (mode == emode_debug) {
        wto << "enigma::debug_scope $current_scope(\"'instance creation' for instance '" << it->first << "'\");\n";
      }

      std::string codeOvr;
      std::string syntOvr;
      if (it->second.pe->code.find("with((")==0) {
        //We're basically replacing "with((100002)){" with "with_room_inst((100002)){" (synt: "ssss((000000)){")
        //This is because room-instance-creation code might need a deactivated instance, which "with" cannot find.
        codeOvr = "with_room_inst(" + it->second.pe->code.substr(5);
        syntOvr = "ssssssssssssss(" + it->second.pe->synt.substr(5);
      }

      print_to_file(
        codeOvr.empty() ? it->second.pe->code : codeOvr,
        syntOvr.empty() ? it->second.pe->synt : syntOvr,
        it->second.pe->strc, it->second.pe->strs, 2, wto
      );
      wto << "  return 0;\n}\n\n";
    }
    
    for (map<int,parsed_room::parsed_icreatecode>::iterator it = pr->instance_precreate_codes.begin(); it != pr->instance_precreate_codes.end(); it++)
    {
      wto << "variant room_"<< es->rooms[i].id <<"_instanceprecreate_" << it->first << "()\n{\n  ";
      if (mode == emode_debug) {
        wto << "enigma::debug_scope $current_scope(\"'instance preCreation' for instance '" << it->first << "'\");\n";
      }

      std::string codeOvr;
      std::string syntOvr;
      if (it->second.pe->code.find("with((")==0) {
        //We're basically replacing "with((100002)){" with "with_room_inst((100002)){" (synt: "ssss((000000)){")
        //This is because room-instance-precreation code might need a deactivated instance, which "with" cannot find.
        codeOvr = "with_room_inst(" + it->second.pe->code.substr(5);
        syntOvr = "ssssssssssssss(" + it->second.pe->synt.substr(5);
      }

      print_to_file(
        codeOvr.empty() ? it->second.pe->code : codeOvr,
        syntOvr.empty() ? it->second.pe->synt : syntOvr,
        it->second.pe->strc, it->second.pe->strs, 2, wto
      );
      wto << "  return 0;\n}\n\n";
    }
    
    wto << "variant roomprecreate" << es->rooms[i].id << "()\n{\n  ";
    if (mode == emode_debug) {
      wto << "enigma::debug_scope $current_scope(\"'room preCreation' for room '" << es->rooms[i].name << "'\");\n";
    }
    //LGM doesn't expose a ROOM PreCreation code yet, only the instance one
    //parsed_event& ev = pr->events[0];
    //print_to_file(ev.code, ev.synt, ev.strc, ev.strs, 2, wto);

    for (map<int,parsed_room::parsed_icreatecode>::iterator it = pr->instance_precreate_codes.begin(); it != pr->instance_precreate_codes.end(); it++)
      wto << "\n  room_"<< es->rooms[i].id <<"_instanceprecreate_" << it->first << "();";

    wto << "\n  return 0;\n  }\n";

    wto << "variant roomcreate" << es->rooms[i].id << "()\n{\n  ";
    if (mode == emode_debug) {
      wto << "enigma::debug_scope $current_scope(\"'room creation' for room '" << es->rooms[i].name << "'\");\n";
    }
    parsed_event& ev = pr->events[0];
    print_to_file(ev.code, ev.synt, ev.strc, ev.strs, 2, wto);

    for (map<int,parsed_room::parsed_icreatecode>::iterator it = pr->instance_create_codes.begin(); it != pr->instance_create_codes.end(); it++)
      wto << "\n  room_"<< es->rooms[i].id <<"_instancecreate_" << it->first << "();";
    
    wto << "\n  return 0;\n  }\n";
  }
wto.close();

  return 0;
}
