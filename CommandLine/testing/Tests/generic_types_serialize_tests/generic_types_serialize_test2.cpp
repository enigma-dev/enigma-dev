#include <gtest/gtest.h>
#include "../../../../ENIGMAsystem/SHELL/Universal_System/Serialization/serialization.h"

TEST(enigma_internal_sizeof_function, Test_tuple) {
  std::tuple<int, double, long> tuple1 = {1233, 22334.24243234, 1241413};
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(tuple1), 28);

  std::tuple<char, bool, int> tuple2 = {'a', true, 123};
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(tuple2), 14);

  std::tuple<long, char> tuple3 = {12424, 'f'};
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(tuple3), 17);

  std::tuple<double> tuple4 = {12424.78557};
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(tuple4), 16);

  std::tuple<> tuple5;
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(tuple5), 8);
}

TEST(enigma_internal_sizeof_function, Test_queue) {
  std::queue<int> queue1;
  queue1.push(123123);
  queue1.push(1231223);
  queue1.push(13341223);
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(queue1), 20);

  std::queue<char> queue2;
  queue2.push('a');
  queue2.push('b');
  queue2.push('c');
  queue2.push('d');
  queue2.push('e');
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(queue2), 13);

  std::queue<bool> queue3;
  queue3.push(true);
  queue3.push(false);
  queue3.push(true);
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(queue3), 11);

  std::queue<long> queue4;
  queue4.push(123123);
  queue4.push(1231223);
  queue4.push(13341223);
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(queue4), 32);
}

TEST(enigma_internal_sizeof_function, Test_stack) {
  std::stack<int> stack1;
  stack1.push(123123);
  stack1.push(1231223);
  stack1.push(13341223);
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(stack1), 20);

  std::stack<char> stack2;
  stack2.push('a');
  stack2.push('b');
  stack2.push('c');
  stack2.push('d');
  stack2.push('e');
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(stack2), 13);

  std::stack<bool> stack3;
  stack3.push(true);
  stack3.push(false);
  stack3.push(true);
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(stack3), 11);

  std::stack<long> stack4;
  stack4.push(123123);
  stack4.push(1231223);
  stack4.push(13341223);
  ASSERT_EQ(enigma::bytes_serialization::enigma_internal_sizeof(stack4), 32);
}

TEST(serialize_deserialize_functions, Test_tuple1) {
  std::tuple<int, char, bool> inputTuple = {12, 'f', true};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 14);

  std::byte* iter = bytes.data();
  std::tuple<int, char, bool> outputTuple;
  len = 0;
  enigma::bytes_serialization::enigma_deserialize(outputTuple, iter, len);

  ASSERT_EQ(outputTuple, inputTuple);
}

TEST(serialize_deserialize_functions, Test_tuple2) {
  std::tuple<double, int, double> inputTuple = {1233232.123232, 123, 3242432.22332};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 28);

  std::byte* iter = bytes.data();
  std::tuple<double, int, double> outputTuple;
  len = 0;
  enigma::bytes_serialization::enigma_deserialize(outputTuple, iter, len);

  ASSERT_EQ(outputTuple, inputTuple);
}

TEST(serialize_deserialize_functions, Test_tuple3) {
  std::tuple<char, char, char> inputTuple = {'a', 'b', 'c'};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 11);

  std::byte* iter = bytes.data();
  std::tuple<char, char, char> outputTuple;
  len = 0;
  enigma::bytes_serialization::enigma_deserialize(outputTuple, iter, len);

  ASSERT_EQ(outputTuple, inputTuple);
}

TEST(serialize_deserialize_functions, Test_tuple4) {
  std::tuple<bool, char, bool> inputTuple = {true, 'f', false};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 11);

  std::byte* iter = bytes.data();
  std::tuple<bool, char, bool> outputTuple;
  len = 0;
  enigma::bytes_serialization::enigma_deserialize(outputTuple, iter, len);

  ASSERT_EQ(outputTuple, inputTuple);
}

