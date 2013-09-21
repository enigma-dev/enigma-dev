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
#include "Universal_System/callbacks_events.h"
#include "libEGMstd.h"
#include "instance_system.h"
#include "instance.h"
#include "planar_object.h"
#include "backgroundstruct.h"

#include "roomsystem.h"
#include "depth_draw.h"

#include "CallbackArrays.h"

#include "lives.h"
#include <string.h>

namespace enigma_user
{

int room_speed  = 60;

int room_width  = 640;
int room_height = 480;

int room_persistent = 0;
var room_caption = "";
var current_caption = "";

int background_color = 0xFFFFFF;
int background_showcolor=1;

var background_visible, background_foreground, background_index, background_x, background_y, background_htiled,
background_vtiled, background_hspeed, background_vspeed,background_alpha,background_coloring,background_width,background_height,background_xscale,background_yscale;

int view_current = 0;
int view_enabled = 0;
rvt view_hborder, view_hport, view_hspeed, view_hview, view_object, view_vborder,
    view_visible, view_vspeed, view_wport, view_wview, view_xport, view_xview, view_yport, view_yview,view_angle;

}

namespace enigma
{
  roomstruct** roomdata;
  roomstruct** roomorder;

  void roomstruct::gotome(bool gamestart)
  {
    using namespace enigma_user;

    //Destroy all objects
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
    {
      it->myevent_roomend();
      if (!((object_planar*)*it)->persistent)
      instance_destroy(it->id, false);
    }
    perform_callbacks_clean_up_roomend();

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
      if (enigma_user::background_exists(background_index[i]))
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

    view_enabled = views_enabled;

    for (int i=0;i<8;i++)
    {
      view_xview[i] = views[i].area_x; view_yview[i] = views[i].area_y; view_wview[i] = views[i].area_w; view_hview[i] = views[i].area_h;
      view_xport[i] = views[i].port_x; view_yport[i] = views[i].port_y; view_wport[i] = views[i].port_w; view_hport[i] = views[i].port_h;
      view_object[i] = views[i].object2follow;
      view_hborder[i] = views[i].hborder; view_vborder[i] = views[i].vborder; view_hspeed[i] = views[i].hspd; view_vspeed[i] = views[i].vspd;
      view_visible[i] = (bool)views[i].start_vis;
    }

    enigma_user::window_default();
    enigma_user::io_clear();
    screen_init();
    screen_refresh();

    //Load tiles
    delete_tiles();
    drawing_depths.clear();
    for (int i = 0; i < tilecount; i++)
    {
        tile t = tiles[i];
        drawing_depths[t.depth].tiles.push_back(t);
    }
    load_tiles();
    //Tiles end

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

    if (createcode)
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
    for (int i = 0; i < room_loadtimecount; i++) {
      roomdata[grd_rooms[i].id] = &grd_rooms[i];
      roomorder[i] = &grd_rooms[i];
    }
  }
}


//Implement the "room" global before we continue
INTERCEPT_DEFAULT_COPY(enigma::roomv)
void enigma::roomv::function(variant oldval) {
  enigma_user::room_goto((int)rval.d);
  rval.d = oldval.rval.d;
}
namespace enigma_user {
  enigma::roomv room;
}

#if DEBUG_MODE || (defined(SHOW_ERRORS) && SHOW_ERRORS)
  #define errcheck(indx,err,v) \
	if (unsigned(indx) >= unsigned(enigma::room_idmax) or !enigma::roomdata[indx]) \
		return (show_error(err,0), (v))
  #define errcheck_o(indx,err) \
	if (unsigned(indx) >= unsigned(enigma::room_loadtimecount)) \
		return (show_error(err,0), 0)
#else
  #define errcheck(indx,err,v)
  #define errcheck_o(indx,err)
#endif

