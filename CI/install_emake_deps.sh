#!/bin/bash

set -e

if [ "$TRAVIS_OS_NAME" != "osx" ]; then
  # new protobuf
  sudo add-apt-repository -y ppa:maarten-fonville/protobuf;

  # new gcc 
  sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
  
  # new clang
  wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
  sudo apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-10 main"

  # new boost for old travis
  if [ "$COMPILER" == "Android" ]; then
    sudo add-apt-repository -y ppa:mhier/libboost-latest;
  else
    # new lcov
    sudo add-apt-repository -y ppa:cheeseboy16/travis-backports
  fi

  sudo apt-get update --option Acquire::Retries=100 --option Acquire::http::Timeout="60";
  sudo apt-get -y install gcc-9 g++-9 cpp-9 build-essential libprotobuf-dev protobuf-compiler zlib1g-dev libglm-dev libpng-dev \
    clang-10 lldb-10 lld-10

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

  g++ --version
  
  sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-10 \
              81 \
              --slave /usr/bin/clang++ clang++ /usr/bin/clang++-10 \
              --slave /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-10 \
              --slave /usr/bin/clang-format clang-format /usr/bin/clang-format-10;
  
  sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-6.0 \
              60 \
              --slave /usr/bin/clang++ clang++ /usr/bin/clang++-7.0 \
              --slave /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-7.0 \
              --slave /usr/bin/clang-format clang-format /usr/bin/clang-format-7.0;

  clang++ --version
fi

if [ "$COMPILER" == "Android" ]; then
  sudo apt-get -y install libboost1.67-dev
elif [ "$TRAVIS_OS_NAME" == "linux" ]; then
  sudo apt-get -y install libboost-program-options-dev pulseaudio libpugixml-dev libyaml-cpp-dev rapidjson-dev
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  brew upgrade gcc || brew install gcc || brew link --overwrite gcc;
  brew install protobuf pugixml yaml-cpp rapidjson
fi
