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

#ifndef DIRECTX11CONTEXTMANAGER
#define DIRECTX11CONTEXTMANAGER

#include "Graphics_Systems/Direct3D11/Direct3D11Headers.h"
#include "Graphics_Systems/General/GSmodel.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <windows.h>
#include <windowsx.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

#include <sstream>
#include <string.h>
#include <vector>
#include <map>

using namespace enigma_user;

using std::string;
using std::stringstream;
using std::vector;
using std::map;

namespace enigma {
	extern HWND hWnd;
}

extern bool m_vsync_enabled;
extern int m_videoCardMemory;
extern char m_videoCardDescription[128];
extern IDXGISwapChain* m_swapChain;
extern ID3D11Device* m_device;
extern ID3D11DeviceContext* m_deviceContext;
extern ID3D11RenderTargetView* m_renderTargetView;
extern ID3D11Texture2D* m_depthStencilBuffer;
extern ID3D11DepthStencilState* m_depthStencilState;
extern ID3D11DepthStencilView* m_depthStencilView;
extern ID3D11RasterizerState* m_rasterState;

//TODO: Replace the fixed function pipeline with shaders

class ContextManager {

float last_depth;
bool hasdrawn;
int shapes_d3d_model;
int shapes_d3d_texture;

bool renderingScene;

public :

ContextManager() {
	hasdrawn = false;
	shapes_d3d_model = -1;
	shapes_d3d_texture = -1;
	last_depth = 0.0f;
}

~ContextManager() {

}

float GetDepth() {
	return last_depth;
}

//TODO: Write this method so that for debugging purposes we can dump the entire render state to a text file.
void DumpState() {

}

//TODO: Write this method so that we can serialize and save the entire render state to be reloaded
void SaveState() {

}

//TODO: Write this method so that we can read and restore a previously saved render state
void LoadState() {

}

// Reapply the render states and other stuff to the device.
void RestoreState() {

}

int GetShapesModel() {
	return shapes_d3d_model;
}

};

extern ContextManager* d3dmgr; // point to our device manager

#endif
