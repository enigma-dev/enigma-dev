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

#include <d3d9.h>

using namespace std;

using std::map;

namespace enigma {

extern bool Direct3D9Managed;

namespace dx9 {

class ContextManager {
private:

protected:
map< D3DRENDERSTATETYPE, DWORD > cacheRenderStates;                  /// cached RenderStates
map< DWORD, LPDIRECT3DTEXTURE9 > cacheTextureStates;                 /// cached Texture States
map< DWORD, map< D3DSAMPLERSTATETYPE, DWORD > > cacheSamplerStates;  /// cached Sampler States
map< DWORD, D3DLIGHT9 > cacheLightStates;                            /// cached Light States
map< DWORD, BOOL > cacheLightEnable;                                 /// cached Light States

public:
LPDIRECT3DDEVICE9 device;    // the pointer to the device class

ContextManager() {

}

~ContextManager() {

}

// Reapply the render states and other stuff to the device.
void RestoreState() {
  // Cached Texture Stage States
  map< DWORD, LPDIRECT3DTEXTURE9 >::iterator tit = cacheTextureStates.begin();
  while (tit != cacheTextureStates.end()) {
    device->SetTexture(tit->first, tit->second);
    tit++;
  }

  // Cached Render States
  map< D3DRENDERSTATETYPE, DWORD >::iterator it = cacheRenderStates.begin();
  while (it != cacheRenderStates.end()) {
    device->SetRenderState(it->first, it->second);
    it++;
  }

  // Cached Sampler States
  map< DWORD, map< D3DSAMPLERSTATETYPE, DWORD > >::iterator sit = cacheSamplerStates.begin();
  while (sit != cacheSamplerStates.end()) {
    map< D3DSAMPLERSTATETYPE, DWORD >::iterator secit = sit->second.begin();
    while (secit != sit->second.end()) {
      device->SetSamplerState(sit->first, secit->first, secit->second);
      secit++;
    }
    sit++;
  }

  // Cached Lights
  map< DWORD, BOOL >::iterator lite = cacheLightEnable.begin();
  while (lite != cacheLightEnable.end()) {
    device->LightEnable(lite->first, lite->second);
    lite++;
  }
  return;
  map< DWORD, D3DLIGHT9 >::iterator lit = cacheLightStates.begin();
  while (lit != cacheLightStates.end()) {
    device->SetLight(lit->first, &lit->second);
    lit++;
  }
}

void LightEnable(DWORD Index, BOOL bEnable) {
  // Update the light state cache
  // If the return value is 'true', the command must be forwarded to the D3D Runtime.
  map< DWORD, BOOL >::iterator it = cacheLightEnable.find(Index);
  if (cacheLightEnable.end() == it) {
    cacheLightEnable.insert(map< DWORD, BOOL >::value_type(Index, bEnable));
    device->LightEnable(Index, bEnable);
    return;
  }
  if (it->second == bEnable)
    return;
  it->second = bEnable;
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
  return;
  // Update the light state cache
  // If the return value is 'true', the command must be forwarded to the D3D Runtime.
  map< DWORD, D3DLIGHT9 >::iterator it = cacheLightStates.find(Index);
  if (cacheLightStates.end() == it) {
    cacheLightStates.insert(map< DWORD, D3DLIGHT9 >::value_type(Index, *pLight));
    device->SetLight(Index, pLight);
    return;
  }
  //if( it->second == *pLight )
    //return;
  it->second = *pLight;
  device->SetLight(Index, pLight);
}

void SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix) {
  device->SetTransform(State, pMatrix);
}

void SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {
  // Update the render state cache
  // If the return value is 'true', the command must be forwarded to the D3D Runtime.
  map< D3DRENDERSTATETYPE, DWORD >::iterator it = cacheRenderStates.find(State);
  if (cacheRenderStates.end() == it) {
    cacheRenderStates.insert(map< D3DRENDERSTATETYPE, DWORD >::value_type(State, Value));
    device->SetRenderState(State, Value);
  }
  if (it->second == Value)
    return;
  it->second = Value;
  device->SetRenderState(State, Value);
}

void SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
  // Update the render state cache
  // If the return value is 'true', the command must be forwarded to the D3D Runtime.
  typedef map<D3DSAMPLERSTATETYPE, DWORD> innerType;
  map< DWORD, map< D3DSAMPLERSTATETYPE, DWORD >  >::iterator it = cacheSamplerStates.find(Sampler);
  if (it == cacheSamplerStates.end()) {
    map<DWORD, innerType> outer;
    innerType inner;
    inner.insert(pair<D3DSAMPLERSTATETYPE, DWORD>(Type, Value));
    cacheSamplerStates.insert(pair<DWORD, innerType>(Sampler, inner));
    device->SetSamplerState( Sampler, Type, Value );
  }
  map< D3DSAMPLERSTATETYPE, DWORD >::iterator sit = it->second.find(Type);
  if (sit != it->second.end()) {
    if (sit->second == Value) {
      return;
    }
    sit->second = Value;
    device->SetSamplerState(Sampler, Type, Value);
  } else {
    it->second.insert(map< D3DSAMPLERSTATETYPE, DWORD >::value_type(Type, Value));
    device->SetSamplerState(Sampler, Type, Value);
  }
}

void SetTexture(DWORD Sampler, LPDIRECT3DTEXTURE9 pTexture) {
  // Update the render state cache
  // If the return value is 'true', the command must be forwarded to the D3D Runtime.
  map< DWORD, LPDIRECT3DTEXTURE9 >::iterator it = cacheTextureStates.find(Sampler);
  if (cacheTextureStates.end() == it) {
    cacheTextureStates.insert(map< DWORD, LPDIRECT3DTEXTURE9 >::value_type(Sampler, pTexture));
    device->SetTexture(Sampler, pTexture);
  }
  if (it->second == pTexture)
    return;
  it->second = pTexture;
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
