/** Copyright (C) 2013 Robert B. Colton
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

#include "Bridges/General/DX9Context.h"
#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSblend.h"
namespace enigma
{
  extern int currentblendmode[2];
  extern int currentblendtype;
}

namespace enigma_user
{

int draw_set_blend_mode(int mode){
    if (enigma::currentblendmode[0] == mode && enigma::currentblendtype == 0) return 0;
    enigma::currentblendmode[0] = mode;
    enigma::currentblendtype = 0;
	switch (mode)
	{
    case bm_add:
		d3dmgr->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		d3dmgr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3dmgr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
      return 0;
    case bm_max:
		d3dmgr->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_MAX);
		d3dmgr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3dmgr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
      return 0;
    case bm_subtract:
		d3dmgr->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
		d3dmgr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		d3dmgr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
      return 0;
    default:
		// bm_normal
        d3dmgr->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		d3dmgr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3dmgr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
      return 0;
  }
}

int draw_set_blend_mode_ext(int src, int dest){
  if (enigma::currentblendmode[0] == src && enigma::currentblendmode[1] == dest && enigma::currentblendtype == 1) return 0;
  const static D3DBLEND blendequivs[11] = {
	  D3DBLEND_ZERO, D3DBLEND_ONE, D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCCOLOR, D3DBLEND_SRCALPHA,
	  D3DBLEND_INVSRCALPHA, D3DBLEND_DESTALPHA, D3DBLEND_INVDESTALPHA, D3DBLEND_DESTCOLOR,
	  D3DBLEND_INVDESTCOLOR, D3DBLEND_SRCALPHASAT
  };
  enigma::currentblendtype = 1;
  enigma::currentblendmode[0] = src;
  enigma::currentblendmode[1] = dest;
  d3dmgr->SetRenderState(D3DRS_SRCBLEND, blendequivs[(src-1)%10]);
  d3dmgr->SetRenderState(D3DRS_DESTBLEND, blendequivs[(dest-1)%10]);
  return 0;
}

int draw_get_blend_mode(){
    return enigma::currentblendmode[0];
}

int draw_get_blend_mode_ext(bool src){
    return enigma::currentblendmode[(src==true?0:1)];
}

int draw_get_blend_mode_type(){
    return enigma::currentblendtype;
}

}
