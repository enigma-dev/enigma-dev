set(EXTENSION_SOURCES
  SB2Dfunctions.cpp
  SB2Dinstances.cpp
  SB2Djoints.cpp
  SB2Dupdate.cpp
)

add_library("EXTENSION_${EXT}" "${EXTENSION_SOURCES}")
target_link_libraries("EXTENSION_${EXT}" Box2D)
