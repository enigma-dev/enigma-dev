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

#ifndef room_system_h
#define room_system_h

#include "var4.h"

#include <string>
#include <vector>

namespace enigma
{

void follow_object(int vob, size_t vc);

}

namespace enigma_user
{

int room_goto(int roomind);
int room_restart();
std::string room_get_name(int index);
int room_goto_absolute(int index);
int room_goto_first(bool restart_game=false);
int room_goto_previous();
int room_goto_next();
int room_next(int num);
int room_previous(int num);
bool room_exists(int roomid);
int room_set_width(int indx, int wid);
int room_set_height(int indx, int hei);
int room_set_background(int indx, int bind, bool vis, bool fore, bool back, double x, double y, bool htiled, bool vtiled, double hspeed, double vspeed, double alpha = 1, int color = 0xFFFFFF);
int room_set_view(int indx, int vind, int vis, int xview, int yview, int wview, int hview, int xport, int yport, int wport, int hport, int hborder, int vborder, int hspeed, int vspeed, int obj);
int room_set_background_color(int indx, int col, bool show);
int room_set_caption(int indx, std::string str);
int room_set_persistent(int indx, bool pers);
int room_set_view_enabled(int indx, int val);
int room_tile_add_ext(int indx, int bck, int left, int top, int width, int height, int x, int y, int depth, double xscale, double yscale, double alpha, int color = 0xFFFFFF);
inline int room_tile_add(int indx, int bck, int left, int top, int width, int height, int x, int y, int depth)
{
    return room_tile_add_ext(indx, bck, left, top, width, height, x, y, depth, 1, 1, 1, 0xFFFFFF);
}
int room_tile_clear(int indx);
int room_instance_add(int indx, int x, int y, int obj);
int room_instance_clear(int indx);
int room_add();
int room_duplicate(int indx, bool ass = false, int assroom = -1);
inline int room_assign(int indx, int roomindx)
{
    return room_duplicate(indx, true, roomindx);
}

int view_set(int vind, int vis, int xview, int yview, int wview, int hview, int xport, int yport, int wport, int hport, int hborder, int vborder, int hspeed, int vspeed, int obj);
int window_views_mouse_get_x(); // same as mouse_x variable, with respect to the view the mouse intersects
int window_views_mouse_get_y(); // same as mouse_y variable, with respect to the view the mouse intersects
void window_views_mouse_set(int x, int y); // with respect to first visible view
void window_update_mouse();

void window_update();

extern int background_color;
extern int background_showcolor;

extern var background_visible, background_foreground, background_index, background_x, background_y, background_htiled,
background_vtiled, background_hspeed, background_vspeed,background_alpha,background_coloring,background_width,background_height,background_xscale,background_yscale;

extern int room_height;
extern int room_persistent;
extern int room_speed;
extern int room_width;
extern int room_first;
extern int room_last;

extern var room_caption;

int room_count();
#define room_count room_count()


extern int view_current;
extern int view_enabled;

}

typedef var room_view_array_type;

namespace enigma_user {
extern room_view_array_type
    view_hborder, view_hport, view_hspeed, view_hview, view_object, view_vborder,
    view_visible, view_vspeed, view_wport, view_wview, view_xport, view_xview,
    view_yport, view_yview, view_angle;
}


namespace enigma
{
  struct inst {
    int id,obj,x,y;
    inst(int id, int obj, int x, int y) : id(id), obj(obj), x(x), y(y) {}
  };
  struct tile {
      int id,bckid,bgx,bgy,depth,height,width,roomX,roomY;
      double alpha, xscale, yscale;
      int color;

      tile(int id, int bckid, int bgx, int bgy, int depth, int height,
             int width, int roomX, int roomY, double alpha,
             double xscale, double yscale, int color) :
        id(id), bckid(bckid), bgx(bgx), bgy(bgy), depth(depth), height(height), width(width),
        roomX(roomX), roomY(roomY), alpha(alpha), xscale(xscale), yscale(yscale), color(color) {}
  };
  struct viewstruct
  {
    int start_vis;
    int area_x,area_y,area_w,area_h;
    int port_x,port_y,port_w,port_h;
    int object2follow;
    int hborder,vborder,hspd,vspd;
  };
  struct backstruct {
    int visible;
    int foreground;
    int background;
    int area_x, area_y, horSpeed, verSpeed;
    int tileHor, tileVert;
    int stretch;
    double alpha;
    int color;
  };
  struct roomstruct
  {
    int id;
    int order;
    std::string name;
    std::string cap;

    int backcolor;
    bool drawbackcolor;
    variant(*createcode)();
    variant(*precreatecode)();
    int width, height, spd, persistent;
    int views_enabled;
    viewstruct views[10];
    backstruct backs[10];
    std::vector<inst> instances;
    std::vector<tile> tiles;

    void end();
    void gotome(bool gamestart = false);
  };
  void update_mouse_variables();
  extern int maxid, maxtileid;
  extern int room_switching_id; // -1 indicates no room set.
  void rooms_switch();
  void rooms_load();
  void game_start();
}

// room variable

#include "multifunction_variant.h"
namespace enigma { struct roomv: multifunction_variant<roomv> {
  INHERIT_OPERATORS(roomv)
  void function(const variant &oldval);
  roomv(): multifunction_variant<roomv>(0) {}
}; }
namespace enigma_user {
  extern enigma::roomv room;

  inline void game_restart() {
      room_goto_first(true);
  }
}

#define action_restart_game game_restart

#endif
