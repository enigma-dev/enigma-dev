#!/bin/sh
# Requires MacPorts to be installed!
sudo xcode-select --install
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
brew install alure
sudo port -N install openjdk8 gmake cmake pkgconfig grpc protobuf-c glew glm libpng openal-soft libsndfile libogg libvorbis vorbis-tools box2D libdumb libsdl2 freetype libffi rapidjson libyaml libvpx pugixml yaml-cpp boost
sudo mkdir -p /usr/local/bin/STIGMA/STIGMA
sudo git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /usr/local/bin/STIGMA/STIGMA/stigma-dev
cd /usr/local/bin/STIGMA/STIGMA/stigma-dev
sudo gmake && sudo gmake emake
sudo cp /usr/local/bin/STIGMA/STIGMA/stigma-dev/*.dylib /usr/local/bin/STIGMA/STIGMA/
sudo cp /usr/local/bin/STIGMA/STIGMA/stigma-dev/*.dylib /usr/local/bin/STIGMA/
sudo chmod +x ./install.sh
sudo ./install.sh
sudo chmod +x ./start.sh
ln -s /usr/local/bin/STIGMA/STIGMA/stigma-dev /Applications/STIGMA
sudo port -f deactivate libiconv
