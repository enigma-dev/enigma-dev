cmake_minimum_required(VERSION 3.14)

set(-DAUDIO_SYSTEM "SDL")
set(AUDIO -DAUDIO_SYSTEM)

#add_subdirectory(OpenAL)
if(-DAUDIO_SYSTEM STREQUAL "OpenAL")
    add_subdirectory(OpenAL)
elseif(-DAUDIO_SYSTEM STREQUAL "DirectSound")
    add_subdirectory(DirectSound)
elseif(-DAUDIO_SYSTEM STREQUAL "SDL")
    add_subdirectory(SDL)
elseif(-DAUDIO_SYSTEMAUDIO STREQUAL "XAudio2")
    add_subdirectory(XAudio2)
else()
    add_subdirectory(None)
endif()


add_executable(${ENGINE_LIB} ${TARGET_LIB}.cxx)

target_link_libraries(${ENGINE_LIB} PUBLIC ${TARGET_LIB})

