#include <gtest/gtest.h>
#include "../../../../ENIGMAsystem/SHELL/Universal_System/Serialization/JSON_Serialization/JSON_types_serialization_includes.h"

TEST(serialize_into_function, Test_Basics) {
  int val1 = 10;
  ASSERT_EQ(enigma::JSONserialization::JSON_serialize_into(val1), "10");

  float val2 = 10.5;
  ASSERT_EQ(enigma::JSONserialization::JSON_serialize_into(val2), "10.5");

  long val3 = 10757957966;
  ASSERT_EQ(enigma::JSONserialization::JSON_serialize_into(val3), "10757957966");

  double val4 = 10.598698;
  ASSERT_EQ(enigma::JSONserialization::JSON_serialize_into(val4), "10.598698");

  bool val5 = true;
  ASSERT_EQ(enigma::JSONserialization::JSON_serialize_into(val5), "true");

  std::string val6 = "hello";
  ASSERT_EQ(enigma::JSONserialization::JSON_serialize_into(val6), "\"hello\"");

  char val7 = 'f';
  ASSERT_EQ(enigma::JSONserialization::JSON_serialize_into(val7), "'f'");
}

TEST(serialize_into_function, Test_Vector) {
  std::vector<int> vector1 = {1, 2, 3, 4, 5};
  ASSERT_EQ(enigma::JSONserialization::JSON_serialize_into(vector1), "[1,2,3,4,5]");

  std::vector<std::string> vector2 = {"fares", "atef", "hello", "world"};
  ASSERT_EQ(enigma::JSONserialization::JSON_serialize_into(vector2), "[\"fares\",\"atef\",\"hello\",\"world\"]");

  std::vector<char> vector3 = {'a', 's', 'f'};
  ASSERT_EQ(enigma::JSONserialization::JSON_serialize_into(vector3), "['a','s','f']");
}