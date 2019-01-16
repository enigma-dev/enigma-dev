set(EXTENSION_SOURCES
  SB2Dfunctions.cpp
  SB2Dinstances.cpp
  SB2Djoints.cpp
  SB2Dupdate.cpp
)

add_library("EXTENSION_${EXTNAME}" "${EXTENSION_SOURCES}")
target_link_libraries("EXTENSION_${EXTNAME}" Box2D)
