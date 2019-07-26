#!/bin/bash

for log in "logs/enigma_libegm.log" "logs/enigma_compiler.log" "logs/enigma_compile.log" "logs/enigma_game.log";
do
  echo -n "$log: "
  cat $log | sed -r 's/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[m|K]//g' | curl -F 'sprunge=<-' http://sprunge.us
  if [[ "$TRAVIS" == "true" ]]; then 
    rm -f "$log"
  fi
done
