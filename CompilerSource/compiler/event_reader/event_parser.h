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

enum e_type {
  et_inline,
  et_stacked,
  et_special,
  et_spec_sys,
  et_system,
  et_none,
  et_error
};

enum p_type {
  p2t_sprite,
  p2t_sound,
  p2t_background,
  p2t_path,
  p2t_script,
  p2t_font,
  p2t_timeline,
  p2t_object,
  p2t_room,
  p2t_key,
  p2t_error
};

#include <string>
#include <vector>
#include <map>

struct event_info
{
  string name; // The identifier-compliant name of this event.
  int    gmid; // The ID used in the popular Game Maker format.
  
  string humanname; // The name the user sees
  p_type par2type; // The type of any parameters in the name; a resource name? keyboard key?
  
  e_type mode; // Executed each step in-line? System called?
  int    mmod; // Specialization ID, or other generic integer info
  
  string def;  // Default code -- In place if nothing else is given
  string cons; // Constant code -- Added whether the event exists or not
  string super; // Check made before iteration begins
  string sub;   // Check made by each instance in each execution
  string prefix; // Inserted at the beginning of existing events
  string suffix; // Appended to the end of existing events
  string instead; // Overrides all other options: Replaces the event loop for this event
  
  string locals; // Any variables that the event requires to perform correctly
  string iterdec, iterdel, iterinit, iterrm; // Overrides iterator code
  
  event_info();
  event_info(string n,int i);
};

struct main_event_info
{
  string name; // The name of this event. Set by "Group:", defaults to first sub event name.
  bool is_group; // Whether or not this event 
  map<int,event_info*> specs;
  typedef map<int,event_info*>::iterator iter;
  main_event_info();
};

int event_parse_resourcefile();
void event_info_clear();

extern string event_get_function_name(int mid, int id);
extern string event_get_human_name(int mid, int id);
extern bool   event_has_default_code(int mid, int id);
extern string event_get_default_code(int mid, int id);
extern bool   event_has_instead(int mid, int id);
extern string event_get_instead(int mid, int id);
extern bool   event_has_super_check(int mid, int id);
extern string event_get_super_check_condition(int mid, int id);
extern string event_get_super_check_function(int mid, int id);
extern bool   event_has_sub_check(int mid, int id);
extern string event_get_sub_check_condition(int mid, int id);
extern bool   event_has_const_code(int mid, int id);
extern string event_get_const_code(int mid, int id);
extern bool   event_has_prefix_code(int mid, int id);
extern string event_get_prefix_code(int mid, int id);
extern bool   event_has_suffix_code(int mid, int id);
extern string event_get_suffix_code(int mid, int id);
extern bool   event_execution_uses_default(int mid, int id);
bool event_is_instance(int mid, int id);
string event_stacked_get_root_name(int mid);

string event_forge_sequence_code(int mid,int id, string preferred_name);

// This is implemented in write_defragged_events.
extern bool   event_used_by_something(string name);

typedef pair<int, int> evpair;
extern  vector<evpair> event_sequence;


bool event_has_iterator_declare_code(int mid, int id);
bool event_has_iterator_initialize_code(int mid, int id);
bool event_has_iterator_unlink_code(int mid, int id);
bool event_has_iterator_delete_code(int mid, int id);
string event_get_iterator_declare_code(int mid, int id);
string event_get_iterator_initialize_code(int mid, int id);
string event_get_iterator_unlink_code(int mid, int id);
string event_get_iterator_delete_code(int mid, int id);

string event_get_locals(int mid, int id);
