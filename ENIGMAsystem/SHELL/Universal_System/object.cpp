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
#include "var4.h"
#include "reflexive_types.h"

#include "object.h"
#include "../libEGMstd.h"

namespace enigma
{
    extern int maxid;
    int instancecount = 0;
    int id_current =0;
    
    double newinst_x, newinst_y;
    int newinst_obj, newinst_id;
    
    void object_basic::unlink() {}
    variant object_basic::myevent_draw()    { return 0; }
    variant object_basic::myevent_roomend() { return 0; }
    variant object_basic::myevent_destroy() { return 0; }
      
    object_basic::object_basic(): id(0), object_index(-4) {}
    object_basic::object_basic(int uid, int uoid): id(uid), object_index(uoid) {}
    object_basic::~object_basic() {}
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
    
    instance->hspeed.reflex1=&instance->vspeed.rval.d;
      instance->hspeed.reflex2=&instance->direction.rval.d;
      instance->hspeed.reflex3=&instance->speed.rval.d;
    instance->vspeed.reflex1=&instance->hspeed.rval.d;
      instance->vspeed.reflex2=&instance->direction.rval.d;
      instance->vspeed.reflex3=&instance->speed.rval.d;
    instance->direction.reflex1=&instance->speed.rval.d;
      instance->direction.reflex2=&instance->hspeed.rval.d;
      instance->direction.reflex3=&instance->vspeed.rval.d;
    instance->speed.reflex1=&instance->direction.rval.d;
      instance->speed.reflex2=&instance->hspeed.rval.d;
      instance->speed.reflex3=&instance->vspeed.rval.d;
    
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

