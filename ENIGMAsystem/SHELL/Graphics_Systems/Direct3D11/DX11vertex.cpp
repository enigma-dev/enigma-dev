/** Copyright (C) 2018 Robert B. Colton
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

#include "Graphics_Systems/General/GSvertex_impl.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Bridges/General/DX11Context.h"

#include <D3Dcompiler.h>

namespace {
CHAR* g_strVS =
  "struct VertexInputType {\n"
  "  float4 position : POSITION;\n"
  "  float4 color : COLOR;\n"
  "};\n"
  "struct PixelInputType {\n"
  "  float4 position : SV_POSITION;\n"
  "  float4 color : COLOR;\n"
  "};\n"
  "PixelInputType VS(VertexInputType input) {\n"
  "  PixelInputType output;\n"
  "  output.position = input.position;\n"
  "  output.color = input.color;\n"
  "  return output;\n"
  "}\n";

CHAR* g_strPS =
  "struct PixelInputType {\n"
  "  float4 position : SV_POSITION;\n"
  "  float4 color : COLOR;\n"
  "};\n"
  "float4 PS(PixelInputType input) : SV_TARGET {\n"
  "  return input.color;\n"
  "}\n";

ID3D10Blob* pBlobVS = NULL;
ID3D10Blob* pBlobPS = NULL;

D3D_PRIMITIVE_TOPOLOGY primitive_types[] = {
  static_cast<D3D_PRIMITIVE_TOPOLOGY>(0),
  D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
  D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
  D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
  D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
  D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
  static_cast<D3D_PRIMITIVE_TOPOLOGY>(0) // fans are deprecated
};

const char* semantic_names[] = { "POSITION", "COLOR", "NORMAL", "TEXCOORD", "BLENDWEIGHT",
  "BLENDINDICES", "DEPTH", "TANGENT", "BINORMAL", "FOG", "SAMPLE" };

DXGI_FORMAT dxgi_formats[] = {
  DXGI_FORMAT_R32_FLOAT,
  DXGI_FORMAT_R32G32_FLOAT,
  DXGI_FORMAT_R32G32B32_FLOAT,
  DXGI_FORMAT_R32G32B32A32_FLOAT,
  DXGI_FORMAT_R8G8B8A8_UNORM,
  DXGI_FORMAT_R8G8B8A8_UINT
};
size_t dxgi_format_sizes[] = {
  sizeof(float) * 1,
  sizeof(float) * 2,
  sizeof(float) * 3,
  sizeof(float) * 4,
  sizeof(unsigned byte) * 4,
  sizeof(unsigned byte) * 4
};

map<int, ID3D11Buffer*> vertexBufferPeers;
map<int, ID3D11Buffer*> indexBufferPeers;
}

namespace enigma {

void graphics_delete_vertex_buffer_peer(int buffer) {
  vertexBufferPeers[buffer]->Release();
  vertexBufferPeers.erase(buffer);
}

void graphics_delete_index_buffer_peer(int buffer) {
  indexBufferPeers[buffer]->Release();
  indexBufferPeers.erase(buffer);
}

void graphics_prepare_vertex_buffer(const int buffer) {
  enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];

  // if the contents of the vertex buffer are dirty then we need to update
  // our native vertex buffer object "peer"
  if (vertexBuffer->dirty) {
    ID3D11Buffer* vertexBufferPeer = NULL;
    auto it = vertexBufferPeers.find(buffer);
    size_t size = enigma_user::vertex_get_size(buffer);

    // if we have already created a native "peer" vbo for this user buffer,
    // then we have to release it if it isn't big enough to hold the new contents
    // or if it has just been frozen (so we can remove its D3DUSAGE_DYNAMIC)
    if (it != vertexBufferPeers.end()) {
      vertexBufferPeer = it->second;

      D3D11_BUFFER_DESC pDesc;
      vertexBufferPeer->GetDesc(&pDesc);

      if (size > pDesc.ByteWidth || vertexBuffer->frozen) {
        vertexBufferPeer->Release();
        vertexBufferPeer = NULL;
      }
    }

    if (!vertexBufferPeer) {
      // create either a static or dynamic vbo peer depending on if the user called
      // vertex_freeze on the buffer and initialize it with the vertex data
      D3D11_BUFFER_DESC bd;
      bd.Usage = vertexBuffer->frozen ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
      bd.ByteWidth = size;
      bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
      bd.CPUAccessFlags = vertexBuffer->frozen ? 0 : D3D11_CPU_ACCESS_WRITE;
      bd.MiscFlags = 0;
      bd.StructureByteStride = 0;
      D3D11_SUBRESOURCE_DATA initData;
      initData.pSysMem = vertexBuffer->vertices.data();
      m_device->CreateBuffer(&bd, &initData, &vertexBufferPeer);

      vertexBufferPeers[buffer] = vertexBufferPeer;
    } else {
      // update the contents of the native peer vbo on the GPU
      m_deviceContext->UpdateSubresource(vertexBufferPeer, 0, NULL, vertexBuffer->vertices.data(), 0, 0);
    }

    vertexBuffer->vertices.clear();
    vertexBuffer->dirty = false;
  }
}

inline ID3D11InputLayout* vertex_format_layout(const enigma::VertexFormat* vertexFormat, UINT &stride) {
  vector<D3D11_INPUT_ELEMENT_DESC> vertexLayoutElements(vertexFormat->flags.size());

  size_t offset = 0;
  map<int,int> useCount;
  for (size_t i = 0; i < vertexFormat->flags.size(); ++i) {
    const pair<int, int> flag = vertexFormat->flags[i];
    D3D11_INPUT_ELEMENT_DESC *vertexLayoutElement = &vertexLayoutElements[i];

    vertexLayoutElement->SemanticName = semantic_names[flag.second];
    vertexLayoutElement->SemanticIndex = useCount[flag.second]++;
    vertexLayoutElement->Format = dxgi_formats[flag.first];
    vertexLayoutElement->InputSlot = 0;
    vertexLayoutElement->AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    vertexLayoutElement->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    vertexLayoutElement->InstanceDataStepRate = 0;

    offset += dxgi_format_sizes[flag.first];
  }
  stride = offset;

  ID3D11InputLayout* vertexLayout;
  m_device->CreateInputLayout(vertexLayoutElements.data(), vertexLayoutElements.size(), pBlobVS->GetBufferPointer(),
                              pBlobVS->GetBufferSize(), &vertexLayout);

  return vertexLayout;
}

void graphics_prepare_default_shader() {
  static ID3D11VertexShader* g_pVertexShader = NULL;
  static ID3D11PixelShader* g_pPixelShader = NULL;

  if (g_pVertexShader == NULL) {
    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef DEBUG_MODE
    dwShaderFlags |= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

    ID3D10Blob* pBlobError = NULL;
    HRESULT hr;

    // create the vertex shader
    hr = D3DCompile(g_strVS, lstrlenA(g_strVS) + 1, "VS", NULL, NULL, "VS",
              "vs_4_0", dwShaderFlags, 0, &pBlobVS, &pBlobError);
    if (FAILED(hr)) {
      if (pBlobError != NULL) {
        OutputDebugStringA((CHAR*)pBlobError->GetBufferPointer());
        pBlobError->Release();
      }
    }
    m_device->CreateVertexShader(pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(),
                                NULL, &g_pVertexShader);

    // create the pixel shader
    hr = D3DCompile(g_strPS, lstrlenA(g_strPS) + 1, "PS", NULL, NULL, "PS",
              "ps_4_0", dwShaderFlags, 0, &pBlobPS, &pBlobError);
    if (FAILED(hr)) {
      if (pBlobError != NULL) {
        OutputDebugStringA((CHAR*)pBlobError->GetBufferPointer());
        pBlobError->Release();
      }
    }
    m_device->CreatePixelShader(pBlobPS->GetBufferPointer(), pBlobPS->GetBufferSize(),
                                NULL, &g_pPixelShader);
  }

  m_deviceContext->VSSetShader(g_pVertexShader, NULL, 0);
  m_deviceContext->PSSetShader(g_pPixelShader, NULL, 0);
}

}

namespace enigma_user {

void vertex_argb(int buffer, unsigned argb) {
  enigma::vertexBuffers[buffer]->vertices.push_back(argb);
}

void vertex_color(int buffer, int color, double alpha) {
  enigma::color_t finalcol = (CLAMP_ALPHA(alpha) << 24) | (COL_GET_R(color) << 16) | (COL_GET_G(color) << 8) | COL_GET_B(color);
  enigma::vertexBuffers[buffer]->vertices.push_back(finalcol);
}

void vertex_submit(int buffer, int primitive, unsigned start, unsigned count) {
  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];
  const enigma::VertexFormat* vertexFormat = enigma::vertexFormats[vertexBuffer->format];

  enigma::graphics_prepare_default_shader();
  enigma::graphics_prepare_vertex_buffer(buffer);

  UINT stride = 0;
  ID3D11InputLayout* vertexLayout = vertex_format_layout(vertexFormat, stride);
  m_deviceContext->IASetInputLayout(vertexLayout);

  UINT offset = 0;
  ID3D11Buffer* vertexBufferPeer = vertexBufferPeers[buffer];
  m_deviceContext->IASetVertexBuffers(0, 1, &vertexBufferPeer, &stride, &offset);

  m_deviceContext->IASetPrimitiveTopology(primitive_types[primitive]);
  m_deviceContext->Draw(count, start);

  vertexLayout->Release();
}

void index_submit(int buffer, int vertex, int primitive, unsigned start, unsigned count) {
  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[vertex];
  const enigma::VertexFormat* vertexFormat = enigma::vertexFormats[vertexBuffer->format];
  const enigma::IndexBuffer* indexBuffer = enigma::indexBuffers[buffer];

  enigma::graphics_prepare_default_shader();
  enigma::graphics_prepare_vertex_buffer(vertex);

  UINT stride = 0;
  ID3D11InputLayout* vertexLayout = vertex_format_layout(vertexFormat, stride);
  m_deviceContext->IASetInputLayout(vertexLayout);

  UINT offset = 0;
  ID3D11Buffer* vertexBufferPeer = vertexBufferPeers[vertex];
  m_deviceContext->IASetVertexBuffers(0, 1, &vertexBufferPeer, &stride, &offset);

  ID3D11Buffer* indexBufferPeer = indexBufferPeers[buffer];
  m_deviceContext->IASetIndexBuffer(
    indexBufferPeer,
    (indexBuffer->type == index_type_uint) ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT,
    0
  );

  m_deviceContext->IASetPrimitiveTopology(primitive_types[primitive]);
  m_deviceContext->DrawIndexed(count, start, 0);

  vertexLayout->Release();
}

}
