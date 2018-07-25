#!/bin/bash

output_location="$1"
vertex_prefix="$2"
vertex_body="$3"
fragment_prefix="$4"
fragment_body="$5"

function write_shader_source() {
  file_name="$1"
  variable_name="$2"
  preserve_empty_lines="$3"
  echo "const char* ${variable_name}=" >> $output_location
  while read p; do
    # in the shader prefix we don't care to preserve empty lines
    # because it always ends with a line directive so that line
    # numbers in the body match the ones the user sees in the IDE
    if [[ ! -z ${p::-1} ]] || [ $preserve_empty_lines = true ]; then
      echo -n '"' >> "$output_location"
      echo -n "${p::-1}" >> "$output_location"
      echo '\n"' >> "$output_location"
    fi
  done < "$file_name"
  echo "\"\\n\";" >> "$output_location"
}

echo "/// Combined shader codes" > "$output_location"
write_shader_source "$vertex_prefix" "vertex_prefix" false
write_shader_source "$vertex_body" "vertex_body" true
write_shader_source "$fragment_prefix" "fragment_prefix" false
write_shader_source "$fragment_body" "fragment_body" true
