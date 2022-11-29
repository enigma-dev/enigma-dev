cmake_minimum_required(VERSION 3.14)

set(NLIB "noneLIB")
set(NLIB_DESCRIPTION "No Audio System")
# set(CMAKE_DEBUG_POSTFIX d)

project(${NLIB} DESCRIPTION ${NLIB_DESCRIPTION})

# include(CTest)
# enable_testing()

file(GLOB_RECURSE NLIB_SOURCES *.cpp)

add_library(${NLIB} ${NLIB_SOURCES})

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
