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
  

  sudo update-alternatives \
        --install /usr/bin/llvm-config       llvm-config      /usr/bin/llvm-config-10 15 \
        --slave   /usr/bin/llvm-ar           llvm-ar          /usr/bin/llvm-ar-10 \
        --slave   /usr/bin/llvm-as           llvm-as          /usr/bin/llvm-as-10 \
        --slave   /usr/bin/llvm-bcanalyzer   llvm-bcanalyzer  /usr/bin/llvm-bcanalyzer-10 \
        --slave   /usr/bin/llvm-cov          llvm-cov         /usr/bin/llvm-cov-10 \
        --slave   /usr/bin/llvm-diff         llvm-diff        /usr/bin/llvm-diff-10 \
        --slave   /usr/bin/llvm-dis          llvm-dis         /usr/bin/llvm-dis-10 \
        --slave   /usr/bin/llvm-dwarfdump    llvm-dwarfdump   /usr/bin/llvm-dwarfdump-10 \
        --slave   /usr/bin/llvm-extract      llvm-extract     /usr/bin/llvm-extract-10 \
        --slave   /usr/bin/llvm-link         llvm-link        /usr/bin/llvm-link-10 \
        --slave   /usr/bin/llvm-mc           llvm-mc          /usr/bin/llvm-mc-10 \
        --slave   /usr/bin/llvm-mcmarkup     llvm-mcmarkup    /usr/bin/llvm-mcmarkup-10 \
        --slave   /usr/bin/llvm-nm           llvm-nm          /usr/bin/llvm-nm-10 \
        --slave   /usr/bin/llvm-objdump      llvm-objdump     /usr/bin/llvm-objdump-10 \
        --slave   /usr/bin/llvm-ranlib       llvm-ranlib      /usr/bin/llvm-ranlib-10 \
        --slave   /usr/bin/llvm-readobj      llvm-readobj     /usr/bin/llvm-readobj-10 \
        --slave   /usr/bin/llvm-rtdyld       llvm-rtdyld      /usr/bin/llvm-rtdyld-10 \
        --slave   /usr/bin/llvm-size         llvm-size        /usr/bin/llvm-size-10 \
        --slave   /usr/bin/llvm-stress       llvm-stress      /usr/bin/llvm-stress-10 \
        --slave   /usr/bin/llvm-symbolizer   llvm-symbolizer  /usr/bin/llvm-symbolizer-10 \
        --slave   /usr/bin/llvm-tblgen       llvm-tblgen      /usr/bin/llvm-tblgen-10

  sudo update-alternatives \
        --install /usr/bin/clang                 clang                 /usr/bin/clang-10 15 \
        --slave   /usr/bin/clang++               clang++               /usr/bin/clang++-10  \
        --slave   /usr/bin/asan_symbolize        asan_symbolize        /usr/bin/asan_symbolize-10 \
        --slave   /usr/bin/c-index-test          c-index-test          /usr/bin/c-index-test-10 \
        --slave   /usr/bin/clang-check           clang-check           /usr/bin/clang-check-10 \
        --slave   /usr/bin/clang-cl              clang-cl              /usr/bin/clang-cl-10 \
        --slave   /usr/bin/clang-cpp             clang-cpp             /usr/bin/clang-cpp-10 \
        --slave   /usr/bin/clang-format          clang-format          /usr/bin/clang-format-10 \
        --slave   /usr/bin/clang-format-diff     clang-format-diff     /usr/bin/clang-format-diff-10 \
        --slave   /usr/bin/clang-import-test     clang-import-test     /usr/bin/clang-import-test-10 \
        --slave   /usr/bin/clang-include-fixer   clang-include-fixer   /usr/bin/clang-include-fixer-10 \
        --slave   /usr/bin/clang-offload-bundler clang-offload-bundler /usr/bin/clang-offload-bundler-10 \
        --slave   /usr/bin/clang-query           clang-query           /usr/bin/clang-query-10 \
        --slave   /usr/bin/clang-rename          clang-rename          /usr/bin/clang-rename-10 \
        --slave   /usr/bin/clang-reorder-fields  clang-reorder-fields  /usr/bin/clang-reorder-fields-10 \
        --slave   /usr/bin/clang-tidy            clang-tidy            /usr/bin/clang-tidy-10 \
        --slave   /usr/bin/lldb                  lldb                  /usr/bin/lldb-10 \
        --slave   /usr/bin/lldb-server           lldb-server           /usr/bin/lldb-server-10

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
