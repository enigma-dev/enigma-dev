#if !defined(JSON_READER_TEST_CPP)
#define JSON_READER_TEST_CPP
#endif  // JSON_READER_TEST_CPP

#include "Universal_System/Extensions/Json/json_reader.cpp"
#include "Universal_System/Extensions/Json/json_value.cpp"
#include "Universal_System/Extensions/Json/json_writer.cpp"
#include "Universal_System/Extensions/Json/reader.h"
#include "Universal_System/Extensions/Json/value.h"

#include <gtest/gtest.h>

#include <string>

TEST(JSONTest, TestSpaces) {
  std::string data{
      " { \"0\":\t{ \"0\": \" s \ttr\n \", \"1\": \n8 } , \"1\"\r\n:{\"0\":true,\"1\" : -4 } ,\r\"2\":\nfalse}"};
  Json::Value root;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(std::string(data), root);

  EXPECT_TRUE(root.isObject());
  ASSERT_EQ(parsingSuccessful, true);
}

TEST(JSONTest, TestPassingArrayOfJSONObjects) {
  std::string data{
      "[{\"0\":{\"0\":{\"0\":1,\"1\":4},\"1\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}],\"2\":{\"0\":1,\"1\":4},"
      "\"3\":7,\"4\":{\"0\":1,\"1\":4},\"5\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}]},\"1\":{"
      "\"0\":1,\"1\":4},\"2\":5},{\"0\":1,\"1\":4}]"};
  Json::Value root;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(std::string(data), root);

  EXPECT_TRUE(root.isArray());
  ASSERT_EQ(parsingSuccessful, true);
}

TEST(JSONTest, TestPassingSingleJSONObject) {
  std::string data{
      "{\"0\":{\"0\":{\"0\":1,\"1\":4},\"1\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}],\"2\":{\"0\":1,\"1\":4},\"3\":"
      "7,\"4\":{\"0\":1,\"1\":4},\"5\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}]},\"1\":{\"0\":1,"
      "\"1\":4},\"2\":5}"};
  Json::Value root;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(std::string(data), root);

  EXPECT_TRUE(root.isObject());
  ASSERT_EQ(parsingSuccessful, true);
}

TEST(JSONTest, TestPassingExtensiveGeneralInput) {
  std::string data{
      "{"
      "\"0\":{"
      "\"0\":{"
      "\"0\":\"]  ][dm895vmfv_,,\","
      "\"1\":true"
      "},"
      "\"1\":["
      "{"
      "\"0\":false,"
      "\"1\":-46.8"
      "},"
      "{"
      "\"0\":\"dsjdjk\","
      "\"1\":65890.3"
      "}"
      "],"
      "\"2\":{"
      "\"0\":-77777777777,"
      "\"1\":44444444"
      "},"
      "\"3\":true,"
      "\"4\":{"
      "\"0\":\"\","
      "\"1\":4567.1"
      "},"
      "\"5\":["
      "{"
      "\"0\":13,"
      "\"1\":0"
      "},"
      "{"
      "\"0\":\"ty  \","
      "\"1\":true"
      "},"
      "{"
      "\"0\":133,"
      "\"1\":\"fghkll::\""
      "}"
      "]"
      "},"
      "\"1\":{"
      "\"0\":1.6678,"
      "\"1\":4"
      "},"
      "\"2\":5"
      "}"};
  Json::Value root;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(std::string(data), root);
  
  EXPECT_TRUE(root.isObject());
  ASSERT_EQ(parsingSuccessful, true);
}
