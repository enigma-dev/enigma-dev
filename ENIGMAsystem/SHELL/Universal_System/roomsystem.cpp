/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2014 Seth N. Hetu
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <map>
#include <math.h>
#include <string>

#include "var4.h"
#include "reflexive_types.h"

#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Universal_System/Instances/callbacks_events.h"
#include "libEGMstd.h"
#include "Instances/instance_system.h"
#include "Instances/instance.h"
#include "Object_Tiers/planar_object.h"
#include "Resources/background.h"

#include "roomsystem.h"
#include "depth_draw.h"

#include "Platforms/General/PFmain.h"

#include "lives.h"
#include <string.h>

namespace enigma_user
{

int room_speed  = 60;

int room_width  = 640;
int room_height = 480;

int room_first = 0;
int room_last = 0;

int room_persistent = 0;
var room_caption = "";

int background_color = 0xFFFFFF;
int background_showcolor=1;

var background_visible, background_foreground, background_index, background_x, background_y, background_htiled,
    background_vtiled, background_hspeed, background_vspeed, background_alpha, background_coloring, background_width,
    background_height, background_xscale, background_yscale;

int view_current = 0;
int view_enabled = 0;
rvt view_hborder, view_hport, view_hspeed, view_hview, view_object, view_vborder,
    view_visible, view_vspeed, view_wport, view_wview, view_xport, view_xview,
    view_yport, view_yview, view_angle;

}

namespace enigma
{

  roomstruct** roomdata;
  roomstruct** roomorder;

  void follow_object(int vob, size_t vc)
  {
    using namespace enigma_user;
    object_basic *instanceexists = fetch_instance_by_int(vob);

    if (instanceexists)
    {
      object_planar* vobr = (object_planar*)instanceexists;

      double vobx = vobr->x, voby = vobr->y;

      //int bbl=*vobr.x+*vobr.bbox_left,bbr=*vobr.x+*vobr.bbox_right,bbt=*vobr.y+*vobr.bbox_top,bbb=*vobr.y+*vobr.bbox_bottom;
      //if (bbl<view_xview[vc]+view_hbor[vc]) view_xview[vc]=bbl-view_hbor[vc];

      double vbc_h, vbc_v;
      (view_hborder[vc] > view_wview[vc]/2) ? vbc_h = view_wview[vc]/2 : vbc_h = view_hborder[vc];
      (view_vborder[vc] > view_hview[vc]/2) ? vbc_v = view_hview[vc]/2 : vbc_v = view_vborder[vc];

      if (view_hspeed[vc] == -1)
      {
        if (vobx < view_xview[vc] + vbc_h)
          view_xview[vc] = vobx - vbc_h;
        else if (vobx > view_xview[vc] + view_wview[vc] - vbc_h)
          view_xview[vc] = vobx + vbc_h - view_wview[vc];
      }
      else
      {
        if (vobx < view_xview[vc] + vbc_h)
        {
          view_xview[vc] -= view_hspeed[vc];
          if (view_xview[vc] < vobx - vbc_h)
            view_xview[vc] = vobx - vbc_h;
        }
        else if (vobx > view_xview[vc] + view_wview[vc] - vbc_h)
        {
          view_xview[vc] += view_hspeed[vc];
          if (view_xview[vc] > vobx + vbc_h - view_wview[vc])
            view_xview[vc] = vobx + vbc_h - view_wview[vc];
        }
      }

      if (view_vspeed[vc] == -1)
      {
        if (voby < view_yview[vc] + vbc_v)
          view_yview[vc] = voby - vbc_v;
        else if (voby > view_yview[vc] + view_hview[vc] - vbc_v)
          view_yview[vc] = voby + vbc_v - view_hview[vc];
      }
      else
      {
        if (voby < view_yview[vc] + vbc_v)
        {
          view_yview[vc] -= view_vspeed[vc];
          if (view_yview[vc] < voby - vbc_v)
            view_yview[vc] = voby - vbc_v;
        }
        if (voby > view_yview[vc] + view_hview[vc] - vbc_v)
        {
          view_yview[vc] += view_vspeed[vc];
          if (view_yview[vc] > voby + vbc_v - view_hview[vc])
            view_yview[vc] = voby + vbc_v - view_hview[vc];
        }
      }

      if (view_xview[vc] < 0)
        view_xview[vc] = 0;
      else if (view_xview[vc] > room_width - view_wview[vc])
        view_xview[vc] = room_width - view_wview[vc];

      if (view_yview[vc] < 0)
        view_yview[vc] = 0;
      else if (view_yview[vc] > room_height - view_hview[vc])
        view_yview[vc] = room_height - view_hview[vc];
    }
  }


