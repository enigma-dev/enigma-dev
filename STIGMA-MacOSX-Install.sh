#!/bin/sh
sudo xcode-select --install
if [ -d "/Library/Java/JavaVirtualMachines/temurin-17.jdk" ]; then
  if [ -f "/opt/local/bin/port" ]; then
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    env PATH=$PATH:/opt/homebrew/bin brew install gcc curl wget cmake pkg-config pugixml wavpack flac opusfile mpg123 libmodplug libsndfile libogg libsndfile libvorbis vorbis-tools sdl2 openal-soft alure fluid-synth libxmp dumb boost
    sudo port -N install gmake protobuf-c glfw glm libpng libvpx box2D freetype libffi rapidjson libyaml pugixml yaml-cpp grpc glew +x11 libGLU xorg-libX11 xorg-libXrandr xorg-libXinerama
    sudo port -N deactivate glew +x11
    sudo port -N install glew
    sudo port -N reclaim
    git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /Applications/stigma-dev
    cd /Applications/stigma-dev
    env PATH=$PATH:/opt/homebrew/bin CC=clang CXX=clang++ gmake && env PATH=$PATH:/opt/homebrew/bin CC=clang CXX=clang++ gmake emake
    chmod +x ./install.sh
    ./install.sh
    chmod +x ./start.sh
    echo "Installation Complete! You may now run the \"/Applications/stigma-dev/start.sh\" script from the terminal app!"
  else
    echo "Macports installation not found! Please download and install Macports first from www.macports.org"
  fi
else
  if [ `uname -m` = "arm64" ]; then
    echo "OpenJDK installation not found! Please download and install OpenJDK first from https://adoptium.net/temurin/releases/?version=17&os=mac&arch=aarch64&package=jdk"
  else
    echo "OpenJDK installation not found! Please download and install OpenJDK first from https://adoptium.net/temurin/releases/?version=17&os=mac&arch=x64&package=jdk"
  fi
fi

