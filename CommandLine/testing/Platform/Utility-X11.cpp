#include "Utility.hpp"

#include <fstream>

int image_compare(std::string image1, std::string image2, std::string diff) {
  //ret 0 = no problemo
  int ret = system(("compare -metric AE " + image1 + " " + image2 + " " + diff + " 2>/tmp/enigma_draw_diff.txt").c_str());
  if (ret) {
    return ret;
  } else {
    // read result from ImageMagick's error stream
    int ae;
    std::ifstream diffFile;
    diffFile.open("/tmp/enigma_draw_diff.txt");
    diffFile >> ae;
    diffFile.close();
    return ae;
  }
}

  void file_delete(std::string fPath) {
    system(("rm -f " + fPath).c_str());
  }