  void roomstruct::end() {
    // Fire the Room End event.
    instance_event_iterator = &dummy_event_iterator;
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it) {
      it->myevent_roomend();
    }
    // This needs to be a separate loop because the room end event for some object may
    // access another instance.
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it) {
      // Destroy the object if it is not persistent
      if (!((object_planar*)*it)->persistent)
        enigma_user::instance_destroy(it->id, false);
    }
  }

  void roomstruct::gotome(bool gamestart)
  {
    using namespace enigma_user;

    this->end();

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

    //We may still be holding on to deactivated instances; they can interact badly with existing instances in certain cases.
    instance_deactivated_list.clear();

    // Initialize background variants so they do not throw uninitialized variable access errors.
    for (unsigned i=0;i<8;i++) {
      background_visible[i] = backs[i].visible;
      background_foreground[i] = backs[i].foreground;
      background_index[i] = backs[i].background;
      background_x[i] = backs[i].area_x; background_y[i] = backs[i].area_y;
      background_hspeed[i] = backs[i].horSpeed; background_vspeed[i] = backs[i].verSpeed;
      background_htiled[i] = backs[i].tileHor; background_vtiled[i] = backs[i].tileVert;
      background_alpha[i] = backs[i].alpha;
      background_coloring[i] = backs[i].color;
      if (enigma_user::background_exists(background_index[i])) {
        background_width[i] = background_get_width(background_index[i]); background_height[i] = background_get_height(background_index[i]);
        background_xscale[i] = (backs[i].stretch) ? room_width/background_width[i] : 1;
        background_yscale[i] = (backs[i].stretch) ? room_height/background_height[i] : 1;
      }
      else {
        background_width[i] = 0; background_height[i] = 0;
        background_xscale[i] = 1;
        background_yscale[i] = 1;
      }
    }
    //Backgrounds end

    view_enabled = views_enabled;

    // Initialize view variants so they do not throw uninitialized variable access errors.
    for (unsigned i=0;i<8;i++) {
      view_xview[i] = views[i].area_x; view_yview[i] = views[i].area_y; view_wview[i] = views[i].area_w; view_hview[i] = views[i].area_h;
      view_xport[i] = views[i].port_x; view_yport[i] = views[i].port_y; view_wport[i] = views[i].port_w; view_hport[i] = views[i].port_h;
      view_object[i] = views[i].object2follow;
      view_hborder[i] = views[i].hborder; view_vborder[i] = views[i].vborder; view_hspeed[i] = views[i].hspd; view_vspeed[i] = views[i].vspd;
      view_visible[i] = (bool)views[i].start_vis;
      view_angle[i] = 0;
    }
    
    // In pull request 1831, it was decided to adopt GMS behavior instead of GM8.
    // The window is no longer moved, centered, or resized when switching rooms.
    // This is always true, even if the room sizes are different.
    enigma_user::window_default(false);
    // window sized by first room, can make visible now
    enigma_user::window_set_visible(true);
    enigma_user::io_clear();
    // we only initialize the screen and clear the window color during game start
    // NOTE: no version of GM has EVER reset the drawing color or alpha during room transition
    if (gamestart) {
      enigma_user::screen_init();
      enigma_user::screen_refresh();
    }

    //Load tiles
    delete_tiles();
    for (enigma::diter dit = drawing_depths.rbegin(); dit != drawing_depths.rend(); dit++){
      if (dit->second.tiles.size()){
        dit->second.tiles.clear();
      }
    }
    for (size_t i = 0; i < tiles.size(); i++) {
      tile t = tiles[i];
      drawing_depths[t.depth].tiles.push_back(t);
    }
    load_tiles();
    //Tiles end

    std::vector<object_basic*> created;
    created.reserve(instances.size());
    for (const inst &obj : instances)
      if (!enigma::fetch_instance_by_id(obj.id))
        created.emplace_back(instance_create_id(obj.x,obj.y,obj.obj,obj.id));

    instance_event_iterator = &dummy_event_iterator;

    // Fire the rooms preCreation code. This code includes instance sprite transformations added in the room editor.
    // (NOTE: This code uses instance_deactivated_list to look up instances by ID, in addition to the normal lookup approach).
    if (precreatecode) {
      precreatecode();
    }

    // Fire the create event of all the new instances.
    for (object_basic *i : created)
      i->myevent_create();

    // Fire the game start event for all the new instances, persistent objects don't matter since this is the first time
    // the game ran they won't even exist yet
    if (gamestart) {
      for (object_basic *i : created)
          i->myevent_gamestart();
    }

    // Fire the rooms creation code. This includes instance creation code.
    // (NOTE: This code uses instance_deactivated_list to look up instances by ID, in addition to the normal lookup approach).
    if (createcode) {
      createcode();
    }

    // Fire the room start event for all persistent objects still kept alive and all the new instances
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it) {
      it->myevent_roomstart();
    }
  }

  extern int room_loadtimecount;
  extern roomstruct grd_rooms[];
  extern size_t room_idmax;
  int room_switching_id = -1;
  int room_switching_restartgame = false;
  void rooms_load()
  {
    roomdata = new roomstruct*[room_idmax];
    roomorder = new roomstruct*[room_loadtimecount];
    if (room_loadtimecount > 0) {
      for (int i = 0; i < room_loadtimecount; i++) {
        roomdata[grd_rooms[i].id] = &grd_rooms[i];
        roomorder[i] = &grd_rooms[i];
      }
      enigma_user::room_first = roomorder[0]->id;
      enigma_user::room_last = roomorder[room_loadtimecount-1]->id;
    }
  }
}


