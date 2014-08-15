/** Copyright (C) 2008, 2014 Josh Ventura
*** Copyright (C) 2013, 2014, Robert B. Colton
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

#include "makedir.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;


#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "compiler/compile_common.h"
#include "compiler/event_reader/event_parser.h"
#include "general/parse_basics_old.h"
#include "settings.h"

#include "languages/lang_CPP.h"

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
  string ret = event_has_sub_check(mainId, id) ? "          if (myevent_" + evname + "_subcheck()) {\n" : "";
  ret += (event_has_super_check(mainId,id) ?
    "        if (" + event_get_super_check_condition(mainId,id) + ") myevent_" : "            myevent_") + evname + "();\n",
  ret += event_has_sub_check(mainId,id) ? "          }\n" : "";
  return ret;
}

static inline void declare_scripts(std::ostream &wto, EnigmaStruct* es) {
  wto << "// Script identifiers\n";
  for (int i = 0; i < es->scriptCount; i++)
    wto << "#define " << es->scripts[i].name << "(arguments...) _SCR_" << es->scripts[i].name << "(arguments)\n";
  wto << "\n\n";

  for (int i = 0; i < es->scriptCount; i++)
  {
    parsed_script* scr = scr_lookup[es->scripts[i].name];
    const char* comma = "";
    wto << "variant _SCR_" << es->scripts[i].name << "(";
    scr->globargs = 16; //Fixes too many arguments error (scripts can be called dynamically with any number of arguments)
    for (int argn = 0; argn < scr->globargs; argn++) {
      wto << comma << "variant argument" << argn << "=0";
      comma = ", ";
    }
    wto << ");\n";
  }
  wto << "\n\n";
}

static inline void declare_timelines(std::ostream &wto, EnigmaStruct* es, map<string, int> &revTlineLookup) {
  //Write timeline/moment names. Timelines are like scripts, but we don't have to worry about arguments or return types.
  for (int i=0; i<es->timelineCount; i++) {
    revTlineLookup[es->timelines[i].name] = es->timelines[i].id;
    for (int j=0; j<es->timelines[i].momentCount; j++) {
      wto << "void TLINE_" <<es->timelines[i].name <<"_MOMENT_" <<es->timelines[i].moments[j].stepNo <<"();\n";
    }
  }
}

static inline void declare_extension_casts(std::ostream &wto) {
  // Write extension cast methods; these are a temporary fix until the new instance system is in place.
  wto << "  namespace extension_cast {\n";
  for (unsigned i = 0; i < parsed_extensions.size(); i++) {
    if (parsed_extensions[i].implements != "") {
      wto << "    " << parsed_extensions[i].implements << " *as_" << parsed_extensions[i].implements << "(object_basic* x) {\n";
      wto << "      return (" << parsed_extensions[i].implements << "*)(object_locals*)x;\n";
      wto << "    }\n";
    }
  }
  wto << "  }\n";
}

    //Build a reverse lookup for timeline names.
    map<string, int> revTlineLookup;
    for (int i=0; i<es->timelineCount; i++) {
      revTlineLookup[es->timelines[i].name] = es->timelines[i].id;
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
static inline void write_extension_casts(std::ostream &wto) {
  // Write extension cast methods; these are a temporary fix until the new instance system is in place.
  wto << "namespace enigma {\n";
  wto << "  namespace extension_cast {\n";
  for (unsigned i = 0; i < parsed_extensions.size(); i++) {
    if (parsed_extensions[i].implements != "") {
      wto << "    " << parsed_extensions[i].implements << " *as_" << parsed_extensions[i].implements << "(object_basic* x) {\n";
      wto << "      return (" << parsed_extensions[i].implements << "*)(object_locals*)x;\n";
      wto << "    }\n";
    }
  }
  wto << "  }\n";
  wto << "}\n\n";
}

// TODO(JoshDreamland): Burn this function into ash and launch the ashes into space
static inline void compute_locals(lang_CPP *lcpp, po_i &object_iter, const po_i &parent_iter, const string addls) {
  pt pos;
  string type, name, pres, sufs;
  for (pos = 0; pos < addls.length(); pos++)
  {
    if (is_useless(addls[pos])) continue;
    if (addls[pos] == ';') { object_iter->second->locals[name] = dectrip(type, pres, sufs); type = pres = sufs = ""; continue; }
    if (addls[pos] == ',') { object_iter->second->locals[name] = dectrip(type, pres, sufs); pres = sufs = ""; continue; }
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
      if (setting::inherit_objects && parent_iter != parsed_objects.end()) {
        for (po_i her = object_iter; her != parsed_objects.end(); her = parsed_objects.find(her->second->parent)) {
          for (size_t j = 0; j < her->second->initializers.size(); j++) {
            if (her->second->initializers[j].first == name) {
              redundant = true;
              break;
            }
          }
          if (redundant) {
            break;
          }
        }
      } else {
        for (size_t j = 0; j < object_iter->second->initializers.size(); j++) {
          if (object_iter->second->initializers[j].first == name) {
            redundant = true;
            break;
          }
        }
      }
      if (!redundant) {
        object_iter->second->initializers.push_back(initpair(name,addls.substr(spos,pos-spos)));
      }
      pos--; continue;
    }
  }
}

static inline bool parent_declares(const po_i parent, const deciter decl) {
  for (po_i her = parent; her != parsed_objects.end(); her = parsed_objects.find(her->second->parent)) {
    for (deciter it =  her->second->locals.begin(); it != her->second->locals.end(); it++) {
      if (it->first == decl->first && it->second.prefix == decl->second.prefix && it->second.type == decl->second.type && it->second.suffix == decl->second.suffix) {
        return true;
      }
    }
  }
  return false;
}

        // Next, we write the list of all the timelines this object will hoard a copy of for itself.
        // NOTE: See below; we actually need to assume this object has the potential to call any timeline. 
        //       BUT we only locally-copy the ones we know about for sure here.
        bool hasKnownTlines = false;
        wto << "\n    //Timelines called by this object\n    ";
        for (parsed_object::tlineit it = t->tlines.begin(); it != t->tlines.end(); it++) //For each timeline potentially set by this object.
        {
          map<string, int>::iterator timit = revTlineLookup.find(it->first); //Check if it's a timeline
          if (timit != revTlineLookup.end()) // If we've got ourselves a script
          //and subscr->second->pev_global) // And it has distinct code for use at the global scope (meaning it's more efficient locally) //NOTE: It seems all timeline MUST be copied locally.
          {
            hasKnownTlines = true;
            for (int j=0; j<es->timelines[timit->second].momentCount; j++) {
              wto << "void TLINE_" <<es->timelines[timit->second].name <<"_MOMENT_" <<es->timelines[timit->second].moments[j].stepNo <<"();\n    ";
            }
          }
        } wto << "\n    ";

        //If at least one timeline is called by this object, override timeline_call_moment_script() to properly dispatch it to the local instance.
        if (hasKnownTlines) {
          wto <<"//Dispatch timelines properly for this object..\n    ";
          wto <<"virtual void timeline_call_moment_script(int timeline_index, int moment_index);\n\n    ";
        }

// TODO(JoshDreamland): ...
// It seems to do this in a way that makes crying babies weep. It looks as though this used to be a much simpler block, and Robert tried to make it work with inheritance.
// Did he even succeed? It looks as though parent groups will always be called... or will cause a compile error if overridden.
/// This function writes event group functions for stacked events, such as keyboard and mouse events, so that only one call is needed to invoke all key kinds.
static inline void write_stacked_event_groups(std::ostream &wto, po_i &object_iter, const po_i parent, event_map &evgroup, const evpairmap &evmap) {
  wto << "      \n      // Grouped event bases\n";
  event_vec parentremains;
  for (event_map::const_iterator it = evgroup.begin(); it != evgroup.end(); it++) {
    int mid = it->first;
    wto << "      void myevent_" << event_stacked_get_root_name(mid) << "()\n      {\n";
    for (event_vec::const_iterator vit = it->second.begin(); vit != it->second.end(); vit++) {
      int id = object_iter->second->events[*vit].id;
      wto << event_forge_group_code(mid, id);
      if (setting::inherit_objects && parent != parsed_objects.end()) {
        for (po_i her = parent; her != parsed_objects.end(); her = parsed_objects.find(her->second->parent)) {
          evpairmap::const_iterator tt = evmap.find(her->second->id);
          if (tt == evmap.end()) continue;
          for (event_map::const_iterator pit = tt->second.begin(); pit != tt->second.end(); pit++) {
            int pmid = pit->first;
            if (pmid == mid) {
              for (event_vec::const_iterator pvit = pit->second.begin(); pvit != pit->second.end(); pvit++) {
                int pid = her->second->events[*pvit].id;
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


static inline void write_event_perform(std::ostream &wto, po_i &object_iter, const po_i parent) {
  /* Event Perform Code */
  wto << "      // Event Perform Code\n";
  wto << "      variant myevents_perf(int type, int numb)\n      {\n";

  for (unsigned ii = 0; ii < object_iter->second->events.size; ii++) {
    if  (object_iter->second->events[ii].code != "")
    {
      //Look up the event name
      string evname = event_get_function_name(object_iter->second->events[ii].mainId, object_iter->second->events[ii].id);
      wto << "        if (type == " << object_iter->second->events[ii].mainId << " && numb == " << object_iter->second->events[ii].id << ")\n";
      wto << "          return myevent_" << evname << "();\n";
    }
  }

  if (setting::inherit_objects && parent != parsed_objects.end()) {
    wto << "          return OBJ_" << parent->second->name << "::myevents_perf(type,numb);\n";
  } else {
    wto << "        return 0;\n";
  }
  
  wto << "      }\n";
}

