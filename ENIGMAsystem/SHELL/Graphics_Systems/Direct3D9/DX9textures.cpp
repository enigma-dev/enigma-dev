/** Copyright (C) 2013-2014 Robert B. Colton
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

#include "DX9textures_impl.h"
#include "Direct3D9Headers.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"

#include <stdio.h>
#include <string.h>

using namespace enigma::dx9;

namespace {

inline unsigned int lgpp2(unsigned int x) {//Trailing zero count. lg for perfect powers of two
	x =  (x & -x) - 1;
	x -= ((x >> 1) & 0x55555555);
	x =  ((x >> 2) & 0x33333333) + (x & 0x33333333);
	x =  ((x >> 4) + x) & 0x0f0f0f0f;
	x += x >> 8;
	return (x + (x >> 16)) & 63;
}

} // namespace anonymous

namespace enigma {

LPDIRECT3DTEXTURE9 get_texture_peer(int texid) {
  return (size_t(texid) >= textures.size() || texid < 0) ? NULL : ((DX9Texture*)textures[texid])->peer;
}

  int graphics_create_texture(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap)
  {
    LPDIRECT3DTEXTURE9 texture = NULL;

    DWORD usage = Direct3D9Managed ? 0 : D3DUSAGE_DYNAMIC;
    if (mipmap) usage |= D3DUSAGE_AUTOGENMIPMAP;
    d3dmgr->device->CreateTexture(fullwidth, fullheight, 1, usage, D3DFMT_A8R8G8B8, Direct3D9Managed ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT, &texture, 0);

    if (pxdata != nullptr) {
      D3DLOCKED_RECT rect;
      texture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD);
      // we have to respect the pitch returned by the lock because some GPU's
      // have exhibited a minimum pitch size for small textures
      // (e.g, 8x8 and 16x16 texture both have 64 pitch when created in the default pool)
      // NOTE: sometime soon we must finally do texture paging...
      for (size_t i = 0; i < height; ++i) {
        memcpy((void*)((intptr_t)rect.pBits + i * rect.Pitch), (void*)((intptr_t)pxdata + i * fullwidth * 4), width * 4);
      }
      texture->UnlockRect(0);
    }

    if (mipmap) {
      texture->GenerateMipSubLevels();
    }

    DX9Texture* textureStruct = new DX9Texture(texture);
    textureStruct->width = width;
    textureStruct->height = height;
    textureStruct->fullwidth = fullwidth;
    textureStruct->fullheight = fullheight;
    const int id = textures.size();
    textures.push_back(textureStruct);
    return id;
  }

  int graphics_duplicate_texture(int tex, bool mipmap)
  {
    unsigned w, h, fw, fh;
    w = textures[tex]->width;
    h = textures[tex]->height;
    fw = textures[tex]->fullwidth;
    fh = textures[tex]->fullheight;

    D3DLOCKED_RECT rect;

    auto peer = get_texture_peer(tex);
    peer->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);
    unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
    peer->UnlockRect(0);

    unsigned dup_tex = graphics_create_texture(w, h, fw, fh, bitmap, mipmap);
    delete[] bitmap;
    return dup_tex;
  }

  void graphics_copy_texture(int source, int destination, int x, int y)
  {
    auto sourcePeer = get_texture_peer(source);
    auto destPeer = get_texture_peer(destination);
    unsigned int sw, sh, sfw;
    sw = textures[source]->width;
    sh = textures[source]->height;
    sfw = textures[source]->fullwidth;

    D3DLOCKED_RECT rect;
    sourcePeer->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);
    unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
    sourcePeer->UnlockRect(0);

    unsigned dw, dh, w, h;
    dw = textures[destination]->width;
    dh = textures[destination]->height;
    w = (x+sw<=dw?sw:dw-x);
    h = (y+sh<=dh?sh:dh-y);
    destPeer->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);
    for (unsigned int i=0; i<h; ++i){
      memcpy(static_cast<unsigned char*>(rect.pBits)+(dw*(i+y)+x)*4, bitmap+sfw*i*4, w*4);
    }
    destPeer->UnlockRect(0);

    delete[] bitmap;
  }

  void graphics_copy_texture_part(int source, int destination, int xoff, int yoff, int w, int h, int x, int y)
  {
    auto sourcePeer = get_texture_peer(source);
    auto destPeer = get_texture_peer(destination);
    unsigned int sw, sh, sfw, sfh;
    sw = w;
    sh = h;
    sfw = textures[source]->fullwidth;
    sfh = textures[source]->fullheight;

    D3DLOCKED_RECT rect;
    sourcePeer->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);
    unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
    sourcePeer->UnlockRect(0);

    if (xoff+sw>sfw) sw = sfw-xoff;
    if (yoff+sh>sfh) sh = sfh-yoff;
    unsigned dw, dh, wi, hi;
    dw = textures[destination]->width;
    dh = textures[destination]->height;
    wi = (x+sw<=dw?sw:dw-x);
    hi = (y+sh<=dh?sh:dh-y);
    destPeer->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);
    for (unsigned int i=0; i<hi; ++i){
      memcpy(static_cast<unsigned char*>(rect.pBits)+(dw*(i+y)+x)*4, bitmap+xoff*4+sfw*(i+yoff)*4, wi*4);
    }
    destPeer->UnlockRect(0);

    delete[] bitmap;
  }

  void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex)
  {
    auto texPeer = get_texture_peer(tex);
    auto copyPeer = get_texture_peer(copy_tex);
    unsigned fw, fh, size;
    fw = textures[tex]->fullwidth;
    fh = textures[tex]->fullheight;
    size = (fh<<(lgpp2(fw)+2))|2;

    D3DLOCKED_RECT rect;

    copyPeer->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);
    unsigned char* bitmap_copy = static_cast<unsigned char*>(rect.pBits);
    copyPeer->UnlockRect(0);

    texPeer->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);
    for (unsigned int i = 3; i < size; i += 4)
        ((unsigned char*)rect.pBits)[i] = (bitmap_copy[i-3] + bitmap_copy[i-2] + bitmap_copy[i-1])/3;
    texPeer->UnlockRect(0);

    delete[] bitmap_copy;
  }

  void graphics_delete_texture(int texid)
  {
    const auto texture = (DX9Texture*)textures[texid];
    texture->peer->Release(), texture->peer = NULL;
  }

  unsigned char* graphics_get_texture_pixeldata(unsigned texture, unsigned* fullwidth, unsigned* fullheight)
  {
    auto peer = get_texture_peer(texture);
    *fullwidth = textures[texture]->fullwidth;
    *fullheight = textures[texture]->fullheight;

    D3DLOCKED_RECT rect;

    peer->LockRect( 0, &rect, NULL, D3DLOCK_READONLY);
    unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
    peer->UnlockRect(0);

    return bitmap;
  }
} // namespace enigma

namespace enigma_user {

void texture_set_priority(int texid, double prio)
{
  draw_batch_flush(batch_flush_deferred);
  // Deprecated in ENIGMA and GM: Studio
  enigma::get_texture_peer(texid)->SetPriority(prio);
}

void texture_set_stage(int stage, int texid) {
  draw_batch_flush(batch_flush_deferred);
  if (texid == -1) { d3dmgr->SetTexture(0, NULL); return; }
	d3dmgr->SetTexture(stage, enigma::get_texture_peer(texid));
	d3dmgr->SetTextureStageState(stage,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

void texture_reset() {
  draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetTexture(0, NULL);
}

void texture_set_blending(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetTextureStageState(0, D3DTSS_COLOROP, enable?D3DTOP_MODULATE:D3DTOP_DISABLE);
}

void texture_set_enabled(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
}

void texture_set_interpolation_ext(int sampler, bool enable)
{
  draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetSamplerState( sampler, D3DSAMP_MINFILTER, enable ? D3DTEXF_LINEAR : D3DTEXF_POINT );
	d3dmgr->SetSamplerState( sampler, D3DSAMP_MAGFILTER, enable ? D3DTEXF_LINEAR : D3DTEXF_POINT );
}

void texture_set_repeat_ext(int sampler, bool repeat)
{
  draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetSamplerState( sampler, D3DSAMP_ADDRESSU, repeat?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
	d3dmgr->SetSamplerState( sampler, D3DSAMP_ADDRESSV, repeat?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
	d3dmgr->SetSamplerState( sampler, D3DSAMP_ADDRESSW, repeat?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
}

void texture_set_wrap_ext(int sampler, bool wrapu, bool wrapv, bool wrapw)
{
  draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetSamplerState( sampler, D3DSAMP_ADDRESSU, wrapu?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
	d3dmgr->SetSamplerState( sampler, D3DSAMP_ADDRESSV, wrapv?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
	d3dmgr->SetSamplerState( sampler, D3DSAMP_ADDRESSW, wrapw?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
}

void texture_set_border_ext(int sampler, int r, int g, int b, double a)
{
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetSamplerState( sampler, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(r, g, b, (unsigned)(a * 255)) );
}

void texture_set_filter_ext(int sampler, int filter)
{
  draw_batch_flush(batch_flush_deferred);
  if (filter == tx_trilinear) {
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
  } else if (filter == tx_bilinear) {
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MIPFILTER, D3DTEXF_POINT );
  } else if (filter == tx_nearest) {
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MINFILTER, D3DTEXF_POINT );
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MIPFILTER, D3DTEXF_POINT );
  } else {
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MINFILTER, D3DTEXF_POINT );
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
    d3dmgr->SetSamplerState( sampler, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
  }
}

void texture_set_lod_ext(int sampler, double minlod, double maxlod, int maxlevel)
{
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetSamplerState( sampler, D3DSAMP_MAXMIPLEVEL, maxlevel );
}

bool texture_mipmapping_supported()
{
  D3DCAPS9 caps;
  d3dmgr->device->GetDeviceCaps(&caps);
  return caps.TextureCaps & D3DPTEXTURECAPS_MIPMAP;
}

bool texture_anisotropy_supported()
{
  D3DCAPS9 caps;
  d3dmgr->device->GetDeviceCaps(&caps);
  return caps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY;
}

float texture_anisotropy_maxlevel()
{
  D3DCAPS9 caps;
  d3dmgr->device->GetDeviceCaps(&caps);
  return caps.MaxAnisotropy;
}

void texture_anisotropy_filter(int sampler, gs_scalar level)
{
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetSamplerState( sampler, D3DSAMP_MAXANISOTROPY, level );
}

} // namespace enigma_user