//Implement the "room" global before we continue
void enigma::roomv::function(const variant &oldval) {
  enigma_user::room_goto((int)rval.d);
  rval.d = oldval.rval.d;
}
namespace enigma_user {
  enigma::roomv room;
}

#if DEBUG_MODE
#define errcheck(indx,err,v) \
  if (unsigned(indx) >= unsigned(enigma::room_idmax) or !enigma::roomdata[indx]) \
    return (DEBUG_MESSAGE(err, MESSAGE_TYPE::M_USER_ERROR), (v))
  #define errcheck_o(indx,err) \
  if (unsigned(indx) >= unsigned(enigma::room_loadtimecount)) \
    return (DEBUG_MESSAGE(err,MESSAGE_TYPE::M_USER_ERROR), 0)
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
  if (unsigned(indx) >= unsigned(enigma::room_idmax) or !enigma::roomdata[indx]){
    return "<undefined>";
  } else {
    return enigma::roomdata[indx]->name;
  }
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
  errcheck(rit->order-1,"Going to next room before first", 0);

  enigma::room_switching_id = rit->id;
  enigma::room_switching_restartgame = false;
  return 1;
}


int room_next(int num)
{
  if (num < 0 or size_t(num) >= enigma::room_idmax)
    return -1;
  enigma::roomstruct *rit = enigma::roomdata[num];
  if (!rit or rit->order+1 >= enigma::room_loadtimecount)
    return -1;
  return enigma::roomorder[rit->order + 1]->id;
}
int room_previous(int num)
{
  if (num < 0 or size_t(num) >= enigma::room_idmax)
    return -1;
  enigma::roomstruct *rit = enigma::roomdata[num];
  if (!rit or rit->order-1 < 0)
    return -1;
  return enigma::roomorder[rit->order - 1]->id;
}

