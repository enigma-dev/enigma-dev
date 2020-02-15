cmake_minimum_required(VERSION 3.11)

set(LIB "Protocols")
set(LIB_VERSION "0.1")
set(LIB_DESCRIPTION "Protobuf interface for ENIGMA and GameMaker files")

set(CMAKE_DEBUG_POSTFIX d)

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0600)
endif(WIN32)

project(${LIB} VERSION ${LIB_VERSION} DESCRIPTION ${LIB_DESCRIPTION})

message(STATUS "Initial build flags:")
set(CompilerFlags
	  CMAKE_C_FLAGS_DEBUG
	  CMAKE_C_FLAGS_MINSIZEREL
	  CMAKE_C_FLAGS_RELEASE
	  CMAKE_C_FLAGS_RELWITHDEBINFO
	  CMAKE_CXX_FLAGS_DEBUG
	  CMAKE_CXX_FLAGS_MINSIZEREL
	  CMAKE_CXX_FLAGS_RELEASE
	  CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )

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

if(MSVC)
  add_library(${LIB} STATIC ${PROTO_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc)
  target_include_directories(${LIB} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/" "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
else()
  add_library(${LIB} SHARED ${PROTO_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc)
endif(MSVC)

set_target_properties(${LIB} PROPERTIES VERSION ${LIB_VERSION})

# Find GRPC
find_library(LIB_GRPC_UNSECURE NAMES grpc++_unsecure)
find_library(LIB_GRPC NAMES grpc)
find_library(LIB_GPR NAMES gpr)
target_link_libraries(${LIB} PRIVATE ${LIB_GRPC_UNSECURE} ${LIB_GRPC} ${LIB_GPR})

# Find Protobuf
include_directories(${Protobuf_INCLUDE_DIRS})
target_link_libraries(${LIB} PRIVATE ${Protobuf_LIBRARIES})

foreach(_proto ${PROTO_FILES})
    get_filename_component(_basename ${_proto} NAME_WE)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${_basename}.pb.h" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/.eobjs")
endforeach()

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.h" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/.eobjs")
get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ENIGMA_DIR "${ENIGMA_DIR}" PATH)
install(TARGETS ${LIB} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB}.dir/Debug/${LIB}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
