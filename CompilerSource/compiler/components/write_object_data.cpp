/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "../../externs/externs.h"
#include "../../parser/parser.h"

#include "../../backend/EnigmaStruct.h" //LateralGM interface structures
#include "../compile_common.h"
#include "../event_reader/event_parser.h"
#include "../../general/parse_basics.h"

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

struct cspair { string c, s; };
int compile_writeObjectData(EnigmaStruct* es, parsed_object* global)
{
  //NEXT FILE ----------------------------------------
  //Object declarations: object classes/names and locals.
  ofstream wto;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h",ios_base::out);
    wto << license;
    wto << "#include \"../Universal_System/collisions_object.h\"\n\n";

    // Write the script names
    wto << "// Script identifiers\n";
    for (int i = 0; i < es->scriptCount; i++)
      wto << "const int " << es->scripts[i].name << " = " << es->scripts[i].id << ";\n";
    wto << "\n";
    for (int i = 0; i < es->scriptCount; i++)
      wto << "#define " << es->scripts[i].name << "(arguments...) _SCR_" << es->scripts[i].name << "(arguments)\n";
    wto << "\n\n";

    for (int i = 0; i < es->scriptCount; i++)
    {
      parsed_script* scr = scr_lookup[es->scripts[i].name];
      const char* comma = "";
      wto << "variant _SCR_" << es->scripts[i].name << "(";
      for (int argn = 0; argn < scr->globargs; argn++) {
        wto << comma << "variant argument" << argn << "=0";
        comma = ", ";
      }
      wto << ");\n";
    }
    wto << "\n";
    wto << "namespace enigma\n{\n";
      wto << "  struct object_locals: event_parent";
        for (unsigned i = 0; i < parsed_extensions.size(); i++)
          if (parsed_extensions[i].implements != "")
            wto << ", virtual " << parsed_extensions[i].implements;
          else wto << " /*" << parsed_extensions[i].name << "*/";
        wto << "\n  {\n";
        wto << "    #include \"../Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h\"\n\n";
        wto << "    object_locals() {}\n";
        wto << "    object_locals(unsigned _x, int _y): event_parent(_x,_y) {}\n  };\n";
      for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
      {
        wto << "  \n  struct OBJ_" << i->second->name << ": object_locals\n  {";

        for (unsigned ii = 0; ii < i->second->events.size; ii++)
        {
          string addls = event_get_locals(i->second->events[ii].mainId,i->second->events[ii].id);
          if (addls != "")
          {
            pt pos;
            string type, name, pres, sufs;
            for (pos = 0; pos < addls.length(); pos++)
            {
              if (is_useless(addls[pos])) continue;
              if (addls[pos] == ';') { i->second->locals[name] = dectrip(type, pres, sufs); type = pres = sufs = ""; continue; }
              if (addls[pos] == ',') { i->second->locals[name] = dectrip(type, pres, sufs); pres = sufs = ""; continue; }
              if (is_letter(addls[pos]) or addls[pos] == '$') {
                const pt spos = pos;
                while (is_letterdd(addls[++pos]));
                string tn = addls.substr(spos,pos-spos);
                (find_extname(tn,EXTFLAG_TYPENAME) ? type : name) = tn;
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
                for (size_t ii = 0; ii < i->second->initializers.size(); ii++)
                  if (i->second->initializers[ii].first == name) { redundant = true; break; }
                if (!redundant)
                  i->second->initializers.push_back(initpair(name,addls.substr(spos,pos-spos)));
                pos--; continue;
              }
            }
          }
        }

        wto << "\n    //Locals to instances of this object\n    ";
        for (deciter ii =  i->second->locals.begin(); ii != i->second->locals.end(); ii++)
        {
          bool writeit = true; //Whether this "local" should be declared such

          // If it's not explicitely defined, we must question whether it should be given a unique presence in this scope
          if (!ii->second.defined())
          {
            parsed_object::globit ve = global->globals.find(ii->first); // So, we look for a global by this name
            if (ve != global->globals.end()) {  // If a global by this name is indeed found,
              if (ve->second.defined()) // And this global is explicitely defined, not just accessed with a dot,
                writeit = false; // We assume that its definition will cover us, and we do not redeclare it as a local.
              cout << "enigma: scopedebug: variable `" << ii->first << "' from object `" << i->second->name << "' will be used from the " << (writeit ? "object" : "global") << " scope." << endl;
            }
          }
          if (writeit)
            wto << tdefault(ii->second.type) << " " << ii->second.prefix << ii->first << ii->second.suffix << ";\n    ";
        }

        // Next, we write the list of all the scripts this object will hoard a copy of for itself.
        wto << "\n    //Scripts called by this object\n    ";
        parsed_object* t = i->second;
        for (parsed_object::funcit it = t->funcs.begin(); it != t->funcs.end(); it++) //For each function called by this object
        {
          map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
          if (subscr != scr_lookup.end() // If we've got ourselves a script
          and subscr->second->pev_global) // And it has distinct code for use at the global scope (meaning it's more efficient locally)
          {
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


        // Now we output all the events this object uses
        // Defaulted events were already added into this array.
        map<int, cspair> nemap; // Keep track of events that need added to honor et_stacked
        for (unsigned ii = 0; ii < i->second->events.size; ii++)
          if  (i->second->events[ii].code != "")
          {
            //Look up the event name
            string evname = event_get_function_name(i->second->events[ii].mainId,i->second->events[ii].id);
            if (event_is_instance(i->second->events[ii].mainId,i->second->events[ii].id))
              nemap[i->second->events[ii].mainId].c += (event_has_super_check(i->second->events[ii].mainId,i->second->events[ii].id) ?
                "        if (" + event_get_super_check_condition(i->second->events[ii].mainId,i->second->events[ii].id) + ") myevent_" : "        myevent_") + evname + "();\n",
              nemap[i->second->events[ii].mainId].s = event_stacked_get_root_name(i->second->events[ii].mainId);
            wto << "    variant myevent_" << evname << "();\n    ";
          }
        if (nemap.size())
        {
          wto << "\n    \n    // Grouped event bases\n    ";
          for (map<int,cspair>::iterator it = nemap.begin(); it != nemap.end(); it++)
            wto << "  void myevent_" << it->second.s << "()\n      {\n" << it->second.c << "      }\n    ";
        }


        /**** Now we write the callable unlinker. Its job is to disconnect the instance for destroy.
        * @ * This is an important component that tracks multiple pieces of the instance. These pieces
        *//// are created for efficiency. See the instance system documentation for full details.

        // Here we write the pieces it tracks
        wto << "\n    // Self-tracking\n";

        // This tracks components of the instance system.
        wto << "      enigma::pinstance_list_iterator ENOBJ_ITER_me;\n";
        for (po_i her = i; her != parsed_objects.end(); her = parsed_objects.find(her->second->parent)) // For this object and each parent thereof
          wto << "      enigma::inst_iter *ENOBJ_ITER_myobj" << her->second->id << ";\n"; // Keep track of a pointer to `this` inside this list.

        // This tracks components of the event system.
          for (unsigned ii = 0; ii < i->second->events.size; ii++) // Export a tracker for all events
            if (!event_is_instance(i->second->events[ii].mainId,i->second->events[ii].id)) { //...well, all events which aren't stacked
              if (event_has_iterator_declare_code(i->second->events[ii].mainId,i->second->events[ii].id)) {
                if (!iscomment(event_get_iterator_declare_code(i->second->events[ii].mainId,i->second->events[ii].id)))
                  wto << "      " << event_get_iterator_declare_code(i->second->events[ii].mainId,i->second->events[ii].id) << ";\n";
              } else
                wto << "      enigma::inst_iter *ENOBJ_ITER_myevent_" << event_get_function_name(i->second->events[ii].mainId,i->second->events[ii].id) << ";\n";
            }
          for (map<int,cspair>::iterator it = nemap.begin(); it != nemap.end(); it++) // The stacked ones should have their root exported
            wto << "      enigma::inst_iter *ENOBJ_ITER_myevent_" << it->second.s << ";\n";

        //This is the actual call to remove the current instance from all linked records before destroying it.
        wto << "\n    void unlink()\n    {\n";
          wto << "      instance_iter_queue_for_destroy(ENOBJ_ITER_me); // Queue for delete while we're still valid\n";
          wto << "      enigma::unlink_main(ENOBJ_ITER_me); // Remove this instance from the non-redundant, tree-structured list.\n";
          for (po_i her = i; her != parsed_objects.end(); her = parsed_objects.find(her->second->parent))
            wto << "      unlink_object_id_iter(ENOBJ_ITER_myobj" << her->second->id << ", " << her->second->id << ");\n";
          for (unsigned ii = 0; ii < i->second->events.size; ii++)
            if (!event_is_instance(i->second->events[ii].mainId,i->second->events[ii].id)) {
              const string evname = event_get_function_name(i->second->events[ii].mainId,i->second->events[ii].id);
              if (event_has_iterator_unlink_code(i->second->events[ii].mainId,i->second->events[ii].id)) {
                if (!iscomment(event_get_iterator_unlink_code(i->second->events[ii].mainId,i->second->events[ii].id)))
                  wto << "      " << event_get_iterator_unlink_code(i->second->events[ii].mainId,i->second->events[ii].id) << ";\n";
              } else
                wto << "      enigma::event_" << evname << "->unlink(ENOBJ_ITER_myevent_" << evname << ");\n";
          }
          for (map<int,cspair>::iterator it = nemap.begin(); it != nemap.end(); it++) // The stacked ones should have their root exported
            wto << "      enigma::event_" << it->second.s << "->unlink(ENOBJ_ITER_myevent_" << it->second.s << ");\n";
          wto << "    }\n    ";


        /**** Next are the constructors. One is automated, the other directed.
        * @ *   Automatic constructor:  The constructor generates the ID from a global maximum and links by that alias.
        *////   Directed constructor:   Meant for use by the room system, the constructor uses a specified ID alias assumed to have been checked for conflict.

        wto <<   "\n    OBJ_" <<  i->second->name << "(int enigma_genericconstructor_newinst_x = 0, int enigma_genericconstructor_newinst_y = 0, const int id = (enigma::maxid++))";
          wto << ": object_locals(id, " << i->second->id << ")";
          for (size_t ii = 0; ii < i->second->initializers.size(); ii++)
            wto << ", " << i->second->initializers[ii].first << "(" << i->second->initializers[ii].second << ")";
          wto << "\n    {\n";
            // Sprite index
              if (used_funcs::object_set_sprite) //We want to initialize
                wto << "      sprite_index = enigma::object_table[" << i->second->id << "].sprite;\n"
                    << "      make_index = enigma::object_table[" << i->second->id << "].mask;\n";
              else
                wto << "      sprite_index = " << i->second->sprite_index << ";\n"
                    << "      mask_index = " << i->second->mask_index << ";\n";
              wto << "      visible = " << i->second->visible << ";\n      solid = " << i->second->solid << ";\n";

            // Depth
              wto << "      depth.init(" << i->second->depth << ", this);\n";

            // Instance system interface
              wto << "      ENOBJ_ITER_me = enigma::link_instance(this);\n";
              for (po_i her = i; her != parsed_objects.end(); her = parsed_objects.find(her->second->parent))
                wto << "      ENOBJ_ITER_myobj" << her->second->id << " = enigma::link_obj_instance(this, " << her->second->id << ");\n";

            // Event system interface
              for (unsigned ii = 0; ii < i->second->events.size; ii++)
                if (!event_is_instance(i->second->events[ii].mainId,i->second->events[ii].id)) {
                  const string evname = event_get_function_name(i->second->events[ii].mainId,i->second->events[ii].id);
                  if (event_has_iterator_initialize_code(i->second->events[ii].mainId,i->second->events[ii].id)) {
                    if (!iscomment(event_get_iterator_initialize_code(i->second->events[ii].mainId,i->second->events[ii].id)))
                      wto << "      " << event_get_iterator_initialize_code(i->second->events[ii].mainId,i->second->events[ii].id) << ";\n";
                  } else
                    wto << "      ENOBJ_ITER_myevent_" << evname << " = enigma::event_" << evname << "->add_inst(this);\n";
              }
              for (map<int,cspair>::iterator it = nemap.begin(); it != nemap.end(); it++)
                wto << "      ENOBJ_ITER_myevent_" << it->second.s << " = enigma::event_" << it->second.s << "->add_inst(this);\n";

            // Coordinates
              wto << "      x = enigma_genericconstructor_newinst_x, y = enigma_genericconstructor_newinst_y;\n";

          wto << "      enigma::constructor(this);\n";
          if (event_used_by_something("create"))
            wto << "      myevent_create();";
          wto << "\n    }\n";

          // Destructor
          wto <<   "    \n    ~OBJ_" <<  i->second->name << "()\n    {\n";
            wto << "      enigma::winstance_list_iterator_delete(ENOBJ_ITER_me);\n";
            for (po_i her = i; her != parsed_objects.end(); her = parsed_objects.find(her->second->parent))
              wto << "      delete ENOBJ_ITER_myobj" << her->second->id << ";\n";
            for (unsigned ii = 0; ii < i->second->events.size; ii++)
              if (!event_is_instance(i->second->events[ii].mainId,i->second->events[ii].id)) {
                if (event_has_iterator_delete_code(i->second->events[ii].mainId,i->second->events[ii].id)) {
                  if (!iscomment(event_get_iterator_delete_code(i->second->events[ii].mainId,i->second->events[ii].id)))
                    wto << "      " << event_get_iterator_delete_code(i->second->events[ii].mainId,i->second->events[ii].id) << ";\n";
                } else
                  wto << "      delete ENOBJ_ITER_myevent_" << event_get_function_name(i->second->events[ii].mainId,i->second->events[ii].id) << ";\n";
              }
            for (map<int,cspair>::iterator it = nemap.begin(); it != nemap.end(); it++) // The stacked ones should have their root exported
              wto << "      delete ENOBJ_ITER_myevent_" << it->second.s << ";\n";
          wto << "    }\n";

        wto << "  };\n";
      }
    wto << "}\n";
  wto.close();



  /* NEXT FILE `******************************************\
  ** Object functions: events, constructors, other codes.
  ********************************************************/

    cout << "DBGMSG 1" << endl;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h",ios_base::out);
    wto << license;

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
      parsed_event& upev = scr->pev_global?*scr->pev_global:scr->pev;
      print_to_file(upev.code,upev.synt,upev.strc,upev.strs,2,wto);
      wto << "\n  return 0;\n}\n\n";
    }

    cout << "DBGMSG 3" << endl;
    // Export everything else
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    {
    cout << "DBGMSG 4" << endl;
      for (unsigned ii = 0; ii < i->second->events.size; ii++)
      if  (i->second->events[ii].code != "")
      {
    cout << "DBGMSG 4-1" << endl;
        const int mid = i->second->events[ii].mainId, id = i->second->events[ii].id;
        string evname = event_get_function_name(mid,id);
    cout << "DBGMSG 4-2" << endl;
        wto << "variant enigma::OBJ_" << i->second->name << "::myevent_" << evname << "()\n{\n  ";
          if (!event_execution_uses_default(i->second->events[ii].mainId,i->second->events[ii].id))
            wto << "enigma::temp_event_scope ENIGMA_PUSH_ITERATOR_AND_VALIDATE(this);\n  ";
    cout << "DBGMSG 4-3" << endl;
          if (event_has_sub_check(mid, id))
            wto << event_get_sub_check_condition(mid, id) << endl;
          if (event_has_const_code(mid, id))
            wto << event_get_const_code(mid, id) << endl;
          if (event_has_prefix_code(mid, id))
            wto << event_get_prefix_code(mid, id) << endl;
    cout << "DBGMSG 4-4" << endl;
          print_to_file(i->second->events[ii].code,i->second->events[ii].synt,i->second->events[ii].strc,i->second->events[ii].strs,2,wto);
          if (event_has_suffix_code(mid, id))
            wto << event_get_suffix_code(mid, id) << endl;
    cout << "DBGMSG 4-5" << endl;
        wto << "\n  return 0;\n}\n\n";
      }
    cout << "DBGMSG 5" << endl;

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
    cout << "DBGMSG 6" << endl;
    }
    cout << "DBGMSG 7" << endl;

    wto << "namespace enigma\n{\n"
    "  callable_script callable_scripts[] = {\n";
    for (int i = 0; i < es->scriptCount; i++)
    {
      if (es->scripts[i].id < i) cout << "ERROR! Why the HELL does this script have a lower ID than the last one sent?" << endl;
      else if (es->scripts[i].id > i) wto << "    { NULL, -1 },\n";
      else wto << "    { (variant(*)())_SCR_" << es->scripts[i].name << ", " << scr_lookup[es->scripts[i].name]->globargs << " },\n";
    }
    wto << "  };\n  \n";

    cout << "DBGMSG 8" << endl;
    wto << "  void constructor(object_basic* instance_b)\n  {\n"
    "    //This is the universal create event code\n    object_locals* instance = (object_locals*)instance_b;\n    \n"
    "    instance->xstart = instance->x;\n    instance->ystart = instance->y;\n    instance->xprevious = instance->x;\n    instance->yprevious = instance->y;\n\n"
    "    instance->gravity=0;\n    instance->gravity_direction=270;\n    instance->friction=0;\n    \n"
    /*instance->sprite_index = enigma::objectdata[instance->obj].sprite_index;
    instance->mask_index = enigma::objectdata[instance->obj].mask_index;
    instance->visible = enigma::objectdata[instance->obj].visible;
    instance->solid = enigma::objectdata[instance->obj].solid;
    instance->persistent = enigma::objectdata[instance->obj].persistent;
    instance->depth = enigma::objectdata[instance->obj].depth;*/
    "    \n"
    "    instance->image_alpha = 1.0;\n    instance->image_angle = 0;\n    instance->image_blend = 0xFFFFFF;\n    instance->image_index = 0;\n"
    "    instance->image_single = -1;\n    instance->image_speed  = 1;\n    instance->image_xscale = 1;\n    instance->image_yscale = 1;\n    \n"
    "instancecount++;\n    instance_count++;\n  }\n}\n";
  wto.close();

  return 0;
}
