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

#include "Bridges/General/DX9Context.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GStextures.h"

#include "../General/GSmodel.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"

#include "Graphics_Systems/General/GScolor_macros.h"

namespace enigma {
  bool d3dMode = false;
  bool d3dHidden = false;
  bool d3dZWriteEnable = true;
  int d3dCulling = 0;
}

D3DCULL cullingstates[3] = {
  D3DCULL_NONE, D3DCULL_CCW, D3DCULL_CW
};

D3DFILLMODE fillmodes[3] = {
  D3DFILL_POINT, D3DFILL_WIREFRAME, D3DFILL_SOLID
};

D3DCMPFUNC depthoperators[8] = {
  D3DCMP_NEVER, D3DCMP_LESS, D3DCMP_EQUAL,
  D3DCMP_LESSEQUAL, D3DCMP_GREATER, D3DCMP_NOTEQUAL,
  D3DCMP_GREATEREQUAL, D3DCMP_ALWAYS
};

D3DFOGMODE fogmodes[3] = {
    D3DFOG_EXP, D3DFOG_EXP2, D3DFOG_LINEAR
};

namespace enigma_user
{

void d3d_depth_clear() {
  d3d_depth_clear_value(1.0f);
}

void d3d_depth_clear_value(float value) {
	d3dmgr->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), value, 0);
}

void d3d_start()
{
	enigma::d3dMode = true;
	enigma::d3dCulling =  rs_none;
	d3dmgr->device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3d_set_hidden(false);

	// Enable texture repetition by default
	d3dmgr->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	d3dmgr->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	d3dmgr->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
}

void d3d_end()
{
	enigma::d3dMode = false;
	enigma::d3dCulling = rs_none;
	d3d_set_hidden(false);
}

void d3d_set_software_vertex_processing(bool software) {
	d3dmgr->SetSoftwareVertexProcessing(software);
}

void d3d_set_hidden(bool enable)
{
	//d3d_set_zwriteenable(enable);
	d3dmgr->SetRenderState(D3DRS_ZENABLE, enable); // enable/disable the z-buffer
    enigma::d3dHidden = enable;
}

void d3d_set_zwriteenable(bool enable)
{
	enigma::d3dZWriteEnable = enable;
	d3dmgr->SetRenderState(D3DRS_ZWRITEENABLE, enable);    // enable/disable z-writing
}

void d3d_set_lighting(bool enable)
{
	d3dmgr->SetRenderState(D3DRS_LIGHTING, enable);    // enable/disable the 3D lighting
}

void d3d_set_fog(bool enable, int color, double start, double end)
{
  d3d_set_fog_enabled(enable);
  d3d_set_fog_color(color);
  d3d_set_fog_start(start);
  d3d_set_fog_end(end);
  d3d_set_fog_hint(rs_nicest);
  d3d_set_fog_mode(rs_linear);
}

void d3d_set_fog_enabled(bool enable)
{
	d3dmgr->SetRenderState(D3DRS_FOGENABLE, enable);
	d3dmgr->SetRenderState(D3DRS_RANGEFOGENABLE, enable);
}

void d3d_set_fog_mode(int mode)
{
	d3dmgr->SetRenderState(D3DRS_FOGTABLEMODE, fogmodes[mode]);
	d3dmgr->SetRenderState(D3DRS_FOGVERTEXMODE, fogmodes[mode]);
}

void d3d_set_fog_hint(int mode) {

}

void d3d_set_fog_color(int color)
{
	d3dmgr->SetRenderState(D3DRS_FOGCOLOR,
                    D3DCOLOR_COLORVALUE(__GETR(color), __GETG(color), __GETB(color), 1.0f)); // Highest 8 bits are not used.
}

void d3d_set_fog_start(double start)
{
	float fFogStart = start;
	d3dmgr->SetRenderState(D3DRS_FOGSTART,*(DWORD*)(&fFogStart));
}

void d3d_set_fog_end(double end)
{
  float fFogEnd = end;
  d3dmgr->SetRenderState(D3DRS_FOGEND,*(DWORD*)(&fFogEnd));
}

void d3d_set_fog_density(double density)
{
	d3dmgr->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&density));
}

void d3d_set_culling(int mode)
{
	enigma::d3dCulling = mode;
	d3dmgr->SetRenderState(D3DRS_CULLMODE, cullingstates[mode]);
}

bool d3d_get_mode()
{
    return enigma::d3dMode;
}

bool d3d_get_hidden()
{
    return enigma::d3dHidden;
}

int d3d_get_culling() {
	return enigma::d3dCulling;
}

void d3d_set_fill_mode(int fill)
{
	d3dmgr->SetRenderState(D3DRS_FILLMODE, fillmodes[fill]);
}

void d3d_set_line_width(float value) {

}

void d3d_set_point_size(float value) {
	d3dmgr->SetRenderState(D3DRS_POINTSIZE, value);
}

