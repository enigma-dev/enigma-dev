/*********************************************************************************\
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
\*********************************************************************************/

const int self=-1;
const int other=-2;
const int all=-3;
const int noone=-4;
const int global=-5;
const int local=-7;



namespace enigma
{
    int maxid=100001;
    int instancecount=0;
    int id_current=0;
    
    double newinst_x, newinst_y;
    int newinst_obj, newinst_id;
    
    struct object
    {
        #include "../Preprocessor_Environment_Editable/IDE_EDIT_inherited_locals.h"
        
        virtual enigma::variant myevent_create()        { return 0; }
        
        virtual enigma::variant myevent_gamestart()     { return 0; }
        virtual enigma::variant myevent_roomstart()     { return 0; }
        virtual enigma::variant myevent_beginstep()     { return 0; }
        virtual enigma::variant myevent_alarm()         { return 0; }
        virtual enigma::variant myevent_keyboard()      { return 0; }
        virtual enigma::variant myevent_keypress()      { return 0; }
        virtual enigma::variant myevent_keyrelease()    { return 0; }
        virtual enigma::variant myevent_mouse()         { return 0; }
        virtual enigma::variant myevent_step()          { return 0; }
        virtual enigma::variant myevent_pathend()       { return 0; }
        virtual enigma::variant myevent_outsideroom()   { return 0; }
        virtual enigma::variant myevent_boundary()      { return 0; }
        virtual enigma::variant myevent_collision()     { return 0; }
        virtual enigma::variant myevent_nomorelives()   { return 0; }
        virtual enigma::variant myevent_nomorehealth()  { return 0; }
        virtual enigma::variant myevent_endstep()       { return 0; }
        virtual enigma::variant myevent_draw()                      ;
        virtual enigma::variant myevent_endanimation()  { return 0; }
        virtual enigma::variant myevent_roomend()       { return 0; }
        virtual enigma::variant myevent_gameend()       { return 0; }
        
        virtual enigma::variant myevent_destroy()       { return 0; }
        
        virtual ~object() {}
    };
    
    struct globals:public object
    {
        #include "../Preprocessor_Environment_Editable/IDE_EDIT_withdeclarations.h"
        globals(void) { object_index=-5; }
    };
    object* ENIGMA_global_instance=new globals;
    
    //#include "imap.h"
    
    /*//imap instance_list;
    iiterator instance_iterator;
    iiterator instance_event_iterator;
    iiterator instance_with_iterator;*/
    
    std::map<int,object*> instance_list;
    std::map<int,object*>::iterator instance_iterator;
    std::map<int,object*>::iterator instance_event_iterator;
    
    std::map<int,int> cleanups;
    int cleancount=0;
    
    
    void constructor(object* instance);//This is the universal create event code
    
    void step_basic(object* instance);
    
    #include "../Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h"
}







/*
Source
*/

namespace enigma
{
    enigma::variant object::myevent_draw()
    { 
      if (sprite_index!=-1) 
      draw_sprite_ext(sprite_index,image_index,x,y,image_xscale,image_yscale,image_angle,image_blend,image_alpha);
      return 0;
    }
    
    void constructor(object* instance)//This is the universal create event code
    {
        instance_list[newinst_id]=instance;
        
        instance->x = newinst_x;
        instance->y = newinst_y;
        instance->object_index = newinst_obj;
        instance->id = newinst_id;
        
        instance->xstart = newinst_x;
        instance->ystart = newinst_y;
        instance->xprevious = newinst_x;
        instance->yprevious = newinst_y;
        
        
        instance->gravity=0; //gravity is a lie!
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
        
        for (int i=0;i<16;i++)
        instance->alarm[i]=-1;
        
        if (instance->sprite_index!=-1)
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
        /*instance->path_endaction;
        instance->path_index;
        instance->path_orientation;
        instance->path_position;
        instance->path_positionprevious;
        instance->path_scale;
        instance->path_speed;
        instance->timeline_index;
        instance->timeline_position;
        instance->timeline_speed;*/
        
        
        //instance->sprite_index = enigma::objectinfo[newinst_obj].sprite_index;
        
        instancecount++;
        instance_count++;
    }
    
    void step_basic(object* instance)
    {
        instance->xprevious=instance->x;
        instance->yprevious=instance->y;
        
        if (instance->gravity != 0 || instance->friction != 0)
        {
          double hb4=instance->hspeed.realval,vb4=instance->vspeed.realval;
          int factr=0; if (instance->speed>0) factr=1; if (instance->speed<0) factr=-1;
            instance->hspeed.realval-=factr*(instance->friction*cos(instance->direction.realval/180.0*pi));
              if ((hb4>0 && instance->hspeed.realval<0) || (hb4<0 && instance->hspeed.realval>0)) instance->hspeed.realval=0;
            instance->vspeed.realval-=factr*(instance->friction*-sin(instance->direction.realval/180.0*pi));
              if ((vb4>0 && instance->vspeed.realval<0) || (vb4<0 && instance->vspeed.realval>0)) instance->vspeed.realval=0;
          
          instance->hspeed.realval+=instance->gravity*cos(instance->gravity_direction/180.0*pi);
          instance->vspeed.realval+=instance->gravity*-sin(instance->gravity_direction/180.0*pi);
          
          if (instance->speed.realval<0)
          {
            instance->direction.realval=180+(int(180+180*(1-atan2(instance->vspeed.realval,instance->hspeed.realval)/pi)))%360; 
            instance->speed.realval=-sqrt(instance->hspeed.realval*instance->hspeed.realval+instance->vspeed.realval*instance->vspeed.realval);
          }
          else
          {
            instance->direction.realval=(int(180+180*(1-atan2(instance->vspeed.realval,instance->hspeed.realval)/pi)))%360; 
            instance->speed.realval=sqrt(instance->hspeed.realval*instance->hspeed.realval+instance->vspeed.realval*instance->vspeed.realval);
          }
        }
        instance->x+=instance->hspeed.realval;
        instance->y+=instance->vspeed.realval;
    }
}