TEST(serialize_deserialize_functions, Test_tuple5) {
  std::tuple<bool, int, float> inputTuple = {};  // (false, 0, 0.0f)
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 17);

  std::byte* iter = bytes.data();
  std::tuple<bool, int, float> outputTuple;
  len = 0;
  enigma::bytes_serialization::enigma_deserialize(outputTuple, iter, len);

  ASSERT_EQ(outputTuple, inputTuple);
}

TEST(serialize_deserialize_functions, Test_tuple6) {
  std::tuple<int, char, bool> inputTuple = {1323, 'f', true};
  int input_int = 123;
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 14);

  enigma::bytes_serialization::enigma_serialize(input_int, len, bytes);
  ASSERT_EQ(bytes.size(), 18);

  std::byte* iter = bytes.data();

  std::tuple<int, char, bool> outputTuple;
  len = 0;
  enigma::bytes_serialization::enigma_deserialize(outputTuple, iter, len);
  ASSERT_EQ(outputTuple, inputTuple);

  int output_int;
  enigma::bytes_serialization::enigma_deserialize(output_int, iter, len);
  ASSERT_EQ(output_int, input_int);
}

TEST(serialize_deserialize_functions, Test_tuple7) {
  std::tuple<int, char, bool> inputTuple1 = {1233, 'f', false};
  std::tuple<bool, char, int> inputTuple2 = {true, 'b', 875758};
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputTuple1, len, bytes);
  ASSERT_EQ(bytes.size(), 14);

  enigma::bytes_serialization::enigma_serialize(inputTuple2, len, bytes);
  ASSERT_EQ(bytes.size(), 28);

  std::byte* iter = bytes.data();

  std::tuple<int, char, bool> outputTuple1;
  std::size_t len1 = 0;
  enigma::bytes_serialization::enigma_deserialize(outputTuple1, iter, len1);
  ASSERT_EQ(outputTuple1, inputTuple1);

  std::tuple<bool, char, int> outputTuple2;
  enigma::bytes_serialization::enigma_deserialize(outputTuple2, iter, len1);
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

  enigma::bytes_serialization::enigma_serialize(inputComplex, len, bytes);
  ASSERT_EQ(bytes.size(), 8);

  enigma::bytes_serialization::enigma_serialize(inputVector, len, bytes);
  ASSERT_EQ(bytes.size(), 40);

  enigma::bytes_serialization::enigma_serialize(inputMap, len, bytes);
  ASSERT_EQ(bytes.size(), 52);

  enigma::bytes_serialization::enigma_serialize(inputSet, len, bytes);
  ASSERT_EQ(bytes.size(), 68);

  enigma::bytes_serialization::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 82);

  std::byte* iter = bytes.data();

  std::complex<int> outputComplex;
  std::size_t len1 = 0;
  enigma::bytes_serialization::enigma_deserialize(outputComplex, iter, len1);
  ASSERT_EQ(outputComplex, inputComplex);

  std::vector<double> outputVector;
  enigma::bytes_serialization::enigma_deserialize(outputVector, iter, len1);
  ASSERT_EQ(outputVector, inputVector);

  std::map<bool, char> outputMap;
  enigma::bytes_serialization::enigma_deserialize(outputMap, iter, len1);
  ASSERT_EQ(outputMap, inputMap);

  std::set<int> outputSet;
  enigma::bytes_serialization::enigma_deserialize(outputSet, iter, len1);
  ASSERT_EQ(outputSet, inputSet);

  std::tuple<int, bool, char> outputTuple;
  enigma::bytes_serialization::enigma_deserialize(outputTuple, iter, len1);
  ASSERT_EQ(outputTuple, inputTuple);
}

