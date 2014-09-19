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
#include "Graphics_Systems/Direct3D9/DX9SurfaceStruct.h"
#include "Bridges/General/DX9Context.h"
#include "Graphics_Systems/General/GScolors.h"

// global declarations
LPDIRECT3D9 d3dobj;    // the pointer to our Direct3D interface
ContextManager* d3dmgr;    // the pointer to the device class

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

namespace enigma
{
	extern bool forceSoftwareVertexProcessing;
	
  void EnableDrawing (HGLRC *hRC)
  {
    d3dmgr = new ContextManager();
    HRESULT hr;
    
    D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information
    d3dobj = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface
    D3DFORMAT format = D3DFMT_A8R8G8B8; //For simplicity we'll hard-code this for now.
    
    ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
    d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
    int ww = window_get_width(),
        wh = window_get_height();
		d3dpp.BackBufferWidth = ww <= 0 ? 1 : ww;
		d3dpp.BackBufferHeight = wh <= 0 ? 1 : wh;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; // 0 Levels of multi-sampling
    d3dpp.MultiSampleQuality = 0;                //No multi-sampling
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;  // Throw away previous frames, we don't need them
    d3dpp.hDeviceWindow = hWnd;  // This is our main (and only) window
    d3dpp.Flags = NULL;            // No flags to set
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //Default Refresh Rate
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   //Present the frame immediately
    d3dpp.BackBufferCount = 1;  //We only need a single back buffer
    d3dpp.BackBufferFormat = format;      //Display format
    d3dpp.EnableAutoDepthStencil = TRUE; // Automatic depth stencil buffer
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; //32-bit zbuffer 24bits for depth 8 for stencil buffer
    // create a device class using this information and information from the d3dpp stuct
    DWORD behaviors = D3DCREATE_MIXED_VERTEXPROCESSING;
    if (forceSoftwareVertexProcessing) {
      behaviors = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
    hr = d3dobj->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      behaviors,
                      &d3dpp,
                      &d3dmgr->device);
    if (FAILED(hr)) {
      MessageBox(hWnd,
               "Failed to create Direct3D 9.0 Device",
         DXGetErrorDescription9(hr), //DXGetErrorString9(hr)
               MB_ICONERROR | MB_OK);
         return; // should probably force the game closed
    }
		
		d3dmgr->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE); 
		
		enigma_user::display_aa = 0;
		for (int i = 16; i > 1; i--) {
			if (SUCCEEDED(d3dobj->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, format, TRUE, (D3DMULTISAMPLE_TYPE)((int)D3DMULTISAMPLE_NONE + i), NULL))) {
				enigma_user::display_aa += i;
			}
		}
		
  }
    
  void OnDeviceLost() {
    for (vector<Surface*>::iterator it = Surfaces.begin(); it != Surfaces.end(); it++) {
      (*it)->OnDeviceLost();
    }
  }
  
  void OnDeviceReset() {
    for (vector<Surface*>::iterator it = Surfaces.begin(); it != Surfaces.end(); it++) {
      (*it)->OnDeviceReset();
    }
  }
	
	void WindowResized() {
		if (d3dmgr == NULL) { return; }
		IDirect3DSwapChain9 *sc;
		d3dmgr->GetSwapChain(0, &sc);
		D3DPRESENT_PARAMETERS d3dpp;
		sc->GetPresentParameters(&d3dpp);
    int ww = window_get_width(),
        wh = window_get_height();
		d3dpp.BackBufferWidth = ww <= 0 ? 1 : ww;
		d3dpp.BackBufferHeight = wh <= 0 ? 1 : wh;
		sc->Release();
    OnDeviceLost();
		d3dmgr->Reset(&d3dpp);
    OnDeviceReset();
    
    // clear the window color, viewport does not need set because backbuffer was just recreated
    enigma_user::draw_clear(enigma_user::window_get_color());
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
int display_aa = 0;

void display_reset(int samples, bool vsync) {
	if (d3dmgr == NULL) { return; }
	IDirect3DSwapChain9 *sc;
	d3dmgr->GetSwapChain(0, &sc);
	D3DPRESENT_PARAMETERS d3dpp;
	sc->GetPresentParameters(&d3dpp);
	if (vsync) {
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;   //Present the frame immediately
	} else {
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   //Present the frame immediately
	}
	d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)((int)D3DMULTISAMPLE_NONE + samples); // Levels of multi-sampling
	d3dpp.MultiSampleQuality = 0;                //No multi-sampling
	if (samples) {
		d3dmgr->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE); 
	} else {
		d3dmgr->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE); 
	}
	sc->Release();

  enigma::OnDeviceLost();
	d3dmgr->Reset(&d3dpp);
  enigma::OnDeviceReset();
}

void screen_refresh() {
  window_set_caption(room_caption);
  enigma::update_mouse_variables();
  d3dmgr->Present(NULL, NULL, NULL, NULL);
}

void set_synchronization(bool enable) //TODO: Needs to be rewritten
{
	if (d3dmgr == NULL) { return; }
	IDirect3DSwapChain9 *sc;
	d3dmgr->GetSwapChain(0, &sc);
	D3DPRESENT_PARAMETERS d3dpp;
	sc->GetPresentParameters(&d3dpp);
	if (enable) {
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;   //Present the frame immediately
	} else {
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   //Present the frame immediately
	}
	sc->Release();

	d3dmgr->Reset(&d3dpp);
}  

}