// TODO(JoshDreamland): UGH. This function is doing the same thing the other cyclomatic clusterfunc is doing, only it's just emitting an unlink.
// Recode both of these things and actually stash the result.
static inline void write_object_unlink(std::ostream &wto, po_i &object_iter, po_i parent, robertvec &parent_undefined, event_map &evgroup) {
  /*
  ** Now we write the callable unlinker. Its job is to disconnect the instance for destroy.
  ** This is an important component that tracks multiple pieces of the instance. These pieces
  ** are created for efficiency. See the instance system documentation for full details.
  */

  // Here we write the pieces it tracks
  wto << "\n    // Self-tracking\n";

  // This tracks components of the instance system.
  bool has_parent = (parent != parsed_objects.end());
  if (!setting::inherit_objects || !has_parent) {
    wto << "      enigma::pinstance_list_iterator ENOBJ_ITER_me;\n";
    for (po_i her = object_iter; her != parsed_objects.end(); her = parsed_objects.find(her->second->parent)) // For this object and each parent thereof
      wto << "      enigma::inst_iter *ENOBJ_ITER_myobj" << her->second->id << ";\n"; // Keep track of a pointer to `this` inside this list.
  } else {
    wto << "      enigma::inst_iter *ENOBJ_ITER_myobj" << object_iter->second->id << ";\n"; // Keep track of a pointer to `this` inside this list.
  }
  // This tracks components of the event system.
  for (vector<unsigned>::iterator it = parent_undefined.begin(); it != parent_undefined.end(); it++) { // Export a tracker for all events
    if (!event_is_instance(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)) { //...well, all events which aren't stacked
      if (event_has_iterator_declare_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)) {
        if (!iscomment(event_get_iterator_declare_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)))
          wto << "      " << event_get_iterator_declare_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id) << ";\n";
      } else
        wto << "      enigma::inst_iter *ENOBJ_ITER_myevent_" << event_get_function_name(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id) << ";\n";
    }
  }
  for (map<int, vector<int> >::iterator it = evgroup.begin(); it != evgroup.end(); it++) { // The stacked ones should have their root exported
     wto << "      enigma::inst_iter *ENOBJ_ITER_myevent_" << event_stacked_get_root_name(it->first) << ";\n";
  }

  //This is the actual call to remove the current instance from all linked records before destroying it.
  wto << "\n    void unlink()\n    {\n";
  wto << "      instance_iter_queue_for_destroy(ENOBJ_ITER_me); // Queue for delete while we're still valid\n";
  wto << "      if (enigma::instance_deactivated_list.erase(id)==0) {\n";
  wto << "        //If it's not in the deactivated list, then it's active (so deactivate it).\n";
  wto << "        deactivate();\n";
  wto << "      }\n";
  wto << "    }\n\n";
  wto << "    void deactivate()\n    {\n";
  if (!setting::inherit_objects || !has_parent) { 
    wto << "      enigma::unlink_main(ENOBJ_ITER_me); // Remove this instance from the non-redundant, tree-structured list.\n";
    for (po_i her = object_iter; her != parsed_objects.end(); her = parsed_objects.find(her->second->parent))
      wto << "      unlink_object_id_iter(ENOBJ_ITER_myobj" << her->second->id << ", " << her->second->id << ");\n";
  } else {
      wto << "      OBJ_" << parent->second->name << "::deactivate();\n";
      wto << "      unlink_object_id_iter(ENOBJ_ITER_myobj" << object_iter->second->id << ", " << object_iter->second->id << ");\n";
  }
  for (vector<unsigned>::iterator it = parent_undefined.begin(); it != parent_undefined.end(); it++) {
    if (!event_is_instance(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)) {
      const string evname = event_get_function_name(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id);
      if (event_has_iterator_unlink_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)) {
        if (!iscomment(event_get_iterator_unlink_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)))
          wto << "      " << event_get_iterator_unlink_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id) << ";\n";
      } else
        wto << "      enigma::event_" << evname << "->unlink(ENOBJ_ITER_myevent_" << evname << ");\n";
    }
  }

  for (map<int, vector<int> >::iterator it = evgroup.begin(); it != evgroup.end(); it++) { // The stacked ones should have their root exported
    wto << "      enigma::event_" << event_stacked_get_root_name(it->first) << "->unlink(ENOBJ_ITER_myevent_" << event_stacked_get_root_name(it->first) << ");\n";
  }
  wto << "    }\n";
}

