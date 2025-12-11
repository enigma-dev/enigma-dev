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
#ifndef ENIGMA_COMPILER_PARSING_FULL_TYPE_h
#define ENIGMA_COMPILER_PARSING_FULL_TYPE_h

#include <JDI/src/Storage/definition.h>

#include "declarator.h"

namespace enigma::parsing {
struct FullType {
  jdi::definition *def = nullptr;
  Declarator decl{};
  std::size_t flags = 0;

  FullType() noexcept = default;
  FullType(FullType &&) noexcept = default;
  FullType &operator=(FullType &&) noexcept = default;

  FullType(jdi::definition *def, Declarator decl, std::size_t flags);
  FullType(jdi::definition *def);

  jdi::full_type to_jdi_fulltype();
};
}

#endif