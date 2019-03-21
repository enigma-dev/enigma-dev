cmake_minimum_required(VERSION 3.11)

project(SuperBuild)

include(ExternalProject)

set(CL_ARGS "")
set(PB_ARGS "") 
if(USE_STATIC_LIBS)
  list(APPEND CL_ARGS 
       "-DUSE_STATIC_LIBS=ON"
       "-DBUILD_STATIC_LIBS=ON"
       "-DBUILD_SHARED_LIBS=OFF" 
  )
  
################ /!\ Google Big Gay /!\ ######################
# Protobuf blocks linking to emake and compiler at same time #
##############################################################

  list(APPEND PB_ARGS 
       "-DBUILD_STATIC_LIBS=ON"
       "-DBUILD_SHARED_LIBS=ON" 
  )
endif()

message("CMake args: ${CL_ARGS}")

ExternalProject_Add(
  libProtocols
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/shared/protos"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/libProtocols"
  CMAKE_ARGS  ${PB_ARGS}
)

ExternalProject_Add(
  libEGM
  DEPENDS libProtocols
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/CommandLine/libEGM"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/libEGM"
  CMAKE_ARGS ${CL_ARGS}
)

ExternalProject_Add(
  compiler
  DEPENDS libProtocols
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/CompilerSource"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/compiler"
  CMAKE_ARGS ${CL_ARGS}
) 

ExternalProject_Add(
  emake
  DEPENDS libEGM compiler
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/CommandLine/emake"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/emake"
  CMAKE_ARGS ${CL_ARGS}
)

ExternalProject_Add(
  gm2egm
  DEPENDS libEGM
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/CommandLine/gm2egm"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/gm2egm"
  CMAKE_ARGS ${CL_ARGS}
)

if(UNIX)
  ExternalProject_Add(
    test-runner
    DEPENDS libEGM
    DOWNLOAD_COMMAND ""
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/CommandLine/testing"
    BINARY_DIR "${CMAKE_SOURCE_DIR}/build/test-runner"
    CMAKE_ARGS ${CL_ARGS}
  )
endif()
