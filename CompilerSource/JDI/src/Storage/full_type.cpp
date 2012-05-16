#include "full_type.h"
#include <iostream>

namespace jdi {
  full_type::full_type(): def(NULL) {}
  full_type::full_type(jdi::definition* d): def(d) {}
  full_type::full_type(jdi::definition* d, int f): def(d), refs(), flags(f) {}
  full_type::full_type(jdi::definition* d, jdi::ref_stack r, int f): def(d), refs(r), flags(f) {}
  full_type::full_type(const full_type& ft): def(ft.def), refs(ft.refs), flags(ft.flags) { cout << "DUPLICATED FULLTYPE" << endl; }
  void full_type::swap(full_type& ft) { 
    jdi::definition* ds = ft.def;
    ft.def = def, def = ds;
    int fs = ft.flags;
    ft.flags = flags, flags = fs;
    refs.swap(ft.refs);
  }
}
