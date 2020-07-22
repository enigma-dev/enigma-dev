/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008-2011 Josh Ventura                                        **
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

#ifndef ENIGMA_OBJECT_STORAGE_H
#define ENIGMA_OBJECT_STORAGE_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "darray.h"
#include "event_reader/event_parser.h"

using namespace std;

//Represent an initializer (name(value) in constructor)
typedef pair<string,string> initpair;

//These parallel ism's structs, but offer additional properties we need to finish compile
struct ParsedScope;
struct ParsedCode {
  string code;
  string synt;
  unsigned int strc;
  varray<string> strs;
  // Redirects this code to apply to another object (wraps in with()).
  int otherObjId;
  // Allows us to add to locals when parsing the code.
  ParsedScope *my_scope;

  ParsedCode(ParsedScope *scope): my_scope(scope) {}
};
struct ParsedEvent : ParsedCode {
  Event ev_id;

  ParsedEvent(Event ev_id, ParsedScope *scope):
    ParsedCode(scope), ev_id(ev_id) {}
};

// Leverages EDL's direct use of C++ declaration syntax to represent type
// declarations as triples of type, prefix, suffix. This way, `int *(x)[3]` has
// type "int", prefix "*(", suffix ")[3]". Note that the name is expected to
// be the key to a map, while this dectrip appears as the value.
struct dectrip {
  string type, prefix, suffix;
  dectrip();
  dectrip(string t);
  dectrip(string t, string p, string s);

  bool defined() const;
  bool operator!=(const dectrip&) const;
};

// Similar to dectrip, but adds a fourth string for the value used to initialize
// this variable.
struct decquad {
  string type, prefix, suffix, value;
  decquad();
  decquad(string t);
  decquad(string t, string p, string s, string v);

  bool defined() const;
  bool operator!= (const decquad&) const;
};

// Represents a logical group of events, for which an event iterator must be
// registered and/or code must be emitted. There are multiple kinds of logical
// groupings, and this struct can represent each of them. Each distinct type of
// logical grouping uses a separate collection in a parsed_object.
struct ParsedEventGroup {
  EventGroupKey event_key;
  std::vector<ParsedEvent*> events;

  bool empty() const { return events.empty(); }
  void push_back(ParsedEvent *ev) { events.push_back(ev); }
  std::vector<ParsedEvent*>::iterator begin() { return events.begin(); }
  std::vector<ParsedEvent*>::iterator end() { return events.end(); }
  std::vector<ParsedEvent*>::const_iterator begin() const { return events.begin(); }
  std::vector<ParsedEvent*>::const_iterator end() const { return events.end(); }
};

template<typename Map> class ValueIterator {
  typedef typename Map::iterator It;
  It it_;

 public:
  ValueIterator(It it): it_(it) {}

  ValueIterator &operator++() { ++it_; return *this; }
  ValueIterator &operator--() { --it_; return *this; }
  ValueIterator operator++(int) { ValueIterator res = *this; ++it_; return res; }
  ValueIterator operator--(int) { ValueIterator res = *this; --it_; return res; }

  bool operator==(const ValueIterator &other) const { return it_ == other.it_; }
  bool operator!=(const ValueIterator &other) const { return it_ != other.it_; }
  
  auto &operator*() const { return it_->second; }
  auto &operator->() const { return it_->second; }
};

// Serves as an index over 
class EventIndex {
  std::map<Event, ParsedEvent*> mapping_;

 public:
  bool declare(ParsedEvent *ev) {
    auto i = mapping_.insert({ev->ev_id, ev});
    return i.first->second;
  }

  typedef ValueIterator<decltype(mapping_)> iterator;
  iterator begin() { return iterator(mapping_.begin()); }
  iterator end() { return iterator(mapping_.end()); }
};

// Groups events by their base event. This has a dual purpose:
//
//   1. Grouping stacked events by their base event. This gives a view of the
//      different kinds of events that are implmented, and an iterable view over
//      each parameterized occurrence thereof.
//
//   2. Storing non-stacked events by their event type. In the old system,
//      non-stacked events could share an ID (most notably, "Other" events).
//      In the new event system, this doesn't happen; each distinct event
//      behavior is paired to exactly one internal event ID.
//
// See notes on its usage in parsed_object.
class EventGroupIndex {
  typedef std::map<EventGroupKey, ParsedEventGroup> Mapping;
  Mapping mapping_;

 public:
  ParsedEventGroup &insert(const Event &ev) {
    auto i = mapping_.insert({EventGroupKey{ev}, ParsedEventGroup{ev, {}}});
    return i.first->second;
  }
  void declare(ParsedEvent *ev) {
    ParsedEventGroup &g = insert(ev->ev_id);
    if (ev->ev_id.IsStacked() || g.empty()) {
      g.push_back(ev);
    } else {
      std::cerr << "ERROR: Multiple declarations of event `"
                << ev->ev_id.HumanName() << "` in one object!";
    }
  }

