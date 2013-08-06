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

#include "Universal_System/scalar.h"
#include "Bridges/General/DX9Device.h"
#include "Direct3D9Headers.h"
#include "DX9shader.h"
#include <math.h>

#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */

#include <vector>
using std::vector;

#include <iostream>
#include <fstream>
using namespace std;

struct Shader {
  int type;
  LPD3DXCONSTANTTABLE       constants;
  
  Shader() 
  {
	type = enigma_user::sh_unknown;
  }
 
  ~Shader()
  {

  }
  
  virtual int Load(string fname) = 0;
  virtual void Use() = 0;
  virtual void SetConstantF(unsigned start, const float* data, unsigned count) = 0;
  
  void SetVector(string name, const D3DXVECTOR4 *vec) {
    D3DXHANDLE handle;
    handle = constants->GetConstantByName(NULL, name.c_str());
    constants->SetVector(d3ddev, handle, vec);
  }
};

struct VertexShader : public Shader {
  LPDIRECT3DVERTEXSHADER9   shader;
  LPDIRECT3DVERTEXDECLARATION9 declaration;

  VertexShader()
  {
	type = enigma_user::sh_vertex;
  }

  ~VertexShader()
  {

  }
  
  int Load(string fname) {
    DWORD dwFlags = 0; 
    //dwFlags |= D3DXSHADER_DEBUG;
    LPD3DXBUFFER pCode   = NULL;
    LPD3DXBUFFER pErrors = NULL;
    HRESULT hrErr = D3DXAssembleShaderFromFile(fname.c_str(), NULL, NULL, dwFlags, &pCode, &pErrors);
    if(pErrors)
    {
      char* szErrors = (char*)pErrors->GetBufferPointer();
      pErrors->Release();
    }
    if(FAILED(hrErr)) 
    {
      MessageBox(NULL, "vertex shader creation failed", "CRendererDX9::Create", MB_OK|MB_ICONEXCLAMATION);
	  return 1;
    }
  
    char* szCode = (char*)pCode->GetBufferPointer();
    hrErr = d3ddev->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), &shader);
    pCode->Release();
    if(FAILED(hrErr))
    {
      MessageBox(NULL,"CreateVertexShader failed","CRendererDX9::Create",MB_OK|MB_ICONEXCLAMATION);
	  return 2;
    }
	return 3;
  }
  
  void Use() {
	d3ddev->SetVertexDeclaration(declaration);
	d3ddev->SetVertexShader(shader);
  }
  
  void SetConstantF(unsigned start, const float* data, unsigned count) {
    d3ddev->SetVertexShaderConstantF(start, data, count);
  }
};

struct PixelShader : public Shader {
  LPDIRECT3DPIXELSHADER9    shader;
	
  PixelShader()
  {
	type = enigma_user::sh_pixel;
  }

  ~PixelShader()
  {

  }
  
  int Load(string fname) {
    DWORD dwFlags = 0; 
	//dwFlags |= D3DXSHADER_DEBUG;
    LPD3DXBUFFER pCode   = NULL;
    LPD3DXBUFFER pErrors = NULL;
    HRESULT hrErr = D3DXCompileShaderFromFile(fname.c_str(), NULL, NULL, "PShade", "ps_1_1", dwFlags, &pCode, &pErrors, &constants);
    if(pErrors)
    {
      char* szErrors = (char*)pErrors->GetBufferPointer();
      pErrors->Release();
    }
    if(FAILED(hrErr)) 
    {
      MessageBox(NULL, "pixel shader creation failed", "CRendererDX9::Create", MB_OK|MB_ICONEXCLAMATION);
      return false;
    }

    char* szCode = (char*)pCode->GetBufferPointer();
    hrErr  = d3ddev->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), &shader);
    pCode->Release();
    if(FAILED(hrErr))
    {
      MessageBox(NULL,"CreatePixelShader failed","CRendererDX9::Create",MB_OK|MB_ICONEXCLAMATION);
      return false;
    }
  }
  
  void Use() {
	d3ddev->SetPixelShader(shader);
  }
  
  void SetConstantF(unsigned start, const float* data, unsigned count) {
    d3ddev->SetPixelShaderConstantF(start, data, count);
  }
};

vector<Shader*> shaders(0);

namespace enigma_user
{

int hlsl_shader_create(int type)
{
  unsigned int id = shaders.size();
  switch (type) {
	case sh_vertex:
		shaders.push_back(new VertexShader());
		break;
	case sh_pixel:
		shaders.push_back(new PixelShader());
		break;
	default:
		// create unknown shader type
		break;
  }
  return id;
}

}

namespace enigma_user
{

int hlsl_shader_load(int id, string fname)
{
  shaders[id]->Load(fname);
}

void hlsl_shader_set_constantf(int id, unsigned start, const float* data, unsigned count) {
  shaders[id]->SetConstantF(start, data, count);
}

void hlsl_shader_set_vector(int id, string name, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar w) {
  D3DXVECTOR4 vec(x, y, z, w);
  shaders[id]->SetVector(name, &vec);
}

void hlsl_shader_use(int id)
{
  shaders[id]->Use();
}

void hlsl_shader_reset()
{
  d3ddev->SetVertexDeclaration(NULL);
  d3ddev->SetVertexShader(NULL);
  d3ddev->SetPixelShader(NULL);
}

void hlsl_shader_free(int id)
{
  delete shaders[id];
}

}

