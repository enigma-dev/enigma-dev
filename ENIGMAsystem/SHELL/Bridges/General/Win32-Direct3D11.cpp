/** Copyright (C) 2013, 2018 Robert Colton
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
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Bridges/General/DX11Context.h"
#include "Graphics_Systems/General/GScolors.h"

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <string>
using namespace std;

IDXGISwapChain* m_swapChain;
ID3D11Device* m_device;
ID3D11DeviceContext* m_deviceContext;
ID3D11RenderTargetView* m_renderTargetView;
ID3D11Texture2D* m_depthStencilBuffer;
ID3D11DepthStencilView* m_depthStencilView;
ID3D11RasterizerState* m_rasterState;

// global declarations
ContextManager* d3dmgr;    // the pointer to the device class

namespace {
int swap_interval = 0;

void initialize_render_targets() {
  HRESULT result;

  // Get the pointer to the back buffer.
  ID3D11Texture2D* backBufferPtr;
  result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
  if (FAILED(result)) {
    show_error("Failed to obtain pointer to Direct3D11 back buffer.", true);
  }

  // Create the render target view with the back buffer pointer.
  result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
  if (FAILED(result)) {
    show_error("Failed to create Direct3D11 render target view.", true);
  }

  // Release pointer to the back buffer as we no longer need it.
  backBufferPtr->Release();
  backBufferPtr = 0;

  int screenWidth = window_get_width(),
      screenHeight = window_get_height();

  // Set up the depth buffer description
  D3D11_TEXTURE2D_DESC depthBufferDesc = { };
  depthBufferDesc.Width = screenWidth;
  depthBufferDesc.Height = screenHeight;
  depthBufferDesc.MipLevels = 1;
  depthBufferDesc.ArraySize = 1;
  depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthBufferDesc.SampleDesc.Count = 1;
  depthBufferDesc.SampleDesc.Quality = 0;
  depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depthBufferDesc.CPUAccessFlags = 0;
  depthBufferDesc.MiscFlags = 0;

  result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
  if (FAILED(result)) {
    show_error("Failed to create Direct3D11 Texture2D for depth stencil buffer.", true);
  }

  // Set up the depth stencil view description.
  D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = { };
  depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  depthStencilViewDesc.Texture2D.MipSlice = 0;

  result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
  if (FAILED(result)) {
    show_error("Failed to create Direct3D11 depth stencil view.", true);
  }

  m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

  // Setup the viewport for rendering.
  D3D11_VIEWPORT viewport = { };
  viewport.TopLeftX = 0.0f;
  viewport.TopLeftY = 0.0f;
  viewport.Width = (float)screenWidth;
  viewport.Height = (float)screenHeight;
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;

  m_deviceContext->RSSetViewports(1, &viewport);
}

} // namespace anonymous

namespace enigma
{
  extern void (*WindowResizedCallback)();
  void WindowResized() {

    // release all the old references first
    m_deviceContext->OMSetRenderTargets(0, 0, 0);
    m_renderTargetView->Release();
    m_depthStencilBuffer->Release();
    m_depthStencilView->Release();

    m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

    // recreate the render target and depth stencil buffers
    // and their related views with the new window size
    initialize_render_targets();

    // clear the window color, viewport does not need set because backbuffer was just recreated
    enigma_user::draw_clear(enigma_user::window_get_color());
  }

  void EnableDrawing(void* handle) {
    WindowResizedCallback = &WindowResized;

    d3dmgr = new ContextManager();
    int screenWidth = window_get_width(),
        screenHeight = window_get_height();

    HRESULT result;

    DXGI_SWAP_CHAIN_DESC swapChainDesc = { };
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.OutputWindow = enigma::hWnd;
    swapChainDesc.Windowed = true; // initially windowed and not fullscreen
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    UINT flags = 0;
    #ifdef DEBUG_MODE
    flags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif
    result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, &featureLevel, 1,
                                           D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
    if (FAILED(result)) {
      show_error("Failed to create Direct3D11 device and swap chain.", true);
    }

    initialize_render_targets();

    // Setup the raster description which will determine how and what polygons will be drawn.
    D3D11_RASTERIZER_DESC rasterDesc = { };
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = false;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
    if (FAILED(result)) {
      show_error("Failed to create Direct3D11 rasterizer state.", true);
    }

    m_deviceContext->RSSetState(m_rasterState);
  }
	
  void WindowResized() {
    // clear the window color, viewport does not need set because backbuffer was just recreated
    enigma_user::draw_clear(enigma_user::window_get_color());
  }

  void DisableDrawing(void* handle) {}

  void ScreenRefresh() {
    m_swapChain->Present(swap_interval, 0);
  }
}

namespace enigma_user
{
  int display_aa = 0;

  void display_reset(int samples, bool vsync) {
    swap_interval = vsync ? 1 : 0;
  }

  void set_synchronization(bool enable)
  {
    swap_interval = enable ? 1 : 0;
  }

}
