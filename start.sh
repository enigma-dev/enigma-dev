#!/bin/sh
cd "${0%/*}"
sudo "java" -Djna.nosys=true -jar "lateralgm.jar" "$1"
