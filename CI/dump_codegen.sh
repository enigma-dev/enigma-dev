#!/bin/bash

GAMES=$(find CommandLine/testing/SimpleTests/* CommandLine/testing/Tests/* -maxdepth 0 -not -name "*.cpp")

for game in ${GAMES}
do
 game_name=$(basename ${game%%.*})
 #echo $game_name
 ./emake -g "OpenGL1" \
         -a "OpenAL" \
         -c "Precise" \
         -e 'Alarms,Timelines,Paths,MotionPlanning,IniFilesystem,ParticleSystems,DateTime,DataStructures,libpng,GTest' \
         --codegen-only \
         --codegen "/tmp/codegen/${game_name}/" \
         -o /tmp/test.exe
done
