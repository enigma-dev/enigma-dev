cmake_minimum_required(VERSION 3.11)

# MUST be done before call to 'project'
get_cmake_property(vars CACHE_VARIABLES)
foreach(var ${vars})
  get_property(currentHelpString CACHE "${var}" PROPERTY HELPSTRING)
    if("${currentHelpString}" MATCHES "No help, variable specified on the command line." OR "${currentHelpString}" STREQUAL "")
        # message("${var} = [${${var}}]  --  ${currentHelpString}") # uncomment to see the variables being processed
        list(APPEND CL_ARGS "-D${var}=${${var}}")
    endif()
endforeach()

project(SuperBuild)

include(ExternalProject)

ExternalProject_Add(
  libProtocols
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/CommandLine/protos"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/libProtocols"
  CMAKE_ARGS ${CL_ARGS}
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
  emake
  DEPENDS libEGM
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/CommandLine/emake"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/emake"
  CMAKE_ARGS ${CL_ARGS}
)

ExternalProject_Add(
  compiler
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/CompilerSource"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/compiler"
  CMAKE_ARGS ${CL_ARGS}
)