#include <gtest/gtest.h>
#include "../../../../ENIGMAsystem/SHELL/Universal_System/Serialization/JSON_Serialization/JSON_types_serialization_includes.h"

TEST(deserialize_function, Test_Basics) {
  int val1 = 10;
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(val1);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<int>(json1), val1);

  float val2 = 10.5;
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(val2);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<float>(json2), val2);

  long val3 = 10757957966;
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(val3);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<long>(json3), val3);

  double val4 = 10.598698;
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(val4);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<double>(json4), val4);

  bool val5 = true;
  std::string json5 = enigma::JSON_serialization::internal_serialize_into_fn(val5);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<bool>(json5), val5);

  std::string val6 = "hello";
  std::string json6 = enigma::JSON_serialization::internal_serialize_into_fn(val6);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::string>(json6), val6);

  std::string val7 = "wor\"ld";
  std::string json7 = enigma::JSON_serialization::internal_serialize_into_fn(val7);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::string>(json7), val7);

  char val8 = 'f';
  std::string json8 = enigma::JSON_serialization::internal_serialize_into_fn(val8);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<char>(json8), val8);

  const int val9Size = 5;
  unsigned char* val9 = new unsigned char[val9Size];
  val9[0] = 0x0;
  val9[1] = 0x34;
  val9[2] = 0x56;
  val9[3] = 0x78;
  val9[4] = 0x9a;

  std::vector<std::string> hexStream;
  for (int i = 0; i < 5; ++i) {
    std::stringstream hexStreamItem;
    hexStreamItem << std::hex << static_cast<int>(val9[i]);
    hexStream.push_back(hexStreamItem.str());
  }

  std::string json9 = enigma::JSON_serialization::internal_serialize_into_fn(hexStream);

  std::vector<std::string> out = enigma::JSON_serialization::internal_deserialize_fn<std::vector<std::string>>(json9);
  unsigned char* val9Out = new unsigned char[val9Size];

  for (int i = 0; i < val9Size; ++i) {
    std::istringstream hexStreamItem(out[i]);
    int value;
    hexStreamItem >> std::hex >> value;
    val9Out[i] = static_cast<unsigned char>(value);
  }

  ASSERT_EQ(val9Out[0], val9[0]);
  ASSERT_EQ(val9Out[1], val9[1]);
  ASSERT_EQ(val9Out[2], val9[2]);
  ASSERT_EQ(val9Out[3], val9[3]);
  ASSERT_EQ(val9Out[4], val9[4]);

  unsigned int val10 = 12;
  std::string json10 = enigma::JSON_serialization::internal_serialize_into_fn(val10);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<unsigned int>(json10), val10);
}

TEST(deserialize_function, Test_Vector) {
  std::vector<int> vector1 = {1, 2, 3, 4, 5};
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(vector1);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::vector<int>>(json1), vector1);

  std::vector<long> vector2 = {936331, 293684, 3343, 237654, 23785};
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(vector2);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::vector<long>>(json2), vector2);

  std::vector<double> vector3 = {13876.5396, 3287452.528763, 2373.286935, 3964.3465, 59863.53246};
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(vector3);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::vector<double>>(json3), vector3);

  std::vector<bool> vector4 = {true, false, true, false, true};
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(vector4);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::vector<bool>>(json4), vector4);

  std::vector<std::string> vector5 = {"hel\"lo", "world", "how", "are", "yo\\u"};
  std::string json5 = enigma::JSON_serialization::internal_serialize_into_fn(vector5);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::vector<std::string>>(json5), vector5);

  std::vector<char> vector6 = {'h', 'e', 'l', 'l', 'o'};
  std::string json6 = enigma::JSON_serialization::internal_serialize_into_fn(vector6);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::vector<char>>(json6), vector6);

  std::vector<std::map<int,int>> vector7 = {{{1,2},{3,4}},{{5,6},{7,8}}};
  std::string json7 = enigma::JSON_serialization::internal_serialize_into_fn(vector7);
  std::vector<std::map<int,int>> out7 = enigma::JSON_serialization::internal_deserialize_fn<std::vector<std::map<int,int>>>(json7);
  ASSERT_EQ(out7, vector7);
}

