/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008-2011 Josh Ventura                                        **
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

#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "libEGMstd.h"
#include "instance_system.h"
#include "instance.h"
#include "planar_object.h"
#include "backgroundstruct.h"

#include "roomsystem.h"
#include "depth_draw.h"

#include "CallbackArrays.h"

#include "lives.h"

int room_speed  = 60;
int room_width  = 640;
int room_height = 480;
int room_persistent = 0;
var room_caption = "";

int background_color = 16777215;
int background_showcolor=1;

var background_visible, background_foreground, background_index, background_x, background_y, background_htiled,
background_vtiled, background_hspeed, background_vspeed,background_alpha,background_coloring,background_width,background_height,background_xscale,background_yscale;

int view_current = 0;
int view_enabled = 0;
rvt view_hborder, view_hport, view_hspeed, view_hview, view_object, view_vborder,
    view_visible, view_vspeed, view_wport, view_wview, view_xport, view_xview, view_yport, view_yview,view_angle;

namespace enigma
{
  roomstruct** roomdata;
  roomstruct** roomorder;

  void roomstruct::gotome(bool gamestart)
  {
    //Destroy all objects
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
    {
      it->myevent_roomend();
      if (!((object_planar*)*it)->persistent)
      instance_destroy(it->id, false);
    }

    graphics_clean_up_roomend();

    // Set the index to self
    room.rval.d = id;

    room_caption = cap;
    room_width   = width;
    room_height  = height;
    room_speed   = spd;
    room_persistent = persistent;

    background_color = backcolor;
    background_showcolor = drawbackcolor;

    if (gamestart) {
      reset_lives();
    }

    //Backgrounds start
    for (int i=0;i<8;i++)
    {
      background_visible[i] = backs[i].visible;
      background_foreground[i] = backs[i].foreground;
      background_index[i] = backs[i].background;
      background_x[i] = backs[i].area_x; background_y[i] = backs[i].area_y;
      background_hspeed[i] = backs[i].horSpeed; background_vspeed[i] = backs[i].verSpeed;
      background_htiled[i] = backs[i].tileHor; background_vtiled[i] = backs[i].tileVert;
      background_alpha[i] = backs[i].alpha;
      background_coloring[i] = backs[i].color;
      if (background_exists(background_index[i]))
      {
        background_width[i] = background_get_width(background_index[i]); background_height[i] = background_get_height(background_index[i]);
        background_xscale[i] = (backs[i].stretch) ? room_width/background_width[i] : 1;
        background_yscale[i] = (backs[i].stretch) ? room_height/background_height[i] : 1;
      }
      else
      {
        background_width[i] = 0; background_height[i] = 0;
        background_xscale[i] = 1;
        background_yscale[i] = 1;
      }
    }
    //Backgrounds end

      //Tiles start
      drawing_depths.clear();
      for (int tilei=0; tilei<tilecount; tilei++) {
          tile t = tiles[tilei];
          drawing_depths[t.depth].tiles.push_back(tiles[tilei]);
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

    window_default();
    io_clear();
    screen_init();
    screen_refresh();

    object_basic* is[instancecount];
    for (int i = 0; i<instancecount; i++) {
      inst *obj = &instances[i];
      is[i] = instance_create_id(obj->x,obj->y,obj->obj,obj->id);
    }

    instance_event_iterator = new inst_iter(NULL,NULL,NULL);

    for (int i = 0; i<instancecount; i++)
      is[i]->myevent_create();
    if (gamestart)
    for (int i = 0; i<instancecount; i++)
      is[i]->myevent_gamestart();

    createcode();
  }

  extern int room_loadtimecount;
  extern roomstruct grd_rooms[];
  extern int room_idmax;
  int room_switching_id = -1;
  int room_switching_restartgame = false;
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
INTERCEPT_DEFAULT_COPY(enigma::roomv);
void enigma::roomv::function(variant oldval) {
  room_goto((int)rval.d);
  rval.d = oldval.rval.d;
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
	enigma::room_switching_id = indx;
	enigma::room_switching_restartgame = false;
	return 0;
}

int room_restart()
{
	int indx=(int)room.rval.d;
	errcheck(indx,"Is this some kind of joke?");
	enigma::room_switching_id = indx;
	enigma::room_switching_restartgame = false;
	return 0;
}

string room_get_name(int indx)
{
	errcheck(indx,"Room index out of range");
	return enigma::roomdata[indx]->name;
}

int room_goto_absolute(int indx)
{
	errcheck_o(indx,"Room index out of range");
	enigma::roomstruct *rit = enigma::roomorder[indx];
	int index = rit->id;

	enigma::room_switching_id = index;
	enigma::room_switching_restartgame = false;
	return 0;
}

#undef room_count
int room_count() {
  return enigma::room_loadtimecount;
}

int room_goto_first(bool restart_game)
{
    errcheck_o(0,"Game must have at least one room to do anything");
    enigma::roomstruct *rit = enigma::roomorder[0];
    enigma::room_switching_id = rit->id;
    enigma::room_switching_restartgame = restart_game;
    return 0;
}

#include <stdio.h>
int room_goto_next()
{
    enigma::roomstruct *rit = enigma::roomdata[(int)room.rval.d];
    errcheck((int)room.rval.d,"Going to next room from invalid room. wat");

    rit = enigma::roomorder[rit->order + 1];
    errcheck(rit->order+1,"Going to next room after last");

    enigma::room_switching_id = rit->id;
    enigma::room_switching_restartgame = false;
    return 0;
}

int room_goto_previous()
{
    enigma::roomstruct *rit = enigma::roomdata[(int)room.rval.d];
    errcheck((int)room.rval.d,"Going to next room from invalid room. wat");

    rit = enigma::roomorder[rit->order - 1];
    errcheck(rit->order-1,"Going to next room after last");

    enigma::room_switching_id = rit->id;
    enigma::room_switching_restartgame = false;
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

bool room_exists(int roomid)
{
    return roomid >= 0 and roomid < enigma::room_idmax and enigma::roomdata[roomid];
}

int room_set_width(int indx, int wid)
{
    errcheck(indx,"Nonexistent room");
    enigma::roomdata[indx]->width = wid;
}

int room_set_height(int indx, int hei)
{
    errcheck(indx,"Nonexistent room");
    enigma::roomdata[indx]->height = hei;
}

int room_set_background(int indx, int bind, bool vis, bool fore, bool back, double x, double y, bool htiled, bool vtiled, double hspeed, double vspeed, double alpha, int color)
{
    errcheck(indx,"Nonexistent room");
    enigma::backstruct bk = enigma::roomdata[indx]->backs[bind];
    bk.visible = vis;
    bk.foreground = fore;
    bk.background = back;
    bk.area_x = x;
    bk.area_y = y;
    bk.tileHor = htiled;
    bk.tileVert = vtiled;
    bk.horSpeed = hspeed;
    bk.verSpeed = vspeed;
    bk.alpha = alpha;
    bk.color = color;
}

int room_set_view(int indx, int vind, int vis, int xview, int yview, int wview, int hview, int xport, int yport, int wport, int hport, int hborder, int vborder, int hspeed, int vspeed, int obj)
{
    errcheck(indx,"Nonexistent room");
    enigma::viewstruct vw = enigma::roomdata[indx]->views[vind];
    vw.start_vis = vis;
    vw.area_x = xview;
    vw.area_y = yview;
    vw.area_w = wview;
    vw.area_h = hview;
    vw.port_x = xport;
    vw.port_y = yport;
    vw.port_w = wport;
    vw.port_h = hport;
    vw.hborder = hborder;
    vw.vborder = vborder;
    vw.hspd = hspeed;
    vw.vspd = vspeed;
    vw.object2follow = obj;
}

int room_set_background_color(int indx, int col, bool show)
{
    errcheck(indx,"Nonexistent room");
    enigma::roomdata[indx]->backcolor = col;
    enigma::roomdata[indx]->drawbackcolor = show;
}

int room_set_caption(int indx, string str)
{
    errcheck(indx,"Nonexistent room");
    enigma::roomdata[indx]->cap = str;
}

int room_set_persistent(int indx, bool pers)
{
    errcheck(indx,"Nonexistent room");
    enigma::roomdata[indx]->persistent = pers;
}

int room_set_view_enabled(int indx, int val)
{
    errcheck(indx,"Nonexistent room");
    enigma::roomdata[indx]->views_enabled = val;
}

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
  void rooms_switch()
  {
    if (room_exists(room_switching_id)) {
      int local_room_switching_id = room_switching_id;
      bool local_room_switching_restartgame = room_switching_restartgame;
      room_switching_id = -1;
      room_switching_restartgame = false;
    	enigma::roomdata[local_room_switching_id]->gotome(local_room_switching_restartgame);
    }
  }
  void game_start() {
    enigma::roomstruct *rit = *enigma::roomorder;
    enigma::roomdata[rit->id]->gotome(true);
  }
}
