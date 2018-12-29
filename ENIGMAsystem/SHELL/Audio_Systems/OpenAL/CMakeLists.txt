set(AUDIO_SOURCES
  ALsystem.cpp
  ALadvanced.cpp
  ALbasic.cpp
)

add_library("AUDIO_${AUDIO}" "${AUDIO_SOURCES}")

find_package(ALURE REQUIRED)
target_link_libraries("AUDIO_${AUDIO}" "${ALURE_LIBRARIES}")
target_include_directories("AUDIO_${AUDIO}" PRIVATE "${ALURE_INCLUDE_DIRS}")

find_package(OpenAL REQUIRED)
target_link_libraries("AUDIO_${AUDIO}" "${OPENAL_LIBRARY}")
target_include_directories("AUDIO_${AUDIO}" PRIVATE "${OPENAL_INCLUDE_DIR}")

include("${CMAKE_SOURCE_DIR}/cmakemodules/LinkCodecs.cmake")
