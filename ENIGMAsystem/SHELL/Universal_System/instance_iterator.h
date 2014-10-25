/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2014 Josh Ventura, Robert B. Colton
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

#ifndef _INSTANCE_ITERATOR_H
#define _INSTANCE_ITERATOR_H

#include "instance_system_base.h"

namespace enigma {
  struct iterator {
    inst_iter temp_iter;
    struct inst_iter* it;
    
    operator bool();
    object_basic* operator*();
    object_basic* operator->();
    
    iterator &operator++();
    iterator operator++(int);
    iterator &operator--();
    iterator operator--(int);
    
    iterator &operator=(const iterator&);
    iterator &operator=(inst_iter*);
    iterator &operator=(object_basic*);
    
    iterator(const iterator&);
    iterator(inst_iter*);
    iterator(object_basic*);
    iterator();
    
    ~iterator();
    
    private: 
      void addme();
      static inst_iter* copy(const iterator& other, inst_iter& temp_iter);
  };
  
  void update_iterators_for_destroy(const inst_iter*);
  iterator instance_list_first();
  iterator fetch_inst_iter_by_id(int id);
  iterator fetch_inst_iter_by_int(int x);
  iterator fetch_roominst_iter_by_id(int x);
}

#endif
