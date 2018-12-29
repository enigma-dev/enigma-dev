set(AUDIO_SOURCES
   XAadvanced.cpp
   XAbasic.cpp
   XAsystem.cpp
)

add_library("AUDIO_${AUDIO}" "${AUDIO_SOURCES}")
