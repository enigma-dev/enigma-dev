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


extern double background_color;
extern double background_showcolor;

extern double room_first;
extern double room_height;
extern double room_last;
extern double room_persistent;
extern double room_speed;
extern double room_width;

extern double view_angle;
extern double view_current;
extern double view_enabled;
extern double view_hborder[9];
extern double view_hport[9];
extern double view_hspeed[9];
extern double view_hview[9];
extern double view_object[9];
extern double view_vborder[9];
extern double view_visible[9];
extern double view_vspeed[9];
extern double view_wport[9];
extern double view_wview[9];
extern double view_xport[9];
extern double view_xview[9];
extern double view_yport[9];
extern double view_yview[9];

#include <map>
#include <string>
#include "var_cr3.h"

extern std::string room_caption;

//Make "room" globsl work
#define TYPEPURPOSE roomv
#define TYPEFUNCTION() room_goto((int)realval);
#include "multifunction_variant.h"
#undef TYPEFUNCTION
#undef TYPEPURPOSE

namespace enigma
{
  struct inst {
    int x,y,id,obj;
  };
  struct roomstruct
  {
    std::string name;
    int backcolor;
    void(*createcode)();
    std::string cap;
    int width, height, spd;
    char views_enabled,start_vis[8];
    int area_x[8],area_y[8],area_w[8],area_h[8];
    int port_x[8],port_y[8],port_w[8],port_h[8];
    int object2follow[8];
    int hborder[8],vborder[8],hspd[8],vspd[8];
      int instancecount;
    
    std::map<int,inst> instances;
    void gotome();
  };
  
  extern int room_max;
  extern std::map<int,roomstruct> roomdata;
  extern std::map<int,enigma::roomstruct>::iterator roomiter;
  
  void room_update();
}

