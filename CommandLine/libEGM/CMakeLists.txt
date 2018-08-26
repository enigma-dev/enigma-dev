cmake_minimum_required(VERSION 3.11)

set(LIB "EGM")
set(LIB_VERSION "0.1")
set(LIB_DESCRIPTION "Interface for reading and writing ENIGMA and GameMaker files")

set(CMAKE_DEBUG_POSTFIX d)

project(${LIB} VERSION ${LIB_VERSION} DESCRIPTION ${LIB_DESCRIPTION})

file(GLOB SRC_FILES *.cpp)
get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ENIGMA_DIR "${ENIGMA_DIR}" PATH)
add_library(${LIB} STATIC ${SRC_FILES} "${ENIGMA_DIR}/shared/lodepng/lodepng.cpp")
set_property(TARGET ${LIB} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${LIB} PROPERTY CXX_STANDARD_REQUIRED ON)

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

find_package(pugixml CONFIG REQUIRED)
target_link_libraries(${LIB} PRIVATE pugixml)

find_package(RapidJSON REQUIRED)
include_directories(${LIB} PRIVATE ${RAPIDJSON_INCLUDE_DIRS})
target_link_libraries(${LIB} PRIVATE rapidjson)

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${LIB} PRIVATE yaml-cpp)

include_directories(. "${ENIGMA_DIR}/CommandLine/protos/" "${ENIGMA_DIR}/shared/lodepng")
link_directories(${ENIGMA_DIR})
target_link_libraries(${LIB} PRIVATE Protocols)

install(TARGETS ${LIB} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB}.dir/Debug/${LIB}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)