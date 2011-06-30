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

#include <map>
#include <math.h>
#include <vector>
#include <string>
#include "var4.h"
#include "reflexive_types.h"
#include <stdio.h>

#include "instance_system.h"

using namespace std;

int instance_count = 0;

namespace enigma
{
  inst_iter::inst_iter(object_basic* i,inst_iter *n = NULL,inst_iter *p = NULL): inst(i), next(n), prev(p) {}
  objectid_base::objectid_base(): inst_iter(NULL,NULL,this), count(0) {}
  event_iter::event_iter(string n): inst_iter(NULL,NULL,this), name(n) {}
  event_iter::event_iter(): inst_iter(NULL,NULL,this) {}

  inst_iter *event_iter::add_inst(object_basic* inst)
  {
    inst_iter *a = new inst_iter(inst,NULL,prev);
    if (prev) prev->next = a;
    else next = a;
    return prev = a;
  }

  void event_iter::unlink(inst_iter* which)
  {
    if (which->prev) which->prev->next = which->next;
    if (which->next) which->next->prev = which->prev;
    if (prev == which) prev = which->prev;
  }

  inst_iter *objectid_base::add_inst(object_basic* inst)
  {
    inst_iter *a = new inst_iter(inst,NULL,prev);
    if (prev) prev->next = a;
    else next = a;
    return prev = a;
  }

  void unlink_object_id_iter(inst_iter* which, int oid)
  {
    if (which->prev) which->prev->next = which->next;
    if (which->next) which->next->prev = which->prev;
    objectid_base *a = objects + oid;
    if (a->prev == which) a->prev = which->prev;
    a->count--;
  }
  
  /* **  Variables ** */
  // This will be instantiated for each event with a unique ID or Sub ID.
  event_iter *events; // It will be allocated towards the beginning.

  // Through these, we will list objects by object_index, and implement heredity.
  objectid_base *objects;

  // This is the all-inclusive, centralized list of instances.
  map<int,inst_iter*> instance_list;
  typedef map<int,inst_iter*>::iterator iliter;
  typedef pair<int,inst_iter*> inode_pair;

  // When you say "global.vname", this is the structure that answers
  extern object_basic *ENIGMA_global_instance; // We also need an iterator for only global.
  inst_iter ENIGMA_global_instance_iterator(ENIGMA_global_instance,0,0);

  // With() will operate on this
  iterator_level::iterator_level(inst_iter* i,object_basic* o, iterator_level* l): it(i), other(o), last(l) {}
  void iterator_level::push(inst_iter* i,object_basic* o) { il_top = new iterator_level(i,o,il_top); }
  void iterator_level::pop() { il_top = il_top->last; }
  iterator_level *il_top = NULL;

  // This is basically a garbage collection list for when instances are destroyed
  vector<inst_iter*> cleanups; // We'll use vector

  // It's a good idea to centralize an event iterator so error reporting can tell where it is.
  static inst_iter dummy_event_iterator(NULL,NULL,NULL); // For create events and such
  inst_iter *instance_event_iterator = &dummy_event_iterator; // Not bad for efficiency, either.
  object_basic *instance_other = NULL;

  temp_event_scope::temp_event_scope(object_basic* ninst): oinst(instance_event_iterator->inst), oiter(instance_event_iterator)
    { instance_event_iterator = &dummy_event_iterator; instance_event_iterator->inst = ninst; }
  temp_event_scope::~temp_event_scope() { instance_event_iterator->inst = oinst; instance_event_iterator = oiter; }

  /* **  Methods ** */
  // Retrieve the first instance on the complete list.
  inst_iter* instance_list_first()
  {
    iliter a = instance_list.begin();
    return a != instance_list.end() ? a->second : NULL;
  }

  extern int object_idmax;
  object_basic* fetch_instance_by_int(int x)
  {
    if (x < 0) switch (x)
    {
      case self:
      case local:  return instance_event_iterator ? instance_event_iterator->inst : NULL;
      case other:  return instance_other;
      case all: {  inst_iter *i = instance_list_first();
                   return  i ? i->inst : NULL; }
      case global: return ENIGMA_global_instance;
      case noone:
         default:  return NULL;
    }

    if (x < 100000)
      return x < object_idmax ? objects[x].next ? objects[x].next->inst : NULL : NULL;

    iliter a = instance_list.find(x);
    return a != instance_list.end() ? a->second->inst : NULL;
  }
  inst_iter dummy_iterator(NULL,NULL,NULL);
  inst_iter* fetch_inst_iter_by_int(int x)
  {
    if (x < 0) switch (x) // Keyword-based lookup
    {
      case self:
      case local:  return instance_event_iterator;
      case other:  return instance_other ? (dummy_iterator.inst = instance_other, &dummy_iterator) : NULL;
      case all:    return instance_list_first();
      case global: return &ENIGMA_global_instance_iterator;
      case noone:
         default:  return NULL;
    }

    if (x < 100000) // Object-index-based lookup
      return objects[x].next;

    // ID-based lookup
    iliter a = instance_list.find(x);
    return a != instance_list.end() ? (dummy_iterator.inst = a->second->inst, &dummy_iterator) : NULL;
  }
  inst_iter* fetch_inst_iter_by_id(int x)
  {
    if (x < 100000)
      return NULL;

    iliter a = instance_list.find(x);
    return a != instance_list.end() ? a->second : NULL;
  }
  
  // Implementation for frontend
  // (Wrapper struct to lower compile time)
  typedef struct winstance_list_iterator {
    instance_list_iterator w;
    winstance_list_iterator(instance_list_iterator n): w(n) {}
  } *pinstance_list_iterator;
  void winstance_list_iterator_delete(pinstance_list_iterator whop) {
    delete whop;
  }
  
  //Link in an instance
  pinstance_list_iterator link_instance(object_basic* who)
  {
    inst_iter *ins = new inst_iter(who);
    pair<iliter,bool> it = instance_list.insert(inode_pair(who->id,ins));
    if (!it.second) {
      delete ins;
      return new winstance_list_iterator(it.first);
    }
    if (it.first != instance_list.begin())
    {
      iliter ib = it.first; ib--; // Find the previous iterator
      ins->prev = ib->second; // Link this to previous instance
      ib->second->next = ins; // Link previous instance to this
    }
    else ins->prev = NULL; // Found nothing.

    iliter in = it.first; in++; // Find next iterator
    if (in != instance_list.end()) // If it exists
      ins->next = in->second, // Link this to next instance
      in->second->prev = ins; // Link next to this
    else ins->next = NULL;
    return new winstance_list_iterator(it.first);
  }
  inst_iter *link_obj_instance(object_basic* who, int oid)
  {
    objects[oid].count++;
    return objects[oid].add_inst(who);
  }
  
  void instance_iter_queue_for_destroy(pinstance_list_iterator whop)
  {
    enigma::cleanups.push_back(whop->w->second);
    enigma::instancecount--;
    instance_count--;
  }
  void dispose_destroyed_instances()
  {
    for (size_t i = 0; i < cleanups.size(); i++)
      delete cleanups[i]->inst;
    cleanups.clear();
  }
  void unlink_main(instance_list_iterator who)
  {
    inst_iter *a = who->second;
    if (a->prev) a->prev->next = a->next;
    if (a->next) a->next->prev = a->prev;
    instance_list.erase(who);
  }
  void unlink_main(pinstance_list_iterator whop)
  {
    inst_iter *a = whop->w->second;
    if (a->prev) a->prev->next = a->next;
    if (a->next) a->next->prev = a->prev;
    instance_list.erase(whop->w);
  }

  //This is the universal create event code
  void constructor(object_basic* instance);
}