TEST(serialize_deserialize_functions, Test_tuple9) {
  std::tuple<int, bool> inputTuple1 = {1285765, false};
  std::tuple<int> inputTuple2 = {12857};
  std::tuple<int, bool, char> inputTuple3 = {1285765, false, 'f'};
  std::tuple<int, bool, long, double, char, bool> inputTuple4 = {1285765, false, 87575, 98656.75, 'g', true};

  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputTuple1, len, bytes);
  ASSERT_EQ(bytes.size(), 13);

  enigma::bytes_serialization::enigma_serialize(inputTuple2, len, bytes);
  ASSERT_EQ(bytes.size(), 25);

  enigma::bytes_serialization::enigma_serialize(inputTuple3, len, bytes);
  ASSERT_EQ(bytes.size(), 39);

  enigma::bytes_serialization::enigma_serialize(inputTuple4, len, bytes);
  ASSERT_EQ(bytes.size(), 70);

  std::size_t len1 = 0;
  std::byte* iter = bytes.data();

  std::tuple<int, bool> outputTuple1;
  enigma::bytes_serialization::enigma_deserialize(outputTuple1, iter, len1);
  ASSERT_EQ(outputTuple1, inputTuple1);

  std::tuple<int> outputTuple2;
  enigma::bytes_serialization::enigma_deserialize(outputTuple2, iter, len1);
  ASSERT_EQ(outputTuple2, inputTuple2);

  std::tuple<int, bool, char> outputTuple3;
  enigma::bytes_serialization::enigma_deserialize(outputTuple3, iter, len1);
  ASSERT_EQ(outputTuple3, inputTuple3);

  std::tuple<int, bool, long, double, char, bool> outputTuple4;
  enigma::bytes_serialization::enigma_deserialize(outputTuple4, iter, len1);
  ASSERT_EQ(outputTuple4, inputTuple4);
}

TEST(serialize_deserialize_functions, Test_queue1) {
  std::queue<int> inputQueue;
  inputQueue.push(1123);
  inputQueue.push(232);
  inputQueue.push(332);
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputQueue, len, bytes);
  ASSERT_EQ(bytes.size(), 20);

  std::byte* iter = bytes.data();
  std::queue<int> outputQueue;
  len = 0;

  enigma::bytes_serialization::enigma_deserialize(outputQueue, iter, len);
  ASSERT_EQ(outputQueue, inputQueue);
}

TEST(serialize_deserialize_functions, Test_queue2) {
  std::queue<double> inputQueue;
  inputQueue.push(12.1232);
  inputQueue.push(123.22);
  inputQueue.push(342432.22);
  inputQueue.push(12312312.2442);
  inputQueue.push(12323.222);
  inputQueue.push(23232.896);
  inputQueue.push(323232.785875);
  inputQueue.push(233.123);
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputQueue, len, bytes);
  ASSERT_EQ(bytes.size(), 72);

  std::byte* iter = bytes.data();
  std::queue<double> outputQueue;
  len = 0;

  enigma::bytes_serialization::enigma_deserialize(outputQueue, iter, len);
  ASSERT_EQ(outputQueue, inputQueue);
}

TEST(serialize_deserialize_functions, Test_queue3) {
  std::queue<char> inputQueue;
  inputQueue.push('a');
  inputQueue.push('b');
  inputQueue.push('c');
  inputQueue.push('d');
  inputQueue.push('e');
  inputQueue.push('f');
  inputQueue.push('g');
  inputQueue.push('h');
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputQueue, len, bytes);
  ASSERT_EQ(bytes.size(), 16);

  std::byte* iter = bytes.data();
  std::queue<char> outputQueue;
  len = 0;

  enigma::bytes_serialization::enigma_deserialize(outputQueue, iter, len);
  ASSERT_EQ(outputQueue, inputQueue);
}

TEST(serialize_deserialize_functions, Test_queue4) {
  std::queue<bool> inputQueue;
  inputQueue.push(true);
  inputQueue.push(false);
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputQueue, len, bytes);
  ASSERT_EQ(bytes.size(), 10);

  std::byte* iter = bytes.data();
  std::queue<bool> outputQueue;
  len = 0;

  enigma::bytes_serialization::enigma_deserialize(outputQueue, iter, len);
  ASSERT_EQ(outputQueue, inputQueue);
}

