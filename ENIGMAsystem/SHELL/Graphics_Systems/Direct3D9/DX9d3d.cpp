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

#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GSblend.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GSmatrix_impl.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Widget_Systems/widgets_mandatory.h"

#include "Universal_System/scalar.h"

#include <glm/gtc/type_ptr.hpp>

using namespace enigma::dx9;

namespace {

template<typename T, typename F>
T alias_cast(F raw_data) {
  T value;
  memcpy(&value, &raw_data, sizeof(value));
  return value;
}

const D3DCULL cullingstates[3] = {
  D3DCULL_NONE, D3DCULL_CCW, D3DCULL_CW
};

const D3DFILLMODE fillmodes[3] = {
  D3DFILL_POINT, D3DFILL_WIREFRAME, D3DFILL_SOLID
};

const D3DCMPFUNC depthoperators[8] = {
  D3DCMP_NEVER, D3DCMP_LESS, D3DCMP_EQUAL,
  D3DCMP_LESSEQUAL, D3DCMP_GREATER, D3DCMP_NOTEQUAL,
  D3DCMP_GREATEREQUAL, D3DCMP_ALWAYS
};

const D3DFOGMODE fogmodes[3] = {
    D3DFOG_EXP, D3DFOG_EXP2, D3DFOG_LINEAR
};

const D3DBLEND blendequivs[11] = {
  D3DBLEND_ZERO, D3DBLEND_ONE, D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCCOLOR, D3DBLEND_SRCALPHA,
  D3DBLEND_INVSRCALPHA, D3DBLEND_DESTALPHA, D3DBLEND_INVDESTALPHA, D3DBLEND_DESTCOLOR,
  D3DBLEND_INVDESTCOLOR, D3DBLEND_SRCALPHASAT
};

} // anonymous namespace

namespace enigma {

void graphics_state_flush() {
  d3dmgr->SetRenderState(D3DRS_POINTSIZE, drawPointSize);

  d3dmgr->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, msaaEnabled);
  d3dmgr->SetRenderState(D3DRS_ZENABLE, d3dHidden);
  d3dmgr->SetRenderState(D3DRS_ZWRITEENABLE, d3dZWriteEnable);
  d3dmgr->SetRenderState(D3DRS_LIGHTING, d3dLighting);
  d3dmgr->SetRenderState(D3DRS_CULLMODE, cullingstates[d3dCulling]);
  d3dmgr->SetRenderState(D3DRS_SHADEMODE, d3dShading?D3DSHADE_GOURAUD:D3DSHADE_FLAT);
  d3dmgr->SetRenderState(D3DRS_AMBIENT, *d3dLightingAmbient);

	DWORD colorWriteMask = 0;
	if (enigma::colorWriteEnable[0]) colorWriteMask |= D3DCOLORWRITEENABLE_RED;
	if (enigma::colorWriteEnable[1]) colorWriteMask |= D3DCOLORWRITEENABLE_GREEN;
	if (enigma::colorWriteEnable[2]) colorWriteMask |= D3DCOLORWRITEENABLE_BLUE;
	if (enigma::colorWriteEnable[3]) colorWriteMask |= D3DCOLORWRITEENABLE_ALPHA;
	d3dmgr->SetRenderState(D3DRS_COLORWRITEENABLE, colorWriteMask);
  d3dmgr->SetRenderState(D3DRS_SRCBLEND, blendequivs[(blendMode[0]-1)%11]);
  d3dmgr->SetRenderState(D3DRS_DESTBLEND, blendequivs[(blendMode[1]-1)%11]);
  d3dmgr->SetRenderState(D3DRS_ALPHABLENDENABLE, alphaBlend);
  d3dmgr->SetRenderState(D3DRS_ALPHATESTENABLE, alphaTest);
  d3dmgr->SetRenderState(D3DRS_ALPHAREF, alphaTestRef);

  d3dmgr->SetRenderState(D3DRS_FOGENABLE, d3dFogEnabled);
  d3dmgr->SetRenderState(D3DRS_RANGEFOGENABLE, d3dFogEnabled);
  d3dmgr->SetRenderState(D3DRS_FOGTABLEMODE, fogmodes[d3dFogMode]);
  d3dmgr->SetRenderState(D3DRS_FOGVERTEXMODE, fogmodes[d3dFogMode]);
  d3dmgr->SetRenderState(D3DRS_FOGCOLOR, *d3dFogColor);

