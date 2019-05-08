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

#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSvertex_impl.h"
#include "Graphics_Systems/General/GSprimitives.h" // for enigma_user::draw_primitive_count
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Graphics_Systems/General/GSstdraw.h"

#include <map>
using std::map;

using namespace enigma::dx9;

namespace {

D3DPRIMITIVETYPE primitive_types[] = { static_cast<D3DPRIMITIVETYPE>(0), D3DPT_POINTLIST, D3DPT_LINELIST, D3DPT_LINESTRIP, D3DPT_TRIANGLELIST, D3DPT_TRIANGLESTRIP, D3DPT_TRIANGLEFAN };
D3DDECLTYPE declaration_types[] = { D3DDECLTYPE_FLOAT1, D3DDECLTYPE_FLOAT2, D3DDECLTYPE_FLOAT3, D3DDECLTYPE_FLOAT4, D3DDECLTYPE_D3DCOLOR, D3DDECLTYPE_UBYTE4 };
size_t declaration_type_sizes[] = { sizeof(float) * 1, sizeof(float) * 2, sizeof(float) * 3, sizeof(float) * 4, sizeof(unsigned char) * 4, sizeof(unsigned char) * 4 };
D3DDECLUSAGE usage_types[] = { D3DDECLUSAGE_POSITION, D3DDECLUSAGE_COLOR, D3DDECLUSAGE_NORMAL, D3DDECLUSAGE_TEXCOORD, D3DDECLUSAGE_BLENDWEIGHT,
  D3DDECLUSAGE_BLENDINDICES, D3DDECLUSAGE_DEPTH, D3DDECLUSAGE_TANGENT, D3DDECLUSAGE_BINORMAL, D3DDECLUSAGE_FOG, D3DDECLUSAGE_SAMPLE };

map<int, LPDIRECT3DVERTEXBUFFER9> vertexBufferPeers;
map<int, LPDIRECT3DINDEXBUFFER9> indexBufferPeers;
map<int, std::pair<LPDIRECT3DVERTEXDECLARATION9, size_t> > vertexFormatPeers;

} // anonymous namespace

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
    LPDIRECT3DVERTEXBUFFER9 vertexBufferPeer = NULL;
    auto it = vertexBufferPeers.find(buffer);
    size_t size = enigma_user::vertex_get_buffer_size(buffer);

    // if we have already created a native "peer" vbo for this user buffer,
    // then we have to release it if it isn't big enough to hold the new contents
    // or if it has just been frozen (so we can remove its D3DUSAGE_DYNAMIC)
    if (it != vertexBufferPeers.end()) {
      vertexBufferPeer = it->second;

      D3DVERTEXBUFFER_DESC pDesc;
      vertexBufferPeer->GetDesc(&pDesc);

      if (size > pDesc.Size || vertexBuffer->frozen) {
        vertexBufferPeer->Release();
        vertexBufferPeer = NULL;
      }
    }

    bool dynamic = (!vertexBuffer->frozen && !Direct3D9Managed);

    // create either a static or dynamic vbo peer depending on if the user called
    // vertex_freeze on the buffer
    if (!vertexBufferPeer) {
      DWORD usage = D3DUSAGE_WRITEONLY;
      if (dynamic) usage |= D3DUSAGE_DYNAMIC;

      d3ddev->CreateVertexBuffer(
        size, usage, 0, Direct3D9Managed ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT, &vertexBufferPeer, NULL
      );
      vertexBufferPeers[buffer] = vertexBufferPeer;
    }

    // copy the vertex buffer contents over to the native peer vbo on the GPU
    VOID* pVoid;
    vertexBufferPeer->Lock(0, 0, (VOID**)&pVoid, dynamic ? D3DLOCK_DISCARD : 0);
    memcpy(pVoid, vertexBuffer->vertices.data(), size);
    vertexBufferPeer->Unlock();

    vertexBuffer->clearData();
  }
}

void graphics_prepare_index_buffer(const int buffer) {
  enigma::IndexBuffer* indexBuffer = enigma::indexBuffers[buffer];

  // if the contents of the index buffer are dirty then we need to update
  // our native index buffer object "peer"
  if (indexBuffer->dirty) {
    LPDIRECT3DINDEXBUFFER9 indexBufferPeer = NULL;
    auto it = indexBufferPeers.find(buffer);
    size_t size = enigma_user::index_get_buffer_size(buffer);

    // if we have already created a native "peer" ibo for this user buffer,
    // then we have to release it if it isn't big enough to hold the new contents
    // or if it has just been frozen (so we can remove its D3DUSAGE_DYNAMIC)
    if (it != indexBufferPeers.end()) {
      indexBufferPeer = it->second;

      D3DINDEXBUFFER_DESC pDesc;
      indexBufferPeer->GetDesc(&pDesc);

      if (size > pDesc.Size || indexBuffer->frozen) {
        indexBufferPeer->Release();
        indexBufferPeer = NULL;
      }
    }

    bool dynamic = (!indexBuffer->frozen && !Direct3D9Managed);

    // create either a static or dynamic ibo peer depending on if the user called
    // index_freeze on the buffer
    if (!indexBufferPeer) {
      DWORD usage = D3DUSAGE_WRITEONLY;
      if (dynamic) usage |= D3DUSAGE_DYNAMIC;

      d3ddev->CreateIndexBuffer(
        size, usage,
        (indexBuffer->type == enigma_user::index_type_uint) ? D3DFMT_INDEX32 : D3DFMT_INDEX16,
        Direct3D9Managed ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT, &indexBufferPeer, NULL
      );
      indexBufferPeers[buffer] = indexBufferPeer;
    }

    // copy the index buffer contents over to the native peer ibo on the GPU
    VOID* pVoid;
    indexBufferPeer->Lock(0, 0, (VOID**)&pVoid, dynamic ? D3DLOCK_DISCARD : 0);
    memcpy(pVoid, indexBuffer->indices.data(), size);
    indexBufferPeer->Unlock();

    indexBuffer->clearData();
  }
}

