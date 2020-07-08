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
  EventDescriptor base_event;
  std::vector<ParsedEvent*> events;

  void push_back(ParsedEvent *ev) { events.push_back(ev); }
  std::vector<ParsedEvent*>::iterator begin() { return events.begin(); }
  std::vector<ParsedEvent*>::iterator end() { return events.end(); }
  std::vector<ParsedEvent*>::const_iterator begin() const { return events.begin(); }
  std::vector<ParsedEvent*>::const_iterator end() const { return events.end(); }
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
 public:
  ParsedEventGroup &insert(const EventDescriptor &ev) {
    auto i = mapping_.insert({ev.internal_id, {ev, {}}});
    return i.first->second;
  }
  void declare(ParsedEvent *ev) {
    insert(ev->ev_id).push_back(ev);
  }
  
  class iterator {
    std::map<int, ParsedEventGroup>::iterator it_;

   public:
    iterator(std::map<int, ParsedEventGroup>::iterator it): it_(it) {}

    iterator &operator++() { ++it_; return *this; }
    iterator &operator--() { --it_; return *this; }
    iterator operator++(int) { iterator res = *this; ++it_; return res; }
    iterator operator--(int) { iterator res = *this; --it_; return res; }

    bool operator==(const iterator &other) const { return it_ == other.it_; }
    bool operator!=(const iterator &other) const { return it_ != other.it_; }
    
    ParsedEventGroup &operator*() const { return it_->second; }
    ParsedEventGroup &operator->() const { return it_->second; }
  };
  
  iterator begin() { return iterator(mapping_.begin()); }
  iterator end() { return iterator(mapping_.end()); }

 private:
  std::map<int, ParsedEventGroup> mapping_;
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
  EventGroupIndex non_stacked_events;
  // An index over stacked (parameterized) events. Generally, ENIGMA only emits
  // one event routine for these to avoid event pollution in generated code and
  // unnecessary overhead for vtable lookups. However, this is not the
  // collection to use for registering events. Use `registered_events`.
  EventGroupIndex stacked_events;
  // An index over events that will be registered with the event loop.
  // This includes both stacked and non-stacked events alike, but excludes
  // special events (like create or destroy).
  EventGroupIndex registered_events;
  // Specific events (including arguments of parameterized events) that were
  // inherited from any other object in the chain.
  std::set<Event> inherited_events;
  // Just the internal IDs of the inherited_events set.
  std::set<int> inherited_base_events;

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
  }
  // Inherits into this group the events owned by the given group.
  // This explicitly excludes transitive inheritance! To achieve that,
  // you must inherit from each object in the chain.
  void InheritFrom(parsed_object *other) {
    for (const auto &e : other->all_events) Inherit(e.ev_id);
  }
  // Checks whether any events of this type are inherited from another object.
  bool InheritsAny(const EventDescriptor &e) const {
    return inherited_base_events.find(e.internal_id) != inherited_base_events.end();
  }
  // Checks whether any events of this type are inherited from another object.
  bool Inherits(const Event &e) const {
    return inherited_events.find(e) != inherited_events.end();
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
