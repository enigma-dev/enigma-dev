# Find libProtocols
if(USE_STATIC_LIBS)
  target_link_libraries(${TARGET} PRIVATE Protocols-static)
else()
  target_link_libraries(${TARGET} PRIVATE Protocols)
endif()

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
