cmake_minimum_required(VERSION 3.14)

include_directories(Info/)

set(TARGET_LIB "OpenAL")

add_library(${TARGET_LIB} 
ALadvanced.cpp 
ALbasic.cpp 
ALsystem.cpp 
)



