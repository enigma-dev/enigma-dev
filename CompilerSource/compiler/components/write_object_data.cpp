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

#include <math.h> //log2 to calculate passes.

int compile_writeObjectData(EnigmaStruct* es, parsed_object* global)
{
  //NEXT FILE ----------------------------------------
  //Object declarations: object classes/names and locals.
  ofstream wto;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h",ios_base::out);
    wto << license;
    wto << "#include \"../Universal_System/collisions_object.h\"\n\n";
    wto << "namespace enigma\n{\n";
      wto << "  struct object_locals: object_collisions\n  {\n";
        wto << "    #include \"../Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h\"\n\n";
        wto << "    object_locals() {}\n";
        wto << "    object_locals(unsigned x, int y): object_collisions(x,y) {}\n  };\n";
      for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
      {
        wto << "  struct OBJ_" << i->second->name << ": object_locals\n  {";
        
        wto << "\n    //Locals to instances of this object\n    ";
        for (deciter ii =  i->second->locals.begin(); ii != i->second->locals.end(); ii++)
        {
          bool writeit = true;
          parsed_object::globit ve = global->globals.find(ii->first);
          if (ve != global->globals.end()) {
            if (ve->second.defined())
              writeit = false;
            cout << "enigma: scopedebug: variable `" << ii->first << "' from object `" << i->second->name << "' will be used from the " << (writeit ? "object" : "global") << " scope." << endl;
          }
          if (writeit)
            wto << tdefault(ii->second.type) << " " << ii->second.prefix << ii->first << ii->second.suffix << ";\n    ";
        }
        
        wto << "\n    //Scripts called by this object\n    ";
        parsed_object* t = i->second;
        for (parsed_object::funcit it = t->funcs.begin(); it != t->funcs.end(); it++) //For each function called by this object
        {
          map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
          if (subscr != scr_lookup.end()) //If we've got ourselves a script
          {
            const char* comma = "";
            wto << "\n    enigma::variant " << it->first << "(";
            for (int argn = 0; argn <= it->second; argn++) //it->second gives max argument count used
            {
              wto << comma << "enigma::variant argument" << argn << " = 0";
              comma = ", ";
            }
            wto << ");";
          }
        } wto << "\n    ";
        
        for (unsigned ii = 0; ii < i->second->events.size; ii++)
        {
          //Look up the event name
          string evname = event_get_function_name(i->second->events[ii].mainId,i->second->events[ii].id);
          wto << "\n    #define ENIGMAEVENT_" << evname << " 1\n";
          wto << "    variant myevent_" << evname << "();\n  ";
        }
        
        //Automatic constructor
        //Directed constructor (ID is specified)
        wto <<   "\n    OBJ_" <<  i->second->name << "(int enigma_genericconstructor_newinst_x = 0, int enigma_genericconstructor_newinst_y = 0, int id = (enigma::maxid++))";
          wto << ": object_locals(id, " << i->second->id << ")";
          wto << "\n    {\n";
            //Sprite index
              if (used_funcs::object_set_sprite) //We want to initialize 
                wto << "      sprite_index = enigma::object_table[" << i->second->id << "].sprite;\n";
              else
                wto << "      sprite_index = " << i->second->sprite_index << ";\n";
            
            //Coordinates
                wto << "      x = enigma_genericconstructor_newinst_x, y = enigma_genericconstructor_newinst_y;\n";
              
          wto << "      enigma::constructor(this);\n      myevent_create();\n    }\n";
        wto << "  };\n";
      }
    wto << "}\n";
  wto.close();
  
  //NEXT FILE ----------------------------------------
  //Object functions: events, constructors, other codes.
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h",ios_base::out);
    wto << license;
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    {
      for (unsigned ii = 0; ii < i->second->events.size; ii++)
      {
        string evname = event_get_function_name(i->second->events[ii].mainId,i->second->events[ii].id);
        wto << "enigma::variant enigma::OBJ_" << i->second->name << "::myevent_" << evname << "()\n{\n  ";
          print_to_file(i->second->events[ii].code,i->second->events[ii].synt,i->second->events[ii].strc,i->second->events[ii].strs,2,wto);
        wto << "\n  return 0;\n}\n\n";
      }
      
      parsed_object* t = i->second;
      for (parsed_object::funcit it = t->funcs.begin(); it != t->funcs.end(); it++) //For each function called by this object
      {
        map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
        if (subscr != scr_lookup.end()) //If we've got ourselves a script
        {
          const char* comma = "";
          wto << "enigma::variant enigma::OBJ_" << i->second->name << "::" << it->first << "(";
          for (int argn = 0; argn <= it->second; argn++) //it->second gives max argument count used
          {
            wto << comma << "enigma::variant argument" << argn;
            comma = ", ";
          }
          wto << ")\n{\n  ";
          print_to_file(subscr->second->pev.code,subscr->second->pev.synt,subscr->second->pev.strc,subscr->second->pev.strs,2,wto);
          wto << "\n  return 0;\n}\n\n";
        }
      }
    }
    
    wto << 
    "namespace enigma\n{\n  void constructor(object_basic* instance_b)\n  {\n"
    "    //This is the universal create event code\n    object_locals* instance = (object_locals*)instance_b;\n    enigma::link_instance(instance_b);\n\n"
    "    instance->xstart = instance->x;\n    instance->ystart = instance->y;\n    instance->xprevious = instance->x;\n    instance->yprevious = instance->y;\n\n"
    "    instance->gravity=0;\n    instance->gravity_direction=270;\n    instance->friction=0;\n    \n"
    /*instance->sprite_index = enigma::objectdata[instance->obj].sprite_index;
    instance->mask_index = enigma::objectdata[instance->obj].mask_index;
    instance->visible = enigma::objectdata[instance->obj].visible;
    instance->solid = enigma::objectdata[instance->obj].solid;
    instance->persistent = enigma::objectdata[instance->obj].persistent;
    instance->depth = enigma::objectdata[instance->obj].depth;*/
    "    for(int i=0;i<16;i++)\n      instance->alarm[i]=-1;\n\n"
    
    "    if(instance->sprite_index!=-1)\n    {\n      instance->bbox_bottom  =   sprite_get_bbox_bottom(instance->sprite_index);\n      "
    "    instance->bbox_left    =   sprite_get_bbox_left(instance->sprite_index);\n      instance->bbox_right   =   sprite_get_bbox_right(instance->sprite_index);\n      "
    "    instance->bbox_top     =   sprite_get_bbox_top(instance->sprite_index);\n      //instance->sprite_height =  sprite_get_height(instance->sprite_index); "
    "    //TODO: IMPLEMENT THESE AS AN IMPLICIT ACCESSOR\n      //instance->sprite_width  =  sprite_get_width(instance->sprite_index);  //TODO: IMPLEMENT THESE AS AN IMPLICIT ACCESSOR\n      "
    "    instance->sprite_xoffset = sprite_get_xoffset(instance->sprite_index);\n      instance->sprite_yoffset = sprite_get_yoffset(instance->sprite_index);\n      "
    "    //instance->image_number  =  sprite_get_number(instance->sprite_index); //TODO: IMPLEMENT THESE AS AN IMPLICIT ACCESSOR\n    }\n    \n"
    
    "    instance->image_alpha = 1.0;\n    instance->image_angle = 0;\n    instance->image_blend = 0xFFFFFF;\n    instance->image_index = 0;\n"
    "    instance->image_single = -1;\n    instance->image_speed  = 1;\n    instance->image_xscale = 1;\n    instance->image_yscale = 1;\n    \n"
    /*instance->path_endaction;
    instance->path_index;
    instance->path_orientation;
        instance->path_position;
        instance->path_positionprevious;
        instance->path_scale;
        instance->path_speed;
        instance->timeline_index;
        instance->timeline_position;
        instance->timeline_speed;     */
        //instance->sprite_index = enigma::objectinfo[newinst_obj].sprite_index;
    "instancecount++;\n    instance_count++;\n  }\n}\n";
  wto.close();
  
  return 0;
}