TEST(serialize_deserialize_functions, Test_queue5) {
  std::queue<bool> inputQueue;
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputQueue, len, bytes);
  ASSERT_EQ(bytes.size(), 8);

  std::byte* iter = bytes.data();
  std::queue<bool> outputQueue;
  len = 0;

  enigma::bytes_serialization::enigma_deserialize(outputQueue, iter, len);
  ASSERT_EQ(outputQueue, inputQueue);
}

TEST(serialize_deserialize_functions, Test_queue6) {
  std::queue<int> inputQueue;
  inputQueue.push(123);
  inputQueue.push(123);

  int input_int = 123;
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputQueue, len, bytes);
  ASSERT_EQ(bytes.size(), 16);

  enigma::bytes_serialization::enigma_serialize(input_int, len, bytes);
  ASSERT_EQ(bytes.size(), 20);

  std::byte* iter = bytes.data();

  std::queue<int> outputQueue;
  len = 0;
  enigma::bytes_serialization::enigma_deserialize(outputQueue, iter, len);
  ASSERT_EQ(outputQueue, inputQueue);

  int output_int;
  enigma::bytes_serialization::enigma_deserialize(output_int, iter, len);
  ASSERT_EQ(output_int, input_int);
}

TEST(serialize_deserialize_functions, Test_queue7) {
  std::queue<int> inputQueue1;
  inputQueue1.push(3);
  inputQueue1.push(4);
  std::queue<char> inputQueue2;
  inputQueue2.push('c');
  inputQueue2.push('d');

  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputQueue1, len, bytes);
  ASSERT_EQ(bytes.size(), 16);

  enigma::bytes_serialization::enigma_serialize(inputQueue2, len, bytes);
  ASSERT_EQ(bytes.size(), 26);

  std::byte* iter = bytes.data();

  std::queue<int> outputQueue1;
  std::size_t len1 = 0;
  enigma::bytes_serialization::enigma_deserialize(outputQueue1, iter, len1);
  ASSERT_EQ(outputQueue1, inputQueue1);

  std::queue<char> outputQueue2;
  enigma::bytes_serialization::enigma_deserialize(outputQueue2, iter, len1);
  ASSERT_EQ(outputQueue2, inputQueue2);
}

TEST(serialize_deserialize_functions, Test_queue8) {
  std::complex<int> inputComplex = {1, 2};
  std::vector<double> inputVector = {74.6798986, 87567.9679, 98669.09708};
  std::map<bool, char> inputMap = {{true, 'a'}, {false, 'b'}};
  std::set<int> inputSet = {12, 123};
  std::tuple<int, bool, char> inputTuple = {1285765, false, 'd'};
  std::queue<long> inputQueue;
  inputQueue.push(123123);
  inputQueue.push(1231223);
  inputQueue.push(13341223);

  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputComplex, len, bytes);
  ASSERT_EQ(bytes.size(), 8);

  enigma::bytes_serialization::enigma_serialize(inputVector, len, bytes);
  ASSERT_EQ(bytes.size(), 40);

  enigma::bytes_serialization::enigma_serialize(inputMap, len, bytes);
  ASSERT_EQ(bytes.size(), 52);

  enigma::bytes_serialization::enigma_serialize(inputSet, len, bytes);
  ASSERT_EQ(bytes.size(), 68);

  enigma::bytes_serialization::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 82);

  enigma::bytes_serialization::enigma_serialize(inputQueue, len, bytes);
  ASSERT_EQ(bytes.size(), 114);

  std::byte* iter = bytes.data();

  std::complex<int> outputComplex;
  std::size_t len1 = 0;
  enigma::bytes_serialization::enigma_deserialize(outputComplex, iter, len1);
  ASSERT_EQ(outputComplex, inputComplex);

  std::vector<double> outputVector;
  enigma::bytes_serialization::enigma_deserialize(outputVector, iter, len1);
  ASSERT_EQ(outputVector, inputVector);

  std::map<bool, char> outputMap;
  enigma::bytes_serialization::enigma_deserialize(outputMap, iter, len1);
  ASSERT_EQ(outputMap, inputMap);

  std::set<int> outputSet;
  enigma::bytes_serialization::enigma_deserialize(outputSet, iter, len1);
  ASSERT_EQ(outputSet, inputSet);

  std::tuple<int, bool, char> outputTuple;
  enigma::bytes_serialization::enigma_deserialize(outputTuple, iter, len1);
  ASSERT_EQ(outputTuple, inputTuple);

  std::queue<long> outputQueue;
  enigma::bytes_serialization::enigma_deserialize(outputQueue, iter, len1);
  ASSERT_EQ(outputQueue, inputQueue);
}

