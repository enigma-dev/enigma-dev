/** Copyright (C) 2008, 2014, 2018 Josh Ventura
*** Copyright (C) 2013, 2014, Robert B. Colton
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
#include "parser/parser.h"
#include "backend/GameData.h"
#include "compiler/compile_common.h"
#include "event_reader/event_parser.h"
#include "general/parse_basics_old.h"
#include "settings.h"
#include "languages/lang_CPP.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

inline bool iscomment(const string &n) {
  if (n.length() < 2 or n[0] != '/') return false;
  if (n[1] == '/') return true;
  if (n[1] != '*') return false;
  for (unsigned i = 2; i < n.length(); i++)
  {
    if (n[i] == '*' and n[i+1] == '/')
      return (i + 2 >= n.length());
  }
  return true;
}

// This just pretties up the codegen a fuzz. It's for pre-parsed code in events.ey.
// Eventually we'll be using the Syntax checker's lexer to build our own tree and
// print that everywhere. Then this code will be useless.
void PrintIndentedCode(std::ostream &wto, std::string_view code, int indent) {
  std::string istr(indent, ' ');
  for (size_t i = 0, j; i != std::string::npos; i = j + 1) {
    j = code.find_first_of("\r\n", i);
    wto << istr << code.substr(i, j - i) << "\n";
    if (j == std::string::npos) break;
    if (code[j] == '\r' && j < code.length() - 1 && code[j + 1] != '\n') ++j;
  }
}

static inline void declare_scripts(std::ostream &wto, const GameData &game, const CompileState &state) {
  wto << "// Script identifiers\n";
  for (size_t i = 0; i < game.scripts.size(); i++)
    wto << "#define " << game.scripts[i].name << "(...) _SCR_" << game.scripts[i].name << "(__VA_ARGS__)\n";
  wto << "\n\n";

  for (size_t i = 0; i < game.scripts.size(); i++) {
    ParsedScript* scr = state.parsed_scripts[i];
    const char* comma = "";
    wto << "variant _SCR_" << game.scripts[i].name << "(";
    scr->globargs = 16; //Fixes too many arguments error (scripts can be called dynamically with any number of arguments)
    for (int argn = 0; argn < scr->globargs; argn++) {
      wto << comma << "variant argument" << argn << "=0";
      comma = ", ";
    }
    wto << ");\n";
  }
  wto << "\n\n";
}

static inline void declare_extension_casts(std::ostream &wto,
    const ParsedExtensionVec &parsed_extensions) {
  // Write extension cast methods; these are a temporary fix until the new instance system is in place.
  wto << "  namespace extension_cast {\n";
  for (unsigned i = 0; i < parsed_extensions.size(); i++) {
    if (!parsed_extensions[i].implements.empty()) {
      wto << "    " << parsed_extensions[i].implements << " *as_" << parsed_extensions[i].implements << "(object_basic* x) {\n";
      wto << "      return (" << parsed_extensions[i].implements << "*)(object_locals*)x;\n";
      wto << "    }\n";
    }
  }
  wto << "  }\n";
}

static inline void declare_object_locals_class(std::ostream &wto,
    const ParsedExtensionVec &parsed_extensions) {
  wto << "  extern std::map<int,object_basic*> instance_deactivated_list;\n";
  wto << "  extern objectstruct** objectdata;\n\n";

  wto << "  struct object_locals: event_parent";
    for (unsigned i = 0; i < parsed_extensions.size(); i++) {
      if (!parsed_extensions[i].implements.empty()) {
        wto << ",\n      virtual " << parsed_extensions[i].implements;
      } else {
        wto << " /* " << parsed_extensions[i].name << " */";
      }
    }
  wto << "\n";

  wto << "  {\n";
  wto << "    #include \"Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h\"\n\n";
  wto << "    std::map<string, var> *vmap;\n";
  wto << "    object_locals() {vmap = NULL;}\n";
  wto << "    object_locals(unsigned _x, int _y): event_parent(_x,_y) {vmap = NULL;}\n";
  wto << "  };\n";
}

// TODO(JoshDreamland): MOVEME: group with extension code; call remains in this file
static inline void write_extension_casts(std::ostream &wto,
    const ParsedExtensionVec &parsed_extensions) {
  // Write extension cast methods; these are a temporary fix until the new instance system is in place.
  wto << "namespace enigma {\n";
  wto << "  namespace extension_cast {\n";
  for (unsigned i = 0; i < parsed_extensions.size(); i++) {
    if (!parsed_extensions[i].implements.empty()) {
      wto << "    " << parsed_extensions[i].implements << " *as_" << parsed_extensions[i].implements << "(object_basic* x) {\n";
      wto << "      return (" << parsed_extensions[i].implements << "*)(object_locals*)x;\n";
      wto << "    }\n";
    }
  }
  wto << "  }\n";
  wto << "}\n\n";
}

