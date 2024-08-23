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
