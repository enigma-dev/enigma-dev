/** Copyright (C) 2013, 2019 Robert B. Colton
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

#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSblend.h"
#include "Graphics_Systems/General/GSprimitives.h"

using namespace enigma::dx9;

namespace enigma_user {

int draw_set_blend_mode(int mode) {
  if (enigma::currentblendmode[0] == mode && enigma::currentblendtype == 0) return 0;
  draw_batch_flush(batch_flush_deferred);
  const static D3DBLEND dest_modes[] = {D3DBLEND_INVSRCALPHA,D3DBLEND_ONE,D3DBLEND_INVSRCCOLOR,D3DBLEND_INVSRCCOLOR};
  enigma::currentblendmode[0] = mode;
  enigma::currentblendtype = 0;
  d3dmgr->SetRenderState(D3DRS_SRCBLEND, (mode == bm_subtract) ? D3DBLEND_ZERO : D3DBLEND_SRCALPHA);
  d3dmgr->SetRenderState(D3DRS_DESTBLEND, dest_modes[mode % 4]);

  return 0;
}

int draw_set_blend_mode_ext(int src, int dest){
  if (enigma::currentblendmode[0] == src && enigma::currentblendmode[1] == dest && enigma::currentblendtype == 1) return 0;
  draw_batch_flush(batch_flush_deferred);
  const static D3DBLEND blendequivs[] = {
    D3DBLEND_ZERO, D3DBLEND_ONE, D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCCOLOR, D3DBLEND_SRCALPHA,
    D3DBLEND_INVSRCALPHA, D3DBLEND_DESTALPHA, D3DBLEND_INVDESTALPHA, D3DBLEND_DESTCOLOR,
    D3DBLEND_INVDESTCOLOR, D3DBLEND_SRCALPHASAT
  };
  enigma::currentblendtype = 1;
  enigma::currentblendmode[0] = src;
  enigma::currentblendmode[1] = dest;
  d3dmgr->SetRenderState(D3DRS_SRCBLEND, blendequivs[(src-1)%11]);
  d3dmgr->SetRenderState(D3DRS_DESTBLEND, blendequivs[(dest-1)%11]);

  return 0;
}

}