void d3d_set_depth_operator(int mode) {
	d3dmgr->SetRenderState(D3DRS_ZFUNC, depthoperators[mode]);
}

void d3d_set_depth(double dep)
{

}

void d3d_set_shading(bool smooth)
{
	d3dmgr->SetRenderState(D3DRS_SHADEMODE, smooth?D3DSHADE_GOURAUD:D3DSHADE_FLAT);
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

    void light_update_positions()
    {
        map<int, posi>::iterator end = ind_pos.end();
        for (map<int, posi>::iterator it = ind_pos.begin(); it != end; it++) {
            const posi pos1 = (*it).second;
            const float pos[4] = {pos1.x, pos1.y, pos1.z, pos1.w};
            //glLightfv(GL_LIGHT0+(*it).first, GL_POSITION, pos);
        }
    }

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
			d3dmgr->GetDeviceCaps(&caps);
            if (ms >= caps.MaxActiveLights)
                return false;

            light_ind.insert(pair<int,int>(id, ms));
            ind_pos.insert(pair<int,posi>(ms, posi(-dx, -dy, -dz, 0.0f)));
        }

		D3DLIGHT9 light;    // create the light struct

		ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
		light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
		light.Diffuse = D3DXCOLOR(__GETR(col), __GETR(col), __GETB(col), 1.0f);    // set the light's color
		light.Direction = D3DXVECTOR3(dx, dy, dz);

		d3dmgr->SetLight(ms, &light);    // send the light struct properties to nth light

		D3DMATERIAL9 material;
		ZeroMemory(&material, sizeof(D3DMATERIAL9));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		d3dmgr->SetMaterial(&material);

		light_update_positions();
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
			d3dmgr->GetDeviceCaps(&caps);
            if (ms >= caps.MaxActiveLights)
                return false;

            light_ind.insert(pair<int,int>(id, ms));
            ind_pos.insert(pair<int,posi>(ms, posi(x, y, z, 1)));
        }
		D3DLIGHT9 light;    // create the light struct

		ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
		light.Type = D3DLIGHT_POINT;    // make the light type 'directional light'
		light.Diffuse = D3DXCOLOR(__GETR(col), __GETG(col), __GETB(col), 1.0f);    // set the light's color
		light.Position = D3DXVECTOR3(x, y, z);
		light.Range = range;
		light.Attenuation0 = 1.0f;    // no constant inverse attenuation
		light.Attenuation1 = 0.0f;    // only .125 inverse attenuation
		light.Attenuation2 = 0.0f;    // no square inverse attenuation
	    //light.Phi = D3DXToRadian(360.0f);    // set the outer cone to 360 degrees
		//light.Theta = D3DXToRadian(360.0f);    // set the inner cone to 360 degrees
		light.Falloff = 1.0f;    // use the typical falloff

		d3dmgr->SetLight(ms, &light);    // send the light struct properties to nth light

		D3DMATERIAL9 material;
		ZeroMemory(&material, sizeof(D3DMATERIAL9));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		d3dmgr->SetMaterial(&material);

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
			d3dmgr->GetDeviceCaps(&caps);
            if (ms >= caps.MaxActiveLights)
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
			d3dmgr->GetDeviceCaps(&caps);
            if (ms >= caps.MaxActiveLights)
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
    return d3d_lighting.light_define_direction(id, dx, dy, dz, col);
}

bool d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
{
    return d3d_lighting.light_define_point(id, x, y, z, range, col);
}

bool d3d_light_define_specularity(int id, int r, int g, int b, double a)
{
    return d3d_lighting.light_define_specularity(id, r, g, b, a);
}

void d3d_light_specularity(int facemode, int r, int g, int b, double a)
{
  float specular[4] = {r, g, b, a};

}

void d3d_light_shininess(int facemode, int shine)
{

}

void d3d_light_define_ambient(int col)
{
	d3dmgr->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(__GETR(col), __GETG(col), __GETB(col), 1));
}

bool d3d_light_enable(int id, bool enable)
{
    return enable?d3d_lighting.light_enable(id):d3d_lighting.light_disable(id);
}


//TODO(harijs) - This seems to be broken like this. Almost works, but stencilmask needs some different value
void d3d_stencil_start_mask(){
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
  d3dmgr->SetRenderState(D3DRS_ZWRITEENABLE, true);
  d3dmgr->SetRenderState(D3DRS_COLORWRITEENABLE, true);

  d3dmgr->SetRenderState(D3DRS_STENCILMASK, 0x0);
  d3dmgr->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
}

void d3d_stencil_end_mask(){
  d3dmgr->SetRenderState(D3DRS_STENCILENABLE, false);
}

}

namespace enigma {
    void d3d_light_update_positions()
    {
        d3d_lighting.light_update_positions();
    }
}

// ***** LIGHTS END *****
