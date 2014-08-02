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

#include <map>
#include <vector>
#include <math.h>
#include <string>
//#include "reflexive_types.h"
//#include "EGMstd.h"
#include "object.h"

#include "instance_system.h"
#include "instance.h"
#include "Collision_Systems/General/CSfuncs.h"

#include <stdio.h>

namespace enigma
{
  int destroycalls = 0, createcalls = 0;
}

typedef std::pair<int,enigma::inst_iter*> inode_pair;


namespace enigma 
{
  bool InstDelayCmd::IsDelay = false;

  //Delete all except a given id. (-1 means "delete all")
  void instance_deactivate_all_except(int exid) {
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it) {
        if (exid>=0 && (*it)->id == exid) continue;

        ((enigma::object_basic*)*it)->deactivate();
        enigma::instance_deactivated_list.insert(inode_pair((*it)->id,it.it));
    }
  }


  void instance_flag_delay_deact() {
    InstDelayCmd::IsDelay = true;
    instance_delayed_commands.clear();
  }
  void instance_flag_clear_and_run() {
    InstDelayCmd::IsDelay = false;
    for (std::vector<InstDelayCmd>::const_iterator it=instance_delayed_commands.begin(); it!=instance_delayed_commands.end(); it++) {
      switch (it->cmd) {
        case InstDelayCmd::DeactObj: enigma_user::instance_deactivate_object(it->id); break;
        case InstDelayCmd::DeactAll: instance_deactivate_all_except(it->id); break;
        case InstDelayCmd::DeactReg: instance_deactivate_region_except(it->id, it->left, it->top, it->width, it->height, it->inside); break;
        case InstDelayCmd::DeactCirc: instance_deactivate_circle_except(it->id, it->x, it->y, it->r, it->inside); break;
        case InstDelayCmd::ActObj: enigma_user::instance_activate_object(it->id); break;
        case InstDelayCmd::ActAll: enigma_user::instance_activate_all(); break;
        case InstDelayCmd::ActReg: enigma_user::instance_activate_region(it->left, it->top, it->width, it->height, it->inside); break;
        case InstDelayCmd::ActCirc: enigma_user::instance_activate_circle(it->x, it->y, it->r, it->inside); break;
      }
    }
    instance_delayed_commands.clear();
  }
}


namespace enigma_user
{

void instance_deactivate_all(bool notme) {
    int exid = notme ? enigma::instance_event_iterator->inst->id : -1;
    if (enigma::InstDelayCmd::IsDelay) {
      enigma::instance_delayed_commands.push_back(enigma::InstDelayCmd(enigma::InstDelayCmd::DeactAll, exid));
      return;
    }

    enigma::instance_deactivate_all_except(exid);
}

void instance_activate_all() {
    if (enigma::InstDelayCmd::IsDelay) {
      enigma::instance_delayed_commands.push_back(enigma::InstDelayCmd(enigma::InstDelayCmd::ActAll));
      return;
    }

    std::map<int,enigma::inst_iter*>::iterator iter = enigma::instance_deactivated_list.begin();
    while (iter != enigma::instance_deactivated_list.end()) {
        ((enigma::object_basic*)(iter->second->inst))->activate();
        enigma::instance_deactivated_list.erase(iter++);
    }
}

void instance_deactivate_object(int obj) {
    if (enigma::InstDelayCmd::IsDelay) {
      enigma::instance_delayed_commands.push_back(enigma::InstDelayCmd(enigma::InstDelayCmd::DeactObj, obj));
      return;
    }

    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(obj); it; ++it) {
        ((enigma::object_basic*)*it)->deactivate();
        enigma::instance_deactivated_list.insert(inode_pair((*it)->id,it.it));
    }
}

void instance_activate_object(int obj) {
    if (enigma::InstDelayCmd::IsDelay) {
      enigma::instance_delayed_commands.push_back(enigma::InstDelayCmd(enigma::InstDelayCmd::ActObj, obj));
      return;
    }

    std::map<int,enigma::inst_iter*>::iterator iter = enigma::instance_deactivated_list.begin();
    while (iter != enigma::instance_deactivated_list.end()) {
        enigma::object_basic* const inst = ((enigma::object_basic*)(iter->second->inst));
        if (obj == all || (obj < 100000? inst->object_index == obj : inst->id == unsigned(obj))) {
            inst->activate();
            enigma::instance_deactivated_list.erase(iter++);
        }
        else {
            iter++;
        }
    }
}

void instance_destroy(int id, bool dest_ev)
{
  enigma::object_basic* who = enigma::fetch_instance_by_id(id);
  if (who and enigma::cleanups.find(who) == enigma::cleanups.end()) {
    if (dest_ev)
        who->myevent_destroy();
    if (enigma::cleanups.find(who) == enigma::cleanups.end())
        who->unlink();
  }
}

void instance_destroy()
{
  enigma::object_basic* const a = enigma::instance_event_iterator->inst;
  if (enigma::cleanups.find(a) == enigma::cleanups.end()) {
    enigma::instance_event_iterator->inst->myevent_destroy();
    if (enigma::cleanups.find(a) == enigma::cleanups.end())
        enigma::instance_event_iterator->inst->unlink();
    if (enigma::cleanups.find(a) == enigma::cleanups.end())
    printf("FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK!\nFFFFFFFFFFFFFFFFFFFFFUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUCK!\nFUCK! %p ISN'T ON THE GOD DAMNED MOTHER FUCKING STACK!", (void*)a);
    if (a != (enigma::object_basic*)enigma::instance_event_iterator->inst)
    printf("FUCKING DAMN IT! THE ITERATOR CHANGED FROM POINTING TO %p TO POINTING TO %p\n", (void*)a, (void*)(enigma::object_basic*)enigma::instance_event_iterator->inst);
  }
}

bool instance_exists(int obj)
{
  return enigma::fetch_instance_by_int(obj) != NULL;
}

int instance_find(int obj, int num)
{
  int nth=0;
  for (enigma::iterator it = enigma::fetch_inst_iter_by_int(obj); it; ++it)
  {
    nth++;
    if (nth>num)
    return (int) it->id;
  }
  return noone;
}
enigma::instance_t instance_last(int obj) {
  return (enigma::objects[obj].count > 0)? enigma::objects[obj].prev->inst->id : noone;
}

int instance_number(int obj)
{
  return enigma::objects[obj].count;
}

}