bool room_exists(int roomid)
{
  return roomid >= 0 and size_t(roomid) < enigma::room_idmax and enigma::roomdata[roomid];
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

namespace enigma_user
{

int room_tile_add_ext(int indx, int bck, int left, int top, int width, int height, int x, int y, int depth, double xscale, double yscale, double alpha, int color)
{
  errcheck(indx,"Nonexistent room", 0);
  enigma::roomstruct *rm = enigma::roomdata[indx];

  rm->tiles.emplace_back(
    enigma::maxtileid++,
    bck,
    left,
    top,
    depth,
    height,
    width,
    x,
    y,
    alpha,
    xscale,
    yscale,
    color
  );

  return 1;
}

int room_tile_clear(int indx)
{
  errcheck(indx,"Nonexistent room", 0);
  enigma::roomstruct *rm = enigma::roomdata[indx];

  rm->tiles.clear();

  return 1;
}

int room_instance_add(int indx, int x, int y, int obj)
{
  errcheck(indx,"Nonexistent room", 0);
  enigma::roomstruct *rm = enigma::roomdata[indx];
  rm->instances.emplace_back(
    enigma::maxid++,
    obj,
    x,
    y
  );
  return 1;
}

int room_instance_clear(int indx)
{
  errcheck(indx,"Nonexistent room", 0);
  enigma::roomstruct *rm = enigma::roomdata[indx];
  rm->instances.clear();
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
  rm->precreatecode = NULL;

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
  rm->precreatecode = copyrm->precreatecode;
  rm->instances = copyrm->instances;
  rm->tiles = copyrm->tiles;

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

int window_view_mouse_get_x(int id)
{
  return window_mouse_get_x()+view_xview[id];
}

int window_view_mouse_get_y(int id)
{
  return window_mouse_get_y()+view_yview[id];
}

void window_view_mouse_set(int id, int x, int y)
{
  window_mouse_set(window_get_x() + x + view_xview[id],window_get_y() + y + view_yview[id]);
}

//NOTE: GM8.1 allowed the mouse to go outside the window, for basically all mouse functions and constants, Studio however
//now wraps the mouse not allowing it to go out of bounds, so it will never report a negative mouse position for constants or functions.
//On top of this, it not only appears that they have wrapped it, but it appears that they in fact stop updating the mouse altogether in Studio
//because moving the mouse outside the window will sometimes freeze at a positive number, so it appears to be a bug in their window manager.
//ENIGMA's behaviour is a modified version of GM8.1, it uses the current view to obtain these positions so that it will work correctly
//for overlapped views while being backwards compatible.
int window_views_mouse_get_x() {
  gs_scalar sx;
  sx = (window_get_width() - window_get_region_width_scaled()) / 2;
  int x = (window_mouse_get_x() - sx) * ((gs_scalar)window_get_region_width() / (gs_scalar)window_get_region_width_scaled());
  if (view_enabled) {
    x = view_xview[view_current]+((x-view_xport[view_current])/(double)view_wport[view_current])*view_wview[view_current];
  }
  return x;
/* This code replicates GM8.1's broken mouse_x/y
  int x = window_mouse_get_x();
  int y = window_mouse_get_y();
  if (view_enabled) {
    for (int i = 0; i < 8; i++) {
      if (view_visible[i]) {
        if (x >= view_xport[i] && x < view_xport[i]+view_wport[i] &&  y >= view_yport[i] && y < view_yport[i]+view_hport[i]) {
          return view_xview[i]+((x-view_xport[i])/(double)view_wport[i])*view_wview[i];
        }
      }
    }
  }
  return x;
*/
}

int window_views_mouse_get_y() {
  gs_scalar sy;
  sy = (window_get_height() - window_get_region_height_scaled()) / 2;
  int y = (window_mouse_get_y() - sy) * ((gs_scalar)window_get_region_height() / (gs_scalar)window_get_region_height_scaled());
  if (view_enabled) {
    y = view_yview[view_current]+((y-view_yport[view_current])/(double)view_hport[view_current])*view_hview[view_current];
  }
  return y;
/*
  int x = window_mouse_get_x();
  int y = window_mouse_get_y();
  if (view_enabled) {
    for (int i = 0; i < 8; i++) {
      if (view_visible[i]) {
        if (x >= view_xport[i] && x < view_xport[i]+view_wport[i] &&  y >= view_yport[i] && y < view_yport[i]+view_hport[i]) {
          return view_yview[i]+((y-view_yport[i])/(double)view_hport[i])*view_hview[i];
        }
      }
    }
  }
  return y;
*/
}

void window_views_mouse_set(int x, int y) {
  if (view_enabled) {
    for (int i = 0; i < 8; i++) {
      if (view_visible[i]) {
        window_view_mouse_set(i, x, y);
        return;
      }
    }
  }
  window_mouse_set(x, y);
  return;
}

void window_update_mouse() {
  enigma::update_mouse_variables();
}

void window_update() {
  enigma::ENIGMA_events();
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

    gs_scalar sx, sy;
    sx = (window_get_width() - window_get_region_width_scaled()) / 2;
    sy = (window_get_height() - window_get_region_height_scaled()) / 2;

    // scale the mouse positions to the area on screen where we put the scaled viewports for the scaling options
    mouse_x = (window_mouse_get_x() - sx) * ((gs_scalar)window_get_region_width() / (gs_scalar)window_get_region_width_scaled());
    mouse_y = (window_mouse_get_y() - sy) * ((gs_scalar)window_get_region_height() / (gs_scalar)window_get_region_height_scaled());

    if (view_enabled) {
      for (int i = 0; i < 8; i++) {
        if (view_visible[i]) {
          if (mouse_x >= view_xport[i] && mouse_x < view_xport[i]+view_wport[i] &&  mouse_y >= view_yport[i] && mouse_y < view_yport[i]+view_hport[i]) {
            mouse_x = view_xview[view_current]+((mouse_x-view_xport[view_current])/(double)view_wport[view_current])*view_wview[i];
            mouse_y = view_yview[view_current]+((mouse_y-view_yport[view_current])/(double)view_hport[view_current])*view_hview[i];
            return;
          }
        }
      }
      mouse_x = view_xview[view_current]+((mouse_x-view_xport[view_current])/(double)view_wport[view_current])*view_wview[view_current];
      mouse_y = view_yview[view_current]+((mouse_y-view_yport[view_current])/(double)view_hport[view_current])*view_hview[view_current];
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
