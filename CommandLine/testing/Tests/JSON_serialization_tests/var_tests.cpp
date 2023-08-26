#include <gtest/gtest.h>
#include "../../../../ENIGMAsystem/SHELL/Universal_System/Serialization/JSON_Serialization/JSON_types_serialization_includes.h"

TEST(serialize_into_function, Test_Var) {
  var var1(12, 2, 2);
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(var1);
  EXPECT_EQ(json1,
            "{\"variant\":{\"type\":\"real\",\"value\":12},\"array1d\":{\"mx_size_part\":2,\"dense_part\":[{\"type\":"
            "\"real\",\"value\":12},{\"type\":\"real\",\"value\":12}],\"sparse_part\":{}},\"array2d\":{\"mx_size_"
            "part\":2,\"dense_part\":[],\"sparse_part\":{\"1\":{\"mx_size_part\":2,\"dense_part\":[{\"type\":\"real\","
            "\"value\":12},{\"type\":\"real\",\"value\":12}],\"sparse_part\":{}}}}}");

  var var2("fares", 2, 2);
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(var2);
  EXPECT_EQ(
      json2,
      "{\"variant\":{\"type\":\"string\",\"value\":\"fares\"},\"array1d\":{\"mx_size_part\":2,\"dense_part\":[{"
      "\"type\":\"string\",\"value\":\"fares\"},{\"type\":\"string\",\"value\":\"fares\"}],\"sparse_part\":{}},"
      "\"array2d\":{\"mx_size_part\":2,\"dense_part\":[],\"sparse_part\":{\"1\":{\"mx_size_part\":2,\"dense_part\":[{"
      "\"type\":\"string\",\"value\":\"fares\"},{\"type\":\"string\",\"value\":\"fares\"}],\"sparse_part\":{}}}}}");

  var var3(12.22, 3, 1);
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(var3);
  EXPECT_EQ(
      json3,
      "{\"variant\":{\"type\":\"real\",\"value\":12.22},\"array1d\":{\"mx_size_part\":3,\"dense_part\":[{"
      "\"type\":\"real\",\"value\":12.22},{\"type\":\"real\",\"value\":12.22},{\"type\":\"real\",\"value\":12.22}],"
      "\"sparse_part\":{}},\"array2d\":{\"mx_size_part\":0,\"dense_part\":[],\"sparse_part\":{}}}");

  var var4(1234, 3, 3);
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(var4);
  EXPECT_EQ(
      json4,
      "{\"variant\":{\"type\":\"real\",\"value\":1234},\"array1d\":{\"mx_size_part\":3,\"dense_part\":[{\"type\":"
      "\"real\",\"value\":1234},{\"type\":\"real\",\"value\":1234},{\"type\":\"real\",\"value\":1234}],\"sparse_part\":"
      "{}},"
      "\"array2d\":{\"mx_size_part\":3,\"dense_part\":[],\"sparse_part\":{\"1\":{\"mx_size_part\":3,\"dense_part\":[{"
      "\"type\":\"real\",\"value\":1234},{\"type\":\"real\",\"value\":1234},{\"type\":\"real\",\"value\":1234}],"
      "\"sparse_part\":{}},\"2\":{\"mx_size_part\":3,\"dense_part\":[{\"type\":\"real\",\"value\":1234},{\"type\":"
      "\"real\",\"value\":1234},{\"type\":\"real\",\"value\":1234}],\"sparse_part\":{}}}}}");
}
