#include "../../ENIGMAsystem/SHELL/Universal_System/Extensions/Json/short_json_converter.cpp"
#include "../../ENIGMAsystem/SHELL/Universal_System/Extensions/Json/short_json_converter.h"

#include <gtest/gtest.h>

#include <string>

TEST(ShortJSONConverterTest, TestShortJSONSyntaxErrorInLevels_1) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"[[\"str\",4],[true,-4]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestShortJSONSyntaxErrorInLevels_2) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"[\"str\",4],[true,-4],false]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestShortJSONSyntaxErrorInLevels_3) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"[[\"str\",4[,[true,-4],false]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestShortJSONSyntaxErrorInLevels_4) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"][[\"str\",4],[true,-4],false]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestShortJSONSyntaxErrorInLevels_5) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"[[\"str\",4],[true,-4],false][]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestShortJSONSyntaxErrorInLevels_6) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"[[\"str\",4],[true,-4],false],["};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestShortJSONSyntaxErrorInLevels_7) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"[[\"str\",4],[true,-4],false]  ,  [1,4]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestShortJSONSyntaxErrorInValues_1) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"[[\"str\",4],[tyue,-4],false]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestShortJSONSyntaxErrorInValues_2) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"[[\"str\",4],[true,-4],facse]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestShortJSONSyntaxErrorInValues_3) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"[[\"str\",4],[true,-4],^]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestShortJSONSyntaxErrorInValues_4) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"[[\"str\",hsdj],[true,-4],false]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestRandomInput_1) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"57hsa2va"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestRandomInput_2) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"hsa2va57"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestRandomInput_3) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"&hsa2va57"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestRandomInput_4) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{",6hsa2va57"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestRandomInput_5) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"]6hsa2va57"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONConverterTest, TestRandomInput_6) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{"[6hsa2va57"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONTest, TestSpaces) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;

  std::string data{
      " [ \n[ \" s \ttr\n \" ,\t 8\n ] ,"
      " \n [ \ntrue\t ,\r\r\n -4\r ] "
      ", \r\nfalse\t ] "};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, true);
  std::string json = *buffer;
  ASSERT_EQ(json, "{\"0\":{\"0\":\" s \ttr\n \",\"1\":8},\"1\":{\"0\":true,\"1\":-4},\"2\":false}");

  delete buffer;
}

TEST(ShortJSONTest, TestPassingArrayOfJSONObjects) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data{"[[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4],5],[1,4]]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);

  ASSERT_EQ(success, true);

  std::string json = *buffer;

  ASSERT_EQ(json,
            "[{\"0\":{\"0\":{\"0\":1,\"1\":4},\"1\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}],\"2\":{\"0\":1,\"1\":4},"
            "\"3\":7,\"4\":{\"0\":1,\"1\":4},\"5\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}]},\"1\":{"
            "\"0\":1,\"1\":4},\"2\":5},{\"0\":1,\"1\":4}]");

  delete buffer;
}

TEST(ShortJSONTest, TestPassingSingleJSONObject) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data{"[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4],5]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);

  ASSERT_EQ(success, true);

  std::string json = *buffer;

  ASSERT_EQ(json,
            "{\"0\":{\"0\":{\"0\":1,\"1\":4},\"1\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}],\"2\":{\"0\":1,\"1\":4},\"3\":"
            "7,\"4\":{\"0\":1,\"1\":4},\"5\":[{\"0\":1,\"1\":4},{\"0\":1,\"1\":4},{\"0\":1,\"1\":4}]},\"1\":{\"0\":1,"
            "\"1\":4},\"2\":5}");

  delete buffer;
}

TEST(ShortJSONTest, TestShortJSONSyntaxError_1) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data{"[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4],5,]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);

  ASSERT_EQ(success, false);

  std::string json = *buffer;

  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONTest, TestShortJSONSyntaxError_2) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data{"[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4,],5]"};
  bool success = shortJSONConverter.parse_into_buffer(data, buffer);

  ASSERT_EQ(success, false);

  std::string json = *buffer;

  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONTest, TestShortJSONSyntaxError_3) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data{"[[[[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4],5]"};

  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONTest, TestShortJSONSyntaxError_4) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data{"[[[1,4],[[1,4],[1,4]],[1,4],7,[1,4],[[1,4],[1,4],[1,4]]],[1,4],5]]]]]"};

  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, false);
  std::string json = *buffer;
  ASSERT_EQ(json, "");

  delete buffer;
}

TEST(ShortJSONTest, TestPassingExtensiveGeneralInput) {
  std::string *buffer = new std::string();
  enigma::ShortJSONConverter shortJSONConverter;
  std::string data {"\r [[[\"]  ][dm895vmfv_,,\" , \t true],"
                    "[[\r\n  false,-46.8] ,"
                    "[\"dsjdjk\" ,\n65890.3]] ,"
                    "[-77777777777,44444444], true ,"
                    "[\"\", 4567.1],"
                    "[[13,0  ],"
                    "[\"ty  \" ,true],"
                    "[133,  \"fghkll::\"]]],"
                    "[\t  1.6678\r,4\n\n],"
                    "5\r ] \n"};

  bool success = shortJSONConverter.parse_into_buffer(data, buffer);
  ASSERT_EQ(success, true);
  std::string json = *buffer;
  std::string expected {"{"
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

  ASSERT_EQ(json,expected);

  delete buffer;
  }
