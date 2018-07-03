#include <gtest/gtest.h>

#include <cstdlib>

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  // run all of the tests on HEAD
  auto result = RUN_ALL_TESTS();

  // git checkout the HEAD of the master branch to compare
  // for regressions
  system("git checkout HEAD");

  // run only the regression tests on HEAD^
  ::testing::GTEST_FLAG(filter) = "Regression.*";
  result = RUN_ALL_TESTS();

  // reset the git state in case somebody wants to
  // run these regression tests locally
  system("git checkout HEAD^");

  return 0;
}
