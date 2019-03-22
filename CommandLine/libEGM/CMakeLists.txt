cmake_minimum_required(VERSION 3.11)

set(TARGET "EGM")
set(TARGET_DESCRIPTION "Interface for reading and writing ENIGMA and GameMaker files")

project(${TARGET} DESCRIPTION ${TARGET_DESCRIPTION})

file(GLOB SRC_FILES *.cpp)
get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ENIGMA_DIR "${ENIGMA_DIR}" PATH)

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(BUILD_STATIC_LIBS "Build Static Libraries" OFF)

include("${ENIGMA_DIR}/shared/BuildFlags.cmake")

if(BUILD_STATIC_LIBS)
  add_library(${TARGET}-static STATIC ${SRC_FILES} "${ENIGMA_DIR}/shared/libpng-util/libpng-util.cpp")
  set_property(TARGET ${TARGET}-static PROPERTY CXX_STANDARD 17)
  set_property(TARGET ${TARGET}-static PROPERTY CXX_STANDARD_REQUIRED ON)
  install(TARGETS ${TARGET}-static DESTINATION "${ENIGMA_DIR}")
endif()

if(BUILD_SHARED_LIBS)
  add_library(${TARGET} SHARED ${SRC_FILES} "${ENIGMA_DIR}/shared/libpng-util/libpng-util.cpp")
  set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 17)
  set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD_REQUIRED ON)
  install(TARGETS ${TARGET} DESTINATION "${ENIGMA_DIR}")
endif()

include_directories(. ../ "${ENIGMA_DIR}/shared/" "${ENIGMA_DIR}/shared/protos/codegen" "${ENIGMA_DIR}/shared/libpng-util")

# Find Protobuf
include(FindProtobuf)
include_directories(${Protobuf_INCLUDE_DIRS})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/Debug/${TARGET}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}-static.dir/Debug/${TARGET}-static.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
