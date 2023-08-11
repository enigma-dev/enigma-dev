#include <gtest/gtest.h>
#include "../../../ENIGMAsystem/SHELL/Universal_System/Serialization/serialization.h"

TEST(byte_size_function, Test_vector) {
  std::vector<int> vec1 = {1, 2, 3, 4, 5};
  ASSERT_EQ(enigma::byte_size(vec1), 20);

  std::vector<double> vec2 = {12.2, 13.3, 1233213212.2, 1232312322.123, 232322, 2};
  ASSERT_EQ(enigma::byte_size(vec2), 48);

  std::vector<char> vec3 = {'a', 'b', 'c', 'd', 'e'};
  ASSERT_EQ(enigma::byte_size(vec3), 5);

  std::vector<bool> vec4 = {true, false};
  ASSERT_EQ(enigma::byte_size(vec4), 2);
}

TEST(byte_size_function, Test_map) {
  std::map<int, int> map1 = {{1, 2}, {3, 4}};
  ASSERT_EQ(enigma::byte_size(map1), 16);

  std::map<int, double> map2 = {{12, 13.3}, {1233213212, 1232312322.123}, {232322, 2.233}};
  ASSERT_EQ(enigma::byte_size(map2), 36);

  std::map<char, int> map3 = {{'a', 12}, {'b', 22}, {'c', 223}, {'d', 298}, {'e', 7869}};
  ASSERT_EQ(enigma::byte_size(map3), 25);

  std::map<bool, char> map4 = {{true, 'f'}, {false, 'g'}};
  ASSERT_EQ(enigma::byte_size(map4), 4);
}

TEST(serialize_deserialize_functions, Test_vector1) {
  std::vector<int> inputVector = {12, 123, 342432, 12312312, 12323, 23232, 323232, 233};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputVector, len, bytes);
  ASSERT_EQ(bytes.size(), 40);

  std::byte* iter = bytes.data();
  std::vector<int> outputVector;
  len = 0;
  enigma::enigma_deserialize(outputVector, iter, len);

  ASSERT_EQ(outputVector, inputVector);
}

TEST(serialize_deserialize_functions, Test_vector2) {
  std::vector<double> inputVector = {12.1232,   123.22,    342432.22,     12312312.2442,
                                     12323.222, 23232.896, 323232.785875, 233.123};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputVector, len, bytes);
  ASSERT_EQ(bytes.size(), 72);

  std::byte* iter = bytes.data();
  std::vector<double> outputVector;
  len = 0;
  enigma::enigma_deserialize(outputVector, iter, len);

  ASSERT_EQ(outputVector, inputVector);
}

TEST(serialize_deserialize_functions, Test_vector3) {
  std::vector<char> inputVector = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputVector, len, bytes);
  ASSERT_EQ(bytes.size(), 16);

  std::byte* iter = bytes.data();
  std::vector<char> outputVector;
  len = 0;
  enigma::enigma_deserialize(outputVector, iter, len);

  ASSERT_EQ(outputVector, inputVector);
}

TEST(serialize_deserialize_functions, Test_vector4) {
  std::vector<bool> inputVector = {true, false};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputVector, len, bytes);
  ASSERT_EQ(bytes.size(), 10);

  std::byte* iter = bytes.data();
  std::vector<bool> outputVector;
  len = 0;
  enigma::enigma_deserialize(outputVector, iter, len);

  ASSERT_EQ(outputVector, inputVector);
}

TEST(serialize_deserialize_functions, Test_vector5) {
  std::vector<bool> inputVector = {};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputVector, len, bytes);
  ASSERT_EQ(bytes.size(), 8);

  std::byte* iter = bytes.data();
  std::vector<bool> outputVector;
  len = 0;
  enigma::enigma_deserialize(outputVector, iter, len);

  ASSERT_EQ(outputVector, inputVector);
}

TEST(serialize_deserialize_functions, Test_vector6) {
  std::vector<int> inputVector = {1, 2};
  int input_int = 123;
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputVector, len, bytes);
  ASSERT_EQ(bytes.size(), 16);

  enigma::enigma_serialize(input_int, len, bytes);
  ASSERT_EQ(bytes.size(), 20);

  std::byte* iter = bytes.data();

  std::vector<int> outputVector;
  len = 0;
  enigma::enigma_deserialize(outputVector, iter, len);
  ASSERT_EQ(outputVector, inputVector);

  int output_int;
  enigma::enigma_deserialize(output_int, iter, len);
  ASSERT_EQ(output_int, input_int);
}

TEST(serialize_deserialize_functions, Test_vector7) {
  std::vector<int> inputVector1 = {1, 2};
  std::vector<char> inputVector2 = {'a', 'b'};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputVector1, len, bytes);
  ASSERT_EQ(bytes.size(), 16);

  enigma::enigma_serialize(inputVector2, len, bytes);
  ASSERT_EQ(bytes.size(), 26);

  std::byte* iter = bytes.data();

  std::vector<int> outputVector1;
  std::size_t len1 = 0;
  enigma::enigma_deserialize(outputVector1, iter, len1);
  ASSERT_EQ(outputVector1, inputVector1);

  std::vector<char> outputVector2;
  enigma::enigma_deserialize(outputVector2, iter, len1);
  ASSERT_EQ(outputVector2, inputVector2);
}