  // NOTE: DWORD is 32 bits maximum meaning it can only hold single-precision
  // floats, so yes, we must cast double to float first too.
  // https://docs.microsoft.com/en-us/windows/desktop/direct3d9/d3drenderstatetype
  d3dmgr->SetRenderState(D3DRS_FOGSTART, alias_cast<DWORD>((float)d3dFogStart));
  d3dmgr->SetRenderState(D3DRS_FOGEND, alias_cast<DWORD>((float)d3dFogEnd));
  d3dmgr->SetRenderState(D3DRS_FOGDENSITY, alias_cast<DWORD>((float)d3dFogDensity));
}

void graphics_set_matrix(int type) {
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
  D3DTRANSFORMSTATETYPE state;
  glm::mat4 matrix;
  switch(type) {
    case enigma_user::matrix_world:
      state = D3DTS_WORLD;
      matrix = enigma::world;
      break;
    case enigma_user::matrix_view:
      state = D3DTS_VIEW;
      matrix = enigma::view;
      break;
    case enigma_user::matrix_projection:
      state = D3DTS_PROJECTION;
      matrix = enigma::projection;
      break;
    default:
      #ifdef DEBUG_MODE
      show_error("Unknown matrix type " + std::to_string(type), false);
      #endif
      return;
  }
  d3dmgr->SetTransform(state, (D3DMATRIX*)glm::value_ptr(matrix));
}

} // namespace enigma

namespace enigma_user {

void d3d_clear_depth(double value) {
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), value, 0);
}

void d3d_set_software_vertex_processing(bool software) {
	d3dmgr->device->SetSoftwareVertexProcessing(software);
}

void d3d_set_fill_mode(int fill)
{
	draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_FILLMODE, fillmodes[fill]);
}

void d3d_set_depth_operator(int mode) {
	draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_ZFUNC, depthoperators[mode]);
}

void d3d_set_depth(double dep)
{

}

void d3d_set_clip_plane(bool enable)
{
   ///TODO: Code this
}

}

namespace enigma {
  extern unsigned char currentcolor[4];
}

// ***** LIGHTS BEGIN *****
#include <map>
#include <list>
#include "Universal_System/fileio.h"

struct posi { // Homogenous point.
    gs_scalar x;
    gs_scalar y;
    gs_scalar z;
    gs_scalar w;
    posi(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar w1) : x(x1), y(y1), z(z1), w(w1){}
};

class d3d_lights
{
    map<int,int> light_ind;
    map<int,posi> ind_pos; // Internal index to position.

    public:
    d3d_lights() {}
    ~d3d_lights() {}

    bool light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
    {
	    int ms;
        if (light_ind.find(id) != light_ind.end())
        {
            ms = (*light_ind.find(id)).second;
            multimap<int,posi>::iterator it = ind_pos.find(ms);
            if (it != ind_pos.end())
                ind_pos.erase(it);
            ind_pos.insert(pair<int,posi>(ms, posi(-dx, -dy, -dz, 0.0f)));
        }
        else
        {
            ms = light_ind.size();
            D3DCAPS9 caps;
			d3dmgr->device->GetDeviceCaps(&caps);
            if (DWORD(ms) >= caps.MaxActiveLights)
                return false;

            light_ind.insert(pair<int,int>(id, ms));
            ind_pos.insert(pair<int,posi>(ms, posi(-dx, -dy, -dz, 0.0f)));
        }

		D3DLIGHT9 light = { };
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse = { COL_GET_Rf(col), COL_GET_Gf(col), COL_GET_Bf(col), 1.0f };
		light.Direction = { dx, dy, dz };

		d3dmgr->SetLight(ms, &light);    // send the light struct properties to nth light

        return true;
    }

