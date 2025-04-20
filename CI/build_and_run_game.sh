#!/bin/bash

set -e

export LIBGL_ALWAYS_SOFTWARE=1

if [ "$TEST_HARNESS" == true ]; then
  export ASAN_OPTIONS=detect_leaks=0;
  ./ci-regression.sh "/tmp/enigma-master" 4
else
  for mode in "Debug" "Run"; 
  do
    MODE="$mode" ./ci-build.sh
    if [ "$COMPILER" == "MinGW64" ]; then
      WINETEST_INTERACTIVE=0 WINEDLLOVERRIDES="mscoree,mshtml=" WINEPREFIX=~/.wine64 WINEARCH=win64 xvfb-run wine64 $OUTPUT > >(tee -a tee /tmp/enigma_game.log) 2> >(tee -a tee /tmp/enigma_game.log >&2)
    elif [ "$COMPILER" == "MinGW32" ]; then
      WINETEST_INTERACTIVE=0 WINEDLLOVERRIDES="mscoree,mshtml=" WINEPREFIX=~/.wine32 WINEARCH=win32 xvfb-run wine $OUTPUT > >(tee -a tee /tmp/enigma_game.log) 2> >(tee -a tee /tmp/enigma_game.log >&2)
    #FIXME: these should run but are bugged
    elif [[ ! "$COMPILER" =~ "clang" ]]; then
      xvfb-run $OUTPUT > >(tee -a tee /tmp/enigma_game.log) 2> >(tee -a tee /tmp/enigma_game.log >&2)
    fi
    ./share_logs.sh
  done
fi
