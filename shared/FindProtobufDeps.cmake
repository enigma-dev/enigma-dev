# Find libProtocols
if(USE_STATIC_LIBS)
  find_library(LIB_PROTO NAMES Protocols-static PATHS ${ENIGMA_DIR})
else()
  find_library(LIB_PROTO NAMES Protocols PATHS ${ENIGMA_DIR})
endif()
target_link_libraries(${TARGET} PRIVATE ${LIB_PROTO})

# Find Protobuf
include(FindProtobuf)
include_directories(${Protobuf_INCLUDE_DIRS})
target_link_libraries(${TARGET} PRIVATE ${Protobuf_LIBRARIES})

# Find libpng
find_package(PNG REQUIRED)
target_link_libraries(${TARGET} PRIVATE ${PNG_LIBRARY})

# Find zlib
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
target_link_libraries(${TARGET} PRIVATE ${ZLIB_LIBRARIES})