namespace enigma_user
{

int room_goto(int indx)
{
	errcheck(indx,"Attempting to go to nonexisting room", 0);
	enigma::room_switching_id = indx;
	enigma::room_switching_restartgame = false;
	return 1;
}

int room_restart()
{
	int indx=(int)room.rval.d;
	errcheck(indx, "Is this some kind of joke?", 0);
	enigma::room_switching_id = indx;
	enigma::room_switching_restartgame = false;
	return 1;
}

string room_get_name(int indx)
{
	errcheck(indx,"Room index out of range", "");
	return enigma::roomdata[indx]->name;
}

int room_goto_absolute(int indx)
{
	errcheck_o(indx,"Room index out of range");
	enigma::roomstruct *rit = enigma::roomorder[indx];
	int index = rit->id;

	enigma::room_switching_id = index;
	enigma::room_switching_restartgame = false;
	return 1;
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
    return 1;
}

int room_goto_next()
{
    enigma::roomstruct *rit = enigma::roomdata[(int)room.rval.d];
    errcheck((int)room.rval.d,"Going to next room from invalid room. wat", 0);

    rit = enigma::roomorder[rit->order + 1];
    errcheck(rit->order+1,"Going to next room after last", 0);

    enigma::room_switching_id = rit->id;
    enigma::room_switching_restartgame = false;
    return 1;
}

int room_goto_previous()
{
    enigma::roomstruct *rit = enigma::roomdata[(int)room.rval.d];
    errcheck((int)room.rval.d,"Going to next room from invalid room. wat", 0);

    rit = enigma::roomorder[rit->order - 1];
    errcheck(rit->order-1,"Going to next room after last", 0);

    enigma::room_switching_id = rit->id;
    enigma::room_switching_restartgame = false;
    return 1;
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
    errcheck(indx,"Nonexistent room", 0);
    enigma::roomdata[indx]->width = wid;
    return 1;
}

int room_set_height(int indx, int hei)
{
    errcheck(indx,"Nonexistent room", 0);
    enigma::roomdata[indx]->height = hei;
    return 1;
}

int room_set_background(int indx, int bind, bool vis, bool fore, bool back, double x, double y, bool htiled, bool vtiled, double hspeed, double vspeed, double alpha, int color)
{
    errcheck(indx,"Nonexistent room", 0);
    enigma::backstruct &bk = enigma::roomdata[indx]->backs[bind];
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
    return 1;
}

int room_set_view(int indx, int vind, int vis, int xview, int yview, int wview, int hview, int xport, int yport, int wport, int hport, int hborder, int vborder, int hspeed, int vspeed, int obj)
{
    errcheck(indx,"Nonexistent room", 0);
    enigma::viewstruct &vw = enigma::roomdata[indx]->views[vind];
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
    return 1;
}

int room_set_background_color(int indx, int col, bool show)
{
    errcheck(indx,"Nonexistent room", 0);
    enigma::roomdata[indx]->backcolor = col;
    enigma::roomdata[indx]->drawbackcolor = show;
    return 1;
}

int room_set_caption(int indx, string str)
{
    errcheck(indx,"Nonexistent room", 0);
    enigma::roomdata[indx]->cap = str;
    return 1;
}

int room_set_persistent(int indx, bool pers)
{
    errcheck(indx,"Nonexistent room", 0);
    enigma::roomdata[indx]->persistent = pers;
    return 1;
}

int room_set_view_enabled(int indx, int val)
{
    errcheck(indx,"Nonexistent room", 0);
    enigma::roomdata[indx]->views_enabled = val;
    return 1;
}

}

namespace enigma
{
    bool tile_alter = false;
    bool instance_alter = false;
}

