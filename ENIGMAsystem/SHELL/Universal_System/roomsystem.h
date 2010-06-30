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

int room_goto(double roomind);
int room_restart();
int room_goto_absolute(double index);
int room_goto_first();
int room_goto_next();
int room_next(double num);
int room_previous(double num);


extern int background_color;
extern int background_showcolor;

extern int room_first;
extern int room_height;
extern int room_last;
extern int room_persistent;
extern int room_speed;
extern int room_width;

extern int view_angle;
extern int view_current;
extern int view_enabled;
extern int view_hborder[9];
extern int view_hport[9];
extern int view_hspeed[9];
extern int view_hview[9];
extern int view_object[9];
extern int view_vborder[9];
extern int view_visible[9];
extern int view_vspeed[9];
extern int view_wport[9];
extern int view_wview[9];
extern int view_xport[9];
extern int view_xview[9];
extern int view_yport[9];
extern int view_yview[9];

#include <map>
#include <string>
#include "var4.h"

extern string room_caption;

//Make "room" globsl work
#define TYPEPURPOSE roomv
#define TYPEVARIABLES 
#define TYPEFUNCTION() room_goto((int)rval.d);
#include "multifunction_variant.h"
#undef TYPEFUNCTION
#undef TYPEVARIABLES
#undef TYPEPURPOSE

namespace enigma
{
  struct inst {
    int x,y,id,obj;
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
    string name;
    int backcolor;
    void(*createcode)();
    string cap;
    int width, height, spd;
    int views_enabled;
    viewstruct views[8];
    int instancecount;
    
    std::map<int,inst> instances;
    void gotome();
    roomstruct();
  };
  
  
  extern int room_max;
  extern std::map<int,roomstruct> roomdata;
  extern std::map<int,enigma::roomstruct>::iterator roomiter;
  
  void room_update();
}

