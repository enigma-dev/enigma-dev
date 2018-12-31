set(EXTENSION_SOURCES
  B3Dbodies.cpp
  B3Dshapes.cpp
  B3Dsoftbodies.cpp
  B3Dworlds.cpp
)

add_library("EXTENSION_${EXT}" "${EXTENSION_SOURCES}")
target_link_libraries("EXTENSION_${EXT}" BulletSoftBody BulletDynamics BulletCollision LinearMath)