TEST(deserialize_function, Test_Set) {
  std::set<int> Set1 = {123, 2332, 223, 23424, 9085};
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(Set1);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::set<int>>(json1), Set1);

  std::set<long> Set2 = {3134599, 223293684, 7093343, 376202376, 387023785};
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(Set2);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::set<long>>(json2), Set2);

  std::set<double> Set3 = {13876.5396, 3287452.3287, 2373.28635, 3964.3165, 5986311.5324};
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(Set3);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::set<double>>(json3), Set3);

  std::set<bool> Set4 = {true, true, false, true};
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(Set4);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::set<bool>>(json4), Set4);

  std::set<std::string> Set5 = {"hel\"lo", "wor\tld", "how", "are", "yo\\u"};
  std::string json5 = enigma::JSON_serialization::internal_serialize_into_fn(Set5);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::set<std::string>>(json5), Set5);

  std::set<char> Set6 = {'h', 'e', 'l', 'l', 'o'};
  std::string json6 = enigma::JSON_serialization::internal_serialize_into_fn(Set6);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::set<char>>(json6), Set6);
}

TEST(deserialize_function, Test_Queue) {
  std::queue<int> Queue1;
  Queue1.push(123);
  Queue1.push(2332);
  Queue1.push(223);
  Queue1.push(23424);
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(Queue1);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::queue<int>>(json1), Queue1);

  std::queue<long> Queue2;
  Queue2.push(3134599);
  Queue2.push(223293684);
  Queue2.push(7093343);
  Queue2.push(376202376);
  Queue2.push(387023785);
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(Queue2);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::queue<long>>(json2), Queue2);

  std::queue<double> Queue3;
  Queue3.push(13876.5396);
  Queue3.push(3287452.3287);
  Queue3.push(2373.28635);
  Queue3.push(3964.3165);
  Queue3.push(5986311.5324);
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(Queue3);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::queue<double>>(json3), Queue3);

  std::queue<bool> Queue4;
  Queue4.push(true);
  Queue4.push(true);
  Queue4.push(false);
  Queue4.push(true);
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(Queue4);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::queue<bool>>(json4), Queue4);

  std::queue<std::string> Queue5;
  Queue5.push("hel\"lo");
  Queue5.push("wor\tld");
  Queue5.push("how");
  Queue5.push("are");
  Queue5.push("yo\\u");
  std::string json5 = enigma::JSON_serialization::internal_serialize_into_fn(Queue5);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::queue<std::string>>(json5), Queue5);
}

TEST(deserialize_function, Test_Complex) {
  std::complex<int> complex1 = {1, 2};
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(complex1);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::complex<int>>(json1), complex1);

  std::complex<long> complex2 = {936331, 293684};
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(complex2);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::complex<long>>(json2), complex2);

  std::complex<double> complex3 = {13876.5396, 3287452.528763};
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(complex3);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::complex<double>>(json3), complex3);

  std::complex<bool> complex4 = {true, false};
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(complex4);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::complex<bool>>(json4), complex4);
}

TEST(deserialize_function, Test_Map) {
  std::map<int, long> Map1 = {{343, 254323}, {3097, 986434}, {3285, 626333}};
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(Map1);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::map<int, long>>(json1)), Map1);

  std::map<long, double> Map2 = {{343, 254323.234}, {3097, 986434.234}, {3285, 626333.234}};
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(Map2);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::map<long, double>>(json2)), Map2);

  std::map<bool, char> Map3 = {{true, 't'}, {false, 'f'}};
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(Map3);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::map<bool, char>>(json3)), Map3);

  std::map<std::string, bool> Map4 = {{"hello", true}, {"wo\"rld", false}};
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(Map4);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::map<std::string, bool>>(json4)), Map4);
}

