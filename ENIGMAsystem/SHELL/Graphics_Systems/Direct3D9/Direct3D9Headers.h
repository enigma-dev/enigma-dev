/** Copyright (C) 2013, 2018-2019 Robert B. Colton
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
#include <cstring> // std::memcmp

#include <d3d9.h>

using namespace std;

using std::map;

namespace enigma {

extern bool Direct3D9Managed;

namespace dx9 {

class ContextManager {
private:

protected:
map< D3DRENDERSTATETYPE, DWORD > cacheRenderStates;                  /// cached render states
map< DWORD, LPDIRECT3DTEXTURE9 > cacheTextureStates;                 /// cached texture states
map< DWORD, map< D3DSAMPLERSTATETYPE, DWORD > > cacheSamplerStates;  /// cached sampler states
map< DWORD, D3DLIGHT9 > cacheLightStates;                            /// cached light states
map< DWORD, BOOL > cacheLightEnable;                                 /// cached light enable states
D3DMATERIAL9 cacheMaterial;                                          /// cached lighting material

public:
LPDIRECT3DDEVICE9 device; // pointer to the device

ContextManager() {

}

~ContextManager() {

}

// Reapply the render states and other stuff to the device.
void RestoreState() {
  // Cached Texture Stage States
  for (auto state : cacheTextureStates) {
    device->SetTexture(state.first, state.second);
  }

  // Cached Render States
  for (auto state : cacheRenderStates) {
    device->SetRenderState(state.first, state.second);
  }

  // Cached Sampler States
  for (auto sampler : cacheSamplerStates) {
    for (auto state : sampler.second) {
      device->SetSamplerState(sampler.first, state.first, state.second);
    }
  }

  // Cached Lights
  device->SetMaterial(&cacheMaterial);
  for (auto lite : cacheLightEnable) {
    device->LightEnable(lite.first, lite.second);
  }
  for (auto lite : cacheLightStates) {
    device->SetLight(lite.first, &lite.second);
  }
}

void Release() {
  device->Release();
}

HRESULT SetMaterial(const D3DMATERIAL9 *pMaterial) {
  cacheMaterial = *pMaterial;
  return device->SetMaterial(pMaterial);
}

HRESULT SetLight(DWORD Index, const D3DLIGHT9 *pLight) {
  //return device->SetLight(Index, pLight);
  // Update the light state cache
  map< DWORD, D3DLIGHT9 >::iterator it = cacheLightStates.find(Index);
  if (cacheLightStates.end() == it) {
    cacheLightStates.insert(map< DWORD, D3DLIGHT9 >::value_type(Index, *pLight));
  } else {
    if (memcmp(&it->second, pLight, sizeof(D3DLIGHT9)) == 0)
      return D3D_OK;
    it->second = *pLight;
  }
  return device->SetLight(Index, pLight);
}

HRESULT LightEnable(DWORD Index, BOOL bEnable) {
  // Update the light state cache
  map< DWORD, BOOL >::iterator it = cacheLightEnable.find(Index);
  if (cacheLightEnable.end() == it) {
    cacheLightEnable.insert(map< DWORD, BOOL >::value_type(Index, bEnable));
  } else {
    if (it->second == bEnable)
      return D3D_OK;
    it->second = bEnable;
  }

  return device->LightEnable(Index, bEnable);
}

HRESULT SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix) {
  return device->SetTransform(State, pMatrix);
}

HRESULT SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {
  // Update the render state cache
  map< D3DRENDERSTATETYPE, DWORD >::iterator it = cacheRenderStates.find(State);
  if (cacheRenderStates.end() == it) {
    cacheRenderStates.insert(map< D3DRENDERSTATETYPE, DWORD >::value_type(State, Value));
  } else {
    if (it->second == Value)
      return D3D_OK;
    it->second = Value;
  }

  return device->SetRenderState(State, Value);
}

HRESULT SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
  // Update the sampler state cache
  typedef map<D3DSAMPLERSTATETYPE, DWORD> innerType;
  map< DWORD, map< D3DSAMPLERSTATETYPE, DWORD >  >::iterator it = cacheSamplerStates.find(Sampler);
  if (it == cacheSamplerStates.end()) {
    cacheSamplerStates.insert(pair<DWORD, innerType>(Sampler, {pair<D3DSAMPLERSTATETYPE, DWORD>(Type, Value)}));
    return device->SetSamplerState( Sampler, Type, Value );
  }
  map< D3DSAMPLERSTATETYPE, DWORD >::iterator sit = it->second.find(Type);
  if (sit == it->second.end()) {
    it->second.insert(map< D3DSAMPLERSTATETYPE, DWORD >::value_type(Type, Value));
  } else {
    if (sit->second == Value) {
      return D3D_OK;
    }
    sit->second = Value;
  }
  return device->SetSamplerState(Sampler, Type, Value);
}

HRESULT SetTexture(DWORD Sampler, LPDIRECT3DTEXTURE9 pTexture) {
  // Update the texture state cache
  map< DWORD, LPDIRECT3DTEXTURE9 >::iterator it = cacheTextureStates.find(Sampler);
  if (cacheTextureStates.end() == it) {
    cacheTextureStates.insert(map< DWORD, LPDIRECT3DTEXTURE9 >::value_type(Sampler, pTexture));
  } else {
    if (it->second == pTexture)
      return D3D_OK;
    it->second = pTexture;
  }
  return device->SetTexture(Sampler, pTexture);
}

HRESULT SetTextureStageState(DWORD Sampler, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {
  return device->SetTextureStageState(Sampler, Type, Value);
}

};

extern ContextManager* d3dmgr; // point to our device manager

} // namespace dx9

} // namespace enigma

#endif // ENIGMA_DIRECT3D9_HEADERS_H
