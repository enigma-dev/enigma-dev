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

#include <map>
#include <math.h>
#include <string>
#include "var_cr3.h"
#include "reflexive_types.h"

const double pi=M_PI;

#include "object.h"
//#include "sprite.h"

#include "objecttable.h"

//#include "GAME_GLOBALS.h"

#include "var_cr3.h"

int show_error(std::string errortext,int fatal);

namespace enigma
{
    int maxid = 100001;
    int instancecount = 0;
    int id_current =0;
    
    double newinst_x, newinst_y;
    int newinst_obj, newinst_id;
    
    enigma::variant object_basic::myevent_create()        { return 0; }
    
    variant object_basic::myevent_gamestart()     { return 0; }
    variant object_basic::myevent_roomstart()     { return 0; }
    variant object_basic::myevent_beginstep()     { return 0; }
    variant object_basic::myevent_alarm()         { return 0; }
    variant object_basic::myevent_keyboard()      { return 0; }
    variant object_basic::myevent_keypress()      { return 0; }
    variant object_basic::myevent_keyrelease()    { return 0; }
    variant object_basic::myevent_mouse()         { return 0; }
    variant object_basic::myevent_step()          { return 0; }
    variant object_basic::myevent_pathend()       { return 0; }
    variant object_basic::myevent_outsideroom()   { return 0; }
    variant object_basic::myevent_boundary()      { return 0; }
    variant object_basic::myevent_collision()     { return 0; }
    variant object_basic::myevent_nomorelives()   { return 0; }
    variant object_basic::myevent_nomorehealth()  { return 0; }
    variant object_basic::myevent_endstep()       { return 0; }
    variant object_basic::myevent_draw()          { return 0; }
    variant object_basic::myevent_endanimation()  { return 0; }
    variant object_basic::myevent_roomend()       { return 0; }
    variant object_basic::myevent_gameend()       { return 0; }

    variant object_basic::myevent_destroy()       { return 0; }
      
    object_basic::object_basic(): id(0), object_index(-4) {}
    object_basic::object_basic(int uid, int uoid): id(uid), object_index(uoid) {}
    object_basic::~object_basic() {}
    
    object_basic *ENIGMA_global_instance;
    


    //#include "imap.h"

    /*//imap instance_list;
    iiterator instance_iterator;
    iiterator instance_event_iterator;
    iiterator instance_with_iterator;*/

    std::map<int,object_basic*> instance_list;
    std::map<int,object_basic*>::iterator instance_iterator;
    std::map<int,object_basic*>::iterator instance_event_iterator;
    
    std::map<int,int> cleanups;
    int cleancount=0;
    
    int object_basic::instance_destroy()
    {
      cleanups[cleancount++] = id;
      return 0;
    }
    
    void constructor(object_basic* instance); //This is the universal create event code
    void step_basic(object_basic* instance);
}



/*
namespace enigma
{
	enigma::variant object_basic::myevent_draw()
	{
		if (sprite_index != -1)
			draw_sprite_ext(sprite_index,image_index,x,y,image_xscale,image_yscale,image_angle,image_blend,image_alpha);
		return 0;
	}
  void constructor(object_basic* instance)
  {
    //This is the universal create event code
    instance_list[newinst_id]=instance;
    
    instance->x = newinst_x;
    instance->y = newinst_y;
    
    instance->xstart = newinst_x;
    instance->ystart = newinst_y;
    instance->xprevious = newinst_x;
    instance->yprevious = newinst_y;
    
    
    instance->gravity=0;
    instance->gravity_direction=270;
    instance->friction=0;
    instance->sprite_index = enigma::objectdata[newinst_obj].sprite_index;
    instance->mask_index = enigma::objectdata[newinst_obj].mask_index;
    instance->visible = enigma::objectdata[newinst_obj].visible;
    instance->solid = enigma::objectdata[newinst_obj].solid;
    instance->persistent = enigma::objectdata[newinst_obj].persistent;
    instance->depth = enigma::objectdata[newinst_obj].depth;
    
    instance->hspeed.reflex1=&instance->vspeed.realval;
      instance->hspeed.reflex2=&instance->direction.realval;
      instance->hspeed.reflex3=&instance->speed.realval;
    instance->vspeed.reflex1=&instance->hspeed.realval;
      instance->vspeed.reflex2=&instance->direction.realval;
      instance->vspeed.reflex3=&instance->speed.realval;
    instance->direction.reflex1=&instance->speed.realval;
      instance->direction.reflex2=&instance->hspeed.realval;
      instance->direction.reflex3=&instance->vspeed.realval;
    instance->speed.reflex1=&instance->direction.realval;
      instance->speed.reflex2=&instance->hspeed.realval;
      instance->speed.reflex3=&instance->vspeed.realval;
    
    for(int i=0;i<16;i++)
      instance->alarm[i]=-1;
    
    if(instance->sprite_index!=-1)
    {
      instance->bbox_bottom=sprite_get_bbox_bottom(instance->sprite_index);
      instance->bbox_left=sprite_get_bbox_left(instance->sprite_index);
      instance->bbox_right=sprite_get_bbox_right(instance->sprite_index);
      instance->bbox_top=sprite_get_bbox_top(instance->sprite_index);
      instance->sprite_height=sprite_get_height(instance->sprite_index);
      instance->sprite_width=sprite_get_width(instance->sprite_index);
      instance->sprite_xoffset=sprite_get_xoffset(instance->sprite_index);
      instance->sprite_yoffset=sprite_get_yoffset(instance->sprite_index);
      instance->image_number=0;//sprite_get_number(instance->sprite_index);
    }
    
    instance->image_alpha=1.0;
    instance->image_angle=0;
    instance->image_blend=0xFFFFFF;
    instance->image_index=0;
    instance->image_single=-1;
    instance->image_speed=1;
    instance->image_xscale=1;
    instance->image_yscale=1;
    
    / * instance->path_endaction;
    instance->path_index;
    instance->path_orientation;
        instance->path_position;
        instance->path_positionprevious;
        instance->path_scale;
        instance->path_speed;
        instance->timeline_index;
        instance->timeline_position;
        instance->timeline_speed;     * /
        //instance->sprite_index = enigma::objectinfo[newinst_obj].sprite_index;
    
    instancecount++;
    instance_count++;
}
  }*/

