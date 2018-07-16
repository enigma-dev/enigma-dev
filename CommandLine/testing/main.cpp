#include <gtest/gtest.h>

#include <string>
#include <cstdlib>
#include <iostream>

std::string gitMasterDir = "/tmp/enigma-master";

int main(int argc, char **argv) {
  if (argc == 1) {
    int regression = system(("bash ./ci-regression.sh " + gitMasterDir).c_str());
    if (regression != 0) {
      std::cerr << "Error: ci-regression.sh returned non-zero " << regression << std::endl;
    }

    // have the regular tests output to a different directory now to avoid
    // any conflicts with binaries produced by the regression tests
    setenv("TEST_HARNESS_WORKDIR", "/tmp/ENIGMA-Travis/", true);
    setenv("TEST_HARNESS_CODEGEN", "/tmp/ENIGMA-Travis/", true);

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "-RegressionCompare.*";
    int ret = RUN_ALL_TESTS();
    if (ret) return ret; // non-zero = error condition
    // if there wasn't an error with the earlier regression tests
    // we can now run a comparison for the regression tests
    if (!regression) {
      ::testing::GTEST_FLAG(filter) = "RegressionCompare.*";
      ret = RUN_ALL_TESTS();
      if (ret && strcmp(getenv("TRAVIS_PULL_REQUEST"), "false") != 0)
        system("bash ./CommandLine/testing/GitHub-ImageDiff.sh");
    }
    return ret;
  }

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
