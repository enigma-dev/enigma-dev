/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008, 2018 Josh Ventura                                       **
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

#include "settings.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

#include "backend/GameData.h"
#include "parser/parser.h"
#include "parser/object_storage.h"
#include "compiler/compile_common.h"

#define flushl '\n' << flush
#define flushs flush

#include "languages/lang_CPP.h"

inline std::string format_color(uint32_t color) {
  std::stringstream ss;
  ss << "0x" << std::hex << color;
  return ss.str();
}

inline string resname(string name) {
  return name.empty() ? "-1" : name;
}

int lang_CPP::compile_writeRoomData(const GameData &game, const CompileState &state, int mode)
{
  ofstream wto((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h").u8string().c_str(),ios_base::out);

  wto << license << "namespace enigma {\n"
  << "  int room_loadtimecount = " << game.rooms.size() << ";\n";
  int room_highid = 0, room_highinstid = 100000,room_hightileid=10000000;

  for (const auto &room : game.rooms) {
    wto << "  std::vector<tile> tiles_" << room.id() << " = {\n";
    int modme = 0;
    for (const auto &tile : room->tiles()) {
      wto << "{"
          << tile.id()      << ","
          << resname(tile.background_name()) << ","
          << tile.xoffset() << ","
          << tile.yoffset() << ","
          << tile.depth()   << ","
          << tile.height()  << ","
          << tile.width()   << ","
          << tile.x()       << ","
          << tile.y()       << ","
          << tile.xscale()  << ","
          << tile.yscale()  << ","
          << tile.alpha()   << ","
          << tile.color()   << "},";
        if (++modme % 16 == 0) wto << "\n        ";
      if (tile.id() > room_hightileid)
        room_hightileid = tile.id();
    }
    wto << "  };\n";
  }

  for (const auto &room : game.rooms) {
    wto << "  std::vector<inst> insts_" << room.id() << " = {\n";
    int modme = 0;
    for (const auto &instance : room->instances()) {
      wto << "{" <<
        instance.id() << "," <<
        instance.object_type() << "," <<
        instance.x() << "," <<
        instance.y() << "},";
        if (++modme % 16 == 0) wto << "\n        ";
      if (instance.id() > room_highinstid)
        room_highinstid = instance.id();
    }
    wto << "  };\n";
  }

  wto << "  roomstruct grd_rooms[" << game.rooms.size() << "] = {\n";
  for (size_t room_index = 0; room_index < game.rooms.size(); ++room_index) {
    const auto &room = game.rooms[room_index];
    wto << "    //Room " << room.id() << "\n" <<
    "    { "
    << room.id() << ", "  // The ID of this room
    << room_index << ", \""  // The tree order index of this room
    << room.name << "\",  \""  // The name of this room
    << room->caption() << "\",\n      "  // The caption of this room

    << format_color(room->color()) << ", "  // Background color
    << (room->show_color() ? "true" : "false")  // Draw background color
    << ", roomcreate" << room.id() << ",\n      "  // Creation code
    << "roomprecreate" << room.id() << ",\n      "  // PreCreation code

    <<  room->width() << ", " << room->height() << ", "  // Width and Height
    <<  room->speed() << ",  "  // Speed
    << (room->persistent() ? "true" : "false")  << ",  "  // Persistent

    << (room->enable_views() ? "true" : "false") << ", {\n";  // Views Enabled

    for (const auto &view : room->views()) {
      wto << "        { "
          << (view.visible() ? "true" : "false") << ",   " // Visible

          << view.xview() << ", "   // Xview
          << view.yview() << ",  "  // Yview
          << view.wview() << ", "    // Wview
          << view.hview() << ",   "  // Hview

          << view.xport() << ", " << view.yport() << ",  "   // Xport and Yport
          << view.wport() << ", " << view.hport() << ",   "  // Wport and Hport

          << resname(view.object_following()) << ",  " // Object2Follow

          << view.hborder() << ", " << view.vborder() << ",  "  // Hborder and Vborder
          << view.hspeed()<< ", " << view.vspeed()  // Hspeed and Vspeed

      << " },\n";
    }
    //Start of Backgrounds
    wto << "      }, {\n      ";
     for (const auto &background : room->backgrounds()) {
        wto << "  { "
        << (background.visible() ? "true" : "false")    << ", "    // Visible
        << (background.foreground() ? "true" : "false") << ", "    // Foreground
        <<  resname(background.background_name())       << ",  "   // Background
        <<  background.x()                              << ", "    // X
        <<  background.y()                              << ",   "  // Y
        <<  background.hspeed()                         << ", "    // HSpeed
        <<  background.vspeed()                         << ",   "  // VSpeed
        << (background.htiled()  ? "true" : "false")    << ", "    // tileHor
        << (background.vtiled()  ? "true" : "false")    << ",   "  // tileVert
        << (background.stretch() ? "true" : "false")    << ",   "  // Stretch
        <<  background.alpha()                          << ",   "  // Alpha
        <<  background.color()  << " },\n      ";                  // Color
     }
    wto <<
    " },\n" //End of Backgrounds
    "      " << "insts_" << room.id() << ",\n"
    "      " << "tiles_" << room.id() << "\n"
    "    },\n";

    if (room.id() > room_highid)
      room_highid = room.id();
  }

  wto << "  };\n  \n"; // End of all rooms
  wto << "  int maxid = " << room_highinstid << " + 1;\n  int maxtileid = " << room_hightileid << " + 1;\n";

  wto << "} // Namespace enigma\n";

  if (game.rooms.size() == 0)
  {
    wto << "int room_first = 0;\n";
    wto << "int room_last = 0;\n";
  }
  else
  {
    wto << "int room_first = " << game.rooms[0].id() << ";\n";
    wto << "int room_last = " << game.rooms[game.rooms.size()-1].id() << ";\n";
  }
  wto.close();


  wto.open((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_roomcreates.h").u8string().c_str(),ios_base::out);
  wto << license;

  wto << "namespace enigma {\n\n";
  wto << "void extensions_initialize() {\n";
  for (const auto &ext : parsed_extensions) {
    if (ext.init.empty()) continue;
    wto << "  " << ext.init << "();\n";
  }
  wto << "}\n\n} // namespace enigma\n\n";

  for (size_t room_index = 0; room_index < game.rooms.size(); ++room_index) {
    const auto &room = game.rooms[room_index];
    parsed_room *pr = state.parsed_rooms[room_index];
    for (const auto &int_ev_pair : pr->instance_create_codes) {
      wto << "variant room_" << room.id()
          << "_instancecreate_" << int_ev_pair.first << "()\n{\n  ";
      if (mode == emode_debug) {
        wto << "enigma::debug_scope $current_scope(\"'instance creation' for instance '" << int_ev_pair.first << "'\");\n  ";
      }

      std::string codeOvr;
      std::string syntOvr;
      auto &junkshit = int_ev_pair.second.code->ast.junkshit;
      if (junkshit.code.find("with((")==0) {
        //We're basically replacing "with((100002)){" with "with_room_inst((100002)){" (synt: "ssss((000000)){")
        //This is because room-instance-creation code might need a deactivated instance, which "with" cannot find.
        codeOvr = "with_room_inst(" + junkshit.code.substr(5);
        syntOvr = "ssssssssssssss(" + junkshit.synt.substr(5);
      }

      print_to_file(
        state.parse_context,
        codeOvr.empty() ? junkshit.code : codeOvr,
        syntOvr.empty() ? junkshit.synt : syntOvr,
        junkshit.strc, junkshit.strs, 2, wto
      );
      wto << "  return 0;\n}\n\n";
    }

    for (map<int,parsed_room::parsed_icreatecode>::iterator it = pr->instance_precreate_codes.begin(); it != pr->instance_precreate_codes.end(); it++)
    {
      wto << "variant room_"<< room.id() <<"_instanceprecreate_" << it->first << "()\n{\n  ";
      if (mode == emode_debug) {
        wto << "enigma::debug_scope $current_scope(\"'instance preCreation' for instance '" << it->first << "'\");\n  ";
      }

      std::string codeOvr;
      std::string syntOvr;
      const auto &junkshit = it->second.code->ast.junkshit;
      if (junkshit.code.find("with((")==0) {
        //We're basically replacing "with((100002)){" with "with_room_inst((100002)){" (synt: "ssss((000000)){")
        //This is because room-instance-precreation code might need a deactivated instance, which "with" cannot find.
        codeOvr = "with_room_inst(" + junkshit.code.substr(5);
        syntOvr = "ssssssssssssss(" + junkshit.synt.substr(5);
      }

      print_to_file(
        state.parse_context,
        codeOvr.empty() ? junkshit.code : codeOvr,
        syntOvr.empty() ? junkshit.synt : syntOvr,
        junkshit.strc, junkshit.strs, 2, wto
      );
      wto << "  return 0;\n}\n\n";
    }

    wto << "variant roomprecreate" << room.id() << "()\n{\n";
    if (mode == emode_debug) {
      wto << "  enigma::debug_scope $current_scope(\"'room preCreation' for room '" << room.name << "'\");\n";
    }
    wto << "  ";

    //LGM doesn't expose a ROOM PreCreation code yet, only the instance one
    //parsed_event& ev = pr->events[0];
    //print_to_file(ev.code, ev.synt, ev.strc, ev.strs, 2, wto);

    for (map<int,parsed_room::parsed_icreatecode>::iterator it = pr->instance_precreate_codes.begin(); it != pr->instance_precreate_codes.end(); it++)
      wto << "\n  room_"<< room.id() <<"_instanceprecreate_" << it->first << "();";

    wto << "\n  return 0;\n}\n\n";

    // TODO: Don't write empty room creation codes
    wto << "variant roomcreate" << room.id() << "()\n{\n";
    if (mode == emode_debug) {
      wto << "  enigma::debug_scope $current_scope(\"'room creation' for room '" << room.name << "'\");\n";
    }
    pr->creation_code->ast.PrettyPrint(wto);

    for (map<int,parsed_room::parsed_icreatecode>::iterator it = pr->instance_create_codes.begin(); it != pr->instance_create_codes.end(); it++)
      wto << "\n  room_"<< room.id() <<"_instancecreate_" << it->first << "();";

    wto << "\n  return 0;\n}\n";
  }
  wto.close();

  return 0;
}