TEST(deserialize_function, Test_Pair) {
  std::pair<int, long> Pair1 = {343, 254323};
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(Pair1);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::pair<int, long>>(json1)), Pair1);

  std::pair<long, double> Pair2 = {343, 254323.234};
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(Pair2);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::pair<long, double>>(json2)), Pair2);

  std::pair<bool, char> Pair3 = {true, 't'};
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(Pair3);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::pair<bool, char>>(json3)), Pair3);

  std::pair<std::string, bool> Pair4 = {"hello", true};
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(Pair4);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::pair<std::string, bool>>(json4)), Pair4);
}

TEST(deserialize_function, Test_Stack) {
  std::stack<int> Stack1;
  Stack1.push(123);
  Stack1.push(2332);
  Stack1.push(223);
  Stack1.push(23424);
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(Stack1);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::stack<int>>(json1), Stack1);

  std::stack<long> Stack2;
  Stack2.push(3134599);
  Stack2.push(223293684);
  Stack2.push(7093343);
  Stack2.push(376202376);
  Stack2.push(387023785);
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(Stack2);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::stack<long>>(json2), Stack2);

  std::stack<double> Stack3;
  Stack3.push(13876.5396);
  Stack3.push(3287452.3287);
  Stack3.push(2373.28635);
  Stack3.push(3964.3165);
  Stack3.push(5986311.5324);
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(Stack3);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::stack<double>>(json3), Stack3);

  std::stack<bool> Stack4;
  Stack4.push(true);
  Stack4.push(true);
  Stack4.push(false);
  Stack4.push(true);
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(Stack4);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::stack<bool>>(json4), Stack4);

  std::stack<std::string> Stack5;
  Stack5.push("hel\"lo");
  Stack5.push("wor\tld");
  Stack5.push("how");
  Stack5.push("are");
  Stack5.push("yo\\u");
  std::string json5 = enigma::JSON_serialization::internal_serialize_into_fn(Stack5);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::stack<std::string>>(json5), Stack5);

  std::stack<char> Stack6;
  Stack6.push('h');
  Stack6.push('e');
  Stack6.push('l');
  Stack6.push('l');
  Stack6.push('o');
  std::string json6 = enigma::JSON_serialization::internal_serialize_into_fn(Stack6);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<std::stack<char>>(json6), Stack6);
}

TEST(deserialize_function, Test_Tuple) {
  std::tuple<int, long, double, bool, std::string, char> Tuple1 = {343, 254323, 254323.234, true, "hello", 't'};
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(Tuple1);
  ASSERT_EQ(
      (enigma::JSON_serialization::internal_deserialize_fn<std::tuple<int, long, double, bool, std::string, char>>(
          json1)),
      Tuple1);

  std::tuple<long, double, bool, std::string, char> Tuple2 = {343, 254323.234, true, "hello", 't'};
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(Tuple2);
  ASSERT_EQ(
      (enigma::JSON_serialization::internal_deserialize_fn<std::tuple<long, double, bool, std::string, char>>(json2)),
      Tuple2);

  std::tuple<double, bool, std::string, char> Tuple3 = {254323.234, true, "hello", 't'};
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(Tuple3);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::tuple<double, bool, std::string, char>>(json3)),
            Tuple3);

  std::tuple<bool, std::string, char> Tuple4 = {true, "hello", 't'};
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(Tuple4);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::tuple<bool, std::string, char>>(json4)), Tuple4);

  std::tuple<std::string, char> Tuple5 = {"hello", 't'};
  std::string json5 = enigma::JSON_serialization::internal_serialize_into_fn(Tuple5);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::tuple<std::string, char>>(json5)), Tuple5);

  std::tuple<char> Tuple6 = {'t'};
  std::string json6 = enigma::JSON_serialization::internal_serialize_into_fn(Tuple6);
  ASSERT_EQ((enigma::JSON_serialization::internal_deserialize_fn<std::tuple<char>>(json6)), Tuple6);
}

