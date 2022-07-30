set(SRC_FILES 
    *.cpp
)

# getting flags from compiler
set(-DAUDIO_SYSTEM "SDL")
# set(AUDIO_SYSTEM -DAUDIO_SYSTEM)
set(AUDIO_SYSTEM "DirectSound")

message("this is my audio_system: ${AUDIO_SYSTEM}")
# checking which audio_system to link
# if(-DAUDIO_SYSTEM STREQUAL "OpenAL")
#     add_subdirectory(OpenAL)
# elseif(-DAUDIO_SYSTEM STREQUAL "DirectSound")
#     add_subdirectory(DirectSound)
# elseif(-DAUDIO_SYSTEM STREQUAL "SDL")
#     add_subdirectory(SDL)
# elseif(-DAUDIO_SYSTEMAUDIO STREQUAL "XAudio2")
#     add_subdirectory(XAudio2)
# else()
#     add_subdirectory(None)
# endif()

add_subdirectory(
    ${CMAKE_SOURCE_DIR}/Audio_Systems/${AUDIO_SYSTEM}/
)
add_library(${AUDIO_LIB}
    ${SRC_FILES}
    ${LIB_SRC_FILES}
)



# add_executable(${AUDIOSYS} ${AUDIO_SYS_SRC_FILES})

# target_link_libraries(${AUDIOSYS} PUBLIC ${TARGET_LIB})



