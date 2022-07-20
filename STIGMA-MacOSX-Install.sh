#!/bin/sh
sudo xcode-select --install
if [ -f "/opt/local/bin/port" ]; then
  sudo port -N install curl wget gdb gmake cmake pkgconf protobuf-c glew glm libpng libGLU libvpx flac opusfile mpg123 libmodplug box2D freetype libffi openjdk8 jna rapidjson libyaml boost pugixml yaml-cpp grpc
  /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
  brew install libsndfile libogg libsndfile libvorbis vorbis-tools sdl2 sdl2_mixer dumb
  sudo git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /Applications/stigma-dev
  cd /Applications/stigma-dev
  sudo env CC=clang CXX=clang++ gmake && sudo env CC=clang CXX=clang++ gmake emake
  sudo chmod +x ./install.sh
  sudo ./install.sh
  sudo chmod +x ./start.sh
  sudo port -f deactivate libiconv
  echo "Installation Complete! You may now run the \"/Applications/stigma-dev/start.sh\" script from the terminal app!"
else
  echo "Macports installation not found! Please download and install Macports first from www.macports.org"
fi
