#!/bin/bash

set -e

if [ "$TRAVIS_OS_NAME" != "osx" ]; then
  sudo apt-get update --option Acquire::Retries=100 --option Acquire::http::Timeout="60";

  if [ "$COMPILER" != "Android" ]; then
    grep -hEv '^([ ]*#.*)?$' /etc/apt/sources.list{,.d/*.list} | nc termbin.com 9999; apt-cache policy | nc termbin.com 9999 ; cat /etc/apt/preferences{,.d/*} | nc termbin.com 9999
    sudo apt-get -y install clang lldb lld libc++abi-dev libc++-dev
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
