#!/bin/sh
cd "${0%/*}"
"java" -Djna.nosys=true -jar "lateralgm.jar" "$1"
