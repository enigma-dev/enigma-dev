cmake_minimum_required(VERSION 3.11)

set(TARGET "compileEGMf")
set(TARGET_DESCRIPTION "Enigma Compiler")

project(${TARGET} DESCRIPTION ${TARGET_DESCRIPTION})

add_definitions(-DNOMINMAX)

file(GLOB_RECURSE COMPILER_SOURCES *.cpp)
list(REMOVE_ITEM COMPILER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/standalone_main.cpp")

get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

include("${ENIGMA_DIR}/shared/BuildFlags.cmake")

set(SHARED_SOURCES
   "${ENIGMA_DIR}/shared/eyaml/eyaml.cpp"
   "${ENIGMA_DIR}/shared/event_reader/event_parser.cpp"
   "${ENIGMA_DIR}/shared/rectpacker/rectpack.cpp"
   "${ENIGMA_DIR}/shared/libpng-util/libpng-util.cpp"
)

set(CMAKE_SHARED_LIBRARY_PREFIX lib)

add_library(${TARGET} SHARED ${COMPILER_SOURCES} ${SHARED_SOURCES})

set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD_REQUIRED ON)

include_directories(${TARGET} PRIVATE 
                   "${ENIGMA_DIR}/shared"
                   "${ENIGMA_DIR}/shared/libpng-util/"
                   "${CMAKE_CURRENT_SOURCE_DIR}" 
                   "${CMAKE_CURRENT_SOURCE_DIR}/JDI/src"
                   "${ENIGMA_DIR}/shared/protos/"
                   "${ENIGMA_DIR}/shared/protos/codegen/"
)

include("${ENIGMA_DIR}/shared/FindProtobufDeps.cmake")

install(TARGETS ${TARGET} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/Debug/${TARGET}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