TEST(serialize_deserialize_functions, Test_map1) {
  std::map<int, int> inputMap = {{12, 123}, {342432, 12312312}};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputMap, len, bytes);
  ASSERT_EQ(bytes.size(), 24);

  std::byte* iter = bytes.data();
  std::map<int, int> outputMap;
  len = 0;
  enigma::enigma_deserialize(outputMap, iter, len);

  ASSERT_EQ(outputMap, inputMap);
}

TEST(serialize_deserialize_functions, Test_map2) {
  std::map<double, int> inputMap = {{12.1232, 123}, {342432.2297, 12312312}, {12323.222, 23232}};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputMap, len, bytes);
  ASSERT_EQ(bytes.size(), 44);

  std::byte* iter = bytes.data();
  std::map<double, int> outputMap;
  len = 0;
  enigma::enigma_deserialize(outputMap, iter, len);

  ASSERT_EQ(outputMap, inputMap);
}

TEST(serialize_deserialize_functions, Test_map3) {
  std::map<char, char> inputMap = {{'a', 'b'}, {'c', 'd'}, {'e', 'f'}};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputMap, len, bytes);
  ASSERT_EQ(bytes.size(), 14);

  std::byte* iter = bytes.data();
  std::map<char, char> outputMap;
  len = 0;
  enigma::enigma_deserialize(outputMap, iter, len);

  ASSERT_EQ(outputMap, inputMap);
}

TEST(serialize_deserialize_functions, Test_map4) {
  std::map<bool, char> inputMap = {{true, 'f'}, {false, 'g'}};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputMap, len, bytes);
  ASSERT_EQ(bytes.size(), 12);

  std::byte* iter = bytes.data();
  std::map<bool, char> outputMap;
  len = 0;
  enigma::enigma_deserialize(outputMap, iter, len);

  ASSERT_EQ(outputMap, inputMap);
}

TEST(serialize_deserialize_functions, Test_map5) {
  std::map<bool, double> inputMap = {};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputMap, len, bytes);
  ASSERT_EQ(bytes.size(), 8);

  std::byte* iter = bytes.data();
  std::map<bool, double> outputMap;
  len = 0;
  enigma::enigma_deserialize(outputMap, iter, len);

  ASSERT_EQ(outputMap, inputMap);
}

TEST(serialize_deserialize_functions, Test_map6) {
  std::map<int, char> inputMap = {{1, 'd'}, {2, 's'}};
  int input_int = 123;
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputMap, len, bytes);
  ASSERT_EQ(bytes.size(), 18);

  enigma::enigma_serialize(input_int, len, bytes);
  ASSERT_EQ(bytes.size(), 22);

  std::byte* iter = bytes.data();

  std::map<int, char> outputMap;
  len = 0;
  enigma::enigma_deserialize(outputMap, iter, len);
  ASSERT_EQ(outputMap, inputMap);

  int output_int;
  enigma::enigma_deserialize(output_int, iter, len);
  ASSERT_EQ(output_int, input_int);
}

TEST(serialize_deserialize_functions, Test_map7) {
  std::map<int, bool> inputMap1 = {{1, true}, {2, false}};
  std::map<char, double> inputMap2 = {{'a', 123233.22}, {'b', 1232.333}};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputMap1, len, bytes);
  ASSERT_EQ(bytes.size(), 18);

  enigma::enigma_serialize(inputMap2, len, bytes);
  ASSERT_EQ(bytes.size(), 44);

  std::byte* iter = bytes.data();

  std::map<int, bool> outputMap1;
  std::size_t len1 = 0;
  enigma::enigma_deserialize(outputMap1, iter, len1);
  ASSERT_EQ(outputMap1, inputMap1);

  std::map<char, double> outputMap2;
  enigma::enigma_deserialize(outputMap2, iter, len1);
  ASSERT_EQ(outputMap2, inputMap2);
}

TEST(serialize_deserialize_functions, Test_map8) {
  std::map<int, bool> inputMap = {{1, true}, {2, false}};
  std::vector<char> inputVector = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputMap, len, bytes);
  ASSERT_EQ(bytes.size(), 18);

  enigma::enigma_serialize(inputVector, len, bytes);
  ASSERT_EQ(bytes.size(), 34);

  std::byte* iter = bytes.data();

  std::map<int, bool> outputMap;
  std::size_t len1 = 0;
  enigma::enigma_deserialize(outputMap, iter, len1);
  ASSERT_EQ(outputMap, inputMap);

  std::vector<char> outputVector;
  enigma::enigma_deserialize(outputVector, iter, len1);
  ASSERT_EQ(outputVector, inputVector);
}