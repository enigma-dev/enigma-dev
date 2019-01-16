set(EXTENSION_SOURCES
  PS_actions.cpp
  PS_effects.cpp
  PS_particle_attractor.cpp
  PS_particle_changer.cpp
  PS_particle_deflector.cpp
  PS_particle_depths.cpp
  PS_particle_destroyer.cpp
  PS_particle_emitter.cpp
  PS_particle_particles_apiimpl.cpp
  PS_particle_sprites.cpp
  PS_particle_system.cpp
  PS_particle_system_apiimpl.cpp
  PS_particle_system_manager.cpp
  PS_particle_type.cpp
  PS_particle_updatedraw.cpp
)

add_library("EXTENSION_${EXTNAME}" "${EXTENSION_SOURCES}")
