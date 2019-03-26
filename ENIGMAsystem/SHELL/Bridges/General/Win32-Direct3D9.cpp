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

#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h" // for show_error()
#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/Direct3D9/DX9SurfaceStruct.h"
#include "Graphics_Systems/Direct3D9/Direct3D9Headers.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Bridges/Win32/WINDOWShandle.h" // for get_window_handle()

#include <windows.h>
#include <d3d9.h>

using namespace enigma::dx9;

namespace {

LPDIRECT3D9 d3dobj; // the pointer to our Direct3D interface

inline void get_d3d_present_params(D3DPRESENT_PARAMETERS* d3dpp) {
  IDirect3DSwapChain9 *sc;
  d3dmgr->device->GetSwapChain(0, &sc);
  sc->GetPresentParameters(d3dpp);
  sc->Release();
}

} // namespace anonymous

namespace enigma {

namespace dx9 {

ContextManager* d3dmgr; // the pointer to the device class

} // namespace dx9

extern HWND hWnd;
extern bool forceSoftwareVertexProcessing;

  void OnDeviceLost() {
    d3dmgr->device->EndScene();
    if (!Direct3D9Managed) return; // lost device only happens in managed d3d9
    for (vector<Surface*>::iterator it = Surfaces.begin(); it != Surfaces.end(); it++) {
      (*it)->OnDeviceLost();
    }
  }

  void OnDeviceReset() {
    d3dmgr->device->BeginScene();
    if (!Direct3D9Managed) return; // lost device only happens in managed d3d9
    for (vector<Surface*>::iterator it = Surfaces.begin(); it != Surfaces.end(); it++) {
      (*it)->OnDeviceReset();
    }
  }

  void Reset(D3DPRESENT_PARAMETERS *d3dpp) {
    OnDeviceLost();
    HRESULT hr = d3dmgr->device->Reset(d3dpp);
    if (FAILED(hr)) {
      show_error("Direct3D 9 Device Reset Failed", true);
    }
    // the normal, managed d3d 9.0 does not automatically restore render state
    if (Direct3D9Managed) d3dmgr->RestoreState();
    OnDeviceReset();
  }

  extern void (*WindowResizedCallback)();
  void WindowResized() {
    if (d3dmgr == NULL) { return; }
    D3DPRESENT_PARAMETERS d3dpp;
    get_d3d_present_params(&d3dpp);
    int ww = enigma_user::window_get_width(),
        wh = enigma_user::window_get_height();
    d3dpp.BackBufferWidth = ww <= 0 ? 1 : ww;
    d3dpp.BackBufferHeight = wh <= 0 ? 1 : wh;
    Reset(&d3dpp);

    // clear the window color, viewport does not need set because backbuffer was just recreated
    enigma_user::draw_clear(enigma_user::window_get_color());
  }

