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

inline string event_forge_group_code(int mainId, int id) {
  string evname = event_get_function_name(mainId,id);
  string ret = event_has_sub_check(mainId, id) ? "        if (myevent_" + evname + "_subcheck()) {\n" : "";
  ret += (event_has_super_check(mainId,id) ?
    "          if (" + event_get_super_check_condition(mainId,id) + ") myevent_" : "          myevent_") + evname + "();\n",
  ret += event_has_sub_check(mainId,id) ? "        }\n" : "";
  return ret;
}

static inline void declare_scripts(std::ostream &wto, const GameData &game, const CompileState &state) {
  wto << "// Script identifiers\n";
  for (size_t i = 0; i < game.scripts.size(); i++)
    wto << "#define " << game.scripts[i].name << "(...) _SCR_" << game.scripts[i].name << "(__VA_ARGS__)\n";
  wto << "\n\n";

  for (size_t i = 0; i < game.scripts.size(); i++) {
    parsed_script* scr = state.parsed_scripts[i];
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
static inline void compute_locals(lang_CPP *lcpp, parsed_object *object, const string addls) {
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
      (lcpp->find_typename(tn) ? type : name) = tn;
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
      if (it->first == decl->first && it->second.prefix == decl->second.prefix && it->second.type == decl->second.type && it->second.suffix == decl->second.suffix) {
        return true;
      }
    }
  }
  return false;
}

static inline bool parent_declares_event(parsed_object *parent, int mid, int sid) {
  for (parsed_object *obj = parent; obj != NULL; obj = obj->parent) {
    for (unsigned i = 0; i < obj->events.size; i++) {
      if (obj->events[i].mainId == mid && obj->events[i].id == sid && (obj->events[i].code.length() > 0
          || event_has_suffix_code(mid, sid) || event_has_prefix_code(mid, sid) || event_has_const_code(mid, sid) || event_has_default_code(mid,sid)
          || event_has_iterator_unlink_code(mid,sid) || event_has_iterator_delete_code(mid,sid) || event_has_iterator_declare_code(mid,sid) ||
      event_has_iterator_initialize_code(mid,sid))) {
          return true;
      }
    }
  }
  return false;
}

static inline bool parent_declares_groupedevent(parsed_object *parent, int mid) {
  for (parsed_object *obj = parent; obj != NULL; obj = obj->parent) {
    for (unsigned i = 0; i < obj->events.size; i++) {
      if (obj->events[i].mainId == mid && !obj->events[i].code.empty() && event_is_instance(obj->events[i].mainId, obj->events[i].id)) {
        return true;
      }
    }
  }
  return false;
}

