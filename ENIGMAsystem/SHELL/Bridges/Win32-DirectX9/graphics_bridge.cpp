/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <string>
#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <dxerr9.h>
using namespace std;

#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/Win32/WINDOWSmain.h"
#include "Platforms/Win32/WINDOWSwindow.h"
#include "../General/DXDevice.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "dxerr9.lib")

// global declarations
LPDIRECT3D9 d3dr;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

namespace enigma
{
    void EnableDrawing (HGLRC *hRC)
    {
		
		HRESULT hr;
		
		d3dr = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface
		D3DFORMAT format = D3DFMT_R5G6B5; //For simplicity we'll hard-code this for now.
		D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information
		
		ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
		d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
		//d3dpp.BackBufferCount = 1;  //We only need a single back buffer
		//d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; //No multi-sampling
		//d3dpp.MultiSampleQuality = 0;                //No multi-sampling
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;  // Throw away previous frames, we don't need them
		//d3dpp.hDeviceWindow = hWnd;  //This is our main (and only) window
		//d3dpp.Flags = 0;            //No flags to set
		//d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //Default Refresh Rate
		//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;   //Default Presentation rate
		//d3dpp.BackBufferFormat = format;      //Display format
		//d3dpp.EnableAutoDepthStencil = FALSE; //No depth/stencil buffer
		
		// create a device class using this information and information from the d3dpp stuct
		hr = d3dr->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);
		if(FAILED(hr)){
			    // create a "Hello World" message box using MessageBox()
    MessageBox(NULL,
               DXGetErrorDescription9(hr),
               DXGetErrorString9(hr),
               MB_ICONERROR | MB_OK);
			}
    }

    void DisableDrawing (HWND hWnd, HDC hDC, HGLRC hRC)
    {
		d3ddev->Release();    // close and release the 3D device
		d3dr->Release();    // close and release Direct3D
    }
}

namespace enigma_user
{

void set_synchronization(bool enable) //TODO: Needs to be rewritten
{

}  

}

