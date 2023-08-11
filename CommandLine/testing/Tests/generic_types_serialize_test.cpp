#include <gtest/gtest.h>
#include "../../../ENIGMAsystem/SHELL/Universal_System/Serialization/serialization.h"

TEST(byte_size_function, Test_vector) {

    std::vector<int> vec1={1,2,3,4,5};
    ASSERT_EQ(enigma::byte_size(vec1), 20);

    std::vector<double> vec4={12.2,13.3,1233213212.2,1232312322.123,232322,2};
    ASSERT_EQ(enigma::byte_size(vec4), 48);

    std::vector<char> vec2={'a','b','c','d','e'};
    ASSERT_EQ(enigma::byte_size(vec2), 5);

    std::vector<bool> vec3={true,false};
    ASSERT_EQ(enigma::byte_size(vec3), 2);
}

TEST(serialize_deserialize_functions, Test_vector1) {

    std::vector<int> inputVector = {12,123,342432,12312312,12323,23232,323232,233};
    std::vector<std::byte> bytes;
    std::size_t len = 0;
    
    enigma::enigma_serialize(inputVector, len, bytes);
    ASSERT_EQ(bytes.size(), 40);

    std::byte* iter = bytes.data();
    std::vector<int> outputVector;
    len=0;
    enigma::enigma_deserialize(outputVector, iter, len);
    
    ASSERT_EQ(outputVector, inputVector);
}

TEST(serialize_deserialize_functions, Test_vector2) {

    std::vector<double> inputVector = {12.1232,123.22,342432.22,12312312.2442,12323.222,23232.896,323232.785875,233.123};
    std::vector<std::byte> bytes;
    std::size_t len = 0;
    
    enigma::enigma_serialize(inputVector, len, bytes);
    ASSERT_EQ(bytes.size(), 72);

    std::byte* iter = bytes.data();
    std::vector<double> outputVector;
    len=0;
    enigma::enigma_deserialize(outputVector, iter, len);
    
    ASSERT_EQ(outputVector, inputVector);
}

TEST(serialize_deserialize_functions, Test_vector3) {

    std::vector<char> inputVector = {'a','b','c','d','e','f','g','h'};
    std::vector<std::byte> bytes;
    std::size_t len = 0;
    
    enigma::enigma_serialize(inputVector, len, bytes);
    ASSERT_EQ(bytes.size(), 16);

    std::byte* iter = bytes.data();
    std::vector<char> outputVector;
    len=0;
    enigma::enigma_deserialize(outputVector, iter, len);
    
    ASSERT_EQ(outputVector, inputVector);
}

TEST(serialize_deserialize_functions, Test_vector4) {

    std::vector<bool> inputVector = {true,false};
    std::vector<std::byte> bytes;
    std::size_t len = 0;
    
    enigma::enigma_serialize(inputVector, len, bytes);
    ASSERT_EQ(bytes.size(), 10);

    std::byte* iter = bytes.data();
    std::vector<bool> outputVector;
    len=0;
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
    len=0;
    enigma::enigma_deserialize(outputVector, iter, len);
    
    ASSERT_EQ(outputVector, inputVector);
}

TEST(serialize_deserialize_functions, Test_vector6) {

    std::vector<int> inputVector1 = {1,2};
    int input_int = 123;
    std::vector<std::byte> bytes;
    std::size_t len = 0;
    
    enigma::enigma_serialize(inputVector1, len, bytes);
    ASSERT_EQ(bytes.size(), 16);

    enigma::enigma_serialize(input_int, len, bytes);
    ASSERT_EQ(bytes.size(), 20);

    std::byte* iter = bytes.data();

    std::vector<int> outputVector1;
    len = 0;
    enigma::enigma_deserialize(outputVector1, iter, len);
    ASSERT_EQ(outputVector1, inputVector1);

    int output_int;
    enigma::enigma_deserialize(output_int, iter, len);
    ASSERT_EQ(output_int, input_int);
}

TEST(serialize_deserialize_functions, Test_vector7) {

    std::vector<int> inputVector1 = {1,2};
    std::vector<char> inputVector2 = {'a','b'};
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