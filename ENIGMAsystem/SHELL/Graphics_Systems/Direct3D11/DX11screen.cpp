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
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GScolors.h"

#include "Universal_System/roomsystem.h"
#include "Platforms/General/PFwindow.h"

#include <string>
#include <cstdio>

using namespace std;
using namespace enigma;
using namespace enigma::dx11;

namespace enigma
{

void scene_begin() {

}

void scene_end() {

}

}

namespace enigma_user
{

void screen_init()
{
  enigma::gui_width = window_get_region_width();
  enigma::gui_height = window_get_region_height();

  m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

  if (!view_enabled)
  {
    screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());
    d3d_set_projection_ortho(0, 0, window_get_region_width(), window_get_region_height(), 0);
  } else {
    for (view_current = 0; view_current < 7; view_current++) {
      if (view_visible[(int)view_current]) {
        int vc = (int)view_current;

        screen_set_viewport(view_xport[vc], view_yport[vc], view_wport[vc], view_hport[vc]);
        d3d_set_projection_ortho(view_xview[vc], view_yview[vc], view_wview[vc], view_hview[vc], view_angle[vc]);
        break;
      }
    }
  }
}

int screen_save(string filename)
{
  draw_batch_flush(batch_flush_deferred);
  return -1; //TODO: implement
}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h)
{
  draw_batch_flush(batch_flush_deferred);
  return -1; //TODO: implement
}

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

//TODO: These need to be in some kind of General
void display_set_gui_size(unsigned int width, unsigned int height) {
  enigma::gui_width = width;
  enigma::gui_height = height;
}

}
