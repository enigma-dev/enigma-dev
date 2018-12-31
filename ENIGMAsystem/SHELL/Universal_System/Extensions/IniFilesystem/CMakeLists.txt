set(EXTENSION_SOURCES
  IniFileIndex.cpp
  inifilesystem.cpp
)

add_library("EXTENSION_${EXT}" "${EXTENSION_SOURCES}")
