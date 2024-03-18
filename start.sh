#!/bin/bash
cd "${0%/*}"
cd "enigma-dev"
"java" -Djna.nosys=true -jar "lateralgm.jar" "$1"