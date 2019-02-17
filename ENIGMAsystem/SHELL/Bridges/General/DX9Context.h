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

#ifndef DIRECTX9CONTEXTMANAGER
#define DIRECTX9CONTEXTMANAGER

#include "Graphics_Systems/Direct3D9/Direct3D9Headers.h"
#include "Graphics_Systems/General/GSmodel.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>

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
  extern bool Direct3D9Managed;
}

extern LPDIRECT3D9 d3dobj;    // the pointer to our Direct3D interface

//TODO: Replace the fixed function pipeline with shaders

class ContextManager {
private:

//Keep track of current shaders.
LPDIRECT3DPIXELSHADER9	pixelShader;
LPDIRECT3DVERTEXSHADER9	vertexShader;

// This is used for resetting the render target if it is changed
LPDIRECT3DSURFACE9 pBackBuffer;
LPDIRECT3DSURFACE9 pRenderTarget;

float last_depth;
bool hasdrawn;
int shapes_d3d_model;
int shapes_d3d_texture;

bool renderingScene;

protected:
   map< D3DRENDERSTATETYPE, DWORD > cacheRenderStates;                  /// cached RenderStates
   map< DWORD, LPDIRECT3DTEXTURE9 > cacheTextureStates;                 /// cached Texture States
   map< DWORD, map< D3DSAMPLERSTATETYPE, DWORD > > cacheSamplerStates;  /// cached Sampler States
   map< DWORD, D3DLIGHT9 > cacheLightStates;                            /// cached Light States
   map< DWORD, BOOL > cacheLightEnable;                                 /// cached Light States

public:
LPDIRECT3DDEVICE9 device;    // the pointer to the device class

ContextManager() {
	hasdrawn = false;
	shapes_d3d_model = -1;
	shapes_d3d_texture = -1;
	vertexShader = NULL;
	pixelShader = NULL;
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
	// Cached Texture Stage States
	map< DWORD, LPDIRECT3DTEXTURE9 >::iterator tit = cacheTextureStates.begin();
    while (tit != cacheTextureStates.end()) {
		device->SetTexture(tit->first, tit->second);
		tit++;
	}

	device->SetVertexShader(vertexShader);
	device->SetPixelShader(pixelShader);

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

/*
void EndSpriteBatch() {
	// Textures should be clamped when rendering 2D sprites and stuff, so memorize it.
	DWORD wrapu, wrapv, wrapw;
	device->GetSamplerState( 0, D3DSAMP_ADDRESSU, &wrapu );
	device->GetSamplerState( 0, D3DSAMP_ADDRESSV, &wrapv );
	device->GetSamplerState( 0, D3DSAMP_ADDRESSW, &wrapw );

	device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	device->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP );
	// The D3D sprite batcher uses clockwise face culling which is default but can't tell if
	// this here should memorize it and force it to CW all the time and then reset what the user had
	// or not.
	DWORD cullmode;
	device->GetRenderState(D3DRS_CULLMODE, &cullmode);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	dsprite->End();
	// And now reset the texture repetition.
	device->SetSamplerState( 0, D3DSAMP_ADDRESSU, wrapu );
	device->SetSamplerState( 0, D3DSAMP_ADDRESSV, wrapv );
	device->SetSamplerState( 0, D3DSAMP_ADDRESSW, wrapw );

	// reset the culling
	device->SetRenderState(D3DRS_CULLMODE, cullmode);
}
*/

int GetShapesModel() {
	return shapes_d3d_model;
}

void Clear(DWORD Count, const D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) {
	device->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

void LightEnable(DWORD Index, BOOL bEnable) {
	// Update the light state cache
    // If the return value is 'true', the command must be forwarded to the D3D Runtime.
	map< DWORD, BOOL >::iterator it = cacheLightEnable.find( Index );
    if( cacheLightEnable.end() == it )
    {
        cacheLightEnable.insert( map< DWORD, BOOL >::value_type(Index, bEnable) );
        device->LightEnable( Index, bEnable );
		return;
    }
    if( it->second == bEnable )
        return;
    it->second = bEnable;
    device->LightEnable( Index, bEnable );
}

void BeginScene() {
	last_depth = 0;
	device->BeginScene();
}

void EndScene() {
	device->EndScene();
}

void Present(const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion) {
	device->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

void Release() {
	device->Release();
}

void Reset(D3DPRESENT_PARAMETERS *pPresentationParameters) {
	HRESULT hr = device->Reset(pPresentationParameters);
	if (FAILED(hr)) {
		MessageBox(
			enigma::hWnd,
			TEXT("Device Reset Failed"), TEXT("Error"),
			MB_ICONERROR | MB_OK
		);
		return;  // should probably force the game closed
	}
	// Reapply the stored render states and what not
	RestoreState();
}

void DrawIndexedPrimitive(D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount) {
	device->DrawIndexedPrimitive(Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
}

void DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
	device->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

void DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void *pVertexStreamZeroData, UINT VertexStreamZeroStride) {
	device->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9 **ppVertexBuffer, HANDLE *pSharedHandle) {
	return device->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT CreateIndexBuffer(UINT Length, DWORD Usage,  D3DFORMAT Format,  D3DPOOL Pool, IDirect3DIndexBuffer9 **ppIndexBuffer, HANDLE *pSharedHandle) {
	return device->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

void CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9 **ppTexture, HANDLE *pSharedHandle) {
	HRESULT hr = device->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
  	if (FAILED(hr)) {
      MessageBox(NULL, TEXT("CreateTexture Failed"), TEXT("Error"), MB_OK|MB_ICONEXCLAMATION);
	  return;
    }
}

void CreateRenderTarget(UINT Width,  UINT Height,  D3DFORMAT Format,  D3DMULTISAMPLE_TYPE MultiSample,
DWORD MultisampleQuality,  BOOL Lockable, IDirect3DSurface9 **ppSurface, HANDLE *pSharedHandle) {
	device->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

void CreateVertexDeclaration(const D3DVERTEXELEMENT9 *pVertexElements, IDirect3DVertexDeclaration9 **ppDecl) {
	device->CreateVertexDeclaration(pVertexElements, ppDecl);
}

void CreatePixelShader(const DWORD *pFunction, IDirect3DPixelShader9 **ppShader) {
	HRESULT hr = device->CreatePixelShader(pFunction, ppShader);
	if (FAILED(hr)) {
      MessageBox(NULL, TEXT("CreatePixelShader Failed"), TEXT("Error"), MB_OK|MB_ICONEXCLAMATION);
      return;
    }
}

void CreateVertexShader(const DWORD *pFunction, IDirect3DVertexShader9 **ppShader) {
	HRESULT hr = device->CreateVertexShader(pFunction, ppShader);
	if (FAILED(hr)) {
      MessageBox(NULL, TEXT("CreateVertexShader Failed"), TEXT("Error"), MB_OK|MB_ICONEXCLAMATION);
	  return;
    }
}

void GetSwapChain(UINT  iSwapChain, IDirect3DSwapChain9 **ppSwapChain) {
	HRESULT hr = device->GetSwapChain(iSwapChain, ppSwapChain);
		if(FAILED(hr)){
			MessageBox(enigma::hWnd,
			   TEXT("GetSwapChain Failed"), TEXT("Error"),
               MB_ICONERROR | MB_OK);
			return;  // should probably force the game closed
		}
}

void GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 **ppRenderTarget) {
	device->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

void GetRenderState(D3DRENDERSTATETYPE State, DWORD *pValue) {
	device->GetRenderState(State, pValue);
}

void GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD *pValue) {
	device->GetSamplerState(Sampler, Type, pValue);
}

void GetDeviceCaps(D3DCAPS9 *pCaps) {
	device->GetDeviceCaps(pCaps);
}

void SetSoftwareVertexProcessing(bool bSoftware) {
	device->SetSoftwareVertexProcessing(bSoftware);
}

void SetMaterial(const D3DMATERIAL9 *pMaterial) {
	device->SetMaterial(pMaterial);
}

void SetFVF(DWORD FVF) {
	device->SetFVF(FVF);
}

void SetViewport(const D3DVIEWPORT9 *pViewport) {
	device->SetViewport(pViewport);
}

void SetLight(DWORD Index, const D3DLIGHT9 *pLight) {
device->SetLight( Index, pLight );
	return;
	// Update the light state cache
    // If the return value is 'true', the command must be forwarded to the D3D Runtime.
	map< DWORD, D3DLIGHT9 >::iterator it = cacheLightStates.find( Index );
    if ( cacheLightStates.end() == it ) {
        cacheLightStates.insert( map< DWORD, D3DLIGHT9 >::value_type(Index, *pLight) );
        device->SetLight( Index, pLight );
		return;
    }
    //if( it->second == *pLight )
        //return;
    it->second = *pLight;
    device->SetLight( Index, pLight );
}

void SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix) {
	device->SetTransform(State, pMatrix);
}

void SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {
	// Update the render state cache
    // If the return value is 'true', the command must be forwarded to the D3D Runtime.
	map< D3DRENDERSTATETYPE, DWORD >::iterator it = cacheRenderStates.find( State );
    if ( cacheRenderStates.end() == it ) {
        cacheRenderStates.insert( map< D3DRENDERSTATETYPE, DWORD >::value_type(State, Value) );
        device->SetRenderState( State, Value );
    }
    if( it->second == Value )
        return;
    it->second = Value;
    device->SetRenderState( State, Value );
}

void GetBackBuffer(UINT  iSwapChain, UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 **ppBackBuffer) {
	device->GetBackBuffer(iSwapChain, BackBuffer, Type, ppBackBuffer);
}

void SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 *pTarget) {
	device->GetRenderTarget(0, &pBackBuffer);
	if (pBackBuffer == pTarget) { return; }
	device->SetRenderTarget(RenderTargetIndex, pTarget);
	pRenderTarget = pTarget;
}

void ResetRenderTarget() {
	device->GetRenderTarget(0, &pBackBuffer);
	if (pBackBuffer == pRenderTarget) { return; }
	device->SetRenderTarget(0, pBackBuffer);
}

void SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
	// Update the render state cache
    // If the return value is 'true', the command must be forwarded to the D3D Runtime.
	typedef map<D3DSAMPLERSTATETYPE, DWORD> innerType;
	map< DWORD, map< D3DSAMPLERSTATETYPE, DWORD >  >::iterator it = cacheSamplerStates.find( Sampler );
    if ( it == cacheSamplerStates.end() ) {
		map<DWORD, innerType> outer;
		innerType inner;
		inner.insert(pair<D3DSAMPLERSTATETYPE, DWORD>(Type, Value));
        cacheSamplerStates.insert( pair<DWORD, innerType>(Sampler, inner) );
        device->SetSamplerState( Sampler, Type, Value );
    }
	map< D3DSAMPLERSTATETYPE, DWORD >::iterator sit = it->second.find( Type );
    if ( sit != it->second.end() ) {
		if (sit->second == Value) {
			return;
		}
		sit->second = Value;
		device->SetSamplerState( Sampler, Type, Value );
	} else {
		it->second.insert(map< D3DSAMPLERSTATETYPE, DWORD >::value_type(Type, Value));
		device->SetSamplerState( Sampler, Type, Value );
	}
}

void SetTexture(DWORD Sampler, LPDIRECT3DTEXTURE9 pTexture) {
	// Update the render state cache
    // If the return value is 'true', the command must be forwarded to the D3D Runtime.
	map< DWORD, LPDIRECT3DTEXTURE9 >::iterator it = cacheTextureStates.find( Sampler );
    if ( cacheTextureStates.end() == it ) {
        cacheTextureStates.insert( map< DWORD, LPDIRECT3DTEXTURE9 >::value_type(Sampler, pTexture) );
        device->SetTexture( Sampler, pTexture );
    }
    if( it->second == pTexture )
        return;
    it->second = pTexture;
    device->SetTexture( Sampler, pTexture );
}

void SetTextureStageState(DWORD Sampler, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {
	device->SetTextureStageState(Sampler, Type, Value);
}

void SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9 *pStreamData, UINT OffsetInBytes, UINT Stride) {
	device->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

void SetVertexDeclaration(IDirect3DVertexDeclaration9 *pDecl) {
	device->SetVertexDeclaration(pDecl);
}

void SetIndices(IDirect3DIndexBuffer9 *pIndexData) {
	device->SetIndices(pIndexData);
}

void SetPixelShader(LPDIRECT3DPIXELSHADER9 shader)
{
	//Nothing to do, return
	if (shader == pixelShader) return;
	pixelShader = shader;
	device->SetPixelShader(shader);
}

void SetVertexShader(LPDIRECT3DVERTEXSHADER9 shader)
{
	//Nothing to do, return
	if (shader == vertexShader) return;
	vertexShader = shader;
	device->SetVertexShader(shader);
}

void SetVertexShaderConstantF(UINT StartRegister, const float *pConstantData, UINT Vector4fCount) {
	device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

void SetPixelShaderConstantF(UINT StartRegister, const float *pConstantData, UINT Vector4fCount) {
	device->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

};

extern ContextManager* d3dmgr; // point to our device manager

#endif
