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

#include "Graphics_Systems/General/GSvertex.h"

#include "Bridges/General/DX9Context.h"

#include <map>
using std::map;

namespace enigma {

D3DPRIMITIVETYPE primitive_types[] = { static_cast<D3DPRIMITIVETYPE>(0), D3DPT_POINTLIST, D3DPT_LINELIST, D3DPT_LINESTRIP, D3DPT_TRIANGLELIST, D3DPT_TRIANGLESTRIP, D3DPT_TRIANGLEFAN };
D3DDECLTYPE declaration_types[] = { D3DDECLTYPE_FLOAT1, D3DDECLTYPE_FLOAT2, D3DDECLTYPE_FLOAT3, D3DDECLTYPE_FLOAT4, D3DDECLTYPE_D3DCOLOR, D3DDECLTYPE_UBYTE4 };
WORD declaration_type_sizes[] = { 4, 8, 12, 16, 4, 4 };
D3DDECLUSAGE usage_types[] = { D3DDECLUSAGE_POSITION, D3DDECLUSAGE_COLOR, D3DDECLUSAGE_NORMAL, D3DDECLUSAGE_TEXCOORD, D3DDECLUSAGE_BLENDWEIGHT,
  D3DDECLUSAGE_BLENDINDICES, D3DDECLUSAGE_DEPTH, D3DDECLUSAGE_TANGENT, D3DDECLUSAGE_BINORMAL, D3DDECLUSAGE_FOG, D3DDECLUSAGE_SAMPLE };

void graphics_create_vertex_buffer_peer(int buffer) {

}

void graphics_destroy_vertex_buffer_peer(int buffer) {

}

inline LPDIRECT3DVERTEXDECLARATION9 vertex_format_declaration(const enigma::VertexFormat* vertexFormat, size_t &stride) {
  vector<D3DVERTEXELEMENT9> vertexDeclarationElements(vertexFormat->flags.size() + 1);

  WORD offset = 0;
  for (size_t i = 0; i < vertexFormat->flags.size(); ++i) {
    const pair<int, int> flag = vertexFormat->flags[i];
    D3DVERTEXELEMENT9 *vertexDeclarationElement = &vertexDeclarationElements[i];

    vertexDeclarationElement->Stream = 0;
    vertexDeclarationElement->Offset = offset;
    vertexDeclarationElement->Type = declaration_types[flag.first];
    vertexDeclarationElement->Method = D3DDECLMETHOD_DEFAULT;
    vertexDeclarationElement->Usage = usage_types[flag.second];
    vertexDeclarationElement->UsageIndex = 0;

    offset += declaration_type_sizes[flag.first];
  }
  stride = offset;
  vertexDeclarationElements[vertexFormat->flags.size()] = D3DDECL_END();

  LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration;
  d3dmgr->CreateVertexDeclaration(&vertexDeclarationElements[0], &vertexDeclaration);

  return vertexDeclaration;
}

}

namespace enigma_user {

void vertex_submit(int buffer, int primitive, unsigned offset, unsigned count) {
  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];
  const enigma::VertexFormat* vertexFormat = enigma::vertexFormats[vertexBuffer->format];

  size_t stride = 0;
  LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration = vertex_format_declaration(vertexFormat, stride);
  d3dmgr->SetVertexDeclaration(vertexDeclaration);

  if (enigma::vertexBuffers[buffer]->frozen) {
    d3dmgr->DrawPrimitive(enigma::primitive_types[primitive], offset, count);
  } else {
    d3dmgr->DrawPrimitiveUP(enigma::primitive_types[primitive], 1, &vertexBuffer->vertices[offset], stride);
  }
}

}
