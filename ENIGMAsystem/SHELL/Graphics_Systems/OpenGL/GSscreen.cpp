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

/*********************************************
int screen_redraw(int dontswap)
**********************************************/

#include <GL/gl.h>

#define __GETR(x) (((unsigned int)x & 0x0000FF))
#define __GETG(x) (((unsigned int)x & 0x00FF00) >> 8)
#define __GETB(x) (((unsigned int)x & 0xFF0000) >> 16)

#include "../../Universal_System/roomsystem.h"
#include "../../Universal_System/instance_system.h"
#include "graphics_object.h"

extern int window_get_width(), window_get_height();

namespace enigma {
  extern event_iter *event_draw;
}

using namespace enigma;

int screen_redraw()
{
    if (!view_enabled)
    {
       glViewport(0,0,window_get_width(),window_get_height());
       glLoadIdentity();
       glScaled(1,-1,1);
       glOrtho(0,room_width,0,room_height,0,1);

      if (background_showcolor)
      {
         int clearcolor=((int)background_color)&0xFFFFFF;
         glClearColor(__GETR(clearcolor)/255.0,__GETG(clearcolor)/255.0,__GETB(clearcolor)/255.0, 1);
         glClear(GL_COLOR_BUFFER_BIT);
      }

      for (inst_iter *i = event_draw->insts; i != NULL; i = i->next)
         i->inst->myevent_draw();
    }
    else 
    for (view_current=0; view_current<7; view_current++)
    if (view_visible[(int)view_current])
    {
      int vc=(int)view_current;
      int vob=(int)view_object[vc];
      
      if (vob != -1)
      {
        object_basic *instanceexists = fetch_instance_by_int(vob);
        
        if (instanceexists)
        {
          object_planar* vobr = (object_planar*)instanceexists;
          
          int vobx=(int)(vobr->x),voby=(int)(vobr->y);
          //int bbl=*vobr.x+*vobr.bbox_left,bbr=*vobr.x+*vobr.bbox_right,bbt=*vobr.y+*vobr.bbox_top,bbb=*vobr.y+*vobr.bbox_bottom;
          //if (bbl<view_xview[vc]+view_hbor[vc]) view_xview[vc]=bbl-view_hbor[vc];
          if (vobx<view_xview[vc]+view_hborder[vc]) view_xview[vc]=vobx-view_hborder[vc];
          if (vobx>view_xview[vc]+view_wview[vc]-view_hborder[vc]) view_xview[vc]=vobx+view_hborder[vc]-view_wview[vc];
          if (voby<view_yview[vc]+view_vborder[vc]) view_yview[vc]=voby-view_vborder[vc];
          if (voby>view_yview[vc]+view_hview[vc]-view_vborder[vc]) view_yview[vc]=voby+view_vborder[vc]-view_hview[vc];
          if (view_xview[vc]<0) view_xview[vc]=0;
          if (view_yview[vc]<0) view_yview[vc]=0;
          if (view_xview[vc]>room_width-view_wview[vc]) view_xview[vc]=room_width-view_wview[vc];
          if (view_yview[vc]>room_height-view_hview[vc]) view_yview[vc]=room_height-view_hview[vc];
        }
      }
      
      glViewport((int)view_xport[vc],(int)view_yport[vc],(int)view_wport[vc],(int)view_hport[vc]);
      glLoadIdentity();
      glScaled(1,-1,1);
      glOrtho((int)view_xview[vc],(int)view_wview[vc]+(int)view_xview[vc],(int)view_yview[vc],(int)view_hview[vc]+(int)view_yview[vc],0,1);
      
      if (background_showcolor)
      {
         int clearcolor=((int)background_color)&0xFFFFFF;
         glClearColor(__GETR(clearcolor)/255.0,__GETG(clearcolor)/255.0,__GETB(clearcolor)/255.0, 1);
         glClear(GL_COLOR_BUFFER_BIT);
      }
      
      for (inst_iter *i = event_draw->insts; i != NULL; i = i->next)
        i->inst->myevent_draw();
    }
    
    return 0;
}


#undef __GETR
#undef __GETG
#undef __GETB

