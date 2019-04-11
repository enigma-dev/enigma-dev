cmake_minimum_required(VERSION 3.11)

project(ENIGMA)

if (MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  string(FIND "${VCPKG_TARGET_TRIPLET}" "static" STATIC_TARGET)
  if (STATIC_TARGET)
    set(USE_STATIC_LIBS ON)
  endif()

  set(Protobuf_IMPORT_DIRS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
  message("Protobuf Dir: ${Protobuf_IMPORT_DIRS}")

endif()

add_definitions(-DCLI_ENABLE_EGM)

if(USE_STATIC_LIBS)
  set(USE_STATIC_LIBS ON)
  set(BUILD_STATIC_LIBS ON)
  set(BUILD_SHARED_LIBS OFF) 
endif()

set(ENIGMA_DIR "${CMAKE_SOURCE_DIR}")

add_subdirectory("${CMAKE_SOURCE_DIR}/CommandLine/libEGM")
add_subdirectory("${CMAKE_SOURCE_DIR}/CompilerSource")
add_subdirectory("${CMAKE_SOURCE_DIR}/CommandLine/emake")
add_subdirectory("${CMAKE_SOURCE_DIR}/CommandLine/gm2egm")

if(UNIX)
  add_subdirectory("${CMAKE_SOURCE_DIR}/CommandLine/testing")
endif()
