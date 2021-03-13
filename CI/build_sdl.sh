#!/bin/bash

pushd /tmp
curl -O "https://www.libsdl.org/release/SDL2-2.0.12.tar.gz"
tar -xf SDL2-2.0.12.tar.gz
pushd SDL2-2.0.12/
./configure --prefix=/usr
make
sudo make install
popd
popd