TEST(deserialize_function, Test_Variant) {
  variant variant1 = 10;
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(variant1);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<variant>(json1), variant1);

  variant variant2 = 10.5968;
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(variant2);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<variant>(json2), variant2);

  variant variant3 = "fares";
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(variant3);
  ASSERT_EQ(enigma::JSON_serialization::internal_deserialize_fn<variant>(json3), variant3);
}

TEST(deserialize_function, Test_Lua_Table) {
  lua_table<int> table1;
  table1[1] = 1;
  table1[2] = 2;
  table1[3] = 3;
  std::string json1 = enigma::JSON_serialization::internal_serialize_into_fn(table1);
  lua_table<int> out1 = enigma::JSON_serialization::internal_deserialize_fn<lua_table<int>>(json1);
  ASSERT_EQ(out1[1], table1[1]);
  ASSERT_EQ(out1[2], table1[2]);
  ASSERT_EQ(out1[3], table1[3]);
  ASSERT_EQ(out1.mx_size_part(), table1.mx_size_part());

  lua_table<int> table2;
  table2.fill(4, 3);
  table2[1] = 1;
  table2[2] = 2;
  table2[12] = 3;
  std::string json2 = enigma::JSON_serialization::internal_serialize_into_fn(table2);
  lua_table<int> out2 = enigma::JSON_serialization::internal_deserialize_fn<lua_table<int>>(json2);
  ASSERT_EQ(out2[0], table2[0]);
  ASSERT_EQ(out2[1], table2[1]);
  ASSERT_EQ(out2[2], table2[2]);
  ASSERT_EQ(out2[12], table2[12]);
  ASSERT_EQ(out2.mx_size_part(), table2.mx_size_part());

  lua_table<char> table3;
  table3.fill('s', 3);
  table3[1] = 'a';
  table3[2] = 's';
  table3[10] = 'f';
  std::string json3 = enigma::JSON_serialization::internal_serialize_into_fn(table3);
  lua_table<char> out3 = enigma::JSON_serialization::internal_deserialize_fn<lua_table<char>>(json3);
  ASSERT_EQ(out3[1], table3[1]);
  ASSERT_EQ(out3[2], table3[2]);
  ASSERT_EQ(out3[3], table3[3]);
  ASSERT_EQ(out3.mx_size_part(), table3.mx_size_part());

  lua_table<std::string> table4;
  table4.fill("ss", 5);
  table4[1] = "fares";
  table4[2] = "atef";
  table4[20] = "he\"llo";
  table4[21] = "wo\trld";
  std::string json4 = enigma::JSON_serialization::internal_serialize_into_fn(table4);
  lua_table<std::string> out4 = enigma::JSON_serialization::internal_deserialize_fn<lua_table<std::string>>(json4);
  ASSERT_EQ(out4[0], table4[0]);
  ASSERT_EQ(out4[1], table4[1]);
  ASSERT_EQ(out4[2], table4[2]);
  ASSERT_EQ(out4[20], table4[20]);
  ASSERT_EQ(out4[21], table4[21]);
  ASSERT_EQ(out4.mx_size_part(), table4.mx_size_part());

  lua_table<double> table5;
  table5.fill(4.5, 3);
  table5[1] = 1.5;
  table5[2] = 2.5;
  table5[12] = 3.5;
  std::string json5 = enigma::JSON_serialization::internal_serialize_into_fn(table5);
  lua_table<double> out5 = enigma::JSON_serialization::internal_deserialize_fn<lua_table<double>>(json5);
  ASSERT_EQ(out5[0], table5[0]);
  ASSERT_EQ(out5[1], table5[1]);
  ASSERT_EQ(out5[2], table5[2]);
  ASSERT_EQ(out5[12], table5[12]);
  ASSERT_EQ(out5.mx_size_part(), table5.mx_size_part());
}