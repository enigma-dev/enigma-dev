cmake_minimum_required(VERSION 3.11)

set(LIB "Protocols")
set(LIB_VERSION "0.1")
set(LIB_DESCRIPTION "Protobuf interface for ENIGMA and GameMaker files")
set(PROTO_OUT_DIR "codegen/")

set(CMAKE_DEBUG_POSTFIX d)

add_definitions(-D_WIN32_WINNT=0x0600)

project(${LIB} VERSION ${LIB_VERSION} DESCRIPTION ${LIB_DESCRIPTION})
file(GLOB PROTO_FILES *.proto)
add_library(${LIB} STATIC ${PROTO_FILES} ${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc)
set_target_properties(${LIB} PROPERTIES VERSION ${LIB_VERSION})

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
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  message(STATUS "  '${CompilerFlag}': ${${CompilerFlag}}")
endforeach()

find_package(protobuf CONFIG REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(gRPC CONFIG REQUIRED)
target_include_directories(${LIB} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/" "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

protobuf_generate(TARGET ${LIB})

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc
  COMMAND  protobuf::protoc
  ARGS --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
	   --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
	   -I=${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include
	   --plugin=protoc-gen-grpc=${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/grpc/grpc_cpp_plugin.exe
	   --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
	   server.proto
  DEPENDS protobuf::protoc
  COMMENT "Generating grpc protocol buffer compiler on ${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc"
  VERBATIM )
 
add_custom_target(generate_grpc DEPENDS ${PROTO_FILES})

# Find GRPC
find_library(LIB_GRPC_UNSECURE NAMES grpc++_unsecure)
find_library(LIB_GRPC NAMES grpc)
find_library(LIB_GPR NAMES gpr)
target_link_libraries(${LIB} PRIVATE ${LIB_GRPC_UNSECURE} ${LIB_GRPC} ${LIB_GPR})

# Find Protobuf
find_library(LIB_PROTOBUF NAMES libprotobuf)
find_library(LIB_PROTOC NAMES libprotoc)
find_library(LIB_PROTOLITE NAMES libprotobuf-lite)
target_link_libraries(${LIB} PRIVATE ${LIB_PROTOC} ${LIB_PROTOBUF} ${LIB_PROTOLITE})

foreach(_proto ${PROTO_FILES})
    get_filename_component(_basename ${_proto} NAME_WE)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${_basename}.pb.h" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/codegen")
endforeach()

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.h" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/codegen")
get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ENIGMA_DIR "${ENIGMA_DIR}" PATH)
install(TARGETS ${LIB} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB}.dir/Debug/${LIB}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)