  typedef ValueIterator<decltype(mapping_)> iterator;
  iterator begin() { return iterator(mapping_.begin()); }
  iterator end() { return iterator(mapping_.end()); }
};

// Represents a collection of variable name usages detected while parsing code
// in some entity, such as an object or script. Game Maker is dynamically-scoped
// (at least partially), which has proven convenient for users, who do not have
// to keep track of all the shared state needed when invoking scripts. To offer
// similar convenience in ENIGMA, we need to be aware of all of the variables
// referenced by each and every game asset.
//
// Another convenience feature to adopt is for variable declaration to also be
// automatic. In Game Maker, this is pretty simple because declaration happens
// dynamically upon assign (presence or absence in one of the scope maps is the
// only mechanism behind declaration in Game Maker). In ENIGMA, this isn't so
// simple that it can be done accidentally. ENIGMA treats undeclared variables
// as implicit `var` declarations. When emitting statically-typed code for the
// game (namely, C++), we need to know all variables that were declared, and
// all variables that were referenced in each asset scope.
struct ParsedScope {
  /// Any variable, KEY, used but not declared, or explicitly declared as
  /// `local <VALUE>` (eg, `local int *name[3]`).
  map<string,dectrip> locals;
  /// Any variable, KEY, used in a `with` statement, but not locally declared.
  map<string,dectrip> ambiguous;
  /// Any variable KEY declared as global VALUE.
  map<string,dectrip> globals;  ///< 
  /// Any variable, KEY, declared as constant VALUE.
  map<string,decquad> consts;
  /// Any shared local variable, KEY, used in this scope.
  map<string,int> globallocals;
  /// Any function, KEY, called with at most <VALUE> parameters.
  map<string,int> funcs;
  /// Any timeline, KEY, set (or possibly set) by this object.
  map<string,int> tlines;
  /// Any attribute KEY accessed via a dot, as in `foo.<KEY>`.
  map<string,int> dots;
  /// Variables that must be initialized in the constructor for this object.
  vector<initpair> initializers;

  void copy_from(const ParsedScope&,
                 const string &sourcename, const string &destname);
  void copy_calls_from(const ParsedScope&);
  void copy_tlines_from(const ParsedScope&);
};

// Stores information about an object that must be cached between parse and emit
// (such as locals and event iteration logic to emit).
struct parsed_object : ParsedScope {
  // An unorganized mess of all events owned by this object.
  // This does not account for inheritance in any way.
  std::vector<ParsedEvent> all_events;

  // An index over normal (non-stacked) events. Because these events do not
  // accept parameters, every group in this index should be a singleton.
  // However, not all of these events will be registered!
  EventIndex non_stacked_events;
  // An index over stacked (parameterized) events. Generally, ENIGMA only emits
  // one event routine for these to avoid event pollution in generated code and
  // unnecessary overhead for vtable lookups. However, this is not the
  // collection to use for registering events. Use `registered_events`.
  EventGroupIndex stacked_events;
  // Index over non-stacked events that will be registered with the event loop.
  // Put simply, this is all non-stacked events for which UsesEventLoop is true.
  // This includes normal and special events, but excludes trigger-once events
  // because they are never iterated for invocation. Stacked events would be
  // included, except they must be indexed by their base event type.
  EventGroupIndex registered_events;
  // Specific events (including arguments of parameterized events) that were
  // inherited from any other object in the chain.
  std::set<Event> inherited_events;
  // Just the internal IDs of the inherited_events set.
  std::set<EventGroupKey> inherited_event_groups;

  string name;
  int id;
  string sprite_name, mask_name, parent_name;
  bool visible, solid, persistent;
  double depth;

  parsed_object* parent; ///< The parent of this object, or NULL if the object has none.
  vector<parsed_object*> children; ///< A vector of the children of this object; parsed_objects which list this object as a parent.
  
  bool has_event(const EventDescriptor &event) const {
    for (const ParsedEvent &ev : all_events) {
      if (ev.ev_id.internal_id == event.internal_id) return true;
    }
    return false;
  }

