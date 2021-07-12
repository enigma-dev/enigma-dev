#ifdef BUILD_XICON
#include <cmath>
#include <cstdio>
#include <iostream>
#include "../../../../shared/libpng-util/libpng-util.h"
#include "../../Universal_System/nlpo2.h"
using enigma::nlpo2;
int main(int argc, char **argv) {
  if (argc == 0) { 
    std::cout << "usage: " << argv[0] << " in.png > out.h";
    return 0;
  }
  unsigned char *data = nullptr;
  unsigned pngwidth, pngheight;
  unsigned error = libpng_decode32_file(&data, &pngwidth, &pngheight, argv[1], true);
  if (error) return error;
  unsigned widfull = nlpo2(pngwidth) + 1,
  hgtfull = nlpo2(pngheight) + 1, ih, iw;
  const int bitmap_size = widfull * hgtfull * 4;
  unsigned char *bitmap = new unsigned char[bitmap_size]();
  std::cout << "static unsigned long window_icon_default[] = {" << std::endl;
  std::cout << pngwidth       << "," << std::endl;
  std::cout << pngheight      << "," << std::endl;
  for (ih = 0; ih < pngheight; ih++) {
    unsigned tmp = ih * widfull * 4;
    for (iw = 0; iw < pngwidth; iw++) {
      bitmap[tmp + 0] = data[4 * pngwidth * ih + iw * 4 + 0];
      bitmap[tmp + 1] = data[4 * pngwidth * ih + iw * 4 + 1];
      bitmap[tmp + 2] = data[4 * pngwidth * ih + iw * 4 + 2];
      bitmap[tmp + 3] = data[4 * pngwidth * ih + iw * 4 + 3];
      printf("0x%lX,\n", bitmap[tmp + 0] | (bitmap[tmp + 1] << 8) | (bitmap[tmp + 2] << 16) | (bitmap[tmp + 3] << 24));
      tmp += 4;
    }
  }
  std::cout << "};" << std::endl;
  delete[] bitmap; 
  delete[] data;
  return 0;
}
#endif