  void EnableDrawing(void* handle) {
    get_window_handle();
    WindowResizedCallback = &WindowResized;

    d3dmgr = new ContextManager();
    HRESULT hr;

    D3DPRESENT_PARAMETERS d3dpp; // create a struct to hold various device information
    D3DFORMAT format = D3DFMT_A8R8G8B8;

    ZeroMemory(&d3dpp, sizeof(d3dpp)); // clear out the struct for use
    d3dpp.Windowed = TRUE; // program windowed, not fullscreen
    int ww = enigma_user::window_get_width(),
        wh = enigma_user::window_get_height();
    d3dpp.BackBufferWidth = ww <= 0 ? 1 : ww;
    d3dpp.BackBufferHeight = wh <= 0 ? 1 : wh;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;                // 0 Levels of multi-sampling
    d3dpp.MultiSampleQuality = 0;                               // No multi-sampling
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                   // Throw away previous frames, we don't need them
    d3dpp.hDeviceWindow = hWnd;                                 // This is our main (and only) window
    d3dpp.Flags = 0;                                            // No flags to set
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // Default Refresh Rate
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Present the frame immediately
    d3dpp.BackBufferCount = 1;                                  // We only need a single back buffer
    d3dpp.BackBufferFormat = format;                            // Display format
    d3dpp.EnableAutoDepthStencil = TRUE;                        // Automatic depth stencil buffer
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;                // 32-bit zbuffer 24bits for depth 8 for stencil buffer
    // create a device class using this information and information from the d3dpp stuct
    DWORD behaviors = D3DCREATE_MIXED_VERTEXPROCESSING;
    if (forceSoftwareVertexProcessing) {
      behaviors = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // Manually load the d3d9.dll library to check for D3D9Ex
    HMODULE libHandle = NULL;
    libHandle = LoadLibrary("d3d9.dll");

    if (libHandle != NULL) {
      // Define a function pointer to the Direct3DCreate9Ex function.
      typedef HRESULT (WINAPI *LPDIRECT3DCREATE9EX)( UINT, void **);

      // Obtain the address of the Direct3DCreate9Ex function.
      LPDIRECT3DCREATE9EX Direct3DCreate9ExPtr = NULL;

      Direct3DCreate9ExPtr = (LPDIRECT3DCREATE9EX)GetProcAddress(libHandle, "Direct3DCreate9Ex");

      if (Direct3DCreate9ExPtr != NULL) {
        // create Direct3D 9 Ex interface
        IDirect3D9Ex* d3dexobj = NULL;
        hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &d3dexobj);

        if (d3dexobj != NULL) {
          IDirect3DDevice9Ex* d3dexdev = NULL;

          hr = d3dexobj->CreateDeviceEx(D3DADAPTER_DEFAULT,
                                        D3DDEVTYPE_HAL,
                                        hWnd,
                                        behaviors,
                                        &d3dpp,
                                        NULL,
                                        &d3dexdev);

          if (FAILED(hr)) {
            // release the d3d9 ex object and fall through to
            // try creating a regular d3d9 object instead
            // (might be Windows Vista with non-WDDM drivers)
            d3dexobj->Release();
          } else {
            d3dobj = d3dexobj;
            d3dmgr->device = d3dexdev;
            Direct3D9Managed = false;
          }
        }
      }

      // Free the library.
      FreeLibrary(libHandle);
    }

    if (Direct3D9Managed) {
      // try creating legacy Direct3D 9 interface
      d3dobj = Direct3DCreate9(D3D_SDK_VERSION);

      hr = d3dobj->CreateDevice(D3DADAPTER_DEFAULT,
                                D3DDEVTYPE_HAL,
                                hWnd,
                                behaviors,
                                &d3dpp,
                                &d3dmgr->device);

      if (FAILED(hr)) {
        show_error("Failed to create Direct3D 9.0 Device", true);
      }
    }

    d3dmgr->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

    enigma_user::display_aa = 0;
    for (int i = 16; i > 1; i--) {
      if (SUCCEEDED(d3dobj->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, format, TRUE, (D3DMULTISAMPLE_TYPE)((int)D3DMULTISAMPLE_NONE + i), NULL))) {
        enigma_user::display_aa += i;
      }
    }

    d3dmgr->device->BeginScene();
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

  void DisableDrawing(void* handle) {
    d3dmgr->device->EndScene();
    d3dmgr->Release(); // close and release the 3D device
    d3dobj->Release(); // close and release Direct3D
  }

  void ScreenRefresh() {
    d3dmgr->device->EndScene();
    d3dmgr->device->Present(NULL, NULL, NULL, NULL);
    d3dmgr->device->BeginScene();
  }
} // namespace enigma

namespace enigma_user {

void display_reset(int samples, bool vsync) {
  if (d3dmgr == NULL) { return; }
  D3DPRESENT_PARAMETERS d3dpp;
  get_d3d_present_params(&d3dpp);
  d3dpp.PresentationInterval = vsync?D3DPRESENT_INTERVAL_DEFAULT:D3DPRESENT_INTERVAL_IMMEDIATE;
  d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)((int)D3DMULTISAMPLE_NONE + samples);
  d3dpp.MultiSampleQuality = 0;
  d3dmgr->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, samples > 0);

  enigma::Reset(&d3dpp);
}

void set_synchronization(bool enable)
{
  if (d3dmgr == NULL) { return; }
  D3DPRESENT_PARAMETERS d3dpp;
  get_d3d_present_params(&d3dpp);
  d3dpp.PresentationInterval = enable?D3DPRESENT_INTERVAL_DEFAULT:D3DPRESENT_INTERVAL_IMMEDIATE;

  enigma::Reset(&d3dpp);
}

}
