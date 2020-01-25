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


#include "event_parser.h"

#include <strings_util.h>

#include <ProtoYaml/proto-yaml.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>


// Note: there are two kinds of legacy in this file.
//
// 1. Legacy event identifiers, from the days of yore. These will forever exist
//    in the wild, and so we can expect to keep the logic to *read* these.
//
// 2. Legacy code in ENIGMA. As of this writing, the compiler still maintains
//    a static copy of event data, rather than instantiating event information
//    in a compilation context. Code to handle this will be deleted in time.

// Good luck.

constexpr int kMinInternalID = 1000;
constexpr int kParameterizedSubId = -1;

// Checks if the given string is an "Expression," which is defined, for the
// purpose of the events file, as anything not starting with a brace.
static bool IsExpression(const std::string &str) {
  return !str.empty() && str[0] != '{';
}

// -----------------------------------------------------------------------------
// Safety mechanisms -----------------------------------------------------------
// -----------------------------------------------------------------------------

namespace cb = buffers::config;
using cb::EventFile;

static cb::EventDescriptor MakeSentinelDescriptor() {
  cb::EventDescriptor res;
  res.set_id("InvalidEvent");
  res.set_type(cb::EventDescriptor::SYSTEM);
  return res;
}
static cb::EventDescriptor kSentinelDescriptor = MakeSentinelDescriptor();
static EventDescriptor kSentinelEventDesc(&kSentinelDescriptor, -1);
static Event kSentinelEvent(kSentinelEventDesc);

bool EventDescriptor::IsValid() const { return event != &kSentinelDescriptor; }


// End safety mechanisms -------------------------------------------------------
// -----------------------------------------------------------------------------
// Begin external parsing API --------------------------------------------------

static EventData *legacy_events = nullptr;

EventFile ParseEventFile(std::istream &file) {
  EventFile res = egm::ReadYamlAs<EventFile>(file);
  // This is a nasty hack to keep the static data current while new systems
  // migrate to use a proper EventData constructor. I expect a CompileContext
  // class to come to be, and to contain an EventData, parsed from either the
  // default events.ey or an events.ey contained within the game being built.
  delete legacy_events;
  legacy_events = new EventData(EventFile(res));
  return res;
}
EventFile ParseEventFile(const std::string &filename) {
  EventFile res =  egm::ReadYamlFileAs<EventFile>(filename);
  delete legacy_events;
  legacy_events = new EventData(EventFile(res));
  return res;
}

void event_parse_resourcefile() {
  // The side-effects of these routines will take care of the rest.
  ParseEventFile("events.ey");
}


// End initialization calls ----------------------------------------------------
// -----------------------------------------------------------------------------
// Begin class method implementations ------------------------------------------

Event EventData::DecodeEventString(const std::string &evstring) {
  std::string id;
  std::vector<std::string> args;
  size_t at = 0, lbracket;
  while ((lbracket = evstring.find_first_of('[', at)) != std::string::npos) {
    id += evstring.substr(at, lbracket - at);
    size_t rbracket = evstring.find_first_of(']', ++lbracket);
    if (rbracket != std::string::npos) {
      args.push_back(evstring.substr(lbracket, rbracket - lbracket));
      at = rbracket + 1;
    } else {
      at = evstring.length();
      break;
    }
  }
  id += evstring.substr(at);
  auto evi = event_index_.find(ToLower(id));
  if (evi == event_index_.end() || !evi->second) {
    std::cerr << "EVENT ERROR: Event `" << id << "` is not known to the system\n";
    return Event(kSentinelEvent);
  }

  const EventDescriptor &base_event = *evi->second;
  Event res(base_event);
  size_t correct_arg_count = base_event.event->parameters_size();
  if (args.size() != correct_arg_count) {
    std::cerr << "EVENT ERROR: Wrong number of arguments given for event " << id
              << ": wanted " << correct_arg_count << ", got " << args.size()
              << std::endl;
    args.resize(correct_arg_count, "0");
  }
  for (size_t argn = 0; argn < correct_arg_count; ++argn) {
    const std::string &arg = args[argn];
    const std::string &arg_kind = base_event.event->parameters(argn);
    auto pv = parameter_values_.find({arg_kind, ToLower(arg)});
    if (pv == parameter_values_.end()) {
      // Assume that spelling == name (this is the case for resource names/ints)
      res.arguments.emplace_back(arg, arg);
    } else {
      res.arguments.emplace_back(pv->second->id(), pv->second->spelling());
    }
  }
  return res;
}

