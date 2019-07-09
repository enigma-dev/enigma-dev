#!/bin/bash

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
  sudo apt-get -y install build-essential zlib1g-dev libboost-dev\
    libboost-filesystem-dev libboost-system-dev libboost-program-options-dev\
    libboost-iostreams-dev libprotobuf-dev protobuf-compiler libglm-dev libpng-dev\
    pulseaudio libpugixml-dev libyaml-cpp-dev rapidjson-dev || exit 1
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  brew upgrade gcc || brew install gcc || brew link --overwrite gcc;
  brew install protobuf pugixml || exit 1
fi
