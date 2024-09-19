###################################################################################
##                                                                               ##
##  Copyright (C) 2024 Saif Kandil (k0T0z)                                       ##
##                                                                               ##
##  This file is a part of the ENIGMA Development Environment.                   ##
##                                                                               ##
##                                                                               ##
##  ENIGMA is free software: you can redistribute it and/or modify it under the  ##
##  terms of the GNU General Public License as published by the Free Software    ##
##  Foundation, version 3 of the license or any later version.                   ##
##                                                                               ##
##  This application and its source code is distributed AS-IS, WITHOUT ANY       ##
##  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS    ##
##  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more        ##
##  details.                                                                     ##
##                                                                               ##
##  You should have recieved a copy of the GNU General Public License along      ##
##  with this code. If not, see <http://www.gnu.org/licenses/>                   ##
##                                                                               ##
##  ENIGMA is an environment designed to create games and other programs with a  ##
##  high-level, fully compilable language. Developers of ENIGMA or anything      ##
##  associated with ENIGMA are in no way responsible for its users or            ##
##  applications created by its users, or damages caused by the environment      ##
##  or programs made in the environment.                                         ##
##                                                                               ##
###################################################################################

cmake_minimum_required(VERSION 3.14)

set(LIB_PROTO "Protocols" CACHE STRING "Protobuf library name")
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

if (NOT GRPC_EXE)
  if(MSVC)
    set(GRPC_EXE "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/grpc/grpc_cpp_plugin.exe")
  elseif(WIN32)
    set(GRPC_EXE "${MINGW_BIN}/grpc_cpp_plugin.exe")
  elseif(APPLE)
    set(GRPC_EXE "/usr/local/opt/grpc/bin/grpc_cpp_plugin")
  else()
    set(GRPC_EXE "/usr/bin/grpc_cpp_plugin")
  endif()
endif()

if (NOT EXISTS ${GRPC_EXE})
  message(FATAL_ERROR "${GRPC_EXE} not found. Please install gRPC and set GRPC_EXE to the grpc_cpp_plugin executable.")
endif()

file(GLOB PROTO_FILES *.proto)

set(GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/server.grpc.pb.cc")

add_library(${LIB_PROTO} ${LIB_TYPE} ${PROTO_SRCS} ${GRPC_SRCS})

if(MSVC)
  target_include_directories(${LIB_PROTO} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/" "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
  # Default MSVC warnings are overkill. Set to Lvl 1
  target_compile_options(${LIB_PROTO} PRIVATE /W1)
endif()

# Find gRPC
find_package(gRPC CONFIG REQUIRED)
target_link_libraries(${LIB_PROTO} PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++)

# Find Protobuf
find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(${LIB_PROTO} PRIVATE protobuf::libprotobuf)

# https://stackoverflow.com/a/54254098/14629018
protobuf_generate(
  LANGUAGE cpp
  TARGET ${LIB_PROTO}
  PROTOS ${PROTO_FILES}
)

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

install(TARGETS ${LIB_PROTO} DESTINATION .)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB_PROTO}.dir/Debug/${LIB_PROTO}.pdb" DESTINATION . OPTIONAL)
