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

set(COMPILER_LIB "compileEGMf" CACHE STRING "Compiler library name")
set(COMPILER_LIB_DESCRIPTION "Enigma Compiler")
set(CMAKE_DEBUG_POSTFIX d)

project(${COMPILER_LIB} DESCRIPTION ${COMPILER_LIB_DESCRIPTION})

add_definitions(-DNOMINMAX)

file(GLOB_RECURSE COMPILER_SOURCES *.cpp)
list(REMOVE_ITEM COMPILER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/standalone_main.cpp")
# Exclude JDI directory (replaced with clang)
list(FILTER COMPILER_SOURCES EXCLUDE REGEX ".*JDI/.*")

get_filename_component(ENIGMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

add_library(${COMPILER_LIB} ${LIB_TYPE} ${COMPILER_SOURCES})

# Find libclang
# Try CMake's FindClang module first
find_package(Clang QUIET)

if(Clang_FOUND)
  # Use CMake's FindClang module
  target_include_directories(${COMPILER_LIB} PRIVATE ${Clang_INCLUDE_DIRS})
  target_link_libraries(${COMPILER_LIB} PRIVATE ${Clang_LIBRARIES})
else()
  # Fallback: find libclang manually (for macOS CommandLineTools)
  find_library(LIBCLANG_LIBRARY
    NAMES clang
    PATHS
      /Library/Developer/CommandLineTools/usr/lib
      /opt/homebrew/opt/llvm/lib
      /usr/local/opt/llvm/lib
      /usr/lib
    NO_DEFAULT_PATH
  )
  
  find_path(LIBCLANG_INCLUDE_DIR
    NAMES clang-c/Index.h
    PATHS
      /opt/homebrew/opt/llvm/include
      /usr/local/opt/llvm/include
      /Library/Developer/CommandLineTools/usr/include
      /usr/include
  )
  
  if(LIBCLANG_LIBRARY AND LIBCLANG_INCLUDE_DIR)
    target_include_directories(${COMPILER_LIB} PRIVATE ${LIBCLANG_INCLUDE_DIR})
    target_link_libraries(${COMPILER_LIB} PRIVATE ${LIBCLANG_LIBRARY})
    
    # Set rpath so the library can be found at runtime
    get_filename_component(LIBCLANG_DIR ${LIBCLANG_LIBRARY} DIRECTORY)
    set_target_properties(${COMPILER_LIB} PROPERTIES
      INSTALL_RPATH "${LIBCLANG_DIR}"
      BUILD_WITH_INSTALL_RPATH TRUE
    )
    if(APPLE)
      # Fix the library reference after building
      add_custom_command(TARGET ${COMPILER_LIB} POST_BUILD
        COMMAND install_name_tool -change @rpath/libclang.dylib ${LIBCLANG_LIBRARY} $<TARGET_FILE:${COMPILER_LIB}>
        COMMENT "Fixing libclang library path for macOS"
      )
    endif()
    
    message(STATUS "Found libclang: ${LIBCLANG_LIBRARY}")
    message(STATUS "Found clang headers: ${LIBCLANG_INCLUDE_DIR}")
    message(STATUS "Setting rpath to: ${LIBCLANG_DIR}")
  else()
    message(FATAL_ERROR "libclang not found. Please install Xcode Command Line Tools or LLVM via Homebrew.")
  endif()
endif()

# Simple test executable for clang adapter (mirrors emake setup)
# Note: This must be built from the parent CMakeLists.txt where LIB_PROTO and SHARED_LIB are defined
if(DEFINED LIB_PROTO AND DEFINED SHARED_LIB)
  SET(CMAKE_SKIP_BUILD_RPATH ON)
  set(CMAKE_INSTALL_RPATH $ORIGIN)

  add_executable(test_clang languages/test_clang.cpp)

  if(MSVC)
    target_compile_options(test_clang PRIVATE /W1)
  endif()

  target_include_directories(test_clang PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

  # Link against the same libraries as emake
  add_dependencies(test_clang ${COMPILER_LIB} ${LIB_PROTO} ${SHARED_LIB})
  target_link_libraries(test_clang PRIVATE ${COMPILER_LIB} ${LIB_PROTO} ${SHARED_LIB})

  # Clang libraries
  if(Clang_FOUND)
    target_include_directories(test_clang PRIVATE ${Clang_INCLUDE_DIRS})
    target_link_libraries(test_clang PRIVATE ${Clang_LIBRARIES})
  else()
    target_include_directories(test_clang PRIVATE ${LIBCLANG_INCLUDE_DIR})
    target_link_libraries(test_clang PRIVATE ${LIBCLANG_LIBRARY})
    
    # Fix libclang path for macOS (same as compiler library)
    if(APPLE AND LIBCLANG_LIBRARY)
      get_filename_component(LIBCLANG_DIR ${LIBCLANG_LIBRARY} DIRECTORY)
      set_target_properties(test_clang PROPERTIES
        INSTALL_RPATH "${LIBCLANG_DIR}"
        BUILD_WITH_INSTALL_RPATH TRUE
      )
      # Fix the library reference after building
      add_custom_command(TARGET test_clang POST_BUILD
        COMMAND install_name_tool -change @rpath/libclang.dylib ${LIBCLANG_LIBRARY} $<TARGET_FILE:test_clang>
        COMMENT "Fixing libclang library path for test_clang on macOS"
      )
    endif()
  endif()

  install(TARGETS test_clang RUNTIME DESTINATION .)
endif()

if(MSVC)
  # Default MSVC warnings are overkill. Set to Lvl 1
  target_compile_options(${COMPILER_LIB} PRIVATE /W1)
  # Fixes 'and' and 'or'
  target_compile_options(${COMPILER_LIB} PRIVATE "/permissive-")
endif()

add_dependencies(${COMPILER_LIB} ${LIB_PROTO} ${SHARED_LIB})
target_link_libraries(${COMPILER_LIB} PRIVATE ${LIB_PROTO} ${SHARED_LIB})

# Find Protobuf
find_package(Protobuf CONFIG REQUIRED)
target_include_directories(${COMPILER_LIB} PRIVATE "${Protobuf_INCLUDE_DIRS}")
target_link_libraries(${COMPILER_LIB} PRIVATE protobuf::libprotobuf)

# Find libpng
find_package(PNG REQUIRED)
target_link_libraries(${COMPILER_LIB} PRIVATE ${PNG_LIBRARY})

# Find zlib
find_package(ZLIB)
target_link_libraries(${COMPILER_LIB} PRIVATE ZLIB::ZLIB)

install(TARGETS ${COMPILER_LIB} DESTINATION .)

# Fix libclang path for macOS after installation
if(APPLE AND LIBCLANG_LIBRARY)
  install(CODE "
    execute_process(
      COMMAND install_name_tool -change @rpath/libclang.dylib ${LIBCLANG_LIBRARY} \$<TARGET_FILE:${COMPILER_LIB}>
      WORKING_DIRECTORY \${CMAKE_INSTALL_PREFIX}
      ERROR_QUIET
    )
  ")
endif()

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${COMPILER_LIB}.dir/Debug/${COMPILER_LIB}.pdb" DESTINATION . OPTIONAL)
