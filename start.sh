#!/bin/sh
cd "${0%/*}";
if [ $(uname) = "Darwin" ]; then
  env PATH=/usr/local/bin:/opt/homebrew/bin:$PATH "java" -Djna.nosys=true -jar "lateralgm.jar" "$1";
else
  "java" -Djna.nosys=true -jar "lateralgm.jar" "$1";
fi
