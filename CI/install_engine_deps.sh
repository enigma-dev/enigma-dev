#!/bin/bash

set -e

LINUX_DEPS="libc++-dev libprotobuf-dev protobuf-compiler libgtest-dev wmctrl xvfb xdotool xfwm4 lcov\
  libsdl2-dev libepoxy-dev libegl1-mesa-dev libgles2-mesa-dev libalure-dev libvorbisfile3 libvorbis-dev libdumb1-dev\
  libsfml-dev libgtk2.0-dev zenity kdebase-bin libgme-dev libglew-dev libxrandr-dev libbox2d-dev libbullet-dev libfreetype6-dev"

OSX_DEPS=""

###### Cross Compilers #######
if [ "$COMPILER" == "gcc32" ] || [ "$COMPILER" == "clang32" ]; then
  LINUX_DEPS="$LINUX_DEPS libc6:i386 libc++-dev:i386 libstdc++6:i386\
    libncurses5:i386 libx11-6:i386 libglew-dev:i386 libglu1-mesa-dev:i386\
    libgl1-mesa-dev:i386 lib32z1-dev libxrandr-dev:i386\
    gcc-multilib g++-multilib libc++abi-dev:i386 libpng-dev:i386"
elif [ "$COMPILER" == "MinGW64" ] || [ "$COMPILER" == "MinGW32" ]; then
  LINUX_DEPS="$LINUX_DEPS mingw-w64 wine-stable"
fi

###### Install Deps #######
if [ "$TRAVIS_OS_NAME" == "linux" ]; then
  sudo apt-get -y install $LINUX_DEPS
  
  if [ "$COMPILER" == "MinGW64" ] || [ "$COMPILER" == "MinGW32" ]; then
    curl -L https://github.com/enigma-dev/enigma-dev/files/2431000/enigma-libs.zip > enigma-libs.zip;
    unzip enigma-libs.zip -d ENIGMAsystem/;
    mv ENIGMAsystem/Install ENIGMAsystem/Additional;
  fi
  
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  brew install "$OSX_DEPS" 
fi
