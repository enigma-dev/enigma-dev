#!/bin/bash

set -e

sudo add-apt-repository -y ppa:maarten-fonville/protobuf;

install_yaml_cpp () {
  git clone https://github.com/jbeder/yaml-cpp /tmp/yaml-cpp
  mkdir /tmp/yaml-cpp/build
  pushd /tmp/yaml-cpp/build
  cmake -DYAML_CPP_BUILD_TESTS=OFF -DYAML_CPP_BUILD_TOLLS=OFF -DYAML_CPP_BUILD_SHARED_LIBS=ON -DCMAKE_CXX_FLAGS="-fPIC" ..
  make
  sudo make install
  popd
}

if [ "$COMPILER" == "Android" ]; then
  sudo add-apt-repository -y ppa:mhier/libboost-latest;
  sudo apt-get update --option Acquire::Retries=100 --option Acquire::http::Timeout="60";
  sudo apt-get -y install build-essential zlib1g-dev libboost1.67-dev\
        libprotobuf-dev protobuf-compiler libglm-dev libpng-dev;
  install_yaml_cpp
elif [ "$TRAVIS_OS_NAME" == "linux" ]; then
  sudo apt-get update --option Acquire::Retries=100 --option Acquire::http::Timeout="60";
  sudo apt-get -y install build-essential zlib1g-dev libboost-dev\
    libboost-filesystem-dev libboost-system-dev libboost-program-options-dev\
    libboost-iostreams-dev libprotobuf-dev protobuf-compiler libglm-dev libpng-dev\
    pulseaudio libpugixml-dev libyaml-cpp-dev rapidjson-dev
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  brew upgrade gcc || brew install gcc || brew link --overwrite gcc;
  brew install protobuf pugixml
fi
