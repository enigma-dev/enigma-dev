/*********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura, Adam Domurad                               **
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
/*
collision_point(x,y,obj,prec,notme)
collision_rectangle(x,y,x2,y2,obj,prec,notme)
collision_circle(xc,yc,radius,obj,prec,notme)
collision_ellipse(x,y,x2,y2,obj,prec,notme)
collision_line(x,y,x2,y2,obj,prec,notme)
*/

int collision_point(double x,double y,double object,double prec,double notme)
{
  if (!prec)
  {
    int obj=(int)object;
    if (obj==-3)
    for (enigma::instance_iterator=enigma::instance_list.begin();
    enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
    {
      int ox=(*enigma::instance_iterator).second->x;
      int oy=(*enigma::instance_iterator).second->y;

      int bl=(*enigma::instance_iterator).second->bbox_left;
      int br=(*enigma::instance_iterator).second->bbox_right;
      int bt=(*enigma::instance_iterator).second->bbox_top;
      int bb=(*enigma::instance_iterator).second->bbox_bottom;
      if (x<ox+br && x>ox+bl && y<oy+bb && y>oy+bt)
      return 1;
    }
    else if (obj>=0 && obj<100000)
    for (enigma::instance_iterator=enigma::instance_list.begin();
    enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
    {
      if ((*enigma::instance_iterator).second->object_index==obj)
      {
        int ox=(*enigma::instance_iterator).second->x;
        int oy=(*enigma::instance_iterator).second->y;

        int bl=(*enigma::instance_iterator).second->bbox_left;
        int br=(*enigma::instance_iterator).second->bbox_right;
        int bt=(*enigma::instance_iterator).second->bbox_top;
        int bb=(*enigma::instance_iterator).second->bbox_bottom;
        if (x<ox+br && x>ox+bl && y<oy+bb && y>oy+bt)
        return 1;
      }
    }
    else if (obj>=100000)
    {
      enigma::instance_iterator=enigma::instance_list.find(obj);
      if (enigma::instance_iterator != enigma::instance_list.end())
      {
        if ((*enigma::instance_iterator).second->id==obj)
        {
          int ox=(*enigma::instance_iterator).second->x;
          int oy=(*enigma::instance_iterator).second->y;

          int bl=(*enigma::instance_iterator).second->bbox_left;
          int br=(*enigma::instance_iterator).second->bbox_right;
          int bt=(*enigma::instance_iterator).second->bbox_top;
          int bb=(*enigma::instance_iterator).second->bbox_bottom;
          if (x<ox+br && x>ox+bl && y<oy+bb && y>oy+bt)
          return 1;
        }
      }
    }
    return 0;
  }
  else
  {
    int obj=(int)object;
    if (obj==-3)
    {
      for (enigma::instance_iterator=enigma::instance_list.begin();
      enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
      {
        if ((*enigma::instance_iterator).second->sprite_index!=-1)
          if (collCheck(*enigma::spritestructarray[(*enigma::instance_iterator).second->sprite_index]->bitmask[(*enigma::instance_iterator).second->image_index],
            (double)(*enigma::instance_iterator).second->x,(double)(*enigma::instance_iterator).second->y,x,y,(double)(*enigma::instance_iterator).second->image_angle,
            (double)(*enigma::instance_iterator).second->image_xscale,(double)(*enigma::instance_iterator).second->image_yscale))
          return (int)(*enigma::instance_iterator).second->id;
      }
      return -4;
    }
    else if (obj>=0 && obj<100000)
    {
      for (enigma::instance_iterator=enigma::instance_list.begin();
      enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
      {
        if ((*enigma::instance_iterator).second->object_index==obj)
        {
          if ((*enigma::instance_iterator).second->sprite_index!=-1)
          if (collCheck(*enigma::spritestructarray[(*enigma::instance_iterator).second->sprite_index]->bitmask[(*enigma::instance_iterator).second->image_index],
            (double)(*enigma::instance_iterator).second->x,(double)(*enigma::instance_iterator).second->y,x,y,(double)(*enigma::instance_iterator).second->image_angle,
            (double)(*enigma::instance_iterator).second->image_xscale,(double)(*enigma::instance_iterator).second->image_yscale))
          return (int)(*enigma::instance_iterator).second->id;
        }
      }
      return -4;
    }
    else if (obj>=100000)
    {
      enigma::instance_iterator=enigma::instance_list.find(obj);
      if (enigma::instance_iterator != enigma::instance_list.end())
      {
        if ((*enigma::instance_iterator).second->sprite_index!=-1)
          if (collCheck(*enigma::spritestructarray[(*enigma::instance_iterator).second->sprite_index]->bitmask[(*enigma::instance_iterator).second->image_index],
            (double)(*enigma::instance_iterator).second->x,(double)(*enigma::instance_iterator).second->y,x,y,(double)(*enigma::instance_iterator).second->image_angle,
            (double)(*enigma::instance_iterator).second->image_xscale,(double)(*enigma::instance_iterator).second->image_yscale))
          return (int)(*enigma::instance_iterator).second->id;
      }
      return -4;
    }
    return -4;
  }
}



bool place_free(double x,double y)
{
  enigma::object* me=(*enigma::instance_event_iterator).second;
      for (enigma::instance_iterator=enigma::instance_list.begin();
      enigma::instance_iterator != enigma::instance_list.end(); enigma::instance_iterator++)
      {
        if ((*enigma::instance_iterator).second->sprite_index!=-1 && me->sprite_index!=-1)
          if (collCheck(*(enigma::spritestructarray[me->sprite_index]->bitmask[me->image_index]), x,y,
          *(enigma::spritestructarray[(*enigma::instance_iterator).second->sprite_index]->bitmask[(*enigma::instance_iterator).second->image_index]),
          (*enigma::instance_iterator).second->x,(*enigma::instance_iterator).second->y,me->image_angle,(*enigma::instance_iterator).second->image_angle))
          return 0;
      }
      return 1;
}
