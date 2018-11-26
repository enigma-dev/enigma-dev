cmake_minimum_required(VERSION 3.11)

include(ExternalProject)

ExternalProject_Add(
  libProtocols
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/CommandLine/protos"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/libProtocols"
)

ExternalProject_Add(
  libEGM
  DEPENDS libProtocols
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/CommandLine/libEGM"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/libEGM"
)

ExternalProject_Add(
  emake
  DEPENDS libEGM
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/CommandLine/emake"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/emake"
)

ExternalProject_Add(
  compiler
  DOWNLOAD_COMMAND ""
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/CompilerSource"
  BINARY_DIR "${CMAKE_SOURCE_DIR}/build/compiler"
)