// TODO(JoshDreamland): Burn this function into ash and launch the ashes into space
static inline void compute_locals(language_adapter *lang, parsed_object *object, const string addls) {
  pt pos;
  string type, name, pres, sufs;
  for (pos = 0; pos < addls.length(); pos++)
  {
    if (is_useless(addls[pos])) continue;
    if (addls[pos] == ';') { object->locals[name] = dectrip(type, pres, sufs); type = pres = sufs = ""; continue; }
    if (addls[pos] == ',') { object->locals[name] = dectrip(type, pres, sufs); pres = sufs = ""; continue; }
    if (is_letter(addls[pos]) or addls[pos] == '$') {
      const pt spos = pos;
      while (is_letterdd(addls[++pos]));
      string tn = addls.substr(spos,pos-spos);
      (lang->find_typename(tn) ? type : name) = tn;
      pos--; continue;
    }
    if (addls[pos] == '*') { pres += '*'; continue; }
    if (addls[pos] == '[') {
      int cnt = 1;
      const pt spos = pos;
      while (cnt and ++pos < addls.length())
        if (addls[pos] == '[' or addls[pos] == '(') cnt++;
        else if (addls[pos] == ')' or addls[pos] == ']') cnt--;
      sufs += addls.substr(spos,pos-spos+1);
      continue;
    }
    if (addls[pos] == '=') {
      int cnt = 0;

      pt spos = ++pos;
      while (is_useless(addls[spos])) spos++;
      pos = spos - 1;

      while (++pos < addls.length() and (cnt or (addls[pos] != ',' and addls[pos] != ';')))
        if (addls[pos] == '[' or addls[pos] == '(') cnt++;
        else if (addls[pos] == ')' or addls[pos] == ']') cnt--;
      bool redundant = false;
      if (object->parent) {
        for (parsed_object *obj = object; obj != NULL; obj = obj->parent) {
          for (size_t j = 0; j < obj->initializers.size(); j++) {
            if (obj->initializers[j].first == name) {
              redundant = true;
              break;
            }
          }
          if (redundant) {
            break;
          }
        }
      } else {
        for (size_t j = 0; j < object->initializers.size(); j++) {
          if (object->initializers[j].first == name) {
            redundant = true;
            break;
          }
        }
      }
      if (!redundant) {
        object->initializers.push_back(initpair(name,addls.substr(spos,pos-spos)));
      }
      pos--; continue;
    }
  }
}

static inline bool parent_declares(parsed_object *parent, const deciter decl) {
  for (parsed_object *obj = parent; obj != NULL; obj = obj->parent) {
    for (deciter it =  obj->locals.begin(); it != obj->locals.end(); it++) {
      if (it->first == decl->first &&
          it->second.prefix == decl->second.prefix &&
          it->second.type   == decl->second.type   &&
          it->second.suffix == decl->second.suffix) {
        return true;
      }
    }
  }
  return false;
}

static void write_object_locals(language_adapter *lang, std::ostream &wto,
                                const ParsedScope *global,
                                parsed_object *object) {
  wto << "    // Local variables\n    ";
  for (const ParsedEvent &pev : object->all_events) {
    string addls = pev.ev_id.LocalDeclarations();
    if (addls.length()) {
      compute_locals(lang, object, addls);
    }
  }

  for (deciter ii =  object->locals.begin(); ii != object->locals.end(); ii++) {
    bool writeit = true; // Whether this "local" should be declared such
    if (parent_declares(object->parent, ii)) {
      continue;
    }

    // If it's not explicitely defined, we must question whether it should be
    // given a unique presence in this scope
    if (!ii->second.defined()) {
      parsed_object::cglobit ve = global->globals.find(ii->first); // So, we look for a global by this name
      if (ve != global->globals.end()) {  // If a global by this name is indeed found,
        if (ve->second.defined()) // And this global is explicitly defined, not just accessed with a dot,
          writeit = false; // We assume that its definition will cover us, and we do not redeclare it as a local.
        cout << "enigma: scopedebug: variable `" << ii->first
             << "' from object `" << object->name
             << "' will be used from the " << (writeit ? "object" : "global")
             << " scope." << endl;
      }
    }
    if (writeit) {
      wto << tdefault(ii->second.type) << " " << ii->second.prefix << ii->first
          << ii->second.suffix << ";\n    ";
    }
  }
}

static inline void write_object_scripts(std::ostream &wto, parsed_object *object, const CompileState &state) {
  // Next, we write the list of all the scripts this object will hoard a copy of for itself.
  wto << "\n    //Scripts called by this object\n    ";
  for (parsed_object::funcit it = object->funcs.begin(); it != object->funcs.end(); it++) //For each function called by this object
  {
    auto subscr = state.script_lookup.find(it->first); //Check if it's a script
    if (subscr != state.script_lookup.end() // If we've got ourselves a script
    and subscr->second->global_code) { // And it has distinct code for use at the global scope (meaning it's more efficient locally)
      const char* comma = "";
      wto << "\n    variant _SCR_" << it->first << "(";
      for (int argn = 0; argn < it->second; argn++) //it->second gives max argument count used
      {
        wto << comma << "variant argument" << argn << " = 0";
        comma = ", ";
      }
      wto << ");";
    }
  } wto << "\n    ";
}

