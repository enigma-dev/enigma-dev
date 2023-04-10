#include "../../ENIGMAsystem/SHELL/Universal_System/Extensions/Json/short_json_converter.h"
#include "../../ENIGMAsystem/SHELL/Universal_System/Extensions/Json/short_json_converter.cpp"

#include <gtest/gtest.h>

#include <string>

TEST(ShortJSONTest, TestParameterArrayOfObjects) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data {"[[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4],5],[1,4]]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);

  ASSERT_EQ(success, true);

  std::string json = *buffer;

  ASSERT_EQ(json, "[{\"0\":{\"0\":{\"0\":1,\"1\":4},\"1\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}],\"2\":{\"0\":1,\"1\":4},\"3\":7,\"4\":{\"0\":1,\"1\":4},\"5\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}]},\"1\":{\"0\":1,\"1\":4},\"2\":5},{\"0\":1,\"1\":4}]");

  delete buffer;
}

TEST(ShortJSONTest, TestParameterObject) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data {"[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4],5]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);

  ASSERT_EQ(success, true);

  std::string json = *buffer;

  ASSERT_EQ(json, "{\"0\":{\"0\":{\"0\":1,\"1\":4},\"1\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}],\"2\":{\"0\":1,\"1\":4},\"3\":7,\"4\":{\"0\":1,\"1\":4},\"5\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}]},\"1\":{\"0\":1,\"1\":4},\"2\":5}");

  delete buffer;
}

TEST(ShortJSONTest, TestWrongShortJSONSyntax) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data {"57hsa2va"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  data = "hsa2va57";
  success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  json = *buffer;
  ASSERT_EQ(json, "");

  data = ",6hsa2va57";
  success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  json = *buffer;
  ASSERT_EQ(json, "");

  data = "]6hsa2va57";
  success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONTest, TestWrongShortJSONSyntaxInArrayOfObjects) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data {"[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4],5,]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);

  ASSERT_EQ(success, false);

  std::string json = *buffer;

  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONTest, TestWrongShortJSONSyntaxInObject) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data {"[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4,],5]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);

  ASSERT_EQ(success, false);

  std::string json = *buffer;

  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONTest, TestWrongShortJSONSyntaxInLevels) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data {"[[[[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4],5]"};

  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  data = "[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4],5]]]]]";
  success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}
