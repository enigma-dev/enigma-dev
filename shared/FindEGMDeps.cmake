# Find libEGM
if(USE_STATIC_LIBS)
  find_library(LIB_EGM NAMES EGM-static PATHS ${ENIGMA_DIR})
else()
  find_library(LIB_EGM NAMES EGM PATHS ${ENIGMA_DIR})
endif()
target_link_libraries(${TARGET} PRIVATE ${LIB_EGM})

# Find PugiXML
find_library(LIB_PUGIXML NAMES pugixml)
target_link_libraries(${TARGET} PRIVATE ${LIB_PUGIXML})

# Find RapidJSON
find_package(RapidJSON REQUIRED)
include_directories(${TARGET} PRIVATE ${RAPIDJSON_INCLUDE_DIRS})

# Find yaml-cpp
find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${TARGET} PRIVATE yaml-cpp)

# Find GRPC
find_library(LIB_GRPC_UNSECURE NAMES grpc++_unsecure)
find_library(LIB_GRPC NAMES grpc)
find_library(LIB_GPR NAMES gpr)
find_library(LIB_CARES NAMES cares)
find_library(LIB_ADDR NAMES address_sorting)
target_link_libraries(${TARGET} PRIVATE 
${LIB_GRPC_UNSECURE} ${LIB_GRPC} ${LIB_GPR} ${LIB_CARES} ${LIB_ADDR})

# Find OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(${TARGET} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

if (NOT MSVC)
  # Find stdfs
  find_library(LIB_FS NAMES stdc++fs)
  target_link_libraries(${TARGET} PRIVATE ${LIB_FS})
endif()

include("${ENIGMA_DIR}/shared/FindProtoBufDeps.cmake")