cmake_minimum_required(VERSION 3.11)

set(TARGET "gm2egm")
set(TARGET_DESCRIPTION "GM to EGM converter")

project(${TARGET})

include("${ENIGMA_DIR}/shared/BuildFlags.cmake")

if(WIN32)
  add_executable(${TARGET} WIN32 "main.cpp")
else()
  add_executable(${TARGET} "main.cpp")
endif()

include_directories(${TARGET} PRIVATE 
                    "${ENIGMA_DIR}/CommandLine/libEGM/"
                    "${ENIGMA_DIR}/shared/protos/"
                    "${ENIGMA_DIR}/shared/protos/codegen/"
)

include("${ENIGMA_DIR}/shared/FindEGMDeps.cmake")

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
endif()

install(TARGETS ${TARGET} DESTINATION "${ENIGMA_DIR}")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/Debug/${TARGET}.pdb" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/Debug/${TARGET}.ilk" DESTINATION "${ENIGMA_DIR}" OPTIONAL)
