set(EXTENSION_SOURCES
  ttf.cpp
)

add_library("EXTENSION_${EXT}" "${EXTENSION_SOURCES}")
target_link_libraries("EXTENSION_${EXT}" freetype)
