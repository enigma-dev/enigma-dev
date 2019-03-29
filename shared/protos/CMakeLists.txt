cmake_minimum_required(VERSION 3.11)

set(TARGET "Protocols")
set(TARGET_DESCRIPTION "Protobuf interface for ENIGMA and GameMaker files")

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0600)
endif(WIN32)

project(${TARGET} DESCRIPTION ${TARGET_DESCRIPTION})

include("${ENIGMA_DIR}/shared/BuildFlags.cmake")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(NOT MSVC)
  set(GRPC_EXE "/usr/bin/grpc_cpp_plugin")
else()
  set(GRPC_EXE "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/grpc/grpc_cpp_plugin.exe")
  message("GRPC_EXE: ${GRPC_EXE}")
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
  add_library(${TARGET}-static STATIC ${PROTO_SRCS} "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc")
  install(TARGETS ${TARGET}-static DESTINATION "${ENIGMA_DIR}")
endif()

if(BUILD_SHARED_LIBS)
  add_library(${TARGET} SHARED ${PROTO_SRCS} "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc")
  install(TARGETS ${TARGET} DESTINATION "${ENIGMA_DIR}")
endif()

if(MSVC)
  include_directories("${CMAKE_CURRENT_BINARY_DIR}/" "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
endif()

# Find GRPC
find_library(LIB_GRPC_UNSECURE NAMES grpc++_unsecure)
find_library(LIB_GRPC NAMES grpc)
find_library(LIB_GPR NAMES gpr)
if(BUILD_STATIC_LIBS)
  target_link_libraries(${TARGET}-static PRIVATE ${LIB_GRPC_UNSECURE} ${LIB_GRPC} ${LIB_GPR})
else()
  target_link_libraries(${TARGET} PRIVATE ${LIB_GRPC_UNSECURE} ${LIB_GRPC} ${LIB_GPR})
endif()

# Find Protobuf
include(FindProtobuf)
include_directories(${Protobuf_INCLUDE_DIRS})
if(BUILD_STATIC_LIBS)
  target_link_libraries(${TARGET}-static PRIVATE ${Protobuf_LIBRARIES})
else()
  target_link_libraries(${TARGET} PRIVATE ${Protobuf_LIBRARIES})
endif()

set(PROTO_HEADERS "")

foreach(_proto ${PROTO_FILES})
  get_filename_component(_basename ${_proto} NAME_WE)
  list(APPEND PROTO_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/${_basename}.pb.h")
endforeach()

list(APPEND PROTO_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.h")

add_custom_target(CopyProtos)
foreach(file ${PROTO_HEADERS})
  get_filename_component(_basename ${file} NAME)
  add_custom_command(TARGET CopyProtos PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy ${file} "${ENIGMA_DIR}/shared/protos/codegen/${_basename}"
                     COMMENT "Installing generated protobuf headers ${file} to ${ENIGMA_DIR}/shared/protos/codegen"
                    )
endforeach()

if(BUILD_STATIC_LIBS)
  add_dependencies(CopyProtos ${TARGET}-static)
endif()

if(BUILD_SHARED_LIBS)
  add_dependencies(CopyProtos ${TARGET})
endif()

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/Debug/${TARGET}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}-static.dir/Debug/${TARGET}-static.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)