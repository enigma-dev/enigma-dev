/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2012 Alasdair Morrison                                        **
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

#include "Universal_System/graphics_object.h"
#include "Universal_System/instance_system_base.h"

namespace enigma {
  extern int destroycalls, createcalls;
}

namespace enigma_user {
enigma::instance_t instance_create(int x,int y,int object);
}

namespace enigma {
  object_basic *instance_create_id(int x,int y,int object,int idg); //This is for use by the system only. Please leave be.

  //These are also required by the system (called for delayed deactivation).
  void instance_deactivate_region_except(int exid, int rleft, int rtop, int rwidth, int rheight, bool inside);
  void instance_deactivate_circle_except(int exid, int x, int y, int r, bool inside);

  //Flag when we are within a "delayed deactivation" zone. This includes room creation and object creation code, and delays 
  //instance_deactivate*() calls until the "clear" function is called. It also delays instance_activate*() calls, since 
  //they would be out-of-order with respect to the deactivates.
  void instance_flag_delay_deact();
  void instance_flag_clear_and_run();

  //This struct is used internally to when and how to "delay" instance deactivation commands.
  struct InstDelayCmd {
    static bool IsDelay; //If true, we are in a "delayed deactivation zone" (room creation code).

    enum Cmd { //The type of command we are delaying.
      DeactObj = 0,  //instance_deactivate_object()
      DeactAll,      //instance_deactivate_all()
      DeactReg,      //instance_deactivate_region()
      DeactCirc,     //instance_deactivate_circle()
      ActObj,        //instance_activate_object()
      ActAll,        //instance_activate_all()
      ActReg,        //instance_activate_region()
      ActCirc,       //instance_activate_circle()
    } cmd;

    //Params for each delayed command type.
    int id; //For the *all() commands, this is the id *not* to delete, or -1 if notme == false.
    union {
      int left; //for region()
      int x;    //for circle()
    };
    union {
      int top; //for region()
      int y;    //for circle()
    };
    union {
      int width; //for region()
      int r;    //for circle()
    };
    int height;
    bool inside;

    //Most commands only take the command type and an id. (This is also the no-args constructor, for storage in a vector).
    explicit InstDelayCmd(Cmd cmd=DeactObj, int id=0) : cmd(cmd), id(id), left(0), top(0), width(0), height(0), inside(false) {}

    //Region commands take left/top/width/height/inside.
    InstDelayCmd(Cmd cmd, int id, int left, int top, int width, int height, bool inside)
      : cmd(cmd), id(id), left(left), top(top), width(width), height(height), inside(inside) {}

    //Circle commands take x/y/r/inside.
    InstDelayCmd(Cmd cmd, int id, int x, int y, int r, bool inside)
      : cmd(cmd), id(id), x(x), y(y), r(r), height(0), inside(inside) {}
  };

  //Our list of delayed commands. If InstDelayCmd::IsDelay is true, then append to this. Else, execute the command.
  static std::vector<InstDelayCmd> instance_delayed_commands;
}

namespace enigma_user {
void instance_deactivate_all(bool notme);
void instance_activate_all();
void instance_activate_object(int obj);
void instance_deactivate_object(int obj);
void instance_destroy(int id, bool dest_ev = true);
void instance_destroy();
bool instance_exists (int obj);
enigma::instance_t instance_find   (int obj,int n);
int instance_number (int obj);
enigma::instance_t instance_last(int obj);
enigma::instance_t instance_nearest (int x,int y,int obj,bool notme = false);
enigma::instance_t instance_furthest(int x,int y,int obj,bool notme = false);
}


//int instance_place(x,y,obj)
//int instance_copy(performevent)
namespace enigma {
  void instance_change_inst(int obj, bool perf, enigma::object_graphics* inst);
}

namespace enigma_user {
void instance_change(int obj, bool perf = false);
void instance_copy(bool perf = true); // this is supposed to return an iterator
inline void action_change_object(int obj, bool perf);
}

