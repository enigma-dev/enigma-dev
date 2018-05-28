#ifndef ENIGMA_GSCOLOR_MACROS_H
#define ENIGMA_GSCOLOR_MACROS_H

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00)>>8)
#define __GETB(x) ((x & 0xFF0000)>>16)
#define __GETRf(x) fmod(__GETR(x),256)
#define __GETGf(x) fmod(x/256,256)
#define __GETBf(x) fmod(x/65536,256)*

#define bind_alpha(alpha) (alpha>1?255:(alpha<0?0:(unsigned char)(alpha*255)))

#endif