std::string Event::ParamSubst(const std::string &str) const {
  std::string res;
  size_t start = 0;
  size_t pc = str.find_first_of('%');
  if (pc == std::string::npos) return str;
  while (pc != std::string::npos) {
    if (++pc < str.length() && str[pc] >= '1' && str[pc] <= '9' &&
        str[pc] <= '0' + (int) arguments.size()) {
      int pnum = str[pc] - '1';
      res += str.substr(start, pc - 1 - start);
      res += arguments[pnum].spelling;
      start = ++pc;
    } else {
      std::cerr << "EVENT ERROR: Ignoring junk parameter " << str[pc] << std::endl;
    }
    pc = str.find_first_of('%', pc);
  }
  res += str.substr(start);
  return res;
}

EventData::EventData(EventFile &&events): event_file_(std::move(events)) {
  for (const auto &aliases : event_file_.aliases()) {
    for (const buffers::config::ParameterAlias &alias : aliases.aliases()) {
      parameter_values_.insert({{aliases.id(), ToLower(alias.id())}, &alias});
    }
  }
  // Start numbering internal IDs in the new system from 1000, for good measure.
  for (long i = 0; i < event_file_.events_size(); ++i) {
    event_wrappers_.emplace_back(&event_file_.events(i), kMinInternalID + i);
  }
  for (const EventDescriptor &event_wrapper : event_wrappers_) {
    const int iid = event_wrapper.internal_id;
    const string evid = StripChar(event_wrapper.event->id(), '.');
    if (!event_index_.insert({ToLower(evid), &event_wrapper}).second) {
      std::cerr << "EVENT ERROR: Duplicate event ID " << evid << std::endl;
    }
    if (!event_wrapper.IsInstance() &&  // Only insert non-parameterized events.
        !event_iid_index_.insert({iid, &event_wrapper}).second) {
      std::cerr << "EVENT ERROR: Duplicate event IID " << iid
                << "; how did this even happen!?";
    }
  }

  // Now that our index is built, we can populate the legacy maps.
  for (const auto &mapping : event_file_.game_maker_event_mappings()) {
    int main_id = mapping.id();
    if (mapping.has_single()) {
      Event cev = DecodeEventString(mapping.single());
      cev.arguments.clear();
      auto insert = compatability_mapping_.insert({{main_id, 0}, cev});
      if (!insert.second) {
        std::cerr << "EVENT ERROR: Duplicate event compatability id ("
                  << main_id << ")\n";
      }
      hacky_reverse_mapping_[cev.internal_id].main_id = main_id;
    } else if (mapping.has_parameterized()) {
      Event cev = DecodeEventString(mapping.parameterized());
      cev.arguments.clear();
      auto insert =
          compatability_mapping_.insert({{main_id, kParameterizedSubId}, cev});
      if (!insert.second) {
        std::cerr << "EVENT ERROR: Duplicate event compatability id ("
                  << main_id << ")\n";
      }
      hacky_reverse_mapping_[cev.internal_id].main_id = main_id;
    } else if (mapping.has_specialized()) {
      for (const auto &ev_case : mapping.specialized().cases()) {
        const int sub_id = ev_case.first;
        Event cev = DecodeEventString(ev_case.second);
        auto insert =
            compatability_mapping_.insert({{main_id, ev_case.first}, cev});
        if (!insert.second) {
          std::cerr << "EVENT ERROR: Duplicate event compatability case ("
                    << main_id << ", " << sub_id << ")\n";
        }
        SubEventCollection &sec = hacky_reverse_mapping_[cev.internal_id];
        sec.main_id = main_id;
        sec.subevents[cev] = sub_id;
      }
    }
  }
}

const Event EventData::get_event(int mid, int sid) const {
  auto it = compatability_mapping_.find({mid, sid});
  if (it == compatability_mapping_.end()) {
    it = compatability_mapping_.find({mid, kParameterizedSubId});
    if (it == compatability_mapping_.end()) {
      auto res = event_iid_index_.find(mid);
      if (res != event_iid_index_.end()) return Event(*res->second);
      std::cerr << "EVENT ERROR: Event (" << mid << ", " << sid
                << ") is not known to the system." << std::endl;
      return kSentinelEvent;
    }
    Event res = it->second;
    string integer = std::to_string(sid);
    res.arguments.emplace_back(integer, integer);
    return res;
  }
  return it->second;
}


