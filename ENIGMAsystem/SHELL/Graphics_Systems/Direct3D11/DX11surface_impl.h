/** Copyright (C) 2008-2013 Josh Ventura, Harijs Grinbergs
*** Copyright (C) 2013 Robert B. Colton, Dave "biggoron"
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

#ifndef ENIGMA_DX11_SURFACES_IMPL_H
#define ENIGMA_DX11_SURFACES_IMPL_H

#include "Direct3D11Headers.h"
#include "Graphics_Systems/General/GSsurface_impl.h"

namespace enigma {

struct Surface : BaseSurface
{
  ID3D11RenderTargetView* renderTargetView;

  Surface(): BaseSurface(), renderTargetView(NULL) {};
  ~Surface() {
    if (renderTargetView != NULL) {
      renderTargetView->Release();
      renderTargetView = NULL;
    }
  };
};

} // namespace enigma

#endif // ENIGMA_DX11_SURFACES_IMPL_H
