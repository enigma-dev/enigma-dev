/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008-2020 Josh Ventura                                        **
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

#ifndef ENIGMA_EVENT_PARSER_H
#define ENIGMA_EVENT_PARSER_H

#include <Configuration/EventDescriptor.pb.h>
#include <boost/container/small_vector.hpp>

#include <string>

struct LegacyEventPair {
  int mid, id;
  operator std::pair<int, int>() const { return {mid, id}; }
};

// C++ wrapper around the EventDescriptor proto.
// Implements basic introspection functions.
struct EventDescriptor {
  // Raw data stored in our event file proto.
  const buffers::config::EventDescriptor *event;
  // An ID arbitrarily assigned to this event for identification purposes
  // during a particular compilation. These IDs are not stable, but can be
  // used to test two events for being of the same kind.
  int internal_id;

  EventDescriptor(const buffers::config::EventDescriptor *ev, int iid):
      event(ev), internal_id(iid) {}

  // Returns whether this event is an instance of a parameterized event.
  bool IsParameterized() const;
  // Returns whether this event is a "stacked" event—a parameterized event whose
  // different parameterizations are fired at the same time.
  bool IsStacked() const;

  // Returns human-readable examples of ID strings belonging to this event.
  std::string ExampleIDStrings() const;

  std::string HumanName() const;
  std::string BaseFunctionName() const;
  std::string LocalDeclarations() const;

  bool HasPrefixCode() const { return event->has_prefix(); }
  bool HasSuffixCode() const { return event->has_suffix(); }
  bool HasDefaultCode() const { return event->has_default_() || HasConstantCode(); }
  bool HasConstantCode() const { return event->has_constant(); }

  std::string DefaultCode() const;
  std::string ConstantCode() const;

  bool HasSubCheck() const { return event->has_sub_check(); }
  bool HasSubCheckFunction() const;
  bool HasSubCheckExpression() const;
  bool HasSuperCheck() const { return event->has_super_check(); }
  bool HasSuperCheckFunction() const;
  bool HasSuperCheckExpression() const;
  bool HasInsteadCode() const { return event->has_instead(); }

  std::string InsteadCode() const;

  bool HasIteratorDeclareCode()    const { return event->has_iterator_declare(); }
  bool HasIteratorInitializeCode() const { return event->has_iterator_initialize(); }
  bool HasIteratorRemoveCode()     const { return event->has_iterator_remove(); }
  bool HasIteratorDeleteCode()     const { return event->has_iterator_delete(); }

  std::string IteratorDeclareCode() const;
  std::string IteratorInitializeCode() const;
  std::string IteratorRemoveCode() const;
  std::string IteratorDeleteCode() const;

  // Returns whether this event will be included in the main event loop.
  // This will be true except for events marked as System events.
  bool UsesEventLoop() const;

  // Explicitly checks this event for validity.
  bool IsValid() const;
};

struct Event : EventDescriptor {
  struct Argument {
    std::string name;
    std::string spelling;

    // TODO: Delete these... use {.name = name, .spelling = spelling}
    Argument() = default;
    Argument(std::string name_, std::string spelling_):
        name(std::move(name_)), spelling(std::move(spelling_)) {}
  };
  // Any arguments to this event, using EGM spelling.
  boost::container::small_vector<Argument, 4> arguments;

  // Construct with a base EventDescriptor.
  explicit Event(const EventDescriptor &edesc): EventDescriptor(edesc) {}

  // Represents this Event as a unique string, including any parameters.
  std::string IdString() const;

  // Formats any arguments into the human name returned by EventDescriptor.
  std::string HumanName() const;

  // Returns a function name specific to this instance of an event.
  // Identical to BaseFunctionName for non-parameterized events.
  std::string FunctionName() const;

  std::string PrefixCode() const;
  std::string SuffixCode() const;

