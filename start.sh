#!/bin/sh
cd "${0%/*}";
if [ $(uname) = "Darwin" ]; then
  sudo env PATH=$PATH:/opt/homebrew/bin "java" -Djna.nosys=true -jar "lateralgm.jar" "$1";
else
  "java" -Djna.nosys=true -jar "lateralgm.jar" "$1";
fi
