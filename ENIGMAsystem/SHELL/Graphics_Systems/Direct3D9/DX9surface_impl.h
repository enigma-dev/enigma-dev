/** Copyright (C) 2008-2013 Josh Ventura, Dave "biggoron", Harijs Grinbergs
*** Copyright (C) 2013 Robert B. Colton
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

#ifndef ENIGMA_DX9_SURFACE_IMPL_H
#define ENIGMA_DX9_SURFACE_IMPL_H

#include "DX9textures_impl.h"
#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSsurface_impl.h"

using namespace enigma::dx9;

namespace enigma {

struct Surface : BaseSurface {
  LPDIRECT3DSURFACE9 surf;

  // for the purpose of restoring the texture,
  // not all lost devices will have lost the data,
  // e.g. when we are about to manually display reset
  LPDIRECT3DSURFACE9 pCopy;

  Surface(): BaseSurface(), surf(NULL), pCopy(NULL) {

  };

  void Release() {
    if (surf != NULL) {
      surf->Release(), surf = NULL;
    }
  }

  ~Surface() {
    Release();
  };

  void OnDeviceLost() {
    D3DSURFACE_DESC backupdesc;
    surf->GetDesc(&backupdesc);

    d3ddev->CreateOffscreenPlainSurface(backupdesc.Width, backupdesc.Height, backupdesc.Format, D3DPOOL_SYSTEMMEM, &pCopy, NULL);
    d3ddev->GetRenderTargetData(surf, pCopy);

    Release();
    DX9Texture* d3dtex = static_cast<DX9Texture*>(enigma::textures[texture].get());
    d3dtex->peer->Release(), d3dtex->peer = NULL;
  }

  void OnDeviceReset() {
    D3DSURFACE_DESC backupdesc;
    pCopy->GetDesc(&backupdesc);

    // recreate the texture peer
    DX9Texture* d3dtex = static_cast<DX9Texture*>(enigma::textures[texture].get());
    d3ddev->CreateTexture(backupdesc.Width, backupdesc.Height, 1, D3DUSAGE_RENDERTARGET, backupdesc.Format, D3DPOOL_DEFAULT, &d3dtex->peer, NULL);
    d3dtex->peer->GetSurfaceLevel(0,&surf);

    // reupload the texture data
    d3ddev->UpdateSurface(pCopy, NULL, surf, NULL);
    pCopy->Release();
  }
};

} // namespace enigma

#endif // ENIGMA_DX9_SURFACE_IMPL_H