static inline void write_object_timelines(std::ostream &wto, const GameData &/*game*/, parsed_object *object, const TimelineLookupMap &timeline_lookup) {
  // Next, we write the list of all the timelines this object will hoard a copy of for itself.
  // NOTE: See below; we actually need to assume this object has the potential to call any timeline.
  //       BUT we only locally-copy the ones we know about for sure here.
  bool hasKnownTlines = false;

  wto << "\n    //Timelines called by this object\n";
  for (parsed_object::tlineit it = object->tlines.begin(); it != object->tlines.end(); it++) //For each timeline potentially set by this object.
  {
    auto timit = timeline_lookup.find(it->first); //Check if it's a timeline
    if (timit != timeline_lookup.end()) // If we've got ourselves a script
    //and subscr->second->pev_global) // And it has distinct code for use at the global scope (meaning it's more efficient locally) //NOTE: It seems all timeline MUST be copied locally.
    {
      hasKnownTlines = true;
      for (const auto &moment : timit->second.moments) {
        wto << "    void TLINE_" << timit->first << "_MOMENT_" << moment.step << "();\n";
      }
    }
  } wto << "\n";

  //If at least one timeline is called by this object, override timeline_call_moment_script() to properly dispatch it to the local instance.
  if (hasKnownTlines) {
    wto << "    // Dispatch timelines properly for this object..\n";
    wto << "    virtual void timeline_call_moment_script(int timeline_index, int moment_index);\n\n";
  }
}

void recursive_inherit(parsed_object *object, set<parsed_object*> &visited) {
  // This is just an optimization. Don't redo work.
  if (visited.find(object) != visited.end()) return;
  visited.insert(object);

  parsed_object *p = object->parent;
  if (!p) return;
  recursive_inherit(p, visited);
  object->InheritFrom(p);
}

// TL;DR once upon a time Robert tried to implement inheritance by having every
// single routine in this file check all of its parents for events every time it
// had a question about them. It would cache information in weird vectors and
// maps, but would always execute the iteration logic from scratch with each new
// routine. This method performs the iteration ONCE and caches it directly for
// later iteration.
static inline void generate_robertvecs(const ParsedObjectVec &objects) {
  // TODO: copy out groups of events, here, then populate children with empty
  // versions of their parents' events
  for (parsed_object *object : objects) {
    for (ParsedEvent &pev : object->all_events) {
      if  (!pev.code.empty()) {
        if (pev.ev_id.IsStacked()) {
          object->stacked_events.declare(&pev);
        } else {
          object->non_stacked_events.declare(&pev);
        }
        if (pev.ev_id.RegistersIterator()) {
          object->registered_events.declare(&pev);
        }
      }
    }
  }
  set<parsed_object*> visited;
  for (parsed_object *object : objects) {
    recursive_inherit(object, visited);
  }
}

// Write out declarations for *all* events implemented by *this* object.
// This include events overridden from a parent event, but does not include
// the parent's events otherwise. This includes special events like create or
// destroy, and stacked events like individual alarms (even though stacked
// events will be wrapped into a single virtual method to be called as part
// of the event loop). Note also that this array also includes events that
// were automatically added to this object per `events.ey`.
static void write_object_events(std::ostream &wto, parsed_object *object) {
  for (const ParsedEvent &pev : object->all_events) {
    string evname = pev.ev_id.TrueFunctionName();
    if (!pev.code.empty() || pev.ev_id.HasDefaultCode()) {
      wto << "    variant myevent_" << evname << "();\n";
      if (pev.ev_id.HasSubCheck()) {
        wto << "    inline bool myevent_" << evname << "_subcheck();\n";
      }
    }
  }
}

// Stacked events need a separate virtual event to trigger from the event loop.
// Otherwise, we'd be writing a great deal of placeholder events, most of them
// calling into nothing. We'd also be wasting a ton of time on vtable lookups
// in general, even if every object implemented the same collision events.
//
// Also, some events require special dispatcher logic. An example of where this
// is strictly required is collision events, which must be run in a secondary
// loop fashion. Regardless of whether they are stacked, dispatched events
// receive a dispatch routine to handle one or all instances of that event.
//
// Because the subchecks and dispatch logic for these event groups can contain
// variable name lookups, all instances must be declared before we implement the
// final routines.

static void declare_event_groups(std::ostream &wto, const parsed_object *object) {
  wto << "    \n    // Stacked event bases and dispatchers\n";
  for (const ParsedEventGroup &event_stack : object->stacked_events) {
    wto << "    void myevent_"
        << event_stack.event_key.BaseFunctionName();
    if (event_stack.event_key.HasDispatcher()) wto << "_dispatcher";
    wto << "() override;\n";
  }
}

static void implement_event_groups(std::ostream &wto, const parsed_object *object) {
  wto << "\n// Stacked event bases and dispatchers\n";
  for (const ParsedEventGroup &event_stack : object->stacked_events) {
    wto << "void enigma::OBJ_" << object->name << "::myevent_"
        << event_stack.event_key.BaseFunctionName();
    if (event_stack.event_key.HasDispatcher()) wto << "_dispatcher";
    wto << "() {\n";
    for (ParsedEvent *event : event_stack) {
      const auto &ev = event->ev_id;
      // Use the full function name to call individual events in this stack.
      const string evname = ev.TrueFunctionName();
      int indent = 2;
      if (ev.HasSubCheck()) {
        indent += 2;
        if (ev.HasSubCheckExpression()) {
          wto << "  if (" << ev.SubCheckExpression() << ") {\n";
        } else {
          wto << "  if (myevent_" + evname + "_subcheck()) {\n";
        }
      }
      const std::string logic = ev.HasDispatcher()
                                    ? ev.DispatcherCode("myevent_" + evname)
                                    : "myevent_" + evname + "();";
      PrintIndentedCode(wto, logic, indent);
      if (ev.HasSubCheck())
        wto << "  }\n";
    }
    wto << "}\n";
  }
}

