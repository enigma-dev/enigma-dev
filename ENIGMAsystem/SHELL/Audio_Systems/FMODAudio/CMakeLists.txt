set(AUDIO_SOURCES
   FMOD_main.cpp
)

add_library("AUDIO_${AUDIO}" "${AUDIO_SOURCES}")
find_package(FMOD REQUIRED)
target_link_libraries("AUDIO_${AUDIO}" "${FMOD_LIBRARIES}")
target_include_directories("AUDIO_${AUDIO}" PRIVATE "${FMOD_INCLUDE_DIRS}")