static inline void write_object_constructors(std::ostream &wto, po_i &object_iter, po_i parent, robertvec &parent_undefined, event_map &evgroup) {
  /*
  ** Next are the constructors. One is automated, the other directed.
  **   Automatic constructor:  The constructor generates the ID from a global maximum and links by that alias.
  **   Directed constructor:   Meant for use by the room system, the constructor uses a specified ID alias assumed to have been checked for conflict.
  */
  wto <<   "\n    OBJ_" <<  object_iter->second->name << "(int enigma_genericconstructor_newinst_x = 0, int enigma_genericconstructor_newinst_y = 0, const int id = (enigma::maxid++)" 
  << ", const int enigma_genericobjid = " << object_iter->second->id << ", bool handle = true)";
      
  if (setting::inherit_objects && parsed_objects.find(object_iter->second->parent) != parsed_objects.end()) { 
    wto << ": OBJ_" << parsed_objects.find(object_iter->second->parent)->second->name << "(enigma_genericconstructor_newinst_x,enigma_genericconstructor_newinst_y,id,enigma_genericobjid,false)";
   } else {
    wto << ": object_locals(id,enigma_genericobjid) ";
   }
      
    for (size_t ii = 0; ii < object_iter->second->initializers.size(); ii++)
      wto << ", " << object_iter->second->initializers[ii].first << "(" << object_iter->second->initializers[ii].second << ")";
    wto << "\n    {\n";
    wto << "      if (!handle) return;\n";
      // Sprite index
        if (used_funcs::object_set_sprite) //We want to initialize
          wto << "      sprite_index = enigma::object_table[" << object_iter->second->id << "].->sprite;\n"
              << "      make_index = enigma::object_table[" << object_iter->second->id << "]->mask;\n";
        else
          wto << "      sprite_index = enigma::objectdata[" << object_iter->second->id << "]->sprite;\n"
              << "      mask_index = enigma::objectdata[" << object_iter->second->id << "]->mask;\n";
        wto << "      visible = enigma::objectdata[" << object_iter->second->id << "]->visible;\n      solid = enigma::objectdata[" << object_iter->second->id << "]->solid;\n";
        wto << "      persistent = enigma::objectdata[" << object_iter->second->id << "]->persistent;\n";
        
      wto << "      activate();\n";
        
        
      // Coordinates
        wto << "      x = enigma_genericconstructor_newinst_x, y = enigma_genericconstructor_newinst_y;\n";

    wto << "      enigma::constructor(this);\n";
    wto << "    }\n\n";

  
    bool has_parent = (parent != parsed_objects.end());
    wto << "    void activate()\n    {\n";
      if (setting::inherit_objects && has_parent) {
          wto << "      OBJ_" << parsed_objects.find(object_iter->second->parent)->second->name << "::activate();\n";
          // Have to remove the one the parent added so we can add our own
          wto << "      depth.remove();\n";
      }
    // Depth iterator used for draw events in graphics system screen_redraw
    wto << "      depth.init(enigma::objectdata[" << object_iter->second->id << "]->depth, this);\n";
// Instance system interface
      if (!setting::inherit_objects || !has_parent) {
  wto << "      ENOBJ_ITER_me = enigma::link_instance(this);\n";
        for (po_i her = object_iter; her != parsed_objects.end(); her = parsed_objects.find(her->second->parent))
    wto << "      ENOBJ_ITER_myobj" << her->second->id << " = enigma::link_obj_instance(this, " << her->second->id << ");\n";
      } else {
          wto << "      ENOBJ_ITER_myobj" << object_iter->second->id << " = enigma::link_obj_instance(this, " << object_iter->second->id << ");\n";
      }
      // Event system interface
      for (vector<unsigned>::iterator it = parent_undefined.begin(); it != parent_undefined.end(); it++) {
        if (!event_is_instance(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)) {
          const string evname = event_get_function_name(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id);
          if (event_has_iterator_initialize_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)) {
            if (!iscomment(event_get_iterator_initialize_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)))
              wto << "      " << event_get_iterator_initialize_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id) << ";\n";
          } else {
            wto << "      ENOBJ_ITER_myevent_" << evname << " = enigma::event_" << evname << "->add_inst(this);\n";
          }
        }
      }
    for (map<int, vector<int> >::iterator it = evgroup.begin(); it != evgroup.end(); it++) { // The stacked ones should have their root exported
      wto << "      ENOBJ_ITER_myevent_" << event_stacked_get_root_name(it->first) << " = enigma::event_" << event_stacked_get_root_name(it->first) << "->add_inst(this);\n";
    }
    wto << "    }\n";
}