static inline void write_event_perform(
    std::ostream &wto, const EventData &events, parsed_object *object) {
  /* Event Perform Code */
  wto << "\n    // Event Perform Code\n";
  wto << "    variant myevents_perf(int type, int numb) override {\n";

  for (const auto &event : object->all_events) {
    string evname = event.ev_id.TrueFunctionName();
    auto legacy_pair = events.reverse_get_event(event.ev_id);
    wto << "      if (type == " << legacy_pair.mid << " && numb == " << legacy_pair.id << ")\n";
    wto << "        return myevent_" << evname << "();\n";
  }

  if (object->parent) {
    wto << "      return OBJ_" << object->parent->name << "::myevents_perf(type,numb);\n";
  } else {
    wto << "      return 0;\n";
  }

  wto << "    }\n";
}

static inline void write_object_unlink(std::ostream &wto, parsed_object *object) {
  // Now we write the callable unlinker. Its job is to disconnect the instance
  // for destroy. This is an important component that tracks multiple pieces
  // of the instance. These pieces are created for efficiency within the engine.
  // See the instance system documentation for full details.

  // Here we write the pieces it tracks
  wto << "\n    // Self-tracking\n";

  // This tracks components of the instance system.
  if (!object->parent)  // The rootmost parent tracks us in the instance list.
    wto << "      enigma::pinstance_list_iterator ENOBJ_ITER_me;\n";
  wto << "      enigma::inst_iter *ENOBJ_ITER_myobj" << object->id << ";\n";

  // This tracks components of the event system.
  for (const ParsedEventGroup &group : object->registered_events) {
    const EventGroupKey &event = group.event_key;
    if (object->InheritsAny(event)) continue;
    if (event.HasIteratorDeclareCode()) {
      if (!iscomment(event.IteratorDeclareCode())) {
        wto << "      " << event.IteratorDeclareCode() << ";\n";
      }
    } else {
      wto << "      enigma::inst_iter *ENOBJ_ITER_myevent_"
          << event.FunctionName() << ";\n";
    }
  }

  //This is the actual call to remove the current instance from all linked records before destroying it.
  wto << "\n    void unlink() {\n";
  wto << "      instance_iter_queue_for_destroy(this); // Queue for delete while we're still valid\n";
  wto << "      if (enigma::instance_deactivated_list.erase(id)==0) {\n";
  wto << "        // If it's not in the deactivated list, then it's active (so deactivate it).\n";
  wto << "        deactivate();\n";
  wto << "      }\n";
  wto << "    }\n\n";

  // Write out the unlink code in a deactivate routine that does not schedule
  // garbage collection of the instance. This is used to implement the
  // `instance_deactivate` family of functions.
  wto << "    void deactivate() {\n";

  // Unlink ourself. The rootmost parent unlinks the instance list entry.
  // Each object then unlinks its respective object list entry.
  if (!object->parent) {
    // We're the rootmost parent; unlink ourself from the instance list.
    wto << "      enigma::unlink_main(ENOBJ_ITER_me);\n";
  } else {
    // Let the parent handle the instance unlink (and its own object unlinking).
    wto << "      OBJ_" << object->parent->name << "::deactivate();\n";
  }
  // This is our object list entry.
  wto << "      unlink_object_id_iter(ENOBJ_ITER_myobj" << object->id << ", "
                                      << object->id << ");\n";

  for (const ParsedEventGroup &group : object->registered_events) {
    const EventGroupKey &event = group.event_key;
    if (object->InheritsAny(event)) continue;
    const string evname = event.FunctionName();
    if (event.HasIteratorRemoveCode()) {
      if (!iscomment(event.IteratorRemoveCode()))
        wto << "      " << event.IteratorRemoveCode() << ";\n";
    } else {
      wto << "      enigma::event_" << evname << "->unlink(ENOBJ_ITER_myevent_" << evname << ");\n";
    }
  }

  wto << "    }\n";
}

