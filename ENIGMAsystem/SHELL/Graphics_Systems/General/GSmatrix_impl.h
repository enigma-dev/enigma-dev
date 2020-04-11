/** Copyright (C) 2008-2012 Josh Ventura
*** Copyright (C) 2013-2014 Robert Colton, Harijs Grinbergs
*** Copyright (C) 2015 Harijs Grinbergs
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

#ifdef INCLUDED_FROM_SHELLMAIN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_GSMATRIX_IMPL_H
#define ENIGMA_GSMATRIX_IMPL_H

// this is for GLM's transition to radians only
// let's define it just in case some enigma
// user's package manager happens to be old
#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
// FreeBSD won't compile otheriwse because this is deprecated
#ifndef GLM_LEFT_HANDED
#define GLM_LEFT_HANDED
#endif

namespace enigma {

extern glm::mat4 world, view, projection;

} // namespace enigma

#endif // ENIGMA_GSMATRIX_IMPL_H
