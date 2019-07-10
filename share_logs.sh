#!/bin/bash
for log in "/tmp/enigma_libegm.log" "/tmp/enigma_compiler.log" "/tmp/enigma_compile.log";
do
  echo -n "$log: "
  cat $log | curl -F 'sprunge=<-' http://sprunge.us
  if [[ "$TRAVIS" == "true" ]] rm -f "$log"
done
