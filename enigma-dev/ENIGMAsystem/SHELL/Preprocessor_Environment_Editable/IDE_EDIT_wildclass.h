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


namespace enigma
{
  struct wildclass
  {
    //Contains 0 variables
    
    //Contains 49 locals
     var  *alarm;
     var  *bbox_bottom;
     var  *bbox_left;
     var  *bbox_right;
     var  *bbox_top;
     var  *depth;
     var  *friction;
     var  *gravity;
     var  *gravity_direction;
     var  *image_alpha;
     var  *image_angle;
     var  *image_blend;
     var  *image_index;
     var  *image_number;
     var  *image_single;
     var  *image_speed;
     var  *image_xscale;
     var  *image_yscale;
     var  *mask_index;
     var  *path_endaction;
     var  *path_index;
     var  *path_orientation;
     var  *path_position;
     var  *path_positionprevious;
     var  *path_scale;
     var  *path_speed;
     var  *persistent;
     var  *solid;
     var  *sprite_height;
     var  *sprite_index;
     var  *sprite_width;
     var  *sprite_xoffset;
     var  *sprite_yoffset;
     var  *timeline_index;
     var  *timeline_position;
     var  *timeline_speed;
     var  *visible;
     var  *x;
     var  *xprevious;
     var  *xstart;
     var  *y;
     var  *yprevious;
     var  *ystart;
     enigma::directionv  *direction;
     enigma::hspeedv  *hspeed;
     enigma::speedv  *speed;
     enigma::vspeedv  *vspeed;
     double  *id;
     double  *object_index;
    //Contains 0 different types
    
    
    void setto(object* from)
    {
      alarm = &from->alarm;
      bbox_bottom = &from->bbox_bottom;
      bbox_left = &from->bbox_left;
      bbox_right = &from->bbox_right;
      bbox_top = &from->bbox_top;
      depth = &from->depth;
      friction = &from->friction;
      gravity = &from->gravity;
      gravity_direction = &from->gravity_direction;
      image_alpha = &from->image_alpha;
      image_angle = &from->image_angle;
      image_blend = &from->image_blend;
      image_index = &from->image_index;
      image_number = &from->image_number;
      image_single = &from->image_single;
      image_speed = &from->image_speed;
      image_xscale = &from->image_xscale;
      image_yscale = &from->image_yscale;
      mask_index = &from->mask_index;
      path_endaction = &from->path_endaction;
      path_index = &from->path_index;
      path_orientation = &from->path_orientation;
      path_position = &from->path_position;
      path_positionprevious = &from->path_positionprevious;
      path_scale = &from->path_scale;
      path_speed = &from->path_speed;
      persistent = &from->persistent;
      solid = &from->solid;
      sprite_height = &from->sprite_height;
      sprite_index = &from->sprite_index;
      sprite_width = &from->sprite_width;
      sprite_xoffset = &from->sprite_xoffset;
      sprite_yoffset = &from->sprite_yoffset;
      timeline_index = &from->timeline_index;
      timeline_position = &from->timeline_position;
      timeline_speed = &from->timeline_speed;
      visible = &from->visible;
      x = &from->x;
      xprevious = &from->xprevious;
      xstart = &from->xstart;
      y = &from->y;
      yprevious = &from->yprevious;
      ystart = &from->ystart;
      direction = &from->direction;
      hspeed = &from->hspeed;
      speed = &from->speed;
      vspeed = &from->vspeed;
      id = &from->id;
      object_index = &from->object_index;
      
      if (from->object_index==-5) //Global
      {
      }
    }
    
    wildclass() {}
    wildclass(object* from)            { setto(from); }
    wildclass &operator=(object *from) { setto(from); return *this; }  };
}
