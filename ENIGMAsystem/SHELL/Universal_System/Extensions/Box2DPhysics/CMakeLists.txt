set(EXTENSION_SOURCES
  B2Dfunctions.cpp
  B2Djoints.cpp
  B2Dshapes.cpp
  B2Dupdate.cpp
)

add_library("EXTENSION_${EXTNAME}" "${EXTENSION_SOURCES}")
target_link_libraries("EXTENSION_${EXTNAME}" Box2D)