inline LPDIRECT3DVERTEXDECLARATION9 vertex_format_declaration(const enigma::VertexFormat* vertexFormat, size_t &stride) {
  vector<D3DVERTEXELEMENT9> vertexDeclarationElements(vertexFormat->flags.size() + 1);

  WORD offset = 0;
  map<int,int> useCount;
  for (size_t i = 0; i < vertexFormat->flags.size(); ++i) {
    const pair<int, int> flag = vertexFormat->flags[i];
    D3DVERTEXELEMENT9 *vertexDeclarationElement = &vertexDeclarationElements[i];

    vertexDeclarationElement->Stream = 0;
    vertexDeclarationElement->Offset = offset;
    vertexDeclarationElement->Type = declaration_types[flag.first];
    vertexDeclarationElement->Method = D3DDECLMETHOD_DEFAULT;
    vertexDeclarationElement->Usage = usage_types[flag.second];
    vertexDeclarationElement->UsageIndex = useCount[flag.second]++;

    offset += declaration_type_sizes[flag.first];
  }
  stride = offset;
  vertexDeclarationElements[vertexFormat->flags.size()] = D3DDECL_END();

  LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration;
  d3ddev->CreateVertexDeclaration(&vertexDeclarationElements[0], &vertexDeclaration);

  return vertexDeclaration;
}

inline void graphics_apply_vertex_format(int format, size_t &stride) {
  const enigma::VertexFormat* vertexFormat = enigma::vertexFormats[format];

  auto search = vertexFormatPeers.find(format);
  LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration = NULL;
  if (search == vertexFormatPeers.end()) {
     vertexDeclaration = vertex_format_declaration(vertexFormat, stride);
     vertexFormatPeers[format] = std::make_pair(vertexDeclaration, stride);
  } else {
    vertexDeclaration = search->second.first;
    stride = search->second.second;
  }

  d3ddev->SetVertexDeclaration(vertexDeclaration);
}

} // namespace enigma

namespace enigma_user {

void vertex_argb(int buffer, unsigned argb) {
  enigma::vertexBuffers[buffer]->vertices.push_back(argb);
}

void vertex_color(int buffer, int color, double alpha) {
  enigma::color_t finalcol = (CLAMP_ALPHA(alpha) << 24) | (COL_GET_R(color) << 16) | (COL_GET_G(color) << 8) | COL_GET_B(color);
  enigma::vertexBuffers[buffer]->vertices.push_back(finalcol);
}

void vertex_submit_offset(int buffer, int primitive, unsigned offset, unsigned start, unsigned count) {
  draw_state_flush();

  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];

  enigma::graphics_prepare_vertex_buffer(buffer);

  size_t stride = 0;
  enigma::graphics_apply_vertex_format(vertexBuffer->format, stride);

  LPDIRECT3DVERTEXBUFFER9 vertexBufferPeer = vertexBufferPeers[buffer];
  d3ddev->SetStreamSource(0, vertexBufferPeer, offset, stride);

  int primitive_count = enigma_user::draw_primitive_count(primitive, count);

  d3ddev->DrawPrimitive(primitive_types[primitive], start, primitive_count);
}

void index_submit_range(int buffer, int vertex, int primitive, unsigned start, unsigned count) {
  draw_state_flush();

  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[vertex];

  enigma::graphics_prepare_vertex_buffer(vertex);
  enigma::graphics_prepare_index_buffer(buffer);

  size_t stride = 0;
  enigma::graphics_apply_vertex_format(vertexBuffer->format, stride);

  LPDIRECT3DVERTEXBUFFER9 vertexBufferPeer = vertexBufferPeers[vertex];
  d3ddev->SetStreamSource(0, vertexBufferPeer, 0, stride);

  LPDIRECT3DINDEXBUFFER9 indexBufferPeer = indexBufferPeers[buffer];
  d3ddev->SetIndices(indexBufferPeer);

  int primitive_count = enigma_user::draw_primitive_count(primitive, count);

  d3ddev->DrawIndexedPrimitive(primitive_types[primitive], 0, 0, count, start, primitive_count);
}

} // namespace enigma_user
