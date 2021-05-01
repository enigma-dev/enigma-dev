#!/bin/bash

set -e

if [ "$TRAVIS_OS_NAME" != "osx" ]; then
  sudo apt-get update --option Acquire::Retries=100 --option Acquire::http::Timeout="60";

  if [ "$COMPILER" != "Android" ]; then
    echo -e "Package: *\nPin: release a=*-proposed\nPin-Priority: 400" | sudo tee /etc/apt/preferences.d/proposed >/dev/null; sudo add-apt-repository -y -u 'deb http://archive.ubuntu.com/ubuntu focal-proposed main universe multiverse'
    sudo apt-get -t focal-proposed -y install clang lldb lld libc++abi-dev libc++-dev
    clang++ --version
  fi
fi

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
  sudo apt-get -y install libboost-program-options-dev pulseaudio libyaml-cpp-dev libpugixml-dev rapidjson-dev libprotobuf-dev protobuf-compiler protobuf-compiler-grpc libgrpc-dev libgrpc++-dev zlib1g-dev libglm-dev libpng-dev
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  brew upgrade gcc || brew install gcc || brew link --overwrite gcc;
  brew install protobuf pugixml yaml-cpp rapidjson
fi

g++ --version