bool Event::IsComplete() const {
  return arguments.size() == (unsigned) event->parameters_size();
}
bool EventDescriptor::IsInstance() const {
  return event->parameters_size();
}

std::string EventDescriptor::ExampleIDStrings() const {
  Event example(*this);
  for (const string &p : event->parameters()) {
    example.arguments.push_back({p, p});
  }
  return example.IdString();
}

std::string EventDescriptor::HumanName() const {
  return event->name();
}
std::string Event::HumanName() const {
  return ParamSubst(event->name());
}
std::string EventDescriptor::BaseFunctionName() const {
  return ToLower(StripChar(event->id(), '.'));
}
std::string EventDescriptor::LocalDeclarations() const {
  return event->locals();
}

std::string EventDescriptor::DefaultCode() const {
  return event->has_default_() ? event->default_() : event->constant();
}
std::string EventDescriptor::ConstantCode() const {
  return event->constant();
}
std::string Event::PrefixCode() const {
  return ParamSubst(event->prefix());
}
std::string Event::SuffixCode() const {
  return ParamSubst(event->suffix());
}


bool EventDescriptor::HasSubCheckFunction() const {
  return HasSubCheck() && !IsExpression(event->sub_check());
}
bool EventDescriptor::HasSubCheckExpression() const {
  return HasSubCheck() && IsExpression(event->sub_check());
}
bool EventDescriptor::HasSuperCheckFunction() const {
  return HasSuperCheck() && !IsExpression(event->super_check());
}
bool EventDescriptor::HasSuperCheckExpression() const {
  return HasSuperCheck() && IsExpression(event->super_check());
}

std::string EventDescriptor::InsteadCode() const {
  return event->instead();
}


std::string EventDescriptor::IteratorDeclareCode() const {
  return event->iterator_declare();
}
std::string EventDescriptor::IteratorInitializeCode() const {
  return event->iterator_initialize();
}
std::string EventDescriptor::IteratorRemoveCode() const {
  return event->iterator_remove();
}
std::string EventDescriptor::IteratorDeleteCode() const {
  return event->iterator_delete();
}

bool EventDescriptor::UsesEventLoop() const {
  return event->type()  != cb::EventDescriptor::SYSTEM;
}

std::string Event::SubCheckExpression() const {
  return ParamSubst(event->sub_check());
}
std::string Event::SubCheckFunction() const {
  if (HasSubCheckFunction()) return ParamSubst(event->sub_check());
  return "{ return " + ParamSubst(FirstNotEmpty(event->sub_check(), "true"))
                     + "; }";
}
std::string Event::SuperCheckFunction() const {
  if (HasSuperCheckFunction()) return ParamSubst(event->super_check());
  return "{ return " + ParamSubst(FirstNotEmpty(event->super_check(), "true"))
                     + "; }";
}
std::string Event::SuperCheckExpression() const {
  return ParamSubst(event->super_check());
}

std::string Event::FunctionName() const {
  std::string res;
  std::string ntempl = event->id();
  size_t arg = 0, at = 0, dot;
  while (arg < arguments.size() &&
         (dot = ntempl.find_first_of('.', at)) != std::string::npos) {
    res += ToLower(ntempl.substr(at, dot - at));
    res += "_" + arguments[arg++].name + "_";
    at = dot + 1;
  }
  res += ToLower(ntempl.substr(at));
  while (arg < arguments.size()) {
    res += "_" + arguments[arg++].name;
  }
  return res;
}
std::string Event::IdString() const {
  std::string res;
  std::string ntempl = event->id();
  size_t arg = 0, at = 0, dot;
  while (arg < arguments.size() &&
         (dot = ntempl.find_first_of('.', at)) != std::string::npos) {
    res += ntempl.substr(at, dot - at);
    res += "[" + arguments[arg++].name + "]";
    at = dot + 1;
  }
  res += ntempl.substr(at);
  while (arg < arguments.size()) {
    res += "[" + arguments[arg++].name + "]";
  }
  return res;
}

bool Event::operator==(const Event &other) const {
  if (internal_id != other.internal_id) return false;
  if (arguments.size() != other.arguments.size()) return false;
  for (size_t i = 0; i < arguments.size(); ++i) {
    if (arguments[i].name != other.arguments[i].name) return false;
  }
  return true;
}

