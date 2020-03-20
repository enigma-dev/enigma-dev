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

if(MSVC)
    # Default MSVC warnings are overkill. Set to Lvl 2
    target_compile_options(${TARGET} PRIVATE /W1)
    # Fixes 'and' and 'or'
    target_compile_options(${TARGET} PRIVATE "/permissive-")
endif()

add_dependencies(${TARGET} "Protocols")

install(TARGETS ${TARGET} DESTINATION .)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB}.dir/Debug/${LIB}.pdb" DESTINATION . OPTIONAL)

target_link_libraries(${TARGET} PRIVATE "Protocols")

# Find Protobuf
include(FindProtobuf)
target_link_libraries(${TARGET} PRIVATE ${Protobuf_LIBRARIES})

# Find libpng
find_package(PNG REQUIRED)
target_link_libraries(${TARGET} PRIVATE ${PNG_LIBRARY})

# Find zlib
find_package(ZLIB)
target_link_libraries(${TARGET} PRIVATE ZLIB::ZLIB)