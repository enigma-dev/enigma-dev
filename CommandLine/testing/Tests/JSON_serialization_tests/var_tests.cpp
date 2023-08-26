#include <gtest/gtest.h>
#include "../../../../ENIGMAsystem/SHELL/Universal_System/Serialization/JSON_Serialization/JSON_types_serialization_includes.h"

TEST(serialize_into_function, Test_Var) {
  var var1(12, 2, 2);
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(var1);
  ASSERT_EQ(json1,
            "{\"variant\":{\"type\":\"real\",\"value\":12},\"array1d\":{\"mx_size_part\":2,\"dense_part\":[{\"type\":"
            "\"real\",\"value\":12},{\"type\":\"real\",\"value\":12}],\"sparse_part\":{}},\"array2d\":{\"mx_size_"
            "part\":2,\"dense_part\":[],\"sparse_part\":{\"1\":{\"mx_size_part\":2,\"dense_part\":[{\"type\":\"real\","
            "\"value\":12},{\"type\":\"real\",\"value\":12}],\"sparse_part\":{}}}}}");

  var var2("fares", 2, 2);
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(var2);
  ASSERT_EQ(
      json2,
      "{\"variant\":{\"type\":\"string\",\"value\":\"fares\"},\"array1d\":{\"mx_size_part\":2,\"dense_part\":[{"
      "\"type\":\"string\",\"value\":\"fares\"},{\"type\":\"string\",\"value\":\"fares\"}],\"sparse_part\":{}},"
      "\"array2d\":{\"mx_size_part\":2,\"dense_part\":[],\"sparse_part\":{\"1\":{\"mx_size_part\":2,\"dense_part\":[{"
      "\"type\":\"string\",\"value\":\"fares\"},{\"type\":\"string\",\"value\":\"fares\"}],\"sparse_part\":{}}}}}");

  var var3(12.22, 3, 1);
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(var3);
  ASSERT_EQ(
      json3,
      "{\"variant\":{\"type\":\"real\",\"value\":12.22},\"array1d\":{\"mx_size_part\":3,\"dense_part\":[{"
      "\"type\":\"real\",\"value\":12.22},{\"type\":\"real\",\"value\":12.22},{\"type\":\"real\",\"value\":12.22}],"
      "\"sparse_part\":{}},\"array2d\":{\"mx_size_part\":0,\"dense_part\":[],\"sparse_part\":{}}}");

  var var4(1234, 3, 3);
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(var4);
  ASSERT_EQ(
      json4,
      "{\"variant\":{\"type\":\"real\",\"value\":1234},\"array1d\":{\"mx_size_part\":3,\"dense_part\":[{\"type\":"
      "\"real\",\"value\":1234},{\"type\":\"real\",\"value\":1234},{\"type\":\"real\",\"value\":1234}],\"sparse_part\":"
      "{}},"
      "\"array2d\":{\"mx_size_part\":3,\"dense_part\":[],\"sparse_part\":{\"1\":{\"mx_size_part\":3,\"dense_part\":[{"
      "\"type\":\"real\",\"value\":1234},{\"type\":\"real\",\"value\":1234},{\"type\":\"real\",\"value\":1234}],"
      "\"sparse_part\":{}},\"2\":{\"mx_size_part\":3,\"dense_part\":[{\"type\":\"real\",\"value\":1234},{\"type\":"
      "\"real\",\"value\":1234},{\"type\":\"real\",\"value\":1234}],\"sparse_part\":{}}}}}");
}

TEST(deserialize_function, Test_Var) {
  var var1(12, 2, 2);
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(var1);
  var out1 = enigma::JSON_serialization::internal_deserialize_fn<var>(json1);

  ASSERT_EQ(out1.rval.d, var1.rval.d);
  ASSERT_EQ(out1[0].rval.d, var1[0].rval.d);
  ASSERT_EQ(out1[1].rval.d, var1[1].rval.d);
  ASSERT_EQ(out1.array1d.mx_size_part(), var1.array1d.mx_size_part());
  ASSERT_EQ(out1.array2d[1][0].rval.d, var1.array2d[1][0].rval.d);
  ASSERT_EQ(out1.array2d[1][1].rval.d, var1.array2d[1][1].rval.d);
  ASSERT_EQ(out1.array2d.mx_size_part(), var1.array2d.mx_size_part());

  var var2("fares", 2, 3);
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(var2);
  var out2 = enigma::JSON_serialization::internal_deserialize_fn<var>(json2);

  ASSERT_EQ(out2.sval(), var2.sval());
  ASSERT_EQ(out2[0].sval(), var2[0].sval());
  ASSERT_EQ(out2[1].sval(), var2[1].sval());
  ASSERT_EQ(out2.array1d.mx_size_part(), var2.array1d.mx_size_part());
  ASSERT_EQ(out2.array2d[1][0].sval(), var2.array2d[1][0].sval());
  ASSERT_EQ(out2.array2d[1][1].sval(), var2.array2d[1][1].sval());
  ASSERT_EQ(out2.array2d[2][0].sval(), var2.array2d[2][0].sval());
  ASSERT_EQ(out2.array2d[2][1].sval(), var2.array2d[2][1].sval());
  ASSERT_EQ(out2.array2d.mx_size_part(), var2.array2d.mx_size_part());

  var var3(12.22, 3, 1);
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(var3);
  var out3 = enigma::JSON_serialization::internal_deserialize_fn<var>(json3);

  ASSERT_EQ(out3.rval.d, var3.rval.d);
  ASSERT_EQ(out3[0].rval.d, var3[0].rval.d);
  ASSERT_EQ(out3[1].rval.d, var3[1].rval.d);
  ASSERT_EQ(out3[2].rval.d, var3[2].rval.d);
  ASSERT_EQ(out3.array1d.mx_size_part(), var3.array1d.mx_size_part());
  ASSERT_EQ(out3.array2d.mx_size_part(), var3.array2d.mx_size_part());

  var var4(1234, 3, 3);
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(var4);
  var out4 = enigma::JSON_serialization::internal_deserialize_fn<var>(json4);

  ASSERT_EQ(out4.rval.d, var4.rval.d);
  ASSERT_EQ(out4[0].rval.d, var4[0].rval.d);
  ASSERT_EQ(out4[1].rval.d, var4[1].rval.d);
  ASSERT_EQ(out4[2].rval.d, var4[2].rval.d);
  ASSERT_EQ(out4.array1d.mx_size_part(), var4.array1d.mx_size_part());
  ASSERT_EQ(out4.array2d[1][0].rval.d, out4.array2d[1][0].rval.d);
  ASSERT_EQ(out4.array2d[1][1].rval.d, out4.array2d[1][1].rval.d);
  ASSERT_EQ(out4.array2d[1][2].rval.d, out4.array2d[1][2].rval.d);
  ASSERT_EQ(out4.array2d[2][0].rval.d, out4.array2d[2][0].rval.d);
  ASSERT_EQ(out4.array2d[2][1].rval.d, out4.array2d[2][1].rval.d);
  ASSERT_EQ(out4.array2d[2][2].rval.d, out4.array2d[2][2].rval.d);
  ASSERT_EQ(out4.array2d.mx_size_part(), var4.array2d.mx_size_part());
}
