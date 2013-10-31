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

#ifndef DIRECTX9DEVICEMANAGER
#define DIRECTX9DEVICEMANAGER

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <dxerr9.h>
#include "Graphics_Systems/Direct3D9/Direct3D9Headers.h"
#include "Graphics_Systems/General/GSmodel.h"
#include "Platforms/Win32/WINDOWSmain.h"
using namespace enigma_user;

#include <vector>
#include <map>
using std::vector;
using std::map;

extern LPDIRECT3D9 d3dobj;    // the pointer to our Direct3D interface
extern LPD3DXSPRITE dsprite; // sprite batching object used to render sprites, backgrounds, and fonts

class DeviceManager {
private:
LPDIRECT3DTEXTURE9 bound_texture;

//Keep track of current shaders.
LPDIRECT3DPIXELSHADER9	pixelShader;
LPDIRECT3DVERTEXSHADER9	vertexShader;

// This is used for resetting the render target if it is changed
LPDIRECT3DSURFACE9 pBackBuffer;
LPDIRECT3DSURFACE9 pRenderTarget;

int shapes_d3d_model;
int shapes_d3d_texture;

bool renderingScene;

protected:
  typedef map<D3DTEXTURESTAGESTATETYPE, DWORD> textureStateStageCache;
  typedef map<D3DSAMPLERSTATETYPE, DWORD> SamplerStateCache;
   map<D3DRENDERSTATETYPE, DWORD> cacheRenderStates;    /// cached RenderStates
   vector<textureStateStageCache> vecCacheTextureStates;       /// cached TextureStage States
   vector<SamplerStateCache> vecCacheSamplerStates;            /// cached SamplerStage States
   map<DWORD, D3DLIGHT9> cacheLightStates; /// cached Light States
   map<DWORD, BOOL> cacheLightEnable; /// cached Light States
  
public:
LPDIRECT3DDEVICE9 device;    // the pointer to the device class

DeviceManager() {
	shapes_d3d_model = -1;
	shapes_d3d_texture = -1;
}

~DeviceManager() {

}

// Event fired whenever a projection change occurs
void ProjectionChanged() {

}

// Reapply the render states and other stuff to the device.
void RestoreState() {

	device->SetTexture(0, bound_texture);
	device->SetVertexShader(vertexShader);
	device->SetPixelShader(pixelShader);
	
	// Cached Render States
	map< D3DRENDERSTATETYPE, DWORD >::iterator it = cacheRenderStates.begin();
    while (it != cacheRenderStates.end()) {
		device->SetRenderState(it->first, it->second);
		it++;
	}
	
	// Cached Sampler States
	vector<SamplerStateCache>::iterator samplerCache = vecCacheSamplerStates.begin();
	unsigned i = 0;
	while (samplerCache != vecCacheSamplerStates.end()) {
		map< D3DSAMPLERSTATETYPE, DWORD >::iterator cit = (*samplerCache).begin();
		while (cit != (*samplerCache).end()) {
			device->SetSamplerState((DWORD)i, cit->first, cit->second);
			cit++;
		}
		samplerCache++; i++;
	}
	
	// Cached Lights
	map< DWORD, D3DLIGHT9 >::iterator lit = cacheLightStates.begin();
    while (lit != cacheLightStates.end()) {
		device->SetLight(lit->first, &lit->second);
		lit++;
	}
	map< DWORD, BOOL >::iterator lite = cacheLightEnable.begin();
    while (lite != cacheLightEnable.end()) {
		device->LightEnable(lite->first, lite->second);
		lite++;
	}
}

void BeginSpriteBatch() {
	dsprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);
}

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

int GetShapesModel() {
	return shapes_d3d_model;
}

void BeginShapesBatching(int texId) {
					
	if (shapes_d3d_model == -1) {
		shapes_d3d_model = d3d_model_create();
	}
	if (texId != shapes_d3d_texture && shapes_d3d_texture != -1) {
		d3d_model_draw(shapes_d3d_model, shapes_d3d_texture);
		d3d_model_destroy(shapes_d3d_model);
		shapes_d3d_model = d3d_model_create();
	}
	shapes_d3d_texture = texId;
}

void EndShapesBatching() {
	if (shapes_d3d_model == -1) { return; }
	d3d_model_draw(shapes_d3d_model, shapes_d3d_texture);
	d3d_model_destroy(shapes_d3d_model);
	shapes_d3d_texture = -1;
	shapes_d3d_model = -1;
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
		EndShapesBatching();
        device->LightEnable( Index, bEnable );
		return;
    }
    if( it->second == bEnable )
        return;
    it->second = bEnable;
	EndShapesBatching();
    device->LightEnable( Index, bEnable );
}

void BeginScene() {
	device->BeginScene();
	BeginSpriteBatch();
	// Reapply the stored render states and what not
	RestoreState();
}

void EndScene() {
	EndSpriteBatch();
	EndShapesBatching();
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
		if(FAILED(hr)){
			MessageBox(enigma::hWnd,
               "Failed to reset Direct3D 9.0 Device",
			   DXGetErrorDescription9(hr), //DXGetErrorString9(hr)
               MB_ICONERROR | MB_OK);
			return;  // should probably force the game closed
		}
}

