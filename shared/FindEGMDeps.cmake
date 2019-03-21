if(USE_STATIC_LIBS)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib ${CMAKE_FIND_LIBRARY_SUFFIXES})
endif()

# Find libEGM
find_library(LIB_EGM NAMES EGM PATHS ${ENIGMA_DIR})
target_link_libraries(${EXE} PRIVATE ${LIB_EGM})

################ /!\ Google Big Gay /!\ ######################
# Protobuf blocks linking to emake and compiler at same time #
##############################################################
if(USE_STATIC_LIBS)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .so .dll .dylib)
endif()

# Find libProtocols
find_library(LIB_PROTO NAMES Protocols PATHS ${ENIGMA_DIR})
target_link_libraries(${EXE} PRIVATE ${LIB_PROTO})

if(USE_STATIC_LIBS)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib ${CMAKE_FIND_LIBRARY_SUFFIXES})
endif()

# Find PugiXML
find_library(LIB_PUGIXML NAMES pugixml)
target_link_libraries(${EXE} PRIVATE ${LIB_PUGIXML})

# Find RapidJSON
find_package(RapidJSON REQUIRED)
include_directories(${EXE} PRIVATE ${RAPIDJSON_INCLUDE_DIRS})

# Find yaml-cpp
find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${EXE} PRIVATE yaml-cpp)

# Find Protobuf
include(FindProtobuf)
include_directories(${Protobuf_INCLUDE_DIRS})
target_link_libraries(${EXE} PRIVATE ${Protobuf_LIBRARIES})

# Find libpng
find_package(PNG REQUIRED)
target_link_libraries(${EXE} PRIVATE ${PNG_LIBRARY})

# Find GRPC
find_library(LIB_GRPC_UNSECURE NAMES grpc++_unsecure)
find_library(LIB_GRPC NAMES grpc)
find_library(LIB_GPR NAMES gpr)
target_link_libraries(${EXE} PRIVATE ${LIB_GRPC_UNSECURE} ${LIB_GRPC} ${LIB_GPR})

# Find OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(${EXE} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Find zlib
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
target_link_libraries(${EXE} PRIVATE ${ZLIB_LIBRARIES})

# Find stdfs
find_library(LIB_FS NAMES stdc++fs)
target_link_libraries(${EXE} PRIVATE ${LIB_FS})
