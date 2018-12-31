set(EXTENSION_SOURCES
  DSvideo.cpp
)

add_library("EXTENSION_${EXT}" "${EXTENSION_SOURCES}")
target_link_libraries("EXTENSION_${EXT}" strmiids ole32)