static inline void write_object_locals(lang_CPP *lcpp, std::ostream &wto, const parsed_object* global, parsed_object* object) {
  wto << "    // Local variables\n    ";
  for (unsigned ii = 0; ii < object->events.size; ii++) {
    string addls = event_get_locals(object->events[ii].mainId, object->events[ii].id);
    if (addls.length()) {
      compute_locals(lcpp, object, addls);
    }
  }

  for (deciter ii =  object->locals.begin(); ii != object->locals.end(); ii++) {
    bool writeit = true; // Whether this "local" should be declared such
    if (parent_declares(object->parent, ii)) {
      continue;
    }

    // If it's not explicitely defined, we must question whether it should be given a unique presence in this scope
    if (!ii->second.defined()) {
      parsed_object::cglobit ve = global->globals.find(ii->first); // So, we look for a global by this name
      if (ve != global->globals.end()) {  // If a global by this name is indeed found,
        if (ve->second.defined()) // And this global is explicitely defined, not just accessed with a dot,
          writeit = false; // We assume that its definition will cover us, and we do not redeclare it as a local.
        cout << "enigma: scopedebug: variable `" << ii->first << "' from object `" << object->name
             << "' will be used from the " << (writeit ? "object" : "global") << " scope." << endl;
      }
    }
    if (writeit) {
      wto << tdefault(ii->second.type) << " " << ii->second.prefix << ii->first << ii->second.suffix << ";\n    ";
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
    and subscr->second->pev_global) { // And it has distinct code for use at the global scope (meaning it's more efficient locally)
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

static inline void write_object_timelines(std::ostream &wto, const GameData &game, parsed_object *object, const TimelineLookupMap &timeline_lookup) {
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

// TODO(JoshDreamland): What the FUCK are these? Really, burn all five of them.
typedef vector<unsigned> robertvec;
typedef map<int, robertvec> robertmap;
typedef vector<int> event_vec;
typedef map<int, event_vec> event_map;
typedef map<int, event_map> evpairmap;
static inline void write_object_events(std::ostream &wto, parsed_object *object, robertvec &parent_undefined, event_map &evgroup) {
  // Now we output all the events this object uses
  // Defaulted events were already added into this array.
  for (unsigned i = 0; i < object->events.size; i++) {
    // If the parent also wrote this grouped event for instance some input events in the parent and some in the child, then we need to call the super method
    if (!parent_declares_event(object->parent, object->events[i].mainId, object->events[i].id)) {
      parent_undefined.push_back(i);
    }
    string evname = event_get_function_name(object->events[i].mainId, object->events[i].id);
    if  (!object->events[i].code.empty())
    {
      if (event_is_instance(object->events[i].mainId, object->events[i].id)) {
        evgroup[object->events[i].mainId].push_back(i);
      }
      wto << "    variant myevent_" << evname << "();\n";
    }

    if  (!object->events[i].code.empty() || event_has_default_code(object->events[i].mainId, object->events[i].id)) {
      if (event_has_sub_check(object->events[i].mainId, object->events[i].id)) {
        wto << "    inline bool myevent_" << evname << "_subcheck();\n";
      }
    }
  }
}

// TODO(JoshDreamland): ...
// It seems to do this in a way that makes crying babies weep. It looks as though this used to be a much simpler block, and Robert tried to make it work with inheritance.
// Did he even succeed? It looks as though parent groups will always be called... or will cause a compile error if overridden.
// Robert: This also needs fixed to sort the events so they don't fire out of order, eg. alarm 0, 2 from child and then alarm 1 from the parent
/// This function writes event group functions for stacked events, such as keyboard and mouse events, so that only one call is needed to invoke all key kinds.
static inline void write_stacked_event_groups(std::ostream &wto, parsed_object *object, event_map &evgroup, const evpairmap &evmap) {
  wto << "      \n      // Grouped event bases\n";
  event_vec parentremains;
  for (event_map::const_iterator it = evgroup.begin(); it != evgroup.end(); it++) {
    int mid = it->first;
    wto << "      void myevent_" << event_stacked_get_root_name(mid) << "()\n      {\n";
    // we have to write all the events we have even if we override them
    for (event_vec::const_iterator vit = it->second.begin(); vit != it->second.end(); vit++) {
      int id = object->events[*vit].id;
      wto << event_forge_group_code(mid, id);
    }
    // we also have to write all the parent events for this main id but only if we didn't already write it
    if (object->parent) {
        for (parsed_object *obj = object->parent; obj != NULL; obj = obj->parent) {
          evpairmap::const_iterator tt = evmap.find(obj->id);
          if (tt == evmap.end()) continue;
          for (event_map::const_iterator pit = tt->second.begin(); pit != tt->second.end(); pit++) {
            int pmid = pit->first;
            if (pmid == mid) {
              for (event_vec::const_iterator pvit = pit->second.begin(); pvit != pit->second.end(); pvit++) {
                int pid = obj->events[*pvit].id;
                bool found = false;
                for (event_vec::const_iterator vit = it->second.begin(); vit != it->second.end(); vit++) {
                  int id = object->events[*vit].id;
                  if (id == pid) { found = true; break; }
                }
                // if we did not already write it for this object but the parent has it then we can still write it
                if (!found) {
                  wto << event_forge_group_code(pmid, pid) << "\n";
                }
              }
            }
          }
        }
    }
    wto << "      }\n";
  }
}


static inline void write_event_perform(std::ostream &wto, parsed_object *object) {
  /* Event Perform Code */
  wto << "      // Event Perform Code\n";
  wto << "      variant myevents_perf(int type, int numb)\n      {\n";

  for (unsigned ii = 0; ii < object->events.size; ii++) {
    if  (!object->events[ii].code.empty())
    {
      //Look up the event name
      string evname = event_get_function_name(object->events[ii].mainId, object->events[ii].id);
      wto << "        if (type == " << object->events[ii].mainId << " && numb == " << object->events[ii].id << ")\n";
      wto << "          return myevent_" << evname << "();\n";
    }
  }

  if (object->parent) {
    wto << "        return OBJ_" << object->parent->name << "::myevents_perf(type,numb);\n";
  } else {
    wto << "        return 0;\n";
  }

  wto << "      }\n";
}

// TODO(JoshDreamland): UGH. This function is doing the same thing the other cyclomatic clusterfunc is doing, only it's just emitting an unlink.
// Recode both of these things and actually stash the result.
static inline void write_object_unlink(std::ostream &wto, parsed_object *object, robertvec &parent_undefined, event_map &evgroup) {
  /*
  ** Now we write the callable unlinker. Its job is to disconnect the instance for destroy.
  ** This is an important component that tracks multiple pieces of the instance. These pieces
  ** are created for efficiency. See the instance system documentation for full details.
  */

  // Here we write the pieces it tracks
  wto << "\n    // Self-tracking\n";

  // This tracks components of the instance system.
  if (!object->parent) {
    wto << "      enigma::pinstance_list_iterator ENOBJ_ITER_me;\n";
    for (parsed_object *obj = object; obj; obj = obj->parent) // For this object and each parent thereof
      wto << "      enigma::inst_iter *ENOBJ_ITER_myobj" << obj->id << ";\n"; // Keep track of a pointer to `this` inside this list.
  } else {
    wto << "      enigma::inst_iter *ENOBJ_ITER_myobj" << object->id << ";\n"; // Keep track of a pointer to `this` inside this list.
  }
  // This tracks components of the event system.
  for (vector<unsigned>::iterator it = parent_undefined.begin(); it != parent_undefined.end(); it++) { // Export a tracker for all events
    if (!event_is_instance(object->events[*it].mainId, object->events[*it].id)) { //...well, all events which aren't stacked
      if (event_has_iterator_declare_code(object->events[*it].mainId, object->events[*it].id)) {
        if (!iscomment(event_get_iterator_declare_code(object->events[*it].mainId, object->events[*it].id)))
          wto << "      " << event_get_iterator_declare_code(object->events[*it].mainId, object->events[*it].id) << ";\n";
      } else
        wto << "      enigma::inst_iter *ENOBJ_ITER_myevent_" << event_get_function_name(object->events[*it].mainId, object->events[*it].id) << ";\n";
    }
  }
  for (map<int, vector<int> >::iterator it = evgroup.begin(); it != evgroup.end(); it++) { // The stacked ones should have their root exported
    if (!object->parent || !parent_declares_groupedevent(object->parent, it->first)) {
      wto << "      enigma::inst_iter *ENOBJ_ITER_myevent_" << event_stacked_get_root_name(it->first) << ";\n";
    }
  }

  //This is the actual call to remove the current instance from all linked records before destroying it.
  wto << "\n    void unlink()\n    {\n";
  wto << "      instance_iter_queue_for_destroy(this); // Queue for delete while we're still valid\n";
  wto << "      if (enigma::instance_deactivated_list.erase(id)==0) {\n";
  wto << "        //If it's not in the deactivated list, then it's active (so deactivate it).\n";
  wto << "        deactivate();\n";
  wto << "      }\n";
  wto << "    }\n\n";
  wto << "    void deactivate()\n    {\n";
  if (!object->parent) {
    wto << "      enigma::unlink_main(ENOBJ_ITER_me); // Remove this instance from the non-redundant, tree-structured list.\n";
    for (parsed_object *obj = object; obj; obj = obj->parent)
      wto << "      unlink_object_id_iter(ENOBJ_ITER_myobj" << obj->id << ", " << obj->id << ");\n";
  } else {
      wto << "      OBJ_" << object->parent->name << "::deactivate();\n";
      wto << "      unlink_object_id_iter(ENOBJ_ITER_myobj" << object->id << ", " << object->id << ");\n";
  }
  for (vector<unsigned>::iterator it = parent_undefined.begin(); it != parent_undefined.end(); it++) {
    if (!event_is_instance(object->events[*it].mainId, object->events[*it].id)) {
      const string evname = event_get_function_name(object->events[*it].mainId, object->events[*it].id);
      if (event_has_iterator_unlink_code(object->events[*it].mainId, object->events[*it].id)) {
        if (!iscomment(event_get_iterator_unlink_code(object->events[*it].mainId, object->events[*it].id)))
          wto << "      " << event_get_iterator_unlink_code(object->events[*it].mainId, object->events[*it].id) << ";\n";
      } else
        wto << "      enigma::event_" << evname << "->unlink(ENOBJ_ITER_myevent_" << evname << ");\n";
    }
  }

  for (map<int, vector<int> >::iterator it = evgroup.begin(); it != evgroup.end(); it++) { // The stacked ones should have their root exported
    if (!object->parent || !parent_declares_groupedevent(object->parent, it->first)) {
      wto << "      enigma::event_" << event_stacked_get_root_name(it->first) << "->unlink(ENOBJ_ITER_myevent_" << event_stacked_get_root_name(it->first) << ");\n";
    }
  }
  wto << "    }\n";
}

static inline void write_object_constructors(std::ostream &wto, parsed_object *object, robertvec &parent_undefined, event_map &evgroup) {
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
  for (vector<unsigned>::iterator it = parent_undefined.begin(); it != parent_undefined.end(); it++) {
    if (!event_is_instance(object->events[*it].mainId, object->events[*it].id)) {
      const string evname = event_get_function_name(object->events[*it].mainId, object->events[*it].id);
      if (event_has_iterator_initialize_code(object->events[*it].mainId, object->events[*it].id)) {
        if (!iscomment(event_get_iterator_initialize_code(object->events[*it].mainId, object->events[*it].id)))
          wto << "      " << event_get_iterator_initialize_code(object->events[*it].mainId, object->events[*it].id) << ";\n";
      } else {
        wto << "      ENOBJ_ITER_myevent_" << evname << " = enigma::event_" << evname << "->add_inst(this);\n";
      }
    }
  }
  for (map<int, vector<int> >::iterator it = evgroup.begin(); it != evgroup.end(); it++) { // The stacked ones should have their root exported
    if (!object->parent || !parent_declares_groupedevent(object->parent, it->first)) {
      wto << "      ENOBJ_ITER_myevent_" << event_stacked_get_root_name(it->first) << " = enigma::event_" << event_stacked_get_root_name(it->first) << "->add_inst(this);\n";
    }
  }
  wto << "    }\n";
}

static inline void write_object_destructor(std::ostream &wto, parsed_object *object, robertvec &parent_undefined, event_map &evgroup) {
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
  for (vector<unsigned>::iterator it = parent_undefined.begin(); it != parent_undefined.end(); it++) {
    if (!event_is_instance(object->events[*it].mainId, object->events[*it].id)) {
      if (event_has_iterator_delete_code(object->events[*it].mainId, object->events[*it].id)) {
        if (!iscomment(event_get_iterator_delete_code(object->events[*it].mainId, object->events[*it].id)))
          wto << "      " << event_get_iterator_delete_code(object->events[*it].mainId, object->events[*it].id) << ";\n";
      } else
        wto << "      delete ENOBJ_ITER_myevent_" << event_get_function_name(object->events[*it].mainId, object->events[*it].id) << ";\n";
    }
  }
  for (map<int, vector<int> >::iterator it = evgroup.begin(); it != evgroup.end(); it++) { // The stacked ones should have their root exported
    if (!object->parent || !parent_declares_groupedevent(object->parent, it->first)) {
      wto << "      delete ENOBJ_ITER_myevent_" << event_stacked_get_root_name(it->first) << ";\n";
    }
  }
  wto << "    }\n";

  //We'll sneak this in here.
  wto << "    virtual bool can_cast(int obj) const;\n";
}

static inline void write_object_class_body(parsed_object* object, lang_CPP *lcpp, std::ostream &wto, const GameData &game, const CompileState &state, robertmap &parent_undefinitions, evpairmap &evmap) {
  wto << "  \n  struct OBJ_" << object->name;
  if (object->parent) {
      wto << ": OBJ_" << object->parent->name;
  } else {
      wto << ": object_locals";
  }
  wto << "\n  {\n";

  robertvec parent_undefined; // Robert probably knew what this was when he wrote it. Probably.
  event_map evgroup; // Josh knew what this was when he wrote it.

  write_object_locals(lcpp, wto, &state.global_object, object);
  write_object_scripts(wto, object, state);
  write_object_timelines(wto, game, object, state.timeline_lookup);
  write_object_events(wto, object, parent_undefined, evgroup);

  parent_undefinitions[object->id] = parent_undefined;
  evmap[object->id] = evgroup;

  write_stacked_event_groups(wto, object, evgroup, evmap);
  write_event_perform(wto, object);
  write_object_unlink(wto, object, parent_undefined, evgroup);
  write_object_constructors(wto, object, parent_undefined, evgroup);
  write_object_destructor(wto, object, parent_undefined, evgroup);

  wto << "  };\n";
}

static inline void write_object_family(parsed_object* object, lang_CPP *lcpp, std::ostream &wto, const GameData &game, const CompileState &state, robertmap &parent_undefinitions, evpairmap &evmap) {
  write_object_class_body(object, lcpp, wto, game, state, parent_undefinitions, evmap);
  for (ParsedObjectVec::iterator child_it = object->children.begin(); child_it != object->children.end(); ++child_it) {
    write_object_family(*child_it, lcpp, wto, game, state, parent_undefinitions, evmap);
  }
}

static inline void write_object_class_bodies(lang_CPP *lcpp, std::ostream &wto, const GameData &game, const CompileState &state, robertmap &parent_undefinitions) {
  // Hold an iterator for our parent for later usage
  evpairmap evmap; // Keep track of events that need added to honor et_stacked

  for (parsed_object *object : state.parsed_objects) {
    if (object->parent) {
      continue; // Do not write out objects before we've written their parent
    }

    write_object_family(object, lcpp, wto, game, state, parent_undefinitions, evmap);
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

static inline void write_object_declarations(lang_CPP* lcpp, const GameData &game, const CompileState &state, robertmap &parent_undefinitions) {
  //NEXT FILE ----------------------------------------
  //Object declarations: object classes/names and locals.
  ofstream wto;
  wto.open((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h").c_str(),ios_base::out);
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

  // TODO(JoshDreamland): Replace with enigma_user:
  wto << "namespace enigma // TODO: Replace with enigma_user\n{\n";
  write_object_class_bodies(lcpp, wto, game, state, parent_undefinitions);
  wto << "}\n\n";

  wto << "namespace enigma {\n";
  write_object_data_structs(wto, state.parsed_objects);
  wto << "}\n";
  wto.close();
}

static inline void write_script_implementations(ofstream& wto, const GameData &game, const CompileState &state, int mode);
static inline void write_timeline_implementations(ofstream& wto, const GameData &game, const CompileState &state);
static inline void write_event_bodies(ofstream& wto, const GameData &game, int mode, const ParsedObjectVec &parsed_objects, robertmap &parent_undefinitions, const ScriptLookupMap &script_lookup, const TimelineLookupMap &timeline_lookup);
static inline void write_global_script_array(ofstream &wto, const GameData &game, const CompileState &state);
static inline void write_basic_constructor(ofstream &wto);

static inline void write_object_functionality(const GameData &game, const CompileState &state, int mode, robertmap &parent_undefinitions) {
  vector<unsigned> parent_undefined;
  ofstream wto((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h").c_str(),ios_base::out);

  wto << license;
  wto << endl << "#define log_xor || log_xor_helper() ||" << endl;
  wto << "struct log_xor_helper { bool value; };" << endl;
  wto << "template<typename LEFT> log_xor_helper operator ||(const LEFT &left, const log_xor_helper &xorh) { log_xor_helper nxor; nxor.value = (bool)left; return nxor; }" << endl;
  wto << "template<typename RIGHT> bool operator ||(const log_xor_helper &xorh, const RIGHT &right) { return xorh.value ^ (bool)right; }" << endl << endl;

  write_script_implementations(wto, game, state, mode);
  write_timeline_implementations(wto, game, state);
  write_event_bodies(wto, game, mode, state.parsed_objects, parent_undefinitions, state.script_lookup, state.timeline_lookup);
  write_global_script_array(wto, game, state);
  write_basic_constructor(wto);

  wto.close();
}

static inline void write_script_implementations(ofstream& wto, const GameData &game, const CompileState &state, int mode) {
  // Export globalized scripts
  for (size_t i = 0; i < game.scripts.size(); i++) {
    parsed_script* scr = state.script_lookup.at(game.scripts[i].name);
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
    parsed_event& upev = scr->pev_global?*scr->pev_global:scr->pev;

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
  for (const auto &tline : state.timeline_lookup) {\
    for (const auto &moment : tline.second.moments) {
      wto << "void TLINE_" << tline.first << "_MOMENT_" << moment.step << "() {\n";
      parsed_event& upev = moment.script->pev_global
          ? *moment.script->pev_global : moment.script->pev;

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

static inline void write_object_script_funcs(ofstream& wto, const parsed_object *const t, const ScriptLookupMap &script_lookup);
static inline void write_object_timeline_funcs(ofstream& wto, const GameData &game, const parsed_object *const t, const TimelineLookupMap &timeline_lookup);
static inline void write_object_event_funcs(ofstream& wto, const parsed_object *const object, int mode, const robertmap &parent_undefinitions);
static inline void write_can_cast_func(ofstream& wto, const parsed_object *const pobj);

static inline void write_event_bodies(ofstream& wto, const GameData &game, int mode, const ParsedObjectVec &parsed_objects, robertmap &parent_undefinitions, const ScriptLookupMap &script_lookup, const TimelineLookupMap &timeline_lookup) {
  // Export everything else
  for (const auto *obj : parsed_objects) {
    write_object_event_funcs(wto, obj, mode, parent_undefinitions);

    //Write local object copies of scripts
    write_object_script_funcs(wto, obj, script_lookup);

    // Write local object copies of timelines
    write_object_timeline_funcs(wto, game, obj, timeline_lookup);

    //Write the required "can_cast()" function.
    write_can_cast_func(wto, obj);
  }
}

static inline void write_event_func(ofstream& wto, const parsed_event &event, string objname, string evname, int mode);
static inline void write_object_event_funcs(ofstream& wto, const parsed_object *const object, int mode, const robertmap &parent_undefinitions) {
  const vector<unsigned> &parent_undefined = parent_undefinitions.find(object->id)->second;
  for (unsigned ii = 0; ii < object->events.size; ii++) {
    const parsed_event &event = object->events[ii];
    const int mid = event.mainId, id = event.id;
    string evname = event_get_function_name(mid, id);
    if (event.code.size()) {
      bool defined_inherited = false;

      // TODO(JoshDreamland): This is a pretty major hack; it's an extra line for no reason nine times in ten,
      // and it doesn't allow us to give feedback as to why a call to event_inherited() may not be valid.
      if (object->parent && std::find(parent_undefined.begin(), parent_undefined.end(), ii) == parent_undefined.end()) {
        wto << "#define event_inherited OBJ_" + object->parent->name + "::myevent_" + evname + "\n";
        defined_inherited = true;
      }

      write_event_func(wto, event, object->name, evname, mode);

      if (defined_inherited) {
        wto << "#undef event_inherited\n";
      }
    }

    if  (event.code.size() || event_has_default_code(mid, id)) {
      // Write event sub check code
      if (event_has_sub_check(mid, id)) {
        wto << "inline bool enigma::OBJ_" << object->name << "::myevent_" << evname << "_subcheck()\n{\n  ";
        cout << "DBGMSG 4-3" << endl;
        wto << event_get_sub_check_condition(mid, id) << endl;
        wto << "\n}\n\n";
      }
    }
  }
}

static inline void write_event_func(ofstream& wto, const parsed_event &event, string objname, string evname, int mode) {
  const int mid = event.mainId, id = event.id;
  wto << "variant enigma::OBJ_" << objname << "::myevent_" << evname << "()\n{\n";
  if (mode == emode_debug) {
    wto << "  enigma::debug_scope $current_scope(\"event '" << evname << "' for object '" << objname << "'\");\n";
  }
  wto << "  ";
  if (!event_execution_uses_default(event.mainId,event.id))
    wto << "enigma::temp_event_scope ENIGMA_PUSH_ITERATOR_AND_VALIDATE(this);\n  ";
  if (event_has_const_code(mid, id))
    wto << event_get_const_code(mid, id) << endl;
  if (event_has_prefix_code(mid, id))
    wto << event_get_prefix_code(mid, id) << endl;

  print_to_file(event.code,event.synt,event.strc,event.strs,2,wto);
  if (event_has_suffix_code(mid, id))
    wto << event_get_suffix_code(mid, id) << endl;
  cout << "DBGMSG 4-5" << endl;
  wto << "\n  return 0;\n}\n\n";
}

static inline void write_object_script_funcs(ofstream& wto, const parsed_object *const t, const ScriptLookupMap &script_lookup) {
  for (parsed_object::const_funcit it = t->funcs.begin(); it != t->funcs.end(); ++it) { // For each function called by this object
    auto subscr = script_lookup.find(it->first); // Check if it's a script
    if (subscr != script_lookup.end() // If we've got ourselves a script
        and subscr->second->pev_global) { // And it has distinct code for use at the global scope (meaning it's more efficient locally)
      const char* comma = "";
      wto << "variant enigma::OBJ_" << t->name << "::_SCR_" << it->first << "(";

      for (int argn = 0; argn < it->second; ++argn) { // it->second gives max argument count used
        wto << comma << "variant argument" << argn;
        comma = ", ";
      }

      wto << ")\n{\n  ";
      print_to_file(subscr->second->pev.code,subscr->second->pev.synt,subscr->second->pev.strc,subscr->second->pev.strs,2,wto);
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
        parsed_script* scr = moment.script;
        wto << "void enigma::OBJ_" << t->name << "::TLINE_" << timit->first
            << "_MOMENT_" << moment.step << "() {\n";
        print_to_file(scr->pev.code, scr->pev.synt, scr->pev.strc, scr->pev.strs, 2, wto);
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
  wto << "  return false";
  for (parsed_object* curr=pobj->parent; curr; curr=curr->parent) {
    wto << " || (obj==" << curr->id << ")";
  }
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
  robertmap parent_undefinitions;
  write_object_declarations(this, game, state, parent_undefinitions);
  write_object_functionality(game, state, mode, parent_undefinitions);
  return 0;
}
