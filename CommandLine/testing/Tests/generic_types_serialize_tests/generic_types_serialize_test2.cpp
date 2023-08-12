#include <gtest/gtest.h>
#include "../../../../ENIGMAsystem/SHELL/Universal_System/Serialization/serialization.h"

TEST(byte_size_function, Test_tuple) {
  std::tuple<int, double, long> tuple1 = {1233, 22334.24243234, 1241413};
  ASSERT_EQ(enigma::byte_size(tuple1), 20);

  std::tuple<char, bool, int> tuple2 = {'a', true, 123};
  ASSERT_EQ(enigma::byte_size(tuple2), 6);
}

TEST(serialize_deserialize_functions, Test_tuple1) {
  std::tuple<int, char, bool> inputTuple = {12, 'f', true};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 6);

  std::byte* iter = bytes.data();
  std::tuple<int, char, bool> outputTuple;
  len = 0;
  enigma::enigma_deserialize(outputTuple, iter, len);

  ASSERT_EQ(outputTuple, inputTuple);
}

TEST(serialize_deserialize_functions, Test_tuple2) {
  std::tuple<double, int, double> inputTuple = {1233232.123232, 123, 3242432.22332};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 20);

  std::byte* iter = bytes.data();
  std::tuple<double, int, double> outputTuple;
  len = 0;
  enigma::enigma_deserialize(outputTuple, iter, len);

  ASSERT_EQ(outputTuple, inputTuple);
}

TEST(serialize_deserialize_functions, Test_tuple3) {
  std::tuple<char, char, char> inputTuple = {'a', 'b', 'c'};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 3);

  std::byte* iter = bytes.data();
  std::tuple<char, char, char> outputTuple;
  len = 0;
  enigma::enigma_deserialize(outputTuple, iter, len);

  ASSERT_EQ(outputTuple, inputTuple);
}

TEST(serialize_deserialize_functions, Test_tuple4) {
  std::tuple<bool, char, bool> inputTuple = {true, 'f', false};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 3);

  std::byte* iter = bytes.data();
  std::tuple<bool, char, bool> outputTuple;
  len = 0;
  enigma::enigma_deserialize(outputTuple, iter, len);

  ASSERT_EQ(outputTuple, inputTuple);
}

TEST(serialize_deserialize_functions, Test_tuple5) {
  std::tuple<bool, int, float> inputTuple = {};  // (false, 0, 0.0f)
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 9);

  std::byte* iter = bytes.data();
  std::tuple<bool, int, float> outputTuple;
  len = 0;
  enigma::enigma_deserialize(outputTuple, iter, len);

  ASSERT_EQ(outputTuple, inputTuple);
}

TEST(serialize_deserialize_functions, Test_tuple6) {
  std::tuple<int, char, bool> inputTuple = {1323, 'f', true};
  int input_int = 123;
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 6);

  enigma::enigma_serialize(input_int, len, bytes);
  ASSERT_EQ(bytes.size(), 10);

  std::byte* iter = bytes.data();

  std::tuple<int, char, bool> outputTuple;
  len = 0;
  enigma::enigma_deserialize(outputTuple, iter, len);
  ASSERT_EQ(outputTuple, inputTuple);

  int output_int;
  enigma::enigma_deserialize(output_int, iter, len);
  ASSERT_EQ(output_int, input_int);
}

TEST(serialize_deserialize_functions, Test_tuple7) {
  std::tuple<int, char, bool> inputTuple1 = {1233, 'f', false};
  std::tuple<bool, char, int> inputTuple2 = {true, 'b', 875758};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::enigma_serialize(inputTuple1, len, bytes);
  ASSERT_EQ(bytes.size(), 6);

  enigma::enigma_serialize(inputTuple2, len, bytes);
  ASSERT_EQ(bytes.size(), 12);

  std::byte* iter = bytes.data();

  std::tuple<int, char, bool> outputTuple1;
  std::size_t len1 = 0;
  enigma::enigma_deserialize(outputTuple1, iter, len1);
  ASSERT_EQ(outputTuple1, inputTuple1);

  std::tuple<bool, char, int> outputTuple2;
  enigma::enigma_deserialize(outputTuple2, iter, len1);
  ASSERT_EQ(outputTuple2, inputTuple2);
}

TEST(serialize_deserialize_functions, Test_tuple8) {
  std::complex<int> inputComplex = {1, 2};
  std::vector<double> inputVector = {74.6798986, 87567.9679, 98669.09708};
  std::map<bool, char> inputMap = {{true, 'a'}, {false, 'b'}};
  std::set<int> inputSet = {12, 123};
  std::tuple<int, bool, char> inputTuple = {1285765, false, 'd'};
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
}
