/** Copyright (C) 2008 Josh Ventura
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

#ifndef INSTANCE_SYSTEM_BASE_h
#define INSTANCE_SYSTEM_BASE_h

#include "Universal_System/Object_Tiers/object.h"
#include <string>

namespace enigma
{
  typedef variant instance_t;

  struct inst_iter
  {
    object_basic* inst;     // Inst is first member for non-arithmetic dereference
    inst_iter *next, *prev; // Double linked for active removal
    bool dead;              // Whether or not this instance has been destroyed. Should be accessed rarely.
    //std::deque<inst_iter*>::iterator instance_id_index;
    inst_iter(object_basic* i,inst_iter *n,inst_iter *p);
    inst_iter();
  };

  class temp_event_scope
  {
    inst_iter *oiter;
    object_basic* prev_other;
    inst_iter niter;

    public:
    temp_event_scope(object_basic*);
    ~temp_event_scope();
  };

  // This structure is for composing lists of events to execute.
  struct event_iter: inst_iter // It is a special instance iterator with an add_inst member and a name.
  {
    // Inherits object_basic *inst: should be NULL
    // Inherits inst_iter *next:    First of instances for which to perform this event (Can be NULL)
    // Inherits inst_iter *prev:    The last instance for which to perform it. (Can be NULL)
    std::string name; // Event name
    inst_iter *add_inst(object_basic* inst);  // Append an instance to the list
    void unlink(inst_iter*);
    event_iter(std::string name);
    event_iter();
  };

  // This structure will store info about and lists of each object by index.
  struct objectid_base: inst_iter
  {
    // Inherits object_basic *inst: should be NULL
    // Inherits inst_iter *next:    First of instances for which to perform this event (Can be NULL)
    // Inherits inst_iter *prev:    The last instance for which to perform it. (Can be NULL)
    size_t count;     // Number of instances on this list
    inst_iter *add_inst(object_basic* inst);  // Append an instance to the list
    objectid_base();
  };

  // The rest is decently commented on in the corresponding source file.
  extern event_iter *events;
  extern objectid_base *objects;
  extern object_basic *ENIGMA_global_instance;
  extern inst_iter dummy_event_iterator;
  extern inst_iter *instance_event_iterator;
  extern object_basic *instance_other;

  // Stack pusher for iterators in use by with() statements and the like.
  struct iterator_level {
    inst_iter* stored_it;
    object_basic* stored_other;
    iterator_level(inst_iter* push_to, object_basic* push_other):
        stored_it(instance_event_iterator), stored_other(instance_other) {
      instance_event_iterator = push_to;
      instance_other = push_other;
    }
    iterator_level(inst_iter* push_to):
        iterator_level(push_to, instance_event_iterator->inst) {}
    ~iterator_level() {
      instance_event_iterator = stored_it;
      instance_other = stored_other;
    }
  };

  object_basic* fetch_instance_by_int(int x);
  object_basic* fetch_instance_by_id(int x);
}

// Other
namespace enigma_user {
  extern int instance_count;
}
#endif
