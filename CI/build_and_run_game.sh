#!/bin/bash

set -e

if [ "$TEST_HARNESS" == true ]; then
  export ASAN_OPTIONS=detect_leaks=0;
  xvfb-run -s "-screen 0 1024x768x24" ./ci-regression.sh "/tmp/enigma-master" 4
else
  for mode in "Debug" "Run"; 
  do
    MODE="$mode" ./ci-build.sh
    if [ "$COMPILER" == "MinGW64" ]; then
      WINEPREFIX=~/.wine64 WINEARCH=win64 xvfb-run wine64 $OUTPUT > >(tee -a tee logs/enigma_game.log) 2> >(tee -a tee logs/enigma_game.log >&2)
    elif [ "$COMPILER" == "MinGW32" ]; then
      WINEPREFIX=~/.wine32 WINEARCH=win32 xvfb-run wine $OUTPUT > >(tee -a tee logs/enigma_game.log) 2> >(tee -a tee logs/enigma_game.log >&2)
    #FIXME: these should run but are bugged
    elif [[ ! "$GRAPHICS" =~ "OpenGLES" ]] && [ "$PLATFORM" != "SDL" ] && [[ ! "$COMPILER" =~ "clang" ]]; then
      xvfb-run $OUTPUT > >(tee -a tee logs/enigma_game.log) 2> >(tee -a tee logs/enigma_game.log >&2)
    fi
    ./share_logs.sh
  done
fi
