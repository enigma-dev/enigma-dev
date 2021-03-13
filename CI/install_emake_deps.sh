#!/bin/bash

set -e

install_yaml_cpp () {
  git clone https://github.com/jbeder/yaml-cpp /tmp/yaml-cpp
  mkdir /tmp/yaml-cpp/build
  pushd /tmp/yaml-cpp/build
  cmake -DYAML_CPP_BUILD_TESTS=OFF -DYAML_CPP_BUILD_TOLLS=OFF -DYAML_CPP_BUILD_SHARED_LIBS=ON -DCMAKE_CXX_FLAGS="-fPIC" ..
  make
  sudo make install
  popd
}

if [ "$TRAVIS_OS_NAME" != "osx" ]; then
  # new boost & yaml-cpp for old travis
  if [ "$COMPILER" == "Android" ]; then
    # new protobuf
    sudo add-apt-repository -y ppa:maarten-fonville/protobuf;

    # new gcc 
    sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
    sudo add-apt-repository -y ppa:mhier/libboost-latest;
    install_yaml_cpp
  else
    # new SDL
    sudo add-apt-repository -y ppa:savoury1/multimedia
  fi

  sudo apt-get update --option Acquire::Retries=100 --option Acquire::http::Timeout="60";

  if [ "$COMPILER" != "Android" ]; then
    sudo apt-get -y install clang-10 lldb-10 lld-10 libc++abi-10-dev libc++-10-dev
    clang++ --version
  fi
fi

if [ "$COMPILER" == "Android" ]; then
  sudo apt-get -y install libboost1.67-dev gcc-9 g++-9 cpp-9 build-essential libprotobuf-dev protobuf-compiler
    sudo update-alternatives --remove-all cpp;
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 \
              15 \
              --slave   /usr/bin/g++ g++ /usr/bin/g++-9 \
              --slave   /usr/bin/gcov gcov /usr/bin/gcov-9 \
              --slave   /usr/bin/gcov-dump gcov-dump /usr/bin/gcov-dump-9 \
              --slave   /usr/bin/gcov-tool gcov-tool /usr/bin/gcov-tool-9 \
              --slave   /usr/bin/gcc-ar gcc-ar /usr/bin/gcc-ar-9 \
              --slave   /usr/bin/gcc-nm gcc-nm /usr/bin/gcc-nm-9 \
              --slave   /usr/bin/gcc-ranlib gcc-ranlib /usr/bin/gcc-ranlib-9;
elif [ "$TRAVIS_OS_NAME" == "linux" ]; then
  sudo apt-get -y install libboost-program-options-dev pulseaudio libyaml-cpp-dev libpugixml-dev rapidjson-dev libprotobuf-dev protobuf-compiler zlib1g-dev libglm-dev libpng-dev
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  brew upgrade gcc || brew install gcc || brew link --overwrite gcc;
  brew install protobuf pugixml yaml-cpp rapidjson
fi

g++ --version