    bool light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
    {
	    if (range <= 0.0) {
            return false;
        }
        int ms;
        if (light_ind.find(id) != light_ind.end())
        {
            ms = (*light_ind.find(id)).second;
            multimap<int,posi>::iterator it = ind_pos.find(ms);
            if (it != ind_pos.end())
                ind_pos.erase(it);
            ind_pos.insert(pair<int,posi>(ms, posi(x, y, z, 1)));
        }
        else
        {
            ms = light_ind.size();
            D3DCAPS9 caps;
			d3dmgr->device->GetDeviceCaps(&caps);
            if (DWORD(ms) >= caps.MaxActiveLights)
                return false;

            light_ind.insert(pair<int,int>(id, ms));
            ind_pos.insert(pair<int,posi>(ms, posi(x, y, z, 1)));
        }
		D3DLIGHT9 light = { };
		light.Type = D3DLIGHT_POINT;
		light.Diffuse = { COL_GET_Rf(col), COL_GET_Gf(col), COL_GET_Bf(col), 1.0f };
		light.Position = { x, y, z };
		light.Range = range;
		light.Attenuation0 = 1.0f;    // no constant inverse attenuation
		light.Attenuation1 = 0.0f;    // only .125 inverse attenuation
		light.Attenuation2 = 0.0f;    // no square inverse attenuation
		//light.Phi = gs_angle_to_radians(360.0f);    // set the outer cone to 360 degrees
		//light.Theta = gs_angle_to_radians(360.0f);    // set the inner cone to 360 degrees
		light.Falloff = 1.0f;    // use the typical falloff

		d3dmgr->SetLight(ms, &light);    // send the light struct properties to nth light

		return true;
    }

    bool light_define_specularity(int id, int r, int g, int b, double a)
    {
        int ms;
        if (light_ind.find(id) != light_ind.end())
        {
            ms = (*light_ind.find(id)).second;
        }
        else
        {
            ms = light_ind.size();
            D3DCAPS9 caps;
			d3dmgr->device->GetDeviceCaps(&caps);
            if (DWORD(ms) >= caps.MaxActiveLights)
                return false;
        }

		return true;
    }

    bool light_enable(int id)
    {
	    map<int, int>::iterator it = light_ind.find(id);
        if (it == light_ind.end()) {
            const int ms = light_ind.size();
			D3DCAPS9 caps;
			d3dmgr->device->GetDeviceCaps(&caps);
            if (DWORD(ms) >= caps.MaxActiveLights)
                return false;
            light_ind.insert(pair<int,int>(id, ms));
			d3dmgr->LightEnable(ms, TRUE);
        } else {
			d3dmgr->LightEnable((*it).second, TRUE);
        }
        return true;
    }

    bool light_disable(int id)
    {
	    map<int, int>::iterator it = light_ind.find(id);
        if (it == light_ind.end()) {
            return false;
        } else {
			d3dmgr->LightEnable((*it).second, FALSE);
        }
        return true;

    }
} d3d_lighting;

namespace enigma_user
{

bool d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
{
    draw_batch_flush(batch_flush_deferred);
    return d3d_lighting.light_define_direction(id, dx, dy, dz, col);
}

bool d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
{
    draw_batch_flush(batch_flush_deferred);
    return d3d_lighting.light_define_point(id, x, y, z, range, col);
}

bool d3d_light_define_specularity(int id, int r, int g, int b, double a)
{
    draw_batch_flush(batch_flush_deferred);
    return d3d_lighting.light_define_specularity(id, r, g, b, a);
}

void d3d_light_specularity(int facemode, int r, int g, int b, double a)
{
    draw_batch_flush(batch_flush_deferred);
}

void d3d_light_shininess(int facemode, int shine)
{
    draw_batch_flush(batch_flush_deferred);
}

bool d3d_light_enable(int id, bool enable)
{
    draw_batch_flush(batch_flush_deferred);
    return enable?d3d_lighting.light_enable(id):d3d_lighting.light_disable(id);
}


//TODO(harijs) - This seems to be broken like this. Almost works, but stencilmask needs some different value
void d3d_stencil_start_mask(){
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetRenderState(D3DRS_STENCILENABLE, true);
  d3dmgr->SetRenderState(D3DRS_ZWRITEENABLE, false);
  d3dmgr->SetRenderState(D3DRS_COLORWRITEENABLE, false);

  d3dmgr->SetRenderState(D3DRS_STENCILREF, 0x1);
  d3dmgr->SetRenderState(D3DRS_STENCILMASK, 0x1);

  d3dmgr->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

  d3dmgr->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
  d3dmgr->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
  d3dmgr->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
}

void d3d_stencil_use_mask(){
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetRenderState(D3DRS_ZWRITEENABLE, true);
  d3dmgr->SetRenderState(D3DRS_COLORWRITEENABLE, true);

  d3dmgr->SetRenderState(D3DRS_STENCILMASK, 0x0);
  d3dmgr->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
}

void d3d_stencil_end_mask(){
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetRenderState(D3DRS_STENCILENABLE, false);
}

}

// ***** LIGHTS END *****
