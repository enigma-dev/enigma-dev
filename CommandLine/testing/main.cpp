#include <gtest/gtest.h>

#include <string>
#include <cstdlib>
#include <iostream> 

std::string gitMasterDir = "/tmp/enigma-master";

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  if (argc == 1) {
    int ret = system(("bash ./ci-regression.sh " + gitMasterDir).c_str());
    if (ret != 0) {
      std::cerr << "Error: ci-regression.sh returned non-zero." << std::endl;
      return ret;
    }
  }

  return RUN_ALL_TESTS();
}
