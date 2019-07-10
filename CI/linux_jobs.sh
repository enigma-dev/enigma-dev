#!/bin/bash

set -e

LINUX_JOBS[0]='PLATFORM=None GRAPHICS=None AUDIO=None COLLISION=None NETWORK=None WIDGETS=None EXTENSIONS="None"'
LINUX_JOBS[1]='PLATFORM=SDL GRAPHICS=OpenGL1'
LINUX_JOBS[2]='PLATFORM=SDL GRAPHICS=OpenGL3'
LINUX_JOBS[3]='GRAPHICS=OpenGL1 EXTENSIONS="ParticleSystems"'
LINUX_JOBS[4]='GRAPHICS=OpenGL3 EXTENSIONS="ParticleSystems"'
LINUX_JOBS[5]='GRAPHICS=OpenGLES2 PLATFORM=SDL EXTENSIONS="ParticleSystems"'
LINUX_JOBS[6]='AUDIO=OpenAL EXTENSIONS="GME"'
LINUX_JOBS[7]='AUDIO=SFML EXTENSIONS="GME"'
LINUX_JOBS[8]='COLLISION=BBox NETWORK=None EXTENSIONS="Paths,MotionPlanning"'
LINUX_JOBS[9]='COLLISION=Precise NETWORK=None EXTENSIONS="Paths,MotionPlanning"'
LINUX_JOBS[10]='NETWORK=Asynchronous'
LINUX_JOBS[11]='NETWORK=BerkeleySockets'
LINUX_JOBS[12]='WIDGETS=GTK+'
LINUX_JOBS[13]='WIDGETS=Zenity'
LINUX_JOBS[14]='WIDGETS=KDialog'
LINUX_JOBS[15]='COLLISION=BBox EXTENSIONS="Alarms,Timelines,DataStructures,Asynchronous,BasicGUI,DateTime,GM5Compat,IniFilesystem,Json,XRandR,Paths,MotionPlanning,ttf,Box2DPhysics,StudioPhysics,BulletDynamics"'

JOB_COUNT=15
TRAVIS_WORKERS=4

SPLIT=$(($JOB_COUNT / $TRAVIS_WORKERS))
START=$(( $WORKER * $SPLIT ))
if [[ $WORKER -lt $((TRAVIS_WORKERS-1)) ]]; then
  END=$(($WORKER * $SPLIT + $SPLIT))
else
  END=$JOB_COUNT
fi

echo "glxinfo"
glxinfo | grep "OpenGL version"

for job in $(seq $START 1 $END);
do
  echo -e "\n\n\e[32m====================================================\n\
\e[32mRunning ${LINUX_JOBS[$job]}\n\
\e[32m====================================================\n\n"
  bash -c "${LINUX_JOBS[$job]} ./CI/build_and_run_game.sh"
done;
