cmake_minimum_required(VERSION 3.14)

set(TARGET "compileEGMf")
set(TARGET_DESCRIPTION "Enigma Compiler")

project(${TARGET} DESCRIPTION ${TARGET_DESCRIPTION})

add_definitions(-DNOMINMAX)

file(GLOB_RECURSE COMPILER_SOURCES *.cpp)
list(REMOVE_ITEM COMPILER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/standalone_main.cpp")

get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

set(SHARED_SOURCES
   "${ENIGMA_DIR}/shared/eyaml/eyaml.cpp"
   "${ENIGMA_DIR}/shared/event_reader/event_parser.cpp"
   "${ENIGMA_DIR}/shared/rectpacker/rectpack.cpp"
   "${ENIGMA_DIR}/shared/libpng-util/libpng-util.cpp"
)

add_library(${TARGET} SHARED ${COMPILER_SOURCES} ${SHARED_SOURCES})

include_directories(${TARGET} PRIVATE 
                   "${ENIGMA_DIR}/shared"
                   "${ENIGMA_DIR}/shared/libpng-util/"
                   "${CMAKE_CURRENT_SOURCE_DIR}" 
                   "${CMAKE_CURRENT_SOURCE_DIR}/JDI/src"
                   "${ENIGMA_DIR}/shared/protos/"
                   "${ENIGMA_DIR}/shared/protos/codegen/"
)

add_dependencies(${TARGET} "Protocols")

install(TARGETS ${TARGET} DESTINATION .)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB}.dir/Debug/${LIB}.pdb" DESTINATION . OPTIONAL)