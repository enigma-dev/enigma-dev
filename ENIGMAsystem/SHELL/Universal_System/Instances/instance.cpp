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


#include "Universal_System/Object_Tiers/object.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "instance_system.h"
#include "instance.h"

#include <map>
#include <string>
#include <sstream>
#include <math.h>
#include <stdio.h>

namespace {
 inline std::string pointer2string(void* ptr) {
   std::stringstream ss;
   ss << ptr;  
   return  ss.str(); 
 }
}

namespace enigma
{
  int destroycalls = 0, createcalls = 0;
}

typedef std::pair<int,enigma::object_basic*> inode_pair;

namespace enigma_user
{

void instance_deactivate_all(bool notme) {
    for (enigma::iterator it = enigma::instance_list_first(); it; ++it) {
        if (notme && (*it)->id == enigma::instance_event_iterator->inst->id) continue;

        (*it)->deactivate();
        enigma::instance_deactivated_list.insert(inode_pair((*it)->id,*it));
    }
}

void instance_activate_all() {

    std::map<int,enigma::object_basic*>::iterator iter = enigma::instance_deactivated_list.begin();
    while (iter != enigma::instance_deactivated_list.end()) {
        iter->second->activate();
        enigma::instance_deactivated_list.erase(iter++);
    }
}

void instance_deactivate_object(int obj) {
    for (enigma::iterator it = enigma::fetch_inst_iter_by_int(obj); it; ++it) {
        (*it)->deactivate();
        enigma::instance_deactivated_list.insert(inode_pair((*it)->id,*it));
    }
}

void instance_activate_object(int obj) {
    std::map<int,enigma::object_basic*>::iterator iter = enigma::instance_deactivated_list.begin();
    while (iter != enigma::instance_deactivated_list.end()) {
        enigma::object_basic* const inst = iter->second;
        if (obj == all || (obj < 100000 ? (inst->object_index==obj || inst->can_cast(obj)) : inst->id == unsigned(obj))) {
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
  for (enigma::iterator it = enigma::fetch_inst_iter_by_int(id); it; ++it) {
    enigma::object_basic* who = (*it);
    if (enigma::cleanups.find(who) == enigma::cleanups.end()) {
      if (dest_ev)
          who->myevent_destroy();
      if (enigma::cleanups.find(who) == enigma::cleanups.end())
          who->unlink();
    }
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
    DEBUG_MESSAGE("FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK! FUCK!\nFFFFFFFFFFFFFFFFFFFFFUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUCK!\nFUCK! " + pointer2string(a) + " ISN'T ON THE GOD DAMNED MOTHER FUCKING STACK!", MESSAGE_TYPE::M_ERROR);
    if (a != (enigma::object_basic*)enigma::instance_event_iterator->inst)
    DEBUG_MESSAGE("FUCKING DAMN IT! THE ITERATOR CHANGED FROM POINTING TO " + pointer2string((void*)a) + " TO POINTING TO " + pointer2string((void*)(enigma::object_basic*)enigma::instance_event_iterator->inst), MESSAGE_TYPE::M_ERROR);
  }
}

bool instance_exists(int obj)
{
  return enigma::fetch_instance_by_int(obj) != NULL;
}

enigma::instance_t instance_find(int obj, int num)
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
  return (enigma::objects[obj].count > 0) ? static_cast<int>(enigma::objects[obj].prev->inst->id) : noone;
}

int instance_number(int obj)
{
  switch (obj) {
    case self:
    case other:
      return (bool) enigma::fetch_inst_iter_by_int(obj);
    case all:
      return enigma::instance_list.size();
    default:
      if (obj < 0) return 0;
  }
  return enigma::objects[obj].count;
}

}
