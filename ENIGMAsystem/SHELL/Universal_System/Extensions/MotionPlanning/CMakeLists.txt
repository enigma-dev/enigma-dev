set(EXTENSION_SOURCES
  motion_planning.cpp
  motion_planning_struct.cpp
  mp_movement.cpp
)

add_library("EXTENSION_${EXTNAME}" "${EXTENSION_SOURCES}")
