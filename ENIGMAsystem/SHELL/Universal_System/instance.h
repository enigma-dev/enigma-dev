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
void instance_copy(bool perf = true);
inline void action_change_object(int obj, bool perf);
}

