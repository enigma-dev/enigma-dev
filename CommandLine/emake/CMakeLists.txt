cmake_minimum_required(VERSION 3.11)

set(EXE "emake")
set(EXE_DESCRIPTION "ENIGMA CLI")

project(${EXE})

file(GLOB EMAKE_SOURCES *.cpp)

get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ENIGMA_DIR "${ENIGMA_DIR}" PATH)

set(SHARED_SOURCES
   "${ENIGMA_DIR}/shared/eyaml/eyaml.cpp"
   "${ENIGMA_DIR}/shared/event_reader/event_parser.cpp"
   "${ENIGMA_DIR}/shared/libpng-util/libpng-util.cpp"
)

option(USE_STATIC_LIBS "Link Static Libraries" OFF)

include("${ENIGMA_DIR}/shared/BuildFlags.cmake")

if(WIN32)
  add_executable(${EXE} WIN32 ${EMAKE_SOURCES} ${SHARED_SOURCES})
else()
  add_executable(${EXE} ${EMAKE_SOURCES} ${SHARED_SOURCES})
endif(WIN32)

include_directories(${EXE} PRIVATE 
                    "${CMAKE_CURRENT_SOURCE_DIR}"
                    "${ENIGMA_DIR}/shared"
                    "${ENIGMA_DIR}/shared/libpng-util/"
                    "${ENIGMA_DIR}/CompilerSource"
                    "${ENIGMA_DIR}/shared/protos/"
                    "${ENIGMA_DIR}/shared/protos/codegen/"
)

include("${ENIGMA_DIR}/shared/FindEGMDeps.cmake")

if(NOT MSVC)
  # libdl
  target_link_libraries(${EXE} PRIVATE dl)
endif()

# Find Boost
if(USE_STATIC_LIBS)
  set(Boost_USE_STATIC_LIBS ON) # only find static libs
endif()

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
endif()

if(WIN32)
  target_link_libraries(${EXE} PRIVATE Ws2_32 Wldap32 Crypt32 Winmm Userenv Netapi32 version Dwmapi Imm32)
endif()

find_package(Boost COMPONENTS system program_options filesystem REQUIRED)
target_link_libraries(${EXE} PRIVATE Boost::program_options Boost::filesystem)

install(TARGETS ${EXE} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${EXE}.dir/Debug/${EXE}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${EXE}.dir/Debug/${EXE}.ilk" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
