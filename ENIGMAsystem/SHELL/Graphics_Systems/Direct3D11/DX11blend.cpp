/** Copyright (C) 2013, 2018 Robert B. Colton
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

#include "Direct3D11Headers.h"
#include "Graphics_Systems/General/GSblend.h"
#include "Graphics_Systems/General/GSprimitives.h"

using namespace enigma::dx11;

namespace {

D3D11_BLEND_DESC blendStateDesc = { };

void update_blend_state() {
  static ID3D11BlendState* pBlendState = NULL;
  if (pBlendState) { pBlendState->Release(); pBlendState = NULL; }
  m_device->CreateBlendState(&blendStateDesc, &pBlendState);
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
  m_deviceContext->OMSetBlendState(pBlendState, NULL, 0xffffffff);
}

} // namespace anonymous

namespace enigma {

void init_blend_state() {
  blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
  blendStateDesc.RenderTarget[0].SrcBlendAlpha = blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  blendStateDesc.RenderTarget[0].DestBlendAlpha = blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  blendStateDesc.RenderTarget[0].BlendOpAlpha = blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  update_blend_state();
}

} // namespace enigma

using namespace enigma;

namespace enigma_user
{

int draw_set_blend_mode(int mode) {
  const static D3D11_BLEND dest_modes[] = {D3D11_BLEND_INV_SRC_ALPHA,D3D11_BLEND_ONE,D3D11_BLEND_INV_SRC_COLOR,D3D11_BLEND_INV_SRC_COLOR};

  blendStateDesc.RenderTarget[0].SrcBlend = (mode == bm_subtract) ? D3D11_BLEND_ZERO : D3D11_BLEND_SRC_ALPHA;
  blendStateDesc.RenderTarget[0].DestBlend = dest_modes[mode % 4];

  update_blend_state();
  return 0;
}

int draw_set_blend_mode_ext(int src, int dest) {
  const static D3D11_BLEND blend_equivs[] = {
    D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_SRC_COLOR, D3D11_BLEND_INV_SRC_COLOR, D3D11_BLEND_SRC_ALPHA,
    D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_DEST_ALPHA, D3D11_BLEND_INV_DEST_ALPHA, D3D11_BLEND_DEST_COLOR,
    D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_SRC_ALPHA_SAT
  };

  blendStateDesc.RenderTarget[0].SrcBlend = blend_equivs[(src-1)%11];
  blendStateDesc.RenderTarget[0].DestBlend = blend_equivs[(dest-1)%11];

  update_blend_state();
  return 0;
}

}
