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

set(COMPILER_LIB "compileEGMf" CACHE STRING "Compiler library name")
set(COMPILER_LIB_DESCRIPTION "Enigma Compiler")
set(CMAKE_DEBUG_POSTFIX d)

project(${COMPILER_LIB} DESCRIPTION ${COMPILER_LIB_DESCRIPTION})

add_definitions(-DNOMINMAX)

file(GLOB_RECURSE COMPILER_SOURCES *.cpp)
list(REMOVE_ITEM COMPILER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/standalone_main.cpp")

get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

add_library(${COMPILER_LIB} ${LIB_TYPE} ${COMPILER_SOURCES})

target_include_directories(${COMPILER_LIB} PRIVATE "JDI/src")

if(MSVC)
  # Default MSVC warnings are overkill. Set to Lvl 1
  target_compile_options(${COMPILER_LIB} PRIVATE /W1)
  # Fixes 'and' and 'or'
  target_compile_options(${COMPILER_LIB} PRIVATE "/permissive-")
endif()

add_dependencies(${COMPILER_LIB} ${LIB_PROTO} ${SHARED_LIB})
target_link_libraries(${COMPILER_LIB} PRIVATE ${LIB_PROTO} ${SHARED_LIB})

# Find Protobuf
find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(${LIB_PROTO} PRIVATE protobuf::libprotobuf)

# Find libpng
find_package(PNG REQUIRED)
target_link_libraries(${COMPILER_LIB} PRIVATE ${PNG_LIBRARY})

# Find zlib
find_package(ZLIB)
target_link_libraries(${COMPILER_LIB} PRIVATE ZLIB::ZLIB)

install(TARGETS ${COMPILER_LIB} DESTINATION .)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${COMPILER_LIB}.dir/Debug/${COMPILER_LIB}.pdb" DESTINATION . OPTIONAL)
