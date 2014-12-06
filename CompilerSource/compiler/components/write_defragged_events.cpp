/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2014 Seth N. Hetu
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

#include "makedir.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "backend/ideprint.h"

using namespace std;


#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "compiler/compile_common.h"

#include "compiler/event_reader/event_parser.h"
#include "languages/lang_CPP.h"

struct foundevent { int mid, id, count; foundevent(): mid(0),id(0),count(0) {} void f2(int m,int i) { id = i, mid = m; } void inc(int m,int i) { mid=m,id=i,count++; } void operator++(int) { count++; } };
typedef map<string,foundevent>::iterator evfit;

int lang_CPP::compile_writeDefraggedEvents(EnigmaStruct* es)
{
  /* Generate a new list of events used by the objects in
  ** this game. Only events on this list will be exported.
  ***********************************************************/
  map<string,foundevent> used_events;
  
  // Defragged events must be written before object data, or object data cannot determine which events were used.
  used_events.clear();
  for (int i = 0; i < es->gmObjectCount; i++) {
    for (int ii = 0; ii < es->gmObjects[i].mainEventCount; ii++) {
      for (int iii = 0; iii < es->gmObjects[i].mainEvents[ii].eventCount; iii++) {
        const int mid = es->gmObjects[i].mainEvents[ii].id, id = es->gmObjects[i].mainEvents[ii].events[iii].id;
        if (event_is_instance(mid,id)) {
          used_events[event_stacked_get_root_name(mid)].inc(mid,id);
        } else {
          used_events[event_get_function_name(mid,id)].inc(mid,id);
        }
      }
    }
  }

  ofstream wto((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_evparent.h").c_str());
  wto << license;

  //Write timeline/moment names. Timelines are like scripts, but we don't have to worry about arguments or return types.
  for (int i=0; i<es->timelineCount; i++) {
    for (int j=0; j<es->timelines[i].momentCount; j++) {
      wto << "void TLINE_" <<es->timelines[i].name <<"_MOMENT_" <<es->timelines[i].moments[j].stepNo <<"();\n";
    }
  }
  wto <<"\n";

  /* Some events are included in all objects, even if the user
  ** hasn't specified code for them. Account for those here.
  ***********************************************************/
  for (size_t i=0; i<event_sequence.size(); i++)
  {
    const int mid = event_sequence[i].first, id = event_sequence[i].second;
    if (event_has_default_code(mid,id)) // We may not be using this event, but if it's persistent... (constant or defaulted: mandatory)
    {
      used_events[event_get_function_name(mid,id)].f2(mid,id); // ...Reserve it anyway.

      for (po_i it = parsed_objects.begin(); it != parsed_objects.end(); it++) // Then shell it out into the other objects.
      {
        bool exists = false;
        for (unsigned j = 0; j < it->second->events.size; j++)
          if (it->second->events[j].mainId == mid and it->second->events[j].id == id)
            { exists = true; break; }
        if (!exists)
          it->second->events[it->second->events.size] = parsed_event(mid, id, it->second);
      }
    }
  }

  /* Now we forge a top-level tier for object declaration
  ** that defines default behavior for any obect's unused events.
  *****************************************************************/
  wto << "namespace enigma" << endl << "{" << endl;

  wto << "  struct event_parent: " << system_get_uppermost_tier() << endl;
  wto << "  {" << endl;
            for (evfit it = used_events.begin(); it != used_events.end(); it++)
            {
              const bool e_is_inst = event_is_instance(it->second.mid, it->second.id);
              if (event_has_sub_check(it->second.mid, it->second.id) && !e_is_inst) {
                wto << "    inline virtual bool myevent_" << it->first << "_subcheck() { return false; }\n";
              } 
              wto << (e_is_inst ? "    virtual void    myevent_" : "    virtual variant myevent_") << it->first << "()";
              if (event_has_default_code(it->second.mid,it->second.id))
                wto << endl << "    {" << endl << "  " << event_get_default_code(it->second.mid,it->second.id) << endl << (e_is_inst ? "    }" : "    return 0;\n    }") << endl;
              else wto << (e_is_inst ? " { } // No default " : " { return 0; } // No default ") << event_get_human_name(it->second.mid,it->second.id) << " code." << endl;
            }

  //The event_parent also contains the definitive lookup table for all timelines, as a fail-safe in case localized instances can't find their own timelines.
  wto << "    virtual void timeline_call_moment_script(int timeline_index, int moment_index) {\n";
  wto << "      switch (timeline_index) {\n";
  for (int i=0; i<es->timelineCount; i++) {
    wto << "        case " <<es->timelines[i].id <<": {\n";
    wto << "          switch (moment_index) {\n";
    for (int j=0; j<es->timelines[i].momentCount; j++) {
      wto << "            case " <<j <<": {\n";
      wto << "              ::TLINE_" <<es->timelines[i].name <<"_MOMENT_" <<es->timelines[i].moments[j].stepNo <<"();\n";
      wto << "              break;\n";
      wto << "            }\n";
    }
    wto << "          }\n";
    wto << "        }\n";
    wto << "        break;\n";
  }
  wto << "      }\n";
  wto << "    }\n";

  wto << "    //virtual void unlink() {} // This is already declared at the super level." << endl;
  wto << "    virtual variant myevents_perf(int type, int numb) {return 0;}" << endl;
  wto << "    event_parent() {}" << endl;
  wto << "    event_parent(unsigned _x, int _y): " << system_get_uppermost_tier() << "(_x,_y) {}" << endl;
  wto << "  };" << endl;
  wto << "}" << endl;
  wto.close();


  /* Now we write the actual event sequence code, as
  ** well as an initializer function for the whole system.
  ***************************************************************/
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_events.h").c_str());
  wto << license;
  wto << "namespace enigma" << endl << "{" << endl;

  // Start by defining storage locations for our event lists to iterate.
  for (evfit it = used_events.begin(); it != used_events.end(); it++)
    wto  << "  event_iter *event_" << it->first << "; // Defined in " << it->second.count << " objects" << endl;

  // Here's the initializer
  wto << "  int event_system_initialize()" << endl << "  {" << endl;
    wto  << "    events = new event_iter[" << used_events.size() << "]; // Allocated here; not really meant to change." << endl;
    int obj_high_id = parsed_objects.rbegin() != parsed_objects.rend() ? parsed_objects.rbegin()->first : 0;
    wto  << "    objects = new objectid_base[" << (obj_high_id+1) << "]; // Allocated here; not really meant to change." << endl;

    int ind = 0;
    for (evfit it = used_events.begin(); it != used_events.end(); it++)
      wto  << "    event_" << it->first << " = events + " << ind++ << ";  event_" << it->first << "->name = \"" << event_get_human_name(it->second.mid,it->second.id) << "\";" << endl;
    wto << "    return 0;" << endl;
  wto << "  }" << endl;

    // Game setting initaliser
  wto << "  int game_settings_initialize()" << endl << "  {" << endl;
  // This should only effect texture interpolation if it has not already been enabled
    if (!es->gameSettings.displayCursor)
        wto  << "    window_set_cursor(cr_none);" << endl;

    if (es->gameSettings.alwaysOnTop)
        wto  << "    window_set_stayontop(true);" << endl;

    wto << "    return 0;" << endl;
  wto << "  }" << endl;

  wto << "  variant ev_perf(int type, int numb)\n  {\n    return ((enigma::event_parent*)(instance_event_iterator->inst))->myevents_perf(type, numb);\n  }\n";

  /* Some Super Checks are more complicated than others, requiring a function. Export those functions here. */
  for (evfit it = used_events.begin(); it != used_events.end(); it++)
    wto << event_get_super_check_function(it->second.mid, it->second.id);

  /* Now the event sequence */
  bool using_gui = false;
  wto << "  int ENIGMA_events()" << endl << "  {" << endl;
    for (size_t i=0; i<event_sequence.size(); i++)
    {
      // First, make sure we're actually using this event in some object
      const int mid = event_sequence[i].first, id = event_sequence[i].second;
      evfit it = used_events.find(event_is_instance(mid,id) ? event_stacked_get_root_name(mid) : event_get_function_name(mid,id));
      if (it == used_events.end()) continue;
      if (mid == 7 && (id >= 10 && id <= 25)) continue;   //User events, don't want to be run in the event sequence. TODO: Remove hard-coded values.
      string seqcode = event_forge_sequence_code(mid,id,it->first);
      if (mid == 8 && id == 64)
      {
          if (seqcode != "")
            using_gui = true;

          continue;       // Don't want gui loop to be added
      }

      if (seqcode != "")
        wto << seqcode,
        wto << "    " << endl,
        wto << "    enigma::update_globals();" << endl,
        wto << "    " << endl;
    }
    wto << "    after_events:" << endl;
    if (es->gameSettings.letEscEndGame)
        wto << "    if (keyboard_check_pressed(vk_escape)) game_end();" << endl;
    if (es->gameSettings.letF4SwitchFullscreen)
        wto << "    if (keyboard_check_pressed(vk_f4)) window_set_fullscreen(!window_get_fullscreen());" << endl;
    if (es->gameSettings.letF1ShowGameInfo)
        wto << "    if (keyboard_check_pressed(vk_f1)) show_info();" << endl;
    if (es->gameSettings.letF9Screenshot)
        wto << "    if (keyboard_check_pressed(vk_f9)) {}" << endl;   //TODO: Screenshot function
    if (es->gameSettings.letF5SaveF6Load)  //TODO: uncomment after game save and load fucntions implemented
    {
        wto << "    //if (keyboard_check_pressed(vk_f5)) game_save('_save" << es->gameSettings.gameId << ".sav');" << endl;
        wto << "    //if (keyboard_check_pressed(vk_f6)) game_load('_save" << es->gameSettings.gameId << ".sav');" << endl;
    }
    // Handle room switching/game restart.
    wto << "    enigma::dispose_destroyed_instances();" << endl;
    wto << "    enigma::rooms_switch();" << endl;
    wto << "    enigma::set_room_speed(room_speed);" << endl;
    wto << "    " << endl;
    wto << "    return 0;" << endl;
  wto << "  } // event function" << endl;

  wto << "  bool gui_used = " << using_gui << ";" << endl;
  // Done, end the namespace
  wto << "} // namespace enigma" << endl;
  wto.close();

  return 0;
}
