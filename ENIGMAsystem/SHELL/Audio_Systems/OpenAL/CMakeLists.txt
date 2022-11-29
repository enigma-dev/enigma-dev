cmake_minimum_required(VERSION 3.14)

set(OpenALLIB "openALLIB")
set(OpenALLIB_DESCRIPTION "OpenAL Audio System")
# set(CMAKE_DEBUG_POSTFIX d)

project(${OpenALLIB} DESCRIPTION ${OpenALLIB_DESCRIPTION})

# include(CTest)
# enable_testing()

file(GLOB_RECURSE OpenALLIB_SOURCES *.cpp)

add_library(${OpenALLIB} ${OpenALLIB_SOURCES})

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
