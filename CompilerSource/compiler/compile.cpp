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

#include "../OS_Switchboard.h" //Tell us where the hell we are
#include "../backend/EnigmaStruct.h" //LateralGM interface structures

#include "../general/darray.h"

#ifdef _WIN32
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif


#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#define flushl (fflush(stdout), "\n")

#include "../externs/externs.h"
#include "../syntax/syncheck.h"
#include "../parser/parser.h"
#include "compile_includes.h"

void clear_ide_editables()
{
  ofstream wto;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/GAME_SETTINGS.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h",ios_base::out); wto.close();
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h",ios_base::out); wto.close();
}

enum cmodes {
  mode_run,
  mode_debug,
  mode_build,
  mode_compile
};

dllexport int compileEGMf(EnigmaStruct *es, const char* filename, int mode)
{
  cout << "Location in memory of structure: " << es << flushl;
  if (es == NULL)
    return -1;//E_ERROR_PLUGIN_FUCKED_UP;
  
  cout << "File version: " << es->fileVersion << endl << flushl;
  if (es->fileVersion != 600)
    cout << "Error: Incorrect version. File is too " << ((es->fileVersion > 600)?"new":"old") << " for this compiler.";
  
  /**
    Segment One: This segment of the compile process is responsible for
    translating the code into C++. Basically, anything essential to the
    compilation of said code is dealt with during this segment.
  */
  
  ///The segment begins by adding resource names to the collection of variables that should not be automatically re-scoped.  
  
  //First, we make a space to put our globals.
    globals_scope = scope_get_using(&global_scope);
    globals_scope = globals_scope->members["ENIGMA Resources"] = new externs;
    globals_scope->name  = "ENIGMA Resources";
    globals_scope->flags = EXTFLAG_NAMESPACE;
    globals_scope->type  = NULL;
  
  
  
  //Next, add the resource names to that list
  cout << "COPYING SOME FUCKING RESOURCES:" << flushl;
  
  cout << "Copying sprite names [" << es->spriteCount << "]" << flushl;
  for (int i = 0; i < es->spriteCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->sprites[i].name);
    
  cout << "Copying sound names [" << es->soundCount << "]" << flushl;
  for (int i = 0; i < es->soundCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->sounds[i].name);
    
  cout << "Copying background names [" << es->backgroundCount << "]" << flushl;
  for (int i = 0; i < es->backgroundCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->backgrounds[i].name);
    
  cout << "Copying path names [kidding, these are totally not implemented :P] [" << es->pathCount << "]" << flushl;
  for (int i = 0; i < es->spriteCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->sprites[i].name);
    
  cout << "Copying script names [" << es->scriptCount << "]" << flushl;
  for (int i = 0; i < es->scriptCount; i++)
    quickmember_script(globals_scope,es->scripts[i].name);
    
  cout << "Copying font names [kidding, these are totally not implemented :P] [" << es->fontCount << "]" << flushl;
  for (int i = 0; i < es->fontCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->fonts[i].name);
    
  cout << "Copying timeline names [kidding, these are totally not implemented :P] [" << es->timelineCount << "]" << flushl;
  for (int i = 0; i < es->timelineCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->timelines[i].name);
  
  cout << "Copying object names [" << es->gmObjectCount << "]" << flushl;
  for (int i = 0; i < es->gmObjectCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->gmObjects[i].name);
  
  cout << "Copying room names [" << es->roomCount << "]" << flushl;
  for (int i = 0; i < es->roomCount; i++)
    quickmember_variable(globals_scope,builtin_type__int,es->rooms[i].name);
  
  
  
  ///Next we do a simple pares of the code, scouting for some variable names and adding semicolons.
  cout << "SYNTAX CHECKING AND PRIMARY PARSING" << flushl;
  
  cout << es->scriptCount << " Scripts:" << endl;
  for (int i = 0; i < es->scriptCount; i++)
  {
    int a = syncheck::syntacheck(es->scripts[i].code);
    if (a != -1) {
      cout << "Syntax error in script `" << es->scripts[i].name << "'" << endl << syncheck::error << flushl;
      return E_ERROR_SYNTAX;
    }
    fflush(stdout);
  }

  cout << es->gmObjectCount << " Objects:" << endl;
  for (int i = 0; i < es->gmObjectCount; i++)
  {
    //For every object in Ism's struct, make our own
    unsigned ev_count = 0;
    parsed_object* pob = parsed_objects[es->gmObjects[i].id] = new parsed_object(es->gmObjects[i].name,es->gmObjects[i].id,es->gmObjects[i].spriteId);
    cout << " " << es->gmObjects[i].name << ": " << es->gmObjects[i].mainEventCount << " sub-events: " << flushl;
    
    for (int ii = 0; ii < es->gmObjects[i].mainEventCount; ii++)
      if (es->gmObjects[i].mainEvents[ii].eventCount) //For every MainEvent that contains event code
    {
      //For each main event in that object, make a copy
      const int mev_id = es->gmObjects[i].mainEvents[ii].id;
      cout << "  Event[" << es->gmObjects[i].mainEvents[ii].id << "]: ";
      
      for (int iii = 0; iii < es->gmObjects[i].mainEvents[ii].eventCount; iii++)
      {
        //For each individual event (like begin_step) in the main event (Step), parse the code
        const int sev_id = es->gmObjects[i].mainEvents[ii].events[iii].id;
        parsed_event &pev = pob->events[ev_count++];
        cout << "[" << mev_id << "," << sev_id << "]";
        pev.mainId = mev_id, pev.id = sev_id;
        
        //Copy the code into a string, and its attributes elsewhere
        string code = es->gmObjects[i].mainEvents[ii].events[iii].code;
        
        //Syntax check the code
        int sc = syncheck::syntacheck(code);
        if (sc != -1)
        {
          cout << "Syntax error in object `" << es->gmObjects[i].name << "', event " << mev_id << ":"
               << es->gmObjects[i].mainEvents[ii].events[iii].id << ":\n" << format_error(code,syncheck::error,sc) << flushl;
          return E_ERROR_SYNTAX;
        }
        
        //Add this to our objects map
        pev.myObj = pob; //link to its parent
        parser_main(code,&pev);
      }
    }
    fflush(stdout);
  }
  
  cout << es->roomCount << " Rooms:" << endl;
  for (int i = 0; i < es->roomCount; i++) {
    cout << " " << es->rooms[i].name << endl;
    fflush(stdout);
  }
  
  ///Now, time to review and "link": Our linking is less complicated than compiled code linking.
  ///This process is designed to offer a better understanding of used variables from event to event
  ///and then from instance to instance.
  cout << "Printing all objects and events: " << flushl;
  for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
  {
    cout << "Object `"<< i->second->name << "':" << flushl;
    for (unsigned ii = 0; ii < i->second->events.size; ii++)
    {
      cout << "Event(" << i->second->events[ii].mainId << "," << i->second->events[ii].id << "):" << flushl;
      cout << i->second->events[ii].code << flushl << flushl << flushl;
    }
  }
  
  //Define some variables based on usage
  bool used__object_set_sprite = 0;
  
  //Export resources to each file.
  
  ofstream wto;
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/GAME_SETTINGS.h",ios_base::out);
    wto << license;
    wto << "#define SHOWERRORS 1\n";
    wto << "#define ABORTONALL 0\n";
    wto << "#define ASSUMEZERO 0\n";
    wto << "#define PRIMBUFFER 0\n";
    wto << "#define PRIMDEPTH2 6\n";
    wto << "#define AUTOLOCALS 0\n";
    wto << "#define MODE3DVARS 0\n";
    wto << "void ABORT_ON_FATAL_ERRORS() { " << (false?"exit(0);":"") << " }\n";
    wto << '\n';
  wto.close();
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h",ios_base::out);
    wto << license;
    wto << "#define BUILDMODE " << 0 << "\n";
    wto << "#define DEBUGMODE " << 0 << "\n";
    wto << '\n';
  wto.close();
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h",ios_base::out);
    wto << license;
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    {
      wto << "case " << i->second->id << ":\n";
      wto << "    enigma::instance_list[idn]=new enigma::" << i->second->name <<";\n";
      wto << "  break;\n";
    }
    wto << '\n';
  wto.close();
  
  
  //NEXT FILE ----------------------------------------
  //Object switch: A listing of all object IDs and the code to allocate them.
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h",ios_base::out);
    wto << license;
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    {
      wto << "case " << i->second->id << ":\n";
      wto << "    enigma::instance_list[idn]=new enigma::OBJ_" << i->second->name <<"(x,y,idn);\n";
      wto << "  break;\n";
    }
    wto << '\n';
  wto.close();
  
  
  //NEXT FILE ----------------------------------------
  //Resource names: Defines integer constants for all resources.
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h",ios_base::out);
    wto << license;
    
    wto << "enum //object names\n{\n";
    for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
      wto << "  " << i->second->name << " = " << i->first << ",\n"; 
    wto << "};\n\n";
    
    wto << "enum //sprite names\n{\n";
    for (int i = 0; i < es->spriteCount; i++)
      wto << "  " << es->sprites[i].name << " = " << es->sprites[i].id << ",\n"; 
    wto << "};\n\n";
    
    wto << "enum //sound names\n{\n";
    for (int i = 0; i < es->soundCount; i++)
      wto << "  " << es->sounds[i].name << " = " << es->sounds[i].id << ",\n"; 
    wto << "};\n\n";
    
    
  wto.close();
  
  
  
  //NEXT FILE ----------------------------------------
  //Object declarations: object classes/names and locals.
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
          wto << tdefault(ii->second.type) << " " << ii->second.prefix << ii->first << ii->second.suffix << ";\n    ";
        
        for (unsigned ii = 0; ii < i->second->events.size; ii++)
        {
          //Look up the event name
          string evname = event_get_enigma_main_name(i->second->events[ii].mainId,i->second->events[ii].id);
          wto << "\n    #define ENIGMAEVENT_" << evname << " 1\n";
          wto << "    variant myevent_" << evname << "();\n  ";
        }
        
        //Automatic constructor
        //Directed constructor (ID is specified)
        wto <<   "\n    OBJ_" <<  i->second->name << "(int enigma_genericconstructor_newinst_x = 0, int enigma_genericconstructor_newinst_y = 0, int id = (enigma::maxid++))";
          wto << ": object_locals(id, " << i->second->id << ")";
          wto << "\n    {\n";
            //Sprite index
              if (used__object_set_sprite) //We want to initialize 
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
        string evname = event_get_enigma_main_name(i->second->events[ii].mainId,i->second->events[ii].id);
        wto << "enigma::variant enigma::OBJ_" << i->second->name << "::myevent_" << evname << "()\n{\n  ";
          print_to_file(i->second->events[ii].code,i->second->events[ii].synt,2,wto);
        wto << "\n  return 0;\n}\n\n";
      }
    }
    
    wto << 
    "namespace enigma\n{\n  void constructor(object_basic* instance_b)\n  {\n"
    "    //This is the universal create event code\n    object_locals* instance = (object_locals*)instance_b;\n    \n"    
    "    instance_list[instance->id]=instance_b;\n\n"
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
  
  //NEXT FILE ----------------------------------------
  //Object functions: events, constructors, other codes.
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h",ios_base::out);
    wto << license;
    int room_highid = 0, room_highinstid = 100000;
    for (int i = 0; i < es->roomCount; i++) 
    {
      wto <<
      "  enigma::roomdata[" << es->rooms[i].id << "].name = \"" << es->rooms[i].name << "\";\n"
      "  enigma::roomdata[" << es->rooms[i].id << "].cap = \"" << es->rooms[i].caption << "\";\n"
      "  enigma::roomdata[" << es->rooms[i].id << "].backcolor = " << lgmRoomBGColor(es->rooms[i].backgroundColor) << ";\n"
      "  enigma::roomdata[" << es->rooms[i].id << "].spd = " << es->rooms[i].speed << ";\n"
      "  enigma::roomdata[" << es->rooms[i].id << "].width = " << es->rooms[i].width << ";\n"
      "  enigma::roomdata[" << es->rooms[i].id << "].height = " << es->rooms[i].height << ";\n"
      "  enigma::roomdata[" << es->rooms[i].id << "].views_enabled = " << es->rooms[i].enableViews << ";\n";
      for (int ii = 0; ii < es->rooms[i].viewCount; ii++)
      {
        wto << 
        "    enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].start_vis = 0;\n"
        "    enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].area_x = 0; enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].area_y = 0;"
           " enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].area_w = 640; enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].area_h = 480;\n"
        "    enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].port_x = 0;   enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].port_y = 0;"
           " enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].port_w = 640; enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].port_h = 480;\n"
        "    enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].object2follow = 0;\n"
        "    enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].hborder=32; enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].vborder = 32;"
           " enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].hspd = -1;  enigma::roomdata[" << es->rooms[i].id << "].views[" << ii << "].vspd = -1;\n";
      }
      wto << 
      "  enigma::roomdata[" << es->rooms[i].id << "].instancecount = " << es->rooms[i].instanceCount << ";\n"
      "  enigma::roomdata[" << es->rooms[i].id << "].createcode = (void(*)())roomcreate" << es->rooms[i].id << ";";
      
      if (es->rooms[i].id > room_highid)
        room_highid = es->rooms[i].id;
    }
    
    wto << "int instdata[] = {";
    for (int i = 0; i < es->roomCount; i++)
      for (int ii = 0; ii < es->rooms[i].instanceCount; ii++) {
        wto << 
          es->rooms[i].instances[ii].id << "," << 
          es->rooms[i].instances[ii].objectId << "," << 
          es->rooms[i].instances[ii].x << "," << 
          es->rooms[i].instances[ii].y << ",";
        if (es->rooms[i].instances[ii].id > room_highinstid)
          room_highinstid = es->rooms[i].instances[ii].id;
      }
    wto << "};\n\n";
    wto << "enigma::room_max = " <<  room_highid << " + 1;\nenigma::maxid = " << room_highinstid << " + 1;\n";
  wto.close();
  
  /*
    Segment three: Add resources into the game executable
  */
  
  cout << es->spriteCount << " Sprites:" << endl;
  for (int i = 0; i < es->spriteCount; i++)
  {
    cout << "Sprite " << es->sprites[i].name << endl;
//    cout << "Data at: " << es->sprites[i].subImages[0].pixels << endl << endl;
    //for (int ii = 0; ii <  es->sprites[i].subImages[0].width *  es->sprites[i].subImages[0].height; ii++) {
    //  cout << (void*)(es->sprites[i].subImages[0].pixels[ii]);
    //}
    fflush(stdout);
  }
  
  cout << es->soundCount << " Sounds:" << endl;
  for (int i = 0; i < es->soundCount; i++) {
    cout << " " << es->sounds[i].name << endl;
    fflush(stdout);
  }
  
  
  /*
  cout << es->includeCount << " Includes:" << endl;
  for (int i = 0; i < es->includeCount; i++) {
    cout << " " << es->includes[i].filepath << endl;
    fflush(stdout);
  }
  */
  return 0;
};
