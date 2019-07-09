#!/bin/bash

LINUX_JOBS[0]='TEST_HARNESS=true AUDIO=OpenAL' 
LINUX_JOBS[1]='PLATFORM=None GRAPHICS=None AUDIO=None COLLISION=None NETWORK=None WIDGETS=None EXTENSIONS="None"'
LINUX_JOBS[2]='PLATFORM=SDL GRAPHICS=OpenGL1'
LINUX_JOBS[3]='PLATFORM=SDL GRAPHICS=OpenGL3'
LINUX_JOBS[4]='GRAPHICS=OpenGL1 EXTENSIONS="ParticleSystems"'
LINUX_JOBS[5]='GRAPHICS=OpenGL3 EXTENSIONS="ParticleSystems"'
LINUX_JOBS[6]='GRAPHICS=OpenGLES2 PLATFORM=SDL EXTENSIONS="ParticleSystems"'
LINUX_JOBS[7]='AUDIO=OpenAL EXTENSIONS="GME"'
LINUX_JOBS[8]='AUDIO=SFML EXTENSIONS="GME"'
LINUX_JOBS[9]='COLLISION=BBox NETWORK=None EXTENSIONS="Paths,MotionPlanning"'
LINUX_JOBS[10]='COLLISION=Precise NETWORK=None EXTENSIONS="Paths,MotionPlanning"'
LINUX_JOBS[11]='NETWORK=Asynchronous'
LINUX_JOBS[12]='NETWORK=BerkeleySockets'
LINUX_JOBS[13]='WIDGETS=GTK+'
LINUX_JOBS[14]='WIDGETS=Zenity'
LINUX_JOBS[15]='WIDGETS=KDialog'
LINUX_JOBS[16]='COLLISION=BBox EXTENSIONS="Alarms,Timelines,DataStructures,Asynchronous,BasicGUI,DateTime,GM5Compat,IniFilesystem,Json,XRandR,Paths,MotionPlanning,ttf,Box2DPhysics,StudioPhysics,BulletDynamics"'

JOB_COUNT=16
TRAVIS_WORKERS=4

SPLIT=$(($JOB_COUNT / $TRAVIS_WORKERS))
START=$(( $WORKER * $SPLIT ))
if [[ $WORKER -lt $((TRAVIS_WORKERS-1)) ]]; then
  END=$(($WORKER * $SPLIT + $SPLIT))
else
  END=$JOB_COUNT
fi 

for job in $(seq $START 1 $END);
do
  echo -e "====================================================\n\
Running ${LINUX_JOBS[$job]} ./ci-build.sh\n\
===================================================="
  bash -c "${LINUX_JOBS[$job]} ./build_and_run_game.sh"
done;
