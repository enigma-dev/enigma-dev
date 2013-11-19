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

#include <string>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <dxerr9.h>
using namespace std;

#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/Win32/WINDOWSmain.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/platforms_mandatory.h"
#include "Universal_System/roomsystem.h"
#include "Graphics_Systems/graphics_mandatory.h"

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

namespace enigma
{
	
    void EnableDrawing (HGLRC *hRC)
    {
		
    }
	
	void WindowResized() {
		if (d3dmgr == NULL) { return; }
		IDirect3DSwapChain9 *sc;
		d3dmgr->GetSwapChain(0, &sc);
		D3DPRESENT_PARAMETERS d3dpp;
		sc->GetPresentParameters(&d3dpp);
		d3dpp.BackBufferWidth = enigma_user::window_get_region_width_scaled();
		d3dpp.BackBufferHeight = enigma_user::window_get_region_height_scaled();
		sc->Release();
		
		d3dmgr->Reset(&d3dpp);
	}

    void DisableDrawing (HWND hWnd, HDC hDC, HGLRC hRC)
    {
		d3dmgr->Release();    // close and release the 3D device
		d3dobj->Release();    // close and release Direct3D
    }
}

#include "Universal_System/roomsystem.h"

namespace enigma_user 
{

void screen_refresh() {
    window_set_caption(room_caption);
    enigma::update_mouse_variables();

}

void set_synchronization(bool enable) //TODO: Needs to be rewritten
{

}  

}