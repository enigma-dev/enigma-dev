set(EXTENSION_SOURCES
  buttons.cpp
  windows.cpp
  skins.cpp
  toggles.cpp
  sliders.cpp
  styles.cpp
  labels.cpp
  groups.cpp
  scrollbars.cpp
  textboxes.cpp
  elements.cpp
  parents.cpp
)

add_library("EXTENSION_${EXTNAME}" "${EXTENSION_SOURCES}")
