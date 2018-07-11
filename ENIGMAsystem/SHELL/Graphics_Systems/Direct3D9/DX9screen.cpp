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

#include <string>
#include <cstdio>
#include "Bridges/General/DX9Context.h"
#include "Direct3D9Headers.h"
#include "../General/GSsprite.h"
#include "../General/GSbackground.h"
#include "../General/GSscreen.h"
#include "../General/GSd3d.h"
#include "../General/GSmatrix.h"
#include "../General/GStextures.h"
#include "../General/GScolors.h"

using namespace std;

#include "Universal_System/image_formats.h"
#include "Universal_System/background_internal.h"
#include "Universal_System/background.h"
#include "Universal_System/var4.h"
#include "Universal_System/estring.h"

#include "Graphics_Systems/General/GScolor_macros.h"

#include "Universal_System/roomsystem.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"
#include "Universal_System/depth_draw.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/General/PFmain.h"
#include "Platforms/platforms_mandatory.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include <limits>

//Fuck whoever did this to the spec
#ifndef GL_BGR
  #define GL_BGR 0x80E0
#endif

using namespace enigma;

#include "../General/GSmodel.h"

namespace enigma_user {
  extern int window_get_width();
  extern int window_get_height();
  extern int window_get_region_width();
  extern int window_get_region_height();
}

namespace enigma
{
  extern bool d3dMode;
  extern bool d3dHidden;
  extern int d3dCulling;
  particles_implementation* particles_impl;
  void set_particles_implementation(particles_implementation* part_impl)
  {
      particles_impl = part_impl;
  }

	unsigned gui_width;
	unsigned gui_height;
}

namespace enigma_user
{

void screen_redraw()
{
  //d3dmgr->EndShapesBatching(); //If called inside bound surface we need to finish drawing
  d3dmgr->BeginScene();
  // Clean up any textures that ENIGMA may still think are binded but actually are not
  d3d_set_zwriteenable(true);
  if (!view_enabled)
  {
    screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());

    clear_view(0, 0, window_get_region_width(), window_get_region_height(), 0, background_showcolor);
    draw_back();
    draw_insts();
    draw_tiles();
  }
  else
  {
    //TODO: Possibly implement view option from Stupido to control which view clears the background
    // Only clear the background on the first visible view by checking if it hasn't been cleared yet
    bool draw_backs = true;
    bool background_allviews = true; // FIXME: Create a setting for this.
    for (view_current = 0; view_current < 8; view_current++)
    {
      int vc = (int)view_current;
      if (!view_visible[vc])
        continue;

      int vob = (int)view_object[vc];
      if (vob != -1)
        follow_object(vob, vc);

      screen_set_viewport(view_xport[vc], view_yport[vc], view_wport[vc], view_hport[vc]);

      clear_view(view_xview[vc], view_yview[vc], view_wview[vc], view_hview[vc], view_angle[vc], background_showcolor && draw_backs);

      if (draw_backs)
        draw_back();

      draw_insts();

      if (draw_tiles())
        break;
      draw_backs = background_allviews;
    }
    // In Studio this variable is not reset until the next iteration of views as is actually 7 in the draw_gui event, in 8.1 however view_current will always be 0 in the step event.
    view_current = 0;
  }

  // Now process the sub event of draw called draw gui
  // It is for drawing GUI elements without view scaling and transformation
  if (enigma::gui_used)
  {
    screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());
    d3d_set_projection_ortho(0, 0, enigma::gui_width, enigma::gui_height, 0);

    // Clear the depth buffer if hidden surface removal is on at the beginning of the draw step.
    if (enigma::d3dMode)
      d3dmgr->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    draw_gui();
  }

  if (sprite_exists(cursor_sprite))
    draw_sprite(cursor_sprite, 0, mouse_x, mouse_y);

  d3dmgr->EndScene();

  ///TODO: screen_refresh() shouldn't be in screen_redraw(). They are separate functions for a reason.
  screen_refresh();
}

