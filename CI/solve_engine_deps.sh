#!/bin/bash

###### Harness #######
if [ "$TEST_HARNESS" == true ]; then
  LINUX_DEPS="$LINUX_DEPS xfwm4 libgtest-dev wmctrl xdotool lcov"
fi

###### Compilers #######
if [ "$COMPILER" == "gcc32" ] || [ "$COMPILER" == "clang32" ]; then
  LINUX_DEPS="$LINUX_DEPS libc6:i386 libc++-dev:i386 libstdc++6:i386\
    libncurses5:i386 libx11-6:i386 libglew-dev:i386 libglu1-mesa-dev:i386\
    libgl1-mesa-dev:i386 lib32z1-dev libxrandr-dev:i386 libxinerama-dev:i386\
    gcc-multilib g++-multilib libc++abi-dev:i386 libpng-dev:i386"
elif [ "$COMPILER" == "MinGW64" ] || [ "$COMPILER" == "MinGW32" ]; then
  LINUX_DEPS="$LINUX_DEPS mingw-w64 wine64 wine32 wine-stable libgl1-mesa-glx:i386"
fi

###### Platforms #######
if [ "$PLATFORM" == "SDL" ] || [ "$TEST_HARNESS" == true ]; then
  #LINUX_DEPS="$LINUX_DEPS libsdl2-dev"
  pushd /tmp
  curl -O https://www.libsdl.org/release/SDL2-2.0.12.tar.gz
  tar -xf SDL2-2.0.12.tar.gz
  pushd SDL2-2.0.12/
  ./configure --prefix=/usr
  make
  sudo make install
  popd
  popd
fi

###### Graphics #######
LINUX_DEPS="$LINUX_DEPS libepoxy-dev libegl1-mesa-dev libgles2-mesa-dev libglew-dev libxrandr-dev libxinerama-dev"

###### Audio #######
if [ "$AUDIO" == "OpenAL" ] || [ "$TEST_HARNESS" == true ]; then
  LINUX_DEPS="$LINUX_DEPS libalure-dev libvorbisfile3 libvorbis-dev libdumb1-dev"
fi
if [ "$AUDIO" == "SFML" ] || [ "$TEST_HARNESS" == true ]; then
  LINUX_DEPS="$LINUX_DEPS libsfml-dev"
fi

###### Widgets #######
if [ "$WIDGETS" == "GTK+" ] || [ "$TEST_HARNESS" == true ]; then
  LINUX_DEPS="$LINUX_DEPS libgtk2.0-dev"
fi
if [ "$WIDGETS" == "xlib" ] || [ "$TEST_HARNESS" == true ]; then
  LINUX_DEPS="$LINUX_DEPS zenity kdialog"
fi

###### Extensions #######
if [[ "$EXTENSIONS" =~ "GME" ]] || [ "$TEST_HARNESS" == true ]; then
  LINUX_DEPS="$LINUX_DEPS libgme-dev"
fi

if [[ "$EXTENSIONS" =~ "Box2DPhysics" ]] || [[ "$EXTENSIONS" =~ "StudioPhysics" ]] || [ "$TEST_HARNESS" == true ]; then
  LINUX_DEPS="$LINUX_DEPS libbox2d-dev"
fi

if [[ "$EXTENSIONS" =~ "BulletDynamics" ]] || [ "$TEST_HARNESS" == true ]; then
  LINUX_DEPS="$LINUX_DEPS libbullet-dev"
fi

if [[ "$EXTENSIONS" =~ "ttf" ]] || [ "$TEST_HARNESS" == true ]; then
  LINUX_DEPS="$LINUX_DEPS libfreetype6-dev"
fi

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
  echo "$LINUX_DEPS"
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  echo "$OSX_DEPS"
fi
