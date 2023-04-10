#include "../../ENIGMAsystem/SHELL/Universal_System/Extensions/Json/short_json_converter.h"
#include "../../ENIGMAsystem/SHELL/Universal_System/Extensions/Json/short_json_converter.cpp"

#include <gtest/gtest.h>

// #include <iostream>
#include <string>

TEST(ShortJSONTest, TestSuccess) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data {"[[[1,4],4],[1,4]]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);

  ASSERT_EQ(success, true);

  std::string json = *buffer;

  delete buffer;

  ASSERT_EQ(json, "[{\"0\":{\"0\":1,\"1\":4},\"1\":4},{\"0\":1,\"1\":4}]");
}
