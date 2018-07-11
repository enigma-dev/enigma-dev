#include <gtest/gtest.h>

#include <string>
#include <cstdlib>
#include <iostream>

std::string gitMasterDir = "/tmp/enigma-master";

int main(int argc, char **argv) {
  if (argc == 1) {
    int ret = system(("bash ./ci-regression.sh " + gitMasterDir).c_str());

    if (ret != 0) {
      if (WIFEXITED(ret)) {
        std::cerr << "Error: ci-regression.sh returned non-zero " << WEXITSTATUS(ret) << std::endl;
        return WEXITSTATUS(ret);
      }
      std::cerr << "Error: ci-regression.sh failed with " << ret << std::endl;
      return -1;
    }

    // have the regular tests output to a different directory now to avoid
    // any conflicts with binaries produced by the regression tests
    setenv("TEST_HARNESS_WORKDIR", "/tmp/ENIGMA-Travis/", true);
    setenv("TEST_HARNESS_CODEGEN", "/tmp/ENIGMA-Travis/", true);

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
