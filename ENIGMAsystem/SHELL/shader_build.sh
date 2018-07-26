#!/bin/bash

output_location="$1"
shader_version="$2"
vertex_prefix="$3"
vertex_body="$4"
fragment_prefix="$5"
fragment_body="$6"

deref() { echo "${!1}"; }

function write_glsl_cpp() {
  in="$1"
  out="$2"
  echo "$(cat "$in")" >> "$out"
}

echo "namespace enigma {" > "$output_location"

for var in shader_version vertex_prefix vertex_body fragment_prefix fragment_body; do
  echo "const char* ${var}=R\"(" >> "$output_location"
  write_glsl_cpp $(deref $var) "$output_location"
  echo ")\";" >> "$output_location"
done

echo "} //namespace enigma" >> "$output_location"
