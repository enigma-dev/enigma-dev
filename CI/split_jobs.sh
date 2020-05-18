#!/bin/bash

set -e

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
  JOBS[0]='PLATFORM=None GRAPHICS=None AUDIO=None COLLISION=None NETWORK=None WIDGETS=None EXTENSIONS="None"'
  JOBS[1]='PLATFORM=SDL GRAPHICS=OpenGL1'
  JOBS[2]='PLATFORM=SDL GRAPHICS=OpenGL3'
  JOBS[3]='GRAPHICS=OpenGL1 EXTENSIONS="ParticleSystems"'
  JOBS[4]='GRAPHICS=OpenGL3 EXTENSIONS="ParticleSystems"'
  JOBS[5]='GRAPHICS=OpenGLES2 PLATFORM=SDL EXTENSIONS="ParticleSystems"'
  JOBS[6]='AUDIO=OpenAL EXTENSIONS="GME"'
  JOBS[7]='COLLISION=BBox NETWORK=None EXTENSIONS="Paths,MotionPlanning"'
  JOBS[8]='COLLISION=Precise NETWORK=None EXTENSIONS="Paths,MotionPlanning"'
  JOBS[9]='NETWORK=Asynchronous'
  JOBS[10]='NETWORK=BerkeleySockets'
  JOBS[11]='WIDGETS=GTK+'
  JOBS[12]='WIDGETS=xlib'
  JOBS[14]='COLLISION=BBox EXTENSIONS="Alarms,Timelines,DataStructures,Asynchronous,BasicGUI,DateTime,GM5Compat,IniFilesystem,Json,XRandR,Paths,MotionPlanning,ttf,Box2DPhysics,StudioPhysics,BulletDynamics,ExternalFuncs,ProcInfo"'
  JOBS[15]='COMPILER=clang'
  #JOBS[16]='COMPILER=gcc32'
  #JOBS[17]='COMPILER=clang32'
  JOB_COUNT=16
  TRAVIS_WORKERS=5
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  JOBS[0]='COMPILER=gcc PLATFORM=None'
  JOBS[1]='COMPILER=clang PLATFORM=None'
  JOB_COUNT=2
  TRAVIS_WORKERS=1
else
  echo "Error: Unsupported OS"
  exit 1
fi

if [ "$WORKER" != "" ]; then 

  SPLIT=$(($JOB_COUNT / $TRAVIS_WORKERS))
  START=$(( $WORKER * $SPLIT ))
  if [[ $WORKER -lt $((TRAVIS_WORKERS-1)) ]]; then
    END=$(($WORKER * $SPLIT + $SPLIT))
  else
    END=$JOB_COUNT
  fi

fi

if [ "$1" == "install" ]; then

  ###### Figure out our deps ######
  LINUX_DEPS="xvfb"
  OSX_DEPS="glew"
  MINGW_DEPS="FALSE"
  for job in $(seq $START 1 $END);
  do
    if [ "$TRAVIS_OS_NAME" == "linux" ]; then
      LINUX_DEPS=$(bash -c "${JOBS[$job]} LINUX_DEPS=\"$LINUX_DEPS\" ./CI/solve_engine_deps.sh")
      if [[ "${JOBS[$job]}" =~ "MinGW" ]]; then
        MINGW_DEPS="TRUE"
      fi
    elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
      OSX_DEPS=$(bash -c "${JOBS[$job]} OSX_DEPS=\"$OSX_DEPS\" ./CI/solve_engine_deps.sh")
    fi
  done

  ###### Install Deps #######
  if [ "$TRAVIS_OS_NAME" == "linux" ]; then
    sudo apt-get -y install $LINUX_DEPS
    
    if [ "$MINGW_DEPS" == "TRUE" ]; then
      sudo update-alternatives --set i686-w64-mingw32-g++ /usr/bin/i686-w64-mingw32-g++-posix
      sudo update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix
      curl -L https://github.com/enigma-dev/enigma-dev/files/2431000/enigma-libs.zip > enigma-libs.zip;
      unzip enigma-libs.zip -d ENIGMAsystem/;
      mv ENIGMAsystem/Install ENIGMAsystem/Additional;
    fi
    
  elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
    brew install $OSX_DEPS
  fi
  
elif [ "$1" == "run" ]; then

  ###### Run Jobs #######
  for job in $(seq $START 1 $END);
  do
    echo -e "\n\n\e[32m====================================================\n\
\e[32mRunning ${JOBS[$job]}\n\
\e[32m====================================================\n\n"
    bash -c "${JOBS[$job]} ./CI/build_and_run_game.sh"
  done;

else
  echo "Error: \"run\" or \"install\" argument required"
  exit 1
fi
