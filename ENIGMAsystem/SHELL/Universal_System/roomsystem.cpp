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

#include "ENIGMA_GLOBALS.h" // TODO: Do away with this sloppy infestation permanently!

#include "../Platforms/platforms_mandatory.h"
#include "../Widget_Systems/widgets_mandatory.h"
#include "../libEGMstd.h"
#include "instance_system.h"
#include "instance.h"
#include "planar_object.h"

#include "roomsystem.h"
#include "depth_draw.h"

int room_first  = 0;
int room_last   = 0;
int room_speed  = 60;
int room_width  = 640;
int room_height = 480;
int room_persistent = 0;
var room_caption = "ENIGMA Engine";

int background_color = 16777215;
int background_showcolor=1;

var background_visible, background_foreground, background_index, background_x, background_y, background_htiled,
background_vtiled, background_hspeed, background_vspeed,background_alpha;

int view_current = 0;
int view_enabled = 0;
rvt view_hborder, view_hport, view_hspeed, view_hview, view_object, view_vborder,
    view_visible, view_vspeed, view_wport, view_wview, view_xport, view_xview, view_yport, view_yview,view_angle;

namespace enigma
{
  roomstruct** roomdata;
  roomstruct** roomorder;

  void roomstruct::gotome()
  {
    //Destroy all objects
    enigma::nodestroy=1;
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
    {
      it->myevent_roomend();
      #ifdef ISCONTROLLER_persistent
      if (!it->persistent)
      #endif
      instance_destroy(it->id);
    }
    enigma::nodestroy = 0;

    // Set the index to self
    room.rval.d = id;

    room_caption = cap;
    room_width   = width;
    room_height  = height;
    room_speed   = spd;

    background_color = backcolor;
    background_showcolor = drawbackcolor;

    //Backgrounds start
    for (int i=0;i<8;i++)
    {
      background_visible[i] = backs[i].visible;
      background_foreground[i] = backs[i].foreground;
      background_index[i] = backs[i].background;
      background_x[i] = backs[i].area_x; background_y[i] = backs[i].area_y;
      background_hspeed[i] = backs[i].horSpeed; background_vspeed[i] = backs[i].verSpeed;
      background_htiled[i] = backs[i].tileHor; background_vtiled[i] = backs[i].tileVert;
      //background_stretch? = backs[i].stretch;
    }
    //Backgrounds end

      //Tiles start
      drawing_depths.clear();
      for (int tilei=0; tilei<enigma::roomdata[room]->tilecount; tilei++) {
          tile t = enigma::roomdata[room]->tiles[tilei];
          drawing_depths[t.depth].tiles.push_back(enigma::roomdata[room]->tiles[tilei]);
      }
      //Tiles end

    view_enabled = views_enabled;

    for (int i=0;i<8;i++)
    {
      view_xview[i] = views[i].area_x; view_yview[i] = views[i].area_y; view_wview[i] = views[i].area_w; view_hview[i] = views[i].area_h;
      view_xport[i] = views[i].port_x; view_yport[i] = views[i].port_y; view_wport[i] = views[i].port_w; view_hport[i] = views[i].port_h;
      view_object[i] = views[i].object2follow;
      view_hborder[i] = views[i].hborder; view_vborder[i] = views[i].vborder; view_hspeed[i] = views[i].hspd; view_vspeed[i] = views[i].vspd;
      view_visible[i] = (bool)views[i].start_vis;
    }

    int xm = int(room_width), ym = int(room_height);
    if (view_enabled)
    {
      int tx = 0, ty = 0;
      for (int i = 0; i < 8; i++)
        if (view_visible[i])
        {
          if (view_xport[i]+view_wport[i] > tx)
            tx = (int)(view_xport[i]+view_wport[i]);
          if (view_yport[i]+view_hport[i] > ty)
            ty = (int)(view_yport[i]+view_hport[i]);
        }
      if (tx and ty)
        xm = tx, ym = ty;
    }
    window_set_size(xm,ym);
    for (int i=0; i<instancecount; i++) {
      inst *obj = &instances[i];
      instance_create_id(obj->x,obj->y,obj->obj,obj->id);
    }
    createcode();
  }