  // Inherits the given event from a parent. This may affect codegen, but will
  // not prevent this object from overriding the event for itself.
  void Inherit(const Event &ev) {
    inherited_events.insert(ev);
    inherited_event_groups.insert({ev});
  }
  // Inherits into this object the events owned by the given object.
  // This explicitly excludes transitive inheritance! To achieve that,
  // you must inherit from each object in the chain.
  void InheritFrom(parsed_object *other) {
    for (const auto &e : other->all_events) Inherit(e.ev_id);
  }
  // Checks whether this event or stack is inherited from another object.
  bool InheritsAny(const EventGroupKey &e) const {
    return inherited_event_groups.find(e) != inherited_event_groups.end();
  }
  // Checks whether this event in particular is inherited from another object.
  // This is for checking specific event inheritance in EDL rather than for
  // vtable generation.
  bool InheritsSpecifically(const Event &e) const {
    return inherited_events.find(e) != inherited_events.end();
  }
  // Used to include default code for a given event in this object.
  // Behaves like inheriting from an object that doesn't exist.
  // This is sort of a hack to make up for how event_parent isn't
  // really an object. Some of this code would be simpler if it was.
  void InheritDefaultedEvent(const Event &event) {
    // Insert the event, but don't put anything in it. Only do this in objects
    // which don't have a parent (otherwise, we'll get redundant declare code).
    // Probably the tackiest thing I've done for this new event system,
    // but shouldn't manage to break anything.
    if (!parent) registered_events.insert(event);
  }

  // These can eventually go away. This was from a time when "auto" was a
  // storage specifier and for-loops had to count things.
  typedef map<string,dectrip>::iterator locit;
  typedef map<string,dectrip>::iterator ambit;
  typedef map<string,dectrip>::iterator globit;
  typedef map<string,dectrip>::const_iterator cglobit;
  typedef map<string,decquad>::iterator constit;
  typedef map<string,int>::iterator funcit;
  typedef map<string,int>::const_iterator const_funcit;
  typedef map<string,int>::iterator tlineit;
  typedef map<string,int>::const_iterator const_tlineit;
  typedef map<string,int>::iterator dotit;

  parsed_object();
  parsed_object(string,int,string,string,string,bool,bool,double,bool);
};

struct ParsedScript {
  string name;
  ParsedScope scope;
  // This scripts code, as an "event".
  ParsedCode code;
  // A global scope version of this script; behaves like `pev` in a `with()`.
  ParsedCode *global_code;
  int globargs; // The maximum number of arguments with which this was invoked from all contexts.
  // Automatically link our event to our object.
  ParsedScript(): scope(), code(&scope), global_code(nullptr), globargs(0) {};
};

struct parsed_moment {
  int step;
  ParsedScript *script;  // FIXME: this is wrong; make a scope for the timeline and use ParsedCode here
  parsed_moment(int s, ParsedScript *scr): step(s), script(scr) {}
};

struct parsed_timeline {
  int id;
  vector<parsed_moment> moments;
};

struct parsed_room {
  struct parsed_icreatecode {
    ParsedCode* code;
    string object_name;
  };
  string name;
  ParsedScope pseudo_scope;
  ParsedCode *creation_code = nullptr;
  map<int, parsed_icreatecode> instance_create_codes;
  //PreCreate code uses the same struct, as nothing is really different
  map<int, parsed_icreatecode> instance_precreate_codes;
};

typedef map<string,dectrip>::iterator deciter;
typedef map<string,dectrip>::const_iterator decciter;

struct parsed_extension {
  string name, path;
  string pathname;
  string implements, init;
};

typedef set<string> NameSet;
typedef set<string> SharedLocalSet;
typedef map<string, dectrip> DotLocalMap;
typedef vector<parsed_object*> ParsedObjectVec;
typedef vector<ParsedScript*> ParsedScriptVec;
typedef vector<parsed_room*> ParsedRoomVec;
typedef vector<parsed_extension> ParsedExtensionVec;

typedef map<string, ParsedScript*> ScriptLookupMap;
typedef map<string, parsed_timeline> TimelineLookupMap;

typedef ParsedObjectVec::iterator po_i;
typedef map<int, ParsedEvent*>::iterator pe_i;
typedef map<int, parsed_room*>::iterator pr_i;

// Global because seriously everything uses it; will need to be moved in a new PR
extern SharedLocalSet shared_object_locals;
// This is global because it's cached between builds
extern vector<string> requested_extensions_last_parse;
extern ParsedExtensionVec parsed_extensions;

struct CompileState {
  //Locals that are inherited by all instances of all objects from the core system.
  DotLocalMap dot_accessed_locals;
  ParsedObjectVec parsed_objects;
  ParsedScriptVec parsed_scripts;
  ParsedScriptVec parsed_tlines;
  ParsedRoomVec parsed_rooms;
  
  ScriptLookupMap script_lookup;
  TimelineLookupMap timeline_lookup;
  
  ParsedScope global_object;

  void add_dot_accessed_local(string name);
};

#endif
