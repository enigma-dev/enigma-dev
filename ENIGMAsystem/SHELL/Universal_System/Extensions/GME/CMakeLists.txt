set(EXTENSION_SOURCES
  gme.cpp
)

add_library("EXTENSION_${EXTNAME}" "${EXTENSION_SOURCES}")
target_link_libraries("EXTENSION_${EXTNAME}" gme)
