set(AUDIO_SOURCES
   DSbasic.cpp
   DSsystem.cpp
)

add_library("AUDIO_${AUDIO}" "${AUDIO_SOURCES}")

find_package(DSound REQUIRED)
target_link_libraries("AUDIO_${AUDIO}" "${DSOUND_LIBRARY}")
target_include_directories("AUDIO_${AUDIO}" PRIVATE "${DSOUND_INCLUDE_DIR}")
