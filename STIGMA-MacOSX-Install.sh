#!/bin/sh
sudo xcode-select --install
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
brew install openjdk@8 make cmake pkg-config grpc protobuf-c glew glm libpng libsndfile libogg libvorbis vorbis-tools box2d dumb SDL2 SDL2_mixer freetype libffi rapidjson libyaml libvpx pugixml yaml-cpp boost flac mpg123 libmodplug opus opusfile fluidsynth
sudo git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /Applications/stigma-dev
cd /Applications/stigma-dev
sudo gmake && sudo gmake emake
sudo chmod +x ./install.sh
sudo ./install.sh
sudo chmod +x ./start.sh
echo "Installation Complete! You may now run the \"/Applications/stigma-dev/start.sh\" script from the terminal app!"