TEST(serialize_deserialize_functions, Test_stack1) {
  std::stack<int> inputStack;
  inputStack.push(1123);
  inputStack.push(232);
  inputStack.push(332);
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputStack, len, bytes);
  ASSERT_EQ(bytes.size(), 20);

  std::byte* iter = bytes.data();
  std::stack<int> outputStack;
  len = 0;

  enigma::bytes_serialization::enigma_deserialize(outputStack, iter, len);
  ASSERT_EQ(outputStack, inputStack);
}

TEST(serialize_deserialize_functions, Test_stack2) {
  std::stack<double> inputStack;
  inputStack.push(12.1232);
  inputStack.push(123.22);
  inputStack.push(342432.22);
  inputStack.push(12312312.2442);
  inputStack.push(12323.222);
  inputStack.push(23232.896);
  inputStack.push(323232.785875);
  inputStack.push(233.123);
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputStack, len, bytes);
  ASSERT_EQ(bytes.size(), 72);

  std::byte* iter = bytes.data();
  std::stack<double> outputStack;
  len = 0;

  enigma::bytes_serialization::enigma_deserialize(outputStack, iter, len);
  ASSERT_EQ(outputStack, inputStack);
}

TEST(serialize_deserialize_functions, Test_stack3) {
  std::stack<char> inputStack;
  inputStack.push('a');
  inputStack.push('b');
  inputStack.push('c');
  inputStack.push('d');
  inputStack.push('e');
  inputStack.push('f');
  inputStack.push('g');
  inputStack.push('h');
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputStack, len, bytes);
  ASSERT_EQ(bytes.size(), 16);

  std::byte* iter = bytes.data();
  std::stack<char> outputStack;
  len = 0;

  enigma::bytes_serialization::enigma_deserialize(outputStack, iter, len);
  ASSERT_EQ(outputStack, inputStack);
}

TEST(serialize_deserialize_functions, Test_stack4) {
  std::stack<bool> inputStack;
  inputStack.push(true);
  inputStack.push(false);
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputStack, len, bytes);
  ASSERT_EQ(bytes.size(), 10);

  std::byte* iter = bytes.data();
  std::stack<bool> outputStack;
  len = 0;

  enigma::bytes_serialization::enigma_deserialize(outputStack, iter, len);
  ASSERT_EQ(outputStack, inputStack);
}

TEST(serialize_deserialize_functions, Test_stack5) {
  std::stack<bool> inputStack;
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputStack, len, bytes);
  ASSERT_EQ(bytes.size(), 8);

  std::byte* iter = bytes.data();
  std::stack<bool> outputStack;
  len = 0;

  enigma::bytes_serialization::enigma_deserialize(outputStack, iter, len);
  ASSERT_EQ(outputStack, inputStack);
}

