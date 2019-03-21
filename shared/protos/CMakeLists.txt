cmake_minimum_required(VERSION 3.11)

set(LIB "Protocols")
set(LIB_DESCRIPTION "Protobuf interface for ENIGMA and GameMaker files")

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0600)
endif(WIN32)

project(${LIB} DESCRIPTION ${LIB_DESCRIPTION})

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(BUILD_STATIC_LIBS "Build Static Libraries" OFF)

get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ENIGMA_DIR "${ENIGMA_DIR}" PATH)

include("${ENIGMA_DIR}/shared/BuildFlags.cmake")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(NOT MSVC)
  set(GRPC_EXE "/usr/bin/grpc_cpp_plugin")
else()
  set(GRPC_EXE "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/grpc/grpc_cpp_plugin.exe")
endif(NOT MSVC)

file(GLOB PROTO_FILES *.proto)
include(FindProtobuf)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

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
  COMMENT "Generating grpc protocol buffer compiler on ${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc"
  VERBATIM )
 
add_custom_target(generate_grpc DEPENDS ${PROTO_FILES})

if(BUILD_STATIC_LIBS)
  add_library(${LIB}-static STATIC ${PROTO_SRCS} "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc")
  set_target_properties(${LIB}-static PROPERTIES OUTPUT_NAME ${LIB})
  install(TARGETS ${LIB}-static DESTINATION "${ENIGMA_DIR}")
endif()

if(BUILD_SHARED_LIBS)
  add_library(${LIB} SHARED ${PROTO_SRCS} "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc")
  install(TARGETS ${LIB} DESTINATION "${ENIGMA_DIR}")
endif()

if(MSVC)
  target_include_directories(${LIB} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/" "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
endif()

# Find Protobuf
include_directories(${Protobuf_INCLUDE_DIRS})

foreach(_proto ${PROTO_FILES})
  get_filename_component(_basename ${_proto} NAME_WE)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${_basename}.pb.h" DESTINATION "${ENIGMA_DIR}/shared/protos/codegen")
endforeach()

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.h" DESTINATION "${ENIGMA_DIR}/shared/protos/codegen")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB}.dir/Debug/${LIB}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
