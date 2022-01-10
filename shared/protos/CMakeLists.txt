cmake_minimum_required(VERSION 3.14)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(LIB_PROTO "Protocols")
set(LIB_PROTO_DESCRIPTION "Protobuf interface for ENIGMA and GameMaker files")

set(CMAKE_DEBUG_POSTFIX d)

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0600)
endif(WIN32)

project(${LIB_PROTO} DESCRIPTION ${LIB_PROTO_DESCRIPTION})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Hack to get msys path
if(WIN32 AND NOT MSVC)
  get_filename_component(MINGW_BIN ${CMAKE_C_COMPILER} DIRECTORY)
endif()

if(MSVC)
  set(GRPC_EXE "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/grpc/grpc_cpp_plugin.exe")
elseif(WIN32)
  set(GRPC_EXE "${MINGW_BIN}/grpc_cpp_plugin.exe")
else()
  set(GRPC_EXE "/usr/bin/grpc_cpp_plugin")
endif()

file(GLOB PROTO_FILES *.proto Configuration/*.proto)
include(FindProtobuf)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})


set(GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc
  COMMAND  protobuf::protoc
  ARGS --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
     --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
     -I=${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include
     --plugin=protoc-gen-grpc=${GRPC_EXE}
     --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
     server.proto
  DEPENDS protobuf::protoc
  COMMENT "Generating grpc protocol buffer compiler on ${GRPC_SRCS}"
  VERBATIM )

add_custom_target(generate_grpc DEPENDS ${PROTO_FILES})

add_library(${LIB_PROTO} ${LIB_TYPE} ${PROTO_SRCS} ${GRPC_SRCS})

if(MSVC)
  target_include_directories(${LIB_PROTO} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/" "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
  # Default MSVC warnings are overkill. Set to Lvl 1
  target_compile_options(${LIB_PROTO} PRIVATE /W1)
endif()

#Find gRPC
find_package(gRPC CONFIG REQUIRED)
target_link_libraries(${LIB_PROTO} PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++)

# Find Protobuf
include_directories(${Protobuf_INCLUDE_DIRS})
target_link_libraries(${LIB_PROTO} PRIVATE ${Protobuf_LIBRARIES})

install(TARGETS ${LIB_PROTO} DESTINATION .)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB_PROTO}.dir/Debug/${LIB_PROTO}.pdb" DESTINATION . OPTIONAL)