namespace enigma_user
{

int room_tile_add_ext(int indx, int bck, int left, int top, int width, int height, int x, int y, int depth, int xscale, int yscale, double alpha, int color)
{
    errcheck(indx,"Nonexistent room", 0);
    enigma::roomstruct *rm = enigma::roomdata[indx];
    const int tcount = rm->tilecount++;
    enigma::tile *ti = rm->tiles;
    enigma::tile *newtiles = new enigma::tile[tcount + 1];
    for (int tilei = 0; tilei < tcount; tilei++)
        newtiles[tilei] = ti[tilei];

    newtiles[tcount].id = enigma::maxtileid++;
    newtiles[tcount].bckid = bck;
    newtiles[tcount].bgx = left;
    newtiles[tcount].bgy = top;
    newtiles[tcount].depth = depth;
    newtiles[tcount].height = height;
    newtiles[tcount].width = width;
    newtiles[tcount].roomX = x;
    newtiles[tcount].roomY = y;
    newtiles[tcount].xscale = xscale;
    newtiles[tcount].yscale = yscale;
    newtiles[tcount].alpha = alpha;
    newtiles[tcount].color = color;

    if (enigma::tile_alter) delete[] rm->tiles;
    rm->tiles = newtiles;
    enigma::tile_alter = true;
    return 1;
}

int room_tile_clear(int indx)
{
    errcheck(indx,"Nonexistent room", 0);
    enigma::roomstruct *rm = enigma::roomdata[indx];
    enigma::tile *newtiles = new enigma::tile[1];
    rm->tilecount = 0;

    if (enigma::tile_alter) delete[] rm->tiles;
    rm->tiles = newtiles;
    enigma::tile_alter = true;
    return 1;
}

int room_instance_add(int indx, int x, int y, int obj)
{
    errcheck(indx,"Nonexistent room", 0);
    enigma::roomstruct *rm = enigma::roomdata[indx];
    const int icount = rm->instancecount++;
    enigma::inst *in = rm->instances;
    enigma::inst *newinst = new enigma::inst[icount + 1];
    for (int insti = 0; insti < icount; insti++)
        newinst[insti] = in[insti];

    newinst[icount].id = enigma::maxid++;
    newinst[icount].x = x;
    newinst[icount].y = y;
    newinst[icount].obj = obj;

    if (enigma::instance_alter) delete[] rm->instances;
    rm->instances = newinst;
    enigma::instance_alter = true;
    return 1;
}

int room_instance_clear(int indx)
{
    errcheck(indx,"Nonexistent room", 0);
    enigma::roomstruct *rm = enigma::roomdata[indx];
    enigma::inst *newinst = new enigma::inst[1];
    rm->instancecount = 0;

    if (enigma::instance_alter) delete[] rm->instances;
    rm->instances = newinst;
    enigma::instance_alter = true;
    return 1;
}

int room_add()
{
    int newrm = enigma::room_idmax++;
    enigma::roomstruct** newroomdata;

    newroomdata = new enigma::roomstruct*[newrm + 1];
    for (int i = 0; i < newrm; i++)
        newroomdata[i] = enigma::roomdata[i];

    enigma::roomstruct *rm = newroomdata[newrm] = new enigma::roomstruct;

    rm->id = newrm;
    rm->order = -1;
    rm->name = "New Room";
    rm->cap = "";
    rm->backcolor = 0xFFFFFF;
    rm->drawbackcolor = true;
    rm->width = 640;
    rm->height = 480;
    rm->spd = 60;
    rm->persistent = false;
    rm->views_enabled = false;
    rm->createcode = NULL;

    enigma::inst *newinst = new enigma::inst[1];
    rm->instances = newinst;
    rm->instancecount = 0;

    enigma::tile *newtiles = new enigma::tile[1];
    rm->tiles = newtiles;
    rm->tilecount = 0;

    enigma::viewstruct vw;
    for (int i = 0; i < 8; i++)
    {
        vw = rm->views[i];
        vw.start_vis = false;
        vw.area_x = 0;
        vw.area_y = 0;
        vw.area_w = 640;
        vw.area_h = 480;
        vw.port_x = 0;
        vw.port_y = 0;
        vw.port_w = 640;
        vw.port_h = 480;
        vw.hborder = 32;
        vw.vborder = 32;
        vw.hspd = -1;
        vw.vspd = -1;
        vw.object2follow = -4;
    }

    enigma::backstruct bk;
    for (int i = 0; i < 8; i++)
    {
        bk = rm->backs[i];
        bk.visible = 0;
        bk.foreground = 0;
        bk.background = 0;
        bk.area_x = 0;
        bk.area_y = 0;
        bk.tileHor = true;
        bk.tileVert = true;
        bk.horSpeed = 0;
        bk.verSpeed = 0;
        bk.alpha = 1;
        bk.color = 0xFFFFFF;
    }

    delete[] enigma::roomdata;
    enigma::roomdata = newroomdata;
    return newrm;
}

int room_duplicate(int indx, bool ass, int assroom)
{
    errcheck(indx,"Nonexistent room", 0);
    if (ass) {
        errcheck(assroom,"Nonexistent room", 0);
    }
    int newrm = (ass)?enigma::room_idmax++ : enigma::room_idmax - 1;

    enigma::roomstruct** newroomdata;
    newroomdata = new enigma::roomstruct*[newrm + 1];
    for (int i = 0; i < newrm; i++)
        newroomdata[i] = enigma::roomdata[i];

    enigma::roomstruct *rm = (ass)?newroomdata[indx] : newroomdata[newrm] = new enigma::roomstruct;
    enigma::roomstruct *copyrm = (ass)?enigma::roomdata[assroom]:enigma::roomdata[indx];

    rm->id = copyrm->id;
    rm->order = (ass)?copyrm->order:-1;
    rm->name = "New Room";
    rm->cap = copyrm->cap;
    rm->backcolor = copyrm->backcolor;
    rm->drawbackcolor = copyrm->drawbackcolor;
    rm->width = copyrm->width;
    rm->height = copyrm->height;
    rm->spd = copyrm->spd;
    rm->persistent = copyrm->persistent;
    rm->views_enabled = copyrm->views_enabled;
    rm->createcode = copyrm->createcode;

    enigma::inst *newinst = new enigma::inst[copyrm->instancecount];
    rm->instances = newinst;
    rm->instancecount = copyrm->instancecount;
    for (int i = 0; i < rm->instancecount; i++)
        rm->instances[i] = copyrm->instances[i];

    enigma::tile *newtiles = new enigma::tile[copyrm->tilecount];
    rm->tiles = newtiles;
    rm->tilecount = copyrm->tilecount;
    for (int i = 0; i < rm->tilecount; i++)
        rm->tiles[i] = copyrm->tiles[i];

    enigma::viewstruct vw, vc;
    for (int i = 0; i < 8; i++)
    {
        vw = rm->views[i];
        vc = copyrm->views[i];
        vw.start_vis = vc.start_vis;
        vw.area_x = vw.area_x;
        vw.area_y = vc.area_y;
        vw.area_w = vc.area_w;
        vw.area_h = vc.area_h;
        vw.port_x = vc.port_x;
        vw.port_y = vc.port_y;
        vw.port_w = vc.port_w;
        vw.port_h = vc.port_h;
        vw.hborder = vc.hborder;
        vw.vborder = vc.vborder;
        vw.hspd = vc.hspd;
        vw.vspd = vc.vspd;
        vw.object2follow = vc.object2follow;
    }

    enigma::backstruct bk, bc;
    for (int i = 0; i < 8; i++)
    {
        bk = rm->backs[i];
        bc = copyrm->backs[i];
        bk.visible = bc.visible;
        bk.foreground = bc.foreground;
        bk.background = bc.background;
        bk.area_x = bc.area_x;
        bk.area_y = bc.area_y;
        bk.tileHor = bc.tileHor;
        bk.tileVert = bc.tileVert;
        bk.horSpeed = bc.horSpeed;
        bk.verSpeed = bc.verSpeed;
        bk.alpha = bc.alpha;
        bk.color = bc.color;
    }

    delete[] enigma::roomdata;
    enigma::roomdata = newroomdata;
    return ((ass)?indx:newrm);
}

int view_set(int vind, int vis, int xview, int yview, int wview, int hview, int xport, int yport, int wport, int hport, int hborder, int vborder, int hspeed, int vspeed, int obj)
{
    view_visible[vind] = vis;
    view_xview[vind] = xview;
    view_yport[vind] = yview;
    view_wport[vind] = wview;
    view_hview[vind] = hview;
    view_xport[vind] = xport;
    view_yport[vind] = yport;
    view_wport[vind] = wport;
    view_hport[vind] = hport;
    view_hborder[vind] = hborder;
    view_vborder[vind] = vborder;
    view_hspeed[vind] = hspeed;
    view_vspeed[vind] = vspeed;
    view_object[vind] = obj;
    return 1;
}

}

namespace enigma
{
  double mouse_xprevious, mouse_yprevious;
  void update_mouse_variables()
  {
    using namespace enigma_user;
    mouse_xprevious = mouse_x;
    mouse_yprevious = mouse_y;
    mouse_x = window_views_mouse_get_x();
    mouse_y = window_views_mouse_get_y();

    if (view_enabled)
      for (int i=0; i<8; i++)
        if (view_visible[i])
        {
          if (mouse_x >= view_xport[i] && mouse_x < view_xport[i]+view_wport[i] &&  mouse_y >= view_yport[i] && mouse_y < view_yport[i]+view_hport[i])
          {
            mouse_x = view_xview[i]+((mouse_x-view_xport[i])/(double)view_wport[i])*view_wview[i];
            mouse_y = view_yview[i]+((mouse_y-view_yport[i])/(double)view_hport[i])*view_hview[i];
            break;
          }
        }
  }
  void rooms_switch()
  {
    if (enigma_user::room_exists(room_switching_id)) {
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
