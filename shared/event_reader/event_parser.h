/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008-2018 Josh Ventura                                        **
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

// TODO: Move this to a common header with GameData as a ResourceType enum.
// We don't currently have a generic resource metadata specifier, and it
// probably doesn't make sense given how different resources are from one
// another. Another consideration is to use TreeNode::TypeCase, but it includes
// settings nodes and folder nodes.
enum p_type {
  p2t_non_resource,
  p2t_sprite,
  p2t_sound,
  p2t_background,
  p2t_path,
  p2t_script,
  p2t_font,
  p2t_timeline,
  p2t_object,
  p2t_room
};

struct Event {
  // Raw data stored in our event file proto.
  const buffers::config::EventDescriptor *event;
  // Any parameters to this event.
  boost::container::small_vector<std::string, 4> parameters;

  bool is_complete() const {
    return parameters.size() == (unsigned) event->parameters_size();
  }

  std::string HumanName() const;
  std::string FunctionName() const;
  std::string BaseFunctionName() const;
  std::string LocalDeclarations() const;

  bool HasPrefixCode() const { return event->has_prefix(); }
  bool HasSuffixCode() const { return event->has_suffix(); }
  bool HasDefaultCode() const { return event->has_default_() || HasConstantCode(); }
  bool HasConstantCode() const { return event->has_constant(); }

  std::string PrefixCode() const;
  std::string SuffixCode() const;
  std::string DefaultCode() const;
  std::string ConstantCode() const;

  bool HasSubCheck() const { return event->has_sub_check(); }
  bool HasSubCheckFunction() const;
  bool HasSubCheckExpression() const;
  bool HasSuperCheck() const { return event->has_super_check(); }
  bool HasSuperCheckFunction() const;
  bool HasSuperCheckExpression() const;
  bool HasInsteadCode() const { return event->has_instead(); }

  std::string SubCheckFunction() const;
  std::string SubCheckExpression() const;
  std::string SuperCheckFunction() const;
  std::string SuperCheckExpression() const;
  std::string InsteadCode() const;

  bool HasIteratorDeclareCode()    const { return event->has_iterator_declare(); }
  bool HasIteratorInitializeCode() const { return event->has_iterator_initialize(); }
  bool HasIteratorRemoveCode()     const { return event->has_iterator_remove(); }
  bool HasIteratorDeleteCode()     const { return event->has_iterator_delete(); }

  const std::string IteratorDeclareCode() const;
  const std::string IteratorInitializeCode() const;
  const std::string IteratorRemoveCode() const;
  const std::string IteratorDeleteCode() const;

  // Returns whether this event will be included in the main event loop.
  // This will be true except for events marked as System events.
  bool UsesEventLoop() const;
};

class EventData {
 public:
  // Look up an event by its legacy ID pair.
  const Event *get_event(int mid, int sid) const;

  EventData(buffers::config::EventFile&&);

 private:
  // Raw data read from the event configuration.
  buffers::config::EventFile event_file_;
  // Index into the event file by Game Maker ID pair.
  std::map<std::pair<int, int>, Event> compatability_mapping_;
  // Map of type and constant name pair (eg, {"key", "enter"}) to alias info.
  std::map<std::pair<std::string, std::string>,
           const buffers::config::ParameterAlias*> parameter_values_;
};

// TODO: Delete all methods below this line.
// Let an object own this state instead of relying on statics.

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

bool event_has_iterator_declare_code(int mid, int id);
bool event_has_iterator_initialize_code(int mid, int id);
bool event_has_iterator_unlink_code(int mid, int id);
bool event_has_iterator_delete_code(int mid, int id);
string event_get_iterator_declare_code(int mid, int id);
string event_get_iterator_initialize_code(int mid, int id);
string event_get_iterator_unlink_code(int mid, int id);
string event_get_iterator_delete_code(int mid, int id);

string event_get_locals(int mid, int id);

string event_forge_sequence_code(int mid,int id, string preferred_name);

void event_parse_resourcefile();
void event_info_clear();

google::protobuf::RepeatedPtrField<Event> event_execution_order();
const Event* translate_legacy_id_pair(int mid, int id);

#endif // ENIGMA_EVENT_PARSER_H
