cmake_minimum_required(VERSION 3.14)

include_directories(Info/)

set(TARGET direct_sound)

# adding DirectSound as library
add_library(${TARGET} 
DSbasic.cpp 
DSsystem.cpp 
)

