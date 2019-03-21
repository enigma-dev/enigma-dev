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

#ifndef ENIGMA_DIRECT3D9_HEADERS_H
#define ENIGMA_DIRECT3D9_HEADERS_H

#include <map>
#include <string>
#include <cmath>

#include <d3d9.h>
#include <windows.h>
#include <windowsx.h>

using namespace std;

using std::map;

namespace enigma {

extern HWND hWnd;
extern bool Direct3D9Managed;

namespace dx9 {

class ContextManager {
public:
LPDIRECT3DDEVICE9 device;    // the pointer to the device class

ContextManager() {

}

~ContextManager() {

}

void LightEnable(DWORD Index, BOOL bEnable) {
  device->LightEnable(Index, bEnable);
}

void Release() {
  device->Release();
}

void SetMaterial(const D3DMATERIAL9 *pMaterial) {
  device->SetMaterial(pMaterial);
}

void SetLight(DWORD Index, const D3DLIGHT9 *pLight) {
  device->SetLight(Index, pLight);
}

void SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix) {
  device->SetTransform(State, pMatrix);
}

void SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {
  device->SetRenderState(State, Value);
}

void SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
  device->SetSamplerState(Sampler, Type, Value);
}

void SetTexture(DWORD Sampler, LPDIRECT3DTEXTURE9 pTexture) {
  device->SetTexture(Sampler, pTexture);
}

void SetTextureStageState(DWORD Sampler, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {
  device->SetTextureStageState(Sampler, Type, Value);
}

};

extern ContextManager* d3dmgr; // point to our device manager

} // namespace dx9

} // namespace enigma

#endif // ENIGMA_DIRECT3D9_HEADERS_H
