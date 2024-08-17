cmake_minimum_required(VERSION 3.14)

set(SHARED_LIB "ENIGMAShared")
set(SHARED_LIB_DESCRIPTION "Enigma Shared Utilities")
set(CMAKE_DEBUG_POSTFIX d)

project(${SHARED_LIB} DESCRIPTION ${SHARED_LIB_DESCRIPTION})

option(GRPC_EXE "Path to grpc_cpp_plugin executable")

set(SHARED_SOURCES
   "eyaml/eyaml.cpp"
   "event_reader/event_parser.cpp"
   "event_reader/egm_events.cpp"
   "rectpacker/rectpack.cpp"
   "libpng-util/libpng-util.cpp"
   "ProtoYaml/proto-yaml.cpp"
)

add_library(${SHARED_LIB} ${LIB_TYPE} ${SHARED_SOURCES})

target_include_directories(${SHARED_LIB} PRIVATE "${ENIGMA_DIR}/CompilerSource")

if(MSVC)
  # Default MSVC warnings are overkill. Set to Lvl 1
  target_compile_options(${SHARED_LIB} PRIVATE /W1)
  # Fixes 'and' and 'or'
  target_compile_options(${SHARED_LIB} PRIVATE "/permissive-")
endif()

# Find libpng
find_package(PNG REQUIRED)
target_link_libraries(${SHARED_LIB} PRIVATE ${PNG_LIBRARY})

add_dependencies(${SHARED_LIB} "Protocols")

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${SHARED_LIB} PRIVATE yaml-cpp)
target_link_libraries(${SHARED_LIB} PRIVATE "Protocols")

# Find Protobuf
find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(${SHARED_LIB} PRIVATE protobuf::libprotobuf)

install(TARGETS ${SHARED_LIB} DESTINATION .)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${SHARED_LIB}.dir/Debug/${SHARED_LIB}.pdb" DESTINATION . OPTIONAL)
