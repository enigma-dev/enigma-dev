set(DSLIB "directSoundLIB")
set(DSLIB_DESCRIPTION "Direct Sound Audio System")
# set(CMAKE_DEBUG_POSTFIX d)

project(${DSLIB} DESCRIPTION ${DSLIB_DESCRIPTION})

# include(CTest)
# enable_testing()

file(GLOB_RECURSE DSLIB_SOURCES *.cpp)

add_library(${DSLIB} ${DSLIB_SOURCES})

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
