#!/bin/sh
# Requires MacPorts to be installed!
sudo xcode-select --install
sudo port -N install openjdk8 +universal gmake +universal cmake +universal pkgconfig +universal grpc +universal protobuf-c +universal glew +universal glm +universal libpng +universal libsndfile +universal libogg +universal libvorbis +universal vorbis-tools +universal box2D +universal libdumb +universal libsdl2 +universal libsdl2_mixer +universal freetype +universal libffi +universal rapidjson +universal libyaml +universal libvpx +universal pugixml +universal yaml-cpp +universal boost +universal flac +universal mpg123 +universal libmodplug +universal libopus +universal opusfile +universal
sudo git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /Applications/stigma-dev
cd /Applications/stigma-dev
sudo port -f activate libiconv
sudo gmake && sudo gmake emake
sudo chmod +x ./install.sh
sudo ./install.sh
sudo chmod +x ./start.sh
sudo port -f deactivate libiconv
echo "Installation Complete! You may now run the \"/Applications/stigma-dev/start.sh\" script from the terminal app!"
