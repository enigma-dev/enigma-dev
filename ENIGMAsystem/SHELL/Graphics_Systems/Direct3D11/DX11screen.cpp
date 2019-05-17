/** Copyright (C) 2008-2014 Josh Ventura, Robert B. Colton
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
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolors.h"

#include "Universal_System/roomsystem.h"
#include "Platforms/General/PFwindow.h"

using namespace enigma;
using namespace enigma::dx11;

namespace enigma {

void scene_begin() {

}

void scene_end() {

}

unsigned char* graphics_copy_back_buffer_pixels(int x, int y, int width, int height, bool* flipped) {
  return nullptr;
}

unsigned char* graphics_copy_back_buffer_pixels(unsigned* fullwidth, unsigned* fullheight, bool* flipped) {
  return nullptr;
}

} // namespace enigma

namespace enigma_user {

void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height) {
  draw_batch_flush(batch_flush_deferred);

  x = (x / window_get_region_width()) * window_get_region_width_scaled();
  y = (y / window_get_region_height()) * window_get_region_height_scaled();
  width = (width / window_get_region_width()) * window_get_region_width_scaled();
  height = (height / window_get_region_height()) * window_get_region_height_scaled();
  gs_scalar sx, sy;
  sx = (window_get_width() - window_get_region_width_scaled()) / 2;
  sy = (window_get_height() - window_get_region_height_scaled()) / 2;

  D3D11_VIEWPORT viewport = { };
  viewport.TopLeftX = sx + x;
  viewport.TopLeftY = sy + y;
  viewport.Width = width;
  viewport.Height = height;
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;

  m_deviceContext->RSSetViewports(1, &viewport);
}

} // namespace enigma_user
