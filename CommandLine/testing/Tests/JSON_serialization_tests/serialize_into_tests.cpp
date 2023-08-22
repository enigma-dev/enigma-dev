#include <gtest/gtest.h>
#include "../../../../ENIGMAsystem/SHELL/Universal_System/Serialization/JSON_Serialization/JSON_types_serialization_includes.h"

TEST(serialize_into_function, Test_Basics) {
  int val1 = 10;
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(val1), "10");

  float val2 = 10.5;
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(val2), "10.5");

  long val3 = 10757957966;
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(val3), "10757957966");

  double val4 = 10.598698;
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(val4), "10.598698");

  bool val5 = true;
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(val5), "\"true\"");

  std::string val6 = "hello";
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(val6), "\"hello\"");

  char val7 = 'f';
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(val7), "\"f\"");
}

TEST(serialize_into_function, Test_Vector) {
  std::vector<int> vector1 = {1, 2, 3, 4, 5};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(vector1), "[1,2,3,4,5]");

  std::vector<std::string> vector2 = {"fares", "atef", "hello", "world"};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(vector2),
            "[\"fares\",\"atef\",\"hello\",\"world\"]");

  std::vector<char> vector3 = {'a', 's', 'f'};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(vector3), "[\"a\",\"s\",\"f\"]");

  std::vector<bool> vector4 = {true, false, true, false};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(vector4), "[\"true\",\"false\",\"true\",\"false\"]");
}

TEST(serialize_into_function, Test_Set) {
  std::set<int> set1 = {1, 2, 3, 4, 5};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(set1), "[1,2,3,4,5]");

  std::set<std::string> set2 = {"fares", "atef", "hello", "world"};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(set2),
            "[\"atef\",\"fares\",\"hello\",\"world\"]");  // set is sorted

  std::set<char> set3 = {'a', 's', 'f'};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(set3), "[\"a\",\"f\",\"s\"]");  // set is sorted

  std::set<bool> set4 = {true, false, true, false};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(set4), "[\"false\",\"true\"]");  // set is sorted
}

TEST(serialize_into_function, Test_Map) {
  std::map<int, char> map1 = {{1, 'f'}, {2, 'g'}, {3, 'y'}, {4, 'w'}, {5, 'q'}};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(map1),
            "{\"1\":\"f\",\"2\":\"g\",\"3\":\"y\",\"4\":\"w\",\"5\":\"q\"}");

  std::map<bool, double> map2 = {{true, 98610.58768}, {false, 76920.59869}};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(map2),
            "{\"false\":\"76920.59869\",\"true\":\"98610.58768\"}");  // map is sorted

  std::map<std::string, long> map3 = {{"fares", 10757957966}, {"atef", 10757959686}};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(map3),
            "{\"atef\":\"10757959686\",\"fares\":\"10757957966\"}");  // map is sorted
}

TEST(serialize_into_function, Test_Tuple) {
  std::tuple<int, double, long> tuple1 = {10, 10.5968, 10757957966};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(tuple1), "[10,10.5968,10757957966]");

  std::tuple<std::string, bool, char> tuple2 = {"fares", true, 'f'};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(tuple2), "[\"fares\",\"true\",\"f\"]");

  std::tuple<std::string, bool, char, int, double, long> tuple3 = {"fares", false, 'f', 10, 10.5968, 10757957966};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(tuple3),
            "[\"fares\",\"false\",\"f\",10,10.5968,10757957966]");
}

TEST(serialize_into_function, Test_Complex) {
  std::complex<int> complex1 = {10, 123430};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(complex1), "[10,123430]");

  std::complex<double> complex2 = {1220.59682, 1550.595685};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(complex2), "[1220.59682,1550.595685]");

  std::complex<long> complex3 = {107579668753, 10757957966};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(complex3), "[107579668753,10757957966]");
}

TEST(serialize_into_function, Test_Pair) {
  std::pair<int, double> pair1 = {10, 10.5968};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(pair1), "[10,10.5968]");

  std::pair<std::string, bool> pair2 = {"fares", true};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(pair2), "[\"fares\",\"true\"]");

  std::pair<char, long> pair3 = {'f', 8364343};
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(pair3), "[\"f\",8364343]");
}

TEST(serialize_into_function, Test_Queue) {
  std::queue<int> queue1;
  queue1.push(10);
  queue1.push(20);
  queue1.push(30);
  queue1.push(40);
  queue1.push(50);
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(queue1), "[10,20,30,40,50]");

  std::queue<std::string> queue2;
  queue2.push("fares");
  queue2.push("atef");
  queue2.push("hello");
  queue2.push("world");
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(queue2), "[\"fares\",\"atef\",\"hello\",\"world\"]");

  std::queue<char> queue3;
  queue3.push('a');
  queue3.push('s');
  queue3.push('f');
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(queue3), "[\"a\",\"s\",\"f\"]");

  std::queue<bool> queue4;
  queue4.push(true);
  queue4.push(false);
  queue4.push(true);
  queue4.push(false);
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(queue4), "[\"true\",\"false\",\"true\",\"false\"]");
}

TEST(serialize_into_function, Test_Stack) {
  std::stack<int> stack1;
  stack1.push(10);
  stack1.push(20);
  stack1.push(30);
  stack1.push(40);
  stack1.push(50);
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(stack1), "[50,40,30,20,10]");

  std::stack<std::string> stack2;
  stack2.push("fares");
  stack2.push("atef");
  stack2.push("hello");
  stack2.push("world");
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(stack2), "[\"world\",\"hello\",\"atef\",\"fares\"]");

  std::stack<char> stack3;
  stack3.push('a');
  stack3.push('s');
  stack3.push('f');
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(stack3), "[\"f\",\"s\",\"a\"]");

  std::stack<bool> stack4;
  stack4.push(true);
  stack4.push(false);
  stack4.push(true);
  stack4.push(false);
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(stack4), "[\"false\",\"true\",\"false\",\"true\"]");
}

TEST(serialize_into_function, Test_Variant) {
  variant variant1 = 10;
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(variant1), "{\"0\":\"10\"}");

  variant variant2 = 10.5968;
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(variant2), "{\"0\":\"10.5968\"}");

  variant variant3 = "fares";
  ASSERT_EQ(enigma::JSON_serialization::internal_serialize_into_fn(variant3), "{\"1\":\"fares\"}");
}
