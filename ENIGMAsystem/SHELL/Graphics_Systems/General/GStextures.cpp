
#include "GStextures.h"
#include "GSstdraw.h"
#include "Graphics_Systems/graphics_mandatory.h"

#include "Universal_System/image_formats.h"

namespace enigma {

Sampler samplers[8];

} // namespace enigma

namespace enigma_user {

int texture_add(string filename, bool mipmap) {
  unsigned int w, h, fullwidth, fullheight;
  int img_num;

  unsigned char *pxdata = enigma::image_load(filename,&w,&h,&fullwidth,&fullheight,&img_num,false);
  if (pxdata == NULL) { printf("ERROR - Failed to append sprite to index!\n"); return -1; }
  unsigned texture = enigma::graphics_create_texture(w, h, fullwidth, fullheight, pxdata, mipmap);
  delete[] pxdata;

  return texture;
}

void texture_save(int texid, string fname) {
  unsigned w = 0, h = 0;
  unsigned char* rgbdata = enigma::graphics_get_texture_pixeldata(texid, &w, &h);

  string ext = enigma::image_get_format(fname);

  enigma::image_save(fname, rgbdata, w, h, w, h, false);

  delete[] rgbdata;
}

void texture_set_stage(int stage, int texid) {
  enigma::drawStateDirty = true;
  enigma::samplers[stage].texture = texid;
}

void texture_reset() {
  enigma::drawStateDirty = true;
  enigma::samplers[0].texture = -1;
}

void texture_set_interpolation_ext(int sampler, bool enable) {
  enigma::drawStateDirty = true;
}

void texture_set_repeat_ext(int sampler, bool repeat) {
  enigma::drawStateDirty = true;
  enigma::samplers[sampler].wrapu = repeat;
  enigma::samplers[sampler].wrapv = repeat;
  enigma::samplers[sampler].wrapw = repeat;
}

void texture_set_wrap_ext(int sampler, bool wrapu, bool wrapv, bool wrapw) {
  enigma::drawStateDirty = true;
  enigma::samplers[sampler].wrapu = wrapu;
  enigma::samplers[sampler].wrapv = wrapv;
  enigma::samplers[sampler].wrapw = wrapw;
}

void texture_set_border_ext(int sampler, int r, int g, int b, double a) {
  enigma::drawStateDirty = true;
}

void texture_set_filter_ext(int sampler, int filter) {
  enigma::drawStateDirty = true;
}

void texture_set_lod_ext(int sampler, double minlod, double maxlod, int maxlevel) {
  enigma::drawStateDirty = true;
}

void texture_anisotropy_filter(int sampler, gs_scalar levels) {
  enigma::drawStateDirty = true;
}

} // namespace enigma_user
