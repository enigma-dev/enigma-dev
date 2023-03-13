/** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
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
<<<<<<< HEAD
#include "Bridges/Win32-Direct3D9/DX9Context.h"
=======
>>>>>>> master
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScolor_macros.h"

<<<<<<< HEAD
#include "Platforms/General/PFwindow.h"

#include "Universal_System/roomsystem.h"
=======
using namespace enigma::dx9;
>>>>>>> master

namespace enigma_user {

void draw_clear_alpha(int col, float alpha)
{
	draw_batch_flush(batch_flush_deferred);
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(COL_GET_R(col), COL_GET_G(col), COL_GET_B(col), CLAMP_ALPHA(alpha)), 1.0f, 0);
}

void draw_clear(int col)
{
	draw_batch_flush(batch_flush_deferred);
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(COL_GET_R(col), COL_GET_G(col), COL_GET_B(col)), 1.0f, 0);
}

int draw_get_msaa_maxlevel()
{
<<<<<<< HEAD
  return display_aa;
=======
  return 0; //TODO: implement
>>>>>>> master
}

bool draw_get_msaa_supported()
{
<<<<<<< HEAD
  return display_aa > 0;
}

void draw_set_msaa_enabled(bool enable)
{
	draw_batch_flush(batch_flush_deferred);
}

void draw_enable_alphablend(bool enable) {
	draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_ALPHABLENDENABLE, enable);
}

bool draw_get_alpha_test() {
	DWORD* enabled;
	d3dmgr->GetRenderState(D3DRS_ALPHATESTENABLE, enabled);
	return *enabled;
}

unsigned draw_get_alpha_test_ref_value()
{
	DWORD* val;
	d3dmgr->GetRenderState(D3DRS_ALPHAREF, val);
	return *val;
}

void draw_set_alpha_test(bool enable)
{
	draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_ALPHATESTENABLE, enable);
}

void draw_set_alpha_test_ref_value(unsigned val)
{
	draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_ALPHAREF, val);
}

void draw_set_line_pattern(int pattern, int scale)
{
	draw_batch_flush(batch_flush_deferred);
}

}

//#include <endian.h>
//TODO: Though serprex, the author of the function below, never included endian.h,
//   // Doing so is necessary for the function to work at its peak.
//   // When ENIGMA generates configuration files, one should be included here.

namespace enigma_user
{

int draw_getpixel(int x, int y)
{
    if (view_enabled)
    {
        x = x + enigma_user::view_xview[enigma_user::view_current];
        y = (y + enigma_user::view_yview[enigma_user::view_current]) - 1;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > enigma_user::view_wview[enigma_user::view_current] || y > enigma_user::view_hview[enigma_user::view_current]) return 0;
    } else {
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > enigma_user::room_width || y > enigma_user::room_height) return 0;
    }

	draw_batch_flush(batch_flush_deferred);

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
	unsigned offset = y * rect.Pitch + x * 4;
	int ret = bitmap[offset + 2] | (bitmap[offset + 1] << 8) | (bitmap[offset + 0] << 16);
	pDestBuffer->UnlockRect();

	pBackBuffer->Release();
	pDestBuffer->Release();

	return ret;
}

int draw_getpixel_ext(int x, int y)
{
    if (view_enabled)
    {
        x = x + enigma_user::view_xview[enigma_user::view_current];
        y = (y + enigma_user::view_yview[enigma_user::view_current]) - 1;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > enigma_user::view_wview[enigma_user::view_current] || y > enigma_user::view_hview[enigma_user::view_current]) return 0;
    } else {
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > enigma_user::room_width || y > enigma_user::room_height) return 0;
    }

	draw_batch_flush(batch_flush_deferred);

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
	unsigned offset = y * rect.Pitch + x * 4;
	int ret = bitmap[offset + 2] | (bitmap[offset + 1] << 8) | (bitmap[offset + 0] << 16) | (bitmap[offset + 3] << 24);
	pDestBuffer->UnlockRect();

	pBackBuffer->Release();
	pDestBuffer->Release();
	return ret;
=======
  return false; //TODO: implement
>>>>>>> master
}

} // namespace enigma_user
