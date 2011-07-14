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

#include <string>
#include "OpenGLHeaders.h"
#include "GSbackground.h"

using namespace std;

#include "../../Universal_System/var4.h"

#define __GETR(x) (((unsigned int)x & 0x0000FF))
#define __GETG(x) (((unsigned int)x & 0x00FF00) >> 8)
#define __GETB(x) (((unsigned int)x & 0xFF0000) >> 16)

#include "../../Universal_System/roomsystem.h"
#include "../../Universal_System/instance_system.h"
#include "../../Universal_System/graphics_object.h"
#include "../../Universal_System/depth_draw.h"
#include "../../Platforms/platforms_mandatory.h"

using namespace enigma;

static inline void draw_back()
{
  //Draw backgrounds
  for (int back_current=0; back_current<7; back_current++)
  {
    if (background_visible[back_current] == 1) {
      // if (background_stretched) draw_background_stretched(back, x, y, w, h);
      draw_background_tiled(background_index[back_current], background_x[back_current], background_y[back_current]);
    }
    // background_foreground, background_index, background_x, background_y, background_htiled,
    // background_vtiled, background_hspeed, background_vspeed;
  }
}

namespace enigma
{
    extern std::map<int,roomstruct*> roomdata;
}

void screen_redraw()
{
  if (!view_enabled)
  {
     glViewport(0,0,window_get_width(),window_get_height());
     glLoadIdentity();
     glScaled(1,-1,1);
     glOrtho(-1,room_width,-1,room_height,0,1);

    if (background_showcolor)
    {
       int clearcolor=((int)background_color)&0xFFFFFF;
       glClearColor(__GETR(clearcolor)/255.0,__GETG(clearcolor)/255.0,__GETB(clearcolor)/255.0, 1);
       glClear(GL_COLOR_BUFFER_BIT);
    }
      draw_back();
      
      
      for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++) {
          
          //loop tiles
          for(std::vector<tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++) {
              tile t = dit->second.tiles[i];
              draw_background_part(t.bckid,t.bgx,t.bgy,t.width,t.height,t.roomX,t.roomY);
          }
          
          for (enigma::instance_event_iterator = dit->second.draw_events->next; enigma::instance_event_iterator != NULL; enigma::instance_event_iterator = enigma::instance_event_iterator->next)
              enigma::instance_event_iterator->inst->myevent_draw();
          
      }
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
    glOrtho(view_xview[vc]-1,view_wview[vc]+view_xview[vc],view_yview[vc]-1,view_hview[vc]+view_yview[vc],0,1);
    
    if (background_showcolor)
    {
       int clearcolor=((int)background_color)&0xFFFFFF;
       glClearColor(__GETR(clearcolor)/255.0,__GETG(clearcolor)/255.0,__GETB(clearcolor)/255.0, 1);
       glClear(GL_COLOR_BUFFER_BIT);
    }
    draw_back();
    
    
      
    for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++) {
        
        //loop tiles
        for(std::vector<tile>::size_type i = 0; i !=  dit->second.tiles.size(); i++) {
            tile t = dit->second.tiles[i];
            draw_background_part(t.bckid,t.bgx,t.bgy,t.width,t.height,t.roomX,t.roomY);
        }
        
        //loop instances
      for (enigma::instance_event_iterator = dit->second.draw_events->next; enigma::instance_event_iterator != NULL; enigma::instance_event_iterator = enigma::instance_event_iterator->next)
        enigma::instance_event_iterator->inst->myevent_draw();
        
       
        
    }
        
    
  }
}

