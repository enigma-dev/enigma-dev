#include <gtest/gtest.h>
#include "../../../../ENIGMAsystem/SHELL/Universal_System/Serialization/serialization.h"

namespace ejs = enigma::JSON_serialization;

TEST(serialize_into_function, Test_Var) {
  var var1(12, 2, 2);
  std::string json1 = ejs::enigma_serialize(var1);
  ASSERT_EQ(json1,
            "{\"variant\":{\"type\":\"real\",\"value\":12},\"array1d\":[{\"0\":{\"type\":"
            "\"real\",\"value\":12}},{\"1\":{\"type\":\"real\",\"value\":12}}],\"array2d\":[{\"1\":[{\"0\":{\"type\":"
            "\"real\","
            "\"value\":12}},{\"1\":{\"type\":\"real\",\"value\":12}}]}]}");

  var var2("fares", 2, 2);
  std::string json2 = ejs::enigma_serialize(var2);
  ASSERT_EQ(json2,
            "{\"variant\":{\"type\":\"string\",\"value\":\"fares\"},\"array1d\":[{\"0\":{\"type\":"
            "\"string\",\"value\":\"fares\"}},{\"1\":{\"type\":\"string\",\"value\":\"fares\"}}],\"array2d\":[{\"1\":[{"
            "\"0\":{\"type\":"
            "\"string\",\"value\":\"fares\"}},{\"1\":{\"type\":\"string\",\"value\":\"fares\"}}]}]}");

  var var3(12.22, 3, 1);
  std::string json3 = ejs::enigma_serialize(var3);
  ASSERT_EQ(json3,
            "{\"variant\":{\"type\":\"real\",\"value\":12.22},\"array1d\":[{\"0\":{\"type\":"
            "\"real\",\"value\":12.22}},{\"1\":{\"type\":\"real\",\"value\":12.22}},{\"2\":{\"type\":"
            "\"real\",\"value\":12.22}}],\"array2d\":[]}");

  var var4(1234, 3, 3);
  std::string json4 = ejs::enigma_serialize(var4);
  ASSERT_EQ(
      json4,
      "{\"variant\":{\"type\":\"real\",\"value\":1234},\"array1d\":[{\"0\":{\"type\":"
      "\"real\",\"value\":1234}},{\"1\":{\"type\":\"real\",\"value\":1234}},{\"2\":{\"type\":"
      "\"real\",\"value\":1234}}],\"array2d\":[{\"1\":[{\"0\":{\"type\":\"real\",\"value\":1234}},{\"1\":{\"type\":"
      "\"real\",\"value\":1234}},{\"2\":{\"type\":\"real\",\"value\":1234}}]},{\"2\":[{\"0\":{\"type\":"
      "\"real\",\"value\":1234}},{\"1\":{\"type\":\"real\",\"value\":1234}},{\"2\":{\"type\":"
      "\"real\",\"value\":1234}}]}]}");
}

TEST(deserialize_function, Test_Var) {
  var var1(12, 2, 2);
  std::string json1 = ejs::enigma_serialize(var1);
  var out1 = ejs::enigma_deserialize<var>(json1);

  ASSERT_EQ(out1.rval.d, var1.rval.d);
  ASSERT_EQ(out1[0].rval.d, var1[0].rval.d);
  ASSERT_EQ(out1[1].rval.d, var1[1].rval.d);
  ASSERT_EQ(out1.array2d[1][0].rval.d, var1.array2d[1][0].rval.d);
  ASSERT_EQ(out1.array2d[1][1].rval.d, var1.array2d[1][1].rval.d);

  var var2("fares", 2, 3);
  std::string json2 = ejs::enigma_serialize(var2);
  var out2 = ejs::enigma_deserialize<var>(json2);

  ASSERT_EQ(out2.sval(), var2.sval());
  ASSERT_EQ(out2[0].sval(), var2[0].sval());
  ASSERT_EQ(out2[1].sval(), var2[1].sval());
  ASSERT_EQ(out2.array2d[1][0].sval(), var2.array2d[1][0].sval());
  ASSERT_EQ(out2.array2d[1][1].sval(), var2.array2d[1][1].sval());
  ASSERT_EQ(out2.array2d[2][0].sval(), var2.array2d[2][0].sval());
  ASSERT_EQ(out2.array2d[2][1].sval(), var2.array2d[2][1].sval());

  var var3(12.22, 3, 1);
  std::string json3 = ejs::enigma_serialize(var3);
  var out3 = ejs::enigma_deserialize<var>(json3);

  ASSERT_EQ(out3.rval.d, var3.rval.d);
  ASSERT_EQ(out3[0].rval.d, var3[0].rval.d);
  ASSERT_EQ(out3[1].rval.d, var3[1].rval.d);
  ASSERT_EQ(out3[2].rval.d, var3[2].rval.d);

  var var4(1234, 3, 3);
  std::string json4 = ejs::enigma_serialize(var4);
  var out4 = ejs::enigma_deserialize<var>(json4);

  ASSERT_EQ(out4.rval.d, var4.rval.d);
  ASSERT_EQ(out4[0].rval.d, var4[0].rval.d);
  ASSERT_EQ(out4[1].rval.d, var4[1].rval.d);
  ASSERT_EQ(out4[2].rval.d, var4[2].rval.d);
  ASSERT_EQ(out4.array2d[1][0].rval.d, out4.array2d[1][0].rval.d);
  ASSERT_EQ(out4.array2d[1][1].rval.d, out4.array2d[1][1].rval.d);
  ASSERT_EQ(out4.array2d[1][2].rval.d, out4.array2d[1][2].rval.d);
  ASSERT_EQ(out4.array2d[2][0].rval.d, out4.array2d[2][0].rval.d);
  ASSERT_EQ(out4.array2d[2][1].rval.d, out4.array2d[2][1].rval.d);
  ASSERT_EQ(out4.array2d[2][2].rval.d, out4.array2d[2][2].rval.d);
}
