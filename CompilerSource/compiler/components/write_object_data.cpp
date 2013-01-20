/**
  @file  write_object_data.cpp
  @brief Implements crucial functions for outputting all object members and events
         to the C++ engine.
  
  @section License
    Copyright (C) 2008-2013 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;


#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "compiler/compile_common.h"
#include "compiler/event_reader/event_parser.h"
#include "general/parse_basics_old.h"

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

struct cspair { string c, s; };
int lang_CPP::compile_writeObjectData(compile_context &ctex)
{
  //NEXT FILE ----------------------------------------
  //Object declarations: object classes/names and locals.
  ofstream wto;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h",ios_base::out);
    wto << gen_license;
    wto << "#include \"../Universal_System/collisions_object.h\"\n\n";

    // Write the script names
    wto << "// Script identifiers\n";
    for (int i = 0; i < ctex.es->scriptCount; i++)
      wto << "const int " << ctex.es->scripts[i].name << " = " << ctex.es->scripts[i].id << ";\n";
    wto << "\n";
    for (int i = 0; i < ctex.es->scriptCount; i++)
      wto << "#define " << ctex.es->scripts[i].name << "(arguments...) _SCR_" << ctex.es->scripts[i].name << "(arguments)\n";
    wto << "\n\n";

    for (ps_i scrit = ctex.parsed_scripts.begin(); scrit != ctex.parsed_scripts.end(); ++scrit)
    {
      const char* comma = "";
      parsed_script* const scr = scrit->second;
      wto << "variant _SCR_" << scrit->first << "(";
      for (int argn = 0; argn < scr->global_args; argn++) {
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
      for (po_i i = ctex.parsed_objects.begin(); i != ctex.parsed_objects.end(); i++)
      {
        wto << "  \n  struct " << i->second->class_name << ": object_locals\n  {";
        
        string additional_local_code;
        for (unsigned ii = 0; ii < i->second->events.size(); ii++) {
          string addls = event_get_locals(i->second->events[ii]->main_id,i->second->events[ii]->id);
          if (addls != "") additional_local_code += addls;
        }
        EDL_AST elocals_ast(&i->second->self, &i->second->self, ctex.global);
        elocals_ast.parse_edl(additional_local_code);

        wto << "\n    //Locals to instances of this object\n";
        for (definition_scope::defiter ii =  i->second->self.members.begin(); ii != i->second->self.members.end(); ++ii)
          wto << "    " << ii->second->toString() << "\n";

        // Next, we write the list of all the scripts this object will hoard a copy of for itself.
        wto << "\n    //Scripts called by this object\n    ";
        parsed_object* t = i->second;
        for (script_it it = t->sh.scripts.begin(); it != t->sh.scripts.end(); ++it) //For each function called by this object
        {
          ps_i subscr = ctex.parsed_scripts.find(it->first); // Check if it's a script
          if (subscr != ctex.parsed_scripts.end() // If we've got ourselves a script
          and (subscr->second->sh.undeclared.size() || subscr->second->self.members.size())) // And it modifies the caller's variables
          {
            const char* comma = "";
            wto << "\n    variant _SCR_" << it->first << "(";
            for (int argn = 0; argn < it->second.arg_max; argn++) {
              wto << comma << "variant argument" << argn << " = 0";
              comma = ", ";
            }
            wto << ");";
          }
        } wto << "\n    ";


        // Now we output all the events this object uses
        // Defaulted events were already added into this array.
        map<int, cspair> nemap; // Keep track of events that need added to honor et_stacked
        for (unsigned ii = 0; ii < i->second->events.size(); ii++)
          if  (!i->second->events[ii]->code.ast.empty())
          {
            //Look up the event name
            string evname = event_get_function_name(i->second->events[ii]->main_id,i->second->events[ii]->id);
            if (event_is_instance(i->second->events[ii]->main_id,i->second->events[ii]->id))
              nemap[i->second->events[ii]->main_id].c += (event_has_super_check(i->second->events[ii]->main_id,i->second->events[ii]->id) ?
                "        if (" + event_get_super_check_condition(i->second->events[ii]->main_id,i->second->events[ii]->id) + ") myevent_" : "        myevent_") + evname + "();\n",
              nemap[i->second->events[ii]->main_id].s = event_stacked_get_root_name(i->second->events[ii]->main_id);
            wto << "    variant myevent_" << evname << "();\n    ";
          }
        if (nemap.size())
        {
          wto << "\n    \n    // Grouped event bases\n    ";
          for (map<int,cspair>::iterator it = nemap.begin(); it != nemap.end(); it++)
            wto << "  void myevent_" << it->second.s << "()\n      {\n" << it->second.c << "      }\n    ";
        }


        // Now we write the callable unlinker. Its job is to disconnect the instance for destroy.
        // This is an important component that tracks multiple pieces of the instance. These pieces
        // exist for efficiency. See the instance system documentation for full details.

        // Here we write the pieces it tracks
        wto << "\n    // Self-tracking\n";

        // This tracks components of the instance system.
        wto << "      enigma::pinstance_list_iterator ENOBJ_ITER_me;\n";
        for (po_i inher = i; inher != ctex.parsed_objects.end(); inher = ctex.parsed_objects.find(inher->second->properties->parentId)) // For this object and each parent thereof
          wto << "      enigma::inst_iter *ENOBJ_ITER_myobj" << inher->second->properties->id << ";\n"; // Keep track of a pointer to `this` inside this list.

        // This tracks components of the event system.
          for (unsigned ii = 0; ii < i->second->events.size(); ii++) // Export a tracker for all events
            if (!event_is_instance(i->second->events[ii]->main_id,i->second->events[ii]->id)) { //...well, all events which aren't stacked
              if (event_has_iterator_declare_code(i->second->events[ii]->main_id,i->second->events[ii]->id)) {
                if (!iscomment(event_get_iterator_declare_code(i->second->events[ii]->main_id,i->second->events[ii]->id)))
                  wto << "      " << event_get_iterator_declare_code(i->second->events[ii]->main_id,i->second->events[ii]->id) << ";\n";
              } else
                wto << "      enigma::inst_iter *ENOBJ_ITER_myevent_" << event_get_function_name(i->second->events[ii]->main_id,i->second->events[ii]->id) << ";\n";
            }
          for (map<int,cspair>::iterator it = nemap.begin(); it != nemap.end(); it++) // The stacked ones should have their root exported
            wto << "      enigma::inst_iter *ENOBJ_ITER_myevent_" << it->second.s << ";\n";

        //This is the actual call to remove the current instance from all linked records before destroying it.
        wto << "\n    void unlink()\n    {\n";
          wto << "      instance_iter_queue_for_destroy(ENOBJ_ITER_me); // Queue for delete while we're still valid\n";
          wto << "      deactivate();\n    }\n void deactivate()\n    {\n";
          wto << "      enigma::unlink_main(ENOBJ_ITER_me); // Remove this instance from the non-redundant, tree-structured list.\n";
          for (po_i her = i; her != ctex.parsed_objects.end(); her = ctex.parsed_objects.find(her->second->properties->parentId))
            wto << "      unlink_object_id_iter(ENOBJ_ITER_myobj" << her->second->properties->id << ", " << her->second->properties->id << ");\n";
          for (unsigned ii = 0; ii < i->second->events.size(); ii++)
            if (!event_is_instance(i->second->events[ii]->main_id,i->second->events[ii]->id)) {
              const string evname = event_get_function_name(i->second->events[ii]->main_id,i->second->events[ii]->id);
              if (event_has_iterator_unlink_code(i->second->events[ii]->main_id,i->second->events[ii]->id)) {
                if (!iscomment(event_get_iterator_unlink_code(i->second->events[ii]->main_id,i->second->events[ii]->id)))
                  wto << "      " << event_get_iterator_unlink_code(i->second->events[ii]->main_id,i->second->events[ii]->id) << ";\n";
              } else
                wto << "      enigma::event_" << evname << "->unlink(ENOBJ_ITER_myevent_" << evname << ");\n";
          }
          for (map<int,cspair>::iterator it = nemap.begin(); it != nemap.end(); it++) // The stacked ones should have their root exported
            wto << "      enigma::event_" << it->second.s << "->unlink(ENOBJ_ITER_myevent_" << it->second.s << ");\n";
          wto << "    }\n    ";


        // Next are the constructors. One is automated, the other directed.
        // Automatic constructor:  The constructor generates the ID from a global maximum and links by that alias.
        // Directed constructor:   Meant for use by the room system, the constructor uses a specified ID alias assumed to have been checked for conflict.

        wto <<   "\n    " <<  i->second->class_name << "(int enigma_genericconstructor_newinst_x = 0, int enigma_genericconstructor_newinst_y = 0, const int id = (enigma::maxid++))";
          wto << ": object_locals(id, " << i->second->properties->id << ")";
          for (const_it ci = i->second->sh.consts.begin(); ci != i->second->sh.consts.end(); ++ci)
            wto << ", " << ci->first << "(" << format_expression(ci->second.initial_value) << ")";
          wto << "\n    {\n";
            // Sprite index
              if (ctex.sh.funcs.find("object_set_sprite") != ctex.sh.funcs.end()) //We want to initialize
                wto << "      sprite_index = enigma::object_table[" << i->second->properties->id << "].sprite;\n"
                    << "      make_index = enigma::object_table[" << i->second->properties->id << "].mask;\n";
              else
                wto << "      sprite_index = " << i->second->properties->spriteId << ";\n"
                    << "      mask_index = " << i->second->properties->maskId << ";\n";
              wto << "      visible = " << i->second->properties->visible << ";\n      solid = " << i->second->properties->solid << ";\n";
              wto << "      persistent = " << i->second->properties->persistent << ";\n";


              wto << "      activate();";
            // Coordinates
              wto << "      x = enigma_genericconstructor_newinst_x, y = enigma_genericconstructor_newinst_y;\n";

          wto << "      enigma::constructor(this);\n";
          wto << "    }\n";


          wto << " void activate()\n    {\n";
          // Depth
              wto << "      depth.init(" << i->second->properties->depth << ", this);\n";
            // Instance system interface
              wto << "      ENOBJ_ITER_me = enigma::link_instance(this);\n";
              for (po_i her = i; her != ctex.parsed_objects.end(); her = ctex.parsed_objects.find(her->second->properties->parentId))
                wto << "      ENOBJ_ITER_myobj" << her->second->properties->id << " = enigma::link_obj_instance(this, " << her->second->properties->id << ");\n";

            // Event system interface
              for (unsigned ii = 0; ii < i->second->events.size(); ii++)
                if (!event_is_instance(i->second->events[ii]->main_id,i->second->events[ii]->id)) {
                  const string evname = event_get_function_name(i->second->events[ii]->main_id,i->second->events[ii]->id);
                  if (event_has_iterator_initialize_code(i->second->events[ii]->main_id,i->second->events[ii]->id)) {
                    if (!iscomment(event_get_iterator_initialize_code(i->second->events[ii]->main_id,i->second->events[ii]->id)))
                      wto << "      " << event_get_iterator_initialize_code(i->second->events[ii]->main_id,i->second->events[ii]->id) << ";\n";
                  } else
                    wto << "      ENOBJ_ITER_myevent_" << evname << " = enigma::event_" << evname << "->add_inst(this);\n";
              }
              for (map<int,cspair>::iterator it = nemap.begin(); it != nemap.end(); it++)
                wto << "      ENOBJ_ITER_myevent_" << it->second.s << " = enigma::event_" << it->second.s << "->add_inst(this);\n";
          wto << "    }\n";


          // Destructor
          wto <<   "    \n    ~" <<  i->second->class_name << "()\n    {\n";
            wto << "      enigma::winstance_list_iterator_delete(ENOBJ_ITER_me);\n";
            for (po_i her = i; her != ctex.parsed_objects.end(); her = ctex.parsed_objects.find(her->second->properties->parentId))
              wto << "      delete ENOBJ_ITER_myobj" << her->second->properties->id << ";\n";
            for (unsigned ii = 0; ii < i->second->events.size(); ii++)
              if (!event_is_instance(i->second->events[ii]->main_id,i->second->events[ii]->id)) {
                if (event_has_iterator_delete_code(i->second->events[ii]->main_id,i->second->events[ii]->id)) {
                  if (!iscomment(event_get_iterator_delete_code(i->second->events[ii]->main_id,i->second->events[ii]->id)))
                    wto << "      " << event_get_iterator_delete_code(i->second->events[ii]->main_id,i->second->events[ii]->id) << ";\n";
                } else
                  wto << "      delete ENOBJ_ITER_myevent_" << event_get_function_name(i->second->events[ii]->main_id,i->second->events[ii]->id) << ";\n";
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

  // NEWPARSER: FIXME: Rewrite as needed
  /*
    cout << "DBGMSG 1" << endl;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h",ios_base::out);
    wto << license;

    cout << "DBGMSG 2" << endl;
    // Export globalized scripts
    for (int i = 0; i < ctex.es->scriptCount; i++)
    {
      parsed_script* scr = ctex.sh.scripts[ctex.es->scripts[i].name].script;
      const char* comma = "";
      wto << "variant _SCR_" << ctex.es->scripts[i].name << "(";
      for (int argn = 0; argn < scr->global_args; argn++) //it->second gives max argument count used
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
    for (po_i i = ctex.parsed_objects.begin(); i != ctex.parsed_objects.end(); i++)
    {
    cout << "DBGMSG 4" << endl;
      for (unsigned ii = 0; ii < i->second->events.size; ii++)
      if  (i->second->events[ii]->code != "")
      {
    cout << "DBGMSG 4-1" << endl;
        const int mid = i->second->events[ii]->main_id, id = i->second->events[ii]->id;
        string evname = event_get_function_name(mid,id);
    cout << "DBGMSG 4-2" << endl;
        wto << "variant enigma::" << i->second->class_name << "::myevent_" << evname << "()\n{\n  ";
          if (!event_execution_uses_default(i->second->events[ii]->main_id,i->second->events[ii]->id))
            wto << "enigma::temp_event_scope ENIGMA_PUSH_ITERATOR_AND_VALIDATE(this);\n  ";
    cout << "DBGMSG 4-3" << endl;
          if (event_has_sub_check(mid, id))
            wto << event_get_sub_check_condition(mid, id) << endl;
          if (event_has_const_code(mid, id))
            wto << event_get_const_code(mid, id) << endl;
          if (event_has_prefix_code(mid, id))
            wto << event_get_prefix_code(mid, id) << endl;
    cout << "DBGMSG 4-4" << endl;
          print_to_file(i->second->events[ii]->code,i->second->events[ii]->synt,i->second->events[ii]->strc,i->second->events[ii]->strs,2,wto);
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
          wto << "variant enigma::" << i->second->class_name << "::_SCR_" << it->first << "(";
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
    int scr_count = 0;
    for (int i = 0; i < ctex.es->scriptCount; i++)
    {
      while (ctex.es->scripts[i].id > scr_count)
      {
          wto << "    { NULL, -1 },\n";
          scr_count++;
      }
      scr_count++;
      wto << "    { (variant(*)())_SCR_" << ctex.es->scripts[i].name << ", " << scr_lookup[ctex.es->scripts[i].name]->globargs << " },\n";
    }
    wto << "  };\n  \n";

    wto << "  int script_idmax = " << scr_count << ";\n \n";

    cout << "DBGMSG 8" << endl;
    wto << "  void constructor(object_basic* instance_b)\n  {\n"
    "    //This is the universal create event code\n    object_locals* instance = (object_locals*)instance_b;\n    \n"
    "    instance->xstart = instance->x;\n    instance->ystart = instance->y;\n    instance->xprevious = instance->x;\n    instance->yprevious = instance->y;\n\n"
    "    instance->gravity=0;\n    instance->gravity_direction=270;\n    instance->friction=0;\n    \n"
    / *instance->sprite_index = enigma::objectdata[instance->obj].sprite_index;
    instance->mask_index = enigma::objectdata[instance->obj].mask_index;
    instance->visible = enigma::objectdata[instance->obj].visible;
    instance->solid = enigma::objectdata[instance->obj].solid;
    instance->persistent = enigma::objectdata[instance->obj].persistent;
    instance->depth = enigma::objectdata[instance->obj].depth;* /
    "    \n"
    "    instance->image_alpha = 1.0;\n    instance->image_angle = 0;\n    instance->image_blend = 0xFFFFFF;\n    instance->image_index = 0;\n"
    "    instance->image_single = -1;\n    instance->image_speed  = 1;\n    instance->image_xscale = 1;\n    instance->image_yscale = 1;\n    \n"
    "instancecount++;\n    instance_count++;\n  }\n}\n";
  wto.close();
*/
  return 0;
}
