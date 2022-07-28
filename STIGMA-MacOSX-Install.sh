#!/bin/sh
sudo xcode-select --install
if [ -f "/opt/local/bin/port" ]; then
  /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
  brew install curl wget gdb make cmake pkgconf flac opusfile mpg123 libmodplug libsndfile libogg libsndfile libvorbis vorbis-tools sdl2 sdl2_mixer dumb boost
  sudo port -N install protobuf-c glew glm libpng libvpx box2D freetype libffi rapidjson libyaml pugixml yaml-cpp grpc
  sudo port -N uninstall libiconv
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
