#ifndef HARNESS_IMAGE_COMP_HPP
#define HARNESS_IMAGE_COMP_HPP

#include <string>

extern std::string gitMasterDir;

int image_compare(std::string image1, std::string image2, std::string diff);
void file_delete(std::string fPath);

#endif
