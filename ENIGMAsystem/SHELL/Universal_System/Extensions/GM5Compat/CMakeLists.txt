set(EXTENSION_SOURCES
  GM5Drawing.cpp
  GM5Graphics.cpp
  GM5Placebo.cpp
  GM5Sound.cpp
  GM5Views.cpp
)

add_library("EXTENSION_${EXTNAME}" "${EXTENSION_SOURCES}")
