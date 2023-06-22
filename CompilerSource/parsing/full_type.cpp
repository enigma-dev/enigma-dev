/** Copyright (C) 2022 Dhruv Chawla
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
#include "full_type.h"

namespace enigma::parsing {
FullType::FullType(jdi::definition *def, Declarator decl, std::size_t flags):
 def{def}, decl{std::move(decl)}, flags{flags} {}

FullType::FullType(jdi::definition *def): FullType(def, {}, 0) {}

jdi::full_type FullType::to_jdi_fulltype() {
  jdi::ref_stack rt;
  decl.to_jdi_refstack(rt);
  return {def, rt, static_cast<int>(flags)};
}
}
