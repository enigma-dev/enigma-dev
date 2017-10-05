/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, Dave "biggoron", Harijs Grinbergs
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

#ifndef DX9_SURFSTRUCT__H
#define DX9_SURFSTRUCT__H
#include <windows.h>
#include <string>
#include "Direct3D9Headers.h"
#include "DX9TextureStruct.h"
using std::string;

#include <vector>
using std::vector;

namespace enigma
{
  struct Surface
  {
    LPDIRECT3DSURFACE9 surf;
    int tex, width, height;

    Surface(): surf(NULL), tex(0), width(0), height(0) {

    };
    
    void Release() {
      if (surf != NULL) {
        surf->Release();
        surf = NULL;
      }
    }
    
    ~Surface() {
      Release();
    };
    
    void OnDeviceLost() {
      Release();
      textureStructs[tex]->OnDeviceLost();
    }
    
    void OnDeviceReset() {
      textureStructs[tex]->OnDeviceReset();
      textureStructs[tex]->gTexture->GetSurfaceLevel(0,&surf);
    }
  };
  
  extern vector<Surface*> Surfaces;
}

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_surface(surf,id)\
    if (id < 0 or id >= enigma::Surfaces.size() or !enigma::Surfaces[id]) {\
      show_error("Attempting to use non-existing surface " + toString(id), false);\
      return;\
    }\
    enigma::Surface* surf = enigma::Surfaces[id];
  #define get_surfacev(surf,id,r)\
    if (id < 0 or size_t(id) >= enigma::Surfaces.size() or !enigma::Surfaces[id]) {\
      show_error("Attempting to use non-existing surface " + toString(id), false);\
      return r;\
    }\
    enigma::Surface* surf = enigma::Surfaces[id];
#else
  #define get_surface(surf,id)\
    enigma::Surface* surf = enigma::Surfaces[id];
  #define get_surfacev(surf,id,r)\
    enigma::Surface* surf = enigma::Surfaces[id];
#endif

#endif // DX9_SURFSTRUCT__H
