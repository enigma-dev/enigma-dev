set(AUDIO_SOURCES
  SFMLbasic.cpp
  SFMLadvanced.cpp
)

add_library("AUDIO_${AUDIO}" "${AUDIO_SOURCES}")

find_package(SFML 2 REQUIRED audio system)
target_include_directories("AUDIO_${AUDIO}" PRIVATE "${SFML_INCLUDE_DIR}")
target_link_libraries("AUDIO_${AUDIO}" "${SFML_LIBRARIES}" "${SFML_DEPENDENCIES}")
