cmake_minimum_required(VERSION 3.14)

include_directories(Info/)

set(TARGET_LIB "none")

message("this is ${TARGET_LIB}")

# adding none as module
add_library(${TARGET_LIB} 
    *.cpp
    *.h
)

