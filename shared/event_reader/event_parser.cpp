/** Copyright (C) 2008-2018 Josh Ventura
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


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
using namespace std;
#define tostring to_string

#include "event_parser.h"
#include "ProtoYaml/proto-yaml.h"

inline bool lc(const string &s, const string &ss)
{
  const size_t l = s.length();
  if (l != ss.length())
    return false;
  for (size_t i=0; i<l; i++)
    if (  (s[i]>='A'&&s[i]<='Z'?s[i]-('A'-'a'):s[i])  !=  (ss[i]>='A'&&ss[i]<='Z'?ss[i]-('A'-'a'):ss[i])  )
      return false;
  return true;
}

const char *p_type_strs[] = {
  "Non-resource",
  "Sprite",
  "Sound",
  "Background",
  "Path",
  "Script",
  "Font",
  "Timeline",
  "Object",
  "Room",
};

using buffers::config::EventFile;
using buffers::config::EventDescriptor;

const Event *EventData::get_event(int mid, int sid) const {
  auto it = compatability_mapping_.find({mid, sid});
  if (it == compatability_mapping_.end()) return nullptr;
  return &it->second;
}

EventData::EventData(EventFile &&events): event_file_(std::move(events)) {
  for (const auto &aliases : event_file_.aliases()) {
    for (const buffers::config::ParameterAlias &alias : aliases.aliases()) {
      parameter_values_.insert({{aliases.id(), alias.id()}, &alias});
    }
  }
}

static EventData *legacy_events = nullptr;

// Used by the rest of these functions
static inline const Event *event_access(int mid, int id) {
  if (!legacy_events) return nullptr;
  return legacy_events->get_event(mid, id);
}

// Query for a name suitable for use as an identifier. No spaces or specials.
string event_get_function_name(int mid, int id) {
  if (const auto *ev = event_access(mid, id))
    return ev->FunctionName();
  return "undefinedEvent" + std::to_string(mid)
                    + "_" + std::to_string(id) + "ERROR";
}

// Query for a root (stacked event) name suitable for use as an identifier.
string event_stacked_get_root_name(int mid) {
  if (const auto *ev = event_access(mid, 0))
    return ev->BaseFunctionName();
  return "undefinedMainEvent" + std::to_string(mid) + "ERROR";
}

// Fetch a user-friendly name for the event
// with the given credentials.
string event_get_human_name(int mid, int id) {
  if (const auto *ev = event_access(mid, 0))
    return ev->HumanName();
  return "undefined or unsupported event (" + std::to_string(mid) + ", "
                                            + std::to_string(id) + ")";
}

// Test whether there is code that will remain
// active if a user has not declared this event.
bool event_has_default_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id))
    return ev->HasDefaultCode();
  return false;
}

string event_get_default_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id))
    return ev->DefaultCode();
  return "";
}


// Test whether this event has code that will remain active regardless of
// whether a user has declared this event.
bool event_has_const_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id))
    return ev->HasConstantCode();
  return false;
}

string event_get_const_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id))
    return ev->ConstantCode();
  return "";
}

// Some events have special behavior as placeholders, instead of simple iteration.
// These two functions will test for and return such.

bool event_has_instead(int mid, int id) {
  if (const auto *ev = event_access(mid, id))
    return ev->HasInsteadCode();
  return false;
}

string event_get_instead(int mid, int id) {
  if (const auto *ev = event_access(mid, id))
    return ev->InsteadCode();
  return "";
}

bool event_has_prefix_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id))
    return ev->HasPrefixCode();
  return false;
}

string event_get_prefix_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id))
    return ev->PrefixCode();
  return "";
}

bool event_has_suffix_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id))
    return ev->HasSuffixCode();
  return false;
}

string event_get_suffix_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id))
    return ev->SuffixCode();
  return "";
}

// Many events check things before executing, some before starting the loop. Deal with them.

bool event_has_sub_check(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->HasSubCheck();
  return false;
}

bool event_has_super_check(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->HasSuperCheck();
  return false;
}

bool event_has_super_check_condition(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->HasSuperCheckExpression();
  return false;
}

string event_get_super_check_function(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->SuperCheckExpression();
  return "";
}

bool event_has_iterator_declare_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->HasIteratorDeclareCode();
  return false;
}
bool event_has_iterator_initialize_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->HasIteratorInitializeCode();
  return false;
}
bool event_has_iterator_unlink_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->HasIteratorRemoveCode();
  return false;
}
bool event_has_iterator_delete_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->HasIteratorDeleteCode();
  return false;
}
string event_get_iterator_declare_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->IteratorDeclareCode();
  return "";
}
string event_get_iterator_initialize_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->IteratorInitializeCode();
  return "";
}
string event_get_iterator_unlink_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->IteratorRemoveCode();
  return "";
}
string event_get_iterator_delete_code(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->IteratorDeleteCode();
  return "";
}

string event_get_locals(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->LocalDeclarations();
  return "";
}

// This is the fucking compiler oversharing. I don't know how this happened,
// but this method always returns a function *body*. Not even a full signature.
// JUST the body. And it's still named identically to the above.
// TODO: When deleting this method, rewire the compiler to actually
// *conditionally* emit a subcheck function, and otherwise embed the
// conditional.
string event_get_sub_check_condition(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->SubCheckFunction();
  return "";
}

// Does this event belong on the list of events to execute?
bool event_execution_uses_default(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->UsesEventLoop();
  return false;
}

// Clear all data from previous parses, save
// main events, which can just be overwritten.
void event_info_clear() {
  delete legacy_events;
  legacy_events = nullptr;
}

// Returns if the event with the given ID pair is an instance of a stacked event
bool event_is_instance(int mid, int id) {
  if (const auto *ev = event_access(mid, id)) return ev->parameters.size();
  return false;
}

string event_forge_sequence_code(int mid, int id, string preferred_name) {
  string base_indent = string(4, ' ');
  const Event *const ev = event_access(mid,id);
  if (!ev) return "#error Invalid event (" + std::to_string(mid) + ", " + std::to_string(id) + "\n";

  if (ev->HasInsteadCode())
    return base_indent + ev->InsteadCode() + "\n\n";

  if (event_execution_uses_default(mid,id))
  {
    string ret = "";
    bool perfsubcheck = event_has_sub_check(mid, id) && !event_is_instance(mid, id);
    if (event_has_super_check(mid,id) and !event_is_instance(mid,id)) {
      ret =        base_indent + "if (" + event_get_super_check_condition(mid,id) + ")\n" +
                   base_indent + "  for (instance_event_iterator = event_" + preferred_name + "->next; instance_event_iterator != NULL; instance_event_iterator = instance_event_iterator->next) {\n";
      if (perfsubcheck) { ret += "    if (((enigma::event_parent*)(instance_event_iterator->inst))->myevent_" + preferred_name + "_subcheck()) {\n"; }
      ret +=       base_indent + "      ((enigma::event_parent*)(instance_event_iterator->inst))->myevent_" + preferred_name + "();\n";
      if (perfsubcheck) { ret += "    }\n"; }
      ret +=       base_indent + "    if (enigma::room_switching_id != -1) goto after_events;\n" +
                   base_indent + "  }\n";
    } else {
       ret =  base_indent + "for (instance_event_iterator = event_" + preferred_name + "->next; instance_event_iterator != NULL; instance_event_iterator = instance_event_iterator->next) {\n";
       if (perfsubcheck) { ret += "    if (((enigma::event_parent*)(instance_event_iterator->inst))->myevent_" + preferred_name + "_subcheck()) {\n"; }
       ret += base_indent + "  ((enigma::event_parent*)(instance_event_iterator->inst))->myevent_" + preferred_name + "();\n";
       if (perfsubcheck) { ret += "    }\n"; }
       ret += base_indent + "  if (enigma::room_switching_id != -1) goto after_events;\n" +
              base_indent + "}\n";
    }
    return ret;
  }
  return "";
}

google::protobuf::RepeatedPtrField<Event> event_execution_order() {
  return legacy_events->events();
}

const Event* translate_legacy_id_pair(int mid, int id) {
  return event_access(mid, id);
}

/*
int main(int,char**)
{
  int a = event_parse_resourcefile();
  if (a) printf("Event Parse: Error %d\n",a);
  else   puts  ("Event Parse: completed successfully");

  for (size_t i=0; i<main_event_infos.size; i++)
  {
    printf("%c %s\n",main_event_infos[i].is_group ? ']' : '+', main_event_infos[i].name.c_str());
    for (main_event_info::iter ii = main_event_infos[i].specs.begin(); ii != main_event_infos[i].specs.end(); )
      printf("%s── [%02d] %-25s [%s]\n",++ii != main_event_infos[i].specs.end()?"├":"╰",ii->first,ii->second->humanname.c_str(),ii->second->name.c_str());
  }
}
*/
