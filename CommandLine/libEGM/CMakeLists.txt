cmake_minimum_required(VERSION 3.14)

set(LIB_EGM "EGM")
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

include(FindProtobuf)
target_link_libraries(${LIB_EGM} PRIVATE ${Protobuf_LIBRARY})

add_dependencies(${LIB_EGM} "Protocols" "ENIGMAShared")
target_link_libraries(${LIB_EGM} PRIVATE "Protocols" "ENIGMAShared")

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(${LIB_EGM} PRIVATE ${PNG_LIBRARY})

find_package(ZLIB)
target_link_libraries(${LIB_EGM} PRIVATE ${ZLIB_LIBRARIES})

install(TARGETS ${LIB_EGM} DESTINATION .)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB_EGM}.dir/Debug/${LIB_EGM}.pdb" DESTINATION . OPTIONAL)
