set(EXTENSION_SOURCES
  DSvideo.cpp
)

add_library("EXTENSION_${EXTNAME}" "${EXTENSION_SOURCES}")
target_link_libraries("EXTENSION_${EXTNAME}" strmiids ole32)