TEST(serialize_deserialize_functions, Test_stack6) {
  std::stack<int> inputStack;
  inputStack.push(123);
  inputStack.push(123);

  int input_int = 123;
  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputStack, len, bytes);
  ASSERT_EQ(bytes.size(), 16);

  enigma::bytes_serialization::enigma_serialize(input_int, len, bytes);
  ASSERT_EQ(bytes.size(), 20);

  std::byte* iter = bytes.data();

  std::stack<int> outputStack;
  len = 0;
  enigma::bytes_serialization::enigma_deserialize(outputStack, iter, len);
  ASSERT_EQ(outputStack, inputStack);

  int output_int;
  enigma::bytes_serialization::enigma_deserialize(output_int, iter, len);
  ASSERT_EQ(output_int, input_int);
}

TEST(serialize_deserialize_functions, Test_stack7) {
  std::stack<int> inputStack1;
  inputStack1.push(3);
  inputStack1.push(4);
  std::stack<char> inputStack2;
  inputStack2.push('c');
  inputStack2.push('d');

  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputStack1, len, bytes);
  ASSERT_EQ(bytes.size(), 16);

  enigma::bytes_serialization::enigma_serialize(inputStack2, len, bytes);
  ASSERT_EQ(bytes.size(), 26);

  std::byte* iter = bytes.data();

  std::stack<int> outputStack1;
  std::size_t len1 = 0;
  enigma::bytes_serialization::enigma_deserialize(outputStack1, iter, len1);
  ASSERT_EQ(outputStack1, inputStack1);

  std::stack<char> outputStack2;
  enigma::bytes_serialization::enigma_deserialize(outputStack2, iter, len1);
  ASSERT_EQ(outputStack2, inputStack2);
}

TEST(serialize_deserialize_functions, Test_stack8) {
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

  std::vector<std::byte> bytes;
  std::size_t len = 0;

  enigma::bytes_serialization::enigma_serialize(inputComplex, len, bytes);
  ASSERT_EQ(bytes.size(), 8);

  enigma::bytes_serialization::enigma_serialize(inputVector, len, bytes);
  ASSERT_EQ(bytes.size(), 40);

  enigma::bytes_serialization::enigma_serialize(inputMap, len, bytes);
  ASSERT_EQ(bytes.size(), 52);

  enigma::bytes_serialization::enigma_serialize(inputSet, len, bytes);
  ASSERT_EQ(bytes.size(), 68);

  enigma::bytes_serialization::enigma_serialize(inputTuple, len, bytes);
  ASSERT_EQ(bytes.size(), 82);

  enigma::bytes_serialization::enigma_serialize(inputQueue, len, bytes);
  ASSERT_EQ(bytes.size(), 114);

  enigma::bytes_serialization::enigma_serialize(inputStack, len, bytes);
  ASSERT_EQ(bytes.size(), 146);

  std::byte* iter = bytes.data();

  std::complex<int> outputComplex;
  std::size_t len1 = 0;
  enigma::bytes_serialization::enigma_deserialize(outputComplex, iter, len1);
  ASSERT_EQ(outputComplex, inputComplex);

  std::vector<double> outputVector;
  enigma::bytes_serialization::enigma_deserialize(outputVector, iter, len1);
  ASSERT_EQ(outputVector, inputVector);

  std::map<bool, char> outputMap;
  enigma::bytes_serialization::enigma_deserialize(outputMap, iter, len1);
  ASSERT_EQ(outputMap, inputMap);

  std::set<int> outputSet;
  enigma::bytes_serialization::enigma_deserialize(outputSet, iter, len1);
  ASSERT_EQ(outputSet, inputSet);

  std::tuple<int, bool, char> outputTuple;
  enigma::bytes_serialization::enigma_deserialize(outputTuple, iter, len1);
  ASSERT_EQ(outputTuple, inputTuple);

  std::queue<long> outputQueue;
  enigma::bytes_serialization::enigma_deserialize(outputQueue, iter, len1);
  ASSERT_EQ(outputQueue, inputQueue);

  std::stack<long> outputStack;
  enigma::bytes_serialization::enigma_deserialize(outputStack, iter, len1);
  ASSERT_EQ(outputStack, inputStack);
}
