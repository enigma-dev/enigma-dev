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

#ifndef ENIGMA_DIRECT3D11_HEADERS_H
#define ENIGMA_DIRECT3D11_HEADERS_H

#include <d3d11.h>

namespace enigma {

namespace dx11 {

extern ID3D11Device* m_device;
extern ID3D11DeviceContext* m_deviceContext;
extern ID3D11RenderTargetView* m_renderTargetView;
extern ID3D11DepthStencilView* m_depthStencilView;

} // namespace dx11

} // namespace enigma

#endif
