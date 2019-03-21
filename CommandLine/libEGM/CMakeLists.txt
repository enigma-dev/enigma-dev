cmake_minimum_required(VERSION 3.11)

set(LIB "EGM")
set(LIB_DESCRIPTION "Interface for reading and writing ENIGMA and GameMaker files")

project(${LIB} DESCRIPTION ${LIB_DESCRIPTION})

file(GLOB SRC_FILES *.cpp)
get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ENIGMA_DIR "${ENIGMA_DIR}" PATH)

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(BUILD_STATIC_LIBS "Build Static Libraries" OFF)

include("${ENIGMA_DIR}/shared/BuildFlags.cmake")

if(BUILD_STATIC_LIBS)
  add_library(${LIB}-static STATIC ${SRC_FILES} "${ENIGMA_DIR}/shared/libpng-util/libpng-util.cpp")
  set_target_properties(${LIB}-static PROPERTIES OUTPUT_NAME ${LIB})
  set_property(TARGET ${LIB}-static PROPERTY CXX_STANDARD 17)
  set_property(TARGET ${LIB}-static PROPERTY CXX_STANDARD_REQUIRED ON)
  install(TARGETS ${LIB}-static DESTINATION "${ENIGMA_DIR}")
endif()

if(BUILD_SHARED_LIBS)
  add_library(${LIB} SHARED ${SRC_FILES} "${ENIGMA_DIR}/shared/libpng-util/libpng-util.cpp")
  set_property(TARGET ${LIB} PROPERTY CXX_STANDARD 17)
  set_property(TARGET ${LIB} PROPERTY CXX_STANDARD_REQUIRED ON)
  install(TARGETS ${LIB} DESTINATION "${ENIGMA_DIR}")
endif()

include_directories(. ../ "${ENIGMA_DIR}/shared/" "${ENIGMA_DIR}/shared/protos/codegen" "${ENIGMA_DIR}/shared/libpng-util")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB}.dir/Debug/${LIB}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
