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

set(SHARED_LIB "ENIGMAShared" CACHE STRING "Shared library name")
set(SHARED_LIB_DESCRIPTION "Enigma Shared Utilities")
set(CMAKE_DEBUG_POSTFIX d)

project(${SHARED_LIB} DESCRIPTION ${SHARED_LIB_DESCRIPTION})

option(GRPC_EXE "Path to grpc_cpp_plugin executable")

set(SHARED_SOURCES
   "eyaml/eyaml.cpp"
   "event_reader/event_parser.cpp"
   "event_reader/egm_events.cpp"
   "rectpacker/rectpack.cpp"
   "libpng-util/libpng-util.cpp"
   "ProtoYaml/proto-yaml.cpp"
)

add_library(${SHARED_LIB} ${LIB_TYPE} ${SHARED_SOURCES})

target_include_directories(${SHARED_LIB} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/" "${ENIGMA_DIR}/CompilerSource")

if(MSVC)
  # Default MSVC warnings are overkill. Set to Lvl 1
  target_compile_options(${SHARED_LIB} PRIVATE /W1)
  # Fixes 'and' and 'or'
  target_compile_options(${SHARED_LIB} PRIVATE "/permissive-")
endif()

# Find libpng
find_package(PNG REQUIRED)
target_link_libraries(${SHARED_LIB} PRIVATE ${PNG_LIBRARY})

add_dependencies(${SHARED_LIB} ${LIB_PROTO})

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${SHARED_LIB} PRIVATE yaml-cpp)
target_link_libraries(${SHARED_LIB} PRIVATE ${LIB_PROTO})

# Find Protobuf
find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(${SHARED_LIB} PRIVATE protobuf::libprotobuf)

install(TARGETS ${SHARED_LIB} DESTINATION .)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${SHARED_LIB}.dir/Debug/${SHARED_LIB}.pdb" DESTINATION . OPTIONAL)
