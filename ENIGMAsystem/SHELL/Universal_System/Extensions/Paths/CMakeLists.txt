set(EXTENSION_SOURCES
  path_functions.cpp
  pathinit.cpp
  pathstruct.cpp
)

add_compile_definitions(PATH_EXT_SET)
add_library("EXTENSION_${EXT}" "${EXTENSION_SOURCES}")
