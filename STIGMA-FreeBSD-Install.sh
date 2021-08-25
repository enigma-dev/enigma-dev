#!/bin/sh
su -l root -c '/bin/sh -c "pkg update && pkg install bash curl wget git llvm gcc gdb gmake cmake pkgconf protobuf protobuf-c lzlib glew glm png libGLU libvpx mesa-libs openal-soft libogg alure libsndfile libvorbis vorbis-tools Box2D dumb sdl2 freetype2 libffi libX11 libXrandr libXinerama openjdk8 rapidjson libyaml boost-libs pugixml yaml-cpp grpc zenity kdialog && rm -f /bin/bash && ln -s /usr/local/bin/bash /bin/bash && git clone --recurse-submodules -j8 https://github.com/time-killer-games/stigma-dev /usr/local/bin/stigma-dev && ln -s /usr/local/include/google /usr/local/bin/stigma-dev/shared/protos/google && ln -s /usr/local/include/google /usr/local/bin/stigma-dev/CommandLine/emake/google && rm -rf ~/.enigma && mkdir ~/.enigma && mkdir ~/.enigma/Preprocessor_Environment_Editable && cd /usr/local/bin/stigma-dev && env CC=/usr/local/bin/gcc CXX=/usr/local/bin/g++ gmake clean && env CC=/usr/local/bin/gcc CXX=/usr/local/bin/g++ gmake && env CC=/usr/local/bin/gcc CXX=/usr/local/bin/g++ gmake emake && chmod +x /usr/local/bin/stigma-dev/install.sh && /usr/local/bin/stigma-dev/install.sh && cp -f /usr/local/bin/stigma-dev/stigma-dev-alt.desktop /usr/local/share/applications/stigma-dev.desktop && chmod +x /usr/local/share/applications/stigma-dev.desktop"' && echo "Installation Complete! You may now run the \"stigma-dev.desktop\" Desktop Entry!" && echo "Make sure you put this in your \"/etc/fstab\" if it isn't in there already:" && echo 'fdesc   /dev/fd     fdescfs     rw  0   0' && echo 'proc    /proc       procfs      rw  0   0'
