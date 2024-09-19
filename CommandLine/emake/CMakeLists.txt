###################################################################################
##                                                                               ##
##  Copyright (C) 2024 Saif Kandil (k0T0z)                                       ##
##                                                                               ##
##  This file is a part of the ENIGMA Development Environment.                   ##
##                                                                               ##
##                                                                               ##
##  ENIGMA is free software: you can redistribute it and/or modify it under the  ##
##  terms of the GNU General Public License as published by the Free Software    ##
##  Foundation, version 3 of the license or any later version.                   ##
##                                                                               ##
##  This application and its source code is distributed AS-IS, WITHOUT ANY       ##
##  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS    ##
##  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more        ##
##  details.                                                                     ##
##                                                                               ##
##  You should have recieved a copy of the GNU General Public License along      ##
##  with this code. If not, see <http://www.gnu.org/licenses/>                   ##
##                                                                               ##
##  ENIGMA is an environment designed to create games and other programs with a  ##
##  high-level, fully compilable language. Developers of ENIGMA or anything      ##
##  associated with ENIGMA are in no way responsible for its users or            ##
##  applications created by its users, or damages caused by the environment      ##
##  or programs made in the environment.                                         ##
##                                                                               ##
###################################################################################

cmake_minimum_required(VERSION 3.14)

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Build type not set - defaulting to Debug")
  set(
    CMAKE_BUILD_TYPE "Debug"
    CACHE
      STRING
      "Choose the type of build from:  Debug Release MinSizeRel RelWithDebInfo."
    FORCE)
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  set(CLI_TARGET "emake-debug" CACHE STRING "CLI target name")
else()
  set(CLI_TARGET "emake" CACHE STRING "CLI target name")
endif()

set(CLI_TARGET_DESCRIPTION "ENIGMA CLI")
set(Boost_NO_BOOST_CMAKE ON)

project(emake)

file(GLOB EMAKE_SOURCES *.cpp)

SET(CMAKE_SKIP_BUILD_RPATH ON)
set(CMAKE_INSTALL_RPATH $ORIGIN)

add_executable(${CLI_TARGET} ${EMAKE_SOURCES})

if(MSVC)
  set(CMAKE_TARGET_LINKER_FLAGS "${CLI_TARGET}" "/SUBSYSTEM:CONSOLE")
endif()

if(MSVC)
    # Default MSVC warnings are overkill. Set to Lvl 1
    target_compile_options(${CLI_TARGET} PRIVATE /W1)
endif()

target_include_directories(${CLI_TARGET} PRIVATE "${ENIGMA_DIR}/CompilerSource")

# Our libs
add_dependencies(${CLI_TARGET} ${LIB_EGM} ${COMPILER_LIB} ${LIB_PROTO} ${SHARED_LIB})
target_link_libraries(${CLI_TARGET} PRIVATE ${LIB_EGM} ${COMPILER_LIB} ${LIB_PROTO} ${SHARED_LIB})

# Find PugiXML
find_library(LIB_PUGIXML NAMES pugixml)
target_link_libraries(${CLI_TARGET} PRIVATE ${LIB_PUGIXML})

# Find RapidJSON
find_package(RapidJSON REQUIRED)
include_directories(${CLI_TARGET} PRIVATE ${RAPIDJSON_INCLUDE_DIRS})

# Find yaml-cpp
find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${CLI_TARGET} PRIVATE yaml-cpp)

#Find gRPC
find_package(gRPC CONFIG REQUIRED)
target_link_libraries(${CLI_TARGET} PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++)

# Find Protobuf
find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(${CLI_TARGET} PRIVATE protobuf::libprotobuf)

# Find absl
find_package(absl CONFIG REQUIRED)
target_link_libraries(${CLI_TARGET} PRIVATE absl::log_internal_check_op)
# Use the one below if you installed abseil manually
# target_link_libraries(${CLI_TARGET} PRIVATE abseil_dll)

# Find OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(${CLI_TARGET} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Find FreeType
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${CLI_TARGET} PRIVATE ${FREETYPE_LIBRARIES})

# Find JPEG
find_library(LIB_JPEG NAMES jpeg)
target_link_libraries(${CLI_TARGET} PRIVATE ${LIB_JPEG})

# Find libpng
find_package(PNG REQUIRED)
target_link_libraries(${CLI_TARGET} PRIVATE ${PNG_LIBRARY})

# Find HarfBuzz
find_library(LIB_HARFBUZZ NAMES harfbuzz)
target_link_libraries(${CLI_TARGET} PRIVATE ${LIB_HARFBUZZ})

# Find Pcre2
find_library(LIB_PCRE2 NAMES "pcre2-16")
target_link_libraries(${CLI_TARGET} PRIVATE ${LIB_PCRE2})

# Find double-conversion
find_library(LIB_DOUBLE_CONVERSION NAMES double-conversion)
target_link_libraries(${CLI_TARGET} PRIVATE ${LIB_DOUBLE_CONVERSION})

if(WIN32)
  # Windows is a turd
  target_link_libraries(${CLI_TARGET} PRIVATE Ws2_32 Wtsapi32 Wldap32 Crypt32 Winmm Userenv Netapi32 version Dwmapi Imm32)
endif(WIN32)

# Find Boost
# https://cmake.org/cmake/help/latest/policy/CMP0167.html
find_package(Boost CONFIG COMPONENTS system program_options filesystem REQUIRED)
target_link_libraries(${CLI_TARGET} PRIVATE Boost::program_options)

install(TARGETS ${CLI_TARGET} RUNTIME DESTINATION .)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${CLI_TARGET}.dir/Debug/${CLI_TARGET}.pdb" DESTINATION . OPTIONAL)
