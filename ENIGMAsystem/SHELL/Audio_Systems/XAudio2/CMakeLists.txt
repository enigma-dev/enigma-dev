set(X2LIB "xAudio2LIB")
set(X2LIB_DESCRIPTION "XAudio2 Audio System")
# set(CMAKE_DEBUG_POSTFIX d)

project(${X2LIB} DESCRIPTION ${X2LIB_DESCRIPTION})

# include(CTest)
# enable_testing()

file(GLOB_RECURSE X2LIB_SOURCES *.cpp)

add_library(${X2LIB} ${X2LIB_SOURCES})

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
