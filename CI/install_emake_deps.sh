#!/bin/bash

set -e

if [ "$TRAVIS_OS_NAME" != "osx" ] && [ "$TRAVIS_OS_NAME" != "windows" ]; then
  sudo apt-get update --option Acquire::Retries=100 --option Acquire::http::Timeout="60";

  if [ "$COMPILER" != "Android" ]; then
    sudo apt-get -y install clang-10 lldb-10 lld-10 libc++abi-10-dev libc++-10-dev
    clang++ --version
  fi
fi

if [ "$TRAVIS_OS_NAME" == "linux" ]; then
  sudo apt-get -y install libboost-program-options-dev pulseaudio libyaml-cpp-dev libpugixml-dev rapidjson-dev libprotobuf-dev protobuf-compiler protobuf-compiler-grpc libgrpc-dev libgrpc++-dev zlib1g-dev libglm-dev libpng-dev
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  brew upgrade gcc || brew install gcc || brew link --overwrite gcc;
  brew install protobuf pugixml yaml-cpp rapidjson
elif [ "$TRAVIS_OS_NAME" == "windows" ]; then
  pacman -Sy --noconfirm --needed mingw-w64-x86_64-boost
fi

#g++ --version
