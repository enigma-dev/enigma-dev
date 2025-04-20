#ifndef ENIGMA_GSCOLOR_MACROS_H
#define ENIGMA_GSCOLOR_MACROS_H

// 32-bit alpha-enabled color macros used for color constants and
// swizzling the color components for model and vertex functions
// they are combined with the alpha component to minimize the bandwidth
// necessary to send vertex data to the GPU
#define COL_GET_R(x) ((x & 0x000000FF))
#define COL_GET_G(x) ((x & 0x0000FF00)>>8)
#define COL_GET_B(x) ((x & 0x00FF0000)>>16)
#define COL_GET_A(x) ((x & 0xFF000000)>>24)

#define COL_GET_Rf(x) (COL_GET_R(x)/255.0f)
#define COL_GET_Gf(x) (COL_GET_G(x)/255.0f)
#define COL_GET_Bf(x) (COL_GET_B(x)/255.0f)
#define COL_GET_Af(x) (COL_GET_A(x)/255.0f)

// clamps a floating-point alpha (from 0 to 1) between 0 and 255
#define CLAMP_ALPHA(alpha) (alpha>1?255:(alpha<0?0:(unsigned char)(alpha*255)))
// clamps a floating-point alpha (from 0 to 1) between 0 and 1
#define CLAMP_ALPHAF(alpha) (alpha <= 0 ? 0: alpha >= 1? 1: alpha)

#endif
