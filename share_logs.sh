#!/bin/bash
for log in "logs/enigma_libegm.log" "logs/enigma_compiler.log" "logs/enigma_compile.log";
do
  echo -n "$log: "
  cat $log | sed 's/\x1b\[[0-9;]*m//g' | curl -F 'sprunge=<-' http://sprunge.us
  if [[ "$TRAVIS" == "true" ]]; then 
    rm -f "$log"
  fi
done
