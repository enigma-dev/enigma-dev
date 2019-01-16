set(EXTENSION_SOURCES
  json.cpp
  json_reader.cpp
  json_value.cpp
  json_writer.cpp
)

add_library("EXTENSION_${EXTNAME}" "${EXTENSION_SOURCES}")
