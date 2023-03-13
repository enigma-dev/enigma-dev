/** Copyright (C) 2008-2013 Josh Ventura
*** Copyright (C) 2013,2019 Robert B. Colton
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

#ifndef ENIGMA_GS_SURFACE_IMPL_H
#define ENIGMA_GS_SURFACE_IMPL_H

#include <vector>
using std::vector;

namespace enigma {

struct BaseSurface {
  int texture, width, height;
protected:
  // we want BaseSurface abstract/non-instantiable
  // each backend assumes it can safely cast
  // to get the peer type, we don't want any
  // kind of generic surface in the vector
  BaseSurface(): texture(0), width(0), height(0) {}
};

extern vector<BaseSurface*> surfaces;

struct Surface; // forward-declaration for get_surface
extern unsigned int bound_framebuffer;

} // namespace enigma

#ifdef DEBUG_MODE
  #include "GSsurface.h"
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #include <string>
  #define get_surface(surf,id)\
    if (surface_exists(id) == false) {\
      DEBUG_MESSAGE("Attempting to use non-existing surface " + toString(id), MESSAGE_TYPE::M_USER_ERROR);\
      return;\
    }\
    enigma::Surface &surf = *((enigma::Surface*)enigma::surfaces[id]);
  #define get_surfacev(surf,id,r)\
    if (surface_exists(id) == false) {\
      DEBUG_MESSAGE("Attempting to use non-existing surface " + toString(id), MESSAGE_TYPE::M_USER_ERROR);\
      return r;\
    }\
    enigma::Surface &surf = *((enigma::Surface*)enigma::surfaces[id]);
#else
  #define get_surface(surf,id)\
    enigma::Surface &surf = *((enigma::Surface*)enigma::surfaces[id]);
  #define get_surfacev(surf,id,r)\
    enigma::Surface &surf = *((enigma::Surface*)enigma::surfaces[id]);
#endif

#endif // ENIGMA_GS_SURFACE_IMPL_H
