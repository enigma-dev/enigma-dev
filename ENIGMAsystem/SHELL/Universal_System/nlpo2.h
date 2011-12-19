// This nightmare belongs to serprex.
// It is released under the terms of the GNU General Public License, version 3.
// It was forged in hell.

#include <stdio.h>
inline unsigned int nlpo2dc(unsigned int x) // Taking x, returns n such that n = 2**k where k is an integer and n >= x.
{
  --x;
	x |= x>>1;
	x |= x>>2;
	x |= x>>4;
	x |= x>>8;
	return x | (x>>16);
}
