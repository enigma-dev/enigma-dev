#include <gtest/gtest.h>

#include <string>
#include <cstdlib>
#include <iostream>

std::string gitMasterDir = "/tmp/enigma-master";

int main(int argc, char **argv) {
  if (argc == 1) {
    int ret = system(("bash ./ci-regression.sh " + gitMasterDir).c_str());
    if (ret != 0) {
      std::cerr << "Error: ci-regression.sh returned non-zero." << std::endl;
      return ret;
    }
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "-RegressionCompare.*";
    ret = RUN_ALL_TESTS();
    if (ret) return ret; // non-zero = error condition
    ::testing::GTEST_FLAG(filter) = "RegressionCompare.*";
    return RUN_ALL_TESTS();
  }

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