bool Event::operator<(const Event &other) const {
  if (internal_id != other.internal_id) return internal_id < other.internal_id;
  if (arguments.size() != other.arguments.size())
    return arguments.size() < other.arguments.size();
  for (size_t i = 0; i < arguments.size(); ++i) {
    if (arguments[i].name != other.arguments[i].name)
      return arguments[i].name < other.arguments[i].name;
  }
  return false;
}


// -----------------------------------------------------------------------------
// Legacy method implementations -----------------------------------------------
// -----------------------------------------------------------------------------

LegacyEventPair EventData::reverse_get_event(const EventDescriptor &ev) const {
  auto it = hacky_reverse_mapping_.find(ev.internal_id);
  if (it == hacky_reverse_mapping_.end()) {
    std::cerr << "EVENT WARNING: Event " << ev.internal_id
              << " (" << ev.HumanName() << ") not found; using own ID\n";
    return {ev.internal_id, 0};
  }
  const SubEventCollection &amap = it->second;
  auto sit = amap.subevents.find(Event(ev));
  if (sit != amap.subevents.end()) {
    return {amap.main_id, sit->second};
  }
  if (!amap.subevents.empty() || !ev.event->parameters().empty()) {
    std::cerr <<  "EVENT ERROR: Event " << ev.internal_id
              << " (" << ev.HumanName()
              << ") cannot be looked up in the compatibility map\n";
  }
  return {amap.main_id, 0};
}

// Look up a legacy ID pair for an event.
LegacyEventPair EventData::reverse_get_event(const Event &ev) const {
  auto it = hacky_reverse_mapping_.find(ev.internal_id);
  if (it == hacky_reverse_mapping_.end()) {
    std::cerr << "EVENT ERROR: Event " << ev.internal_id
              << " (" << ev.HumanName() << ") not found; using own ID\n";
    return {ev.internal_id, 0};
  }
  const SubEventCollection &amap = it->second;
  auto sit = amap.subevents.find(ev);
  if (sit != amap.subevents.end()) {
    return {amap.main_id, sit->second};
  }
  if (ev.arguments.empty() || ev.event->parameters().empty()) {
    return {amap.main_id, 0};
  }
  if (!amap.subevents.empty() || ev.arguments.size() != 1) {
    std::cerr <<  "EVENT ERROR: Event " << ev.internal_id
              << " (" << ev.HumanName() << ") missing from subevent map\n";
  }
  const std::string &arg = ev.arguments[0].name;
  const std::string &arg_kind = ev.event->parameters(0);
  auto pv = parameter_values_.find({arg_kind, ToLower(arg)});
  if (pv != parameter_values_.end()) {
    return LegacyEventPair{amap.main_id, pv->second->value()};
  }
  auto iv = SafeAtoL(arg);
  if (iv.first) return LegacyEventPair{amap.main_id, iv.second};
  std::cerr << "EVENT ERROR: Unknown " << arg_kind << " parameter value " << arg
            << ": cannot map argument to event sub-ID\n";
  return LegacyEventPair{amap.main_id, 0};
}


// End method implementations --------------------------------------------------
// -----------------------------------------------------------------------------
// Legacy shit continnues below ------------------------------------------------

// Used by the rest of these functions
static inline const Event event_access(int mid, int id) {
  if (!legacy_events) {
    std::cerr << "EVENT ERROR: Someone is querying event data without parsing events.\n";
    return kSentinelEvent;
  }
  return legacy_events->get_event(mid, id);
}

const std::vector<EventDescriptor> &event_declarations() {
  if (!legacy_events) {
    static std::vector<EventDescriptor> empty;
    return empty;
  }
  return legacy_events->events();
}

// Query for a name suitable for use as an identifier. No spaces or specials.
string event_get_function_name(int mid, int id) {
  Event ev = event_access(mid, id);
  if (ev.IsValid()) return ev.FunctionName();
  return "undefinedEvent" + std::to_string(mid)
                    + "_" + std::to_string(id) + "ERROR";
}

// Query for a root (stacked event) name suitable for use as an identifier.
string event_stacked_get_root_name(int mid) {
  Event ev = event_access(mid, 0);
  if (ev.IsValid()) return ev.BaseFunctionName();
  return "undefinedMainEvent" + std::to_string(mid) + "ERROR";
}

// Fetch a user-friendly name for the event
// with the given credentials.
string event_get_human_name(int mid, int id) {
  Event ev = event_access(mid, 0);
  if (ev.IsValid()) return ev.HumanName();
  return "undefined or unsupported event (" + std::to_string(mid) + ", "
                                            + std::to_string(id) + ")";
}

