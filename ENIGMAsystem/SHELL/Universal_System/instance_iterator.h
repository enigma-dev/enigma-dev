/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2014 Josh Ventura, Robert B. Colton
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

#ifndef ENIGMA_INSTANCE_ITERATOR_H
#define ENIGMA_INSTANCE_ITERATOR_H

#include "instance_system_base.h"

namespace enigma {

class iterator {
  enigma::inst_iter temp_iter;
  enigma::inst_iter* it;

  void addme();
  void copy(const iterator& other);

 public:
  operator bool();
  object_basic* operator*() const;
  object_basic* operator->() const;

  void handle_unlink(const inst_iter* dead);

  iterator& operator++();
  iterator operator++(int);
  iterator& operator--();
  iterator operator--(int);

  iterator& operator=(const iterator&);
  iterator& operator=(inst_iter*);
  iterator& operator=(object_basic*);

  iterator(const iterator&);
  iterator(inst_iter*);
  iterator(object_basic*);
  iterator();

  ~iterator();

  class with;
};

class iterator::with : iterator, iterator_level {
 public:
  with(const iterator& push) : iterator(push), iterator_level(it) {}
};

void update_iterators_for_destroy(const inst_iter*);
iterator instance_list_first();
iterator fetch_inst_iter_by_id(int id);
iterator fetch_inst_iter_by_int(int x);
iterator fetch_roominst_iter_by_id(int x);

}  //namespace enigma

#endif  //ENIGMA_INSTANCE_ITERATOR_H