static inline void write_object_constructors(std::ostream &wto, parsed_object *object) {
  /*
  ** Next are the constructors. One is automated, the other directed.
  **   Automatic constructor:  The constructor generates the ID from a global maximum and links by that alias.
  **   Directed constructor:   Meant for use by the room system, the constructor uses a specified ID alias assumed to have been checked for conflict.
  */
  wto <<   "\n    OBJ_" <<  object->name << "(int enigma_genericconstructor_newinst_x = 0, int enigma_genericconstructor_newinst_y = 0, const int id = (enigma::maxid++)"
      << ", const int enigma_genericobjid = " << object->id << ", bool handle = true)";

  if (object->parent) {
    wto << ": OBJ_" << object->parent->name << "(enigma_genericconstructor_newinst_x,enigma_genericconstructor_newinst_y,id,enigma_genericobjid,false)";
  } else {
    wto << ": object_locals(id,enigma_genericobjid) ";
  }

  for (size_t ii = 0; ii < object->initializers.size(); ii++)
    wto << ", " << object->initializers[ii].first << "(" << object->initializers[ii].second << ")";
  wto << "\n    {\n";
  wto << "      if (!handle) return;\n";
  // Sprite index
  if (used_funcs::object_set_sprite) //We want to initialize
    wto << "      sprite_index = enigma::object_table[" << object->id << "].->sprite;\n"
        << "      make_index = enigma::object_table[" << object->id << "]->mask;\n";
  else
    wto << "      sprite_index = enigma::objectdata[" << object->id << "]->sprite;\n"
        << "      mask_index = enigma::objectdata[" << object->id << "]->mask;\n";
  wto << "      visible = enigma::objectdata[" << object->id << "]->visible;\n      solid = enigma::objectdata[" << object->id << "]->solid;\n";
  wto << "      persistent = enigma::objectdata[" << object->id << "]->persistent;\n";

  wto << "      activate();\n";


  // Coordinates
  wto << "      x = enigma_genericconstructor_newinst_x, y = enigma_genericconstructor_newinst_y;\n";

  wto << "      enigma::constructor(this);\n";
  wto << "    }\n\n";

  wto << "    void activate()\n    {\n";
  if (object->parent) {
      wto << "      OBJ_" << object->parent->name << "::activate();\n";
      // Have to remove the one the parent added so we can add our own
      wto << "      depth.remove();\n";
  }
  // Depth iterator used for draw events in graphics system screen_redraw
  wto << "      depth.init(enigma::objectdata[" << object->id << "]->depth, this);\n";
  // Instance system interface
  if (!object->parent) {
    wto << "      ENOBJ_ITER_me = enigma::link_instance(this);\n";
    for (parsed_object *obj = object; obj; obj = obj->parent) {
      wto << "      ENOBJ_ITER_myobj" << obj->id << " = enigma::link_obj_instance(this, " << obj->id << ");\n";
    }
  } else {
    wto << "      ENOBJ_ITER_myobj" << object->id << " = enigma::link_obj_instance(this, " << object->id << ");\n";
  }
  // Event system interface
  for (const ParsedEventGroup &group : object->registered_events) {
    const EventGroupKey &event = group.event_key;
    if (object->InheritsAny(event)) continue;
    const string evname = event.FunctionName();
    if (event.HasIteratorInitializeCode()) {
      if (!iscomment(event.IteratorInitializeCode()))
        wto << "      " << event.IteratorInitializeCode() << ";\n";
    } else {
      wto << "      ENOBJ_ITER_myevent_" << evname
          << " = enigma::event_" << evname << "->add_inst(this);\n";
    }
  }
  wto << "    }\n";
}

static void write_object_destructor(std::ostream &wto, parsed_object *object) {
  wto <<   "    \n    ~OBJ_" <<  object->name << "()\n    {\n";

  if (!object->parent) {
    wto << "      delete vmap;\n";
    wto << "      enigma::winstance_list_iterator_delete(ENOBJ_ITER_me);\n";
    for (parsed_object *obj = object; obj; obj = obj->parent) {
      wto << "      delete ENOBJ_ITER_myobj" << obj->id << ";\n";
    }
  } else {
    wto << "      delete ENOBJ_ITER_myobj" << object->id << ";\n";
  }
  for (const ParsedEventGroup &group : object->registered_events) {
    const EventGroupKey &event = group.event_key;
    if (object->InheritsAny(event)) continue;
    if (event.HasIteratorDeleteCode()) {
      if (!iscomment(event.IteratorDeleteCode()))
        wto << "      " << event.IteratorDeleteCode() << ";\n";
    } else {
      wto << "      delete ENOBJ_ITER_myevent_" << event.FunctionName() << ";\n";
    }
  }
  wto << "    }\n";

  //We'll sneak this in here.
  wto << "    virtual bool can_cast(int obj) const;\n";
}

static void write_object_class_body(parsed_object* object, language_adapter *lang, std::ostream &wto, const GameData &game, const CompileState &state) {
  wto << "  \n  struct OBJ_" << object->name;
  if (object->parent) {
      wto << ": OBJ_" << object->parent->name;
  } else {
      wto << ": object_locals";
  }
  wto << "\n  {\n";

  write_object_locals(lang, wto, &state.global_object, object);
  write_object_scripts(wto, object, state);
  write_object_timelines(wto, game, object, state.timeline_lookup);
  write_object_events(wto, object);

  declare_event_groups(wto, object);
  write_event_perform(wto, lang->event_data(), object);
  write_object_unlink(wto, object);
  write_object_constructors(wto, object);
  write_object_destructor(wto, object);

  wto << "  };\n";
}

static inline void write_object_family(parsed_object* object, language_adapter *lang, std::ostream &wto, const GameData &game, const CompileState &state) {
  write_object_class_body(object, lang, wto, game, state);
  for (ParsedObjectVec::iterator child_it = object->children.begin(); child_it != object->children.end(); ++child_it) {
    write_object_family(*child_it, lang, wto, game, state);
  }
}

static inline void write_object_class_bodies(language_adapter *lang, std::ostream &wto, const GameData &game, const CompileState &state) {
  for (parsed_object *object : state.parsed_objects) {
    if (object->parent) {
      continue; // Do not write out objects before we've written their parent
    }

    write_object_family(object, lang, wto, game, state);
  }
}

static inline string resname(string name) {
  return name.empty() ? "-1" : name;
}

