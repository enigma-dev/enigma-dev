################ /!\ Google Big Gay /!\ ######################
# Protobuf blocks linking to emake and compiler at same time #
##############################################################
if(USE_STATIC_LIBS)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .so .lib .dylib)
endif()

# Find libProtocols
find_library(LIB_PROTO NAMES Protocols PATHS ${ENIGMA_DIR})
target_link_libraries(${TARGET} PRIVATE ${LIB_PROTO})

if(USE_STATIC_LIBS)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib ${CMAKE_FIND_LIBRARY_SUFFIXES})
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
