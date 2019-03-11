/** Copyright (C) 2008, 2018 Josh Ventura
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

#include "backend/GameData.h"
#include "compiler/compile_common.h"

#include "event_reader/event_parser.h"
#include "languages/lang_CPP.h"

struct foundevent {
  int mid, id, count;
  foundevent(): mid(0),id(0),count(0) {}
  void f2(int m, int i)  { id = i, mid = m; }
  void inc(int m, int i) { mid = m, id = i, count++; }
  void operator++(int) { count++; }
};
typedef map<string,foundevent>::iterator evfit;

static inline int event_get_number(const buffers::resources::Event &event) {
  if (event.has_name()) {
    std::cerr << "ERROR! IMPLEMENT ME: Event names not supported in compiler.\n";
  }
  return event.number();
}

int lang_CPP::compile_writeDefraggedEvents(const GameData &game, const ParsedObjectVec &parsed_objects)
{
  ofstream wto((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_evparent.h").c_str());
  wto << license;

  //Write timeline/moment names. Timelines are like scripts, but we don't have to worry about arguments or return types.
  for (int i=0; i<es->timelineCount; i++) {
    for (int j=0; j<es->timelines[i].momentCount; j++) {
      wto << "void TLINE_" <<es->timelines[i].name <<"_MOMENT_" <<es->timelines[i].moments[j].stepNo <<"();\n";
    }
  }
  wto <<"\n";

  /* Generate a new list of events used by the objects in
  ** this game. Only events on this list will be exported.
  ***********************************************************/
  map<string,foundevent> used_events;

  // Defragged events must be written before object data, or object data cannot determine which events were used.
  used_events.clear();
  for (size_t i = 0; i < game.objects.size(); i++) {
    for (int ii = 0; ii < game.objects[i]->events().size(); ii++) {
      const int mid = game.objects[i]->events(ii).type();
      const int  id = event_get_number(game.objects[i]->events(ii));
      if (event_is_instance(mid, id)) {
        used_events[event_stacked_get_root_name(mid)].inc(mid,id);
      } else {
        used_events[event_get_function_name(mid,id)].inc(mid,id);
      }
    }
  }

  ofstream wto((codegen_directory + "Preprocessor_Environment_Editable/IDE_EDIT_evparent.h").c_str());
  wto << license;

  //Write timeline/moment names. Timelines are like scripts, but we don't have to worry about arguments or return types.
  for (size_t i = 0; i < game.timelines.size(); i++) {
    for (int j = 0; j < game.timelines[i]->moments().size(); j++) {
      wto << "void TLINE_" << game.timelines[i].name << "_MOMENT_"
          << game.timelines[i]->moments(j).step() <<"();\n";
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

      for (parsed_object *obj : parsed_objects) { // Then shell it out into the other objects.
        bool exists = false;
        for (unsigned j = 0; j < obj->events.size; j++)
          if (obj->events[j].mainId == mid and obj->events[j].id == id)
            { exists = true; break; }
        if (!exists)
          obj->events[obj->events.size] = parsed_event(mid, id, obj);
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
  for (size_t i = 0; i < game.timelines.size(); i++) {
    wto << "        case " << game.timelines[i].id() <<": {\n";
    wto << "          switch (moment_index) {\n";
    for (int j = 0; j < game.timelines[i]->moments().size(); j++) {
      wto << "            case " <<j <<": {\n";
      wto << "              ::TLINE_" << game.timelines[i].name << "_MOMENT_"
                                      << game.timelines[i]->moments(j).step() << "();\n";
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
  wto.open((codegen_directory + "Preprocessor_Environment_Editable/IDE_EDIT_events.h").c_str());
  wto << license;
  wto << "namespace enigma" << endl << "{" << endl;

  // Start by defining storage locations for our event lists to iterate.
  for (evfit it = used_events.begin(); it != used_events.end(); it++)
    wto << "  event_iter *event_" << it->first << "; // Defined in " << it->second.count << " objects" << endl;

  // Here's the initializer
  wto << "  int event_system_initialize()" << endl << "  {" << endl;
  wto << "    events = new event_iter[" << used_events.size() << "]; // Allocated here; not really meant to change." << endl;
  
  int obj_high_id = 0;
  for (parsed_object *obj : parsed_objects) {
    if (obj->id > obj_high_id) obj_high_id = obj->id;
  }
  wto << "    objects = new objectid_base[" << (obj_high_id+1) << "]; // Allocated here; not really meant to change." << endl;

  int ind = 0;
  for (evfit it = used_events.begin(); it != used_events.end(); it++)
    wto << "    event_" << it->first << " = events + " << ind++ << ";  event_" << it->first << "->name = \"" << event_get_human_name(it->second.mid,it->second.id) << "\";" << endl;
  wto << "    return 0;" << endl;
  wto << "  }" << endl;

    // Game setting initaliser
  wto << "  int game_settings_initialize()" << endl << "  {" << endl;
  // This should only effect texture interpolation if it has not already been enabled
  if (!game.settings.general().show_cursor())
    wto << "    window_set_cursor(cr_none);" << endl;

  if (game.settings.windowing().stay_on_top())
    wto << "    window_set_stayontop(true);" << endl;

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
  if (game.settings.shortcuts().let_escape_end_game())
    wto << "    if (keyboard_check_pressed(vk_escape)) game_end();" << endl;
  if (game.settings.shortcuts().let_f4_switch_fullscreen())
    wto << "    if (keyboard_check_pressed(vk_f4)) window_set_fullscreen(!window_get_fullscreen());" << endl;
  if (game.settings.shortcuts().let_f1_show_game_info())
    wto << "    if (keyboard_check_pressed(vk_f1)) show_info();" << endl;
  if (game.settings.shortcuts().let_f9_screenshot())
    wto << "    if (keyboard_check_pressed(vk_f9)) {}" << endl;   //TODO: Screenshot function
  if (game.settings.shortcuts().let_f5_save_f6_load())  //TODO: uncomment after game save and load fucntions implemented
  {
    wto << "    //if (keyboard_check_pressed(vk_f5)) game_save('_save" << game.settings.general().game_id() << ".sav');" << endl;
    wto << "    //if (keyboard_check_pressed(vk_f6)) game_load('_save" << game.settings.general().game_id() << ".sav');" << endl;
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