static inline void write_object_destructor(std::ostream &wto, po_i &object_iter, po_i parent, robertvec &parent_undefined, event_map &evgroup) {
    wto <<   "    \n    ~OBJ_" <<  object_iter->second->name << "()\n    {\n";
      
    bool has_parent = (parent != parsed_objects.end());
      if (!setting::inherit_objects || !has_parent) {
          wto << "      delete vmap;\n";
          wto << "      enigma::winstance_list_iterator_delete(ENOBJ_ITER_me);\n";
          for (po_i her = object_iter; her != parsed_objects.end(); her = parsed_objects.find(her->second->parent))
              wto << "      delete ENOBJ_ITER_myobj" << her->second->id << ";\n";
      } else {
          wto << "      delete ENOBJ_ITER_myobj" << object_iter->second->id << ";\n";
      }
      for (vector<unsigned>::iterator it = parent_undefined.begin(); it != parent_undefined.end(); it++) {
        if (!event_is_instance(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)) {
          if (event_has_iterator_delete_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)) {
            if (!iscomment(event_get_iterator_delete_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id)))
              wto << "      " << event_get_iterator_delete_code(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id) << ";\n";
          } else
            wto << "      delete ENOBJ_ITER_myevent_" << event_get_function_name(object_iter->second->events[*it].mainId, object_iter->second->events[*it].id) << ";\n";
        }
      }
    for (map<int, vector<int> >::iterator it = evgroup.begin(); it != evgroup.end(); it++) { // The stacked ones should have their root exported
      wto << "      delete ENOBJ_ITER_myevent_" << event_stacked_get_root_name(it->first) << ";\n";
    }
    wto << "    }\n";
}

