/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

/**Blend Mode Functions********************************************************\

bm_normal, bm_add, bm_max, bm_subtract, bm_zero, bm_one, bm_src_color,
bm_inv_src_color, bm_src_alpha, bm_inv_src_alpha, bm_dest_alpha,
bm_inv_dest_alpha, bm_dest_color, bm_inv_dest_color, bm_src_alpha_sat

int draw_set_blend_mode(ARG mode)
int draw_set_blend_mode_ext(ARG src,ARG2 dest)

\******************************************************************************/
#ifndef ENIGMA_GSBLEND_H
#define ENIGMA_GSBLEND_H

namespace enigma {

extern int currentblendmode[2];
extern int currentblendtype;

} // namespace enigma

namespace enigma_user
{
  enum {
    bm_normal   = 0,
    bm_add      = 1,
    bm_max      = 2,
    bm_subtract = 3
  };

  enum {
    bm_zero             =  1,  // GL_ZERO                  D3DBLEND_ZERO
    bm_one              =  2,  // GL_ONE                   D3DBLEND_ONE
    bm_src_color        =  3,  // GL_SRC_COLOR             D3DBLEND_SRCCOLOR    //only for dest
    bm_inv_src_color    =  4,  // GL_ONE_MINUS_SRC_COLOR   D3DBLEND_INVSRCCOLOR //only for dest
    bm_src_alpha        =  5,  // GL_SRC_ALPHA             D3DBLEND_SRCALPHA
    bm_inv_src_alpha    =  6,  // GL_ONE_MINUS_SRC_ALPHA   D3DBLEND_INVSRCALPHA
    bm_dest_alpha       =  7,  // GL_DST_ALPHA             D3DBLEND_DESTALPHA
    bm_inv_dest_alpha   =  8,  // GL_ONE_MINUS_DST_ALPHA   D3DBLEND_INVDESTALPHA
    bm_dest_color       =  9,  // GL_DST_COLOR             D3DBLEND_DESTCOLOR     //only for src
    bm_inv_dest_color   = 10,  // GL_ONE_MINUS_DST_COLOR   D3DBLEND_INVDESTCOLOR  //only for src
    bm_src_alpha_sat    = 11   // GL_SRC_ALPHA_SATURATE    D3DBLEND_SRCALPHASAT   //only for src
  };

  int draw_set_blend_mode(int mode);
  int draw_set_blend_mode_ext(int src,int dest);

  int draw_get_blend_mode();
  int draw_get_blend_mode_ext(bool src);
  int draw_get_blend_mode_type();
}

#endif // ENIGMA_GSBLEND_H
