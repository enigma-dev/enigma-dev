#!/bin/sh
cd "${0%/*}";
if [ $(uname) = "Darwin" ]; then
  sudo "java" -Djna.nosys=true -jar "lateralgm.jar" "$1";
else
  "java" -Djna.nosys=true -jar "lateralgm.jar" "$1";
fi