static inline void write_object_class_bodies(lang_CPP *lcpp, std::ostream &wto, EnigmaStruct *es, parsed_object* global, robertmap &parent_undefinitions, map<string, int> &revTlineLookup) {
  po_i object_iter = parsed_objects.begin();
  vector<int> parsed(0);

  // Hold an iterator for our parent for later usage
  po_i parent = parsed_objects.find(object_iter->second->parent);
  evpairmap evmap; // Keep track of events that need added to honor et_stacked

  // TODO(JoshDreamland): This is apparently a Robert-flavored worker queue.
  // Replace with an actual worker queue, or do the right thing and generate
  // an inheritance graph to traverse
  while (parsed.size() < parsed_objects.size()) {
    if (object_iter == parsed_objects.end()) {
      object_iter = parsed_objects.begin();
    }

    // if we have already been written or we have a parent that has not been written, continue
    if (find(parsed.begin(), parsed.end(), object_iter->first) != parsed.end() 
        || (parent != parsed_objects.end()
            && find(parsed.begin(), parsed.end(), object_iter->second->parent) == parsed.end())) { 
      object_iter++;
      continue; 
    }

    wto << "  \n  struct OBJ_" << object_iter->second->name;
    if (setting::inherit_objects && parent != parsed_objects.end()) {
        wto << ": OBJ_" << parent->second->name;
    } else {
        wto << ": object_locals";
    }
    wto << "\n  {\n";

    robertvec parent_undefined; // Robert probably knew what this was when he wrote it. Probably.
    event_map evgroup; // Josh knew what this was when he wrote it.

    write_object_locals(lcpp, wto, global, object_iter, parent);
    write_object_scripts(wto, object_iter);
    write_object_timelines(wto, es, object_iter, revTlineLookup);
    write_object_events(wto, object_iter, parent, parent_undefined, evgroup);

    parent_undefinitions[object_iter->second->id] = parent_undefined;
    evmap[object_iter->second->id] = evgroup;

    write_stacked_event_groups(wto, object_iter, parent, evgroup, evmap);
    write_event_perform(wto, object_iter, parent);
    write_object_unlink(wto, object_iter, parent, parent_undefined, evgroup);
    write_object_constructors(wto, object_iter, parent, parent_undefined, evgroup);
    write_object_destructor(wto, object_iter, parent, parent_undefined, evgroup);

    wto << "  };\n";

    parsed.push_back(object_iter->first);
    object_iter++;
  }
}

