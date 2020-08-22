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

#include "settings.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>

#include "backend/ideprint.h"

using namespace std;


#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/GameData.h"
#include "compiler/compile_common.h"

#include "event_reader/event_parser.h"
#include "languages/lang_CPP.h"


int lang_CPP::compile_writeDefraggedEvents(
    const GameData &game, const std::set<EventGroupKey> &used_events,
    const ParsedObjectVec &parsed_objects) {
  ofstream wto((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_evparent.h").u8string().c_str());
  wto << license;

  //Write timeline/moment names. Timelines are like scripts, but we don't have to worry about arguments or return types.
  for (size_t i = 0; i < game.timelines.size(); i++) {
    for (int j = 0; j < game.timelines[i]->moments().size(); j++) {
      wto << "void TLINE_" << game.timelines[i].name << "_MOMENT_"
          << game.timelines[i]->moments(j).step() <<"();\n";
    }
  }
  wto <<"\n";

  /* Now we forge a top-level tier for object declaration
  ** that defines default behavior for any obect's unused events.
  *****************************************************************/
  wto << "namespace enigma" << endl << "{" << endl;

  wto << "  struct event_parent: " << system_get_uppermost_tier() << endl;
  wto << "  {" << endl;
  for (const auto &event : used_events) {
    const string fname = event.FunctionName();
    const bool e_is_inst = event.IsStacked();
    const bool e_has_dispatch = event.HasDispatcher();
    if (event.HasSubCheck() && !e_is_inst) {
      if (event.HasLocalDeclarations()) {
        wto << "    virtual bool myevent_" << fname
            << "_subcheck() { return false; }";
      } else {
        wto << "    bool myevent_" << fname << "_subcheck() ";
        if (event.HasSubCheckFunction()) {
          wto << event.SubCheckFunction() << "\n";
        } else {
          wto << "{\n  return " << event.SubCheckExpression() << ";\n}\n";
        }
      }
    }
    const bool e_is_void = e_is_inst || e_has_dispatch;
    wto << "    virtual " << (e_is_void ? "void" : "variant")
        << " myevent_" << fname << (e_has_dispatch ? "_dispatcher()" : "()");
    if (event.HasDefaultCode()) {
      wto << " {" << endl << "  " << event.DefaultCode() << endl
          << (e_is_void ? "    }" : "      return 0;\n    }") << endl;
    } else {
      wto << (e_is_void ? " { } // No default " : " { return 0; } // No default ")
          << event.HumanName() << " code." << endl;
    }
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


  /* Now we writes an initializer function for the whole system. This allocates
  ** event iterator queue heads and populates some metadata for error reporting.
  *****************************************************************************/
  wto.open((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_events.h").u8string().c_str());
  wto << license;
  wto << "namespace enigma" << endl << "{" << endl;

  // Start by defining storage locations for our event lists to iterate.
  for (const auto &event : used_events)
    wto << "  event_iter *event_" << event.FunctionName() << ";" << endl;

  // Here's the initializer
  wto << "  int event_system_initialize()" << endl << "  {" << endl;
  wto << "    events = new event_iter[" << used_events.size() << "]; // Allocated here; not really meant to change." << endl;

  int obj_high_id = 0;
  for (parsed_object *obj : parsed_objects) {
    if (obj->id > obj_high_id) obj_high_id = obj->id;
  }
  wto << "    objects = new objectid_base[" << (obj_high_id+1) << "]; // Allocated here; not really meant to change." << endl;

  int ind = 0;
  for (const auto &event : used_events) {
    const string evname = event.FunctionName();
    wto << "    event_" << evname << " = events + " << ind++ << ";\n"
        << "    event_" << evname << "->name = \"" << event.HumanName() << "\";"
           "\n\n";
  }
  wto << "    return 0;" << endl;
  wto << "  }" << endl;

  // Game setting initaliser
  wto << "  int game_settings_initialize() {" << endl;
  // This should only effect texture interpolation if it has not already been enabled
  if (!game.settings.general().show_cursor())
    wto << "    window_set_cursor(cr_none);" << endl;

  if (game.settings.windowing().stay_on_top())
    wto << "    window_set_stayontop(true);" << endl;

  wto << "    return 0;" << endl;
  wto << "  }" << endl;

  wto << "  variant ev_perf(int type, int numb) {\n"
         "    return ((enigma::event_parent*) instance_event_iterator->inst)->myevents_perf(type, numb);\n"
         "  }\n";

  /* Some Super Checks are more complicated than others, requiring a function. Export those functions here. */
  for (const auto &event : used_events) {
    if (event.HasSuperCheckFunction()) {
      wto << "  static inline bool supercheck_" << event.FunctionName()
          << "() " << event.SuperCheckFunction() << "\n\n";
    }
  }

  /* Now for the grand finale:  the actual event sequence.
  *****************************************************************************/
  wto << "  int ENIGMA_events()" << endl << "  {" << endl;
  for (const EventGroupKey &event : used_events) {
    if (!event.UsesEventLoop()) continue;

    string base_indent =  string(4, ' ');
    bool callsubcheck =   event.HasSubCheck()   && !event.IsStacked();
    bool emitsupercheck = event.HasSuperCheck() && !event.IsStacked();
    const string fname =  event.FunctionName();

    if (((EventDescriptor&) event).HasInsteadCode()) {
      wto << base_indent << event.InsteadCode();
    } else {
      if (emitsupercheck) {
        if (event.HasSuperCheckExpression()) {
          wto << base_indent << "if (" << event.SuperCheckExpression() << ")\n";
        } else {
          wto << base_indent << "if (myevent_" << fname + "_supercheck())\n";
        }
      }
      wto <<   base_indent << "  for (instance_event_iterator = event_" << fname << "->next; instance_event_iterator != NULL; instance_event_iterator = instance_event_iterator->next) {\n";
      if (callsubcheck) {
        wto << base_indent << "    if (((enigma::event_parent*)(instance_event_iterator->inst))->myevent_" << fname << "_subcheck()) {\n";
      }

      // Invoke the actual event function (or its dispatcher).
      wto <<   base_indent << "      ((enigma::event_parent*) (instance_event_iterator->inst))->myevent_" << fname;
      if (event.HasDispatcher()) {
        wto << "_dispatcher();\n";
      } else {
        wto << "();\n";
      }

      if (callsubcheck) {
        wto << base_indent << "    }\n";
      }
      wto <<   base_indent << "    if (enigma::room_switching_id != -1) goto after_events;\n"
          <<   base_indent << "  }\n";
    }
    wto <<     base_indent << endl
        <<     base_indent << "enigma::update_globals();" << endl
        <<     base_indent << endl;
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

  // Done, end the namespace
  wto << "} // namespace enigma" << endl;
  wto.close();

  return 0;
}
