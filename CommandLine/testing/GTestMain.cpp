#include "TestHarness.hpp"
#include <gtest/gtest.h>

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::TestSuite;
using ::testing::UnitTest;

class ConfigPrinter : public EmptyTestEventListener {
  void OnTestStart(const TestInfo& test_info) override{
    TestHarness::configpool = "";
  }
  void OnTestEnd(const TestInfo& test_info) override{
    testing::Test::RecordProperty("configpool",TestHarness::configpool);
  }
};
int main(int argc,char **argv) {
  InitGoogleTest(&argc, argv);

  UnitTest& unit_test=*UnitTest::GetInstance();
  TestEventListeners& listeners = unit_test.listeners();
  //delete listeners.Release(listeners.default_result_printer());
  listeners.Append(new ConfigPrinter);

  return RUN_ALL_TESTS();
}
