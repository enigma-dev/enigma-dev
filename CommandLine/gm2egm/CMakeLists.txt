cmake_minimum_required(VERSION 3.11)

set(EXE "gm2egm")
set(EXE_DESCRIPTION "GM to EGM converter")

project(${EXE})

get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ENIGMA_DIR "${ENIGMA_DIR}" PATH)

include("${ENIGMA_DIR}/shared/BuildFlags.cmake")

if(WIN32)
  add_executable(${EXE} WIN32 "main.cpp")
else()
  add_executable(${EXE} "main.cpp")
endif()

include_directories(${EXE} PRIVATE 
                    "${ENIGMA_DIR}/CommandLine/libEGM/"
                    "${ENIGMA_DIR}/shared/protos/"
                    "${ENIGMA_DIR}/shared/protos/codegen/"
)

include("${ENIGMA_DIR}/shared/FindEGMDeps.cmake")

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
endif()

install(TARGETS ${EXE} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${EXE}.dir/Debug/${EXE}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${EXE}.dir/Debug/${EXE}.ilk" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