// Test whether there is code that will remain
// active if a user has not declared this event.
bool event_has_default_code(int mid, int id) {
  return event_access(mid, id).HasDefaultCode();
}

string event_get_default_code(int mid, int id) {
  return event_access(mid, id).DefaultCode();
}


// Test whether this event has code that will remain active regardless of
// whether a user has declared this event.
bool event_has_const_code(int mid, int id) {
  return event_access(mid, id).HasConstantCode();
}

string event_get_const_code(int mid, int id) {
  return event_access(mid, id).ConstantCode();
}

// Some events have special behavior as placeholders, instead of simple iteration.
// These two functions will test for and return such.

bool event_has_instead(int mid, int id) {
  return event_access(mid, id).EventDescriptor::HasInsteadCode();
}

string event_get_instead(int mid, int id) {
  return event_access(mid, id).InsteadCode();
}

bool event_has_prefix_code(int mid, int id) {
  return event_access(mid, id).HasPrefixCode();
}

string event_get_prefix_code(int mid, int id) {
  return event_access(mid, id).PrefixCode();
}

bool event_has_suffix_code(int mid, int id) {
  return event_access(mid, id).HasSuffixCode();
}

string event_get_suffix_code(int mid, int id) {
  return event_access(mid, id).SuffixCode();
}

// Many events check things before executing, some before starting the loop. Deal with them.

bool event_has_sub_check(int mid, int id) {
  return event_access(mid, id).HasSubCheck();
}
bool event_has_super_check(int mid, int id) {
  return event_access(mid, id).HasSuperCheck();
}
bool event_has_super_check_condition(int mid, int id) {
  return event_access(mid, id).HasSuperCheckExpression();
}

bool event_has_iterator_declare_code(int mid, int id) {
  return event_access(mid, id).HasIteratorDeclareCode();
}
bool event_has_iterator_initialize_code(int mid, int id) {
  return event_access(mid, id).HasIteratorInitializeCode();
}
bool event_has_iterator_unlink_code(int mid, int id) {
  return event_access(mid, id).HasIteratorRemoveCode();
}
bool event_has_iterator_delete_code(int mid, int id) {
  return event_access(mid, id).HasIteratorDeleteCode();
}
string event_get_iterator_declare_code(int mid, int id) {
  return event_access(mid, id).IteratorDeclareCode();
}
string event_get_iterator_initialize_code(int mid, int id) {
  return event_access(mid, id).IteratorInitializeCode();
}
string event_get_iterator_unlink_code(int mid, int id) {
  return event_access(mid, id).IteratorRemoveCode();
}
string event_get_iterator_delete_code(int mid, int id) {
  return event_access(mid, id).IteratorDeleteCode();
}

string event_get_locals(int mid, int id) {
  return event_access(mid, id).LocalDeclarations();
}

// This is the fucking compiler oversharing. I don't know how this happened,
// but this method always returns a function *body*. Not even a full signature.
// JUST the body. And it's still named identically to the above.
// TODO: When deleting this method, rewire the compiler to actually
// *conditionally* emit a subcheck function, and otherwise embed the
// conditional.
string event_get_sub_check_condition(int mid, int id) {
  return event_access(mid, id).SubCheckFunction();
}

string event_get_super_check_condition(int mid, int id) {
  return event_access(mid, id).SuperCheckExpression();
}

// Does this event belong on the list of events to execute?
bool event_execution_uses_default(int mid, int id) {
  return event_access(mid, id).UsesEventLoop();
}

// Clear all data from previous parses, save
// main events, which can just be overwritten.
void event_info_clear() {
  delete legacy_events;
  legacy_events = nullptr;
}

// Returns if the event with the given ID pair is an instance of a stacked event
bool event_is_instance(int mid, int id) {
  return event_access(mid, id).IsInstance();
}

const std::vector<EventDescriptor> &event_execution_order() {
  return legacy_events->events();
}

const Event translate_legacy_id_pair(int mid, int id) {
  return event_access(mid, id);
}

// This method exists to prolong the life of robertmap, which is prohibitively
// inscrutable. Delete this method after deleting that entire system or
// replacing its keys/values with the new Event ids.
LegacyEventPair reverse_lookup_legacy_event(const Event &ev) {
  return legacy_events->reverse_get_event(ev);
}
LegacyEventPair reverse_lookup_legacy_event(const EventDescriptor &evd) {
  return legacy_events->reverse_get_event(evd);
}