static inline void write_object_data_structs(std::ostream &wto,
      const ParsedObjectVec &parsed_objects) {
  wto << "  std::vector<objectstruct> objs = {\n" << std::fixed;
  for (parsed_object *object : parsed_objects) {
    wto << "    { "
        << resname(object->sprite_name)  << ", "
        << resname(object->polygon_name) << ", "
        << object->solid                 << ", "
        << object->visible               << ", "
        << object->depth                 << ", "
        << object->persistent            << ", "
        << resname(object->mask_name)    << ", "
        << resname(object->parent_name)  << ", "
        << object->id
        << " },\n";
  }
  wto.unsetf(ios_base::floatfield);
  wto << "  };\n";
  wto << "  int objectcount = " << parsed_objects.size() << ";\n";
}

// [ CODEGEN FILE ] ------------------------------------------------------------
// Object declarations: object classes/names and locals. -----------------------
// -----------------------------------------------------------------------------
static inline void write_object_declarations(
    lang_CPP* lcpp, const GameData &game, const CompileState &state) {
  ofstream wto;
  wto.open(codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h",ios_base::out);
  wto << license;
  wto << "#include \"Universal_System/Object_Tiers/collisions_object.h\"\n";
  wto << "#include \"Universal_System/Object_Tiers/object.h\"\n\n";
  wto << "#include <map>";

  declare_scripts(wto, game, state);

  wto << "namespace enigma\n{\n";
  declare_object_locals_class(wto, parsed_extensions);
  wto << "\n";
  declare_extension_casts(wto, parsed_extensions);
  wto << "}\n\n";

  generate_robertvecs(state.parsed_objects);

  // TODO(JoshDreamland): Replace with enigma_user:
  wto << "namespace enigma // TODO: Replace with enigma_user\n{\n";
  write_object_class_bodies(lcpp, wto, game, state);
  wto << "}\n\n";

  wto << "namespace enigma {\n";
  write_object_data_structs(wto, state.parsed_objects);
  wto << "}\n";
  wto.close();
}

static inline void write_script_implementations(ofstream& wto, const GameData &game, const CompileState &state, int mode);
static inline void write_timeline_implementations(ofstream& wto, const GameData &game, const CompileState &state);
static inline void write_event_bodies(ofstream& wto, const GameData &game, int mode, const ParsedObjectVec &parsed_objects, const ScriptLookupMap &script_lookup, const TimelineLookupMap &timeline_lookup);
static inline void write_global_script_array(ofstream &wto, const GameData &game, const CompileState &state);
static inline void write_basic_constructor(ofstream &wto);

// [ CODEGEN FILE ] ------------------------------------------------------------
// Object functionality: implements event routines and scripts declared earlier.
// -----------------------------------------------------------------------------
static inline void write_object_functionality(
    const GameData &game, const CompileState &state, int mode) {
  vector<unsigned> parent_undefined;
  ofstream wto((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h").u8string().c_str(),ios_base::out);

  wto << license;
  wto << endl << "#define log_xor || log_xor_helper() ||" << endl;
  wto << "struct log_xor_helper { bool value; };" << endl;
  wto << "template<typename LEFT> log_xor_helper operator ||(const LEFT &left, const log_xor_helper &xorh) { log_xor_helper nxor; nxor.value = (bool)left; return nxor; }" << endl;
  wto << "template<typename RIGHT> bool operator ||(const log_xor_helper &xorh, const RIGHT &right) { return xorh.value ^ (bool)right; }" << endl << endl;

  write_script_implementations(wto, game, state, mode);
  write_timeline_implementations(wto, game, state);
  write_event_bodies(wto, game, mode, state.parsed_objects, state.script_lookup, state.timeline_lookup);
  write_global_script_array(wto, game, state);
  write_basic_constructor(wto);

  wto.close();
}

static inline void write_script_implementations(ofstream& wto, const GameData &game, const CompileState &state, int mode) {
  // Export globalized scripts
  for (size_t i = 0; i < game.scripts.size(); i++) {
    ParsedScript* scr = state.script_lookup.at(game.scripts[i].name);
    const char* comma = "";
    wto << "variant _SCR_" << game.scripts[i].name << "(";
    for (int argn = 0; argn < scr->globargs; argn++) { //it->second gives max argument count used
      wto << comma << "variant argument" << argn;
      comma = ", ";
    }
    wto << ")\n{\n";
    if (mode == emode_debug) {
      wto << "  enigma::debug_scope $current_scope(\"script '" << game.scripts[i].name << "'\");\n";
    }
    wto << "  ";
    ParsedCode &upev = scr->global_code ? *scr->global_code : scr->code;

    // TODO(JoshDreamland): Super-hacky
    string override_code, override_synt;
    if (upev.code.compare(0, 12, "with((self))") == 0) {
      override_code = upev.code.substr(12);
      override_synt = upev.synt.substr(12);
    }
    print_to_file(
      override_code.empty() ? upev.code : override_code,
      override_synt.empty() ? upev.synt : override_synt,
      upev.strc,
      upev.strs,
      2,wto
    );
    wto << "\n  return 0;\n}\n\n";
  }
}

static inline void write_timeline_implementations(ofstream& wto, const GameData &game, const CompileState &state) {
  // Export globalized timelines.
  // TODO: Is there such a thing as a localized timeline?
  (void) game;  // XXX: why the hell is this needed for everything but timelines?
  for (const auto &tline : state.timeline_lookup) {\
    for (const auto &moment : tline.second.moments) {
      wto << "void TLINE_" << tline.first << "_MOMENT_" << moment.step << "() {\n";
      ParsedCode& upev = moment.script->global_code
          ? *moment.script->global_code : moment.script->code;

      string override_code, override_synt;
      if (upev.code.compare(0, 12, "with((self))") == 0) {
        override_code = upev.code.substr(12);
        override_synt = upev.synt.substr(12);
      }
      print_to_file(
          override_code.empty() ? upev.code : override_code,
          override_synt.empty() ? upev.synt : override_synt,
          upev.strc,
          upev.strs,
          2, wto);
      wto << "\n}\n\n";
    }
  }
}

static void write_event_func(ofstream& wto, const ParsedEvent &event, string objname, string evname, int mode);
static void write_object_event_funcs(ofstream& wto, const parsed_object *const object, int mode);
static void write_object_script_funcs(ofstream& wto, const parsed_object *const t, const ScriptLookupMap &script_lookup);
static void write_object_timeline_funcs(ofstream& wto, const GameData &game, const parsed_object *const t, const TimelineLookupMap &timeline_lookup);
static void write_can_cast_func(ofstream& wto, const parsed_object *const pobj);

static void write_event_bodies(
    ofstream& wto, const GameData &game, int mode,
    const ParsedObjectVec &parsed_objects, const ScriptLookupMap &script_lookup,
    const TimelineLookupMap &timeline_lookup) {
  for (const auto *obj : parsed_objects) {
    // Write infrastructure to trigger grouped events (stacked/dispatched)
    implement_event_groups(wto, obj);

    // Write the user-defined event implementations.
    write_object_event_funcs(wto, obj, mode);

    // Write local object copies of scripts
    write_object_script_funcs(wto, obj, script_lookup);

    // Write local object copies of timelines
    write_object_timeline_funcs(wto, game, obj, timeline_lookup);

    //Write the required "can_cast()" function.
    write_can_cast_func(wto, obj);
  }
}

static void write_object_event_funcs(ofstream& wto, const parsed_object *const object, int mode) {
  for (const ParsedEvent &event : object->all_events) {
    string evname = event.ev_id.TrueFunctionName();

    // Inherit default code from object_locals. Don't generate the same default
    // code for all objects.
    if (event.code.empty()) continue;

    bool defined_inherited = false;

    // TODO(JoshDreamland): This is a pretty major hack; it's an extra line
    // for no reason 99% of the time, and it doesn't allow us to give any
    // feedback as to why a call to event_inherited() may not be valid.
    if (object->InheritsSpecifically(event.ev_id) &&
        event.code.find("event_inherited") != std::string::npos) {
      wto << "#define event_inherited OBJ_" + object->parent->name + "::myevent_" + evname + "\n";
      defined_inherited = true;
    }

    write_event_func(wto, event, object->name, evname, mode);

    if (defined_inherited) {
      wto << "#undef event_inherited\n";
    }

    if (event.ev_id.HasSubCheck()) {
      // Write event sub check code
      wto << "inline bool enigma::OBJ_" << object->name
          << "::myevent_" << evname << "_subcheck() ";
      if (event.ev_id.HasSubCheckFunction()) {
        wto << event.ev_id.SubCheckFunction();
      } else {
        wto << "{\n  return " << event.ev_id.SubCheckExpression() << ";\n}";
      }
      wto << "\n\n";
    }
  }
}

static void write_event_func(ofstream& wto, const ParsedEvent &event, string objname, string evname, int mode) {
  std::string evfuncname = "myevent_" + evname;
  wto << "variant enigma::OBJ_" << objname << "::" << evfuncname << "()\n{\n";
  if (mode == emode_debug) {
    wto << "  enigma::debug_scope $current_scope(\"event '" << evname << "' for object '" << objname << "'\");\n";
  }
  wto << "  ";
  if (!event.ev_id.UsesEventLoop())
    wto << "enigma::temp_event_scope ENIGMA_PUSH_ITERATOR_AND_VALIDATE(this);\n  ";
  if (event.ev_id.HasConstantCode())
    wto << event.ev_id.ConstantCode() << endl;

  print_to_file(event.code,event.synt,event.strc,event.strs,2,wto);
  wto << "\n  return 0;\n}\n\n";
}

static inline void write_object_script_funcs(ofstream& wto, const parsed_object *const t, const ScriptLookupMap &script_lookup) {
  for (parsed_object::const_funcit it = t->funcs.begin(); it != t->funcs.end(); ++it) { // For each function called by this object
    auto subscr = script_lookup.find(it->first); // Check if it's a script
    if (subscr != script_lookup.end() // If we've got ourselves a script
        and subscr->second->global_code) { // And it has distinct code for use at the global scope (meaning it's more efficient locally)
      const char* comma = "";
      wto << "variant enigma::OBJ_" << t->name << "::_SCR_" << it->first << "(";

      for (int argn = 0; argn < it->second; ++argn) { // it->second gives max argument count used
        wto << comma << "variant argument" << argn;
        comma = ", ";
      }

      wto << ")\n{\n  ";
      print_to_file(subscr->second->code.code,subscr->second->code.synt,subscr->second->code.strc,subscr->second->code.strs,2,wto);
      wto << "\n  return 0;\n}\n\n";
    }
  }
}

static inline void write_known_timelines(ofstream& wto, const GameData &game, const parsed_object *const t, const TimelineLookupMap &timeline_lookup);
static inline void write_object_timeline_funcs(ofstream& wto, const GameData &game, const parsed_object *const t, const TimelineLookupMap &timeline_lookup) {
  bool hasKnownTlines = false;
  for (parsed_object::const_tlineit it = t->tlines.begin(); it != t->tlines.end(); ++it) { //For each timeline potentially set by this object
    auto timit = timeline_lookup.find(it->first); //Check if it's a timeline
    if (timit != timeline_lookup.end()) {  // If we've got ourselves a timeline
      hasKnownTlines = true;  // Apparently we're always writing all timelines to all objects
      for (const auto &moment : timit->second.moments) {
        ParsedScript* scr = moment.script;
        wto << "void enigma::OBJ_" << t->name << "::TLINE_" << timit->first
            << "_MOMENT_" << moment.step << "() {\n";
        print_to_file(scr->code.code, scr->code.synt, scr->code.strc, scr->code.strs, 2, wto);
        wto << "}\n";
      }
      wto << "\n";
    }
  }

  // If no timelines are ever used by this script, it can rely on the default lookup table.
  // NOTE: We have to allow it to fall through to the default in cases where instances (by id) are given a timeline.
  if (hasKnownTlines) {
    write_known_timelines(wto, game, t, timeline_lookup);
  }
}

static inline void write_known_timelines(ofstream& wto, const GameData &game, const parsed_object *const t, const TimelineLookupMap &timeline_lookup) {
  (void) game;  // XXX: why the hell is this needed for everything but timelines?
  wto << "void enigma::OBJ_" << t->name << "::timeline_call_moment_script(int timeline_index, int moment_index) {\n";
  wto << "  switch (timeline_index) {\n";
  for (parsed_object::const_tlineit it = t->tlines.begin(); it != t->tlines.end(); it++) {
    auto timit = timeline_lookup.find(it->first); //Check if it's a timeline
    if (timit != timeline_lookup.end()) { // If we've got ourselves a timeline
      wto << "    case " << timit->second.id << ": {\n";
      wto << "      switch (moment_index) {\n";
      for (size_t j = 0; j < timit->second.moments.size(); ++j) {
        const auto &moment = timit->second.moments[j];
        wto << "        case " << j << ": {\n";
        wto << "          TLINE_" << timit->first << "_MOMENT_" << moment.step << "();\n";
        wto << "          break;\n";
        wto << "        }\n";
      }
      wto << "      }\n";
      wto << "      break;\n";
      wto << "    }\n";
    }
  }
  // Fall through to the default case.
  wto << "    default: event_parent::timeline_call_moment_script(timeline_index, moment_index);\n";
  wto << "  }\n";
  wto << "}\n\n";
}

static inline void write_can_cast_func(ofstream& wto, const parsed_object *const pobj) {
  wto << "bool enigma::OBJ_" << pobj->name << "::can_cast(int obj) const {\n";
  bool written = false;
  wto << "  return ";
  for (parsed_object* curr=pobj->parent; curr; curr=curr->parent) {
    if (written) wto << " || ";
    wto << "(obj==" << curr->id << ")";
    written = true;
  }
  if (!written) wto << "false";
  wto << ";\n" << "}\n\n";
}

static inline void write_global_script_array(ofstream &wto, const GameData &game, const CompileState &state) {
  wto << "namespace enigma\n{\n"
  "  std::vector<callable_script> callable_scripts = {\n";
  int scr_count = 0;
  for (size_t i = 0; i < game.scripts.size(); i++)
  {
    while (game.scripts[i].id() > scr_count)
    {
        wto << "    { NULL, -1 },\n";
        scr_count++;
    }
    scr_count++;
    wto << "    { (variant(*)())_SCR_" << game.scripts[i].name << ", "
        << state.script_lookup.at(game.scripts[i].name)->globargs << " },\n";
  }
  wto << "  };\n  \n";
}

static inline void write_basic_constructor(ofstream &wto) {
  wto <<
      "  void constructor(object_basic* instance_b) {\n"
      "    //This is the universal create event code\n"
      "    object_locals* instance = (object_locals*)instance_b;\n"
      "    \n"
      "    instance->xstart = instance->x;\n"
      "    instance->ystart = instance->y;\n"
      "    instance->xprevious = instance->x;\n"
      "    instance->yprevious = instance->y;\n"
      "    \n"
      "    instance->gravity=0;\n"
      "    instance->gravity_direction=270;\n"
      "    instance->friction=0;\n    \n"
      "    \n"
      "    instance->timeline_index = -1;\n"
      "    instance->timeline_running = " << (setting::compliance_mode <= setting::COMPL_GM7? "true" : "false") << ";\n"
      "    instance->timeline_speed = 1;\n"
      "    instance->timeline_position = 0;\n"
      "    instance->timeline_loop = false;\n"
      "    \n"
      "    instance->image_alpha = 1.0;\n"
      "    instance->image_angle = 0;\n"
      "    instance->image_blend = 0xFFFFFF;\n"
      "    instance->image_index = 0;\n"
      "    instance->image_speed  = 1;\n"
      "    instance->image_xscale = 1;\n"
      "    instance->image_yscale = 1;\n"
      "    \n"
      "    instance_count++;\n"
      "  }\n"
      "}\n";
}

int lang_CPP::compile_writeObjectData(const GameData &game, const CompileState &state, int mode) {
  write_object_declarations(this, game, state);
  write_object_functionality(game, state, mode);
  return 0;
}
