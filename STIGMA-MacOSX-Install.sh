#!/bin/sh
# Requires MacPorts to be installed. (Don't use Homebrew!)
/usr/bin/sudo xcode-select --install 
/usr/bin/sudo port install boost
/usr/bin/sudo port install openjdk17 +universal gmake +universal cmake +universal pkgconfig +universal grpc +universal protobuf-c +universal glew +universal libpng +universal libGLU +universal openal-soft +universal libogg +universal libvorbis +universal vorbis-tools +universal box2D +universal libdumb +universal libsdl2 +universal freetype +universal libffi +universal rapidjson +universal libyaml +universal pugixml +universal yaml-cpp +universal
/usr/bin/sudo mkdir -p /usr/local/bin/STIGMA/STIGMA
/usr/bin/sudo git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /usr/local/bin/STIGMA/STIGMA/stigma-dev
cd /usr/local/bin/STIGMA/STIGMA/stigma-dev
/usr/bin/sudo gmake && /usr/bin/sudo gmake emake
/usr/bin/sudo cp /usr/local/bin/STIGMA/STIGMA/stigma-dev/*.dylib /usr/local/bin/STIGMA/STIGMA/
/usr/bin/sudo cp /usr/local/bin/STIGMA/STIGMA/stigma-dev/*.dylib /usr/local/bin/STIGMA/
/usr/bin/sudo chmod +x ./install.sh
/usr/bin/sudo ./install.sh
/usr/bin/sudo chmod +x ./start.sh
ln -s /usr/local/bin/STIGMA/STIGMA/stigma-dev/start.sh $HOME/STIGMA-launch.sh
chmod +x $HOME/STIGMA-launch.sh