void DrawIndexedPrimitive(D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount) {
	//EndShapesBatching();
	device->DrawIndexedPrimitive(Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
}

void DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
	//EndShapesBatching();
	device->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

void DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void *pVertexStreamZeroData, UINT VertexStreamZeroStride) {
	//EndShapesBatching();
	device->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

void CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9 **ppVertexBuffer, HANDLE *pSharedHandle) {
	device->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

void CreateIndexBuffer(UINT Length, DWORD Usage,  D3DFORMAT Format,  D3DPOOL Pool, IDirect3DIndexBuffer9 **ppIndexBuffer, HANDLE *pSharedHandle) {
	device->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

void CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9 **ppTexture, HANDLE *pSharedHandle) {
	device->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
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
      MessageBox(NULL, "CreatePixelShader failed", "CRendererDX9::Create", MB_OK|MB_ICONEXCLAMATION);
      return;
    }
}

void CreateVertexShader(const DWORD *pFunction, IDirect3DVertexShader9 **ppShader) {
	HRESULT hr = device->CreateVertexShader(pFunction, ppShader);
	if (FAILED(hr)) {
      MessageBox(NULL, "CreateVertexShader failed", "CRendererDX9::Create", MB_OK|MB_ICONEXCLAMATION);
	  return;
    }
}

void GetSwapChain(UINT  iSwapChain, IDirect3DSwapChain9 **ppSwapChain) {
	HRESULT hr = device->GetSwapChain(iSwapChain, ppSwapChain);
		if(FAILED(hr)){
			MessageBox(enigma::hWnd,
               "Failed to retrieve the Direct3D 9.0 Swap Chain",
			   DXGetErrorDescription9(hr), //DXGetErrorString9(hr)
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

void SetMaterial(const D3DMATERIAL9 *pMaterial) {
	device->SetMaterial(pMaterial);
}

void SetFVF(DWORD FVF) {
	//EndShapesBatching();
	device->SetFVF(FVF);
}

void SetViewport(const D3DVIEWPORT9 *pViewport) {
	EndShapesBatching();
	device->SetViewport(pViewport);
}

void SetLight(DWORD Index, const D3DLIGHT9 *pLight) {
	// Update the light state cache
    // If the return value is 'true', the command must be forwarded to the D3D Runtime.
	map< DWORD, D3DLIGHT9 >::iterator it = cacheLightStates.find( Index );
    if ( cacheLightStates.end() == it ) {
        cacheLightStates.insert( map< DWORD, D3DLIGHT9 >::value_type(Index, *pLight) );
		EndShapesBatching();
        device->SetLight( Index, pLight );
		return;
    }
    //if( it->second == *pLight )
        //return;
    it->second = *pLight;
	EndShapesBatching();
    device->SetLight( Index, pLight );
}

void SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix) {
	// if projection EndShapesBatching();
	EndShapesBatching();
	device->SetTransform(State, pMatrix);
}

void SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {
	// Update the render state cache
    // If the return value is 'true', the command must be forwarded to the D3D Runtime.
	map< D3DRENDERSTATETYPE, DWORD >::iterator it = cacheRenderStates.find( State );
    if ( cacheRenderStates.end() == it ) {
        cacheRenderStates.insert( map< D3DRENDERSTATETYPE, DWORD >::value_type(State, Value) );
		EndShapesBatching();
        device->SetRenderState( State, Value );
    }
    if( it->second == Value )
        return;
    it->second = Value;
	EndShapesBatching();
    device->SetRenderState( State, Value );
}

void SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 *pTarget) {
	device->GetRenderTarget(0, &pBackBuffer);
	if (pBackBuffer == pTarget) { return; }
	EndShapesBatching();
	EndSpriteBatch();
	device->SetRenderTarget(RenderTargetIndex, pTarget);
	pRenderTarget = pTarget;
	BeginSpriteBatch();
}

void ResetRenderTarget() {
	device->GetRenderTarget(0, &pBackBuffer);
	if (pBackBuffer == pRenderTarget) { return; }
	EndShapesBatching();
	EndSpriteBatch();
	device->SetRenderTarget(0, pBackBuffer);
	BeginSpriteBatch();
}

void SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
	// Update the sampler state cache
    // If the return value is 'true', the command must be forwarded to the D3D Runtime.
	vector<SamplerStateCache>::iterator samplerCache = vecCacheSamplerStates.begin() + Sampler;
	if (samplerCache == vecCacheSamplerStates.end()) { 
		vecCacheSamplerStates.push_back(SamplerStateCache()); 
		samplerCache = vecCacheSamplerStates.begin() + Sampler; 
	}

	map< D3DSAMPLERSTATETYPE, DWORD >::iterator it = (*samplerCache).find( Type );

    if( (*samplerCache).end() == it)
    {
        (*samplerCache).insert( map< D3DSAMPLERSTATETYPE, DWORD >::value_type(Type, Value) );
		EndShapesBatching();
        device->SetSamplerState( Sampler, Type, Value );
		return;
    }
    if( it->second == Value )
        return;
    it->second = Value;
	EndShapesBatching();
    device->SetSamplerState( Sampler, Type, Value );
}

void SetTexture(DWORD Sampler, LPDIRECT3DTEXTURE9 pTexture) {
	if (bound_texture != pTexture) {
		//EndShapesBatching();
		device->SetTexture(Sampler, bound_texture = pTexture);
	}
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
	EndShapesBatching();
	pixelShader = shader;	
	device->SetPixelShader(shader);
}

void SetVertexShader(LPDIRECT3DVERTEXSHADER9 shader)
{
	//Nothing to do, return
	if (shader == vertexShader) return;	
	EndShapesBatching();
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

extern DeviceManager* d3ddev; // point to our device manager

#endif