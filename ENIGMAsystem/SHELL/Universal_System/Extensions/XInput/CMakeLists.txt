set(EXTENSION_SOURCES
  XIgamepad.cpp
)

add_library("EXTENSION_${EXT}" "${EXTENSION_SOURCES}")
target_link_libraries("EXTENSION_${EXT}" xinput)
