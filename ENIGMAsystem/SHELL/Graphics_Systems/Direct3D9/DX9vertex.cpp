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
#include "Graphics_Systems/General/GSprimitives.h" // for enigma_user::draw_primitive_count
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Bridges/General/DX9Context.h"

#include <map>
using std::map;

namespace {

D3DPRIMITIVETYPE primitive_types[] = { static_cast<D3DPRIMITIVETYPE>(0), D3DPT_POINTLIST, D3DPT_LINELIST, D3DPT_LINESTRIP, D3DPT_TRIANGLELIST, D3DPT_TRIANGLESTRIP, D3DPT_TRIANGLEFAN };
D3DDECLTYPE declaration_types[] = { D3DDECLTYPE_FLOAT1, D3DDECLTYPE_FLOAT2, D3DDECLTYPE_FLOAT3, D3DDECLTYPE_FLOAT4, D3DDECLTYPE_D3DCOLOR, D3DDECLTYPE_UBYTE4 };
size_t declaration_type_sizes[] = { sizeof(float) * 1, sizeof(float) * 2, sizeof(float) * 3, sizeof(float) * 4, sizeof(unsigned byte) * 4, sizeof(unsigned byte) * 4 };
D3DDECLUSAGE usage_types[] = { D3DDECLUSAGE_POSITION, D3DDECLUSAGE_COLOR, D3DDECLUSAGE_NORMAL, D3DDECLUSAGE_TEXCOORD, D3DDECLUSAGE_BLENDWEIGHT,
  D3DDECLUSAGE_BLENDINDICES, D3DDECLUSAGE_DEPTH, D3DDECLUSAGE_TANGENT, D3DDECLUSAGE_BINORMAL, D3DDECLUSAGE_FOG, D3DDECLUSAGE_SAMPLE };

map<int, LPDIRECT3DVERTEXBUFFER9> vertexBufferPeers;
map<int, LPDIRECT3DINDEXBUFFER9> indexBufferPeers;

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
    LPDIRECT3DVERTEXBUFFER9 vertexBufferPeer = NULL;
    auto it = vertexBufferPeers.find(buffer);
    size_t size = enigma_user::vertex_get_size(buffer);

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

    // create either a static or dynamic vbo peer depending on if the user called
    // vertex_freeze on the buffer
    if (!vertexBufferPeer) {
      d3dmgr->CreateVertexBuffer(
        size, vertexBuffer->frozen ? D3DUSAGE_WRITEONLY : (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY),
        0, D3DPOOL_DEFAULT, &vertexBufferPeer, NULL
      );
      vertexBufferPeers[buffer] = vertexBufferPeer;
    }

    // copy the vertex buffer contents over to the native peer vbo on the GPU
    VOID* pVoid;
    vertexBufferPeer->Lock(0, 0, (VOID**)&pVoid, vertexBuffer->frozen ? 0 : D3DLOCK_DISCARD);
    memcpy(pVoid, vertexBuffer->vertices.data(), size);
    vertexBufferPeer->Unlock();

    vertexBuffer->vertices.clear();
    vertexBuffer->dirty = false;
  }
}

void graphics_prepare_index_buffer(const int buffer) {
  enigma::IndexBuffer* indexBuffer = enigma::indexBuffers[buffer];

  // if the contents of the index buffer are dirty then we need to update
  // our native index buffer object "peer"
  if (indexBuffer->dirty) {
    LPDIRECT3DINDEXBUFFER9 indexBufferPeer = NULL;
    auto it = indexBufferPeers.find(buffer);
    size_t size = enigma_user::index_get_size(buffer);

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

    // create either a static or dynamic ibo peer depending on if the user called
    // index_freeze on the buffer
    if (!indexBufferPeer) {
      d3dmgr->CreateIndexBuffer(
        size, indexBuffer->frozen ? D3DUSAGE_WRITEONLY : (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY),
        (indexBuffer->type == index_type_uint) ? D3DFMT_INDEX32 : D3DFMT_INDEX16, D3DPOOL_DEFAULT, &indexBufferPeer, NULL
      );
      indexBufferPeers[buffer] = indexBufferPeer;
    }

    // copy the index buffer contents over to the native peer ibo on the GPU
    VOID* pVoid;
    indexBufferPeer->Lock(0, 0, (VOID**)&pVoid, D3DLOCK_DISCARD);
    memcpy(pVoid, indexBuffer->indices.data(), size);
    indexBufferPeer->Unlock();

    indexBuffer->indices.clear();
    indexBuffer->dirty = false;
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
  d3dmgr->CreateVertexDeclaration(&vertexDeclarationElements[0], &vertexDeclaration);

  return vertexDeclaration;
}

inline void graphics_apply_vertex_format(int format, size_t &stride) {
  const enigma::VertexFormat* vertexFormat = enigma::vertexFormats[format];

  static int last_format = -1;
  static size_t last_stride = 0;
  static LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration = nullptr;
  // cache the format and only recreate it when switching formats
  if (last_format != format) {
    last_format = format;
    if (vertexDeclaration) vertexDeclaration->Release();
    vertexDeclaration = vertex_format_declaration(vertexFormat, last_stride);
  }
  stride = last_stride;
  d3dmgr->SetVertexDeclaration(vertexDeclaration);
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

  // this is fucking temporary until we rewrite the model classes and
  // figure out a proper way to flush
  d3dmgr->EndShapesBatching();

  enigma::graphics_prepare_vertex_buffer(buffer);

  size_t stride = 0;
  enigma::graphics_apply_vertex_format(vertexBuffer->format, stride);

  LPDIRECT3DVERTEXBUFFER9 vertexBufferPeer = vertexBufferPeers[buffer];
  d3dmgr->SetStreamSource(0, vertexBufferPeer, 0, stride);

  int primitive_count = enigma_user::draw_primitive_count(primitive, count);

  d3dmgr->DrawPrimitive(primitive_types[primitive], start, primitive_count);
}

void index_submit(int buffer, int vertex, int primitive, unsigned start, unsigned count) {
  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[vertex];

  // this is fucking temporary until we rewrite the model classes and
  // figure out a proper way to flush
  d3dmgr->EndShapesBatching();

  enigma::graphics_prepare_vertex_buffer(vertex);
  enigma::graphics_prepare_index_buffer(buffer);

  size_t stride = 0;
  enigma::graphics_apply_vertex_format(vertexBuffer->format, stride);

  LPDIRECT3DVERTEXBUFFER9 vertexBufferPeer = vertexBufferPeers[vertex];
  d3dmgr->SetStreamSource(0, vertexBufferPeer, 0, stride);

  LPDIRECT3DINDEXBUFFER9 indexBufferPeer = indexBufferPeers[buffer];
  d3dmgr->SetIndices(indexBufferPeer);

  int primitive_count = enigma_user::draw_primitive_count(primitive, count);

  d3dmgr->DrawIndexedPrimitive(primitive_types[primitive], 0, 0, count, start, primitive_count);
}

}
