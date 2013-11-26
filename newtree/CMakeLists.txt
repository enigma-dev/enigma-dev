
project (ENIGMA)
cmake_minimum_required (VERSION 2.8)

set (ENIGMA_VERSION_MAJOR 1)
set (ENIGMA_VERSION_MINOR 0)

set (PACKAGE_NAME ENIGMA)
set (PACKAGE_VERSION "${ENIGMA_VERSION_MAJOR}.${ENIGMA_VERSION_MINOR}")
set (PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set (PACKAGE_BUGREPORT "https://github.com/enigma-dev/enigma-dev/issues/")

add_subdirectory (packages)
