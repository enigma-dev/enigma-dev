#!/bin/bash

###### Harness #######
if [ "$TEST_HARNESS" == true ]; then
  LINUX_DEPS="$LINUX_DEPS xfwm4 libgtest-dev wmctrl xdotool lcov"
fi

###### Compilers #######
if [ "$COMPILER" == "gcc32" ] || [ "$COMPILER" == "clang32" ]; then
  LINUX_DEPS="$LINUX_DEPS libc6:i386 libc++-dev:i386 libstdc++6:i386\
    libncurses5:i386 libx11-6:i386 libglew-dev:i386 libglu1-mesa-dev:i386\
    libgl1-mesa-dev:i386 lib32z1-dev libxrandr-dev:i386\
    gcc-multilib g++-multilib libc++abi-dev:i386 libpng-dev:i386"
elif [ "$COMPILER" == "MinGW64" ] || [ "$COMPILER" == "MinGW32" ]; then
  LINUX_DEPS="$LINUX_DEPS mingw-w64 wine64 wine32 wine-stable"
fi

###### Platforms #######
if [ "$PLATFORM" == "SDL" ]; then
  LINUX_DEPS="$LINUX_DEPS libsdl2-dev"
fi

if [ "$GRAPHICS" == "OpenGLES2" ]; then
  LINUX_DEPS="$LINUX_DEPS libepoxy-dev libegl1-mesa-dev libgles2-mesa-dev"
else
  LINUX_DEPS="$LINUX_DEPS libglew-dev libxrandr-dev"
fi

###### Audio #######
if [ "$AUDIO" == "OpenAL" ]; then
  LINUX_DEPS="$LINUX_DEPS libalure-dev libvorbisfile3 libvorbis-dev libdumb1-dev"
elif [ "$AUDIO" == "SFML" ]; then
  LINUX_DEPS="$LINUX_DEPS libsfml-dev"
fi

###### Widgets #######
if [ "$WIDGETS" == "GTK+" ]; then
  LINUX_DEPS="$LINUX_DEPS libgtk2.0-dev"
elif [ "$WIDGETS" == "Zenity" ]; then
  LINUX_DEPS="$LINUX_DEPS zenity"
elif [ "$WIDGETS" == "KDialog" ]; then
  LINUX_DEPS="$LINUX_DEPS kdebase-bin"
fi

###### Extensions #######
if [[ "$EXTENSIONS" =~ "GME" ]]; then
  LINUX_DEPS="$LINUX_DEPS libgme-dev"
fi

if [[ "$EXTENSIONS" =~ "Box2DPhysics" ]] || [[ "$EXTENSIONS" =~ "StudioPhysics" ]]; then
  LINUX_DEPS="$LINUX_DEPS libbox2d-dev"
fi

if [[ "$EXTENSIONS" =~ "BulletDynamics" ]]; then
  LINUX_DEPS="$LINUX_DEPS libbullet-dev"
fi

if [[ "$EXTENSIONS" =~ "ttf" ]]; then
  LINUX_DEPS="$LINUX_DEPS libfreetype6-dev"
fi

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
  echo "$LINUX_DEPS"
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  echo "$OSX_DEPS"
fi
