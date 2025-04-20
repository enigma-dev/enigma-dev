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
  res.set_type(cb::EventDescriptor::TRIGGER_ONCE);
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

Event EventData::DecodeEventString(const std::string &evstring) const {
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
  return get_event(id, args);
}

Event EventData::get_event(const std::string &id,
                           const std::vector<std::string> &args) const {
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
  }
  std::string defv = "0";
  for (size_t argn = 0; argn < correct_arg_count; ++argn) {
    const std::string &arg = argn < correct_arg_count ? args[argn] : defv;
    const std::string &arg_kind = base_event.event->parameters(argn);
    auto pv = parameter_ids_.find({arg_kind, ToLower(arg)});
    if (pv == parameter_ids_.end()) {
      // Assume that spelling == name (this is the case for resource names/ints)
      res.arguments.emplace_back(arg, arg);
    } else {
      res.arguments.emplace_back(pv->second->id(), pv->second->spelling());
    }
  }
  return res;
}

const std::map<std::string, const cb::ParameterAlias*>
    &EventData::value_names_for_type(const std::string &type) const {
  static const std::map<std::string, const cb::ParameterAlias*> kUnknown;
  auto res_it = parameter_index_.find(type);
  if (res_it != parameter_index_.end()) return res_it->second;
  return kUnknown;
}

std::string Event::ParamSubstImpl(const std::string &str, bool code) const {
  std::string res;
  size_t start = 0;
  size_t pc = str.find_first_of('%');
  if (pc == std::string::npos) return str;
  while (pc != std::string::npos) {
    if (++pc < str.length() && str[pc] >= '1' && str[pc] <= '9' &&
        str[pc] <= '0' + (int) arguments.size()) {
      int pnum = str[pc] - '1';
      res += str.substr(start, pc - 1 - start);
      res += code ? arguments[pnum].spelling : arguments[pnum].name;
      start = ++pc;
    } else {
      std::cerr << "EVENT ERROR: Ignoring junk parameter " << str[pc] << std::endl;
    }
    pc = str.find_first_of('%', pc);
  }
  res += str.substr(start);
  return StrTrim(res);
}

EventData::EventData(EventFile &&events): event_file_(std::move(events)) {
  for (const auto &aliases : event_file_.aliases()) {
    for (const buffers::config::ParameterAlias &alias : aliases.aliases()) {
      parameter_ids_.insert({{aliases.id(), ToLower(alias.id())}, &alias});
      parameter_vals_.insert({{aliases.id(), alias.value()}, &alias});
      parameter_index_[aliases.id()].insert({ToLower(alias.id()), &alias});
    }
  }
  // Start numbering internal IDs in the new system from 1000, for good measure.
  for (long i = 0; i < event_file_.events_size(); ++i) {
    event_wrappers_.emplace_back(&event_file_.events(i), kMinInternalID + i);
  }
  for (const EventDescriptor &event_wrapper : event_wrappers_) {
    const int iid = event_wrapper.internal_id;
    const std::string evid = StripChar(event_wrapper.event->id(), '.');
    if (!event_index_.insert({ToLower(evid), &event_wrapper}).second) {
      std::cerr << "EVENT ERROR: Duplicate event ID " << evid << std::endl;
    }
    if (!event_wrapper.IsParameterized() &&  // Only insert non-parameterized events.
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
  // Check for a direct mapping (used for specialized ID pairs).
  auto it = compatability_mapping_.find({mid, sid});
  if (it != compatability_mapping_.end()) return it->second;

  // Check for parameterized events.
  it = compatability_mapping_.find({mid, kParameterizedSubId});
  if (it == compatability_mapping_.end()) {
    std::cerr << "EVENT ERROR: Legacy event (" << mid << ", " << sid
              << ") is not known to the system. Using EGM event." << std::endl;
    // Error case. Default to internal event ID. This will show up funny in the
    // IDE, but at least the user's code will be intact.
    auto res = event_iid_index_.find(mid);
    if (res != event_iid_index_.end()) {
    std::cerr << "EVENT ERROR: Event (" << mid << ", " << sid
              << ") is not a known Legacy event nor EGM event." << std::endl;
      return Event(*res->second);
    }
    return kSentinelEvent;
  }

  Event res = it->second;
  std::string value, spelling;
  const std::string &kind = res.ParameterKind(0);
  auto pit = parameter_vals_.find({kind, sid});
  if (pit != parameter_vals_.end()) {
    value = pit->second->id();
    spelling = pit->second->spelling();
  } else {
    if (kind != "object" && kind != "integer") {
      std::cerr << "Failed to look up " << kind << " parameter " << sid << ".\n";
    }
    value = std::to_string(sid);
    spelling = value;
  }
  res.arguments.emplace_back(value, spelling);
  return res;
}

Event EventData::get_event(const buffers::resources::Object::EgmEvent &event) const {
  return get_event(event.id(), {event.arguments().begin(),
                                event.arguments().end()});
}

bool Event::IsComplete() const {
  return arguments.size() == (unsigned) event->parameters_size();
}
bool EventDescriptor::IsParameterized() const {
  return event->parameters_size();
}
bool EventDescriptor::IsStacked() const {
  return event->type() == cb::EventDescriptor::STACKED;
}

std::string EventDescriptor::ExampleIDStrings() const {
  Event example(*this);
  for (const std::string &p : event->parameters()) {
    example.arguments.push_back({p, p});
  }
  return example.IdString();
}

std::string EventDescriptor::HumanName() const {
  return event->name();
}
std::string Event::HumanName() const {
  return NameSubst(event->name());
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
std::string Event::DispatcherCode(std::string_view funcname) const {
  return ParamSubst(string_replace_all(event->dispatcher(), "%event", funcname));
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
  return event->type() != cb::EventDescriptor::TRIGGER_ONCE &&
         event->type() != cb::EventDescriptor::TRIGGER_ALL;
}
bool EventDescriptor::RegistersIterator() const {
  return event->type() != cb::EventDescriptor::TRIGGER_ONCE &&
         event->type() != cb::EventDescriptor::INLINE;
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

std::string Event::TrueFunctionName() const {
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

bool EventGroupKey::operator<(const Event &other) const {
  if (internal_id != other.internal_id) return internal_id < other.internal_id;

  // This is the operative difference. We treat stacked events as the same event
  // group, even if the parameters are different.
  if (IsStacked()) return false;

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
  auto pv = parameter_ids_.find({arg_kind, ToLower(arg)});
  if (pv != parameter_ids_.end()) {
    return LegacyEventPair{amap.main_id, pv->second->value()};
  }
  auto iv = SafeAtoL(arg);
  if (iv.first) return LegacyEventPair{amap.main_id, iv.second};
  std::cerr << "EVENT ERROR: Unknown " << arg_kind << " parameter value " << arg
            << ": cannot map argument to event sub-ID\n";
  return LegacyEventPair{amap.main_id, 0};
}