  // Returns whether all parameters of this event are set.
  bool IsComplete() const;
  // This should have been obtained from the EventDescriptor.
  // It's not applicable to a user event.
  bool HasInsteadCode() const = delete;

  std::string SubCheckFunction() const;
  std::string SubCheckExpression() const;
  std::string SuperCheckFunction() const;
  std::string SuperCheckExpression() const;

  bool operator==(const Event &other) const;
  bool operator<(const Event &other) const;

 private:
  // Replaces %1 with parameter 1, %2 with parameter 2, etc.
  std::string ParamSubst(const std::string &str) const;
};

class EventData {
 public:
  // Look up an event by its legacy ID pair.
  const Event get_event(int mid, int sid) const;
  // Look up a legacy ID pair for a non-parameterized event.
  LegacyEventPair reverse_get_event(const EventDescriptor &) const;
  // Look up a legacy ID pair for an event.
  LegacyEventPair reverse_get_event(const Event &) const;
  // Retrieve the sequence of all declared events.
  const std::vector<EventDescriptor> &events() const { return event_wrappers_; }

  // Decodes an Event ID string, such as Keyboard[Left], into an Event object.
  Event DecodeEventString(const std::string &evstring);

  EventData(buffers::config::EventFile&&);

 private:
  // Raw data read from the event configuration.
  buffers::config::EventFile event_file_;
  // Map of type and constant name pair (eg, {"key", "enter"}) to alias info.
  std::map<std::pair<std::string, std::string>,
           const buffers::config::ParameterAlias*> parameter_values_;
  // Wraps the `EventDescriptor`s read in from the events file.
  std::vector<EventDescriptor> event_wrappers_;
  // Index over event ID strings.
  std::map<std::string, const EventDescriptor*> event_index_;
  // Index over event Internal IDs.
  std::map<int, const EventDescriptor*> event_iid_index_;

  // Legacy shit.

  // Index into the event file by Game Maker ID pair.
  std::map<std::pair<int, int>, Event> compatability_mapping_;
  struct SubEventCollection {
    int main_id;
    std::map<Event, int> subevents;
  };
  // Reverse of the above index, keyed by internal_id.
  std::map<int, SubEventCollection> hacky_reverse_mapping_;
};

// Reads the given file in as an EventFile proto.
// This is just a convenience method around ReadYamlFileAs<EventFile>().
buffers::config::EventFile ParseEventFile(std::istream &file);

// Reads the given file in as an EventFile proto.
// This is just a convenience method around ReadYamlFileAs<EventFile>().
buffers::config::EventFile ParseEventFile(const std::string &filename);

// TODO: Delete all methods below this line.
// Let an object own this state instead of relying on statics.

using std::string;
extern string event_get_function_name(int mid, int id);
extern string event_get_human_name(int mid, int id);
extern bool   event_has_default_code(int mid, int id);
extern string event_get_default_code(int mid, int id);
extern bool   event_has_instead(int mid, int id);
extern string event_get_instead(int mid, int id);
extern bool   event_has_super_check(int mid, int id);
extern string event_get_super_check_condition(int mid, int id);
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

bool event_has_iterator_declare_code(int mid, int id);
bool event_has_iterator_initialize_code(int mid, int id);
bool event_has_iterator_unlink_code(int mid, int id);
bool event_has_iterator_delete_code(int mid, int id);
string event_get_iterator_declare_code(int mid, int id);
string event_get_iterator_initialize_code(int mid, int id);
string event_get_iterator_unlink_code(int mid, int id);
string event_get_iterator_delete_code(int mid, int id);

string event_get_locals(int mid, int id);

void event_parse_resourcefile();
void event_info_clear();

const std::vector<EventDescriptor> &event_declarations();
const Event translate_legacy_id_pair(int mid, int id);

LegacyEventPair reverse_lookup_legacy_event(const Event &e);
LegacyEventPair reverse_lookup_legacy_event(const EventDescriptor &e);

#endif // ENIGMA_EVENT_PARSER_H
