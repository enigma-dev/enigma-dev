
project (ENIGMA)
cmake_minimum_required (VERSION 2.8)

set (ENIGMA_VERSION_MAJOR 1)
set (ENIGMA_VERSION_MINOR 0)

set (PACKAGE_NAME ENIGMA)
set (PACKAGE_VERSION "${ENIGMA_VERSION_MAJOR}.${ENIGMA_VERSION_MINOR}")
set (PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set (PACKAGE_BUGREPORT "https://github.com/enigma-dev/enigma-dev/issues/")

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

# Increase warning levels when the compiler is GCC
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
endif ()

add_subdirectory (packages)