static inline void write_object_data_structs(std::ostream &wto) {
  wto << "  objectstruct objs[] = {\n" <<std::fixed;
  int objcount = 0, obmx = 0;
  for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++, objcount++)
  {
    wto << "    {"
        << i->second->sprite_index << "," << i->second->solid << "," 
        << i->second->visible << "," << i->second->depth << ","
        << i->second->persistent << "," << i->second->mask_index
        << "," << i->second->parent << "," << i->second->id
        << "},\n";
    if (i->second->id >= obmx) obmx = i->second->id;
  }
  wto.unsetf(ios_base::floatfield);
  wto << "  };\n";
  wto << "  int objectcount = " << objcount << ";\n";
  wto << "  int obj_idmax = " << obmx+1 << ";\n";
}

int lang_CPP::compile_writeObjectData(EnigmaStruct* es, parsed_object* global, int mode)
{
  //NEXT FILE ----------------------------------------
  //Object declarations: object classes/names and locals.
  ofstream wto;
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h").c_str(),ios_base::out);
    wto << license;
    wto << "#include \"Universal_System/collisions_object.h\"\n";
    wto << "#include \"Universal_System/object.h\"\n\n";
    wto << "#include <map>";

    map<string, int> revTlineLookup; //We'll need this lookup later.

    declare_scripts(wto, es);
    declare_timelines(wto, es, revTlineLookup);

    wto << "namespace enigma\n{\n";
    declare_object_locals_class(wto);
    wto << "\n";
    declare_extension_casts(wto);
    wto << "}\n\n";
    
    robertmap parent_undefinitions;
    // TODO(JoshDreamland): Replace with enigma_user
    wto << "namespace enigma // TODO: Replace with enigma_user\n{\n";
    write_object_class_bodies(this, wto, es, global, parent_undefinitions, revTlineLookup);
    wto << "}\n\n";
    
    wto << "namespace enigma {\n";
    write_object_data_structs(wto);
    wto << "}\n";
  wto.close();



  /* NEXT FILE `******************************************\
  ** Object functions: events, constructors, other codes.
  ********************************************************/

  vector<unsigned> parent_undefined;

  cout << "DBGMSG 1" << endl;
  wto.open((makedir +"Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h").c_str(),ios_base::out);
    wto << license;

    wto << endl << "#define log_xor || log_xor_helper() ||" << endl;
    wto << "struct log_xor_helper { bool value; };" << endl;
    wto << "template<typename LEFT> log_xor_helper operator ||(const LEFT &left, const log_xor_helper &xorh) { log_xor_helper nxor; nxor.value = (bool)left; return nxor; }" << endl;
    wto << "template<typename RIGHT> bool operator ||(const log_xor_helper &xorh, const RIGHT &right) { return xorh.value ^ (bool)right; }" << endl << endl;

    cout << "DBGMSG 2" << endl;
    // Export globalized scripts
    for (int i = 0; i < es->scriptCount; i++)
    {
      parsed_script* scr = scr_lookup[es->scripts[i].name];
      const char* comma = "";
      wto << "variant _SCR_" << es->scripts[i].name << "(";
      for (int argn = 0; argn < scr->globargs; argn++) //it->second gives max argument count used
      {
        wto << comma << "variant argument" << argn;
        comma = ", ";
      }
      wto << ")\n{\n  ";
      if (mode == emode_debug) {
        wto << "enigma::debug_scope $current_scope(\"script '" << es->scripts[i].name << "'\");\n";
      }
      parsed_event& upev = scr->pev_global?*scr->pev_global:scr->pev;

      //Super-hacky
      std::string override_code = "";
      std::string override_synt = "";
      if (upev.code.find("with((self))")==0) {
        override_code = upev.code.substr(12, std::string::npos);
        override_synt = upev.synt.substr(12, std::string::npos);
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

    // Export globalized timelines.
    // TODO: Is there such a thing as a localized timeline?
    for (int i=0; i<es->timelineCount; i++)
    {
      for (int j=0; j<es->timelines[i].momentCount; j++)
      {
        parsed_script* scr = tline_lookup[es->timelines[i].name][j];
        wto << "void TLINE_" <<es->timelines[i].name <<"_MOMENT_" <<es->timelines[i].moments[j].stepNo <<"()\n{\n";
        parsed_event& upev = scr->pev_global?*scr->pev_global:scr->pev;

        std::string override_code = "";
        std::string override_synt = "";
        if (upev.code.find("with((self))")==0) {
          override_code = upev.code.substr(12, std::string::npos);
          override_synt = upev.synt.substr(12, std::string::npos);
        }
        print_to_file(
          override_code.empty() ? upev.code : override_code,
          override_synt.empty() ? upev.synt : override_synt,
          upev.strc,
          upev.strs,
          2,wto
        );

        wto << "\n}\n\n";
      }
    }

    cout << "DBGMSG 3" << endl;
    // Export everything else
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    {
      cout << "DBGMSG 4" << endl;
      parent_undefined = parent_undefinitions.find(i->first)->second;
      for (unsigned ii = 0; ii < i->second->events.size; ii++) {
        const int mid = i->second->events[ii].mainId, id = i->second->events[ii].id;
        string evname = event_get_function_name(mid,id);
        if  (i->second->events[ii].code != "")
        {
          cout << "DBGMSG 4-1" << endl;
          
          bool defined_inherited = false;
          if (setting::inherit_objects) {
            po_i parent = parsed_objects.find(i->second->parent);
            if (parent != parsed_objects.end() && std::find(parent_undefined.begin(), parent_undefined.end(), ii) == parent_undefined.end()) {
              wto << "#define event_inherited OBJ_" + parent->second->name + "::myevent_" + evname + "\n";
              defined_inherited = true;
            }
          }

          // Write event code
          cout << "DBGMSG 4-2" << endl;
          wto << "variant enigma::OBJ_" << i->second->name << "::myevent_" << evname << "()\n{\n  ";
          if (mode == emode_debug) {
            wto << "enigma::debug_scope $current_scope(\"event '" << evname << "' for object '" << i->second->name << "'\");\n";
          }
          if (!event_execution_uses_default(i->second->events[ii].mainId,i->second->events[ii].id))
            wto << "enigma::temp_event_scope ENIGMA_PUSH_ITERATOR_AND_VALIDATE(this);\n  ";
          if (event_has_const_code(mid, id))
            wto << event_get_const_code(mid, id) << endl;
          if (event_has_prefix_code(mid, id))
            wto << event_get_prefix_code(mid, id) << endl;
          cout << "DBGMSG 4-4" << endl;
          print_to_file(i->second->events[ii].code,i->second->events[ii].synt,i->second->events[ii].strc,i->second->events[ii].strs,2,wto);
          if (event_has_suffix_code(mid, id))
            wto << event_get_suffix_code(mid, id) << endl;
          cout << "DBGMSG 4-5" << endl;
          wto << "\n  return 0;\n}\n";

          if (defined_inherited) {
            wto << "#undef event_inherited\n";
          }
        }
        
        if  (i->second->events[ii].code != "" || event_has_default_code(mid,id))
        {
          // Write event sub check code
          if (event_has_sub_check(mid, id)) {
            wto << "inline bool enigma::OBJ_" << i->second->name << "::myevent_" << evname << "_subcheck()\n{\n  ";
            cout << "DBGMSG 4-3" << endl;
            wto << event_get_sub_check_condition(mid, id) << endl;
            wto << "\n}\n";
          }
        }
      }
        
      cout << "DBGMSG 5" << endl;

    
      //Write local object copies of scripts
      parsed_object* t = i->second;
      for (parsed_object::funcit it = t->funcs.begin(); it != t->funcs.end(); it++) //For each function called by this object
      {
        map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
        if (subscr != scr_lookup.end() // If we've got ourselves a script
        and subscr->second->pev_global) // And it has distinct code for use at the global scope (meaning it's more efficient locally)
        {
          const char* comma = "";
          wto << "variant enigma::OBJ_" << i->second->name << "::_SCR_" << it->first << "(";
          for (int argn = 0; argn < it->second; argn++) //it->second gives max argument count used
          {
            wto << comma << "variant argument" << argn;
            comma = ", ";
          }
          wto << ")\n{\n  ";
          print_to_file(subscr->second->pev.code,subscr->second->pev.synt,subscr->second->pev.strc,subscr->second->pev.strs,2,wto);
          wto << "\n  return 0;\n}\n\n";
        }
      }


      //Write local object copies of timelines
      bool hasKnownTlines = false;
      for (parsed_object::tlineit it = t->tlines.begin(); it != t->tlines.end(); it++) //For each timeline potentially set by this object
      {
        map<string, int>::iterator timit = revTlineLookup.find(it->first); //Check if it's a timeline
        if (timit != revTlineLookup.end()) // If we've got ourselves a script
        //and subscr->second->pev_global) // And it has distinct code for use at the global scope (meaning it's more efficient locally) //NOTE: It seems all timeline MUST be copied locally.
        {
          hasKnownTlines = true;
          for (int j=0; j<es->timelines[timit->second].momentCount; j++) {
            parsed_script* scr = tline_lookup[timit->first][j];
            wto << "void enigma::OBJ_" <<i->second->name <<"::TLINE_" <<es->timelines[timit->second].name <<"_MOMENT_" <<es->timelines[timit->second].moments[j].stepNo <<"() {\n    ";
            print_to_file(scr->pev.code, scr->pev.synt, scr->pev.strc, scr->pev.strs, 2, wto);
            wto <<"}\n";
          }
        } wto << "\n";
      }

      //If no timelines are ever used by this script, it can rely on the default lookup table.
      //NOTE: We have to allow it to fall through to the default in cases where instances (by id) are given a timeline.
      if (hasKnownTlines) {
        wto <<"void enigma::OBJ_" <<i->second->name <<"::timeline_call_moment_script(int timeline_index, int moment_index) {\n";
        wto <<"  switch (timeline_index) {\n";
        for (parsed_object::tlineit it = t->tlines.begin(); it != t->tlines.end(); it++) {
          map<string, int>::iterator timit = revTlineLookup.find(it->first);
          if (timit != revTlineLookup.end()) {
            wto <<"    case " <<es->timelines[timit->second].id <<": {\n";
            wto <<"      switch (moment_index) {\n";
            for (int j=0; j<es->timelines[timit->second].momentCount; j++) {
              wto <<"        case " <<j <<": {\n";
              wto <<"          TLINE_" <<es->timelines[timit->second].name <<"_MOMENT_" <<es->timelines[timit->second].moments[j].stepNo <<"();\n";
              wto <<"          break;\n";
              wto <<"        }\n";
            }
            wto <<"      }\n";
            wto <<"      break;\n";
           wto <<"    }\n";
          }
        }
        //Fall through to the default case.
        wto <<"    default: event_parent::timeline_call_moment_script(timeline_index, moment_index);\n";
        wto <<"  }\n";
        wto <<"}\n\n";
      }

    cout << "DBGMSG 6" << endl;
    }
    cout << "DBGMSG 7" << endl;
    
    parent_undefined.clear();
    parent_undefinitions.clear();

    wto << "namespace enigma\n{\n"
    "  callable_script callable_scripts[] = {\n";
    int scr_count = 0;
    for (int i = 0; i < es->scriptCount; i++)
    {
      while (es->scripts[i].id > scr_count)
      {
          wto << "    { NULL, -1 },\n";
          scr_count++;
      }
      scr_count++;
      wto << "    { (variant(*)())_SCR_" << es->scripts[i].name << ", " << scr_lookup[es->scripts[i].name]->globargs << " },\n";
    }
    wto << "  };\n  \n";

    cout << "DBGMSG 8" << endl;
    wto << "  void constructor(object_basic* instance_b)\n  {\n"
    "    //This is the universal create event code\n    object_locals* instance = (object_locals*)instance_b;\n    \n"
    "    instance->xstart = instance->x;\n    instance->ystart = instance->y;\n    instance->xprevious = instance->x;\n    instance->yprevious = instance->y;\n\n"
    "    instance->gravity=0;\n    instance->gravity_direction=270;\n    instance->friction=0;\n    \n"
    "    \n"
    "    instance->timeline_index = -1;\n    instance->timeline_running = " <<(setting::compliance_mode==setting::COMPL_GM5?"true":"false") <<";\n    instance->timeline_speed = 1;\n    instance->timeline_position = 0;\n"
    "    instance->timeline_loop = false;\n    \n"
    "    \n"
    "    instance->image_alpha = 1.0;\n    instance->image_angle = 0;\n    instance->image_blend = 0xFFFFFF;\n    instance->image_index = 0;\n"
    "    instance->image_speed  = 1;\n    instance->image_xscale = 1;\n    instance->image_yscale = 1;\n    \n"
    "instancecount++;\n    instance_count++;\n  }\n}\n";
  wto.close();

  return 0;
}
