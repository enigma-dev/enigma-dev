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

#include "var4.h"

int room_goto(int roomind);
int room_restart();
int room_goto_absolute(int index);
int room_goto_first();
int room_goto_next();
int room_next(int num);
int room_previous(int num);

extern int background_color;
extern int background_showcolor;

extern int room_first;
extern int room_height;
extern int room_last;
extern int room_persistent;
extern int room_speed;
extern int room_width;

extern string room_caption;

int room_count();
#define room_count room_count()

extern int view_angle;
extern int view_current;
extern int view_enabled;
extern var view_hborder, view_hport, view_hspeed, view_hview, view_object, view_vborder, view_visible, 
           view_vspeed, view_wport, view_wview, view_xport, view_xview, view_yport, view_yview;

namespace enigma
{
  struct inst {
    int id,obj,x,y;
  };
  struct viewstruct
  {
    int start_vis;
    int area_x,area_y,area_w,area_h;
    int port_x,port_y,port_w,port_h;
    int object2follow;
    int hborder,vborder,hspd,vspd;
  };
  struct roomstruct
  {
    int id;
    string name;
    string cap;
    
    int backcolor;
    void(*createcode)();
    int width, height, spd;
    int views_enabled;
    viewstruct views[8];
    
    int instancecount;
    inst *instances;
    
    void gotome();
  };
  void room_update();
  extern int room_max, maxid;
  void rooms_load();
}

// room variable

  #define TYPEPURPOSE roomv
  #define TYPEVARIABLES
    #include "multifunction_variant.h"
  #undef TYPEVARIABLES
  #undef TYPEPURPOSE
extern enigma::roomv room;
