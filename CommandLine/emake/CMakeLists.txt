cmake_minimum_required(VERSION 3.11)

set(EXE "emake")
set(EXE_VERSION "0.1")
set(EXE_DESCRIPTION "ENIGMA CLI")

project(emake)

set(EMAKE_SOURCES
    EnigmaCallbacks.cpp
    EnigmaPlugin.cpp
    SOG.cpp
    Main.cpp
    Game.cpp
    Server.cpp
    OptionsParser.cpp
    Proto2ES.cpp
)

get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ENIGMA_DIR "${ENIGMA_DIR}" PATH)

set(SHARED_SOURCES
   ${ENIGMA_DIR}/shared/eyaml/eyaml.cpp
   ${ENIGMA_DIR}/shared/event_reader/event_parser.cpp
   ${ENIGMA_DIR}/shared/lodepng/lodepng.cpp
)

if(WIN32)
  add_executable(${EXE} WIN32 ${EMAKE_SOURCES} ${SHARED_SOURCES})
else()
  add_executable(${EXE} ${EMAKE_SOURCES} ${SHARED_SOURCES})
endif(WIN32)

include_directories(${LIB} PRIVATE 
                    "${CMAKE_CURRENT_SOURCE_DIR}"
                    "${ENIGMA_DIR}/shared"
                    "${ENIGMA_DIR}/shared/lodepng"
                    "${ENIGMA_DIR}/CompilerSource"
                    "${ENIGMA_DIR}/CommandLine/protos"
                    "${ENIGMA_DIR}/CommandLine/protos/codegen"
)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
endif(MSVC)

message(STATUS "Initial build flags:")
set(CompilerFlags
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_MINSIZEREL
    CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_MINSIZEREL
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )
foreach(CompilerFlag ${CompilerFlags})
  if(MSVC)
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  endif(MSVC)
  message(STATUS "  '${CompilerFlag}': ${${CompilerFlag}}")
endforeach()

# Find PugiXML
find_library(LIB_PUGIXML NAMES pugixml)
target_link_libraries(${EXE} PRIVATE ${LIB_PUGIXML})

# Find RapidJSON
find_package(RapidJSON REQUIRED)
include_directories(${EXE} PRIVATE ${RAPIDJSON_INCLUDE_DIRS})

# Find yaml-cpp
find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${EXE} PRIVATE yaml-cpp)

# Find libEGM
find_library(LIB_EGM NAMES EGM PATHS ${ENIGMA_DIR})
target_link_libraries(${EXE} PRIVATE ${LIB_EGM})

# Find libProtocols
find_library(LIB_PROTO NAMES Protocols PATHS ${ENIGMA_DIR})
target_link_libraries(${EXE} PRIVATE ${LIB_PROTO})

# Find GRPC
find_library(LIB_GRPC_UNSECURE NAMES grpc++_unsecure)
find_library(LIB_GRPC NAMES grpc)
find_library(LIB_GPR NAMES gpr)
target_link_libraries(${EXE} PRIVATE ${LIB_GRPC_UNSECURE} ${LIB_GRPC} ${LIB_GPR})

# Find Protobuf
include(FindProtobuf)
include_directories(${Protobuf_INCLUDE_DIRS})
target_link_libraries(${EXE} PRIVATE ${Protobuf_LIBRARIES})

# Find OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(${EXE} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Find zlib
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
target_link_libraries(${EXE} PRIVATE ${ZLIB_LIBRARIES})

if(NOT MSVC)
  # libdl
  target_link_libraries(${EXE} PRIVATE dl)
endif(NOT MSVC)

# Find Boost
if(MSVC)
  set(Boost_USE_STATIC_LIBS ON) # only find static libs
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
  target_link_libraries(${EXE} PRIVATE Ws2_32 Wldap32 Crypt32 Winmm Userenv Netapi32 version Dwmapi Imm32)
endif(MSVC)

find_package(Boost COMPONENTS system program_options filesystem REQUIRED)
target_link_libraries(${EXE} PRIVATE Boost::program_options Boost::filesystem)

install(TARGETS ${EXE} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${EXE}.dir/Debug/${EXE}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${EXE}.dir/Debug/${EXE}.ilk" DESTINATION "${ENIGMA_DIR}" OPTIONAL)