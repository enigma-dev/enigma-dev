#include <gtest/gtest.h>
#include "../../../../ENIGMAsystem/SHELL/Universal_System/Serialization/serialization.h"

TEST(byte_size_function, Test_pair) {
  std::pair<int, char> pair1 = {1, 2};
  ASSERT_EQ(enigma::enigma_internal_sizeof(pair1), 5);

  std::pair<double, long> pair2 = {12232.324342, 13123};
  ASSERT_EQ(enigma::enigma_internal_sizeof(pair2), 16);

  std::pair<char, bool> pair3 = {'a', false};
  ASSERT_EQ(enigma::enigma_internal_sizeof(pair3), 2);
}

TEST(byte_size_function, Test_lua_table) {
  lua_table<int> lua_table1;
  lua_table1.fill(0, 10);

  lua_table1[2] = 42;
  lua_table1[5] = 17;

  ASSERT_EQ(enigma::enigma_internal_sizeof(lua_table1), 64);  // 8+ 8+ 10*4+ 8

  lua_table<char> lua_table2;
  lua_table2.fill('.', 15);

  lua_table2[2] = 'f';
  lua_table2[3] = 'a';
  lua_table2[4] = 'r';
  lua_table2[5] = 'e';
  lua_table2[6] = 's';

  ASSERT_EQ(enigma::enigma_internal_sizeof(lua_table2), 39);  // 8+ 8+ 15+ 8

  lua_table<long> lua_table3;
  lua_table3.fill(376433, 20);

  lua_table3[2] = 4283754;
  lua_table3[5] = 87587588;

  ASSERT_EQ(enigma::enigma_internal_sizeof(lua_table3), 184);  // 8+ 8+ 20*8+ 8
}

TEST(serialize_deserialize_functions, Test_pair1) {
  std::pair<int, int> inputPair = {12, 123};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputPair, len, bytes);
  ASSERT_EQ(bytes.size(), 8);  // not 16 because we don't store the size of the pair, it is always 1

  std::byte* iter = bytes.data();
  std::pair<int, int> outputPair;
  len = 0;
  enigma::enigma_deserialize(outputPair, iter, len);

  ASSERT_EQ(outputPair, inputPair);
}

TEST(serialize_deserialize_functions, Test_pair2) {
  std::pair<double, long> inputPair = {1223.123297, 123933};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputPair, len, bytes);
  ASSERT_EQ(bytes.size(), 16);

  std::byte* iter = bytes.data();
  std::pair<double, long> outputPair;
  len = 0;
  enigma::enigma_deserialize(outputPair, iter, len);

  ASSERT_EQ(outputPair, inputPair);
}

TEST(serialize_deserialize_functions, Test_pair3) {
  std::pair<long, long> inputPair = {};  // {0, 0}
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputPair, len, bytes);
  ASSERT_EQ(bytes.size(), 16);

  std::byte* iter = bytes.data();
  std::pair<long, long> outputPair;
  len = 0;
  enigma::enigma_deserialize(outputPair, iter, len);

  ASSERT_EQ(outputPair, inputPair);
}

TEST(serialize_deserialize_functions, Test_pair4) {
  std::pair<int, char> inputPair = {123, 'w'};
  int input_int = 123;
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputPair, len, bytes);
  ASSERT_EQ(bytes.size(), 5);

  enigma::enigma_serialize(input_int, len, bytes);
  ASSERT_EQ(bytes.size(), 9);

  std::byte* iter = bytes.data();

  std::pair<int, char> outputPair;
  len = 0;
  enigma::enigma_deserialize(outputPair, iter, len);
  ASSERT_EQ(outputPair, inputPair);

  int output_int;
  enigma::enigma_deserialize(output_int, iter, len);
  ASSERT_EQ(output_int, input_int);
}

TEST(serialize_deserialize_functions, Test_pair5) {
  std::pair<int, char> inputPair1 = {1, '2'};
  std::pair<double, bool> inputPair2 = {74.6798986, false};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputPair1, len, bytes);
  ASSERT_EQ(bytes.size(), 5);

  enigma::enigma_serialize(inputPair2, len, bytes);
  ASSERT_EQ(bytes.size(), 14);

  std::byte* iter = bytes.data();

  std::pair<int, char> outputPair1;
  std::size_t len1 = 0;
  enigma::enigma_deserialize(outputPair1, iter, len1);
  ASSERT_EQ(outputPair1, inputPair1);

  std::pair<double, bool> outputPair2;
  enigma::enigma_deserialize(outputPair2, iter, len1);
  ASSERT_EQ(outputPair2, inputPair2);
}

TEST(serialize_deserialize_functions, Test_pair6) {
  std::complex<int> inputComplex = {1, 2};
  std::vector<double> inputVector = {74.6798986, 87567.9679, 98669.09708};
  std::map<bool, char> inputMap = {{true, 'a'}, {false, 'b'}};
  std::set<int> inputSet = {12, 123};
  std::tuple<int, bool, char> inputTuple = {1285765, false, 'd'};
  std::queue<long> inputQueue;
  inputQueue.push(123123);
  inputQueue.push(1231223);
  inputQueue.push(13341223);

  std::stack<long> inputStack;
  inputStack.push(123123);
  inputStack.push(1231223);
  inputStack.push(13341223);

  std::pair<bool, char> inputPair = {false, 'f'};

  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputComplex, len, bytes);
  ASSERT_EQ(bytes.size(), 8);

  enigma::enigma_serialize(inputVector, len, bytes);
  ASSERT_EQ(bytes.size(), 40);

  enigma::enigma_serialize(inputMap, len, bytes);
  ASSERT_EQ(bytes.size(), 52);

  enigma::enigma_serialize(inputSet, len, bytes);
  ASSERT_EQ(bytes.size(), 68);

  enigma::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 74);

  enigma::enigma_serialize(inputQueue, len, bytes);
  ASSERT_EQ(bytes.size(), 106);

  enigma::enigma_serialize(inputStack, len, bytes);
  ASSERT_EQ(bytes.size(), 138);

  enigma::enigma_serialize(inputPair, len, bytes);
  ASSERT_EQ(bytes.size(), 140);

  std::byte* iter = bytes.data();

  std::complex<int> outputComplex;
  std::size_t len1 = 0;
  enigma::enigma_deserialize(outputComplex, iter, len1);
  ASSERT_EQ(outputComplex, inputComplex);

  std::vector<double> outputVector;
  enigma::enigma_deserialize(outputVector, iter, len1);
  ASSERT_EQ(outputVector, inputVector);

  std::map<bool, char> outputMap;
  enigma::enigma_deserialize(outputMap, iter, len1);
  ASSERT_EQ(outputMap, inputMap);

  std::set<int> outputSet;
  enigma::enigma_deserialize(outputSet, iter, len1);
  ASSERT_EQ(outputSet, inputSet);

  std::tuple<int, bool, char> outputTuple;
  enigma::enigma_deserialize(outputTuple, iter, len1);
  ASSERT_EQ(outputTuple, inputTuple);

  std::queue<long> outputQueue;
  enigma::enigma_deserialize(outputQueue, iter, len1);
  ASSERT_EQ(outputQueue, inputQueue);

  std::stack<long> outputStack;
  enigma::enigma_deserialize(outputStack, iter, len1);
  ASSERT_EQ(outputStack, inputStack);

  std::pair<bool, char> outputPair;
  enigma::enigma_deserialize(outputPair, iter, len1);
  ASSERT_EQ(outputPair, inputPair);
}
