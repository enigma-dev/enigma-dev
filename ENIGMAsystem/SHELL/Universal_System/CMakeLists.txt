set(UNIVERSAL_SYS_LIB "universal_sys_lib")
set(UNIVERSAL_SYS_LIB_DESCRIPTION "Universal System")

project(${UNIVERSAL_SYS_LIB} DESCRIPTION ${UNIVERSAL_SYS_LIB_DESCRIPTION})

file(GLOB_RECURSE UNIVERSAL_SYS_LIB_SOURCES 
    *.cpp 
    Instances/*.cpp 
    Object_Tiers/*.cpp 
    Resources/*.cpp)

add_library(${UNIVERSAL_SYS_LIB} ${UNIVERSAL_SYS_LIB_SOURCES})