void screen_init()
{
  enigma::gui_width = window_get_region_width();
  enigma::gui_height = window_get_region_height();

  d3dmgr->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  d3dmgr->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    if (!view_enabled)
    {
      d3dmgr->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

      screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());
      d3d_set_projection_ortho(0, 0, window_get_region_width(), window_get_region_height(), 0);
    } else {
      for (view_current = 0; view_current < 7; view_current++)
      {
        if (view_visible[(int)view_current])
        {
          int vc = (int)view_current;

          screen_set_viewport(view_xport[vc], view_yport[vc],
            (window_get_region_width_scaled() - view_xport[vc]), (window_get_region_height_scaled() - view_yport[vc]));
          d3d_set_projection_ortho(view_xview[vc], view_wview[vc] + view_xview[vc], view_yview[vc], view_hview[vc] + view_yview[vc], view_angle[vc]);
          break;
        }
      }
    }

  d3dmgr->SetRenderState(D3DRS_LIGHTING, FALSE);
  d3dmgr->SetRenderState(D3DRS_ZENABLE, FALSE);
  // make the same default as GL, keep in mind GM uses reverse depth ordering for ortho projections, where the higher the z value the further into the screen you are
  // but that is currently taken care of by using 32000/-32000 for znear/zfar respectively
  d3dmgr->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
  d3dmgr->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  d3dmgr->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  d3dmgr->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
  d3dmgr->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  d3dmgr->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
  d3dmgr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  d3dmgr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  draw_set_color(c_white);
}

int screen_save(string filename) //Assumes native integers are little endian
{
	string ext = enigma::image_get_format(filename);

	d3dmgr->EndShapesBatching();
	LPDIRECT3DSURFACE9 pBackBuffer;
	LPDIRECT3DSURFACE9 pDestBuffer;
	d3dmgr->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	D3DSURFACE_DESC desc;
	pBackBuffer->GetDesc(&desc);

	d3dmgr->device->CreateOffscreenPlainSurface( desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &pDestBuffer, NULL );
	d3dmgr->device->GetRenderTargetData(pBackBuffer, pDestBuffer);

	D3DLOCKED_RECT rect;

	pDestBuffer->LockRect(&rect, NULL, D3DLOCK_READONLY);
	unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
	pDestBuffer->UnlockRect();

	int ret = image_save(filename, bitmap, desc.Width, desc.Height, desc.Width, desc.Height, false);

  pBackBuffer->Release();
	pDestBuffer->Release();

	//delete[] bitmap; <- no need cause RAII
	return ret;
}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h) //Assumes native integers are little endian
{
	string ext = enigma::image_get_format(filename);

	d3dmgr->EndShapesBatching();
	LPDIRECT3DSURFACE9 pBackBuffer;
	LPDIRECT3DSURFACE9 pDestBuffer;
	d3dmgr->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	D3DSURFACE_DESC desc;
	pBackBuffer->GetDesc(&desc);

	d3dmgr->device->CreateOffscreenPlainSurface( desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &pDestBuffer, NULL );
	d3dmgr->device->GetRenderTargetData(pBackBuffer, pDestBuffer);

	D3DLOCKED_RECT rect;

	pDestBuffer->LockRect(&rect, NULL, D3DLOCK_READONLY);
	unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
	pDestBuffer->UnlockRect();

	int ret = image_save(filename, bitmap, w, h, desc.Width, desc.Height, false);

  pBackBuffer->Release();
	pDestBuffer->Release();

  return ret;
}

void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height) {
  x = (x / window_get_region_width()) * window_get_region_width_scaled();
  y = (y / window_get_region_height()) * window_get_region_height_scaled();
  width = (width / window_get_region_width()) * window_get_region_width_scaled();
  height = (height / window_get_region_height()) * window_get_region_height_scaled();
  gs_scalar sx, sy;
  sx = (window_get_width() - window_get_region_width_scaled()) / 2;
  sy = (window_get_height() - window_get_region_height_scaled()) / 2;
	D3DVIEWPORT9 pViewport = { sx + x, sy + y, width, height, 0, 1.0f };
	d3dmgr->SetViewport(&pViewport);
}

//TODO: These need to be in some kind of General
void display_set_gui_size(unsigned int width, unsigned int height) {
	enigma::gui_width = width;
	enigma::gui_height = height;
}

}
