cmake_minimum_required(VERSION 3.11)

set(LIB "compileEGMf")
set(LIB_DESCRIPTION "Enigma Compiler")

project(${LIB} DESCRIPTION ${LIB_DESCRIPTION})

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

add_library(${LIB} SHARED ${COMPILER_SOURCES} ${SHARED_SOURCES})

include_directories(${LIB} PRIVATE 
                   "${ENIGMA_DIR}/shared"
                   "${ENIGMA_DIR}/shared/libpng-util/"
                   "${CMAKE_CURRENT_SOURCE_DIR}" 
                   "${CMAKE_CURRENT_SOURCE_DIR}/JDI/src"
                   "${ENIGMA_DIR}/shared/protos/"
                   "${ENIGMA_DIR}/shared/protos/codegen/"
)

set_property(TARGET ${LIB} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${LIB} PROPERTY CXX_STANDARD_REQUIRED ON)

install(TARGETS ${LIB} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB}.dir/Debug/${LIB}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
