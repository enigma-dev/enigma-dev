#ifndef ENIGMA_GSCOLOR_MACROS_H
#define ENIGMA_GSCOLOR_MACROS_H

// 24-bit color macros used for color constants (e.g. c_red, c_green, c_blue)
// they lack an alpha component because alpha is traditionally specified in
// floating point form between 0 and 1
#define COL_GET_R(x) ((x & 0x0000FF))
#define COL_GET_G(x) ((x & 0x00FF00)>>8)
#define COL_GET_B(x) ((x & 0xFF0000)>>16)

// 32-bit alpha-enabled color macros used for model and vertex functions
// they are combined with the alpha component to minimize the bandwidth
// necessary to send vertex data to the GPU
#define COL_GET_R32(x) ((x & 0x000000FF))
#define COL_GET_G32(x) ((x & 0x0000FF00)>>8)
#define COL_GET_B32(x) ((x & 0x00FF0000)>>16)
#define COL_GET_A32(x) ((x & 0xFF000000)>>24)

// clamps a floating-point alpha (from 0 to 1) between 0 and 255
#define CLAMP_ALPHA(alpha) (alpha>1?255:(alpha<0?0:(unsigned char)(alpha*255)))
// clamps a floating-point alpha (from 0 to 1) between 0 and 1
#define CLAMP_ALPHAF(alpha) (alpha <= 0 ? 0: alpha >= 1? 1: alpha)

#endif