  extern int room_loadtimecount;
  extern roomstruct grd_rooms[];
  extern int room_idmax;
  void rooms_load()
  {
    roomdata = new roomstruct*[room_idmax];
    roomorder = new roomstruct*[room_loadtimecount];
    for (int i = 0; i < room_idmax; i++)
      roomdata[i] = 0;
    for (int i = 0; i < room_loadtimecount; i++) {
      roomdata[grd_rooms[i].id] = &grd_rooms[i];
      roomorder[i] = &grd_rooms[i];
    }
  }
}


//Implement the "room" global before we continue

void enigma::roomv::function() {
  room_goto((int)rval.d);
} enigma::roomv room;

#if SHOWERRORS
  #define errcheck(indx,err) \
	if (indx < 0 or indx >= enigma::room_idmax or !enigma::roomdata[indx]) \
		return (show_error(err,0), 0)
  #define errcheck_o(indx,err) \
	if (indx < 0 or indx >= enigma::room_loadcount) \
		return (show_error(err,0), 0)
#else
  #define errcheck(indx,err)
  #define errcheck_o(indx,err)
#endif

int room_goto(int indx)
{
	errcheck(indx,"Attempting to go to nonexisting room");
	enigma::roomdata[indx]->gotome();
	return 0;
}

int room_restart()
{
	int indx=(int)room.rval.d;
	errcheck(indx,"Is this some kind of joke?");
	enigma::roomdata[indx]->gotome();
	return 0;
}

string room_get_name(int index)
{
	errcheck(indx,"Room index out of range");
	return enigma::roomdata[index]->name;
}

int room_goto_absolute(int index)
{
	errcheck_o(index,"Room index out of range");
	enigma::roomstruct *rit = enigma::roomorder[index];
	int indx = rit->id;

	enigma::roomdata[indx]->gotome();
	return 0;
}

#undef room_count
int room_count() {
  return enigma::room_loadtimecount;
}

int room_goto_first()
{
    enigma::roomstruct *rit = enigma::roomorder[0];
    errcheck_o(0,"Game must have at least one room to do anything");
    rit->gotome();
    return 0;
}

#include <stdio.h>
int room_goto_next()
{
    enigma::roomstruct *rit = enigma::roomdata[(int)room.rval.d];
    errcheck((int)room.rval.d,"Going to next room from invalid room. wat");

    rit = enigma::roomorder[rit->order + 1];
    errcheck(rit->order+1,"Going to next room after last");

    rit->gotome();
    return 0;
}

int room_goto_previous()
{
    enigma::roomstruct *rit = enigma::roomdata[(int)room.rval.d];
    errcheck((int)room.rval.d,"Going to next room from invalid room. wat");

    rit = enigma::roomorder[rit->order - 1];
    errcheck(rit->order-1,"Going to next room after last");

    rit->gotome();
    return 0;
}


int room_next(int num)
{
    if (num < 0 or num >= enigma::room_idmax)
      return -1;
    enigma::roomstruct *rit = enigma::roomdata[num];
    if (!rit or rit->order+1 >= enigma::room_loadtimecount)
      return -1;
    return enigma::roomorder[rit->order + 1]->id;
}
int room_previous(int num)
{
    if (num < 0 or num >= enigma::room_idmax)
      return -1;
    enigma::roomstruct *rit = enigma::roomdata[num];
    if (!rit or rit->order-1 < 0)
      return -1;
    return enigma::roomorder[rit->order - 1]->id;
}

#include "CallbackArrays.h"

namespace enigma
{
  void room_update()
  {
    window_set_caption(room_caption);
    if (view_enabled)
    {
      for (int i=0;i<8;i++)
      if (view_visible[i])
      {
        if (mouse_x >= view_xport[i] && mouse_x < view_xport[i]+view_wport[i]
        &&  mouse_y >= view_yport[i] && mouse_y < view_yport[i]+view_hport[i]) {
          mouse_x=view_xview[i]+((mouse_x-view_xport[i])/(double)view_wport[i])*view_wview[i];
          mouse_y=view_yview[i]+((mouse_y-view_yport[i])/(double)view_hport[i])*view_hview[i];
          break;
        }
      }
    }
  }
}
