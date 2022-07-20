#!/bin/sh
sudo xcode-select --install
if [ -f "/opt/local/lib/port" ]; then
  sudo port -N install curl wget gdb gmake cmake pkgconf flac opusfile mpg123 libmodplug libsndfile protobuf-c glew glm libpng libGLU libvpx libogg libsndfile libvorbis vorbis-tools box2D libsdl2 libsdl2_mixer freetype libffi openjdk8 jna rapidjson libyaml boost pugixml yaml-cpp grpc libdumb
  sudo git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /Applications/stigma-dev
  cd /Applications/stigma-dev
  sudo env CC=clang CXX=clang++ gmake && sudo env CC=clang CXX=clang++ gmake emake
  sudo chmod +x ./install.sh
  sudo ./install.sh
  sudo chmod +x ./start.sh
  echo "Installation Complete! You may now run the \"/Applications/stigma-dev/start.sh\" script from the terminal app!"
else
  echo "Macports installation not found! Please download and install Macports first from www.macports.org"
fi
