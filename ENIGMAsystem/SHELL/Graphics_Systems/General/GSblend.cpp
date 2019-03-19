#include "GSblend.h"

namespace enigma {

int blendMode[2] = { enigma_user::bm_src_alpha, enigma_user::bm_inv_src_alpha };

} // namespace enigma

namespace enigma_user {

void draw_set_blend_mode(int mode) {
  const static int dest_modes[] = {bm_inv_src_alpha,bm_one,bm_inv_src_color,bm_inv_src_color};

  enigma::blendMode[0] = (mode == bm_subtract) ? bm_zero : bm_src_alpha;
  enigma::blendMode[1] = dest_modes[mode % 4];
}

void draw_set_blend_mode_ext(int src, int dest) {
  enigma::blendMode[0] = src;
  enigma::blendMode[1] = dest;
}

int draw_get_blend_mode_src() {
  return enigma::blendMode[0];
}

int draw_get_blend_mode_dest() {
  return enigma::blendMode[1];
}

} // namespace enigma_user
