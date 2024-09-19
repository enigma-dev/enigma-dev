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

set(LIB_EGM "EGM" CACHE STRING "EGM library name")
set(LIB_EGM_DESCRIPTION "Interface for reading and writing ENIGMA and GameMaker files")

set(CMAKE_DEBUG_POSTFIX d)

project(${LIB_EGM} DESCRIPTION ${LIB_EGM_DESCRIPTION})

file(GLOB_RECURSE SRC_FILES *.cpp)

add_library(${LIB_EGM} ${LIB_TYPE} ${SRC_FILES})

if(MSVC)
  # Default MSVC warnings are overkill. Set to Lvl 1
  target_compile_options(${LIB_EGM} PRIVATE /W1)
endif()

find_package(pugixml CONFIG REQUIRED)
target_link_libraries(${LIB_EGM} PRIVATE pugixml)

find_package(RapidJSON REQUIRED)
include_directories(${LIB_EGM} PRIVATE ${RAPIDJSON_INCLUDE_DIRS})

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${LIB_EGM} PRIVATE yaml-cpp)

include_directories(. ../ "${CMAKE_BINARY_DIR}/Submodules/enigma-dev/shared/protos/")

# Find Protobuf
find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(${LIB_EGM} PRIVATE protobuf::libprotobuf)

add_dependencies(${LIB_EGM} ${LIB_PROTO} ${SHARED_LIB})
target_link_libraries(${LIB_EGM} PRIVATE ${LIB_PROTO} ${SHARED_LIB})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(${LIB_EGM} PRIVATE ${PNG_LIBRARY})

find_package(ZLIB)
target_link_libraries(${LIB_EGM} PRIVATE ${ZLIB_LIBRARIES})

install(TARGETS ${LIB_EGM} DESTINATION .)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB_EGM}.dir/Debug/${LIB_EGM}.pdb" DESTINATION . OPTIONAL)
